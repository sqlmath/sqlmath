/*jslint beta, browser, devel, nomen*/
/*global jQuery*/
// init debugInline
let debugInline = (function () {
    let __consoleError = function () {
        return;
    };
    function debug(...argv) {

// This function will print <argv> to stderr and then return <argv>[0].

        __consoleError("\n\ndebugInline");
        __consoleError(...argv);
        __consoleError("\n");
        return argv[0];
    }
    debug(); // Coverage-hack.
    __consoleError = console.error;
    return debug;
}());

// The display buffer is what Scroller uses to calculate how many rows it
// should pre-fetch for scrolling. Scroller automatically adjusts DataTables'
// display length to pre-fetch rows that will be shown in "near scrolling"
// (i.e. just beyond the current display area). The value is based upon the
// number of rows that can be displayed in the viewport (i.e. a value of 1),
// and will apply the display range to records before before and after the
// current viewport - i.e. a factor of 3 will allow Scroller to pre-fetch 1
// viewport's worth of rows before the current viewport, the current viewport's
// rows and 1 viewport's worth of rows after the current viewport. Adjusting
// this value can be useful for ensuring smooth scrolling based on your data
// set.
let DATATABLE_ROW_BUFFER_SCALE = 8; //jslint-quiet

// Assume height of each row in datatable is constant for performance-reasons.
let DATATABLE_ROW_HEIGHT = 16;

// Scroller uses the boundary scaling factor to decide when to redraw the
// table - which it typically does before you reach the end of the currently
// loaded data set (in order to allow the data to look continuous to a user
// scrolling through the data). If given as 0 then the table will be redrawn
// whenever the viewport is scrolled, while 1 would not redraw the table until
// the currently loaded data has all been shown. You will want something in
// the middle - the default factor of 0.5 is usually suitable.
let DATATABLE_SCROLLER_BOUNDARY_SCALE = 0.5;

// When using server-side processing, Scroller will wait a small amount of time
// to allow the scrolling to finish before requesting more data from the
// server. This prevents you from DoSing your own server! The wait time can be
// configured by this parameter.
let DATATABLE_SCROLLER_WAIT = 200;

let DataTable;

function _fnAjaxUpdateDraw2(baton, {
    data,
    iRecordsCount
}) {
// Data the data from the server (nuking the old) and redraw the table
//  @param {object} baton dataTables baton object
//  @param {object} json json data return from the server.
//  @param {string} sEcho Tracking flag for DataTables to match requests
//  @param {int} iTotalRecords Number of records in the data set,
//      not accounting for filtering
//  @param {int} iTotalDisplayRecords Number of records in the data set,
//      accounting for filtering
//  @param {array} aaData The data to display on this page
//  @param {string} [sColumns] Column ordering (sName, comma separated)
//  @memberof DataTable#oApi
    // _fnClearTable(baton); - start
    baton.aoData.length = 0;
    baton.aiDisplay.length = 0;
    // _fnClearTable(baton); - end
    baton.iRecordsCount = iRecordsCount;
    data.forEach(function (aDataIn) {
// Add a data array to the table, creating DOM node etc. This is the parallel to
// _fnGatherData, but for adding rows from a Javascript source, rather than a
// DOM source.
        /* Create the object for storing information about this new row */
        let iRow = baton.aoData.length;
        let oData = Object.assign({}, DataTable.models.oRow, {
            idx: iRow,
            src: "data"
        });
        oData._aData = aDataIn;
        baton.aoData.push(oData);
        /* Create the cells */
        // Invalidate the column types as the new data needs to be revalidated
        baton.aoColumns.forEach(function (col) {
            col.sType = null;
        });
        /* Add to the display array */
        baton.aiDisplay.push(iRow);
    });
    baton.bAjaxDataGet = false;
    onDatatableDraw(baton);
    if (!baton._bInitComplete) {
        baton._bInitComplete = true;
        // When data was added after the initialisation (data or Ajax)
        // we need to calculate the column sizing
        // hack-datatables - optimization - disable slow-onresize-handler
        datatableEventEmit(baton, "aoInitComplete", "init", [baton, {
            data,
            iRecordsCount
        }]);
    }
    baton.bAjaxDataGet = true;
    uiRenderDatatableLoading(baton, "none");
}

function assertOrThrow(condition, message) {

// This function will throw <message> if <condition> is falsy.

    if (!condition) {
        throw (
            (!message || typeof message === "string")
            ? new Error(String(message).slice(0, 2048))
            : message
        );
    }
}

function datatableEventEmit(baton, callbackList, eventName, argList) {
// Fire callback functions and trigger events. Note that the loop over the
// callback array store is done backwards! Further note that you do not want to
// fire off triggers in time sensitive applications (for example cell creation)
// as its slow.
//  @param {object} baton dataTables baton object
//  @param {string} callbackList Name of the array storage for the callbacks in
//      baton
//  @param {string} eventName Name of the jQuery custom event to trigger. If
//      null no trigger is fired
//  @param {array} argList Array of arguments to pass to the callback function /
//      trigger
//  @memberof DataTable#oApi
    if (callbackList) {
        baton[callbackList].slice().reverse().forEach(function (val) {
            return val.fn.apply(baton.oInstance, argList);
        });
    }
    jQuery(baton.nTable).trigger(new jQuery.Event(eventName + ".dt"), argList);
}

function domDivCreate(innerHTML) {
// this function will return div-element with rendered <innerHTML>
    let elem = document.createElement("div");
    elem.innerHTML = innerHTML;
    return elem;
}

function onDatatableDraw(baton) {
// Insert the required TR nodes into the table for display
//  @param {object} baton dataTables baton object
//  @memberof DataTable#oApi
    let html = "";
    uiRenderDatatableLoading(baton, "block");
    // Provide a pre-callback function which can be used to cancel the draw
    // is false is returned
    datatableEventEmit(
        baton,
        "aoPreDrawCallback",
        "preDraw",
        [
            baton
        ]
    );
    if (!baton.bDeferLoading && !baton.bDestroying && baton.bAjaxDataGet) {
        // Create an Ajax call based on the table's baton
        baton.ajax(
            baton,
            function (json) {
                _fnAjaxUpdateDraw2(baton, json);
            },
            baton
        );
        return;
    }
    // Server-side processing draw intercept
    if (baton.bDeferLoading) {
        baton.bDeferLoading = false;
    }
    baton.aiDisplay.forEach(function (rowIi) {
        // Create a new TR element (and it's TD children) for a row
        html += `<tr>`;
        // Process each column
        baton.aoColumns.forEach(function (ignore, colIi) {
            let val = baton.aoData[rowIi]._aData[colIi];
            html += `<td>`;
            // Need to create the HTML if new
            html += stringHtmlSafe(
                val === null
                ? ""
                : val
            );
            html += `</td>`;
        });
        html += `</tr>`;
    });
    baton.nTBody.innerHTML = html;
    Array.from(baton.nTBody.children).forEach(function (elemTr, ii) {
        let row = baton.aoData[baton.aiDisplay[ii]];
        row.nTr = elemTr;
        row.anCells = Array.from(elemTr.children);
    });
    // Table is empty - create a row with an empty message in it
    if (baton.aiDisplay.length === 0) {
        baton.nTBody.innerHTML = (
            `<tr><td colspan="${baton.aoColumns.length}">`
            + `No data available in table`
            + `</td></tr>`
        );
    }
    // Call all required callback functions for the end of a draw
    datatableEventEmit(baton, "aoDrawCallback", "draw", [baton]);
    // Draw is complete, sorting and filtering must be as well
    baton.bSorted = false;
    baton.bFiltered = false;
}

function onDatatableScroll(baton) {
// Scrolling function - fired whenever the scrolling position is changed.
// This method needs to use the stored values to see if the table should be
// redrawn as we are moving towards the end of the information that is
// currently drawn or not. If needed, then it will redraw the table based on
// the new position.
    let iScrollTop = baton.nScroller.scrollTop;
    let iTopRow;
    if (baton.skip) {
        return;
    }
    if (baton.ingnoreScroll) {
        return;
    }
    // If the table has been sorted or filtered, then we use the redraw that
    // DataTables as done, rather than performing our own
    if (baton.bFiltered || baton.bSorted) {
        return;
    }
    // Update the table's information display for what is now in the viewport
    uiRenderScrollerInfo(baton);
    // If scroll point is inside the trigger boundary,
    // we can skip expensive table redraw.
    if (baton.redrawTop <= iScrollTop && iScrollTop <= baton.redrawBottom) {
        baton.topRowFloat = scrollerPixelsToRow(baton, iScrollTop, true);
        return;
    }
    // Perform expensive table redraw.
    iTopRow = (
        parseInt(
            (
                scrollerDomainConvert(baton, true, iScrollTop)
                / DATATABLE_ROW_HEIGHT
            ),
            10
        )
        - Math.ceil(
            0.5 * (DATATABLE_ROW_BUFFER_SCALE - 1) * baton.viewportRows
        )
    );
    baton.topRowFloat = (
        scrollerDomainConvert(baton, true, iScrollTop)
        / DATATABLE_ROW_HEIGHT
    );
    if (iTopRow <= 0) {
        // At the start of the table
        iTopRow = 0;
    } else if (iTopRow + baton._iDisplayLength > baton.iRecordsCount) {
        // At the end of the table
        iTopRow = Math.max(0, baton.iRecordsCount - baton._iDisplayLength);
    } else if (iTopRow % 2 !== 0) {
        // For the row-striping classes (odd/even) we want only to start
        // on evens otherwise the stripes will change between draws and
        // look rubbish
        iTopRow += 1;
    }
    if (iTopRow !== baton._iDisplayStart) {
        // Do the DataTables redraw based on the calculated start point
        // - note that when using server-side processing
        // we introduce a small delay to not DoS the server...
        clearTimeout(baton.drawTO);
        baton.drawTO = setTimeout(function () {
            baton._iDisplayStart = iTopRow;
            onDatatableDraw(baton.dt);
        }, DATATABLE_SCROLLER_WAIT);
        uiRenderDatatableLoading(baton, "block");
    }
}

function scrollerDomainConvert(baton, modePhysicalToVirtual, val) {
// Convert from one domain to another. The physical domain is the actual
// pixel count on the screen, while the virtual is if we had browsers which
// had scrolling containers of infinite height (i.e. the absolute value)
    let coeff;
    let maxScroll;
    let maxVirtual;
    // If the virtual and physical height match, then we use a linear
    // transform between the two, allowing the scrollbar to be linear
    if (baton.scrollerHeightVirtual === baton.scrollerHeightScroll) {
        return val;
    }
    // Otherwise, we want a non-linear scrollbar to take account of the
    // redrawing regions at the start and end of the table, otherwise these
    // can stutter badly - on large tables 30px (for example) scroll might
    // be hundreds of rows, so the table would be redrawing every few px at
    // the start and end. Use a simple quadratic to stop this. It does mean
    // the scrollbar is non-linear, but with such massive data sets, the
    // scrollbar is going to be a best guess anyway
    maxScroll = 0.5 * (baton.scrollerHeightScroll - baton.viewport);
    maxVirtual = 0.5 * (baton.scrollerHeightVirtual - baton.viewport);
    coeff = maxVirtual / (maxScroll * maxScroll);
    // physical to virtual
    if (modePhysicalToVirtual) {
        if (val < maxScroll) {
            return val * val * coeff;
        }
        val = (2 * maxScroll) - val;
        return (
            val < 0
            ? baton.scrollerHeightVirtual
            : (2 * maxVirtual) - (val * val * coeff)
        );
    }
    // virtual to physical
    if (val < maxVirtual) {
        return Math.sqrt(val / coeff);
    }
    val = (2 * maxVirtual) - val;
    return (
        val < 0
        ? baton.scrollerHeightScroll
        : (2 * maxScroll) - Math.sqrt(val / coeff)
    );
}

function scrollerPixelsToRow(baton, pixels, virtual) {
// Calculate the row number that will be found at the given pixel position
// (y-scroll).
// Please note that when the height of the full table exceeds 1 million
// pixels, Scroller switches into a non-linear mode for the scrollbar to fit
// all of the records into a finite area, but this function returns a linear
// value (relative to the last non-linear positioning).
    let diff = pixels - baton.baseScrollTop;
    return (
        virtual
        ? (
            scrollerDomainConvert(
                baton,
                true,
                baton.baseScrollTop
            ) + diff
        ) / DATATABLE_ROW_HEIGHT
        : (diff / DATATABLE_ROW_HEIGHT) + baton.baseRowTop
    );
}

function stringHtmlSafe(str) {
// this function will make <str> html-safe
// https://stackoverflow.com/questions/7381974
    if (typeof str !== "string") {
        str = String(str);
    }
    return str.replace((
        /&/gu
    ), "&amp;").replace((
        /</gu
    ), "&lt;").replace((
        />/gu
    ), "&gt;").replace((
        /"/gu
    ), "&quot;");
}

function uiRenderDatatableLoading(baton, styleDisplay) {
// this function will display or hide the datatable-loading indicator
    baton.nLoading.style.display = styleDisplay;
}

function uiRenderScrollerInfo(baton) {
// Update any information elements that are controlled by the DataTable based
// on the scrolling viewport and what rows are visible in it. This function
// basically acts in the same way as _fnUpdateInfo in DataTables, and
// effectively replaces that function.
    let iScrollTop;
    if (baton.iRecordsCount === 0) {
        baton.nScrollInfo.textContent = "Showing 0 to 0 of 0 entries";
        return;
    }
    iScrollTop = baton.nScroller.scrollTop;
    baton.nScrollInfo.textContent = (
        "Showing "
        + new Intl.NumberFormat("en-US").format(
            Math.floor(scrollerPixelsToRow(baton, iScrollTop, baton.ani) + 1)
        )
        + " to "
        + new Intl.NumberFormat("en-US").format(Math.min(
            baton.iRecordsCount,
            Math.ceil(scrollerPixelsToRow(
                baton,
                iScrollTop + baton.viewport,
                baton.ani
            ))
        ))
        + " of "
        + new Intl.NumberFormat("en-US").format(baton.iRecordsCount)
        + " entries"
    );
}

/*jslint-disable*/
/*
shRawLibFetch
{
    "fetchList": [
        {
            "comment": true,
            "url": "https://github.com/jquery/jquery/blob/3.6.0/LICENSE.txt"
        },
        {
            "url": "https://github.com/jquery/jquery/blob/3.6.0/dist/jquery.js",
            "url2": "https://cdnjs.cloudflare.com/ajax/libs/jquery/3.6.0/jquery.slim.js"
        },
        {
            "comment": true,
            "url": "https://github.com/DataTables/Dist-DataTables/blob/1.10.21/License.txt"
        },
        {
            "url": "https://github.com/DataTables/DataTables/blob/1.10.21/media/js/jquery.dataTables.js"
        },
        {
            "url": "https://github.com/DataTables/Scroller/blob/1.5.1/js/scroller.dataTables.js",
            "url2": "https://cdn.datatables.net/scroller/1.5.1/js/dataTables.scroller.js"
        }
    ],
    "replaceList": [
        {
            "aa": "\t",
            "bb": "  ",
            "flags": "g"
        }
    ]
}
*/


/*
repo https://github.com/jquery/jquery/tree/3.6.0
committed 2021-03-02T17:08:15Z
*/


/*
file https://github.com/jquery/jquery/blob/3.6.0/LICENSE.txt
*/
/*
Copyright OpenJS Foundation and other contributors, https://openjsf.org/

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


/*
file https://github.com/jquery/jquery/blob/3.6.0/dist/jquery.js
*/
/*!
 * jQuery JavaScript Library v3.6.0 -ajax,-ajax/jsonp,-ajax/load,-ajax/script,-ajax/var/location,-ajax/var/nonce,-ajax/var/rquery,-ajax/xhr,-manipulation/_evalUrl,-deprecated/ajax-event-alias,-effects,-effects/Tween,-effects/animatedSelector
 * https://jquery.com/
 *
 * Includes Sizzle.js
 * https://sizzlejs.com/
 *
 * Copyright OpenJS Foundation and other contributors
 * Released under the MIT license
 * https://jquery.org/license
 *
 * Date: 2021-03-02T17:08Z
 */
( function( global, factory ) {

  "use strict";

  if ( typeof module === "object" && typeof module.exports === "object" ) {

    // For CommonJS and CommonJS-like environments where a proper `window`
    // is present, execute the factory and get jQuery.
    // For environments that do not have a `window` with a `document`
    // (such as Node.js), expose a factory as module.exports.
    // This accentuates the need for the creation of a real `window`.
    // e.g. var jQuery = require("jquery")(window);
    // See ticket #14549 for more info.
    module.exports = global.document ?
      factory( global, true ) :
      function( w ) {
        if ( !w.document ) {
          throw new Error( "jQuery requires a window with a document" );
        }
        return factory( w );
      };
  } else {
    factory( global );
  }

// Pass this if window is not defined yet
} )( typeof window !== "undefined" ? window : this, function( window, noGlobal ) {

// Edge <= 12 - 13+, Firefox <=18 - 45+, IE 10 - 11, Safari 5.1 - 9+, iOS 6 - 9.1
// throw exceptions when non-strict code (e.g., ASP.NET 4.5) accesses strict mode
// arguments.callee.caller (trac-13335). But as of jQuery 3.0 (2016), strict mode should be common
// enough that all such attempts are guarded in a try block.
"use strict";

var arr = [];

var getProto = Object.getPrototypeOf;

var slice = arr.slice;

var flat = arr.flat ? function( array ) {
  return arr.flat.call( array );
} : function( array ) {
  return arr.concat.apply( [], array );
};


var push = arr.push;

var indexOf = arr.indexOf;

var class2type = {};

var toString = class2type.toString;

var hasOwn = class2type.hasOwnProperty;

var fnToString = hasOwn.toString;

var ObjectFunctionString = fnToString.call( Object );

var support = {};

var isFunction = function isFunction( obj ) {

    // Support: Chrome <=57, Firefox <=52
    // In some browsers, typeof returns "function" for HTML <object> elements
    // (i.e., `typeof document.createElement( "object" ) === "function"`).
    // We don't want to classify *any* DOM node as a function.
    // Support: QtWeb <=3.8.5, WebKit <=534.34, wkhtmltopdf tool <=0.12.5
    // Plus for old WebKit, typeof returns "function" for HTML collections
    // (e.g., `typeof document.getElementsByTagName("div") === "function"`). (gh-4756)
    return typeof obj === "function" && typeof obj.nodeType !== "number" &&
      typeof obj.item !== "function";
  };


var isWindow = function isWindow( obj ) {
    return obj != null && obj === obj.window;
  };


var document = window.document;


  var preservedScriptAttributes = {
    type: true,
    src: true,
    nonce: true,
    noModule: true
  };

  function DOMEval( code, node, doc ) {
    doc = doc || document;

    var i, val,
      script = doc.createElement( "script" );

    script.text = code;
    if ( node ) {
      for ( i in preservedScriptAttributes ) {

        // Support: Firefox 64+, Edge 18+
        // Some browsers don't support the "nonce" property on scripts.
        // On the other hand, just using `getAttribute` is not enough as
        // the `nonce` attribute is reset to an empty string whenever it
        // becomes browsing-context connected.
        // See https://github.com/whatwg/html/issues/2369
        // See https://html.spec.whatwg.org/#nonce-attributes
        // The `node.getAttribute` check was added for the sake of
        // `jQuery.globalEval` so that it can fake a nonce-containing node
        // via an object.
        val = node[ i ] || node.getAttribute && node.getAttribute( i );
        if ( val ) {
          script.setAttribute( i, val );
        }
      }
    }
    doc.head.appendChild( script ).parentNode.removeChild( script );
  }


function toType( obj ) {
  if ( obj == null ) {
    return obj + "";
  }

  // Support: Android <=2.3 only (functionish RegExp)
  return typeof obj === "object" || typeof obj === "function" ?
    class2type[ toString.call( obj ) ] || "object" :
    typeof obj;
}
/* global Symbol */
// Defining this global in .eslintrc.json would create a danger of using the global
// unguarded in another place, it seems safer to define global only for this module


var
  version = "3.6.0 -ajax,-ajax/jsonp,-ajax/load,-ajax/script,-ajax/var/location,-ajax/var/nonce,-ajax/var/rquery,-ajax/xhr,-manipulation/_evalUrl,-deprecated/ajax-event-alias,-effects,-effects/Tween,-effects/animatedSelector",

  // Define a local copy of jQuery
  jQuery = function( selector, context ) {

    // The jQuery object is actually just the init constructor 'enhanced'
    // Need init if jQuery is called (just allow error to be thrown if not included)
    return new jQuery.fn.init( selector, context );
  };

jQuery.fn = jQuery.prototype = {

  // The current version of jQuery being used
  jquery: version,

  constructor: jQuery,

  // The default length of a jQuery object is 0
  length: 0,

  toArray: function() {
    return slice.call( this );
  },

  // Get the Nth element in the matched element set OR
  // Get the whole matched element set as a clean array
  get: function( num ) {

    // Return all the elements in a clean array
    if ( num == null ) {
      return slice.call( this );
    }

    // Return just the one element from the set
    return num < 0 ? this[ num + this.length ] : this[ num ];
  },

  // Take an array of elements and push it onto the stack
  // (returning the new matched element set)
  pushStack: function( elems ) {

    // Build a new jQuery matched element set
    var ret = jQuery.merge( this.constructor(), elems );

    // Add the old object onto the stack (as a reference)
    ret.prevObject = this;

    // Return the newly-formed element set
    return ret;
  },

  // Execute a callback for every element in the matched set.
  each: function( callback ) {
    return jQuery.each( this, callback );
  },

  map: function( callback ) {
    return this.pushStack( jQuery.map( this, function( elem, i ) {
      return callback.call( elem, i, elem );
    } ) );
  },

  slice: function() {
    return this.pushStack( slice.apply( this, arguments ) );
  },

  first: function() {
    return this.eq( 0 );
  },

  last: function() {
    return this.eq( -1 );
  },

  even: function() {
    return this.pushStack( jQuery.grep( this, function( _elem, i ) {
      return ( i + 1 ) % 2;
    } ) );
  },

  odd: function() {
    return this.pushStack( jQuery.grep( this, function( _elem, i ) {
      return i % 2;
    } ) );
  },

  eq: function( i ) {
    var len = this.length,
      j = +i + ( i < 0 ? len : 0 );
    return this.pushStack( j >= 0 && j < len ? [ this[ j ] ] : [] );
  },

  end: function() {
    return this.prevObject || this.constructor();
  },

  // For internal use only.
  // Behaves like an Array's method, not like a jQuery method.
  push: push,
  sort: arr.sort,
  splice: arr.splice
};

jQuery.extend = jQuery.fn.extend = function() {
  var options, name, src, copy, copyIsArray, clone,
    target = arguments[ 0 ] || {},
    i = 1,
    length = arguments.length,
    deep = false;

  // Handle a deep copy situation
  if ( typeof target === "boolean" ) {
    deep = target;

    // Skip the boolean and the target
    target = arguments[ i ] || {};
    i++;
  }

  // Handle case when target is a string or something (possible in deep copy)
  if ( typeof target !== "object" && !isFunction( target ) ) {
    target = {};
  }

  // Extend jQuery itself if only one argument is passed
  if ( i === length ) {
    target = this;
    i--;
  }

  for ( ; i < length; i++ ) {

    // Only deal with non-null/undefined values
    if ( ( options = arguments[ i ] ) != null ) {

      // Extend the base object
      for ( name in options ) {
        copy = options[ name ];

        // Prevent Object.prototype pollution
        // Prevent never-ending loop
        if ( name === "__proto__" || target === copy ) {
          continue;
        }

        // Recurse if we're merging plain objects or arrays
        if ( deep && copy && ( jQuery.isPlainObject( copy ) ||
          ( copyIsArray = Array.isArray( copy ) ) ) ) {
          src = target[ name ];

          // Ensure proper type for the source value
          if ( copyIsArray && !Array.isArray( src ) ) {
            clone = [];
          } else if ( !copyIsArray && !jQuery.isPlainObject( src ) ) {
            clone = {};
          } else {
            clone = src;
          }
          copyIsArray = false;

          // Never move original objects, clone them
          target[ name ] = jQuery.extend( deep, clone, copy );

        // Don't bring in undefined values
        } else if ( copy !== undefined ) {
          target[ name ] = copy;
        }
      }
    }
  }

  // Return the modified object
  return target;
};

jQuery.extend( {

  // Unique for each copy of jQuery on the page
  expando: "jQuery" + ( version + Math.random() ).replace( /\D/g, "" ),

  // Assume jQuery is ready without the ready module
  isReady: true,

  error: function( msg ) {
    throw new Error( msg );
  },

  noop: function() {},

  isPlainObject: function( obj ) {
    var proto, Ctor;

    // Detect obvious negatives
    // Use toString instead of jQuery.type to catch host objects
    if ( !obj || toString.call( obj ) !== "[object Object]" ) {
      return false;
    }

    proto = getProto( obj );

    // Objects with no prototype (e.g., `Object.create( null )`) are plain
    if ( !proto ) {
      return true;
    }

    // Objects with prototype are plain iff they were constructed by a global Object function
    Ctor = hasOwn.call( proto, "constructor" ) && proto.constructor;
    return typeof Ctor === "function" && fnToString.call( Ctor ) === ObjectFunctionString;
  },

  isEmptyObject: function( obj ) {
    var name;

    for ( name in obj ) {
      return false;
    }
    return true;
  },

  // Evaluates a script in a provided context; falls back to the global one
  // if not specified.
  globalEval: function( code, options, doc ) {
    DOMEval( code, { nonce: options && options.nonce }, doc );
  },

  each: function( obj, callback ) {
    var length, i = 0;

    if ( isArrayLike( obj ) ) {
      length = obj.length;
      for ( ; i < length; i++ ) {
        if ( callback.call( obj[ i ], i, obj[ i ] ) === false ) {
          break;
        }
      }
    } else {
      for ( i in obj ) {
        if ( callback.call( obj[ i ], i, obj[ i ] ) === false ) {
          break;
        }
      }
    }

    return obj;
  },

  // results is for internal usage only
  makeArray: function( arr, results ) {
    var ret = results || [];

    if ( arr != null ) {
      if ( isArrayLike( Object( arr ) ) ) {
        jQuery.merge( ret,
          typeof arr === "string" ?
            [ arr ] : arr
        );
      } else {
        push.call( ret, arr );
      }
    }

    return ret;
  },

  inArray: function( elem, arr, i ) {
    return arr == null ? -1 : indexOf.call( arr, elem, i );
  },

  // Support: Android <=4.0 only, PhantomJS 1 only
  // push.apply(_, arraylike) throws on ancient WebKit
  merge: function( first, second ) {
    var len = +second.length,
      j = 0,
      i = first.length;

    for ( ; j < len; j++ ) {
      first[ i++ ] = second[ j ];
    }

    first.length = i;

    return first;
  },

  grep: function( elems, callback, invert ) {
    var callbackInverse,
      matches = [],
      i = 0,
      length = elems.length,
      callbackExpect = !invert;

    // Go through the array, only saving the items
    // that pass the validator function
    for ( ; i < length; i++ ) {
      callbackInverse = !callback( elems[ i ], i );
      if ( callbackInverse !== callbackExpect ) {
        matches.push( elems[ i ] );
      }
    }

    return matches;
  },

  // arg is for internal usage only
  map: function( elems, callback, arg ) {
    var length, value,
      i = 0,
      ret = [];

    // Go through the array, translating each of the items to their new values
    if ( isArrayLike( elems ) ) {
      length = elems.length;
      for ( ; i < length; i++ ) {
        value = callback( elems[ i ], i, arg );

        if ( value != null ) {
          ret.push( value );
        }
      }

    // Go through every key on the object,
    } else {
      for ( i in elems ) {
        value = callback( elems[ i ], i, arg );

        if ( value != null ) {
          ret.push( value );
        }
      }
    }

    // Flatten any nested arrays
    return flat( ret );
  },

  // A global GUID counter for objects
  guid: 1,

  // jQuery.support is not used in Core but other projects attach their
  // properties to it so it needs to exist.
  support: support
} );

if ( typeof Symbol === "function" ) {
  jQuery.fn[ Symbol.iterator ] = arr[ Symbol.iterator ];
}

// Populate the class2type map
jQuery.each( "Boolean Number String Function Array Date RegExp Object Error Symbol".split( " " ),
  function( _i, name ) {
    class2type[ "[object " + name + "]" ] = name.toLowerCase();
  } );

function isArrayLike( obj ) {

  // Support: real iOS 8.2 only (not reproducible in simulator)
  // `in` check used to prevent JIT error (gh-2145)
  // hasOwn isn't used here due to false negatives
  // regarding Nodelist length in IE
  var length = !!obj && "length" in obj && obj.length,
    type = toType( obj );

  if ( isFunction( obj ) || isWindow( obj ) ) {
    return false;
  }

  return type === "array" || length === 0 ||
    typeof length === "number" && length > 0 && ( length - 1 ) in obj;
}
var Sizzle =
/*!
 * Sizzle CSS Selector Engine v2.3.6
 * https://sizzlejs.com/
 *
 * Copyright JS Foundation and other contributors
 * Released under the MIT license
 * https://js.foundation/
 *
 * Date: 2021-02-16
 */
( function( window ) {
var i,
  support,
  Expr,
  getText,
  isXML,
  tokenize,
  compile,
  select,
  outermostContext,
  sortInput,
  hasDuplicate,

  // Local document vars
  setDocument,
  document,
  docElem,
  documentIsHTML,
  rbuggyQSA,
  rbuggyMatches,
  matches,
  contains,

  // Instance-specific data
  expando = "sizzle" + 1 * new Date(),
  preferredDoc = window.document,
  dirruns = 0,
  done = 0,
  classCache = createCache(),
  tokenCache = createCache(),
  compilerCache = createCache(),
  nonnativeSelectorCache = createCache(),
  sortOrder = function( a, b ) {
    if ( a === b ) {
      hasDuplicate = true;
    }
    return 0;
  },

  // Instance methods
  hasOwn = ( {} ).hasOwnProperty,
  arr = [],
  pop = arr.pop,
  pushNative = arr.push,
  push = arr.push,
  slice = arr.slice,

  // Use a stripped-down indexOf as it's faster than native
  // https://jsperf.com/thor-indexof-vs-for/5
  indexOf = function( list, elem ) {
    var i = 0,
      len = list.length;
    for ( ; i < len; i++ ) {
      if ( list[ i ] === elem ) {
        return i;
      }
    }
    return -1;
  },

  booleans = "checked|selected|async|autofocus|autoplay|controls|defer|disabled|hidden|" +
    "ismap|loop|multiple|open|readonly|required|scoped",

  // Regular expressions

  // http://www.w3.org/TR/css3-selectors/#whitespace
  whitespace = "[\\x20\\t\\r\\n\\f]",

  // https://www.w3.org/TR/css-syntax-3/#ident-token-diagram
  identifier = "(?:\\\\[\\da-fA-F]{1,6}" + whitespace +
    "?|\\\\[^\\r\\n\\f]|[\\w-]|[^\0-\\x7f])+",

  // Attribute selectors: http://www.w3.org/TR/selectors/#attribute-selectors
  attributes = "\\[" + whitespace + "*(" + identifier + ")(?:" + whitespace +

    // Operator (capture 2)
    "*([*^$|!~]?=)" + whitespace +

    // "Attribute values must be CSS identifiers [capture 5]
    // or strings [capture 3 or capture 4]"
    "*(?:'((?:\\\\.|[^\\\\'])*)'|\"((?:\\\\.|[^\\\\\"])*)\"|(" + identifier + "))|)" +
    whitespace + "*\\]",

  pseudos = ":(" + identifier + ")(?:\\((" +

    // To reduce the number of selectors needing tokenize in the preFilter, prefer arguments:
    // 1. quoted (capture 3; capture 4 or capture 5)
    "('((?:\\\\.|[^\\\\'])*)'|\"((?:\\\\.|[^\\\\\"])*)\")|" +

    // 2. simple (capture 6)
    "((?:\\\\.|[^\\\\()[\\]]|" + attributes + ")*)|" +

    // 3. anything else (capture 2)
    ".*" +
    ")\\)|)",

  // Leading and non-escaped trailing whitespace, capturing some non-whitespace characters preceding the latter
  rwhitespace = new RegExp( whitespace + "+", "g" ),
  rtrim = new RegExp( "^" + whitespace + "+|((?:^|[^\\\\])(?:\\\\.)*)" +
    whitespace + "+$", "g" ),

  rcomma = new RegExp( "^" + whitespace + "*," + whitespace + "*" ),
  rcombinators = new RegExp( "^" + whitespace + "*([>+~]|" + whitespace + ")" + whitespace +
    "*" ),
  rdescend = new RegExp( whitespace + "|>" ),

  rpseudo = new RegExp( pseudos ),
  ridentifier = new RegExp( "^" + identifier + "$" ),

  matchExpr = {
    "ID": new RegExp( "^#(" + identifier + ")" ),
    "CLASS": new RegExp( "^\\.(" + identifier + ")" ),
    "TAG": new RegExp( "^(" + identifier + "|[*])" ),
    "ATTR": new RegExp( "^" + attributes ),
    "PSEUDO": new RegExp( "^" + pseudos ),
    "CHILD": new RegExp( "^:(only|first|last|nth|nth-last)-(child|of-type)(?:\\(" +
      whitespace + "*(even|odd|(([+-]|)(\\d*)n|)" + whitespace + "*(?:([+-]|)" +
      whitespace + "*(\\d+)|))" + whitespace + "*\\)|)", "i" ),
    "bool": new RegExp( "^(?:" + booleans + ")$", "i" ),

    // For use in libraries implementing .is()
    // We use this for POS matching in `select`
    "needsContext": new RegExp( "^" + whitespace +
      "*[>+~]|:(even|odd|eq|gt|lt|nth|first|last)(?:\\(" + whitespace +
      "*((?:-\\d)?\\d*)" + whitespace + "*\\)|)(?=[^-]|$)", "i" )
  },

  rhtml = /HTML$/i,
  rinputs = /^(?:input|select|textarea|button)$/i,
  rheader = /^h\d$/i,

  rnative = /^[^{]+\{\s*\[native \w/,

  // Easily-parseable/retrievable ID or TAG or CLASS selectors
  rquickExpr = /^(?:#([\w-]+)|(\w+)|\.([\w-]+))$/,

  rsibling = /[+~]/,

  // CSS escapes
  // http://www.w3.org/TR/CSS21/syndata.html#escaped-characters
  runescape = new RegExp( "\\\\[\\da-fA-F]{1,6}" + whitespace + "?|\\\\([^\\r\\n\\f])", "g" ),
  funescape = function( escape, nonHex ) {
    var high = "0x" + escape.slice( 1 ) - 0x10000;

    return nonHex ?

      // Strip the backslash prefix from a non-hex escape sequence
      nonHex :

      // Replace a hexadecimal escape sequence with the encoded Unicode code point
      // Support: IE <=11+
      // For values outside the Basic Multilingual Plane (BMP), manually construct a
      // surrogate pair
      high < 0 ?
        String.fromCharCode( high + 0x10000 ) :
        String.fromCharCode( high >> 10 | 0xD800, high & 0x3FF | 0xDC00 );
  },

  // CSS string/identifier serialization
  // https://drafts.csswg.org/cssom/#common-serializing-idioms
  rcssescape = /([\0-\x1f\x7f]|^-?\d)|^-$|[^\0-\x1f\x7f-\uFFFF\w-]/g,
  fcssescape = function( ch, asCodePoint ) {
    if ( asCodePoint ) {

      // U+0000 NULL becomes U+FFFD REPLACEMENT CHARACTER
      if ( ch === "\0" ) {
        return "\uFFFD";
      }

      // Control characters and (dependent upon position) numbers get escaped as code points
      return ch.slice( 0, -1 ) + "\\" +
        ch.charCodeAt( ch.length - 1 ).toString( 16 ) + " ";
    }

    // Other potentially-special ASCII characters get backslash-escaped
    return "\\" + ch;
  },

  // Used for iframes
  // See setDocument()
  // Removing the function wrapper causes a "Permission Denied"
  // error in IE
  unloadHandler = function() {
    setDocument();
  },

  inDisabledFieldset = addCombinator(
    function( elem ) {
      return elem.disabled === true && elem.nodeName.toLowerCase() === "fieldset";
    },
    { dir: "parentNode", next: "legend" }
  );

// Optimize for push.apply( _, NodeList )
try {
  push.apply(
    ( arr = slice.call( preferredDoc.childNodes ) ),
    preferredDoc.childNodes
  );

  // Support: Android<4.0
  // Detect silently failing push.apply
  // eslint-disable-next-line no-unused-expressions
  arr[ preferredDoc.childNodes.length ].nodeType;
} catch ( e ) {
  push = { apply: arr.length ?

    // Leverage slice if possible
    function( target, els ) {
      pushNative.apply( target, slice.call( els ) );
    } :

    // Support: IE<9
    // Otherwise append directly
    function( target, els ) {
      var j = target.length,
        i = 0;

      // Can't trust NodeList.length
      while ( ( target[ j++ ] = els[ i++ ] ) ) {}
      target.length = j - 1;
    }
  };
}

function Sizzle( selector, context, results, seed ) {
  var m, i, elem, nid, match, groups, newSelector,
    newContext = context && context.ownerDocument,

    // nodeType defaults to 9, since context defaults to document
    nodeType = context ? context.nodeType : 9;

  results = results || [];

  // Return early from calls with invalid selector or context
  if ( typeof selector !== "string" || !selector ||
    nodeType !== 1 && nodeType !== 9 && nodeType !== 11 ) {

    return results;
  }

  // Try to shortcut find operations (as opposed to filters) in HTML documents
  if ( !seed ) {
    setDocument( context );
    context = context || document;

    if ( documentIsHTML ) {

      // If the selector is sufficiently simple, try using a "get*By*" DOM method
      // (excepting DocumentFragment context, where the methods don't exist)
      if ( nodeType !== 11 && ( match = rquickExpr.exec( selector ) ) ) {

        // ID selector
        if ( ( m = match[ 1 ] ) ) {

          // Document context
          if ( nodeType === 9 ) {
            if ( ( elem = context.getElementById( m ) ) ) {

              // Support: IE, Opera, Webkit
              // TODO: identify versions
              // getElementById can match elements by name instead of ID
              if ( elem.id === m ) {
                results.push( elem );
                return results;
              }
            } else {
              return results;
            }

          // Element context
          } else {

            // Support: IE, Opera, Webkit
            // TODO: identify versions
            // getElementById can match elements by name instead of ID
            if ( newContext && ( elem = newContext.getElementById( m ) ) &&
              contains( context, elem ) &&
              elem.id === m ) {

              results.push( elem );
              return results;
            }
          }

        // Type selector
        } else if ( match[ 2 ] ) {
          push.apply( results, context.getElementsByTagName( selector ) );
          return results;

        // Class selector
        } else if ( ( m = match[ 3 ] ) && support.getElementsByClassName &&
          context.getElementsByClassName ) {

          push.apply( results, context.getElementsByClassName( m ) );
          return results;
        }
      }

      // Take advantage of querySelectorAll
      if ( support.qsa &&
        !nonnativeSelectorCache[ selector + " " ] &&
        ( !rbuggyQSA || !rbuggyQSA.test( selector ) ) &&

        // Support: IE 8 only
        // Exclude object elements
        ( nodeType !== 1 || context.nodeName.toLowerCase() !== "object" ) ) {

        newSelector = selector;
        newContext = context;

        // qSA considers elements outside a scoping root when evaluating child or
        // descendant combinators, which is not what we want.
        // In such cases, we work around the behavior by prefixing every selector in the
        // list with an ID selector referencing the scope context.
        // The technique has to be used as well when a leading combinator is used
        // as such selectors are not recognized by querySelectorAll.
        // Thanks to Andrew Dupont for this technique.
        if ( nodeType === 1 &&
          ( rdescend.test( selector ) || rcombinators.test( selector ) ) ) {

          // Expand context for sibling selectors
          newContext = rsibling.test( selector ) && testContext( context.parentNode ) ||
            context;

          // We can use :scope instead of the ID hack if the browser
          // supports it & if we're not changing the context.
          if ( newContext !== context || !support.scope ) {

            // Capture the context ID, setting it first if necessary
            if ( ( nid = context.getAttribute( "id" ) ) ) {
              nid = nid.replace( rcssescape, fcssescape );
            } else {
              context.setAttribute( "id", ( nid = expando ) );
            }
          }

          // Prefix every selector in the list
          groups = tokenize( selector );
          i = groups.length;
          while ( i-- ) {
            groups[ i ] = ( nid ? "#" + nid : ":scope" ) + " " +
              toSelector( groups[ i ] );
          }
          newSelector = groups.join( "," );
        }

        try {
          push.apply( results,
            newContext.querySelectorAll( newSelector )
          );
          return results;
        } catch ( qsaError ) {
          nonnativeSelectorCache( selector, true );
        } finally {
          if ( nid === expando ) {
            context.removeAttribute( "id" );
          }
        }
      }
    }
  }

  // All others
  return select( selector.replace( rtrim, "$1" ), context, results, seed );
}

/**
 * Create key-value caches of limited size
 * @returns {function(string, object)} Returns the Object data after storing it on itself with
 *  property name the (space-suffixed) string and (if the cache is larger than Expr.cacheLength)
 *  deleting the oldest entry
 */
function createCache() {
  var keys = [];

  function cache( key, value ) {

    // Use (key + " ") to avoid collision with native prototype properties (see Issue #157)
    if ( keys.push( key + " " ) > Expr.cacheLength ) {

      // Only keep the most recent entries
      delete cache[ keys.shift() ];
    }
    return ( cache[ key + " " ] = value );
  }
  return cache;
}

/**
 * Mark a function for special use by Sizzle
 * @param {Function} fn The function to mark
 */
function markFunction( fn ) {
  fn[ expando ] = true;
  return fn;
}

/**
 * Support testing using an element
 * @param {Function} fn Passed the created element and returns a boolean result
 */
function assert( fn ) {
  var el = document.createElement( "fieldset" );

  try {
    return !!fn( el );
  } catch ( e ) {
    return false;
  } finally {

    // Remove from its parent by default
    if ( el.parentNode ) {
      el.parentNode.removeChild( el );
    }

    // release memory in IE
    el = null;
  }
}

/**
 * Adds the same handler for all of the specified attrs
 * @param {String} attrs Pipe-separated list of attributes
 * @param {Function} handler The method that will be applied
 */
function addHandle( attrs, handler ) {
  var arr = attrs.split( "|" ),
    i = arr.length;

  while ( i-- ) {
    Expr.attrHandle[ arr[ i ] ] = handler;
  }
}

/**
 * Checks document order of two siblings
 * @param {Element} a
 * @param {Element} b
 * @returns {Number} Returns less than 0 if a precedes b, greater than 0 if a follows b
 */
function siblingCheck( a, b ) {
  var cur = b && a,
    diff = cur && a.nodeType === 1 && b.nodeType === 1 &&
      a.sourceIndex - b.sourceIndex;

  // Use IE sourceIndex if available on both nodes
  if ( diff ) {
    return diff;
  }

  // Check if b follows a
  if ( cur ) {
    while ( ( cur = cur.nextSibling ) ) {
      if ( cur === b ) {
        return -1;
      }
    }
  }

  return a ? 1 : -1;
}

/**
 * Returns a function to use in pseudos for input types
 * @param {String} type
 */
function createInputPseudo( type ) {
  return function( elem ) {
    var name = elem.nodeName.toLowerCase();
    return name === "input" && elem.type === type;
  };
}

/**
 * Returns a function to use in pseudos for buttons
 * @param {String} type
 */
function createButtonPseudo( type ) {
  return function( elem ) {
    var name = elem.nodeName.toLowerCase();
    return ( name === "input" || name === "button" ) && elem.type === type;
  };
}

/**
 * Returns a function to use in pseudos for :enabled/:disabled
 * @param {Boolean} disabled true for :disabled; false for :enabled
 */
function createDisabledPseudo( disabled ) {

  // Known :disabled false positives: fieldset[disabled] > legend:nth-of-type(n+2) :can-disable
  return function( elem ) {

    // Only certain elements can match :enabled or :disabled
    // https://html.spec.whatwg.org/multipage/scripting.html#selector-enabled
    // https://html.spec.whatwg.org/multipage/scripting.html#selector-disabled
    if ( "form" in elem ) {

      // Check for inherited disabledness on relevant non-disabled elements:
      // * listed form-associated elements in a disabled fieldset
      //   https://html.spec.whatwg.org/multipage/forms.html#category-listed
      //   https://html.spec.whatwg.org/multipage/forms.html#concept-fe-disabled
      // * option elements in a disabled optgroup
      //   https://html.spec.whatwg.org/multipage/forms.html#concept-option-disabled
      // All such elements have a "form" property.
      if ( elem.parentNode && elem.disabled === false ) {

        // Option elements defer to a parent optgroup if present
        if ( "label" in elem ) {
          if ( "label" in elem.parentNode ) {
            return elem.parentNode.disabled === disabled;
          } else {
            return elem.disabled === disabled;
          }
        }

        // Support: IE 6 - 11
        // Use the isDisabled shortcut property to check for disabled fieldset ancestors
        return elem.isDisabled === disabled ||

          // Where there is no isDisabled, check manually
          /* jshint -W018 */
          elem.isDisabled !== !disabled &&
          inDisabledFieldset( elem ) === disabled;
      }

      return elem.disabled === disabled;

    // Try to winnow out elements that can't be disabled before trusting the disabled property.
    // Some victims get caught in our net (label, legend, menu, track), but it shouldn't
    // even exist on them, let alone have a boolean value.
    } else if ( "label" in elem ) {
      return elem.disabled === disabled;
    }

    // Remaining elements are neither :enabled nor :disabled
    return false;
  };
}

/**
 * Returns a function to use in pseudos for positionals
 * @param {Function} fn
 */
function createPositionalPseudo( fn ) {
  return markFunction( function( argument ) {
    argument = +argument;
    return markFunction( function( seed, matches ) {
      var j,
        matchIndexes = fn( [], seed.length, argument ),
        i = matchIndexes.length;

      // Match elements found at the specified indexes
      while ( i-- ) {
        if ( seed[ ( j = matchIndexes[ i ] ) ] ) {
          seed[ j ] = !( matches[ j ] = seed[ j ] );
        }
      }
    } );
  } );
}

/**
 * Checks a node for validity as a Sizzle context
 * @param {Element|Object=} context
 * @returns {Element|Object|Boolean} The input node if acceptable, otherwise a falsy value
 */
function testContext( context ) {
  return context && typeof context.getElementsByTagName !== "undefined" && context;
}

// Expose support vars for convenience
support = Sizzle.support = {};

/**
 * Detects XML nodes
 * @param {Element|Object} elem An element or a document
 * @returns {Boolean} True iff elem is a non-HTML XML node
 */
isXML = Sizzle.isXML = function( elem ) {
  var namespace = elem && elem.namespaceURI,
    docElem = elem && ( elem.ownerDocument || elem ).documentElement;

  // Support: IE <=8
  // Assume HTML when documentElement doesn't yet exist, such as inside loading iframes
  // https://bugs.jquery.com/ticket/4833
  return !rhtml.test( namespace || docElem && docElem.nodeName || "HTML" );
};

/**
 * Sets document-related variables once based on the current document
 * @param {Element|Object} [doc] An element or document object to use to set the document
 * @returns {Object} Returns the current document
 */
setDocument = Sizzle.setDocument = function( node ) {
  var hasCompare, subWindow,
    doc = node ? node.ownerDocument || node : preferredDoc;

  // Return early if doc is invalid or already selected
  // Support: IE 11+, Edge 17 - 18+
  // IE/Edge sometimes throw a "Permission denied" error when strict-comparing
  // two documents; shallow comparisons work.
  // eslint-disable-next-line eqeqeq
  if ( doc == document || doc.nodeType !== 9 || !doc.documentElement ) {
    return document;
  }

  // Update global variables
  document = doc;
  docElem = document.documentElement;
  documentIsHTML = !isXML( document );

  // Support: IE 9 - 11+, Edge 12 - 18+
  // Accessing iframe documents after unload throws "permission denied" errors (jQuery #13936)
  // Support: IE 11+, Edge 17 - 18+
  // IE/Edge sometimes throw a "Permission denied" error when strict-comparing
  // two documents; shallow comparisons work.
  // eslint-disable-next-line eqeqeq
  if ( preferredDoc != document &&
    ( subWindow = document.defaultView ) && subWindow.top !== subWindow ) {

    // Support: IE 11, Edge
    if ( subWindow.addEventListener ) {
      subWindow.addEventListener( "unload", unloadHandler, false );

    // Support: IE 9 - 10 only
    } else if ( subWindow.attachEvent ) {
      subWindow.attachEvent( "onunload", unloadHandler );
    }
  }

  // Support: IE 8 - 11+, Edge 12 - 18+, Chrome <=16 - 25 only, Firefox <=3.6 - 31 only,
  // Safari 4 - 5 only, Opera <=11.6 - 12.x only
  // IE/Edge & older browsers don't support the :scope pseudo-class.
  // Support: Safari 6.0 only
  // Safari 6.0 supports :scope but it's an alias of :root there.
  support.scope = assert( function( el ) {
    docElem.appendChild( el ).appendChild( document.createElement( "div" ) );
    return typeof el.querySelectorAll !== "undefined" &&
      !el.querySelectorAll( ":scope fieldset div" ).length;
  } );

  /* Attributes
  ---------------------------------------------------------------------- */

  // Support: IE<8
  // Verify that getAttribute really returns attributes and not properties
  // (excepting IE8 booleans)
  support.attributes = assert( function( el ) {
    el.className = "i";
    return !el.getAttribute( "className" );
  } );

  /* getElement(s)By*
  ---------------------------------------------------------------------- */

  // Check if getElementsByTagName("*") returns only elements
  support.getElementsByTagName = assert( function( el ) {
    el.appendChild( document.createComment( "" ) );
    return !el.getElementsByTagName( "*" ).length;
  } );

  // Support: IE<9
  support.getElementsByClassName = rnative.test( document.getElementsByClassName );

  // Support: IE<10
  // Check if getElementById returns elements by name
  // The broken getElementById methods don't pick up programmatically-set names,
  // so use a roundabout getElementsByName test
  support.getById = assert( function( el ) {
    docElem.appendChild( el ).id = expando;
    return !document.getElementsByName || !document.getElementsByName( expando ).length;
  } );

  // ID filter and find
  if ( support.getById ) {
    Expr.filter[ "ID" ] = function( id ) {
      var attrId = id.replace( runescape, funescape );
      return function( elem ) {
        return elem.getAttribute( "id" ) === attrId;
      };
    };
    Expr.find[ "ID" ] = function( id, context ) {
      if ( typeof context.getElementById !== "undefined" && documentIsHTML ) {
        var elem = context.getElementById( id );
        return elem ? [ elem ] : [];
      }
    };
  } else {
    Expr.filter[ "ID" ] =  function( id ) {
      var attrId = id.replace( runescape, funescape );
      return function( elem ) {
        var node = typeof elem.getAttributeNode !== "undefined" &&
          elem.getAttributeNode( "id" );
        return node && node.value === attrId;
      };
    };

    // Support: IE 6 - 7 only
    // getElementById is not reliable as a find shortcut
    Expr.find[ "ID" ] = function( id, context ) {
      if ( typeof context.getElementById !== "undefined" && documentIsHTML ) {
        var node, i, elems,
          elem = context.getElementById( id );

        if ( elem ) {

          // Verify the id attribute
          node = elem.getAttributeNode( "id" );
          if ( node && node.value === id ) {
            return [ elem ];
          }

          // Fall back on getElementsByName
          elems = context.getElementsByName( id );
          i = 0;
          while ( ( elem = elems[ i++ ] ) ) {
            node = elem.getAttributeNode( "id" );
            if ( node && node.value === id ) {
              return [ elem ];
            }
          }
        }

        return [];
      }
    };
  }

  // Tag
  Expr.find[ "TAG" ] = support.getElementsByTagName ?
    function( tag, context ) {
      if ( typeof context.getElementsByTagName !== "undefined" ) {
        return context.getElementsByTagName( tag );

      // DocumentFragment nodes don't have gEBTN
      } else if ( support.qsa ) {
        return context.querySelectorAll( tag );
      }
    } :

    function( tag, context ) {
      var elem,
        tmp = [],
        i = 0,

        // By happy coincidence, a (broken) gEBTN appears on DocumentFragment nodes too
        results = context.getElementsByTagName( tag );

      // Filter out possible comments
      if ( tag === "*" ) {
        while ( ( elem = results[ i++ ] ) ) {
          if ( elem.nodeType === 1 ) {
            tmp.push( elem );
          }
        }

        return tmp;
      }
      return results;
    };

  // Class
  Expr.find[ "CLASS" ] = support.getElementsByClassName && function( className, context ) {
    if ( typeof context.getElementsByClassName !== "undefined" && documentIsHTML ) {
      return context.getElementsByClassName( className );
    }
  };

  /* QSA/matchesSelector
  ---------------------------------------------------------------------- */

  // QSA and matchesSelector support

  // matchesSelector(:active) reports false when true (IE9/Opera 11.5)
  rbuggyMatches = [];

  // qSa(:focus) reports false when true (Chrome 21)
  // We allow this because of a bug in IE8/9 that throws an error
  // whenever `document.activeElement` is accessed on an iframe
  // So, we allow :focus to pass through QSA all the time to avoid the IE error
  // See https://bugs.jquery.com/ticket/13378
  rbuggyQSA = [];

  if ( ( support.qsa = rnative.test( document.querySelectorAll ) ) ) {

    // Build QSA regex
    // Regex strategy adopted from Diego Perini
    assert( function( el ) {

      var input;

      // Select is set to empty string on purpose
      // This is to test IE's treatment of not explicitly
      // setting a boolean content attribute,
      // since its presence should be enough
      // https://bugs.jquery.com/ticket/12359
      docElem.appendChild( el ).innerHTML = "<a id='" + expando + "'></a>" +
        "<select id='" + expando + "-\r\\' msallowcapture=''>" +
        "<option selected=''></option></select>";

      // Support: IE8, Opera 11-12.16
      // Nothing should be selected when empty strings follow ^= or $= or *=
      // The test attribute must be unknown in Opera but "safe" for WinRT
      // https://msdn.microsoft.com/en-us/library/ie/hh465388.aspx#attribute_section
      if ( el.querySelectorAll( "[msallowcapture^='']" ).length ) {
        rbuggyQSA.push( "[*^$]=" + whitespace + "*(?:''|\"\")" );
      }

      // Support: IE8
      // Boolean attributes and "value" are not treated correctly
      if ( !el.querySelectorAll( "[selected]" ).length ) {
        rbuggyQSA.push( "\\[" + whitespace + "*(?:value|" + booleans + ")" );
      }

      // Support: Chrome<29, Android<4.4, Safari<7.0+, iOS<7.0+, PhantomJS<1.9.8+
      if ( !el.querySelectorAll( "[id~=" + expando + "-]" ).length ) {
        rbuggyQSA.push( "~=" );
      }

      // Support: IE 11+, Edge 15 - 18+
      // IE 11/Edge don't find elements on a `[name='']` query in some cases.
      // Adding a temporary attribute to the document before the selection works
      // around the issue.
      // Interestingly, IE 10 & older don't seem to have the issue.
      input = document.createElement( "input" );
      input.setAttribute( "name", "" );
      el.appendChild( input );
      if ( !el.querySelectorAll( "[name='']" ).length ) {
        rbuggyQSA.push( "\\[" + whitespace + "*name" + whitespace + "*=" +
          whitespace + "*(?:''|\"\")" );
      }

      // Webkit/Opera - :checked should return selected option elements
      // http://www.w3.org/TR/2011/REC-css3-selectors-20110929/#checked
      // IE8 throws error here and will not see later tests
      if ( !el.querySelectorAll( ":checked" ).length ) {
        rbuggyQSA.push( ":checked" );
      }

      // Support: Safari 8+, iOS 8+
      // https://bugs.webkit.org/show_bug.cgi?id=136851
      // In-page `selector#id sibling-combinator selector` fails
      if ( !el.querySelectorAll( "a#" + expando + "+*" ).length ) {
        rbuggyQSA.push( ".#.+[+~]" );
      }

      // Support: Firefox <=3.6 - 5 only
      // Old Firefox doesn't throw on a badly-escaped identifier.
      el.querySelectorAll( "\\\f" );
      rbuggyQSA.push( "[\\r\\n\\f]" );
    } );

    assert( function( el ) {
      el.innerHTML = "<a href='' disabled='disabled'></a>" +
        "<select disabled='disabled'><option/></select>";

      // Support: Windows 8 Native Apps
      // The type and name attributes are restricted during .innerHTML assignment
      var input = document.createElement( "input" );
      input.setAttribute( "type", "hidden" );
      el.appendChild( input ).setAttribute( "name", "D" );

      // Support: IE8
      // Enforce case-sensitivity of name attribute
      if ( el.querySelectorAll( "[name=d]" ).length ) {
        rbuggyQSA.push( "name" + whitespace + "*[*^$|!~]?=" );
      }

      // FF 3.5 - :enabled/:disabled and hidden elements (hidden elements are still enabled)
      // IE8 throws error here and will not see later tests
      if ( el.querySelectorAll( ":enabled" ).length !== 2 ) {
        rbuggyQSA.push( ":enabled", ":disabled" );
      }

      // Support: IE9-11+
      // IE's :disabled selector does not pick up the children of disabled fieldsets
      docElem.appendChild( el ).disabled = true;
      if ( el.querySelectorAll( ":disabled" ).length !== 2 ) {
        rbuggyQSA.push( ":enabled", ":disabled" );
      }

      // Support: Opera 10 - 11 only
      // Opera 10-11 does not throw on post-comma invalid pseudos
      el.querySelectorAll( "*,:x" );
      rbuggyQSA.push( ",.*:" );
    } );
  }

  if ( ( support.matchesSelector = rnative.test( ( matches = docElem.matches ||
    docElem.webkitMatchesSelector ||
    docElem.mozMatchesSelector ||
    docElem.oMatchesSelector ||
    docElem.msMatchesSelector ) ) ) ) {

    assert( function( el ) {

      // Check to see if it's possible to do matchesSelector
      // on a disconnected node (IE 9)
      support.disconnectedMatch = matches.call( el, "*" );

      // This should fail with an exception
      // Gecko does not error, returns false instead
      matches.call( el, "[s!='']:x" );
      rbuggyMatches.push( "!=", pseudos );
    } );
  }

  rbuggyQSA = rbuggyQSA.length && new RegExp( rbuggyQSA.join( "|" ) );
  rbuggyMatches = rbuggyMatches.length && new RegExp( rbuggyMatches.join( "|" ) );

  /* Contains
  ---------------------------------------------------------------------- */
  hasCompare = rnative.test( docElem.compareDocumentPosition );

  // Element contains another
  // Purposefully self-exclusive
  // As in, an element does not contain itself
  contains = hasCompare || rnative.test( docElem.contains ) ?
    function( a, b ) {
      var adown = a.nodeType === 9 ? a.documentElement : a,
        bup = b && b.parentNode;
      return a === bup || !!( bup && bup.nodeType === 1 && (
        adown.contains ?
          adown.contains( bup ) :
          a.compareDocumentPosition && a.compareDocumentPosition( bup ) & 16
      ) );
    } :
    function( a, b ) {
      if ( b ) {
        while ( ( b = b.parentNode ) ) {
          if ( b === a ) {
            return true;
          }
        }
      }
      return false;
    };

  /* Sorting
  ---------------------------------------------------------------------- */

  // Document order sorting
  sortOrder = hasCompare ?
  function( a, b ) {

    // Flag for duplicate removal
    if ( a === b ) {
      hasDuplicate = true;
      return 0;
    }

    // Sort on method existence if only one input has compareDocumentPosition
    var compare = !a.compareDocumentPosition - !b.compareDocumentPosition;
    if ( compare ) {
      return compare;
    }

    // Calculate position if both inputs belong to the same document
    // Support: IE 11+, Edge 17 - 18+
    // IE/Edge sometimes throw a "Permission denied" error when strict-comparing
    // two documents; shallow comparisons work.
    // eslint-disable-next-line eqeqeq
    compare = ( a.ownerDocument || a ) == ( b.ownerDocument || b ) ?
      a.compareDocumentPosition( b ) :

      // Otherwise we know they are disconnected
      1;

    // Disconnected nodes
    if ( compare & 1 ||
      ( !support.sortDetached && b.compareDocumentPosition( a ) === compare ) ) {

      // Choose the first element that is related to our preferred document
      // Support: IE 11+, Edge 17 - 18+
      // IE/Edge sometimes throw a "Permission denied" error when strict-comparing
      // two documents; shallow comparisons work.
      // eslint-disable-next-line eqeqeq
      if ( a == document || a.ownerDocument == preferredDoc &&
        contains( preferredDoc, a ) ) {
        return -1;
      }

      // Support: IE 11+, Edge 17 - 18+
      // IE/Edge sometimes throw a "Permission denied" error when strict-comparing
      // two documents; shallow comparisons work.
      // eslint-disable-next-line eqeqeq
      if ( b == document || b.ownerDocument == preferredDoc &&
        contains( preferredDoc, b ) ) {
        return 1;
      }

      // Maintain original order
      return sortInput ?
        ( indexOf( sortInput, a ) - indexOf( sortInput, b ) ) :
        0;
    }

    return compare & 4 ? -1 : 1;
  } :
  function( a, b ) {

    // Exit early if the nodes are identical
    if ( a === b ) {
      hasDuplicate = true;
      return 0;
    }

    var cur,
      i = 0,
      aup = a.parentNode,
      bup = b.parentNode,
      ap = [ a ],
      bp = [ b ];

    // Parentless nodes are either documents or disconnected
    if ( !aup || !bup ) {

      // Support: IE 11+, Edge 17 - 18+
      // IE/Edge sometimes throw a "Permission denied" error when strict-comparing
      // two documents; shallow comparisons work.
      /* eslint-disable eqeqeq */
      return a == document ? -1 :
        b == document ? 1 :
        /* eslint-enable eqeqeq */
        aup ? -1 :
        bup ? 1 :
        sortInput ?
        ( indexOf( sortInput, a ) - indexOf( sortInput, b ) ) :
        0;

    // If the nodes are siblings, we can do a quick check
    } else if ( aup === bup ) {
      return siblingCheck( a, b );
    }

    // Otherwise we need full lists of their ancestors for comparison
    cur = a;
    while ( ( cur = cur.parentNode ) ) {
      ap.unshift( cur );
    }
    cur = b;
    while ( ( cur = cur.parentNode ) ) {
      bp.unshift( cur );
    }

    // Walk down the tree looking for a discrepancy
    while ( ap[ i ] === bp[ i ] ) {
      i++;
    }

    return i ?

      // Do a sibling check if the nodes have a common ancestor
      siblingCheck( ap[ i ], bp[ i ] ) :

      // Otherwise nodes in our document sort first
      // Support: IE 11+, Edge 17 - 18+
      // IE/Edge sometimes throw a "Permission denied" error when strict-comparing
      // two documents; shallow comparisons work.
      /* eslint-disable eqeqeq */
      ap[ i ] == preferredDoc ? -1 :
      bp[ i ] == preferredDoc ? 1 :
      /* eslint-enable eqeqeq */
      0;
  };

  return document;
};

Sizzle.matches = function( expr, elements ) {
  return Sizzle( expr, null, null, elements );
};

Sizzle.matchesSelector = function( elem, expr ) {
  setDocument( elem );

  if ( support.matchesSelector && documentIsHTML &&
    !nonnativeSelectorCache[ expr + " " ] &&
    ( !rbuggyMatches || !rbuggyMatches.test( expr ) ) &&
    ( !rbuggyQSA     || !rbuggyQSA.test( expr ) ) ) {

    try {
      var ret = matches.call( elem, expr );

      // IE 9's matchesSelector returns false on disconnected nodes
      if ( ret || support.disconnectedMatch ||

        // As well, disconnected nodes are said to be in a document
        // fragment in IE 9
        elem.document && elem.document.nodeType !== 11 ) {
        return ret;
      }
    } catch ( e ) {
      nonnativeSelectorCache( expr, true );
    }
  }

  return Sizzle( expr, document, null, [ elem ] ).length > 0;
};

Sizzle.contains = function( context, elem ) {

  // Set document vars if needed
  // Support: IE 11+, Edge 17 - 18+
  // IE/Edge sometimes throw a "Permission denied" error when strict-comparing
  // two documents; shallow comparisons work.
  // eslint-disable-next-line eqeqeq
  if ( ( context.ownerDocument || context ) != document ) {
    setDocument( context );
  }
  return contains( context, elem );
};

Sizzle.attr = function( elem, name ) {

  // Set document vars if needed
  // Support: IE 11+, Edge 17 - 18+
  // IE/Edge sometimes throw a "Permission denied" error when strict-comparing
  // two documents; shallow comparisons work.
  // eslint-disable-next-line eqeqeq
  if ( ( elem.ownerDocument || elem ) != document ) {
    setDocument( elem );
  }

  var fn = Expr.attrHandle[ name.toLowerCase() ],

    // Don't get fooled by Object.prototype properties (jQuery #13807)
    val = fn && hasOwn.call( Expr.attrHandle, name.toLowerCase() ) ?
      fn( elem, name, !documentIsHTML ) :
      undefined;

  return val !== undefined ?
    val :
    support.attributes || !documentIsHTML ?
      elem.getAttribute( name ) :
      ( val = elem.getAttributeNode( name ) ) && val.specified ?
        val.value :
        null;
};

Sizzle.escape = function( sel ) {
  return ( sel + "" ).replace( rcssescape, fcssescape );
};

Sizzle.error = function( msg ) {
  throw new Error( "Syntax error, unrecognized expression: " + msg );
};

/**
 * Document sorting and removing duplicates
 * @param {ArrayLike} results
 */
Sizzle.uniqueSort = function( results ) {
  var elem,
    duplicates = [],
    j = 0,
    i = 0;

  // Unless we *know* we can detect duplicates, assume their presence
  hasDuplicate = !support.detectDuplicates;
  sortInput = !support.sortStable && results.slice( 0 );
  results.sort( sortOrder );

  if ( hasDuplicate ) {
    while ( ( elem = results[ i++ ] ) ) {
      if ( elem === results[ i ] ) {
        j = duplicates.push( i );
      }
    }
    while ( j-- ) {
      results.splice( duplicates[ j ], 1 );
    }
  }

  // Clear input after sorting to release objects
  // See https://github.com/jquery/sizzle/pull/225
  sortInput = null;

  return results;
};

/**
 * Utility function for retrieving the text value of an array of DOM nodes
 * @param {Array|Element} elem
 */
getText = Sizzle.getText = function( elem ) {
  var node,
    ret = "",
    i = 0,
    nodeType = elem.nodeType;

  if ( !nodeType ) {

    // If no nodeType, this is expected to be an array
    while ( ( node = elem[ i++ ] ) ) {

      // Do not traverse comment nodes
      ret += getText( node );
    }
  } else if ( nodeType === 1 || nodeType === 9 || nodeType === 11 ) {

    // Use textContent for elements
    // innerText usage removed for consistency of new lines (jQuery #11153)
    if ( typeof elem.textContent === "string" ) {
      return elem.textContent;
    } else {

      // Traverse its children
      for ( elem = elem.firstChild; elem; elem = elem.nextSibling ) {
        ret += getText( elem );
      }
    }
  } else if ( nodeType === 3 || nodeType === 4 ) {
    return elem.nodeValue;
  }

  // Do not include comment or processing instruction nodes

  return ret;
};

Expr = Sizzle.selectors = {

  // Can be adjusted by the user
  cacheLength: 50,

  createPseudo: markFunction,

  match: matchExpr,

  attrHandle: {},

  find: {},

  relative: {
    ">": { dir: "parentNode", first: true },
    " ": { dir: "parentNode" },
    "+": { dir: "previousSibling", first: true },
    "~": { dir: "previousSibling" }
  },

  preFilter: {
    "ATTR": function( match ) {
      match[ 1 ] = match[ 1 ].replace( runescape, funescape );

      // Move the given value to match[3] whether quoted or unquoted
      match[ 3 ] = ( match[ 3 ] || match[ 4 ] ||
        match[ 5 ] || "" ).replace( runescape, funescape );

      if ( match[ 2 ] === "~=" ) {
        match[ 3 ] = " " + match[ 3 ] + " ";
      }

      return match.slice( 0, 4 );
    },

    "CHILD": function( match ) {

      /* matches from matchExpr["CHILD"]
        1 type (only|nth|...)
        2 what (child|of-type)
        3 argument (even|odd|\d*|\d*n([+-]\d+)?|...)
        4 xn-component of xn+y argument ([+-]?\d*n|)
        5 sign of xn-component
        6 x of xn-component
        7 sign of y-component
        8 y of y-component
      */
      match[ 1 ] = match[ 1 ].toLowerCase();

      if ( match[ 1 ].slice( 0, 3 ) === "nth" ) {

        // nth-* requires argument
        if ( !match[ 3 ] ) {
          Sizzle.error( match[ 0 ] );
        }

        // numeric x and y parameters for Expr.filter.CHILD
        // remember that false/true cast respectively to 0/1
        match[ 4 ] = +( match[ 4 ] ?
          match[ 5 ] + ( match[ 6 ] || 1 ) :
          2 * ( match[ 3 ] === "even" || match[ 3 ] === "odd" ) );
        match[ 5 ] = +( ( match[ 7 ] + match[ 8 ] ) || match[ 3 ] === "odd" );

        // other types prohibit arguments
      } else if ( match[ 3 ] ) {
        Sizzle.error( match[ 0 ] );
      }

      return match;
    },

    "PSEUDO": function( match ) {
      var excess,
        unquoted = !match[ 6 ] && match[ 2 ];

      if ( matchExpr[ "CHILD" ].test( match[ 0 ] ) ) {
        return null;
      }

      // Accept quoted arguments as-is
      if ( match[ 3 ] ) {
        match[ 2 ] = match[ 4 ] || match[ 5 ] || "";

      // Strip excess characters from unquoted arguments
      } else if ( unquoted && rpseudo.test( unquoted ) &&

        // Get excess from tokenize (recursively)
        ( excess = tokenize( unquoted, true ) ) &&

        // advance to the next closing parenthesis
        ( excess = unquoted.indexOf( ")", unquoted.length - excess ) - unquoted.length ) ) {

        // excess is a negative index
        match[ 0 ] = match[ 0 ].slice( 0, excess );
        match[ 2 ] = unquoted.slice( 0, excess );
      }

      // Return only captures needed by the pseudo filter method (type and argument)
      return match.slice( 0, 3 );
    }
  },

  filter: {

    "TAG": function( nodeNameSelector ) {
      var nodeName = nodeNameSelector.replace( runescape, funescape ).toLowerCase();
      return nodeNameSelector === "*" ?
        function() {
          return true;
        } :
        function( elem ) {
          return elem.nodeName && elem.nodeName.toLowerCase() === nodeName;
        };
    },

    "CLASS": function( className ) {
      var pattern = classCache[ className + " " ];

      return pattern ||
        ( pattern = new RegExp( "(^|" + whitespace +
          ")" + className + "(" + whitespace + "|$)" ) ) && classCache(
            className, function( elem ) {
              return pattern.test(
                typeof elem.className === "string" && elem.className ||
                typeof elem.getAttribute !== "undefined" &&
                  elem.getAttribute( "class" ) ||
                ""
              );
        } );
    },

    "ATTR": function( name, operator, check ) {
      return function( elem ) {
        var result = Sizzle.attr( elem, name );

        if ( result == null ) {
          return operator === "!=";
        }
        if ( !operator ) {
          return true;
        }

        result += "";

        /* eslint-disable max-len */

        return operator === "=" ? result === check :
          operator === "!=" ? result !== check :
          operator === "^=" ? check && result.indexOf( check ) === 0 :
          operator === "*=" ? check && result.indexOf( check ) > -1 :
          operator === "$=" ? check && result.slice( -check.length ) === check :
          operator === "~=" ? ( " " + result.replace( rwhitespace, " " ) + " " ).indexOf( check ) > -1 :
          operator === "|=" ? result === check || result.slice( 0, check.length + 1 ) === check + "-" :
          false;
        /* eslint-enable max-len */

      };
    },

    "CHILD": function( type, what, _argument, first, last ) {
      var simple = type.slice( 0, 3 ) !== "nth",
        forward = type.slice( -4 ) !== "last",
        ofType = what === "of-type";

      return first === 1 && last === 0 ?

        // Shortcut for :nth-*(n)
        function( elem ) {
          return !!elem.parentNode;
        } :

        function( elem, _context, xml ) {
          var cache, uniqueCache, outerCache, node, nodeIndex, start,
            dir = simple !== forward ? "nextSibling" : "previousSibling",
            parent = elem.parentNode,
            name = ofType && elem.nodeName.toLowerCase(),
            useCache = !xml && !ofType,
            diff = false;

          if ( parent ) {

            // :(first|last|only)-(child|of-type)
            if ( simple ) {
              while ( dir ) {
                node = elem;
                while ( ( node = node[ dir ] ) ) {
                  if ( ofType ?
                    node.nodeName.toLowerCase() === name :
                    node.nodeType === 1 ) {

                    return false;
                  }
                }

                // Reverse direction for :only-* (if we haven't yet done so)
                start = dir = type === "only" && !start && "nextSibling";
              }
              return true;
            }

            start = [ forward ? parent.firstChild : parent.lastChild ];

            // non-xml :nth-child(...) stores cache data on `parent`
            if ( forward && useCache ) {

              // Seek `elem` from a previously-cached index

              // ...in a gzip-friendly way
              node = parent;
              outerCache = node[ expando ] || ( node[ expando ] = {} );

              // Support: IE <9 only
              // Defend against cloned attroperties (jQuery gh-1709)
              uniqueCache = outerCache[ node.uniqueID ] ||
                ( outerCache[ node.uniqueID ] = {} );

              cache = uniqueCache[ type ] || [];
              nodeIndex = cache[ 0 ] === dirruns && cache[ 1 ];
              diff = nodeIndex && cache[ 2 ];
              node = nodeIndex && parent.childNodes[ nodeIndex ];

              while ( ( node = ++nodeIndex && node && node[ dir ] ||

                // Fallback to seeking `elem` from the start
                ( diff = nodeIndex = 0 ) || start.pop() ) ) {

                // When found, cache indexes on `parent` and break
                if ( node.nodeType === 1 && ++diff && node === elem ) {
                  uniqueCache[ type ] = [ dirruns, nodeIndex, diff ];
                  break;
                }
              }

            } else {

              // Use previously-cached element index if available
              if ( useCache ) {

                // ...in a gzip-friendly way
                node = elem;
                outerCache = node[ expando ] || ( node[ expando ] = {} );

                // Support: IE <9 only
                // Defend against cloned attroperties (jQuery gh-1709)
                uniqueCache = outerCache[ node.uniqueID ] ||
                  ( outerCache[ node.uniqueID ] = {} );

                cache = uniqueCache[ type ] || [];
                nodeIndex = cache[ 0 ] === dirruns && cache[ 1 ];
                diff = nodeIndex;
              }

              // xml :nth-child(...)
              // or :nth-last-child(...) or :nth(-last)?-of-type(...)
              if ( diff === false ) {

                // Use the same loop as above to seek `elem` from the start
                while ( ( node = ++nodeIndex && node && node[ dir ] ||
                  ( diff = nodeIndex = 0 ) || start.pop() ) ) {

                  if ( ( ofType ?
                    node.nodeName.toLowerCase() === name :
                    node.nodeType === 1 ) &&
                    ++diff ) {

                    // Cache the index of each encountered element
                    if ( useCache ) {
                      outerCache = node[ expando ] ||
                        ( node[ expando ] = {} );

                      // Support: IE <9 only
                      // Defend against cloned attroperties (jQuery gh-1709)
                      uniqueCache = outerCache[ node.uniqueID ] ||
                        ( outerCache[ node.uniqueID ] = {} );

                      uniqueCache[ type ] = [ dirruns, diff ];
                    }

                    if ( node === elem ) {
                      break;
                    }
                  }
                }
              }
            }

            // Incorporate the offset, then check against cycle size
            diff -= last;
            return diff === first || ( diff % first === 0 && diff / first >= 0 );
          }
        };
    },

    "PSEUDO": function( pseudo, argument ) {

      // pseudo-class names are case-insensitive
      // http://www.w3.org/TR/selectors/#pseudo-classes
      // Prioritize by case sensitivity in case custom pseudos are added with uppercase letters
      // Remember that setFilters inherits from pseudos
      var args,
        fn = Expr.pseudos[ pseudo ] || Expr.setFilters[ pseudo.toLowerCase() ] ||
          Sizzle.error( "unsupported pseudo: " + pseudo );

      // The user may use createPseudo to indicate that
      // arguments are needed to create the filter function
      // just as Sizzle does
      if ( fn[ expando ] ) {
        return fn( argument );
      }

      // But maintain support for old signatures
      if ( fn.length > 1 ) {
        args = [ pseudo, pseudo, "", argument ];
        return Expr.setFilters.hasOwnProperty( pseudo.toLowerCase() ) ?
          markFunction( function( seed, matches ) {
            var idx,
              matched = fn( seed, argument ),
              i = matched.length;
            while ( i-- ) {
              idx = indexOf( seed, matched[ i ] );
              seed[ idx ] = !( matches[ idx ] = matched[ i ] );
            }
          } ) :
          function( elem ) {
            return fn( elem, 0, args );
          };
      }

      return fn;
    }
  },

  pseudos: {

    // Potentially complex pseudos
    "not": markFunction( function( selector ) {

      // Trim the selector passed to compile
      // to avoid treating leading and trailing
      // spaces as combinators
      var input = [],
        results = [],
        matcher = compile( selector.replace( rtrim, "$1" ) );

      return matcher[ expando ] ?
        markFunction( function( seed, matches, _context, xml ) {
          var elem,
            unmatched = matcher( seed, null, xml, [] ),
            i = seed.length;

          // Match elements unmatched by `matcher`
          while ( i-- ) {
            if ( ( elem = unmatched[ i ] ) ) {
              seed[ i ] = !( matches[ i ] = elem );
            }
          }
        } ) :
        function( elem, _context, xml ) {
          input[ 0 ] = elem;
          matcher( input, null, xml, results );

          // Don't keep the element (issue #299)
          input[ 0 ] = null;
          return !results.pop();
        };
    } ),

    "has": markFunction( function( selector ) {
      return function( elem ) {
        return Sizzle( selector, elem ).length > 0;
      };
    } ),

    "contains": markFunction( function( text ) {
      text = text.replace( runescape, funescape );
      return function( elem ) {
        return ( elem.textContent || getText( elem ) ).indexOf( text ) > -1;
      };
    } ),

    // "Whether an element is represented by a :lang() selector
    // is based solely on the element's language value
    // being equal to the identifier C,
    // or beginning with the identifier C immediately followed by "-".
    // The matching of C against the element's language value is performed case-insensitively.
    // The identifier C does not have to be a valid language name."
    // http://www.w3.org/TR/selectors/#lang-pseudo
    "lang": markFunction( function( lang ) {

      // lang value must be a valid identifier
      if ( !ridentifier.test( lang || "" ) ) {
        Sizzle.error( "unsupported lang: " + lang );
      }
      lang = lang.replace( runescape, funescape ).toLowerCase();
      return function( elem ) {
        var elemLang;
        do {
          if ( ( elemLang = documentIsHTML ?
            elem.lang :
            elem.getAttribute( "xml:lang" ) || elem.getAttribute( "lang" ) ) ) {

            elemLang = elemLang.toLowerCase();
            return elemLang === lang || elemLang.indexOf( lang + "-" ) === 0;
          }
        } while ( ( elem = elem.parentNode ) && elem.nodeType === 1 );
        return false;
      };
    } ),

    // Miscellaneous
    "target": function( elem ) {
      var hash = window.location && window.location.hash;
      return hash && hash.slice( 1 ) === elem.id;
    },

    "root": function( elem ) {
      return elem === docElem;
    },

    "focus": function( elem ) {
      return elem === document.activeElement &&
        ( !document.hasFocus || document.hasFocus() ) &&
        !!( elem.type || elem.href || ~elem.tabIndex );
    },

    // Boolean properties
    "enabled": createDisabledPseudo( false ),
    "disabled": createDisabledPseudo( true ),

    "checked": function( elem ) {

      // In CSS3, :checked should return both checked and selected elements
      // http://www.w3.org/TR/2011/REC-css3-selectors-20110929/#checked
      var nodeName = elem.nodeName.toLowerCase();
      return ( nodeName === "input" && !!elem.checked ) ||
        ( nodeName === "option" && !!elem.selected );
    },

    "selected": function( elem ) {

      // Accessing this property makes selected-by-default
      // options in Safari work properly
      if ( elem.parentNode ) {
        // eslint-disable-next-line no-unused-expressions
        elem.parentNode.selectedIndex;
      }

      return elem.selected === true;
    },

    // Contents
    "empty": function( elem ) {

      // http://www.w3.org/TR/selectors/#empty-pseudo
      // :empty is negated by element (1) or content nodes (text: 3; cdata: 4; entity ref: 5),
      //   but not by others (comment: 8; processing instruction: 7; etc.)
      // nodeType < 6 works because attributes (2) do not appear as children
      for ( elem = elem.firstChild; elem; elem = elem.nextSibling ) {
        if ( elem.nodeType < 6 ) {
          return false;
        }
      }
      return true;
    },

    "parent": function( elem ) {
      return !Expr.pseudos[ "empty" ]( elem );
    },

    // Element/input types
    "header": function( elem ) {
      return rheader.test( elem.nodeName );
    },

    "input": function( elem ) {
      return rinputs.test( elem.nodeName );
    },

    "button": function( elem ) {
      var name = elem.nodeName.toLowerCase();
      return name === "input" && elem.type === "button" || name === "button";
    },

    "text": function( elem ) {
      var attr;
      return elem.nodeName.toLowerCase() === "input" &&
        elem.type === "text" &&

        // Support: IE<8
        // New HTML5 attribute values (e.g., "search") appear with elem.type === "text"
        ( ( attr = elem.getAttribute( "type" ) ) == null ||
          attr.toLowerCase() === "text" );
    },

    // Position-in-collection
    "first": createPositionalPseudo( function() {
      return [ 0 ];
    } ),

    "last": createPositionalPseudo( function( _matchIndexes, length ) {
      return [ length - 1 ];
    } ),

    "eq": createPositionalPseudo( function( _matchIndexes, length, argument ) {
      return [ argument < 0 ? argument + length : argument ];
    } ),

    "even": createPositionalPseudo( function( matchIndexes, length ) {
      var i = 0;
      for ( ; i < length; i += 2 ) {
        matchIndexes.push( i );
      }
      return matchIndexes;
    } ),

    "odd": createPositionalPseudo( function( matchIndexes, length ) {
      var i = 1;
      for ( ; i < length; i += 2 ) {
        matchIndexes.push( i );
      }
      return matchIndexes;
    } ),

    "lt": createPositionalPseudo( function( matchIndexes, length, argument ) {
      var i = argument < 0 ?
        argument + length :
        argument > length ?
          length :
          argument;
      for ( ; --i >= 0; ) {
        matchIndexes.push( i );
      }
      return matchIndexes;
    } ),

    "gt": createPositionalPseudo( function( matchIndexes, length, argument ) {
      var i = argument < 0 ? argument + length : argument;
      for ( ; ++i < length; ) {
        matchIndexes.push( i );
      }
      return matchIndexes;
    } )
  }
};

Expr.pseudos[ "nth" ] = Expr.pseudos[ "eq" ];

// Add button/input type pseudos
for ( i in { radio: true, checkbox: true, file: true, password: true, image: true } ) {
  Expr.pseudos[ i ] = createInputPseudo( i );
}
for ( i in { submit: true, reset: true } ) {
  Expr.pseudos[ i ] = createButtonPseudo( i );
}

// Easy API for creating new setFilters
function setFilters() {}
setFilters.prototype = Expr.filters = Expr.pseudos;
Expr.setFilters = new setFilters();

tokenize = Sizzle.tokenize = function( selector, parseOnly ) {
  var matched, match, tokens, type,
    soFar, groups, preFilters,
    cached = tokenCache[ selector + " " ];

  if ( cached ) {
    return parseOnly ? 0 : cached.slice( 0 );
  }

  soFar = selector;
  groups = [];
  preFilters = Expr.preFilter;

  while ( soFar ) {

    // Comma and first run
    if ( !matched || ( match = rcomma.exec( soFar ) ) ) {
      if ( match ) {

        // Don't consume trailing commas as valid
        soFar = soFar.slice( match[ 0 ].length ) || soFar;
      }
      groups.push( ( tokens = [] ) );
    }

    matched = false;

    // Combinators
    if ( ( match = rcombinators.exec( soFar ) ) ) {
      matched = match.shift();
      tokens.push( {
        value: matched,

        // Cast descendant combinators to space
        type: match[ 0 ].replace( rtrim, " " )
      } );
      soFar = soFar.slice( matched.length );
    }

    // Filters
    for ( type in Expr.filter ) {
      if ( ( match = matchExpr[ type ].exec( soFar ) ) && ( !preFilters[ type ] ||
        ( match = preFilters[ type ]( match ) ) ) ) {
        matched = match.shift();
        tokens.push( {
          value: matched,
          type: type,
          matches: match
        } );
        soFar = soFar.slice( matched.length );
      }
    }

    if ( !matched ) {
      break;
    }
  }

  // Return the length of the invalid excess
  // if we're just parsing
  // Otherwise, throw an error or return tokens
  return parseOnly ?
    soFar.length :
    soFar ?
      Sizzle.error( selector ) :

      // Cache the tokens
      tokenCache( selector, groups ).slice( 0 );
};

function toSelector( tokens ) {
  var i = 0,
    len = tokens.length,
    selector = "";
  for ( ; i < len; i++ ) {
    selector += tokens[ i ].value;
  }
  return selector;
}

function addCombinator( matcher, combinator, base ) {
  var dir = combinator.dir,
    skip = combinator.next,
    key = skip || dir,
    checkNonElements = base && key === "parentNode",
    doneName = done++;

  return combinator.first ?

    // Check against closest ancestor/preceding element
    function( elem, context, xml ) {
      while ( ( elem = elem[ dir ] ) ) {
        if ( elem.nodeType === 1 || checkNonElements ) {
          return matcher( elem, context, xml );
        }
      }
      return false;
    } :

    // Check against all ancestor/preceding elements
    function( elem, context, xml ) {
      var oldCache, uniqueCache, outerCache,
        newCache = [ dirruns, doneName ];

      // We can't set arbitrary data on XML nodes, so they don't benefit from combinator caching
      if ( xml ) {
        while ( ( elem = elem[ dir ] ) ) {
          if ( elem.nodeType === 1 || checkNonElements ) {
            if ( matcher( elem, context, xml ) ) {
              return true;
            }
          }
        }
      } else {
        while ( ( elem = elem[ dir ] ) ) {
          if ( elem.nodeType === 1 || checkNonElements ) {
            outerCache = elem[ expando ] || ( elem[ expando ] = {} );

            // Support: IE <9 only
            // Defend against cloned attroperties (jQuery gh-1709)
            uniqueCache = outerCache[ elem.uniqueID ] ||
              ( outerCache[ elem.uniqueID ] = {} );

            if ( skip && skip === elem.nodeName.toLowerCase() ) {
              elem = elem[ dir ] || elem;
            } else if ( ( oldCache = uniqueCache[ key ] ) &&
              oldCache[ 0 ] === dirruns && oldCache[ 1 ] === doneName ) {

              // Assign to newCache so results back-propagate to previous elements
              return ( newCache[ 2 ] = oldCache[ 2 ] );
            } else {

              // Reuse newcache so results back-propagate to previous elements
              uniqueCache[ key ] = newCache;

              // A match means we're done; a fail means we have to keep checking
              if ( ( newCache[ 2 ] = matcher( elem, context, xml ) ) ) {
                return true;
              }
            }
          }
        }
      }
      return false;
    };
}

function elementMatcher( matchers ) {
  return matchers.length > 1 ?
    function( elem, context, xml ) {
      var i = matchers.length;
      while ( i-- ) {
        if ( !matchers[ i ]( elem, context, xml ) ) {
          return false;
        }
      }
      return true;
    } :
    matchers[ 0 ];
}

function multipleContexts( selector, contexts, results ) {
  var i = 0,
    len = contexts.length;
  for ( ; i < len; i++ ) {
    Sizzle( selector, contexts[ i ], results );
  }
  return results;
}

function condense( unmatched, map, filter, context, xml ) {
  var elem,
    newUnmatched = [],
    i = 0,
    len = unmatched.length,
    mapped = map != null;

  for ( ; i < len; i++ ) {
    if ( ( elem = unmatched[ i ] ) ) {
      if ( !filter || filter( elem, context, xml ) ) {
        newUnmatched.push( elem );
        if ( mapped ) {
          map.push( i );
        }
      }
    }
  }

  return newUnmatched;
}

function setMatcher( preFilter, selector, matcher, postFilter, postFinder, postSelector ) {
  if ( postFilter && !postFilter[ expando ] ) {
    postFilter = setMatcher( postFilter );
  }
  if ( postFinder && !postFinder[ expando ] ) {
    postFinder = setMatcher( postFinder, postSelector );
  }
  return markFunction( function( seed, results, context, xml ) {
    var temp, i, elem,
      preMap = [],
      postMap = [],
      preexisting = results.length,

      // Get initial elements from seed or context
      elems = seed || multipleContexts(
        selector || "*",
        context.nodeType ? [ context ] : context,
        []
      ),

      // Prefilter to get matcher input, preserving a map for seed-results synchronization
      matcherIn = preFilter && ( seed || !selector ) ?
        condense( elems, preMap, preFilter, context, xml ) :
        elems,

      matcherOut = matcher ?

        // If we have a postFinder, or filtered seed, or non-seed postFilter or preexisting results,
        postFinder || ( seed ? preFilter : preexisting || postFilter ) ?

          // ...intermediate processing is necessary
          [] :

          // ...otherwise use results directly
          results :
        matcherIn;

    // Find primary matches
    if ( matcher ) {
      matcher( matcherIn, matcherOut, context, xml );
    }

    // Apply postFilter
    if ( postFilter ) {
      temp = condense( matcherOut, postMap );
      postFilter( temp, [], context, xml );

      // Un-match failing elements by moving them back to matcherIn
      i = temp.length;
      while ( i-- ) {
        if ( ( elem = temp[ i ] ) ) {
          matcherOut[ postMap[ i ] ] = !( matcherIn[ postMap[ i ] ] = elem );
        }
      }
    }

    if ( seed ) {
      if ( postFinder || preFilter ) {
        if ( postFinder ) {

          // Get the final matcherOut by condensing this intermediate into postFinder contexts
          temp = [];
          i = matcherOut.length;
          while ( i-- ) {
            if ( ( elem = matcherOut[ i ] ) ) {

              // Restore matcherIn since elem is not yet a final match
              temp.push( ( matcherIn[ i ] = elem ) );
            }
          }
          postFinder( null, ( matcherOut = [] ), temp, xml );
        }

        // Move matched elements from seed to results to keep them synchronized
        i = matcherOut.length;
        while ( i-- ) {
          if ( ( elem = matcherOut[ i ] ) &&
            ( temp = postFinder ? indexOf( seed, elem ) : preMap[ i ] ) > -1 ) {

            seed[ temp ] = !( results[ temp ] = elem );
          }
        }
      }

    // Add elements to results, through postFinder if defined
    } else {
      matcherOut = condense(
        matcherOut === results ?
          matcherOut.splice( preexisting, matcherOut.length ) :
          matcherOut
      );
      if ( postFinder ) {
        postFinder( null, results, matcherOut, xml );
      } else {
        push.apply( results, matcherOut );
      }
    }
  } );
}

function matcherFromTokens( tokens ) {
  var checkContext, matcher, j,
    len = tokens.length,
    leadingRelative = Expr.relative[ tokens[ 0 ].type ],
    implicitRelative = leadingRelative || Expr.relative[ " " ],
    i = leadingRelative ? 1 : 0,

    // The foundational matcher ensures that elements are reachable from top-level context(s)
    matchContext = addCombinator( function( elem ) {
      return elem === checkContext;
    }, implicitRelative, true ),
    matchAnyContext = addCombinator( function( elem ) {
      return indexOf( checkContext, elem ) > -1;
    }, implicitRelative, true ),
    matchers = [ function( elem, context, xml ) {
      var ret = ( !leadingRelative && ( xml || context !== outermostContext ) ) || (
        ( checkContext = context ).nodeType ?
          matchContext( elem, context, xml ) :
          matchAnyContext( elem, context, xml ) );

      // Avoid hanging onto element (issue #299)
      checkContext = null;
      return ret;
    } ];

  for ( ; i < len; i++ ) {
    if ( ( matcher = Expr.relative[ tokens[ i ].type ] ) ) {
      matchers = [ addCombinator( elementMatcher( matchers ), matcher ) ];
    } else {
      matcher = Expr.filter[ tokens[ i ].type ].apply( null, tokens[ i ].matches );

      // Return special upon seeing a positional matcher
      if ( matcher[ expando ] ) {

        // Find the next relative operator (if any) for proper handling
        j = ++i;
        for ( ; j < len; j++ ) {
          if ( Expr.relative[ tokens[ j ].type ] ) {
            break;
          }
        }
        return setMatcher(
          i > 1 && elementMatcher( matchers ),
          i > 1 && toSelector(

          // If the preceding token was a descendant combinator, insert an implicit any-element `*`
          tokens
            .slice( 0, i - 1 )
            .concat( { value: tokens[ i - 2 ].type === " " ? "*" : "" } )
          ).replace( rtrim, "$1" ),
          matcher,
          i < j && matcherFromTokens( tokens.slice( i, j ) ),
          j < len && matcherFromTokens( ( tokens = tokens.slice( j ) ) ),
          j < len && toSelector( tokens )
        );
      }
      matchers.push( matcher );
    }
  }

  return elementMatcher( matchers );
}

function matcherFromGroupMatchers( elementMatchers, setMatchers ) {
  var bySet = setMatchers.length > 0,
    byElement = elementMatchers.length > 0,
    superMatcher = function( seed, context, xml, results, outermost ) {
      var elem, j, matcher,
        matchedCount = 0,
        i = "0",
        unmatched = seed && [],
        setMatched = [],
        contextBackup = outermostContext,

        // We must always have either seed elements or outermost context
        elems = seed || byElement && Expr.find[ "TAG" ]( "*", outermost ),

        // Use integer dirruns iff this is the outermost matcher
        dirrunsUnique = ( dirruns += contextBackup == null ? 1 : Math.random() || 0.1 ),
        len = elems.length;

      if ( outermost ) {

        // Support: IE 11+, Edge 17 - 18+
        // IE/Edge sometimes throw a "Permission denied" error when strict-comparing
        // two documents; shallow comparisons work.
        // eslint-disable-next-line eqeqeq
        outermostContext = context == document || context || outermost;
      }

      // Add elements passing elementMatchers directly to results
      // Support: IE<9, Safari
      // Tolerate NodeList properties (IE: "length"; Safari: <number>) matching elements by id
      for ( ; i !== len && ( elem = elems[ i ] ) != null; i++ ) {
        if ( byElement && elem ) {
          j = 0;

          // Support: IE 11+, Edge 17 - 18+
          // IE/Edge sometimes throw a "Permission denied" error when strict-comparing
          // two documents; shallow comparisons work.
          // eslint-disable-next-line eqeqeq
          if ( !context && elem.ownerDocument != document ) {
            setDocument( elem );
            xml = !documentIsHTML;
          }
          while ( ( matcher = elementMatchers[ j++ ] ) ) {
            if ( matcher( elem, context || document, xml ) ) {
              results.push( elem );
              break;
            }
          }
          if ( outermost ) {
            dirruns = dirrunsUnique;
          }
        }

        // Track unmatched elements for set filters
        if ( bySet ) {

          // They will have gone through all possible matchers
          if ( ( elem = !matcher && elem ) ) {
            matchedCount--;
          }

          // Lengthen the array for every element, matched or not
          if ( seed ) {
            unmatched.push( elem );
          }
        }
      }

      // `i` is now the count of elements visited above, and adding it to `matchedCount`
      // makes the latter nonnegative.
      matchedCount += i;

      // Apply set filters to unmatched elements
      // NOTE: This can be skipped if there are no unmatched elements (i.e., `matchedCount`
      // equals `i`), unless we didn't visit _any_ elements in the above loop because we have
      // no element matchers and no seed.
      // Incrementing an initially-string "0" `i` allows `i` to remain a string only in that
      // case, which will result in a "00" `matchedCount` that differs from `i` but is also
      // numerically zero.
      if ( bySet && i !== matchedCount ) {
        j = 0;
        while ( ( matcher = setMatchers[ j++ ] ) ) {
          matcher( unmatched, setMatched, context, xml );
        }

        if ( seed ) {

          // Reintegrate element matches to eliminate the need for sorting
          if ( matchedCount > 0 ) {
            while ( i-- ) {
              if ( !( unmatched[ i ] || setMatched[ i ] ) ) {
                setMatched[ i ] = pop.call( results );
              }
            }
          }

          // Discard index placeholder values to get only actual matches
          setMatched = condense( setMatched );
        }

        // Add matches to results
        push.apply( results, setMatched );

        // Seedless set matches succeeding multiple successful matchers stipulate sorting
        if ( outermost && !seed && setMatched.length > 0 &&
          ( matchedCount + setMatchers.length ) > 1 ) {

          Sizzle.uniqueSort( results );
        }
      }

      // Override manipulation of globals by nested matchers
      if ( outermost ) {
        dirruns = dirrunsUnique;
        outermostContext = contextBackup;
      }

      return unmatched;
    };

  return bySet ?
    markFunction( superMatcher ) :
    superMatcher;
}

compile = Sizzle.compile = function( selector, match /* Internal Use Only */ ) {
  var i,
    setMatchers = [],
    elementMatchers = [],
    cached = compilerCache[ selector + " " ];

  if ( !cached ) {

    // Generate a function of recursive functions that can be used to check each element
    if ( !match ) {
      match = tokenize( selector );
    }
    i = match.length;
    while ( i-- ) {
      cached = matcherFromTokens( match[ i ] );
      if ( cached[ expando ] ) {
        setMatchers.push( cached );
      } else {
        elementMatchers.push( cached );
      }
    }

    // Cache the compiled function
    cached = compilerCache(
      selector,
      matcherFromGroupMatchers( elementMatchers, setMatchers )
    );

    // Save selector and tokenization
    cached.selector = selector;
  }
  return cached;
};

/**
 * A low-level selection function that works with Sizzle's compiled
 *  selector functions
 * @param {String|Function} selector A selector or a pre-compiled
 *  selector function built with Sizzle.compile
 * @param {Element} context
 * @param {Array} [results]
 * @param {Array} [seed] A set of elements to match against
 */
select = Sizzle.select = function( selector, context, results, seed ) {
  var i, tokens, token, type, find,
    compiled = typeof selector === "function" && selector,
    match = !seed && tokenize( ( selector = compiled.selector || selector ) );

  results = results || [];

  // Try to minimize operations if there is only one selector in the list and no seed
  // (the latter of which guarantees us context)
  if ( match.length === 1 ) {

    // Reduce context if the leading compound selector is an ID
    tokens = match[ 0 ] = match[ 0 ].slice( 0 );
    if ( tokens.length > 2 && ( token = tokens[ 0 ] ).type === "ID" &&
      context.nodeType === 9 && documentIsHTML && Expr.relative[ tokens[ 1 ].type ] ) {

      context = ( Expr.find[ "ID" ]( token.matches[ 0 ]
        .replace( runescape, funescape ), context ) || [] )[ 0 ];
      if ( !context ) {
        return results;

      // Precompiled matchers will still verify ancestry, so step up a level
      } else if ( compiled ) {
        context = context.parentNode;
      }

      selector = selector.slice( tokens.shift().value.length );
    }

    // Fetch a seed set for right-to-left matching
    i = matchExpr[ "needsContext" ].test( selector ) ? 0 : tokens.length;
    while ( i-- ) {
      token = tokens[ i ];

      // Abort if we hit a combinator
      if ( Expr.relative[ ( type = token.type ) ] ) {
        break;
      }
      if ( ( find = Expr.find[ type ] ) ) {

        // Search, expanding context for leading sibling combinators
        if ( ( seed = find(
          token.matches[ 0 ].replace( runescape, funescape ),
          rsibling.test( tokens[ 0 ].type ) && testContext( context.parentNode ) ||
            context
        ) ) ) {

          // If seed is empty or no tokens remain, we can return early
          tokens.splice( i, 1 );
          selector = seed.length && toSelector( tokens );
          if ( !selector ) {
            push.apply( results, seed );
            return results;
          }

          break;
        }
      }
    }
  }

  // Compile and execute a filtering function if one is not provided
  // Provide `match` to avoid retokenization if we modified the selector above
  ( compiled || compile( selector, match ) )(
    seed,
    context,
    !documentIsHTML,
    results,
    !context || rsibling.test( selector ) && testContext( context.parentNode ) || context
  );
  return results;
};

// One-time assignments

// Sort stability
support.sortStable = expando.split( "" ).sort( sortOrder ).join( "" ) === expando;

// Support: Chrome 14-35+
// Always assume duplicates if they aren't passed to the comparison function
support.detectDuplicates = !!hasDuplicate;

// Initialize against the default document
setDocument();

// Support: Webkit<537.32 - Safari 6.0.3/Chrome 25 (fixed in Chrome 27)
// Detached nodes confoundingly follow *each other*
support.sortDetached = assert( function( el ) {

  // Should return 1, but returns 4 (following)
  return el.compareDocumentPosition( document.createElement( "fieldset" ) ) & 1;
} );

// Support: IE<8
// Prevent attribute/property "interpolation"
// https://msdn.microsoft.com/en-us/library/ms536429%28VS.85%29.aspx
if ( !assert( function( el ) {
  el.innerHTML = "<a href='#'></a>";
  return el.firstChild.getAttribute( "href" ) === "#";
} ) ) {
  addHandle( "type|href|height|width", function( elem, name, isXML ) {
    if ( !isXML ) {
      return elem.getAttribute( name, name.toLowerCase() === "type" ? 1 : 2 );
    }
  } );
}

// Support: IE<9
// Use defaultValue in place of getAttribute("value")
if ( !support.attributes || !assert( function( el ) {
  el.innerHTML = "<input/>";
  el.firstChild.setAttribute( "value", "" );
  return el.firstChild.getAttribute( "value" ) === "";
} ) ) {
  addHandle( "value", function( elem, _name, isXML ) {
    if ( !isXML && elem.nodeName.toLowerCase() === "input" ) {
      return elem.defaultValue;
    }
  } );
}

// Support: IE<9
// Use getAttributeNode to fetch booleans when getAttribute lies
if ( !assert( function( el ) {
  return el.getAttribute( "disabled" ) == null;
} ) ) {
  addHandle( booleans, function( elem, name, isXML ) {
    var val;
    if ( !isXML ) {
      return elem[ name ] === true ? name.toLowerCase() :
        ( val = elem.getAttributeNode( name ) ) && val.specified ?
          val.value :
          null;
    }
  } );
}

return Sizzle;
} )( window );


jQuery.find = Sizzle;
jQuery.expr = Sizzle.selectors;

// Deprecated
jQuery.expr[ ":" ] = jQuery.expr.pseudos;
jQuery.uniqueSort = jQuery.unique = Sizzle.uniqueSort;
jQuery.text = Sizzle.getText;
jQuery.isXMLDoc = Sizzle.isXML;
jQuery.contains = Sizzle.contains;
jQuery.escapeSelector = Sizzle.escape;


var dir = function( elem, dir, until ) {
  var matched = [],
    truncate = until !== undefined;

  while ( ( elem = elem[ dir ] ) && elem.nodeType !== 9 ) {
    if ( elem.nodeType === 1 ) {
      if ( truncate && jQuery( elem ).is( until ) ) {
        break;
      }
      matched.push( elem );
    }
  }
  return matched;
};


var siblings = function( n, elem ) {
  var matched = [];

  for ( ; n; n = n.nextSibling ) {
    if ( n.nodeType === 1 && n !== elem ) {
      matched.push( n );
    }
  }

  return matched;
};


var rneedsContext = jQuery.expr.match.needsContext;


function nodeName( elem, name ) {

  return elem.nodeName && elem.nodeName.toLowerCase() === name.toLowerCase();
}
var rsingleTag = ( /^<([a-z][^\/\0>:\x20\t\r\n\f]*)[\x20\t\r\n\f]*\/?>(?:<\/\1>|)$/i );


// Implement the identical functionality for filter and not
function winnow( elements, qualifier, not ) {
  if ( isFunction( qualifier ) ) {
    return jQuery.grep( elements, function( elem, i ) {
      return !!qualifier.call( elem, i, elem ) !== not;
    } );
  }

  // Single element
  if ( qualifier.nodeType ) {
    return jQuery.grep( elements, function( elem ) {
      return ( elem === qualifier ) !== not;
    } );
  }

  // Arraylike of elements (jQuery, arguments, Array)
  if ( typeof qualifier !== "string" ) {
    return jQuery.grep( elements, function( elem ) {
      return ( indexOf.call( qualifier, elem ) > -1 ) !== not;
    } );
  }

  // Filtered directly for both simple and complex selectors
  return jQuery.filter( qualifier, elements, not );
}

jQuery.filter = function( expr, elems, not ) {
  var elem = elems[ 0 ];

  if ( not ) {
    expr = ":not(" + expr + ")";
  }

  if ( elems.length === 1 && elem.nodeType === 1 ) {
    return jQuery.find.matchesSelector( elem, expr ) ? [ elem ] : [];
  }

  return jQuery.find.matches( expr, jQuery.grep( elems, function( elem ) {
    return elem.nodeType === 1;
  } ) );
};

jQuery.fn.extend( {
  find: function( selector ) {
    var i, ret,
      len = this.length,
      self = this;

    if ( typeof selector !== "string" ) {
      return this.pushStack( jQuery( selector ).filter( function() {
        for ( i = 0; i < len; i++ ) {
          if ( jQuery.contains( self[ i ], this ) ) {
            return true;
          }
        }
      } ) );
    }

    ret = this.pushStack( [] );

    for ( i = 0; i < len; i++ ) {
      jQuery.find( selector, self[ i ], ret );
    }

    return len > 1 ? jQuery.uniqueSort( ret ) : ret;
  },
  filter: function( selector ) {
    return this.pushStack( winnow( this, selector || [], false ) );
  },
  not: function( selector ) {
    return this.pushStack( winnow( this, selector || [], true ) );
  },
  is: function( selector ) {
    return !!winnow(
      this,

      // If this is a positional/relative selector, check membership in the returned set
      // so $("p:first").is("p:last") won't return true for a doc with two "p".
      typeof selector === "string" && rneedsContext.test( selector ) ?
        jQuery( selector ) :
        selector || [],
      false
    ).length;
  }
} );


// Initialize a jQuery object


// A central reference to the root jQuery(document)
var rootjQuery,

  // A simple way to check for HTML strings
  // Prioritize #id over <tag> to avoid XSS via location.hash (#9521)
  // Strict HTML recognition (#11290: must start with <)
  // Shortcut simple #id case for speed
  rquickExpr = /^(?:\s*(<[\w\W]+>)[^>]*|#([\w-]+))$/,

  init = jQuery.fn.init = function( selector, context, root ) {
    var match, elem;

    // HANDLE: $(""), $(null), $(undefined), $(false)
    if ( !selector ) {
      return this;
    }

    // Method init() accepts an alternate rootjQuery
    // so migrate can support jQuery.sub (gh-2101)
    root = root || rootjQuery;

    // Handle HTML strings
    if ( typeof selector === "string" ) {
      if ( selector[ 0 ] === "<" &&
        selector[ selector.length - 1 ] === ">" &&
        selector.length >= 3 ) {

        // Assume that strings that start and end with <> are HTML and skip the regex check
        match = [ null, selector, null ];

      } else {
        match = rquickExpr.exec( selector );
      }

      // Match html or make sure no context is specified for #id
      if ( match && ( match[ 1 ] || !context ) ) {

        // HANDLE: $(html) -> $(array)
        if ( match[ 1 ] ) {
          context = context instanceof jQuery ? context[ 0 ] : context;

          // Option to run scripts is true for back-compat
          // Intentionally let the error be thrown if parseHTML is not present
          jQuery.merge( this, jQuery.parseHTML(
            match[ 1 ],
            context && context.nodeType ? context.ownerDocument || context : document,
            true
          ) );

          // HANDLE: $(html, props)
          if ( rsingleTag.test( match[ 1 ] ) && jQuery.isPlainObject( context ) ) {
            for ( match in context ) {

              // Properties of context are called as methods if possible
              if ( isFunction( this[ match ] ) ) {
                this[ match ]( context[ match ] );

              // ...and otherwise set as attributes
              } else {
                this.attr( match, context[ match ] );
              }
            }
          }

          return this;

        // HANDLE: $(#id)
        } else {
          elem = document.getElementById( match[ 2 ] );

          if ( elem ) {

            // Inject the element directly into the jQuery object
            this[ 0 ] = elem;
            this.length = 1;
          }
          return this;
        }

      // HANDLE: $(expr, $(...))
      } else if ( !context || context.jquery ) {
        return ( context || root ).find( selector );

      // HANDLE: $(expr, context)
      // (which is just equivalent to: $(context).find(expr)
      } else {
        return this.constructor( context ).find( selector );
      }

    // HANDLE: $(DOMElement)
    } else if ( selector.nodeType ) {
      this[ 0 ] = selector;
      this.length = 1;
      return this;

    // HANDLE: $(function)
    // Shortcut for document ready
    } else if ( isFunction( selector ) ) {
      return root.ready !== undefined ?
        root.ready( selector ) :

        // Execute immediately if ready is not present
        selector( jQuery );
    }

    return jQuery.makeArray( selector, this );
  };

// Give the init function the jQuery prototype for later instantiation
init.prototype = jQuery.fn;

// Initialize central reference
rootjQuery = jQuery( document );


var rparentsprev = /^(?:parents|prev(?:Until|All))/,

  // Methods guaranteed to produce a unique set when starting from a unique set
  guaranteedUnique = {
    children: true,
    contents: true,
    next: true,
    prev: true
  };

jQuery.fn.extend( {
  has: function( target ) {
    var targets = jQuery( target, this ),
      l = targets.length;

    return this.filter( function() {
      var i = 0;
      for ( ; i < l; i++ ) {
        if ( jQuery.contains( this, targets[ i ] ) ) {
          return true;
        }
      }
    } );
  },

  closest: function( selectors, context ) {
    var cur,
      i = 0,
      l = this.length,
      matched = [],
      targets = typeof selectors !== "string" && jQuery( selectors );

    // Positional selectors never match, since there's no _selection_ context
    if ( !rneedsContext.test( selectors ) ) {
      for ( ; i < l; i++ ) {
        for ( cur = this[ i ]; cur && cur !== context; cur = cur.parentNode ) {

          // Always skip document fragments
          if ( cur.nodeType < 11 && ( targets ?
            targets.index( cur ) > -1 :

            // Don't pass non-elements to Sizzle
            cur.nodeType === 1 &&
              jQuery.find.matchesSelector( cur, selectors ) ) ) {

            matched.push( cur );
            break;
          }
        }
      }
    }

    return this.pushStack( matched.length > 1 ? jQuery.uniqueSort( matched ) : matched );
  },

  // Determine the position of an element within the set
  index: function( elem ) {

    // No argument, return index in parent
    if ( !elem ) {
      return ( this[ 0 ] && this[ 0 ].parentNode ) ? this.first().prevAll().length : -1;
    }

    // Index in selector
    if ( typeof elem === "string" ) {
      return indexOf.call( jQuery( elem ), this[ 0 ] );
    }

    // Locate the position of the desired element
    return indexOf.call( this,

      // If it receives a jQuery object, the first element is used
      elem.jquery ? elem[ 0 ] : elem
    );
  },

  add: function( selector, context ) {
    return this.pushStack(
      jQuery.uniqueSort(
        jQuery.merge( this.get(), jQuery( selector, context ) )
      )
    );
  },

  addBack: function( selector ) {
    return this.add( selector == null ?
      this.prevObject : this.prevObject.filter( selector )
    );
  }
} );

function sibling( cur, dir ) {
  while ( ( cur = cur[ dir ] ) && cur.nodeType !== 1 ) {}
  return cur;
}

jQuery.each( {
  parent: function( elem ) {
    var parent = elem.parentNode;
    return parent && parent.nodeType !== 11 ? parent : null;
  },
  parents: function( elem ) {
    return dir( elem, "parentNode" );
  },
  parentsUntil: function( elem, _i, until ) {
    return dir( elem, "parentNode", until );
  },
  next: function( elem ) {
    return sibling( elem, "nextSibling" );
  },
  prev: function( elem ) {
    return sibling( elem, "previousSibling" );
  },
  nextAll: function( elem ) {
    return dir( elem, "nextSibling" );
  },
  prevAll: function( elem ) {
    return dir( elem, "previousSibling" );
  },
  nextUntil: function( elem, _i, until ) {
    return dir( elem, "nextSibling", until );
  },
  prevUntil: function( elem, _i, until ) {
    return dir( elem, "previousSibling", until );
  },
  siblings: function( elem ) {
    return siblings( ( elem.parentNode || {} ).firstChild, elem );
  },
  children: function( elem ) {
    return siblings( elem.firstChild );
  },
  contents: function( elem ) {
    if ( elem.contentDocument != null &&

      // Support: IE 11+
      // <object> elements with no `data` attribute has an object
      // `contentDocument` with a `null` prototype.
      getProto( elem.contentDocument ) ) {

      return elem.contentDocument;
    }

    // Support: IE 9 - 11 only, iOS 7 only, Android Browser <=4.3 only
    // Treat the template element as a regular one in browsers that
    // don't support it.
    if ( nodeName( elem, "template" ) ) {
      elem = elem.content || elem;
    }

    return jQuery.merge( [], elem.childNodes );
  }
}, function( name, fn ) {
  jQuery.fn[ name ] = function( until, selector ) {
    var matched = jQuery.map( this, fn, until );

    if ( name.slice( -5 ) !== "Until" ) {
      selector = until;
    }

    if ( selector && typeof selector === "string" ) {
      matched = jQuery.filter( selector, matched );
    }

    if ( this.length > 1 ) {

      // Remove duplicates
      if ( !guaranteedUnique[ name ] ) {
        jQuery.uniqueSort( matched );
      }

      // Reverse order for parents* and prev-derivatives
      if ( rparentsprev.test( name ) ) {
        matched.reverse();
      }
    }

    return this.pushStack( matched );
  };
} );
var rnothtmlwhite = ( /[^\x20\t\r\n\f]+/g );


// Convert String-formatted options into Object-formatted ones
function createOptions( options ) {
  var object = {};
  jQuery.each( options.match( rnothtmlwhite ) || [], function( _, flag ) {
    object[ flag ] = true;
  } );
  return object;
}

/*
 * Create a callback list using the following parameters:
 *
 *  options: an optional list of space-separated options that will change how
 *      the callback list behaves or a more traditional option object
 *
 * By default a callback list will act like an event callback list and can be
 * "fired" multiple times.
 *
 * Possible options:
 *
 *  once:      will ensure the callback list can only be fired once (like a Deferred)
 *
 *  memory:      will keep track of previous values and will call any callback added
 *          after the list has been fired right away with the latest "memorized"
 *          values (like a Deferred)
 *
 *  unique:      will ensure a callback can only be added once (no duplicate in the list)
 *
 *  stopOnFalse:  interrupt callings when a callback returns false
 *
 */
jQuery.Callbacks = function( options ) {

  // Convert options from String-formatted to Object-formatted if needed
  // (we check in cache first)
  options = typeof options === "string" ?
    createOptions( options ) :
    jQuery.extend( {}, options );

  var // Flag to know if list is currently firing
    firing,

    // Last fire value for non-forgettable lists
    memory,

    // Flag to know if list was already fired
    fired,

    // Flag to prevent firing
    locked,

    // Actual callback list
    list = [],

    // Queue of execution data for repeatable lists
    queue = [],

    // Index of currently firing callback (modified by add/remove as needed)
    firingIndex = -1,

    // Fire callbacks
    fire = function() {

      // Enforce single-firing
      locked = locked || options.once;

      // Execute callbacks for all pending executions,
      // respecting firingIndex overrides and runtime changes
      fired = firing = true;
      for ( ; queue.length; firingIndex = -1 ) {
        memory = queue.shift();
        while ( ++firingIndex < list.length ) {

          // Run callback and check for early termination
          if ( list[ firingIndex ].apply( memory[ 0 ], memory[ 1 ] ) === false &&
            options.stopOnFalse ) {

            // Jump to end and forget the data so .add doesn't re-fire
            firingIndex = list.length;
            memory = false;
          }
        }
      }

      // Forget the data if we're done with it
      if ( !options.memory ) {
        memory = false;
      }

      firing = false;

      // Clean up if we're done firing for good
      if ( locked ) {

        // Keep an empty list if we have data for future add calls
        if ( memory ) {
          list = [];

        // Otherwise, this object is spent
        } else {
          list = "";
        }
      }
    },

    // Actual Callbacks object
    self = {

      // Add a callback or a collection of callbacks to the list
      add: function() {
        if ( list ) {

          // If we have memory from a past run, we should fire after adding
          if ( memory && !firing ) {
            firingIndex = list.length - 1;
            queue.push( memory );
          }

          ( function add( args ) {
            jQuery.each( args, function( _, arg ) {
              if ( isFunction( arg ) ) {
                if ( !options.unique || !self.has( arg ) ) {
                  list.push( arg );
                }
              } else if ( arg && arg.length && toType( arg ) !== "string" ) {

                // Inspect recursively
                add( arg );
              }
            } );
          } )( arguments );

          if ( memory && !firing ) {
            fire();
          }
        }
        return this;
      },

      // Remove a callback from the list
      remove: function() {
        jQuery.each( arguments, function( _, arg ) {
          var index;
          while ( ( index = jQuery.inArray( arg, list, index ) ) > -1 ) {
            list.splice( index, 1 );

            // Handle firing indexes
            if ( index <= firingIndex ) {
              firingIndex--;
            }
          }
        } );
        return this;
      },

      // Check if a given callback is in the list.
      // If no argument is given, return whether or not list has callbacks attached.
      has: function( fn ) {
        return fn ?
          jQuery.inArray( fn, list ) > -1 :
          list.length > 0;
      },

      // Remove all callbacks from the list
      empty: function() {
        if ( list ) {
          list = [];
        }
        return this;
      },

      // Disable .fire and .add
      // Abort any current/pending executions
      // Clear all callbacks and values
      disable: function() {
        locked = queue = [];
        list = memory = "";
        return this;
      },
      disabled: function() {
        return !list;
      },

      // Disable .fire
      // Also disable .add unless we have memory (since it would have no effect)
      // Abort any pending executions
      lock: function() {
        locked = queue = [];
        if ( !memory && !firing ) {
          list = memory = "";
        }
        return this;
      },
      locked: function() {
        return !!locked;
      },

      // Call all callbacks with the given context and arguments
      fireWith: function( context, args ) {
        if ( !locked ) {
          args = args || [];
          args = [ context, args.slice ? args.slice() : args ];
          queue.push( args );
          if ( !firing ) {
            fire();
          }
        }
        return this;
      },

      // Call all the callbacks with the given arguments
      fire: function() {
        self.fireWith( this, arguments );
        return this;
      },

      // To know if the callbacks have already been called at least once
      fired: function() {
        return !!fired;
      }
    };

  return self;
};


function Identity( v ) {
  return v;
}
function Thrower( ex ) {
  throw ex;
}

function adoptValue( value, resolve, reject, noValue ) {
  var method;

  try {

    // Check for promise aspect first to privilege synchronous behavior
    if ( value && isFunction( ( method = value.promise ) ) ) {
      method.call( value ).done( resolve ).fail( reject );

    // Other thenables
    } else if ( value && isFunction( ( method = value.then ) ) ) {
      method.call( value, resolve, reject );

    // Other non-thenables
    } else {

      // Control `resolve` arguments by letting Array#slice cast boolean `noValue` to integer:
      // * false: [ value ].slice( 0 ) => resolve( value )
      // * true: [ value ].slice( 1 ) => resolve()
      resolve.apply( undefined, [ value ].slice( noValue ) );
    }

  // For Promises/A+, convert exceptions into rejections
  // Since jQuery.when doesn't unwrap thenables, we can skip the extra checks appearing in
  // Deferred#then to conditionally suppress rejection.
  } catch ( value ) {

    // Support: Android 4.0 only
    // Strict mode functions invoked without .call/.apply get global-object context
    reject.apply( undefined, [ value ] );
  }
}

jQuery.extend( {

  Deferred: function( func ) {
    var tuples = [

        // action, add listener, callbacks,
        // ... .then handlers, argument index, [final state]
        [ "notify", "progress", jQuery.Callbacks( "memory" ),
          jQuery.Callbacks( "memory" ), 2 ],
        [ "resolve", "done", jQuery.Callbacks( "once memory" ),
          jQuery.Callbacks( "once memory" ), 0, "resolved" ],
        [ "reject", "fail", jQuery.Callbacks( "once memory" ),
          jQuery.Callbacks( "once memory" ), 1, "rejected" ]
      ],
      state = "pending",
      promise = {
        state: function() {
          return state;
        },
        always: function() {
          deferred.done( arguments ).fail( arguments );
          return this;
        },
        "catch": function( fn ) {
          return promise.then( null, fn );
        },

        // Keep pipe for back-compat
        pipe: function( /* fnDone, fnFail, fnProgress */ ) {
          var fns = arguments;

          return jQuery.Deferred( function( newDefer ) {
            jQuery.each( tuples, function( _i, tuple ) {

              // Map tuples (progress, done, fail) to arguments (done, fail, progress)
              var fn = isFunction( fns[ tuple[ 4 ] ] ) && fns[ tuple[ 4 ] ];

              // deferred.progress(function() { bind to newDefer or newDefer.notify })
              // deferred.done(function() { bind to newDefer or newDefer.resolve })
              // deferred.fail(function() { bind to newDefer or newDefer.reject })
              deferred[ tuple[ 1 ] ]( function() {
                var returned = fn && fn.apply( this, arguments );
                if ( returned && isFunction( returned.promise ) ) {
                  returned.promise()
                    .progress( newDefer.notify )
                    .done( newDefer.resolve )
                    .fail( newDefer.reject );
                } else {
                  newDefer[ tuple[ 0 ] + "With" ](
                    this,
                    fn ? [ returned ] : arguments
                  );
                }
              } );
            } );
            fns = null;
          } ).promise();
        },
        then: function( onFulfilled, onRejected, onProgress ) {
          var maxDepth = 0;
          function resolve( depth, deferred, handler, special ) {
            return function() {
              var that = this,
                args = arguments,
                mightThrow = function() {
                  var returned, then;

                  // Support: Promises/A+ section 2.3.3.3.3
                  // https://promisesaplus.com/#point-59
                  // Ignore double-resolution attempts
                  if ( depth < maxDepth ) {
                    return;
                  }

                  returned = handler.apply( that, args );

                  // Support: Promises/A+ section 2.3.1
                  // https://promisesaplus.com/#point-48
                  if ( returned === deferred.promise() ) {
                    throw new TypeError( "Thenable self-resolution" );
                  }

                  // Support: Promises/A+ sections 2.3.3.1, 3.5
                  // https://promisesaplus.com/#point-54
                  // https://promisesaplus.com/#point-75
                  // Retrieve `then` only once
                  then = returned &&

                    // Support: Promises/A+ section 2.3.4
                    // https://promisesaplus.com/#point-64
                    // Only check objects and functions for thenability
                    ( typeof returned === "object" ||
                      typeof returned === "function" ) &&
                    returned.then;

                  // Handle a returned thenable
                  if ( isFunction( then ) ) {

                    // Special processors (notify) just wait for resolution
                    if ( special ) {
                      then.call(
                        returned,
                        resolve( maxDepth, deferred, Identity, special ),
                        resolve( maxDepth, deferred, Thrower, special )
                      );

                    // Normal processors (resolve) also hook into progress
                    } else {

                      // ...and disregard older resolution values
                      maxDepth++;

                      then.call(
                        returned,
                        resolve( maxDepth, deferred, Identity, special ),
                        resolve( maxDepth, deferred, Thrower, special ),
                        resolve( maxDepth, deferred, Identity,
                          deferred.notifyWith )
                      );
                    }

                  // Handle all other returned values
                  } else {

                    // Only substitute handlers pass on context
                    // and multiple values (non-spec behavior)
                    if ( handler !== Identity ) {
                      that = undefined;
                      args = [ returned ];
                    }

                    // Process the value(s)
                    // Default process is resolve
                    ( special || deferred.resolveWith )( that, args );
                  }
                },

                // Only normal processors (resolve) catch and reject exceptions
                process = special ?
                  mightThrow :
                  function() {
                    try {
                      mightThrow();
                    } catch ( e ) {

                      if ( jQuery.Deferred.exceptionHook ) {
                        jQuery.Deferred.exceptionHook( e,
                          process.stackTrace );
                      }

                      // Support: Promises/A+ section 2.3.3.3.4.1
                      // https://promisesaplus.com/#point-61
                      // Ignore post-resolution exceptions
                      if ( depth + 1 >= maxDepth ) {

                        // Only substitute handlers pass on context
                        // and multiple values (non-spec behavior)
                        if ( handler !== Thrower ) {
                          that = undefined;
                          args = [ e ];
                        }

                        deferred.rejectWith( that, args );
                      }
                    }
                  };

              // Support: Promises/A+ section 2.3.3.3.1
              // https://promisesaplus.com/#point-57
              // Re-resolve promises immediately to dodge false rejection from
              // subsequent errors
              if ( depth ) {
                process();
              } else {

                // Call an optional hook to record the stack, in case of exception
                // since it's otherwise lost when execution goes async
                if ( jQuery.Deferred.getStackHook ) {
                  process.stackTrace = jQuery.Deferred.getStackHook();
                }
                window.setTimeout( process );
              }
            };
          }

          return jQuery.Deferred( function( newDefer ) {

            // progress_handlers.add( ... )
            tuples[ 0 ][ 3 ].add(
              resolve(
                0,
                newDefer,
                isFunction( onProgress ) ?
                  onProgress :
                  Identity,
                newDefer.notifyWith
              )
            );

            // fulfilled_handlers.add( ... )
            tuples[ 1 ][ 3 ].add(
              resolve(
                0,
                newDefer,
                isFunction( onFulfilled ) ?
                  onFulfilled :
                  Identity
              )
            );

            // rejected_handlers.add( ... )
            tuples[ 2 ][ 3 ].add(
              resolve(
                0,
                newDefer,
                isFunction( onRejected ) ?
                  onRejected :
                  Thrower
              )
            );
          } ).promise();
        },

        // Get a promise for this deferred
        // If obj is provided, the promise aspect is added to the object
        promise: function( obj ) {
          return obj != null ? jQuery.extend( obj, promise ) : promise;
        }
      },
      deferred = {};

    // Add list-specific methods
    jQuery.each( tuples, function( i, tuple ) {
      var list = tuple[ 2 ],
        stateString = tuple[ 5 ];

      // promise.progress = list.add
      // promise.done = list.add
      // promise.fail = list.add
      promise[ tuple[ 1 ] ] = list.add;

      // Handle state
      if ( stateString ) {
        list.add(
          function() {

            // state = "resolved" (i.e., fulfilled)
            // state = "rejected"
            state = stateString;
          },

          // rejected_callbacks.disable
          // fulfilled_callbacks.disable
          tuples[ 3 - i ][ 2 ].disable,

          // rejected_handlers.disable
          // fulfilled_handlers.disable
          tuples[ 3 - i ][ 3 ].disable,

          // progress_callbacks.lock
          tuples[ 0 ][ 2 ].lock,

          // progress_handlers.lock
          tuples[ 0 ][ 3 ].lock
        );
      }

      // progress_handlers.fire
      // fulfilled_handlers.fire
      // rejected_handlers.fire
      list.add( tuple[ 3 ].fire );

      // deferred.notify = function() { deferred.notifyWith(...) }
      // deferred.resolve = function() { deferred.resolveWith(...) }
      // deferred.reject = function() { deferred.rejectWith(...) }
      deferred[ tuple[ 0 ] ] = function() {
        deferred[ tuple[ 0 ] + "With" ]( this === deferred ? undefined : this, arguments );
        return this;
      };

      // deferred.notifyWith = list.fireWith
      // deferred.resolveWith = list.fireWith
      // deferred.rejectWith = list.fireWith
      deferred[ tuple[ 0 ] + "With" ] = list.fireWith;
    } );

    // Make the deferred a promise
    promise.promise( deferred );

    // Call given func if any
    if ( func ) {
      func.call( deferred, deferred );
    }

    // All done!
    return deferred;
  },

  // Deferred helper
  when: function( singleValue ) {
    var

      // count of uncompleted subordinates
      remaining = arguments.length,

      // count of unprocessed arguments
      i = remaining,

      // subordinate fulfillment data
      resolveContexts = Array( i ),
      resolveValues = slice.call( arguments ),

      // the primary Deferred
      primary = jQuery.Deferred(),

      // subordinate callback factory
      updateFunc = function( i ) {
        return function( value ) {
          resolveContexts[ i ] = this;
          resolveValues[ i ] = arguments.length > 1 ? slice.call( arguments ) : value;
          if ( !( --remaining ) ) {
            primary.resolveWith( resolveContexts, resolveValues );
          }
        };
      };

    // Single- and empty arguments are adopted like Promise.resolve
    if ( remaining <= 1 ) {
      adoptValue( singleValue, primary.done( updateFunc( i ) ).resolve, primary.reject,
        !remaining );

      // Use .then() to unwrap secondary thenables (cf. gh-3000)
      if ( primary.state() === "pending" ||
        isFunction( resolveValues[ i ] && resolveValues[ i ].then ) ) {

        return primary.then();
      }
    }

    // Multiple arguments are aggregated like Promise.all array elements
    while ( i-- ) {
      adoptValue( resolveValues[ i ], updateFunc( i ), primary.reject );
    }

    return primary.promise();
  }
} );


// These usually indicate a programmer mistake during development,
// warn about them ASAP rather than swallowing them by default.
var rerrorNames = /^(Eval|Internal|Range|Reference|Syntax|Type|URI)Error$/;

jQuery.Deferred.exceptionHook = function( error, stack ) {

  // Support: IE 8 - 9 only
  // Console exists when dev tools are open, which can happen at any time
  if ( window.console && window.console.warn && error && rerrorNames.test( error.name ) ) {
    window.console.warn( "jQuery.Deferred exception: " + error.message, error.stack, stack );
  }
};


jQuery.readyException = function( error ) {
  window.setTimeout( function() {
    throw error;
  } );
};


// The deferred used on DOM ready
var readyList = jQuery.Deferred();

jQuery.fn.ready = function( fn ) {

  readyList
    .then( fn )

    // Wrap jQuery.readyException in a function so that the lookup
    // happens at the time of error handling instead of callback
    // registration.
    .catch( function( error ) {
      jQuery.readyException( error );
    } );

  return this;
};

jQuery.extend( {

  // Is the DOM ready to be used? Set to true once it occurs.
  isReady: false,

  // A counter to track how many items to wait for before
  // the ready event fires. See #6781
  readyWait: 1,

  // Handle when the DOM is ready
  ready: function( wait ) {

    // Abort if there are pending holds or we're already ready
    if ( wait === true ? --jQuery.readyWait : jQuery.isReady ) {
      return;
    }

    // Remember that the DOM is ready
    jQuery.isReady = true;

    // If a normal DOM Ready event fired, decrement, and wait if need be
    if ( wait !== true && --jQuery.readyWait > 0 ) {
      return;
    }

    // If there are functions bound, to execute
    readyList.resolveWith( document, [ jQuery ] );
  }
} );

jQuery.ready.then = readyList.then;

// The ready event handler and self cleanup method
function completed() {
  document.removeEventListener( "DOMContentLoaded", completed );
  window.removeEventListener( "load", completed );
  jQuery.ready();
}

// Catch cases where $(document).ready() is called
// after the browser event has already occurred.
// Support: IE <=9 - 10 only
// Older IE sometimes signals "interactive" too soon
if ( document.readyState === "complete" ||
  ( document.readyState !== "loading" && !document.documentElement.doScroll ) ) {

  // Handle it asynchronously to allow scripts the opportunity to delay ready
  window.setTimeout( jQuery.ready );
} else {

  // Use the handy event callback
  document.addEventListener( "DOMContentLoaded", completed );

  // A fallback to window.onload, that will always work
  window.addEventListener( "load", completed );
}


// Multifunctional method to get and set values of a collection
// The value/s can optionally be executed if it's a function
var access = function( elems, fn, key, value, chainable, emptyGet, raw ) {
  var i = 0,
    len = elems.length,
    bulk = key == null;

  // Sets many values
  if ( toType( key ) === "object" ) {
    chainable = true;
    for ( i in key ) {
      access( elems, fn, i, key[ i ], true, emptyGet, raw );
    }

  // Sets one value
  } else if ( value !== undefined ) {
    chainable = true;

    if ( !isFunction( value ) ) {
      raw = true;
    }

    if ( bulk ) {

      // Bulk operations run against the entire set
      if ( raw ) {
        fn.call( elems, value );
        fn = null;

      // ...except when executing function values
      } else {
        bulk = fn;
        fn = function( elem, _key, value ) {
          return bulk.call( jQuery( elem ), value );
        };
      }
    }

    if ( fn ) {
      for ( ; i < len; i++ ) {
        fn(
          elems[ i ], key, raw ?
            value :
            value.call( elems[ i ], i, fn( elems[ i ], key ) )
        );
      }
    }
  }

  if ( chainable ) {
    return elems;
  }

  // Gets
  if ( bulk ) {
    return fn.call( elems );
  }

  return len ? fn( elems[ 0 ], key ) : emptyGet;
};


// Matches dashed string for camelizing
var rmsPrefix = /^-ms-/,
  rdashAlpha = /-([a-z])/g;

// Used by camelCase as callback to replace()
function fcamelCase( _all, letter ) {
  return letter.toUpperCase();
}

// Convert dashed to camelCase; used by the css and data modules
// Support: IE <=9 - 11, Edge 12 - 15
// Microsoft forgot to hump their vendor prefix (#9572)
function camelCase( string ) {
  return string.replace( rmsPrefix, "ms-" ).replace( rdashAlpha, fcamelCase );
}
var acceptData = function( owner ) {

  // Accepts only:
  //  - Node
  //    - Node.ELEMENT_NODE
  //    - Node.DOCUMENT_NODE
  //  - Object
  //    - Any
  return owner.nodeType === 1 || owner.nodeType === 9 || !( +owner.nodeType );
};


function Data() {
  this.expando = jQuery.expando + Data.uid++;
}

Data.uid = 1;

Data.prototype = {

  cache: function( owner ) {

    // Check if the owner object already has a cache
    var value = owner[ this.expando ];

    // If not, create one
    if ( !value ) {
      value = {};

      // We can accept data for non-element nodes in modern browsers,
      // but we should not, see #8335.
      // Always return an empty object.
      if ( acceptData( owner ) ) {

        // If it is a node unlikely to be stringify-ed or looped over
        // use plain assignment
        if ( owner.nodeType ) {
          owner[ this.expando ] = value;

        // Otherwise secure it in a non-enumerable property
        // configurable must be true to allow the property to be
        // deleted when data is removed
        } else {
          Object.defineProperty( owner, this.expando, {
            value: value,
            configurable: true
          } );
        }
      }
    }

    return value;
  },
  set: function( owner, data, value ) {
    var prop,
      cache = this.cache( owner );

    // Handle: [ owner, key, value ] args
    // Always use camelCase key (gh-2257)
    if ( typeof data === "string" ) {
      cache[ camelCase( data ) ] = value;

    // Handle: [ owner, { properties } ] args
    } else {

      // Copy the properties one-by-one to the cache object
      for ( prop in data ) {
        cache[ camelCase( prop ) ] = data[ prop ];
      }
    }
    return cache;
  },
  get: function( owner, key ) {
    return key === undefined ?
      this.cache( owner ) :

      // Always use camelCase key (gh-2257)
      owner[ this.expando ] && owner[ this.expando ][ camelCase( key ) ];
  },
  access: function( owner, key, value ) {

    // In cases where either:
    //
    //   1. No key was specified
    //   2. A string key was specified, but no value provided
    //
    // Take the "read" path and allow the get method to determine
    // which value to return, respectively either:
    //
    //   1. The entire cache object
    //   2. The data stored at the key
    //
    if ( key === undefined ||
        ( ( key && typeof key === "string" ) && value === undefined ) ) {

      return this.get( owner, key );
    }

    // When the key is not a string, or both a key and value
    // are specified, set or extend (existing objects) with either:
    //
    //   1. An object of properties
    //   2. A key and value
    //
    this.set( owner, key, value );

    // Since the "set" path can have two possible entry points
    // return the expected data based on which path was taken[*]
    return value !== undefined ? value : key;
  },
  remove: function( owner, key ) {
    var i,
      cache = owner[ this.expando ];

    if ( cache === undefined ) {
      return;
    }

    if ( key !== undefined ) {

      // Support array or space separated string of keys
      if ( Array.isArray( key ) ) {

        // If key is an array of keys...
        // We always set camelCase keys, so remove that.
        key = key.map( camelCase );
      } else {
        key = camelCase( key );

        // If a key with the spaces exists, use it.
        // Otherwise, create an array by matching non-whitespace
        key = key in cache ?
          [ key ] :
          ( key.match( rnothtmlwhite ) || [] );
      }

      i = key.length;

      while ( i-- ) {
        delete cache[ key[ i ] ];
      }
    }

    // Remove the expando if there's no more data
    if ( key === undefined || jQuery.isEmptyObject( cache ) ) {

      // Support: Chrome <=35 - 45
      // Webkit & Blink performance suffers when deleting properties
      // from DOM nodes, so set to undefined instead
      // https://bugs.chromium.org/p/chromium/issues/detail?id=378607 (bug restricted)
      if ( owner.nodeType ) {
        owner[ this.expando ] = undefined;
      } else {
        delete owner[ this.expando ];
      }
    }
  },
  hasData: function( owner ) {
    var cache = owner[ this.expando ];
    return cache !== undefined && !jQuery.isEmptyObject( cache );
  }
};
var dataPriv = new Data();

var dataUser = new Data();


//  Implementation Summary
//
//  1. Enforce API surface and semantic compatibility with 1.9.x branch
//  2. Improve the module's maintainability by reducing the storage
//    paths to a single mechanism.
//  3. Use the same single mechanism to support "private" and "user" data.
//  4. _Never_ expose "private" data to user code (TODO: Drop _data, _removeData)
//  5. Avoid exposing implementation details on user objects (eg. expando properties)
//  6. Provide a clear path for implementation upgrade to WeakMap in 2014

var rbrace = /^(?:\{[\w\W]*\}|\[[\w\W]*\])$/,
  rmultiDash = /[A-Z]/g;

function getData( data ) {
  if ( data === "true" ) {
    return true;
  }

  if ( data === "false" ) {
    return false;
  }

  if ( data === "null" ) {
    return null;
  }

  // Only convert to a number if it doesn't change the string
  if ( data === +data + "" ) {
    return +data;
  }

  if ( rbrace.test( data ) ) {
    return JSON.parse( data );
  }

  return data;
}

function dataAttr( elem, key, data ) {
  var name;

  // If nothing was found internally, try to fetch any
  // data from the HTML5 data-* attribute
  if ( data === undefined && elem.nodeType === 1 ) {
    name = "data-" + key.replace( rmultiDash, "-$&" ).toLowerCase();
    data = elem.getAttribute( name );

    if ( typeof data === "string" ) {
      try {
        data = getData( data );
      } catch ( e ) {}

      // Make sure we set the data so it isn't changed later
      dataUser.set( elem, key, data );
    } else {
      data = undefined;
    }
  }
  return data;
}

jQuery.extend( {
  hasData: function( elem ) {
    return dataUser.hasData( elem ) || dataPriv.hasData( elem );
  },

  data: function( elem, name, data ) {
    return dataUser.access( elem, name, data );
  },

  removeData: function( elem, name ) {
    dataUser.remove( elem, name );
  },

  // TODO: Now that all calls to _data and _removeData have been replaced
  // with direct calls to dataPriv methods, these can be deprecated.
  _data: function( elem, name, data ) {
    return dataPriv.access( elem, name, data );
  },

  _removeData: function( elem, name ) {
    dataPriv.remove( elem, name );
  }
} );

jQuery.fn.extend( {
  data: function( key, value ) {
    var i, name, data,
      elem = this[ 0 ],
      attrs = elem && elem.attributes;

    // Gets all values
    if ( key === undefined ) {
      if ( this.length ) {
        data = dataUser.get( elem );

        if ( elem.nodeType === 1 && !dataPriv.get( elem, "hasDataAttrs" ) ) {
          i = attrs.length;
          while ( i-- ) {

            // Support: IE 11 only
            // The attrs elements can be null (#14894)
            if ( attrs[ i ] ) {
              name = attrs[ i ].name;
              if ( name.indexOf( "data-" ) === 0 ) {
                name = camelCase( name.slice( 5 ) );
                dataAttr( elem, name, data[ name ] );
              }
            }
          }
          dataPriv.set( elem, "hasDataAttrs", true );
        }
      }

      return data;
    }

    // Sets multiple values
    if ( typeof key === "object" ) {
      return this.each( function() {
        dataUser.set( this, key );
      } );
    }

    return access( this, function( value ) {
      var data;

      // The calling jQuery object (element matches) is not empty
      // (and therefore has an element appears at this[ 0 ]) and the
      // `value` parameter was not undefined. An empty jQuery object
      // will result in `undefined` for elem = this[ 0 ] which will
      // throw an exception if an attempt to read a data cache is made.
      if ( elem && value === undefined ) {

        // Attempt to get data from the cache
        // The key will always be camelCased in Data
        data = dataUser.get( elem, key );
        if ( data !== undefined ) {
          return data;
        }

        // Attempt to "discover" the data in
        // HTML5 custom data-* attrs
        data = dataAttr( elem, key );
        if ( data !== undefined ) {
          return data;
        }

        // We tried really hard, but the data doesn't exist.
        return;
      }

      // Set the data...
      this.each( function() {

        // We always store the camelCased key
        dataUser.set( this, key, value );
      } );
    }, null, value, arguments.length > 1, null, true );
  },

  removeData: function( key ) {
    return this.each( function() {
      dataUser.remove( this, key );
    } );
  }
} );


jQuery.extend( {
  queue: function( elem, type, data ) {
    var queue;

    if ( elem ) {
      type = ( type || "fx" ) + "queue";
      queue = dataPriv.get( elem, type );

      // Speed up dequeue by getting out quickly if this is just a lookup
      if ( data ) {
        if ( !queue || Array.isArray( data ) ) {
          queue = dataPriv.access( elem, type, jQuery.makeArray( data ) );
        } else {
          queue.push( data );
        }
      }
      return queue || [];
    }
  },

  dequeue: function( elem, type ) {
    type = type || "fx";

    var queue = jQuery.queue( elem, type ),
      startLength = queue.length,
      fn = queue.shift(),
      hooks = jQuery._queueHooks( elem, type ),
      next = function() {
        jQuery.dequeue( elem, type );
      };

    // If the fx queue is dequeued, always remove the progress sentinel
    if ( fn === "inprogress" ) {
      fn = queue.shift();
      startLength--;
    }

    if ( fn ) {

      // Add a progress sentinel to prevent the fx queue from being
      // automatically dequeued
      if ( type === "fx" ) {
        queue.unshift( "inprogress" );
      }

      // Clear up the last queue stop function
      delete hooks.stop;
      fn.call( elem, next, hooks );
    }

    if ( !startLength && hooks ) {
      hooks.empty.fire();
    }
  },

  // Not public - generate a queueHooks object, or return the current one
  _queueHooks: function( elem, type ) {
    var key = type + "queueHooks";
    return dataPriv.get( elem, key ) || dataPriv.access( elem, key, {
      empty: jQuery.Callbacks( "once memory" ).add( function() {
        dataPriv.remove( elem, [ type + "queue", key ] );
      } )
    } );
  }
} );

jQuery.fn.extend( {
  queue: function( type, data ) {
    var setter = 2;

    if ( typeof type !== "string" ) {
      data = type;
      type = "fx";
      setter--;
    }

    if ( arguments.length < setter ) {
      return jQuery.queue( this[ 0 ], type );
    }

    return data === undefined ?
      this :
      this.each( function() {
        var queue = jQuery.queue( this, type, data );

        // Ensure a hooks for this queue
        jQuery._queueHooks( this, type );

        if ( type === "fx" && queue[ 0 ] !== "inprogress" ) {
          jQuery.dequeue( this, type );
        }
      } );
  },
  dequeue: function( type ) {
    return this.each( function() {
      jQuery.dequeue( this, type );
    } );
  },
  clearQueue: function( type ) {
    return this.queue( type || "fx", [] );
  },

  // Get a promise resolved when queues of a certain type
  // are emptied (fx is the type by default)
  promise: function( type, obj ) {
    var tmp,
      count = 1,
      defer = jQuery.Deferred(),
      elements = this,
      i = this.length,
      resolve = function() {
        if ( !( --count ) ) {
          defer.resolveWith( elements, [ elements ] );
        }
      };

    if ( typeof type !== "string" ) {
      obj = type;
      type = undefined;
    }
    type = type || "fx";

    while ( i-- ) {
      tmp = dataPriv.get( elements[ i ], type + "queueHooks" );
      if ( tmp && tmp.empty ) {
        count++;
        tmp.empty.add( resolve );
      }
    }
    resolve();
    return defer.promise( obj );
  }
} );
var pnum = ( /[+-]?(?:\d*\.|)\d+(?:[eE][+-]?\d+|)/ ).source;

var rcssNum = new RegExp( "^(?:([+-])=|)(" + pnum + ")([a-z%]*)$", "i" );


var cssExpand = [ "Top", "Right", "Bottom", "Left" ];

var documentElement = document.documentElement;


  var isAttached = function( elem ) {
      return jQuery.contains( elem.ownerDocument, elem );
    },
    composed = { composed: true };

  // Support: IE 9 - 11+, Edge 12 - 18+, iOS 10.0 - 10.2 only
  // Check attachment across shadow DOM boundaries when possible (gh-3504)
  // Support: iOS 10.0-10.2 only
  // Early iOS 10 versions support `attachShadow` but not `getRootNode`,
  // leading to errors. We need to check for `getRootNode`.
  if ( documentElement.getRootNode ) {
    isAttached = function( elem ) {
      return jQuery.contains( elem.ownerDocument, elem ) ||
        elem.getRootNode( composed ) === elem.ownerDocument;
    };
  }
var isHiddenWithinTree = function( elem, el ) {

    // isHiddenWithinTree might be called from jQuery#filter function;
    // in that case, element will be second argument
    elem = el || elem;

    // Inline style trumps all
    return elem.style.display === "none" ||
      elem.style.display === "" &&

      // Otherwise, check computed style
      // Support: Firefox <=43 - 45
      // Disconnected elements can have computed display: none, so first confirm that elem is
      // in the document.
      isAttached( elem ) &&

      jQuery.css( elem, "display" ) === "none";
  };


function adjustCSS( elem, prop, valueParts, tween ) {
  var adjusted, scale,
    maxIterations = 20,
    currentValue = tween ?
      function() {
        return tween.cur();
      } :
      function() {
        return jQuery.css( elem, prop, "" );
      },
    initial = currentValue(),
    unit = valueParts && valueParts[ 3 ] || ( jQuery.cssNumber[ prop ] ? "" : "px" ),

    // Starting value computation is required for potential unit mismatches
    initialInUnit = elem.nodeType &&
      ( jQuery.cssNumber[ prop ] || unit !== "px" && +initial ) &&
      rcssNum.exec( jQuery.css( elem, prop ) );

  if ( initialInUnit && initialInUnit[ 3 ] !== unit ) {

    // Support: Firefox <=54
    // Halve the iteration target value to prevent interference from CSS upper bounds (gh-2144)
    initial = initial / 2;

    // Trust units reported by jQuery.css
    unit = unit || initialInUnit[ 3 ];

    // Iteratively approximate from a nonzero starting point
    initialInUnit = +initial || 1;

    while ( maxIterations-- ) {

      // Evaluate and update our best guess (doubling guesses that zero out).
      // Finish if the scale equals or crosses 1 (making the old*new product non-positive).
      jQuery.style( elem, prop, initialInUnit + unit );
      if ( ( 1 - scale ) * ( 1 - ( scale = currentValue() / initial || 0.5 ) ) <= 0 ) {
        maxIterations = 0;
      }
      initialInUnit = initialInUnit / scale;

    }

    initialInUnit = initialInUnit * 2;
    jQuery.style( elem, prop, initialInUnit + unit );

    // Make sure we update the tween properties later on
    valueParts = valueParts || [];
  }

  if ( valueParts ) {
    initialInUnit = +initialInUnit || +initial || 0;

    // Apply relative offset (+=/-=) if specified
    adjusted = valueParts[ 1 ] ?
      initialInUnit + ( valueParts[ 1 ] + 1 ) * valueParts[ 2 ] :
      +valueParts[ 2 ];
    if ( tween ) {
      tween.unit = unit;
      tween.start = initialInUnit;
      tween.end = adjusted;
    }
  }
  return adjusted;
}


var defaultDisplayMap = {};

function getDefaultDisplay( elem ) {
  var temp,
    doc = elem.ownerDocument,
    nodeName = elem.nodeName,
    display = defaultDisplayMap[ nodeName ];

  if ( display ) {
    return display;
  }

  temp = doc.body.appendChild( doc.createElement( nodeName ) );
  display = jQuery.css( temp, "display" );

  temp.parentNode.removeChild( temp );

  if ( display === "none" ) {
    display = "block";
  }
  defaultDisplayMap[ nodeName ] = display;

  return display;
}

function showHide( elements, show ) {
  var display, elem,
    values = [],
    index = 0,
    length = elements.length;

  // Determine new display value for elements that need to change
  for ( ; index < length; index++ ) {
    elem = elements[ index ];
    if ( !elem.style ) {
      continue;
    }

    display = elem.style.display;
    if ( show ) {

      // Since we force visibility upon cascade-hidden elements, an immediate (and slow)
      // check is required in this first loop unless we have a nonempty display value (either
      // inline or about-to-be-restored)
      if ( display === "none" ) {
        values[ index ] = dataPriv.get( elem, "display" ) || null;
        if ( !values[ index ] ) {
          elem.style.display = "";
        }
      }
      if ( elem.style.display === "" && isHiddenWithinTree( elem ) ) {
        values[ index ] = getDefaultDisplay( elem );
      }
    } else {
      if ( display !== "none" ) {
        values[ index ] = "none";

        // Remember what we're overwriting
        dataPriv.set( elem, "display", display );
      }
    }
  }

  // Set the display of the elements in a second loop to avoid constant reflow
  for ( index = 0; index < length; index++ ) {
    if ( values[ index ] != null ) {
      elements[ index ].style.display = values[ index ];
    }
  }

  return elements;
}

jQuery.fn.extend( {
  show: function() {
    return showHide( this, true );
  },
  hide: function() {
    return showHide( this );
  },
  toggle: function( state ) {
    if ( typeof state === "boolean" ) {
      return state ? this.show() : this.hide();
    }

    return this.each( function() {
      if ( isHiddenWithinTree( this ) ) {
        jQuery( this ).show();
      } else {
        jQuery( this ).hide();
      }
    } );
  }
} );
var rcheckableType = ( /^(?:checkbox|radio)$/i );

var rtagName = ( /<([a-z][^\/\0>\x20\t\r\n\f]*)/i );

var rscriptType = ( /^$|^module$|\/(?:java|ecma)script/i );


( function() {
  var fragment = document.createDocumentFragment(),
    div = fragment.appendChild( document.createElement( "div" ) ),
    input = document.createElement( "input" );

  // Support: Android 4.0 - 4.3 only
  // Check state lost if the name is set (#11217)
  // Support: Windows Web Apps (WWA)
  // `name` and `type` must use .setAttribute for WWA (#14901)
  input.setAttribute( "type", "radio" );
  input.setAttribute( "checked", "checked" );
  input.setAttribute( "name", "t" );

  div.appendChild( input );

  // Support: Android <=4.1 only
  // Older WebKit doesn't clone checked state correctly in fragments
  support.checkClone = div.cloneNode( true ).cloneNode( true ).lastChild.checked;

  // Support: IE <=11 only
  // Make sure textarea (and checkbox) defaultValue is properly cloned
  div.innerHTML = "<textarea>x</textarea>";
  support.noCloneChecked = !!div.cloneNode( true ).lastChild.defaultValue;

  // Support: IE <=9 only
  // IE <=9 replaces <option> tags with their contents when inserted outside of
  // the select element.
  div.innerHTML = "<option></option>";
  support.option = !!div.lastChild;
} )();


// We have to close these tags to support XHTML (#13200)
var wrapMap = {

  // XHTML parsers do not magically insert elements in the
  // same way that tag soup parsers do. So we cannot shorten
  // this by omitting <tbody> or other required elements.
  thead: [ 1, "<table>", "</table>" ],
  col: [ 2, "<table><colgroup>", "</colgroup></table>" ],
  tr: [ 2, "<table><tbody>", "</tbody></table>" ],
  td: [ 3, "<table><tbody><tr>", "</tr></tbody></table>" ],

  _default: [ 0, "", "" ]
};

wrapMap.tbody = wrapMap.tfoot = wrapMap.colgroup = wrapMap.caption = wrapMap.thead;
wrapMap.th = wrapMap.td;

// Support: IE <=9 only
if ( !support.option ) {
  wrapMap.optgroup = wrapMap.option = [ 1, "<select multiple='multiple'>", "</select>" ];
}


function getAll( context, tag ) {

  // Support: IE <=9 - 11 only
  // Use typeof to avoid zero-argument method invocation on host objects (#15151)
  var ret;

  if ( typeof context.getElementsByTagName !== "undefined" ) {
    ret = context.getElementsByTagName( tag || "*" );

  } else if ( typeof context.querySelectorAll !== "undefined" ) {
    ret = context.querySelectorAll( tag || "*" );

  } else {
    ret = [];
  }

  if ( tag === undefined || tag && nodeName( context, tag ) ) {
    return jQuery.merge( [ context ], ret );
  }

  return ret;
}


// Mark scripts as having already been evaluated
function setGlobalEval( elems, refElements ) {
  var i = 0,
    l = elems.length;

  for ( ; i < l; i++ ) {
    dataPriv.set(
      elems[ i ],
      "globalEval",
      !refElements || dataPriv.get( refElements[ i ], "globalEval" )
    );
  }
}


var rhtml = /<|&#?\w+;/;

function buildFragment( elems, context, scripts, selection, ignored ) {
  var elem, tmp, tag, wrap, attached, j,
    fragment = context.createDocumentFragment(),
    nodes = [],
    i = 0,
    l = elems.length;

  for ( ; i < l; i++ ) {
    elem = elems[ i ];

    if ( elem || elem === 0 ) {

      // Add nodes directly
      if ( toType( elem ) === "object" ) {

        // Support: Android <=4.0 only, PhantomJS 1 only
        // push.apply(_, arraylike) throws on ancient WebKit
        jQuery.merge( nodes, elem.nodeType ? [ elem ] : elem );

      // Convert non-html into a text node
      } else if ( !rhtml.test( elem ) ) {
        nodes.push( context.createTextNode( elem ) );

      // Convert html into DOM nodes
      } else {
        tmp = tmp || fragment.appendChild( context.createElement( "div" ) );

        // Deserialize a standard representation
        tag = ( rtagName.exec( elem ) || [ "", "" ] )[ 1 ].toLowerCase();
        wrap = wrapMap[ tag ] || wrapMap._default;
        tmp.innerHTML = wrap[ 1 ] + jQuery.htmlPrefilter( elem ) + wrap[ 2 ];

        // Descend through wrappers to the right content
        j = wrap[ 0 ];
        while ( j-- ) {
          tmp = tmp.lastChild;
        }

        // Support: Android <=4.0 only, PhantomJS 1 only
        // push.apply(_, arraylike) throws on ancient WebKit
        jQuery.merge( nodes, tmp.childNodes );

        // Remember the top-level container
        tmp = fragment.firstChild;

        // Ensure the created nodes are orphaned (#12392)
        tmp.textContent = "";
      }
    }
  }

  // Remove wrapper from fragment
  fragment.textContent = "";

  i = 0;
  while ( ( elem = nodes[ i++ ] ) ) {

    // Skip elements already in the context collection (trac-4087)
    if ( selection && jQuery.inArray( elem, selection ) > -1 ) {
      if ( ignored ) {
        ignored.push( elem );
      }
      continue;
    }

    attached = isAttached( elem );

    // Append to fragment
    tmp = getAll( fragment.appendChild( elem ), "script" );

    // Preserve script evaluation history
    if ( attached ) {
      setGlobalEval( tmp );
    }

    // Capture executables
    if ( scripts ) {
      j = 0;
      while ( ( elem = tmp[ j++ ] ) ) {
        if ( rscriptType.test( elem.type || "" ) ) {
          scripts.push( elem );
        }
      }
    }
  }

  return fragment;
}


var rtypenamespace = /^([^.]*)(?:\.(.+)|)/;

function returnTrue() {
  return true;
}

function returnFalse() {
  return false;
}

// Support: IE <=9 - 11+
// focus() and blur() are asynchronous, except when they are no-op.
// So expect focus to be synchronous when the element is already active,
// and blur to be synchronous when the element is not already active.
// (focus and blur are always synchronous in other supported browsers,
// this just defines when we can count on it).
function expectSync( elem, type ) {
  return ( elem === safeActiveElement() ) === ( type === "focus" );
}

// Support: IE <=9 only
// Accessing document.activeElement can throw unexpectedly
// https://bugs.jquery.com/ticket/13393
function safeActiveElement() {
  try {
    return document.activeElement;
  } catch ( err ) { }
}

function on( elem, types, selector, data, fn, one ) {
  var origFn, type;

  // Types can be a map of types/handlers
  if ( typeof types === "object" ) {

    // ( types-Object, selector, data )
    if ( typeof selector !== "string" ) {

      // ( types-Object, data )
      data = data || selector;
      selector = undefined;
    }
    for ( type in types ) {
      on( elem, type, selector, data, types[ type ], one );
    }
    return elem;
  }

  if ( data == null && fn == null ) {

    // ( types, fn )
    fn = selector;
    data = selector = undefined;
  } else if ( fn == null ) {
    if ( typeof selector === "string" ) {

      // ( types, selector, fn )
      fn = data;
      data = undefined;
    } else {

      // ( types, data, fn )
      fn = data;
      data = selector;
      selector = undefined;
    }
  }
  if ( fn === false ) {
    fn = returnFalse;
  } else if ( !fn ) {
    return elem;
  }

  if ( one === 1 ) {
    origFn = fn;
    fn = function( event ) {

      // Can use an empty set, since event contains the info
      jQuery().off( event );
      return origFn.apply( this, arguments );
    };

    // Use same guid so caller can remove using origFn
    fn.guid = origFn.guid || ( origFn.guid = jQuery.guid++ );
  }
  return elem.each( function() {
    jQuery.event.add( this, types, fn, data, selector );
  } );
}

/*
 * Helper functions for managing events -- not part of the public interface.
 * Props to Dean Edwards' addEvent library for many of the ideas.
 */
jQuery.event = {

  global: {},

  add: function( elem, types, handler, data, selector ) {

    var handleObjIn, eventHandle, tmp,
      events, t, handleObj,
      special, handlers, type, namespaces, origType,
      elemData = dataPriv.get( elem );

    // Only attach events to objects that accept data
    if ( !acceptData( elem ) ) {
      return;
    }

    // Caller can pass in an object of custom data in lieu of the handler
    if ( handler.handler ) {
      handleObjIn = handler;
      handler = handleObjIn.handler;
      selector = handleObjIn.selector;
    }

    // Ensure that invalid selectors throw exceptions at attach time
    // Evaluate against documentElement in case elem is a non-element node (e.g., document)
    if ( selector ) {
      jQuery.find.matchesSelector( documentElement, selector );
    }

    // Make sure that the handler has a unique ID, used to find/remove it later
    if ( !handler.guid ) {
      handler.guid = jQuery.guid++;
    }

    // Init the element's event structure and main handler, if this is the first
    if ( !( events = elemData.events ) ) {
      events = elemData.events = Object.create( null );
    }
    if ( !( eventHandle = elemData.handle ) ) {
      eventHandle = elemData.handle = function( e ) {

        // Discard the second event of a jQuery.event.trigger() and
        // when an event is called after a page has unloaded
        return typeof jQuery !== "undefined" && jQuery.event.triggered !== e.type ?
          jQuery.event.dispatch.apply( elem, arguments ) : undefined;
      };
    }

    // Handle multiple events separated by a space
    types = ( types || "" ).match( rnothtmlwhite ) || [ "" ];
    t = types.length;
    while ( t-- ) {
      tmp = rtypenamespace.exec( types[ t ] ) || [];
      type = origType = tmp[ 1 ];
      namespaces = ( tmp[ 2 ] || "" ).split( "." ).sort();

      // There *must* be a type, no attaching namespace-only handlers
      if ( !type ) {
        continue;
      }

      // If event changes its type, use the special event handlers for the changed type
      special = jQuery.event.special[ type ] || {};

      // If selector defined, determine special event api type, otherwise given type
      type = ( selector ? special.delegateType : special.bindType ) || type;

      // Update special based on newly reset type
      special = jQuery.event.special[ type ] || {};

      // handleObj is passed to all event handlers
      handleObj = jQuery.extend( {
        type: type,
        origType: origType,
        data: data,
        handler: handler,
        guid: handler.guid,
        selector: selector,
        needsContext: selector && jQuery.expr.match.needsContext.test( selector ),
        namespace: namespaces.join( "." )
      }, handleObjIn );

      // Init the event handler queue if we're the first
      if ( !( handlers = events[ type ] ) ) {
        handlers = events[ type ] = [];
        handlers.delegateCount = 0;

        // Only use addEventListener if the special events handler returns false
        if ( !special.setup ||
          special.setup.call( elem, data, namespaces, eventHandle ) === false ) {

          if ( elem.addEventListener ) {
            elem.addEventListener( type, eventHandle );
          }
        }
      }

      if ( special.add ) {
        special.add.call( elem, handleObj );

        if ( !handleObj.handler.guid ) {
          handleObj.handler.guid = handler.guid;
        }
      }

      // Add to the element's handler list, delegates in front
      if ( selector ) {
        handlers.splice( handlers.delegateCount++, 0, handleObj );
      } else {
        handlers.push( handleObj );
      }

      // Keep track of which events have ever been used, for event optimization
      jQuery.event.global[ type ] = true;
    }

  },

  // Detach an event or set of events from an element
  remove: function( elem, types, handler, selector, mappedTypes ) {

    var j, origCount, tmp,
      events, t, handleObj,
      special, handlers, type, namespaces, origType,
      elemData = dataPriv.hasData( elem ) && dataPriv.get( elem );

    if ( !elemData || !( events = elemData.events ) ) {
      return;
    }

    // Once for each type.namespace in types; type may be omitted
    types = ( types || "" ).match( rnothtmlwhite ) || [ "" ];
    t = types.length;
    while ( t-- ) {
      tmp = rtypenamespace.exec( types[ t ] ) || [];
      type = origType = tmp[ 1 ];
      namespaces = ( tmp[ 2 ] || "" ).split( "." ).sort();

      // Unbind all events (on this namespace, if provided) for the element
      if ( !type ) {
        for ( type in events ) {
          jQuery.event.remove( elem, type + types[ t ], handler, selector, true );
        }
        continue;
      }

      special = jQuery.event.special[ type ] || {};
      type = ( selector ? special.delegateType : special.bindType ) || type;
      handlers = events[ type ] || [];
      tmp = tmp[ 2 ] &&
        new RegExp( "(^|\\.)" + namespaces.join( "\\.(?:.*\\.|)" ) + "(\\.|$)" );

      // Remove matching events
      origCount = j = handlers.length;
      while ( j-- ) {
        handleObj = handlers[ j ];

        if ( ( mappedTypes || origType === handleObj.origType ) &&
          ( !handler || handler.guid === handleObj.guid ) &&
          ( !tmp || tmp.test( handleObj.namespace ) ) &&
          ( !selector || selector === handleObj.selector ||
            selector === "**" && handleObj.selector ) ) {
          handlers.splice( j, 1 );

          if ( handleObj.selector ) {
            handlers.delegateCount--;
          }
          if ( special.remove ) {
            special.remove.call( elem, handleObj );
          }
        }
      }

      // Remove generic event handler if we removed something and no more handlers exist
      // (avoids potential for endless recursion during removal of special event handlers)
      if ( origCount && !handlers.length ) {
        if ( !special.teardown ||
          special.teardown.call( elem, namespaces, elemData.handle ) === false ) {

          jQuery.removeEvent( elem, type, elemData.handle );
        }

        delete events[ type ];
      }
    }

    // Remove data and the expando if it's no longer used
    if ( jQuery.isEmptyObject( events ) ) {
      dataPriv.remove( elem, "handle events" );
    }
  },

  dispatch: function( nativeEvent ) {

    var i, j, ret, matched, handleObj, handlerQueue,
      args = new Array( arguments.length ),

      // Make a writable jQuery.Event from the native event object
      event = jQuery.event.fix( nativeEvent ),

      handlers = (
        dataPriv.get( this, "events" ) || Object.create( null )
      )[ event.type ] || [],
      special = jQuery.event.special[ event.type ] || {};

    // Use the fix-ed jQuery.Event rather than the (read-only) native event
    args[ 0 ] = event;

    for ( i = 1; i < arguments.length; i++ ) {
      args[ i ] = arguments[ i ];
    }

    event.delegateTarget = this;

    // Call the preDispatch hook for the mapped type, and let it bail if desired
    if ( special.preDispatch && special.preDispatch.call( this, event ) === false ) {
      return;
    }

    // Determine handlers
    handlerQueue = jQuery.event.handlers.call( this, event, handlers );

    // Run delegates first; they may want to stop propagation beneath us
    i = 0;
    while ( ( matched = handlerQueue[ i++ ] ) && !event.isPropagationStopped() ) {
      event.currentTarget = matched.elem;

      j = 0;
      while ( ( handleObj = matched.handlers[ j++ ] ) &&
        !event.isImmediatePropagationStopped() ) {

        // If the event is namespaced, then each handler is only invoked if it is
        // specially universal or its namespaces are a superset of the event's.
        if ( !event.rnamespace || handleObj.namespace === false ||
          event.rnamespace.test( handleObj.namespace ) ) {

          event.handleObj = handleObj;
          event.data = handleObj.data;

          ret = ( ( jQuery.event.special[ handleObj.origType ] || {} ).handle ||
            handleObj.handler ).apply( matched.elem, args );

          if ( ret !== undefined ) {
            if ( ( event.result = ret ) === false ) {
              event.preventDefault();
              event.stopPropagation();
            }
          }
        }
      }
    }

    // Call the postDispatch hook for the mapped type
    if ( special.postDispatch ) {
      special.postDispatch.call( this, event );
    }

    return event.result;
  },

  handlers: function( event, handlers ) {
    var i, handleObj, sel, matchedHandlers, matchedSelectors,
      handlerQueue = [],
      delegateCount = handlers.delegateCount,
      cur = event.target;

    // Find delegate handlers
    if ( delegateCount &&

      // Support: IE <=9
      // Black-hole SVG <use> instance trees (trac-13180)
      cur.nodeType &&

      // Support: Firefox <=42
      // Suppress spec-violating clicks indicating a non-primary pointer button (trac-3861)
      // https://www.w3.org/TR/DOM-Level-3-Events/#event-type-click
      // Support: IE 11 only
      // ...but not arrow key "clicks" of radio inputs, which can have `button` -1 (gh-2343)
      !( event.type === "click" && event.button >= 1 ) ) {

      for ( ; cur !== this; cur = cur.parentNode || this ) {

        // Don't check non-elements (#13208)
        // Don't process clicks on disabled elements (#6911, #8165, #11382, #11764)
        if ( cur.nodeType === 1 && !( event.type === "click" && cur.disabled === true ) ) {
          matchedHandlers = [];
          matchedSelectors = {};
          for ( i = 0; i < delegateCount; i++ ) {
            handleObj = handlers[ i ];

            // Don't conflict with Object.prototype properties (#13203)
            sel = handleObj.selector + " ";

            if ( matchedSelectors[ sel ] === undefined ) {
              matchedSelectors[ sel ] = handleObj.needsContext ?
                jQuery( sel, this ).index( cur ) > -1 :
                jQuery.find( sel, this, null, [ cur ] ).length;
            }
            if ( matchedSelectors[ sel ] ) {
              matchedHandlers.push( handleObj );
            }
          }
          if ( matchedHandlers.length ) {
            handlerQueue.push( { elem: cur, handlers: matchedHandlers } );
          }
        }
      }
    }

    // Add the remaining (directly-bound) handlers
    cur = this;
    if ( delegateCount < handlers.length ) {
      handlerQueue.push( { elem: cur, handlers: handlers.slice( delegateCount ) } );
    }

    return handlerQueue;
  },

  addProp: function( name, hook ) {
    Object.defineProperty( jQuery.Event.prototype, name, {
      enumerable: true,
      configurable: true,

      get: isFunction( hook ) ?
        function() {
          if ( this.originalEvent ) {
            return hook( this.originalEvent );
          }
        } :
        function() {
          if ( this.originalEvent ) {
            return this.originalEvent[ name ];
          }
        },

      set: function( value ) {
        Object.defineProperty( this, name, {
          enumerable: true,
          configurable: true,
          writable: true,
          value: value
        } );
      }
    } );
  },

  fix: function( originalEvent ) {
    return originalEvent[ jQuery.expando ] ?
      originalEvent :
      new jQuery.Event( originalEvent );
  },

  special: {
    load: {

      // Prevent triggered image.load events from bubbling to window.load
      noBubble: true
    },
    click: {

      // Utilize native event to ensure correct state for checkable inputs
      setup: function( data ) {

        // For mutual compressibility with _default, replace `this` access with a local var.
        // `|| data` is dead code meant only to preserve the variable through minification.
        var el = this || data;

        // Claim the first handler
        if ( rcheckableType.test( el.type ) &&
          el.click && nodeName( el, "input" ) ) {

          // dataPriv.set( el, "click", ... )
          leverageNative( el, "click", returnTrue );
        }

        // Return false to allow normal processing in the caller
        return false;
      },
      trigger: function( data ) {

        // For mutual compressibility with _default, replace `this` access with a local var.
        // `|| data` is dead code meant only to preserve the variable through minification.
        var el = this || data;

        // Force setup before triggering a click
        if ( rcheckableType.test( el.type ) &&
          el.click && nodeName( el, "input" ) ) {

          leverageNative( el, "click" );
        }

        // Return non-false to allow normal event-path propagation
        return true;
      },

      // For cross-browser consistency, suppress native .click() on links
      // Also prevent it if we're currently inside a leveraged native-event stack
      _default: function( event ) {
        var target = event.target;
        return rcheckableType.test( target.type ) &&
          target.click && nodeName( target, "input" ) &&
          dataPriv.get( target, "click" ) ||
          nodeName( target, "a" );
      }
    },

    beforeunload: {
      postDispatch: function( event ) {

        // Support: Firefox 20+
        // Firefox doesn't alert if the returnValue field is not set.
        if ( event.result !== undefined && event.originalEvent ) {
          event.originalEvent.returnValue = event.result;
        }
      }
    }
  }
};

// Ensure the presence of an event listener that handles manually-triggered
// synthetic events by interrupting progress until reinvoked in response to
// *native* events that it fires directly, ensuring that state changes have
// already occurred before other listeners are invoked.
function leverageNative( el, type, expectSync ) {

  // Missing expectSync indicates a trigger call, which must force setup through jQuery.event.add
  if ( !expectSync ) {
    if ( dataPriv.get( el, type ) === undefined ) {
      jQuery.event.add( el, type, returnTrue );
    }
    return;
  }

  // Register the controller as a special universal handler for all event namespaces
  dataPriv.set( el, type, false );
  jQuery.event.add( el, type, {
    namespace: false,
    handler: function( event ) {
      var notAsync, result,
        saved = dataPriv.get( this, type );

      if ( ( event.isTrigger & 1 ) && this[ type ] ) {

        // Interrupt processing of the outer synthetic .trigger()ed event
        // Saved data should be false in such cases, but might be a leftover capture object
        // from an async native handler (gh-4350)
        if ( !saved.length ) {

          // Store arguments for use when handling the inner native event
          // There will always be at least one argument (an event object), so this array
          // will not be confused with a leftover capture object.
          saved = slice.call( arguments );
          dataPriv.set( this, type, saved );

          // Trigger the native event and capture its result
          // Support: IE <=9 - 11+
          // focus() and blur() are asynchronous
          notAsync = expectSync( this, type );
          this[ type ]();
          result = dataPriv.get( this, type );
          if ( saved !== result || notAsync ) {
            dataPriv.set( this, type, false );
          } else {
            result = {};
          }
          if ( saved !== result ) {

            // Cancel the outer synthetic event
            event.stopImmediatePropagation();
            event.preventDefault();

            // Support: Chrome 86+
            // In Chrome, if an element having a focusout handler is blurred by
            // clicking outside of it, it invokes the handler synchronously. If
            // that handler calls `.remove()` on the element, the data is cleared,
            // leaving `result` undefined. We need to guard against this.
            return result && result.value;
          }

        // If this is an inner synthetic event for an event with a bubbling surrogate
        // (focus or blur), assume that the surrogate already propagated from triggering the
        // native event and prevent that from happening again here.
        // This technically gets the ordering wrong w.r.t. to `.trigger()` (in which the
        // bubbling surrogate propagates *after* the non-bubbling base), but that seems
        // less bad than duplication.
        } else if ( ( jQuery.event.special[ type ] || {} ).delegateType ) {
          event.stopPropagation();
        }

      // If this is a native event triggered above, everything is now in order
      // Fire an inner synthetic event with the original arguments
      } else if ( saved.length ) {

        // ...and capture the result
        dataPriv.set( this, type, {
          value: jQuery.event.trigger(

            // Support: IE <=9 - 11+
            // Extend with the prototype to reset the above stopImmediatePropagation()
            jQuery.extend( saved[ 0 ], jQuery.Event.prototype ),
            saved.slice( 1 ),
            this
          )
        } );

        // Abort handling of the native event
        event.stopImmediatePropagation();
      }
    }
  } );
}

jQuery.removeEvent = function( elem, type, handle ) {

  // This "if" is needed for plain objects
  if ( elem.removeEventListener ) {
    elem.removeEventListener( type, handle );
  }
};

jQuery.Event = function( src, props ) {

  // Allow instantiation without the 'new' keyword
  if ( !( this instanceof jQuery.Event ) ) {
    return new jQuery.Event( src, props );
  }

  // Event object
  if ( src && src.type ) {
    this.originalEvent = src;
    this.type = src.type;

    // Events bubbling up the document may have been marked as prevented
    // by a handler lower down the tree; reflect the correct value.
    this.isDefaultPrevented = src.defaultPrevented ||
        src.defaultPrevented === undefined &&

        // Support: Android <=2.3 only
        src.returnValue === false ?
      returnTrue :
      returnFalse;

    // Create target properties
    // Support: Safari <=6 - 7 only
    // Target should not be a text node (#504, #13143)
    this.target = ( src.target && src.target.nodeType === 3 ) ?
      src.target.parentNode :
      src.target;

    this.currentTarget = src.currentTarget;
    this.relatedTarget = src.relatedTarget;

  // Event type
  } else {
    this.type = src;
  }

  // Put explicitly provided properties onto the event object
  if ( props ) {
    jQuery.extend( this, props );
  }

  // Create a timestamp if incoming event doesn't have one
  this.timeStamp = src && src.timeStamp || Date.now();

  // Mark it as fixed
  this[ jQuery.expando ] = true;
};

// jQuery.Event is based on DOM3 Events as specified by the ECMAScript Language Binding
// https://www.w3.org/TR/2003/WD-DOM-Level-3-Events-20030331/ecma-script-binding.html
jQuery.Event.prototype = {
  constructor: jQuery.Event,
  isDefaultPrevented: returnFalse,
  isPropagationStopped: returnFalse,
  isImmediatePropagationStopped: returnFalse,
  isSimulated: false,

  preventDefault: function() {
    var e = this.originalEvent;

    this.isDefaultPrevented = returnTrue;

    if ( e && !this.isSimulated ) {
      e.preventDefault();
    }
  },
  stopPropagation: function() {
    var e = this.originalEvent;

    this.isPropagationStopped = returnTrue;

    if ( e && !this.isSimulated ) {
      e.stopPropagation();
    }
  },
  stopImmediatePropagation: function() {
    var e = this.originalEvent;

    this.isImmediatePropagationStopped = returnTrue;

    if ( e && !this.isSimulated ) {
      e.stopImmediatePropagation();
    }

    this.stopPropagation();
  }
};

// Includes all common event props including KeyEvent and MouseEvent specific props
jQuery.each( {
  altKey: true,
  bubbles: true,
  cancelable: true,
  changedTouches: true,
  ctrlKey: true,
  detail: true,
  eventPhase: true,
  metaKey: true,
  pageX: true,
  pageY: true,
  shiftKey: true,
  view: true,
  "char": true,
  code: true,
  charCode: true,
  key: true,
  keyCode: true,
  button: true,
  buttons: true,
  clientX: true,
  clientY: true,
  offsetX: true,
  offsetY: true,
  pointerId: true,
  pointerType: true,
  screenX: true,
  screenY: true,
  targetTouches: true,
  toElement: true,
  touches: true,
  which: true
}, jQuery.event.addProp );

jQuery.each( { focus: "focusin", blur: "focusout" }, function( type, delegateType ) {
  jQuery.event.special[ type ] = {

    // Utilize native event if possible so blur/focus sequence is correct
    setup: function() {

      // Claim the first handler
      // dataPriv.set( this, "focus", ... )
      // dataPriv.set( this, "blur", ... )
      leverageNative( this, type, expectSync );

      // Return false to allow normal processing in the caller
      return false;
    },
    trigger: function() {

      // Force setup before trigger
      leverageNative( this, type );

      // Return non-false to allow normal event-path propagation
      return true;
    },

    // Suppress native focus or blur as it's already being fired
    // in leverageNative.
    _default: function() {
      return true;
    },

    delegateType: delegateType
  };
} );

// Create mouseenter/leave events using mouseover/out and event-time checks
// so that event delegation works in jQuery.
// Do the same for pointerenter/pointerleave and pointerover/pointerout
//
// Support: Safari 7 only
// Safari sends mouseenter too often; see:
// https://bugs.chromium.org/p/chromium/issues/detail?id=470258
// for the description of the bug (it existed in older Chrome versions as well).
jQuery.each( {
  mouseenter: "mouseover",
  mouseleave: "mouseout",
  pointerenter: "pointerover",
  pointerleave: "pointerout"
}, function( orig, fix ) {
  jQuery.event.special[ orig ] = {
    delegateType: fix,
    bindType: fix,

    handle: function( event ) {
      var ret,
        target = this,
        related = event.relatedTarget,
        handleObj = event.handleObj;

      // For mouseenter/leave call the handler if related is outside the target.
      // NB: No relatedTarget if the mouse left/entered the browser window
      if ( !related || ( related !== target && !jQuery.contains( target, related ) ) ) {
        event.type = handleObj.origType;
        ret = handleObj.handler.apply( this, arguments );
        event.type = fix;
      }
      return ret;
    }
  };
} );

jQuery.fn.extend( {

  on: function( types, selector, data, fn ) {
    return on( this, types, selector, data, fn );
  },
  one: function( types, selector, data, fn ) {
    return on( this, types, selector, data, fn, 1 );
  },
  off: function( types, selector, fn ) {
    var handleObj, type;
    if ( types && types.preventDefault && types.handleObj ) {

      // ( event )  dispatched jQuery.Event
      handleObj = types.handleObj;
      jQuery( types.delegateTarget ).off(
        handleObj.namespace ?
          handleObj.origType + "." + handleObj.namespace :
          handleObj.origType,
        handleObj.selector,
        handleObj.handler
      );
      return this;
    }
    if ( typeof types === "object" ) {

      // ( types-object [, selector] )
      for ( type in types ) {
        this.off( type, selector, types[ type ] );
      }
      return this;
    }
    if ( selector === false || typeof selector === "function" ) {

      // ( types [, fn] )
      fn = selector;
      selector = undefined;
    }
    if ( fn === false ) {
      fn = returnFalse;
    }
    return this.each( function() {
      jQuery.event.remove( this, types, fn, selector );
    } );
  }
} );


var

  // Support: IE <=10 - 11, Edge 12 - 13 only
  // In IE/Edge using regex groups here causes severe slowdowns.
  // See https://connect.microsoft.com/IE/feedback/details/1736512/
  rnoInnerhtml = /<script|<style|<link/i,

  // checked="checked" or checked
  rchecked = /checked\s*(?:[^=]|=\s*.checked.)/i,
  rcleanScript = /^\s*<!(?:\[CDATA\[|--)|(?:\]\]|--)>\s*$/g;

// Prefer a tbody over its parent table for containing new rows
function manipulationTarget( elem, content ) {
  if ( nodeName( elem, "table" ) &&
    nodeName( content.nodeType !== 11 ? content : content.firstChild, "tr" ) ) {

    return jQuery( elem ).children( "tbody" )[ 0 ] || elem;
  }

  return elem;
}

// Replace/restore the type attribute of script elements for safe DOM manipulation
function disableScript( elem ) {
  elem.type = ( elem.getAttribute( "type" ) !== null ) + "/" + elem.type;
  return elem;
}
function restoreScript( elem ) {
  if ( ( elem.type || "" ).slice( 0, 5 ) === "true/" ) {
    elem.type = elem.type.slice( 5 );
  } else {
    elem.removeAttribute( "type" );
  }

  return elem;
}

function cloneCopyEvent( src, dest ) {
  var i, l, type, pdataOld, udataOld, udataCur, events;

  if ( dest.nodeType !== 1 ) {
    return;
  }

  // 1. Copy private data: events, handlers, etc.
  if ( dataPriv.hasData( src ) ) {
    pdataOld = dataPriv.get( src );
    events = pdataOld.events;

    if ( events ) {
      dataPriv.remove( dest, "handle events" );

      for ( type in events ) {
        for ( i = 0, l = events[ type ].length; i < l; i++ ) {
          jQuery.event.add( dest, type, events[ type ][ i ] );
        }
      }
    }
  }

  // 2. Copy user data
  if ( dataUser.hasData( src ) ) {
    udataOld = dataUser.access( src );
    udataCur = jQuery.extend( {}, udataOld );

    dataUser.set( dest, udataCur );
  }
}

// Fix IE bugs, see support tests
function fixInput( src, dest ) {
  var nodeName = dest.nodeName.toLowerCase();

  // Fails to persist the checked state of a cloned checkbox or radio button.
  if ( nodeName === "input" && rcheckableType.test( src.type ) ) {
    dest.checked = src.checked;

  // Fails to return the selected option to the default selected state when cloning options
  } else if ( nodeName === "input" || nodeName === "textarea" ) {
    dest.defaultValue = src.defaultValue;
  }
}

function domManip( collection, args, callback, ignored ) {

  // Flatten any nested arrays
  args = flat( args );

  var fragment, first, scripts, hasScripts, node, doc,
    i = 0,
    l = collection.length,
    iNoClone = l - 1,
    value = args[ 0 ],
    valueIsFunction = isFunction( value );

  // We can't cloneNode fragments that contain checked, in WebKit
  if ( valueIsFunction ||
      ( l > 1 && typeof value === "string" &&
        !support.checkClone && rchecked.test( value ) ) ) {
    return collection.each( function( index ) {
      var self = collection.eq( index );
      if ( valueIsFunction ) {
        args[ 0 ] = value.call( this, index, self.html() );
      }
      domManip( self, args, callback, ignored );
    } );
  }

  if ( l ) {
    fragment = buildFragment( args, collection[ 0 ].ownerDocument, false, collection, ignored );
    first = fragment.firstChild;

    if ( fragment.childNodes.length === 1 ) {
      fragment = first;
    }

    // Require either new content or an interest in ignored elements to invoke the callback
    if ( first || ignored ) {
      scripts = jQuery.map( getAll( fragment, "script" ), disableScript );
      hasScripts = scripts.length;

      // Use the original fragment for the last item
      // instead of the first because it can end up
      // being emptied incorrectly in certain situations (#8070).
      for ( ; i < l; i++ ) {
        node = fragment;

        if ( i !== iNoClone ) {
          node = jQuery.clone( node, true, true );

          // Keep references to cloned scripts for later restoration
          if ( hasScripts ) {

            // Support: Android <=4.0 only, PhantomJS 1 only
            // push.apply(_, arraylike) throws on ancient WebKit
            jQuery.merge( scripts, getAll( node, "script" ) );
          }
        }

        callback.call( collection[ i ], node, i );
      }

      if ( hasScripts ) {
        doc = scripts[ scripts.length - 1 ].ownerDocument;

        // Reenable scripts
        jQuery.map( scripts, restoreScript );

        // Evaluate executable scripts on first document insertion
        for ( i = 0; i < hasScripts; i++ ) {
          node = scripts[ i ];
          if ( rscriptType.test( node.type || "" ) &&
            !dataPriv.access( node, "globalEval" ) &&
            jQuery.contains( doc, node ) ) {

            if ( node.src && ( node.type || "" ).toLowerCase()  !== "module" ) {

              // Optional AJAX dependency, but won't run scripts if not present
              if ( jQuery._evalUrl && !node.noModule ) {
                jQuery._evalUrl( node.src, {
                  nonce: node.nonce || node.getAttribute( "nonce" )
                }, doc );
              }
            } else {
              DOMEval( node.textContent.replace( rcleanScript, "" ), node, doc );
            }
          }
        }
      }
    }
  }

  return collection;
}

function remove( elem, selector, keepData ) {
  var node,
    nodes = selector ? jQuery.filter( selector, elem ) : elem,
    i = 0;

  for ( ; ( node = nodes[ i ] ) != null; i++ ) {
    if ( !keepData && node.nodeType === 1 ) {
      jQuery.cleanData( getAll( node ) );
    }

    if ( node.parentNode ) {
      if ( keepData && isAttached( node ) ) {
        setGlobalEval( getAll( node, "script" ) );
      }
      node.parentNode.removeChild( node );
    }
  }

  return elem;
}

jQuery.extend( {
  htmlPrefilter: function( html ) {
    return html;
  },

  clone: function( elem, dataAndEvents, deepDataAndEvents ) {
    var i, l, srcElements, destElements,
      clone = elem.cloneNode( true ),
      inPage = isAttached( elem );

    // Fix IE cloning issues
    if ( !support.noCloneChecked && ( elem.nodeType === 1 || elem.nodeType === 11 ) &&
        !jQuery.isXMLDoc( elem ) ) {

      // We eschew Sizzle here for performance reasons: https://jsperf.com/getall-vs-sizzle/2
      destElements = getAll( clone );
      srcElements = getAll( elem );

      for ( i = 0, l = srcElements.length; i < l; i++ ) {
        fixInput( srcElements[ i ], destElements[ i ] );
      }
    }

    // Copy the events from the original to the clone
    if ( dataAndEvents ) {
      if ( deepDataAndEvents ) {
        srcElements = srcElements || getAll( elem );
        destElements = destElements || getAll( clone );

        for ( i = 0, l = srcElements.length; i < l; i++ ) {
          cloneCopyEvent( srcElements[ i ], destElements[ i ] );
        }
      } else {
        cloneCopyEvent( elem, clone );
      }
    }

    // Preserve script evaluation history
    destElements = getAll( clone, "script" );
    if ( destElements.length > 0 ) {
      setGlobalEval( destElements, !inPage && getAll( elem, "script" ) );
    }

    // Return the cloned set
    return clone;
  },

  cleanData: function( elems ) {
    var data, elem, type,
      special = jQuery.event.special,
      i = 0;

    for ( ; ( elem = elems[ i ] ) !== undefined; i++ ) {
      if ( acceptData( elem ) ) {
        if ( ( data = elem[ dataPriv.expando ] ) ) {
          if ( data.events ) {
            for ( type in data.events ) {
              if ( special[ type ] ) {
                jQuery.event.remove( elem, type );

              // This is a shortcut to avoid jQuery.event.remove's overhead
              } else {
                jQuery.removeEvent( elem, type, data.handle );
              }
            }
          }

          // Support: Chrome <=35 - 45+
          // Assign undefined instead of using delete, see Data#remove
          elem[ dataPriv.expando ] = undefined;
        }
        if ( elem[ dataUser.expando ] ) {

          // Support: Chrome <=35 - 45+
          // Assign undefined instead of using delete, see Data#remove
          elem[ dataUser.expando ] = undefined;
        }
      }
    }
  }
} );

jQuery.fn.extend( {
  detach: function( selector ) {
    return remove( this, selector, true );
  },

  remove: function( selector ) {
    return remove( this, selector );
  },

  text: function( value ) {
    return access( this, function( value ) {
      return value === undefined ?
        jQuery.text( this ) :
        this.empty().each( function() {
          if ( this.nodeType === 1 || this.nodeType === 11 || this.nodeType === 9 ) {
            this.textContent = value;
          }
        } );
    }, null, value, arguments.length );
  },

  append: function() {
    return domManip( this, arguments, function( elem ) {
      if ( this.nodeType === 1 || this.nodeType === 11 || this.nodeType === 9 ) {
        var target = manipulationTarget( this, elem );
        target.appendChild( elem );
      }
    } );
  },

  prepend: function() {
    return domManip( this, arguments, function( elem ) {
      if ( this.nodeType === 1 || this.nodeType === 11 || this.nodeType === 9 ) {
        var target = manipulationTarget( this, elem );
        target.insertBefore( elem, target.firstChild );
      }
    } );
  },

  before: function() {
    return domManip( this, arguments, function( elem ) {
      if ( this.parentNode ) {
        this.parentNode.insertBefore( elem, this );
      }
    } );
  },

  after: function() {
    return domManip( this, arguments, function( elem ) {
      if ( this.parentNode ) {
        this.parentNode.insertBefore( elem, this.nextSibling );
      }
    } );
  },

  empty: function() {
    var elem,
      i = 0;

    for ( ; ( elem = this[ i ] ) != null; i++ ) {
      if ( elem.nodeType === 1 ) {

        // Prevent memory leaks
        jQuery.cleanData( getAll( elem, false ) );

        // Remove any remaining nodes
        elem.textContent = "";
      }
    }

    return this;
  },

  clone: function( dataAndEvents, deepDataAndEvents ) {
    dataAndEvents = dataAndEvents == null ? false : dataAndEvents;
    deepDataAndEvents = deepDataAndEvents == null ? dataAndEvents : deepDataAndEvents;

    return this.map( function() {
      return jQuery.clone( this, dataAndEvents, deepDataAndEvents );
    } );
  },

  html: function( value ) {
    return access( this, function( value ) {
      var elem = this[ 0 ] || {},
        i = 0,
        l = this.length;

      if ( value === undefined && elem.nodeType === 1 ) {
        return elem.innerHTML;
      }

      // See if we can take a shortcut and just use innerHTML
      if ( typeof value === "string" && !rnoInnerhtml.test( value ) &&
        !wrapMap[ ( rtagName.exec( value ) || [ "", "" ] )[ 1 ].toLowerCase() ] ) {

        value = jQuery.htmlPrefilter( value );

        try {
          for ( ; i < l; i++ ) {
            elem = this[ i ] || {};

            // Remove element nodes and prevent memory leaks
            if ( elem.nodeType === 1 ) {
              jQuery.cleanData( getAll( elem, false ) );
              elem.innerHTML = value;
            }
          }

          elem = 0;

        // If using innerHTML throws an exception, use the fallback method
        } catch ( e ) {}
      }

      if ( elem ) {
        this.empty().append( value );
      }
    }, null, value, arguments.length );
  },

  replaceWith: function() {
    var ignored = [];

    // Make the changes, replacing each non-ignored context element with the new content
    return domManip( this, arguments, function( elem ) {
      var parent = this.parentNode;

      if ( jQuery.inArray( this, ignored ) < 0 ) {
        jQuery.cleanData( getAll( this ) );
        if ( parent ) {
          parent.replaceChild( elem, this );
        }
      }

    // Force callback invocation
    }, ignored );
  }
} );

jQuery.each( {
  appendTo: "append",
  prependTo: "prepend",
  insertBefore: "before",
  insertAfter: "after",
  replaceAll: "replaceWith"
}, function( name, original ) {
  jQuery.fn[ name ] = function( selector ) {
    var elems,
      ret = [],
      insert = jQuery( selector ),
      last = insert.length - 1,
      i = 0;

    for ( ; i <= last; i++ ) {
      elems = i === last ? this : this.clone( true );
      jQuery( insert[ i ] )[ original ]( elems );

      // Support: Android <=4.0 only, PhantomJS 1 only
      // .get() because push.apply(_, arraylike) throws on ancient WebKit
      push.apply( ret, elems.get() );
    }

    return this.pushStack( ret );
  };
} );
var rnumnonpx = new RegExp( "^(" + pnum + ")(?!px)[a-z%]+$", "i" );

var getStyles = function( elem ) {

    // Support: IE <=11 only, Firefox <=30 (#15098, #14150)
    // IE throws on elements created in popups
    // FF meanwhile throws on frame elements through "defaultView.getComputedStyle"
    var view = elem.ownerDocument.defaultView;

    if ( !view || !view.opener ) {
      view = window;
    }

    return view.getComputedStyle( elem );
  };

var swap = function( elem, options, callback ) {
  var ret, name,
    old = {};

  // Remember the old values, and insert the new ones
  for ( name in options ) {
    old[ name ] = elem.style[ name ];
    elem.style[ name ] = options[ name ];
  }

  ret = callback.call( elem );

  // Revert the old values
  for ( name in options ) {
    elem.style[ name ] = old[ name ];
  }

  return ret;
};


var rboxStyle = new RegExp( cssExpand.join( "|" ), "i" );


( function() {

  // Executing both pixelPosition & boxSizingReliable tests require only one layout
  // so they're executed at the same time to save the second computation.
  function computeStyleTests() {

    // This is a singleton, we need to execute it only once
    if ( !div ) {
      return;
    }

    container.style.cssText = "position:absolute;left:-11111px;width:60px;" +
      "margin-top:1px;padding:0;border:0";
    div.style.cssText =
      "position:relative;display:block;box-sizing:border-box;overflow:scroll;" +
      "margin:auto;border:1px;padding:1px;" +
      "width:60%;top:1%";
    documentElement.appendChild( container ).appendChild( div );

    var divStyle = window.getComputedStyle( div );
    pixelPositionVal = divStyle.top !== "1%";

    // Support: Android 4.0 - 4.3 only, Firefox <=3 - 44
    reliableMarginLeftVal = roundPixelMeasures( divStyle.marginLeft ) === 12;

    // Support: Android 4.0 - 4.3 only, Safari <=9.1 - 10.1, iOS <=7.0 - 9.3
    // Some styles come back with percentage values, even though they shouldn't
    div.style.right = "60%";
    pixelBoxStylesVal = roundPixelMeasures( divStyle.right ) === 36;

    // Support: IE 9 - 11 only
    // Detect misreporting of content dimensions for box-sizing:border-box elements
    boxSizingReliableVal = roundPixelMeasures( divStyle.width ) === 36;

    // Support: IE 9 only
    // Detect overflow:scroll screwiness (gh-3699)
    // Support: Chrome <=64
    // Don't get tricked when zoom affects offsetWidth (gh-4029)
    div.style.position = "absolute";
    scrollboxSizeVal = roundPixelMeasures( div.offsetWidth / 3 ) === 12;

    documentElement.removeChild( container );

    // Nullify the div so it wouldn't be stored in the memory and
    // it will also be a sign that checks already performed
    div = null;
  }

  function roundPixelMeasures( measure ) {
    return Math.round( parseFloat( measure ) );
  }

  var pixelPositionVal, boxSizingReliableVal, scrollboxSizeVal, pixelBoxStylesVal,
    reliableTrDimensionsVal, reliableMarginLeftVal,
    container = document.createElement( "div" ),
    div = document.createElement( "div" );

  // Finish early in limited (non-browser) environments
  if ( !div.style ) {
    return;
  }

  // Support: IE <=9 - 11 only
  // Style of cloned element affects source element cloned (#8908)
  div.style.backgroundClip = "content-box";
  div.cloneNode( true ).style.backgroundClip = "";
  support.clearCloneStyle = div.style.backgroundClip === "content-box";

  jQuery.extend( support, {
    boxSizingReliable: function() {
      computeStyleTests();
      return boxSizingReliableVal;
    },
    pixelBoxStyles: function() {
      computeStyleTests();
      return pixelBoxStylesVal;
    },
    pixelPosition: function() {
      computeStyleTests();
      return pixelPositionVal;
    },
    reliableMarginLeft: function() {
      computeStyleTests();
      return reliableMarginLeftVal;
    },
    scrollboxSize: function() {
      computeStyleTests();
      return scrollboxSizeVal;
    },

    // Support: IE 9 - 11+, Edge 15 - 18+
    // IE/Edge misreport `getComputedStyle` of table rows with width/height
    // set in CSS while `offset*` properties report correct values.
    // Behavior in IE 9 is more subtle than in newer versions & it passes
    // some versions of this test; make sure not to make it pass there!
    //
    // Support: Firefox 70+
    // Only Firefox includes border widths
    // in computed dimensions. (gh-4529)
    reliableTrDimensions: function() {
      var table, tr, trChild, trStyle;
      if ( reliableTrDimensionsVal == null ) {
        table = document.createElement( "table" );
        tr = document.createElement( "tr" );
        trChild = document.createElement( "div" );

        table.style.cssText = "position:absolute;left:-11111px;border-collapse:separate";
        tr.style.cssText = "border:1px solid";

        // Support: Chrome 86+
        // Height set through cssText does not get applied.
        // Computed height then comes back as 0.
        tr.style.height = "1px";
        trChild.style.height = "9px";

        // Support: Android 8 Chrome 86+
        // In our bodyBackground.html iframe,
        // display for all div elements is set to "inline",
        // which causes a problem only in Android 8 Chrome 86.
        // Ensuring the div is display: block
        // gets around this issue.
        trChild.style.display = "block";

        documentElement
          .appendChild( table )
          .appendChild( tr )
          .appendChild( trChild );

        trStyle = window.getComputedStyle( tr );
        reliableTrDimensionsVal = ( parseInt( trStyle.height, 10 ) +
          parseInt( trStyle.borderTopWidth, 10 ) +
          parseInt( trStyle.borderBottomWidth, 10 ) ) === tr.offsetHeight;

        documentElement.removeChild( table );
      }
      return reliableTrDimensionsVal;
    }
  } );
} )();


function curCSS( elem, name, computed ) {
  var width, minWidth, maxWidth, ret,

    // Support: Firefox 51+
    // Retrieving style before computed somehow
    // fixes an issue with getting wrong values
    // on detached elements
    style = elem.style;

  computed = computed || getStyles( elem );

  // getPropertyValue is needed for:
  //   .css('filter') (IE 9 only, #12537)
  //   .css('--customProperty) (#3144)
  if ( computed ) {
    ret = computed.getPropertyValue( name ) || computed[ name ];

    if ( ret === "" && !isAttached( elem ) ) {
      ret = jQuery.style( elem, name );
    }

    // A tribute to the "awesome hack by Dean Edwards"
    // Android Browser returns percentage for some values,
    // but width seems to be reliably pixels.
    // This is against the CSSOM draft spec:
    // https://drafts.csswg.org/cssom/#resolved-values
    if ( !support.pixelBoxStyles() && rnumnonpx.test( ret ) && rboxStyle.test( name ) ) {

      // Remember the original values
      width = style.width;
      minWidth = style.minWidth;
      maxWidth = style.maxWidth;

      // Put in the new values to get a computed value out
      style.minWidth = style.maxWidth = style.width = ret;
      ret = computed.width;

      // Revert the changed values
      style.width = width;
      style.minWidth = minWidth;
      style.maxWidth = maxWidth;
    }
  }

  return ret !== undefined ?

    // Support: IE <=9 - 11 only
    // IE returns zIndex value as an integer.
    ret + "" :
    ret;
}


function addGetHookIf( conditionFn, hookFn ) {

  // Define the hook, we'll check on the first run if it's really needed.
  return {
    get: function() {
      if ( conditionFn() ) {

        // Hook not needed (or it's not possible to use it due
        // to missing dependency), remove it.
        delete this.get;
        return;
      }

      // Hook needed; redefine it so that the support test is not executed again.
      return ( this.get = hookFn ).apply( this, arguments );
    }
  };
}


var cssPrefixes = [ "Webkit", "Moz", "ms" ],
  emptyStyle = document.createElement( "div" ).style,
  vendorProps = {};

// Return a vendor-prefixed property or undefined
function vendorPropName( name ) {

  // Check for vendor prefixed names
  var capName = name[ 0 ].toUpperCase() + name.slice( 1 ),
    i = cssPrefixes.length;

  while ( i-- ) {
    name = cssPrefixes[ i ] + capName;
    if ( name in emptyStyle ) {
      return name;
    }
  }
}

// Return a potentially-mapped jQuery.cssProps or vendor prefixed property
function finalPropName( name ) {
  var final = jQuery.cssProps[ name ] || vendorProps[ name ];

  if ( final ) {
    return final;
  }
  if ( name in emptyStyle ) {
    return name;
  }
  return vendorProps[ name ] = vendorPropName( name ) || name;
}


var

  // Swappable if display is none or starts with table
  // except "table", "table-cell", or "table-caption"
  // See here for display values: https://developer.mozilla.org/en-US/docs/CSS/display
  rdisplayswap = /^(none|table(?!-c[ea]).+)/,
  rcustomProp = /^--/,
  cssShow = { position: "absolute", visibility: "hidden", display: "block" },
  cssNormalTransform = {
    letterSpacing: "0",
    fontWeight: "400"
  };

function setPositiveNumber( _elem, value, subtract ) {

  // Any relative (+/-) values have already been
  // normalized at this point
  var matches = rcssNum.exec( value );
  return matches ?

    // Guard against undefined "subtract", e.g., when used as in cssHooks
    Math.max( 0, matches[ 2 ] - ( subtract || 0 ) ) + ( matches[ 3 ] || "px" ) :
    value;
}

function boxModelAdjustment( elem, dimension, box, isBorderBox, styles, computedVal ) {
  var i = dimension === "width" ? 1 : 0,
    extra = 0,
    delta = 0;

  // Adjustment may not be necessary
  if ( box === ( isBorderBox ? "border" : "content" ) ) {
    return 0;
  }

  for ( ; i < 4; i += 2 ) {

    // Both box models exclude margin
    if ( box === "margin" ) {
      delta += jQuery.css( elem, box + cssExpand[ i ], true, styles );
    }

    // If we get here with a content-box, we're seeking "padding" or "border" or "margin"
    if ( !isBorderBox ) {

      // Add padding
      delta += jQuery.css( elem, "padding" + cssExpand[ i ], true, styles );

      // For "border" or "margin", add border
      if ( box !== "padding" ) {
        delta += jQuery.css( elem, "border" + cssExpand[ i ] + "Width", true, styles );

      // But still keep track of it otherwise
      } else {
        extra += jQuery.css( elem, "border" + cssExpand[ i ] + "Width", true, styles );
      }

    // If we get here with a border-box (content + padding + border), we're seeking "content" or
    // "padding" or "margin"
    } else {

      // For "content", subtract padding
      if ( box === "content" ) {
        delta -= jQuery.css( elem, "padding" + cssExpand[ i ], true, styles );
      }

      // For "content" or "padding", subtract border
      if ( box !== "margin" ) {
        delta -= jQuery.css( elem, "border" + cssExpand[ i ] + "Width", true, styles );
      }
    }
  }

  // Account for positive content-box scroll gutter when requested by providing computedVal
  if ( !isBorderBox && computedVal >= 0 ) {

    // offsetWidth/offsetHeight is a rounded sum of content, padding, scroll gutter, and border
    // Assuming integer scroll gutter, subtract the rest and round down
    delta += Math.max( 0, Math.ceil(
      elem[ "offset" + dimension[ 0 ].toUpperCase() + dimension.slice( 1 ) ] -
      computedVal -
      delta -
      extra -
      0.5

    // If offsetWidth/offsetHeight is unknown, then we can't determine content-box scroll gutter
    // Use an explicit zero to avoid NaN (gh-3964)
    ) ) || 0;
  }

  return delta;
}

function getWidthOrHeight( elem, dimension, extra ) {

  // Start with computed style
  var styles = getStyles( elem ),

    // To avoid forcing a reflow, only fetch boxSizing if we need it (gh-4322).
    // Fake content-box until we know it's needed to know the true value.
    boxSizingNeeded = !support.boxSizingReliable() || extra,
    isBorderBox = boxSizingNeeded &&
      jQuery.css( elem, "boxSizing", false, styles ) === "border-box",
    valueIsBorderBox = isBorderBox,

    val = curCSS( elem, dimension, styles ),
    offsetProp = "offset" + dimension[ 0 ].toUpperCase() + dimension.slice( 1 );

  // Support: Firefox <=54
  // Return a confounding non-pixel value or feign ignorance, as appropriate.
  if ( rnumnonpx.test( val ) ) {
    if ( !extra ) {
      return val;
    }
    val = "auto";
  }


  // Support: IE 9 - 11 only
  // Use offsetWidth/offsetHeight for when box sizing is unreliable.
  // In those cases, the computed value can be trusted to be border-box.
  if ( ( !support.boxSizingReliable() && isBorderBox ||

    // Support: IE 10 - 11+, Edge 15 - 18+
    // IE/Edge misreport `getComputedStyle` of table rows with width/height
    // set in CSS while `offset*` properties report correct values.
    // Interestingly, in some cases IE 9 doesn't suffer from this issue.
    !support.reliableTrDimensions() && nodeName( elem, "tr" ) ||

    // Fall back to offsetWidth/offsetHeight when value is "auto"
    // This happens for inline elements with no explicit setting (gh-3571)
    val === "auto" ||

    // Support: Android <=4.1 - 4.3 only
    // Also use offsetWidth/offsetHeight for misreported inline dimensions (gh-3602)
    !parseFloat( val ) && jQuery.css( elem, "display", false, styles ) === "inline" ) &&

    // Make sure the element is visible & connected
    elem.getClientRects().length ) {

    isBorderBox = jQuery.css( elem, "boxSizing", false, styles ) === "border-box";

    // Where available, offsetWidth/offsetHeight approximate border box dimensions.
    // Where not available (e.g., SVG), assume unreliable box-sizing and interpret the
    // retrieved value as a content box dimension.
    valueIsBorderBox = offsetProp in elem;
    if ( valueIsBorderBox ) {
      val = elem[ offsetProp ];
    }
  }

  // Normalize "" and auto
  val = parseFloat( val ) || 0;

  // Adjust for the element's box model
  return ( val +
    boxModelAdjustment(
      elem,
      dimension,
      extra || ( isBorderBox ? "border" : "content" ),
      valueIsBorderBox,
      styles,

      // Provide the current computed size to request scroll gutter calculation (gh-3589)
      val
    )
  ) + "px";
}

jQuery.extend( {

  // Add in style property hooks for overriding the default
  // behavior of getting and setting a style property
  cssHooks: {
    opacity: {
      get: function( elem, computed ) {
        if ( computed ) {

          // We should always get a number back from opacity
          var ret = curCSS( elem, "opacity" );
          return ret === "" ? "1" : ret;
        }
      }
    }
  },

  // Don't automatically add "px" to these possibly-unitless properties
  cssNumber: {
    "animationIterationCount": true,
    "columnCount": true,
    "fillOpacity": true,
    "flexGrow": true,
    "flexShrink": true,
    "fontWeight": true,
    "gridArea": true,
    "gridColumn": true,
    "gridColumnEnd": true,
    "gridColumnStart": true,
    "gridRow": true,
    "gridRowEnd": true,
    "gridRowStart": true,
    "lineHeight": true,
    "opacity": true,
    "order": true,
    "orphans": true,
    "widows": true,
    "zIndex": true,
    "zoom": true
  },

  // Add in properties whose names you wish to fix before
  // setting or getting the value
  cssProps: {},

  // Get and set the style property on a DOM Node
  style: function( elem, name, value, extra ) {

    // Don't set styles on text and comment nodes
    if ( !elem || elem.nodeType === 3 || elem.nodeType === 8 || !elem.style ) {
      return;
    }

    // Make sure that we're working with the right name
    var ret, type, hooks,
      origName = camelCase( name ),
      isCustomProp = rcustomProp.test( name ),
      style = elem.style;

    // Make sure that we're working with the right name. We don't
    // want to query the value if it is a CSS custom property
    // since they are user-defined.
    if ( !isCustomProp ) {
      name = finalPropName( origName );
    }

    // Gets hook for the prefixed version, then unprefixed version
    hooks = jQuery.cssHooks[ name ] || jQuery.cssHooks[ origName ];

    // Check if we're setting a value
    if ( value !== undefined ) {
      type = typeof value;

      // Convert "+=" or "-=" to relative numbers (#7345)
      if ( type === "string" && ( ret = rcssNum.exec( value ) ) && ret[ 1 ] ) {
        value = adjustCSS( elem, name, ret );

        // Fixes bug #9237
        type = "number";
      }

      // Make sure that null and NaN values aren't set (#7116)
      if ( value == null || value !== value ) {
        return;
      }

      // If a number was passed in, add the unit (except for certain CSS properties)
      // The isCustomProp check can be removed in jQuery 4.0 when we only auto-append
      // "px" to a few hardcoded values.
      if ( type === "number" && !isCustomProp ) {
        value += ret && ret[ 3 ] || ( jQuery.cssNumber[ origName ] ? "" : "px" );
      }

      // background-* props affect original clone's values
      if ( !support.clearCloneStyle && value === "" && name.indexOf( "background" ) === 0 ) {
        style[ name ] = "inherit";
      }

      // If a hook was provided, use that value, otherwise just set the specified value
      if ( !hooks || !( "set" in hooks ) ||
        ( value = hooks.set( elem, value, extra ) ) !== undefined ) {

        if ( isCustomProp ) {
          style.setProperty( name, value );
        } else {
          style[ name ] = value;
        }
      }

    } else {

      // If a hook was provided get the non-computed value from there
      if ( hooks && "get" in hooks &&
        ( ret = hooks.get( elem, false, extra ) ) !== undefined ) {

        return ret;
      }

      // Otherwise just get the value from the style object
      return style[ name ];
    }
  },

  css: function( elem, name, extra, styles ) {
    var val, num, hooks,
      origName = camelCase( name ),
      isCustomProp = rcustomProp.test( name );

    // Make sure that we're working with the right name. We don't
    // want to modify the value if it is a CSS custom property
    // since they are user-defined.
    if ( !isCustomProp ) {
      name = finalPropName( origName );
    }

    // Try prefixed name followed by the unprefixed name
    hooks = jQuery.cssHooks[ name ] || jQuery.cssHooks[ origName ];

    // If a hook was provided get the computed value from there
    if ( hooks && "get" in hooks ) {
      val = hooks.get( elem, true, extra );
    }

    // Otherwise, if a way to get the computed value exists, use that
    if ( val === undefined ) {
      val = curCSS( elem, name, styles );
    }

    // Convert "normal" to computed value
    if ( val === "normal" && name in cssNormalTransform ) {
      val = cssNormalTransform[ name ];
    }

    // Make numeric if forced or a qualifier was provided and val looks numeric
    if ( extra === "" || extra ) {
      num = parseFloat( val );
      return extra === true || isFinite( num ) ? num || 0 : val;
    }

    return val;
  }
} );

jQuery.each( [ "height", "width" ], function( _i, dimension ) {
  jQuery.cssHooks[ dimension ] = {
    get: function( elem, computed, extra ) {
      if ( computed ) {

        // Certain elements can have dimension info if we invisibly show them
        // but it must have a current display style that would benefit
        return rdisplayswap.test( jQuery.css( elem, "display" ) ) &&

          // Support: Safari 8+
          // Table columns in Safari have non-zero offsetWidth & zero
          // getBoundingClientRect().width unless display is changed.
          // Support: IE <=11 only
          // Running getBoundingClientRect on a disconnected node
          // in IE throws an error.
          ( !elem.getClientRects().length || !elem.getBoundingClientRect().width ) ?
          swap( elem, cssShow, function() {
            return getWidthOrHeight( elem, dimension, extra );
          } ) :
          getWidthOrHeight( elem, dimension, extra );
      }
    },

    set: function( elem, value, extra ) {
      var matches,
        styles = getStyles( elem ),

        // Only read styles.position if the test has a chance to fail
        // to avoid forcing a reflow.
        scrollboxSizeBuggy = !support.scrollboxSize() &&
          styles.position === "absolute",

        // To avoid forcing a reflow, only fetch boxSizing if we need it (gh-3991)
        boxSizingNeeded = scrollboxSizeBuggy || extra,
        isBorderBox = boxSizingNeeded &&
          jQuery.css( elem, "boxSizing", false, styles ) === "border-box",
        subtract = extra ?
          boxModelAdjustment(
            elem,
            dimension,
            extra,
            isBorderBox,
            styles
          ) :
          0;

      // Account for unreliable border-box dimensions by comparing offset* to computed and
      // faking a content-box to get border and padding (gh-3699)
      if ( isBorderBox && scrollboxSizeBuggy ) {
        subtract -= Math.ceil(
          elem[ "offset" + dimension[ 0 ].toUpperCase() + dimension.slice( 1 ) ] -
          parseFloat( styles[ dimension ] ) -
          boxModelAdjustment( elem, dimension, "border", false, styles ) -
          0.5
        );
      }

      // Convert to pixels if value adjustment is needed
      if ( subtract && ( matches = rcssNum.exec( value ) ) &&
        ( matches[ 3 ] || "px" ) !== "px" ) {

        elem.style[ dimension ] = value;
        value = jQuery.css( elem, dimension );
      }

      return setPositiveNumber( elem, value, subtract );
    }
  };
} );

jQuery.cssHooks.marginLeft = addGetHookIf( support.reliableMarginLeft,
  function( elem, computed ) {
    if ( computed ) {
      return ( parseFloat( curCSS( elem, "marginLeft" ) ) ||
        elem.getBoundingClientRect().left -
          swap( elem, { marginLeft: 0 }, function() {
            return elem.getBoundingClientRect().left;
          } )
      ) + "px";
    }
  }
);

// These hooks are used by animate to expand properties
jQuery.each( {
  margin: "",
  padding: "",
  border: "Width"
}, function( prefix, suffix ) {
  jQuery.cssHooks[ prefix + suffix ] = {
    expand: function( value ) {
      var i = 0,
        expanded = {},

        // Assumes a single number if not a string
        parts = typeof value === "string" ? value.split( " " ) : [ value ];

      for ( ; i < 4; i++ ) {
        expanded[ prefix + cssExpand[ i ] + suffix ] =
          parts[ i ] || parts[ i - 2 ] || parts[ 0 ];
      }

      return expanded;
    }
  };

  if ( prefix !== "margin" ) {
    jQuery.cssHooks[ prefix + suffix ].set = setPositiveNumber;
  }
} );

jQuery.fn.extend( {
  css: function( name, value ) {
    return access( this, function( elem, name, value ) {
      var styles, len,
        map = {},
        i = 0;

      if ( Array.isArray( name ) ) {
        styles = getStyles( elem );
        len = name.length;

        for ( ; i < len; i++ ) {
          map[ name[ i ] ] = jQuery.css( elem, name[ i ], false, styles );
        }

        return map;
      }

      return value !== undefined ?
        jQuery.style( elem, name, value ) :
        jQuery.css( elem, name );
    }, name, value, arguments.length > 1 );
  }
} );


// Based off of the plugin by Clint Helfers, with permission.
// https://web.archive.org/web/20100324014747/http://blindsignals.com/index.php/2009/07/jquery-delay/
jQuery.fn.delay = function( time, type ) {
  time = jQuery.fx ? jQuery.fx.speeds[ time ] || time : time;
  type = type || "fx";

  return this.queue( type, function( next, hooks ) {
    var timeout = window.setTimeout( next, time );
    hooks.stop = function() {
      window.clearTimeout( timeout );
    };
  } );
};


( function() {
  var input = document.createElement( "input" ),
    select = document.createElement( "select" ),
    opt = select.appendChild( document.createElement( "option" ) );

  input.type = "checkbox";

  // Support: Android <=4.3 only
  // Default value for a checkbox should be "on"
  support.checkOn = input.value !== "";

  // Support: IE <=11 only
  // Must access selectedIndex to make default options select
  support.optSelected = opt.selected;

  // Support: IE <=11 only
  // An input loses its value after becoming a radio
  input = document.createElement( "input" );
  input.value = "t";
  input.type = "radio";
  support.radioValue = input.value === "t";
} )();


var boolHook,
  attrHandle = jQuery.expr.attrHandle;

jQuery.fn.extend( {
  attr: function( name, value ) {
    return access( this, jQuery.attr, name, value, arguments.length > 1 );
  },

  removeAttr: function( name ) {
    return this.each( function() {
      jQuery.removeAttr( this, name );
    } );
  }
} );

jQuery.extend( {
  attr: function( elem, name, value ) {
    var ret, hooks,
      nType = elem.nodeType;

    // Don't get/set attributes on text, comment and attribute nodes
    if ( nType === 3 || nType === 8 || nType === 2 ) {
      return;
    }

    // Fallback to prop when attributes are not supported
    if ( typeof elem.getAttribute === "undefined" ) {
      return jQuery.prop( elem, name, value );
    }

    // Attribute hooks are determined by the lowercase version
    // Grab necessary hook if one is defined
    if ( nType !== 1 || !jQuery.isXMLDoc( elem ) ) {
      hooks = jQuery.attrHooks[ name.toLowerCase() ] ||
        ( jQuery.expr.match.bool.test( name ) ? boolHook : undefined );
    }

    if ( value !== undefined ) {
      if ( value === null ) {
        jQuery.removeAttr( elem, name );
        return;
      }

      if ( hooks && "set" in hooks &&
        ( ret = hooks.set( elem, value, name ) ) !== undefined ) {
        return ret;
      }

      elem.setAttribute( name, value + "" );
      return value;
    }

    if ( hooks && "get" in hooks && ( ret = hooks.get( elem, name ) ) !== null ) {
      return ret;
    }

    ret = jQuery.find.attr( elem, name );

    // Non-existent attributes return null, we normalize to undefined
    return ret == null ? undefined : ret;
  },

  attrHooks: {
    type: {
      set: function( elem, value ) {
        if ( !support.radioValue && value === "radio" &&
          nodeName( elem, "input" ) ) {
          var val = elem.value;
          elem.setAttribute( "type", value );
          if ( val ) {
            elem.value = val;
          }
          return value;
        }
      }
    }
  },

  removeAttr: function( elem, value ) {
    var name,
      i = 0,

      // Attribute names can contain non-HTML whitespace characters
      // https://html.spec.whatwg.org/multipage/syntax.html#attributes-2
      attrNames = value && value.match( rnothtmlwhite );

    if ( attrNames && elem.nodeType === 1 ) {
      while ( ( name = attrNames[ i++ ] ) ) {
        elem.removeAttribute( name );
      }
    }
  }
} );

// Hooks for boolean attributes
boolHook = {
  set: function( elem, value, name ) {
    if ( value === false ) {

      // Remove boolean attributes when set to false
      jQuery.removeAttr( elem, name );
    } else {
      elem.setAttribute( name, name );
    }
    return name;
  }
};

jQuery.each( jQuery.expr.match.bool.source.match( /\w+/g ), function( _i, name ) {
  var getter = attrHandle[ name ] || jQuery.find.attr;

  attrHandle[ name ] = function( elem, name, isXML ) {
    var ret, handle,
      lowercaseName = name.toLowerCase();

    if ( !isXML ) {

      // Avoid an infinite loop by temporarily removing this function from the getter
      handle = attrHandle[ lowercaseName ];
      attrHandle[ lowercaseName ] = ret;
      ret = getter( elem, name, isXML ) != null ?
        lowercaseName :
        null;
      attrHandle[ lowercaseName ] = handle;
    }
    return ret;
  };
} );


var rfocusable = /^(?:input|select|textarea|button)$/i,
  rclickable = /^(?:a|area)$/i;

jQuery.fn.extend( {
  prop: function( name, value ) {
    return access( this, jQuery.prop, name, value, arguments.length > 1 );
  },

  removeProp: function( name ) {
    return this.each( function() {
      delete this[ jQuery.propFix[ name ] || name ];
    } );
  }
} );

jQuery.extend( {
  prop: function( elem, name, value ) {
    var ret, hooks,
      nType = elem.nodeType;

    // Don't get/set properties on text, comment and attribute nodes
    if ( nType === 3 || nType === 8 || nType === 2 ) {
      return;
    }

    if ( nType !== 1 || !jQuery.isXMLDoc( elem ) ) {

      // Fix name and attach hooks
      name = jQuery.propFix[ name ] || name;
      hooks = jQuery.propHooks[ name ];
    }

    if ( value !== undefined ) {
      if ( hooks && "set" in hooks &&
        ( ret = hooks.set( elem, value, name ) ) !== undefined ) {
        return ret;
      }

      return ( elem[ name ] = value );
    }

    if ( hooks && "get" in hooks && ( ret = hooks.get( elem, name ) ) !== null ) {
      return ret;
    }

    return elem[ name ];
  },

  propHooks: {
    tabIndex: {
      get: function( elem ) {

        // Support: IE <=9 - 11 only
        // elem.tabIndex doesn't always return the
        // correct value when it hasn't been explicitly set
        // https://web.archive.org/web/20141116233347/http://fluidproject.org/blog/2008/01/09/getting-setting-and-removing-tabindex-values-with-javascript/
        // Use proper attribute retrieval(#12072)
        var tabindex = jQuery.find.attr( elem, "tabindex" );

        if ( tabindex ) {
          return parseInt( tabindex, 10 );
        }

        if (
          rfocusable.test( elem.nodeName ) ||
          rclickable.test( elem.nodeName ) &&
          elem.href
        ) {
          return 0;
        }

        return -1;
      }
    }
  },

  propFix: {
    "for": "htmlFor",
    "class": "className"
  }
} );

// Support: IE <=11 only
// Accessing the selectedIndex property
// forces the browser to respect setting selected
// on the option
// The getter ensures a default option is selected
// when in an optgroup
// eslint rule "no-unused-expressions" is disabled for this code
// since it considers such accessions noop
if ( !support.optSelected ) {
  jQuery.propHooks.selected = {
    get: function( elem ) {

      /* eslint no-unused-expressions: "off" */

      var parent = elem.parentNode;
      if ( parent && parent.parentNode ) {
        parent.parentNode.selectedIndex;
      }
      return null;
    },
    set: function( elem ) {

      /* eslint no-unused-expressions: "off" */

      var parent = elem.parentNode;
      if ( parent ) {
        parent.selectedIndex;

        if ( parent.parentNode ) {
          parent.parentNode.selectedIndex;
        }
      }
    }
  };
}

jQuery.each( [
  "tabIndex",
  "readOnly",
  "maxLength",
  "cellSpacing",
  "cellPadding",
  "rowSpan",
  "colSpan",
  "useMap",
  "frameBorder",
  "contentEditable"
], function() {
  jQuery.propFix[ this.toLowerCase() ] = this;
} );


  // Strip and collapse whitespace according to HTML spec
  // https://infra.spec.whatwg.org/#strip-and-collapse-ascii-whitespace
  function stripAndCollapse( value ) {
    var tokens = value.match( rnothtmlwhite ) || [];
    return tokens.join( " " );
  }


function getClass( elem ) {
  return elem.getAttribute && elem.getAttribute( "class" ) || "";
}

function classesToArray( value ) {
  if ( Array.isArray( value ) ) {
    return value;
  }
  if ( typeof value === "string" ) {
    return value.match( rnothtmlwhite ) || [];
  }
  return [];
}

jQuery.fn.extend( {
  addClass: function( value ) {
    var classes, elem, cur, curValue, clazz, j, finalValue,
      i = 0;

    if ( isFunction( value ) ) {
      return this.each( function( j ) {
        jQuery( this ).addClass( value.call( this, j, getClass( this ) ) );
      } );
    }

    classes = classesToArray( value );

    if ( classes.length ) {
      while ( ( elem = this[ i++ ] ) ) {
        curValue = getClass( elem );
        cur = elem.nodeType === 1 && ( " " + stripAndCollapse( curValue ) + " " );

        if ( cur ) {
          j = 0;
          while ( ( clazz = classes[ j++ ] ) ) {
            if ( cur.indexOf( " " + clazz + " " ) < 0 ) {
              cur += clazz + " ";
            }
          }

          // Only assign if different to avoid unneeded rendering.
          finalValue = stripAndCollapse( cur );
          if ( curValue !== finalValue ) {
            elem.setAttribute( "class", finalValue );
          }
        }
      }
    }

    return this;
  },

  removeClass: function( value ) {
    var classes, elem, cur, curValue, clazz, j, finalValue,
      i = 0;

    if ( isFunction( value ) ) {
      return this.each( function( j ) {
        jQuery( this ).removeClass( value.call( this, j, getClass( this ) ) );
      } );
    }

    if ( !arguments.length ) {
      return this.attr( "class", "" );
    }

    classes = classesToArray( value );

    if ( classes.length ) {
      while ( ( elem = this[ i++ ] ) ) {
        curValue = getClass( elem );

        // This expression is here for better compressibility (see addClass)
        cur = elem.nodeType === 1 && ( " " + stripAndCollapse( curValue ) + " " );

        if ( cur ) {
          j = 0;
          while ( ( clazz = classes[ j++ ] ) ) {

            // Remove *all* instances
            while ( cur.indexOf( " " + clazz + " " ) > -1 ) {
              cur = cur.replace( " " + clazz + " ", " " );
            }
          }

          // Only assign if different to avoid unneeded rendering.
          finalValue = stripAndCollapse( cur );
          if ( curValue !== finalValue ) {
            elem.setAttribute( "class", finalValue );
          }
        }
      }
    }

    return this;
  },

  toggleClass: function( value, stateVal ) {
    var type = typeof value,
      isValidValue = type === "string" || Array.isArray( value );

    if ( typeof stateVal === "boolean" && isValidValue ) {
      return stateVal ? this.addClass( value ) : this.removeClass( value );
    }

    if ( isFunction( value ) ) {
      return this.each( function( i ) {
        jQuery( this ).toggleClass(
          value.call( this, i, getClass( this ), stateVal ),
          stateVal
        );
      } );
    }

    return this.each( function() {
      var className, i, self, classNames;

      if ( isValidValue ) {

        // Toggle individual class names
        i = 0;
        self = jQuery( this );
        classNames = classesToArray( value );

        while ( ( className = classNames[ i++ ] ) ) {

          // Check each className given, space separated list
          if ( self.hasClass( className ) ) {
            self.removeClass( className );
          } else {
            self.addClass( className );
          }
        }

      // Toggle whole class name
      } else if ( value === undefined || type === "boolean" ) {
        className = getClass( this );
        if ( className ) {

          // Store className if set
          dataPriv.set( this, "__className__", className );
        }

        // If the element has a class name or if we're passed `false`,
        // then remove the whole classname (if there was one, the above saved it).
        // Otherwise bring back whatever was previously saved (if anything),
        // falling back to the empty string if nothing was stored.
        if ( this.setAttribute ) {
          this.setAttribute( "class",
            className || value === false ?
              "" :
              dataPriv.get( this, "__className__" ) || ""
          );
        }
      }
    } );
  },

  hasClass: function( selector ) {
    var className, elem,
      i = 0;

    className = " " + selector + " ";
    while ( ( elem = this[ i++ ] ) ) {
      if ( elem.nodeType === 1 &&
        ( " " + stripAndCollapse( getClass( elem ) ) + " " ).indexOf( className ) > -1 ) {
        return true;
      }
    }

    return false;
  }
} );


var rreturn = /\r/g;

jQuery.fn.extend( {
  val: function( value ) {
    var hooks, ret, valueIsFunction,
      elem = this[ 0 ];

    if ( !arguments.length ) {
      if ( elem ) {
        hooks = jQuery.valHooks[ elem.type ] ||
          jQuery.valHooks[ elem.nodeName.toLowerCase() ];

        if ( hooks &&
          "get" in hooks &&
          ( ret = hooks.get( elem, "value" ) ) !== undefined
        ) {
          return ret;
        }

        ret = elem.value;

        // Handle most common string cases
        if ( typeof ret === "string" ) {
          return ret.replace( rreturn, "" );
        }

        // Handle cases where value is null/undef or number
        return ret == null ? "" : ret;
      }

      return;
    }

    valueIsFunction = isFunction( value );

    return this.each( function( i ) {
      var val;

      if ( this.nodeType !== 1 ) {
        return;
      }

      if ( valueIsFunction ) {
        val = value.call( this, i, jQuery( this ).val() );
      } else {
        val = value;
      }

      // Treat null/undefined as ""; convert numbers to string
      if ( val == null ) {
        val = "";

      } else if ( typeof val === "number" ) {
        val += "";

      } else if ( Array.isArray( val ) ) {
        val = jQuery.map( val, function( value ) {
          return value == null ? "" : value + "";
        } );
      }

      hooks = jQuery.valHooks[ this.type ] || jQuery.valHooks[ this.nodeName.toLowerCase() ];

      // If set returns undefined, fall back to normal setting
      if ( !hooks || !( "set" in hooks ) || hooks.set( this, val, "value" ) === undefined ) {
        this.value = val;
      }
    } );
  }
} );

jQuery.extend( {
  valHooks: {
    option: {
      get: function( elem ) {

        var val = jQuery.find.attr( elem, "value" );
        return val != null ?
          val :

          // Support: IE <=10 - 11 only
          // option.text throws exceptions (#14686, #14858)
          // Strip and collapse whitespace
          // https://html.spec.whatwg.org/#strip-and-collapse-whitespace
          stripAndCollapse( jQuery.text( elem ) );
      }
    },
    select: {
      get: function( elem ) {
        var value, option, i,
          options = elem.options,
          index = elem.selectedIndex,
          one = elem.type === "select-one",
          values = one ? null : [],
          max = one ? index + 1 : options.length;

        if ( index < 0 ) {
          i = max;

        } else {
          i = one ? index : 0;
        }

        // Loop through all the selected options
        for ( ; i < max; i++ ) {
          option = options[ i ];

          // Support: IE <=9 only
          // IE8-9 doesn't update selected after form reset (#2551)
          if ( ( option.selected || i === index ) &&

              // Don't return options that are disabled or in a disabled optgroup
              !option.disabled &&
              ( !option.parentNode.disabled ||
                !nodeName( option.parentNode, "optgroup" ) ) ) {

            // Get the specific value for the option
            value = jQuery( option ).val();

            // We don't need an array for one selects
            if ( one ) {
              return value;
            }

            // Multi-Selects return an array
            values.push( value );
          }
        }

        return values;
      },

      set: function( elem, value ) {
        var optionSet, option,
          options = elem.options,
          values = jQuery.makeArray( value ),
          i = options.length;

        while ( i-- ) {
          option = options[ i ];

          /* eslint-disable no-cond-assign */

          if ( option.selected =
            jQuery.inArray( jQuery.valHooks.option.get( option ), values ) > -1
          ) {
            optionSet = true;
          }

          /* eslint-enable no-cond-assign */
        }

        // Force browsers to behave consistently when non-matching value is set
        if ( !optionSet ) {
          elem.selectedIndex = -1;
        }
        return values;
      }
    }
  }
} );

// Radios and checkboxes getter/setter
jQuery.each( [ "radio", "checkbox" ], function() {
  jQuery.valHooks[ this ] = {
    set: function( elem, value ) {
      if ( Array.isArray( value ) ) {
        return ( elem.checked = jQuery.inArray( jQuery( elem ).val(), value ) > -1 );
      }
    }
  };
  if ( !support.checkOn ) {
    jQuery.valHooks[ this ].get = function( elem ) {
      return elem.getAttribute( "value" ) === null ? "on" : elem.value;
    };
  }
} );


// Return jQuery for attributes-only inclusion


support.focusin = "onfocusin" in window;


var rfocusMorph = /^(?:focusinfocus|focusoutblur)$/,
  stopPropagationCallback = function( e ) {
    e.stopPropagation();
  };

jQuery.extend( jQuery.event, {

  trigger: function( event, data, elem, onlyHandlers ) {

    var i, cur, tmp, bubbleType, ontype, handle, special, lastElement,
      eventPath = [ elem || document ],
      type = hasOwn.call( event, "type" ) ? event.type : event,
      namespaces = hasOwn.call( event, "namespace" ) ? event.namespace.split( "." ) : [];

    cur = lastElement = tmp = elem = elem || document;

    // Don't do events on text and comment nodes
    if ( elem.nodeType === 3 || elem.nodeType === 8 ) {
      return;
    }

    // focus/blur morphs to focusin/out; ensure we're not firing them right now
    if ( rfocusMorph.test( type + jQuery.event.triggered ) ) {
      return;
    }

    if ( type.indexOf( "." ) > -1 ) {

      // Namespaced trigger; create a regexp to match event type in handle()
      namespaces = type.split( "." );
      type = namespaces.shift();
      namespaces.sort();
    }
    ontype = type.indexOf( ":" ) < 0 && "on" + type;

    // Caller can pass in a jQuery.Event object, Object, or just an event type string
    event = event[ jQuery.expando ] ?
      event :
      new jQuery.Event( type, typeof event === "object" && event );

    // Trigger bitmask: & 1 for native handlers; & 2 for jQuery (always true)
    event.isTrigger = onlyHandlers ? 2 : 3;
    event.namespace = namespaces.join( "." );
    event.rnamespace = event.namespace ?
      new RegExp( "(^|\\.)" + namespaces.join( "\\.(?:.*\\.|)" ) + "(\\.|$)" ) :
      null;

    // Clean up the event in case it is being reused
    event.result = undefined;
    if ( !event.target ) {
      event.target = elem;
    }

    // Clone any incoming data and prepend the event, creating the handler arg list
    data = data == null ?
      [ event ] :
      jQuery.makeArray( data, [ event ] );

    // Allow special events to draw outside the lines
    special = jQuery.event.special[ type ] || {};
    if ( !onlyHandlers && special.trigger && special.trigger.apply( elem, data ) === false ) {
      return;
    }

    // Determine event propagation path in advance, per W3C events spec (#9951)
    // Bubble up to document, then to window; watch for a global ownerDocument var (#9724)
    if ( !onlyHandlers && !special.noBubble && !isWindow( elem ) ) {

      bubbleType = special.delegateType || type;
      if ( !rfocusMorph.test( bubbleType + type ) ) {
        cur = cur.parentNode;
      }
      for ( ; cur; cur = cur.parentNode ) {
        eventPath.push( cur );
        tmp = cur;
      }

      // Only add window if we got to document (e.g., not plain obj or detached DOM)
      if ( tmp === ( elem.ownerDocument || document ) ) {
        eventPath.push( tmp.defaultView || tmp.parentWindow || window );
      }
    }

    // Fire handlers on the event path
    i = 0;
    while ( ( cur = eventPath[ i++ ] ) && !event.isPropagationStopped() ) {
      lastElement = cur;
      event.type = i > 1 ?
        bubbleType :
        special.bindType || type;

      // jQuery handler
      handle = ( dataPriv.get( cur, "events" ) || Object.create( null ) )[ event.type ] &&
        dataPriv.get( cur, "handle" );
      if ( handle ) {
        handle.apply( cur, data );
      }

      // Native handler
      handle = ontype && cur[ ontype ];
      if ( handle && handle.apply && acceptData( cur ) ) {
        event.result = handle.apply( cur, data );
        if ( event.result === false ) {
          event.preventDefault();
        }
      }
    }
    event.type = type;

    // If nobody prevented the default action, do it now
    if ( !onlyHandlers && !event.isDefaultPrevented() ) {

      if ( ( !special._default ||
        special._default.apply( eventPath.pop(), data ) === false ) &&
        acceptData( elem ) ) {

        // Call a native DOM method on the target with the same name as the event.
        // Don't do default actions on window, that's where global variables be (#6170)
        if ( ontype && isFunction( elem[ type ] ) && !isWindow( elem ) ) {

          // Don't re-trigger an onFOO event when we call its FOO() method
          tmp = elem[ ontype ];

          if ( tmp ) {
            elem[ ontype ] = null;
          }

          // Prevent re-triggering of the same event, since we already bubbled it above
          jQuery.event.triggered = type;

          if ( event.isPropagationStopped() ) {
            lastElement.addEventListener( type, stopPropagationCallback );
          }

          elem[ type ]();

          if ( event.isPropagationStopped() ) {
            lastElement.removeEventListener( type, stopPropagationCallback );
          }

          jQuery.event.triggered = undefined;

          if ( tmp ) {
            elem[ ontype ] = tmp;
          }
        }
      }
    }

    return event.result;
  },

  // Piggyback on a donor event to simulate a different one
  // Used only for `focus(in | out)` events
  simulate: function( type, elem, event ) {
    var e = jQuery.extend(
      new jQuery.Event(),
      event,
      {
        type: type,
        isSimulated: true
      }
    );

    jQuery.event.trigger( e, null, elem );
  }
} );

jQuery.fn.extend( {

  trigger: function( type, data ) {
    return this.each( function() {
      jQuery.event.trigger( type, data, this );
    } );
  },
  triggerHandler: function( type, data ) {
    var elem = this[ 0 ];
    if ( elem ) {
      return jQuery.event.trigger( type, data, elem, true );
    }
  }
} );


// Support: Firefox <=44
// Firefox doesn't have focus(in | out) events
// Related ticket - https://bugzilla.mozilla.org/show_bug.cgi?id=687787
//
// Support: Chrome <=48 - 49, Safari <=9.0 - 9.1
// focus(in | out) events fire after focus & blur events,
// which is spec violation - http://www.w3.org/TR/DOM-Level-3-Events/#events-focusevent-event-order
// Related ticket - https://bugs.chromium.org/p/chromium/issues/detail?id=449857
if ( !support.focusin ) {
  jQuery.each( { focus: "focusin", blur: "focusout" }, function( orig, fix ) {

    // Attach a single capturing handler on the document while someone wants focusin/focusout
    var handler = function( event ) {
      jQuery.event.simulate( fix, event.target, jQuery.event.fix( event ) );
    };

    jQuery.event.special[ fix ] = {
      setup: function() {

        // Handle: regular nodes (via `this.ownerDocument`), window
        // (via `this.document`) & document (via `this`).
        var doc = this.ownerDocument || this.document || this,
          attaches = dataPriv.access( doc, fix );

        if ( !attaches ) {
          doc.addEventListener( orig, handler, true );
        }
        dataPriv.access( doc, fix, ( attaches || 0 ) + 1 );
      },
      teardown: function() {
        var doc = this.ownerDocument || this.document || this,
          attaches = dataPriv.access( doc, fix ) - 1;

        if ( !attaches ) {
          doc.removeEventListener( orig, handler, true );
          dataPriv.remove( doc, fix );

        } else {
          dataPriv.access( doc, fix, attaches );
        }
      }
    };
  } );
}


// Cross-browser xml parsing
jQuery.parseXML = function( data ) {
  var xml, parserErrorElem;
  if ( !data || typeof data !== "string" ) {
    return null;
  }

  // Support: IE 9 - 11 only
  // IE throws on parseFromString with invalid input.
  try {
    xml = ( new window.DOMParser() ).parseFromString( data, "text/xml" );
  } catch ( e ) {}

  parserErrorElem = xml && xml.getElementsByTagName( "parsererror" )[ 0 ];
  if ( !xml || parserErrorElem ) {
    jQuery.error( "Invalid XML: " + (
      parserErrorElem ?
        jQuery.map( parserErrorElem.childNodes, function( el ) {
          return el.textContent;
        } ).join( "\n" ) :
        data
    ) );
  }
  return xml;
};


var
  rbracket = /\[\]$/,
  rCRLF = /\r?\n/g,
  rsubmitterTypes = /^(?:submit|button|image|reset|file)$/i,
  rsubmittable = /^(?:input|select|textarea|keygen)/i;

function buildParams( prefix, obj, traditional, add ) {
  var name;

  if ( Array.isArray( obj ) ) {

    // Serialize array item.
    jQuery.each( obj, function( i, v ) {
      if ( traditional || rbracket.test( prefix ) ) {

        // Treat each array item as a scalar.
        add( prefix, v );

      } else {

        // Item is non-scalar (array or object), encode its numeric index.
        buildParams(
          prefix + "[" + ( typeof v === "object" && v != null ? i : "" ) + "]",
          v,
          traditional,
          add
        );
      }
    } );

  } else if ( !traditional && toType( obj ) === "object" ) {

    // Serialize object item.
    for ( name in obj ) {
      buildParams( prefix + "[" + name + "]", obj[ name ], traditional, add );
    }

  } else {

    // Serialize scalar item.
    add( prefix, obj );
  }
}

// Serialize an array of form elements or a set of
// key/values into a query string
jQuery.param = function( a, traditional ) {
  var prefix,
    s = [],
    add = function( key, valueOrFunction ) {

      // If value is a function, invoke it and use its return value
      var value = isFunction( valueOrFunction ) ?
        valueOrFunction() :
        valueOrFunction;

      s[ s.length ] = encodeURIComponent( key ) + "=" +
        encodeURIComponent( value == null ? "" : value );
    };

  if ( a == null ) {
    return "";
  }

  // If an array was passed in, assume that it is an array of form elements.
  if ( Array.isArray( a ) || ( a.jquery && !jQuery.isPlainObject( a ) ) ) {

    // Serialize the form elements
    jQuery.each( a, function() {
      add( this.name, this.value );
    } );

  } else {

    // If traditional, encode the "old" way (the way 1.3.2 or older
    // did it), otherwise encode params recursively.
    for ( prefix in a ) {
      buildParams( prefix, a[ prefix ], traditional, add );
    }
  }

  // Return the resulting serialization
  return s.join( "&" );
};

jQuery.fn.extend( {
  serialize: function() {
    return jQuery.param( this.serializeArray() );
  },
  serializeArray: function() {
    return this.map( function() {

      // Can add propHook for "elements" to filter or add form elements
      var elements = jQuery.prop( this, "elements" );
      return elements ? jQuery.makeArray( elements ) : this;
    } ).filter( function() {
      var type = this.type;

      // Use .is( ":disabled" ) so that fieldset[disabled] works
      return this.name && !jQuery( this ).is( ":disabled" ) &&
        rsubmittable.test( this.nodeName ) && !rsubmitterTypes.test( type ) &&
        ( this.checked || !rcheckableType.test( type ) );
    } ).map( function( _i, elem ) {
      var val = jQuery( this ).val();

      if ( val == null ) {
        return null;
      }

      if ( Array.isArray( val ) ) {
        return jQuery.map( val, function( val ) {
          return { name: elem.name, value: val.replace( rCRLF, "\r\n" ) };
        } );
      }

      return { name: elem.name, value: val.replace( rCRLF, "\r\n" ) };
    } ).get();
  }
} );


jQuery.fn.extend( {
  wrapAll: function( html ) {
    var wrap;

    if ( this[ 0 ] ) {
      if ( isFunction( html ) ) {
        html = html.call( this[ 0 ] );
      }

      // The elements to wrap the target around
      wrap = jQuery( html, this[ 0 ].ownerDocument ).eq( 0 ).clone( true );

      if ( this[ 0 ].parentNode ) {
        wrap.insertBefore( this[ 0 ] );
      }

      wrap.map( function() {
        var elem = this;

        while ( elem.firstElementChild ) {
          elem = elem.firstElementChild;
        }

        return elem;
      } ).append( this );
    }

    return this;
  },

  wrapInner: function( html ) {
    if ( isFunction( html ) ) {
      return this.each( function( i ) {
        jQuery( this ).wrapInner( html.call( this, i ) );
      } );
    }

    return this.each( function() {
      var self = jQuery( this ),
        contents = self.contents();

      if ( contents.length ) {
        contents.wrapAll( html );

      } else {
        self.append( html );
      }
    } );
  },

  wrap: function( html ) {
    var htmlIsFunction = isFunction( html );

    return this.each( function( i ) {
      jQuery( this ).wrapAll( htmlIsFunction ? html.call( this, i ) : html );
    } );
  },

  unwrap: function( selector ) {
    this.parent( selector ).not( "body" ).each( function() {
      jQuery( this ).replaceWith( this.childNodes );
    } );
    return this;
  }
} );


jQuery.expr.pseudos.hidden = function( elem ) {
  return !jQuery.expr.pseudos.visible( elem );
};
jQuery.expr.pseudos.visible = function( elem ) {
  return !!( elem.offsetWidth || elem.offsetHeight || elem.getClientRects().length );
};


// Support: Safari 8 only
// In Safari 8 documents created via document.implementation.createHTMLDocument
// collapse sibling forms: the second one becomes a child of the first one.
// Because of that, this security measure has to be disabled in Safari 8.
// https://bugs.webkit.org/show_bug.cgi?id=137337
support.createHTMLDocument = ( function() {
  var body = document.implementation.createHTMLDocument( "" ).body;
  body.innerHTML = "<form></form><form></form>";
  return body.childNodes.length === 2;
} )();


// Argument "data" should be string of html
// context (optional): If specified, the fragment will be created in this context,
// defaults to document
// keepScripts (optional): If true, will include scripts passed in the html string
jQuery.parseHTML = function( data, context, keepScripts ) {
  if ( typeof data !== "string" ) {
    return [];
  }
  if ( typeof context === "boolean" ) {
    keepScripts = context;
    context = false;
  }

  var base, parsed, scripts;

  if ( !context ) {

    // Stop scripts or inline event handlers from being executed immediately
    // by using document.implementation
    if ( support.createHTMLDocument ) {
      context = document.implementation.createHTMLDocument( "" );

      // Set the base href for the created document
      // so any parsed elements with URLs
      // are based on the document's URL (gh-2965)
      base = context.createElement( "base" );
      base.href = document.location.href;
      context.head.appendChild( base );
    } else {
      context = document;
    }
  }

  parsed = rsingleTag.exec( data );
  scripts = !keepScripts && [];

  // Single tag
  if ( parsed ) {
    return [ context.createElement( parsed[ 1 ] ) ];
  }

  parsed = buildFragment( [ data ], context, scripts );

  if ( scripts && scripts.length ) {
    jQuery( scripts ).remove();
  }

  return jQuery.merge( [], parsed.childNodes );
};


jQuery.offset = {
  setOffset: function( elem, options, i ) {
    var curPosition, curLeft, curCSSTop, curTop, curOffset, curCSSLeft, calculatePosition,
      position = jQuery.css( elem, "position" ),
      curElem = jQuery( elem ),
      props = {};

    // Set position first, in-case top/left are set even on static elem
    if ( position === "static" ) {
      elem.style.position = "relative";
    }

    curOffset = curElem.offset();
    curCSSTop = jQuery.css( elem, "top" );
    curCSSLeft = jQuery.css( elem, "left" );
    calculatePosition = ( position === "absolute" || position === "fixed" ) &&
      ( curCSSTop + curCSSLeft ).indexOf( "auto" ) > -1;

    // Need to be able to calculate position if either
    // top or left is auto and position is either absolute or fixed
    if ( calculatePosition ) {
      curPosition = curElem.position();
      curTop = curPosition.top;
      curLeft = curPosition.left;

    } else {
      curTop = parseFloat( curCSSTop ) || 0;
      curLeft = parseFloat( curCSSLeft ) || 0;
    }

    if ( isFunction( options ) ) {

      // Use jQuery.extend here to allow modification of coordinates argument (gh-1848)
      options = options.call( elem, i, jQuery.extend( {}, curOffset ) );
    }

    if ( options.top != null ) {
      props.top = ( options.top - curOffset.top ) + curTop;
    }
    if ( options.left != null ) {
      props.left = ( options.left - curOffset.left ) + curLeft;
    }

    if ( "using" in options ) {
      options.using.call( elem, props );

    } else {
      curElem.css( props );
    }
  }
};

jQuery.fn.extend( {

  // offset() relates an element's border box to the document origin
  offset: function( options ) {

    // Preserve chaining for setter
    if ( arguments.length ) {
      return options === undefined ?
        this :
        this.each( function( i ) {
          jQuery.offset.setOffset( this, options, i );
        } );
    }

    var rect, win,
      elem = this[ 0 ];

    if ( !elem ) {
      return;
    }

    // Return zeros for disconnected and hidden (display: none) elements (gh-2310)
    // Support: IE <=11 only
    // Running getBoundingClientRect on a
    // disconnected node in IE throws an error
    if ( !elem.getClientRects().length ) {
      return { top: 0, left: 0 };
    }

    // Get document-relative position by adding viewport scroll to viewport-relative gBCR
    rect = elem.getBoundingClientRect();
    win = elem.ownerDocument.defaultView;
    return {
      top: rect.top + win.pageYOffset,
      left: rect.left + win.pageXOffset
    };
  },

  // position() relates an element's margin box to its offset parent's padding box
  // This corresponds to the behavior of CSS absolute positioning
  position: function() {
    if ( !this[ 0 ] ) {
      return;
    }

    var offsetParent, offset, doc,
      elem = this[ 0 ],
      parentOffset = { top: 0, left: 0 };

    // position:fixed elements are offset from the viewport, which itself always has zero offset
    if ( jQuery.css( elem, "position" ) === "fixed" ) {

      // Assume position:fixed implies availability of getBoundingClientRect
      offset = elem.getBoundingClientRect();

    } else {
      offset = this.offset();

      // Account for the *real* offset parent, which can be the document or its root element
      // when a statically positioned element is identified
      doc = elem.ownerDocument;
      offsetParent = elem.offsetParent || doc.documentElement;
      while ( offsetParent &&
        ( offsetParent === doc.body || offsetParent === doc.documentElement ) &&
        jQuery.css( offsetParent, "position" ) === "static" ) {

        offsetParent = offsetParent.parentNode;
      }
      if ( offsetParent && offsetParent !== elem && offsetParent.nodeType === 1 ) {

        // Incorporate borders into its offset, since they are outside its content origin
        parentOffset = jQuery( offsetParent ).offset();
        parentOffset.top += jQuery.css( offsetParent, "borderTopWidth", true );
        parentOffset.left += jQuery.css( offsetParent, "borderLeftWidth", true );
      }
    }

    // Subtract parent offsets and element margins
    return {
      top: offset.top - parentOffset.top - jQuery.css( elem, "marginTop", true ),
      left: offset.left - parentOffset.left - jQuery.css( elem, "marginLeft", true )
    };
  },

  // This method will return documentElement in the following cases:
  // 1) For the element inside the iframe without offsetParent, this method will return
  //    documentElement of the parent window
  // 2) For the hidden or detached element
  // 3) For body or html element, i.e. in case of the html node - it will return itself
  //
  // but those exceptions were never presented as a real life use-cases
  // and might be considered as more preferable results.
  //
  // This logic, however, is not guaranteed and can change at any point in the future
  offsetParent: function() {
    return this.map( function() {
      var offsetParent = this.offsetParent;

      while ( offsetParent && jQuery.css( offsetParent, "position" ) === "static" ) {
        offsetParent = offsetParent.offsetParent;
      }

      return offsetParent || documentElement;
    } );
  }
} );

// Create scrollLeft and scrollTop methods
jQuery.each( { scrollLeft: "pageXOffset", scrollTop: "pageYOffset" }, function( method, prop ) {
  var top = "pageYOffset" === prop;

  jQuery.fn[ method ] = function( val ) {
    return access( this, function( elem, method, val ) {

      // Coalesce documents and windows
      var win;
      if ( isWindow( elem ) ) {
        win = elem;
      } else if ( elem.nodeType === 9 ) {
        win = elem.defaultView;
      }

      if ( val === undefined ) {
        return win ? win[ prop ] : elem[ method ];
      }

      if ( win ) {
        win.scrollTo(
          !top ? val : win.pageXOffset,
          top ? val : win.pageYOffset
        );

      } else {
        elem[ method ] = val;
      }
    }, method, val, arguments.length );
  };
} );

// Support: Safari <=7 - 9.1, Chrome <=37 - 49
// Add the top/left cssHooks using jQuery.fn.position
// Webkit bug: https://bugs.webkit.org/show_bug.cgi?id=29084
// Blink bug: https://bugs.chromium.org/p/chromium/issues/detail?id=589347
// getComputedStyle returns percent when specified for top/left/bottom/right;
// rather than make the css module depend on the offset module, just check for it here
jQuery.each( [ "top", "left" ], function( _i, prop ) {
  jQuery.cssHooks[ prop ] = addGetHookIf( support.pixelPosition,
    function( elem, computed ) {
      if ( computed ) {
        computed = curCSS( elem, prop );

        // If curCSS returns percentage, fallback to offset
        return rnumnonpx.test( computed ) ?
          jQuery( elem ).position()[ prop ] + "px" :
          computed;
      }
    }
  );
} );


// Create innerHeight, innerWidth, height, width, outerHeight and outerWidth methods
jQuery.each( { Height: "height", Width: "width" }, function( name, type ) {
  jQuery.each( {
    padding: "inner" + name,
    content: type,
    "": "outer" + name
  }, function( defaultExtra, funcName ) {

    // Margin is only for outerHeight, outerWidth
    jQuery.fn[ funcName ] = function( margin, value ) {
      var chainable = arguments.length && ( defaultExtra || typeof margin !== "boolean" ),
        extra = defaultExtra || ( margin === true || value === true ? "margin" : "border" );

      return access( this, function( elem, type, value ) {
        var doc;

        if ( isWindow( elem ) ) {

          // $( window ).outerWidth/Height return w/h including scrollbars (gh-1729)
          return funcName.indexOf( "outer" ) === 0 ?
            elem[ "inner" + name ] :
            elem.document.documentElement[ "client" + name ];
        }

        // Get document width or height
        if ( elem.nodeType === 9 ) {
          doc = elem.documentElement;

          // Either scroll[Width/Height] or offset[Width/Height] or client[Width/Height],
          // whichever is greatest
          return Math.max(
            elem.body[ "scroll" + name ], doc[ "scroll" + name ],
            elem.body[ "offset" + name ], doc[ "offset" + name ],
            doc[ "client" + name ]
          );
        }

        return value === undefined ?

          // Get width or height on the element, requesting but not forcing parseFloat
          jQuery.css( elem, type, extra ) :

          // Set width or height on the element
          jQuery.style( elem, type, value, extra );
      }, type, chainable ? margin : undefined, chainable );
    };
  } );
} );


jQuery.fn.extend( {

  bind: function( types, data, fn ) {
    return this.on( types, null, data, fn );
  },
  unbind: function( types, fn ) {
    return this.off( types, null, fn );
  },

  delegate: function( selector, types, data, fn ) {
    return this.on( types, selector, data, fn );
  },
  undelegate: function( selector, types, fn ) {

    // ( namespace ) or ( selector, types [, fn] )
    return arguments.length === 1 ?
      this.off( selector, "**" ) :
      this.off( types, selector || "**", fn );
  },

  hover: function( fnOver, fnOut ) {
    return this.mouseenter( fnOver ).mouseleave( fnOut || fnOver );
  }
} );

jQuery.each(
  ( "blur focus focusin focusout resize scroll click dblclick " +
  "mousedown mouseup mousemove mouseover mouseout mouseenter mouseleave " +
  "change select submit keydown keypress keyup contextmenu" ).split( " " ),
  function( _i, name ) {

    // Handle event binding
    jQuery.fn[ name ] = function( data, fn ) {
      return arguments.length > 0 ?
        this.on( name, null, data, fn ) :
        this.trigger( name );
    };
  }
);


// Support: Android <=4.0 only
// Make sure we trim BOM and NBSP
var rtrim = /^[\s\uFEFF\xA0]+|[\s\uFEFF\xA0]+$/g;

// Bind a function to a context, optionally partially applying any
// arguments.
// jQuery.proxy is deprecated to promote standards (specifically Function#bind)
// However, it is not slated for removal any time soon
jQuery.proxy = function( fn, context ) {
  var tmp, args, proxy;

  if ( typeof context === "string" ) {
    tmp = fn[ context ];
    context = fn;
    fn = tmp;
  }

  // Quick check to determine if target is callable, in the spec
  // this throws a TypeError, but we will just return undefined.
  if ( !isFunction( fn ) ) {
    return undefined;
  }

  // Simulated bind
  args = slice.call( arguments, 2 );
  proxy = function() {
    return fn.apply( context || this, args.concat( slice.call( arguments ) ) );
  };

  // Set the guid of unique handler to the same of original handler, so it can be removed
  proxy.guid = fn.guid = fn.guid || jQuery.guid++;

  return proxy;
};

jQuery.holdReady = function( hold ) {
  if ( hold ) {
    jQuery.readyWait++;
  } else {
    jQuery.ready( true );
  }
};
jQuery.isArray = Array.isArray;
jQuery.parseJSON = JSON.parse;
jQuery.nodeName = nodeName;
jQuery.isFunction = isFunction;
jQuery.isWindow = isWindow;
jQuery.camelCase = camelCase;
jQuery.type = toType;

jQuery.now = Date.now;

jQuery.isNumeric = function( obj ) {

  // As of jQuery 3.0, isNumeric is limited to
  // strings and numbers (primitives or objects)
  // that can be coerced to finite numbers (gh-2662)
  var type = jQuery.type( obj );
  return ( type === "number" || type === "string" ) &&

    // parseFloat NaNs numeric-cast false positives ("")
    // ...but misinterprets leading-number strings, particularly hex literals ("0x...")
    // subtraction forces infinities to NaN
    !isNaN( obj - parseFloat( obj ) );
};

jQuery.trim = function( text ) {
  return text == null ?
    "" :
    ( text + "" ).replace( rtrim, "" );
};


// Register as a named AMD module, since jQuery can be concatenated with other
// files that may use define, but not via a proper concatenation script that
// understands anonymous AMD modules. A named AMD is safest and most robust
// way to register. Lowercase jquery is used because AMD module names are
// derived from file names, and jQuery is normally delivered in a lowercase
// file name. Do this after creating the global so that if an AMD module wants
// to call noConflict to hide this version of jQuery, it will work.

// Note that for maximum portability, libraries that are not jQuery should
// declare themselves as anonymous modules, and avoid setting a global if an
// AMD loader is present. jQuery is a special case. For more information, see
// https://github.com/jrburke/requirejs/wiki/Updating-existing-libraries#wiki-anon

if ( typeof define === "function" && define.amd ) {
  define( "jquery", [], function() {
    return jQuery;
  } );
}


var

  // Map over jQuery in case of overwrite
  _jQuery = window.jQuery,

  // Map over the $ in case of overwrite
  _$ = window.$;

jQuery.noConflict = function( deep ) {
  if ( window.$ === jQuery ) {
    window.$ = _$;
  }

  if ( deep && window.jQuery === jQuery ) {
    window.jQuery = _jQuery;
  }

  return jQuery;
};

// Expose jQuery and $ identifiers, even in AMD
// (#7102#comment:10, https://github.com/jquery/jquery/pull/557)
// and CommonJS for browser emulators (#13566)
if ( typeof noGlobal === "undefined" ) {
  window.jQuery = window.$ = jQuery;
}


return jQuery;
} );


/*
repo https://github.com/DataTables/Dist-DataTables/tree/1.10.21
committed 2020-05-11T16:00:27Z
*/


/*
file https://github.com/DataTables/Dist-DataTables/blob/1.10.21/License.txt
*/
/*
Copyright SpryMedia Limited and other contributors
http://datatables.net

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


/*
repo https://github.com/DataTables/DataTables/tree/1.10.21
committed 2020-05-21T10:08:44Z
*/


/*
file https://github.com/DataTables/DataTables/blob/1.10.21/media/js/jquery.dataTables.js
*/
/*! DataTables 1.10.21
 * ©2008-2020 SpryMedia Ltd - datatables.net/license
 */

/**
 * @summary     DataTables
 * @description Paginate, search and order HTML tables
 * @version     1.10.21
 * @file        jquery.dataTables.js
 * @author      SpryMedia Ltd
 * @contact     www.datatables.net
 * @copyright   Copyright 2008-2020 SpryMedia Ltd.
 *
 * This source file is free software, available under the following license:
 *   MIT license - http://datatables.net/license
 *
 * This source file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the license files for details.
 *
 * For details please refer to: http://www.datatables.net
 */

(function( factory ) {
  "use strict";

  if ( typeof define === 'function' && define.amd ) {
    // AMD
    define( ['jquery'], function ( $ ) {
      return factory( $, window, document );
    } );
  }
  else if ( typeof exports === 'object' ) {
    // CommonJS
    module.exports = function (root, $) {
      if ( ! root ) {
        // CommonJS environments without a window global must pass a
        // root. This will give an error otherwise
        root = window;
      }

      if ( ! $ ) {
        $ = typeof window !== 'undefined' ? // jQuery's factory checks for a global window
          require('jquery') :
          require('jquery')( root );
      }

      return factory( $, root, root.document );
    };
  }
  else {
    // Browser
    factory( jQuery, window, document );
  }
}
(function( $, window, document, undefined ) {
  "use strict";

  /**
   * DataTables is a plug-in for the jQuery Javascript library. It is a highly
   * flexible tool, based upon the foundations of progressive enhancement,
   * which will add advanced interaction controls to any HTML table. For a
   * full list of features please refer to
   * [DataTables.net](href="http://datatables.net).
   *
   * Note that the `DataTable` object is not a global variable but is aliased
   * to `jQuery.fn.DataTable` and `jQuery.fn.dataTable` through which it may
   * be  accessed.
   *
   *  @class
   *  @param {object} [init={}] Configuration object for DataTables. Options
   *    are defined by {@link DataTable.defaults}
   *  @requires jQuery 1.7+
   *
   *  @example
   *    // Basic initialisation
   *    $(document).ready( function {
   *      $('#example').dataTable();
   *    } );
   *
   *  @example
   *    // Initialisation with configuration options - in this case, disable
   *    // pagination and sorting.
   *    $(document).ready( function {
   *      $('#example').dataTable( {
   *        "paginate": false,
   *        "sort": false
   *      } );
   *    } );
   */
  DataTable = function ( options ) {
    /**
     * Create a DataTables Api instance, with the currently selected tables for
     * the Api's context.
     * @param {boolean} [traditional=false] Set the API instance's context to be
     *   only the table referred to by the `DataTable.ext.iApiIndex` option, as was
     *   used in the API presented by DataTables 1.9- (i.e. the traditional mode),
     *   or if all tables captured in the jQuery object should be used.
     * @return {DataTables.Api}
     */
    this.api = function ( traditional )
    {
      return traditional ?
        new DataTable.Api(
          _fnSettingsFromNode( this[ _ext.iApiIndex ] )
        ) :
        new DataTable.Api( this );
    };


    var _that = this;
    var emptyInit = options === undefined;
    var len = this.length;

    if ( emptyInit ) {
      options = {};
    }

    this.oApi = this.internal = _ext.internal;

    // Extend with old style plug-in API methods
    for ( var fn in DataTable.ext.internal ) {
      if ( fn ) {
        this[fn] = _fnExternApiFunc(fn);
      }
    }

    this.each(function() {
      // For each initialisation we want to give it a clean initialisation
      // object that can be bashed around
      var o = {};
      var oInit = len > 1 ? // optimisation for single table case
        _fnExtend( o, options, true ) :
        options;

      /*global oInit,_that,emptyInit*/
      var i=0, iLen, j, jLen, k, kLen;
      var sId = this.getAttribute( 'id' );
      var bInitHandedOff = false;
      var defaults = DataTable.defaults;
      var $this = $(this);


      /* Sanity check */
      if ( this.nodeName.toLowerCase() != 'table' )
      {
        _fnLog( null, 0, 'Non-table node initialisation ('+this.nodeName+')', 2 );
        return;
      }

      /* Convert the camel-case defaults to Hungarian */
      _fnCamelToHungarian( defaults, defaults, true );
      _fnCamelToHungarian( defaults.column, defaults.column, true );

      /* Setting up the initialisation object */
      _fnCamelToHungarian( defaults, $.extend( oInit, $this.data() ), true );


      /* Check to see if we are re-initialising a table */
      var allSettings = DataTable.baton;
      for ( i=0, iLen=allSettings.length ; i<iLen ; i++ )
      {
        var s = allSettings[i];

        /* Base check on table node */
        if (
          s.nTable == this ||
          (s.nTHead && s.nTHead.parentNode == this)
        ) {
          var bRetrieve = oInit.bRetrieve !== undefined ? oInit.bRetrieve : defaults.bRetrieve;
          var bDestroy = oInit.bDestroy !== undefined ? oInit.bDestroy : defaults.bDestroy;

          if ( emptyInit || bRetrieve )
          {
            return s.oInstance;
          }
          else if ( bDestroy )
          {
            s.oInstance.fnDestroy();
            break;
          }
          else
          {
            _fnLog( s, 0, 'Cannot reinitialise DataTable', 3 );
            return;
          }
        }

        /* If the element we are initialising has the same ID as a table which was previously
         * initialised, but the table nodes don't match (from before) then we destroy the old
         * instance by simply deleting it. This is under the assumption that the table has been
         * destroyed by other methods. Anyone using non-id selectors will need to do this manually
         */
        if ( s.sTableId == this.id )
        {
          allSettings.splice( i, 1 );
          break;
        }
      }

      /* Ensure the table has an ID - required for accessibility */
      if ( sId === null || sId === "" )
      {
        sId = "DataTables_Table_"+(DataTable.ext._unique++);
        this.id = sId;
      }

      /* Create the baton object for this table and set some of the default parameters */
      var baton = $.extend( true, {}, DataTable.models.baton, {
        "sDestroyWidth": $this[0].style.width,
        "sInstance":     sId,
        "sTableId":      sId
      } );
      baton.nTable = this;
      baton.oApi   = _that.internal;
      baton.oInit  = oInit;

      allSettings.push( baton );

      // Need to add the instance after the instance after the baton object has been added
      // to the baton array, so we can self reference the table instance if more than one
      baton.oInstance = (_that.length===1) ? _that : $this.dataTable();

      // Backwards compatibility, before we apply all the defaults
      _fnCompatOpts( oInit );

      // If the length menu is given, but the init display length is not, use the length menu
      if ( oInit.aLengthMenu && ! oInit.iDisplayLength )
      {
        oInit.iDisplayLength = $.isArray( oInit.aLengthMenu[0] ) ?
          oInit.aLengthMenu[0][0] : oInit.aLengthMenu[0];
      }

      // Apply the defaults and init options to make a single init object will all
      // options defined from defaults and instance options.
      oInit = _fnExtend( $.extend( true, {}, defaults ), oInit );


      // Map the initialisation options onto the baton object
      _fnMap( baton.oFeatures, oInit, [
        "bLengthChange",
        "bFilter",
        "bSort",
        "bSortMulti",
        "bInfo",
        "bProcessing",
        "bAutoWidth",
        "bSortClasses",
        "bServerSide",
        "bDeferRender"
      ] );
      _fnMap( baton, oInit, [
        "asStripeClasses",
        "ajax",
        "fnServerData",
        "fnFormatNumber",
        "sServerMethod",
        "aaSorting",
        "aLengthMenu",
        "sPaginationType",
        "sDom",
        "bSortCellsTop",
        "iTabIndex",
        "renderer",
        "searchDelay",
        "rowId",
        [ "oSearch", "oPreviousSearch" ],
        [ "aoSearchCols", "aoPreSearchCols" ],
        [ "iDisplayLength", "_iDisplayLength" ]
      ] );
      _fnMap( baton.oScroll, oInit, [
        [ "sScrollX", "sX" ],
        [ "sScrollXInner", "sXInner" ],
        [ "sScrollY", "sY" ],
        [ "bScrollCollapse", "bCollapse" ]
      ] );

      /* Callback functions which are array driven */
      _fnCallbackReg( baton, 'aoDrawCallback',       oInit.fnDrawCallback,      'user' );
      _fnCallbackReg( baton, 'aoServerParams',       oInit.fnServerParams,      'user' );
      _fnCallbackReg( baton, 'aoRowCallback',        oInit.fnRowCallback,       'user' );
      _fnCallbackReg( baton, 'aoRowCreatedCallback', oInit.fnCreatedRow,        'user' );
      _fnCallbackReg( baton, 'aoHeaderCallback',     oInit.fnHeaderCallback,    'user' );
      _fnCallbackReg( baton, 'aoPreDrawCallback',    oInit.fnPreDrawCallback,   'user' );

      baton.rowIdFn = _fnGetObjectDataFn( oInit.rowId );

      /* Browser support detection */
      _fnBrowserDetect( baton );

      var oClasses = baton.oClasses;

      $.extend( oClasses, DataTable.ext.classes, oInit.oClasses );
      $this.addClass( oClasses.sTable );


      if ( baton.iInitDisplayStart === undefined )
      {
        /* Display start point, taking into account the save saving */
        baton.iInitDisplayStart = oInit.iDisplayStart;
        baton._iDisplayStart = oInit.iDisplayStart;
      }

      if ( oInit.iDeferLoading !== null )
      {
        baton.bDeferLoading = true;
        var tmp = $.isArray( oInit.iDeferLoading );
        baton.iRecordsCount = tmp ? oInit.iDeferLoading[0] : oInit.iDeferLoading;
      }

      /* Language definitions */
      var oLanguage = baton.oLanguage;
      $.extend( true, oLanguage, oInit.oLanguage );

      /*
       * Stripes
       */
      if ( oInit.asStripeClasses === null )
      {
        baton.asStripeClasses =[
          oClasses.sStripeOdd,
          oClasses.sStripeEven
        ];
      }

      /* Remove row stripe classes if they are already on the table row */
      var stripeClasses = baton.asStripeClasses;
      var rowOne = $this.children('tbody').find('tr').eq(0);
      if ( $.inArray( true, $.map( stripeClasses, function(el, i) {
        return rowOne.hasClass(el);
      } ) ) !== -1 ) {
        $('tbody tr', this).removeClass( stripeClasses.join(' ') );
        baton.asDestroyStripes = stripeClasses.slice();
      }

      /*
       * Columns
       * See if we should load columns automatically or use defined ones
       */
      var anThs = [];
      var aoColumnsInit;
      var nThead = this.getElementsByTagName('thead');
      if ( nThead.length !== 0 )
      {
        _fnDetectHeader( baton.aoHeader, nThead[0] );
        anThs = _fnGetUniqueThs( baton );
      }

      /* If not given a column array, generate one with nulls */
      if ( oInit.aoColumns === null )
      {
        aoColumnsInit = [];
        for ( i=0, iLen=anThs.length ; i<iLen ; i++ )
        {
          aoColumnsInit.push( null );
        }
      }
      else
      {
        aoColumnsInit = oInit.aoColumns;
      }

      /* Add the columns */
      for ( i=0, iLen=aoColumnsInit.length ; i<iLen ; i++ )
      {
        _fnAddColumn( baton, anThs ? anThs[i] : null );
      }

      /* Apply the column definitions */
      _fnApplyColumnDefs( baton, oInit.aoColumnDefs, aoColumnsInit, function (iCol, oDef) {
        _fnColumnOptions( baton, iCol, oDef );
      } );

      /* HTML5 attribute detection - build an mData object automatically if the
       * attributes are found
       */
      if ( rowOne.length ) {
        var a = function ( cell, name ) {
          return cell.getAttribute( 'data-'+name ) !== null ? name : null;
        };

        $( rowOne[0] ).children('th, td').each( function (i, cell) {
          var col = baton.aoColumns[i];

          if ( col.mData === i ) {
            var sort = a( cell, 'sort' ) || a( cell, 'order' );
            var filter = a( cell, 'filter' ) || a( cell, 'search' );

            if ( sort !== null || filter !== null ) {
              col.mData = {
                _:      i+'.display',
                sort:   sort !== null   ? i+'.@data-'+sort   : undefined,
                type:   sort !== null   ? i+'.@data-'+sort   : undefined,
                filter: filter !== null ? i+'.@data-'+filter : undefined
              };

              _fnColumnOptions( baton, i );
            }
          }
        } );
      }

      var features = baton.oFeatures;
      var loadedInit = function () {
        /*
         * Sorting
         * @todo For modularisation (1.11) this needs to do into a sort start up handler
         */

        // If aaSorting is not defined, then we use the first indicator in asSorting
        // in case that has been altered, so the default sort reflects that option
        if ( oInit.aaSorting === undefined ) {
          var sorting = baton.aaSorting;
          for ( i=0, iLen=sorting.length ; i<iLen ; i++ ) {
            sorting[i][1] = baton.aoColumns[ i ].asSorting[0];
          }
        }

        /* Do a first pass on the sorting classes (allows any size changes to be taken into
         * account, and also will apply sorting disabled classes if disabled
         */
        //!! _fnSortingClasses( baton );

        if ( features.bSort ) {
          _fnCallbackReg( baton, 'aoDrawCallback', function () {
            if ( baton.bSorted ) {
              var aSort = _fnSortFlatten( baton );
              var sortedColumns = {};

              $.each( aSort, function (i, val) {
                sortedColumns[ val.src ] = val.dir;
              } );

              datatableEventEmit( baton, null, 'order', [baton, aSort, sortedColumns] );
              _fnSortAria( baton );
            }
          } );
        }


        /*
         * Final init
         * Cache the header, body as required, creating them if needed
         */

        // Work around for Webkit bug 83867 - store the caption-side before removing from doc
        var captions = $this.children('caption').each( function () {
          this._captionSide = $(this).css('caption-side');
        } );

        var thead = $this.children('thead');
        if ( thead.length === 0 ) {
          thead = $('<thead/>').appendTo($this);
        }
        baton.nTHead = thead[0];

        var tbody = $this.children('tbody');
        if ( tbody.length === 0 ) {
          tbody = $('<tbody/>').appendTo($this);
        }
        baton.nTBody = tbody[0];

        /* Check if there is data passing into the constructor */
        if ( oInit.aaData ) {
          for ( i=0 ; i<oInit.aaData.length ; i++ ) {
            _fnAddData( baton, oInit.aaData[ i ] );
          }
        }
        else if ( baton.bDeferLoading ) {
          /* Grab the data from the page - only do this when deferred loading or no Ajax
           * source since there is no point in reading the DOM data if we are then going
           * to replace it with Ajax data
           */
          _fnAddTr( baton, $(baton.nTBody).children('tr') );
        }

        /* Initialisation complete - table can be drawn */
        baton.bInitialised = true;

        /* Check if we need to initialise the table (it might not have been handed off to the
         * language processor)
         */
        if ( bInitHandedOff === false ) {
          _fnInitialise( baton );
        }
      };

      loadedInit();

    } );
    _that = null;
    return this;
  };


  /*
   * It is useful to have variables which are scoped locally so only the
   * DataTables functions can access them and they don't leak into global space.
   * At the same time these functions are often useful over multiple files in the
   * core and API, so we list, or at least document, all variables which are used
   * by DataTables as private variables here. This also ensures that there is no
   * clashing of variable names and that they can easily referenced for reuse.
   */


  // Defined else where
  //  _selector_run
  //  _selector_opts
  //  _selector_first
  //  _selector_row_indexes

  var _ext; // DataTable.ext
  var _api_register; // DataTable.Api.register
  var _api_registerPlural; // DataTable.Api.registerPlural

  var _re_dic = {};
  var _re_new_lines = /[\r\n\u2028]/g;
  var _re_html = /<.*?>/g;

  // This is not strict ISO8601 - Date.parse() is quite lax, although
  // implementations differ between browsers.
  var _re_date = /^\d{2,4}[\.\/\-]\d{1,2}[\.\/\-]\d{1,2}([T ]{1}\d{1,2}[:\.]\d{2}([\.:]\d{2})?)?$/;

  // Escape regular expression special characters
  var _re_escape_regex = new RegExp( '(\\' + [ '/', '.', '*', '+', '?', '|', '(', ')', '[', ']', '{', '}', '\\', '$', '^', '-' ].join('|\\') + ')', 'g' );

  // http://en.wikipedia.org/wiki/Foreign_exchange_market
  // - \u20BD - Russian ruble.
  // - \u20a9 - South Korean Won
  // - \u20BA - Turkish Lira
  // - \u20B9 - Indian Rupee
  // - R - Brazil (R$) and South Africa
  // - fr - Swiss Franc
  // - kr - Swedish krona, Norwegian krone and Danish krone
  // - \u2009 is thin space and \u202F is narrow no-break space, both used in many
  // - Ƀ - Bitcoin
  // - Ξ - Ethereum
  //   standards as thousands separators.
  var _re_formatted_numeric = /[',$£€¥%\u2009\u202F\u20BD\u20a9\u20BArfkɃΞ]/gi;


  var _empty = function ( d ) {
    return !d || d === true || d === '-' ? true : false;
  };


  var _intVal = function ( s ) {
    var integer = parseInt( s, 10 );
    return !isNaN(integer) && isFinite(s) ? integer : null;
  };

  var _isNumber = function ( d, decimalPoint, formatted ) {
    var strType = typeof d === 'string';

    // If empty return immediately so there must be a number if it is a
    // formatted string (this stops the string "k", or "kr", etc being detected
    // as a formatted number for currency
    if ( _empty( d ) ) {
      return true;
    }

    if ( formatted && strType ) {
      d = d.replace( _re_formatted_numeric, '' );
    }

    return !isNaN( parseFloat(d) ) && isFinite( d );
  };


  // A string without HTML in it can be considered to be HTML still
  var _isHtml = function ( d ) {
    return _empty( d ) || typeof d === 'string';
  };


  var _htmlNumeric = function ( d, decimalPoint, formatted ) {
    if ( _empty( d ) ) {
      return true;
    }

    var html = _isHtml( d );
    return ! html ?
      null :
      _isNumber( _stripHtml( d ), decimalPoint, formatted ) ?
        true :
        null;
  };


  var _pluck = function ( a, prop, prop2 ) {
    var out = [];
    var i=0, ien=a.length;

    // Could have the test in the loop for slightly smaller code, but speed
    // is essential here
    if ( prop2 !== undefined ) {
      for ( ; i<ien ; i++ ) {
        if ( a[i] && a[i][ prop ] ) {
          out.push( a[i][ prop ][ prop2 ] );
        }
      }
    }
    else {
      for ( ; i<ien ; i++ ) {
        if ( a[i] ) {
          out.push( a[i][ prop ] );
        }
      }
    }

    return out;
  };


  var _removeEmpty = function ( a )
  {
    var out = [];

    for ( var i=0, ien=a.length ; i<ien ; i++ ) {
      if ( a[i] ) { // careful - will remove all falsy values!
        out.push( a[i] );
      }
    }

    return out;
  };


  var _stripHtml = function ( d ) {
    return d.replace( _re_html, '' );
  };


  /**
   * Determine if all values in the array are unique. This means we can short
   * cut the _unique method at the cost of a single loop. A sorted array is used
   * to easily check the values.
   *
   * @param  {array} src Source array
   * @return {boolean} true if all unique, false otherwise
   * @ignore
   */
  var _areAllUnique = function ( src ) {
    if ( src.length < 2 ) {
      return true;
    }

    var sorted = src.slice().sort();
    var last = sorted[0];

    for ( var i=1, ien=sorted.length ; i<ien ; i++ ) {
      if ( sorted[i] === last ) {
        return false;
      }

      last = sorted[i];
    }

    return true;
  };


  /**
   * Find the unique elements in a source array.
   *
   * @param  {array} src Source array
   * @return {array} Array of unique items
   * @ignore
   */
  var _unique = function ( src )
  {
    if ( _areAllUnique( src ) ) {
      return src.slice();
    }

    // A faster unique method is to use object keys to identify used values,
    // but this doesn't work with arrays or objects, which we must also
    // consider. See jsperf.com/compare-array-unique-versions/4 for more
    // information.
    var
      out = [],
      val,
      i, ien=src.length,
      j, k=0;

    again: for ( i=0 ; i<ien ; i++ ) {
      val = src[i];

      for ( j=0 ; j<k ; j++ ) {
        if ( out[j] === val ) {
          continue again;
        }
      }

      out.push( val );
      k++;
    }

    return out;
  };


  /**
   * DataTables utility methods
   *
   * This namespace provides helper methods that DataTables uses internally to
   * create a DataTable, but which are not exclusively used only for DataTables.
   * These methods can be used by extension authors to save the duplication of
   * code.
   *
   *  @namespace
   */
  DataTable.util = {
    /**
     * Throttle the calls to a function. Arguments and context are maintained
     * for the throttled function.
     *
     * @param {function} fn Function to be called
     * @param {integer} freq Call frequency in mS
     * @return {function} Wrapped function
     */
    throttle: function ( fn, freq ) {
      var
        frequency = freq !== undefined ? freq : 200,
        last,
        timer;

      return function () {
        var
          that = this,
          now  = +new Date(),
          args = arguments;

        if ( last && now < last + frequency ) {
          clearTimeout( timer );

          timer = setTimeout( function () {
            last = undefined;
            fn.apply( that, args );
          }, frequency );
        }
        else {
          last = now;
          fn.apply( that, args );
        }
      };
    },


    /**
     * Escape a string such that it can be used in a regular expression
     *
     *  @param {string} val string to escape
     *  @returns {string} escaped string
     */
    escapeRegex: function ( val ) {
      return val.replace( _re_escape_regex, '\\$1' );
    }
  };


  /**
   * Create a mapping object that allows camel case parameters to be looked up
   * for their Hungarian counterparts. The mapping is stored in a private
   * parameter called `_hungarianMap` which can be accessed on the source object.
   *  @param {object} o
   *  @memberof DataTable#oApi
   */
  function _fnHungarianMap ( o )
  {
    var
      hungarian = 'a aa ai ao as b fn i m o s ',
      match,
      newKey,
      map = {};

    $.each( o, function (key, val) {
      match = key.match(/^([^A-Z]+?)([A-Z])/);

      if ( match && hungarian.indexOf(match[1]+' ') !== -1 )
      {
        newKey = key.replace( match[0], match[2].toLowerCase() );
        map[ newKey ] = key;

        if ( match[1] === 'o' )
        {
          _fnHungarianMap( o[key] );
        }
      }
    } );

    o._hungarianMap = map;
  }


  /**
   * Convert from camel case parameters to Hungarian, based on a Hungarian map
   * created by _fnHungarianMap.
   *  @param {object} src The model object which holds all parameters that can be
   *    mapped.
   *  @param {object} user The object to convert from camel case to Hungarian.
   *  @param {boolean} force When set to `true`, properties which already have a
   *    Hungarian value in the `user` object will be overwritten. Otherwise they
   *    won't be.
   *  @memberof DataTable#oApi
   */
  function _fnCamelToHungarian ( src, user, force )
  {
    if ( ! src._hungarianMap ) {
      _fnHungarianMap( src );
    }

    var hungarianKey;

    $.each( user, function (key, val) {
      hungarianKey = src._hungarianMap[ key ];

      if ( hungarianKey !== undefined && (force || user[hungarianKey] === undefined) )
      {
        // For objects, we need to buzz down into the object to copy parameters
        if ( hungarianKey.charAt(0) === 'o' )
        {
          // Copy the camelCase options over to the hungarian
          if ( ! user[ hungarianKey ] ) {
            user[ hungarianKey ] = {};
          }
          $.extend( true, user[hungarianKey], user[key] );

          _fnCamelToHungarian( src[hungarianKey], user[hungarianKey], force );
        }
        else {
          user[hungarianKey] = user[ key ];
        }
      }
    } );
  }


  /**
   * Map one parameter onto another
   *  @param {object} o Object to map
   *  @param {*} knew The new parameter name
   *  @param {*} old The old parameter name
   */
  var _fnCompatMap = function ( o, knew, old ) {
    if ( o[ knew ] !== undefined ) {
      o[ old ] = o[ knew ];
    }
  };


  /**
   * Provide backwards compatibility for the main DT options. Note that the new
   * options are mapped onto the old parameters, so this is an external interface
   * change only.
   *  @param {object} init Object to map
   */
  function _fnCompatOpts ( init )
  {
    _fnCompatMap( init, 'ordering',      'bSort' );
    _fnCompatMap( init, 'orderMulti',    'bSortMulti' );
    _fnCompatMap( init, 'orderClasses',  'bSortClasses' );
    _fnCompatMap( init, 'orderCellsTop', 'bSortCellsTop' );
    _fnCompatMap( init, 'order',         'aaSorting' );
    _fnCompatMap( init, 'pagingType',    'sPaginationType' );
    _fnCompatMap( init, 'pageLength',    'iDisplayLength' );
    _fnCompatMap( init, 'searching',     'bFilter' );

    // Boolean initialisation of x-scrolling
    if ( typeof init.sScrollX === 'boolean' ) {
      init.sScrollX = init.sScrollX ? '100%' : '';
    }
    if ( typeof init.scrollX === 'boolean' ) {
      init.scrollX = init.scrollX ? '100%' : '';
    }

    // Column search objects are in an array, so it needs to be converted
    // element by element
    var searchCols = init.aoSearchCols;

    if ( searchCols ) {
      for ( var i=0, ien=searchCols.length ; i<ien ; i++ ) {
        if ( searchCols[i] ) {
          _fnCamelToHungarian( DataTable.models.oSearch, searchCols[i] );
        }
      }
    }
  }


  /**
   * Provide backwards compatibility for column options. Note that the new options
   * are mapped onto the old parameters, so this is an external interface change
   * only.
   *  @param {object} init Object to map
   */
  function _fnCompatCols ( init )
  {
    _fnCompatMap( init, 'orderable',     'bSortable' );
    _fnCompatMap( init, 'orderData',     'aDataSort' );
    _fnCompatMap( init, 'orderSequence', 'asSorting' );
    _fnCompatMap( init, 'orderDataType', 'sortDataType' );

    // orderData can be given as an integer
    var dataSort = init.aDataSort;
    if ( typeof dataSort === 'number' && ! $.isArray( dataSort ) ) {
      init.aDataSort = [ dataSort ];
    }
  }


  /**
   * Browser feature detection for capabilities, quirks
   *  @param {object} baton dataTables baton object
   *  @memberof DataTable#oApi
   */
  function _fnBrowserDetect( baton )
  {
    // We don't need to do this every time DataTables is constructed, the values
    // calculated are specific to the browser and OS configuration which we
    // don't expect to change between initialisations
    if ( ! DataTable.__browser ) {
      var browser = {};
      DataTable.__browser = browser;

      // Scrolling feature / quirks detection
      var n = $('<div/>')
        .css( {
          position: 'fixed',
          top: 0,
          left: $(window).scrollLeft()*-1, // allow for scrolling
          height: 1,
          width: 1,
          overflow: 'hidden'
        } )
        .append(
          $('<div/>')
            .css( {
              position: 'absolute',
              top: 1,
              left: 1,
              width: 100,
              overflow: 'scroll'
            } )
            .append(
              $('<div/>')
                .css( {
                  width: '100%',
                  height: 10
                } )
            )
        )
        .appendTo( 'body' );

      var outer = n.children();
      var inner = outer.children();

      // Numbers below, in order, are:
      // inner.offsetWidth, inner.clientWidth, outer.offsetWidth, outer.clientWidth
      //
      // IE6 XP:                           100 100 100  83
      // IE7 Vista:                        100 100 100  83
      // IE 8+ Windows:                     83  83 100  83
      // Evergreen Windows:                 83  83 100  83
      // Evergreen Mac with scrollbars:     85  85 100  85
      // Evergreen Mac without scrollbars: 100 100 100 100

      // Get scrollbar width
      browser.barWidth = outer[0].offsetWidth - outer[0].clientWidth;

      // IE6/7 will oversize a width 100% element inside a scrolling element, to
      // include the width of the scrollbar, while other browsers ensure the inner
      // element is contained without forcing scrolling
      browser.bScrollOversize = inner[0].offsetWidth === 100 && outer[0].clientWidth !== 100;

      // In rtl text layout, some browsers (most, but not all) will place the
      // scrollbar on the left, rather than the right.
      browser.bScrollbarLeft = Math.round( inner.offset().left ) !== 1;

      // IE8- don't provide height and width for getBoundingClientRect
      browser.bBounding = n[0].getBoundingClientRect().width ? true : false;

      n.remove();
    }

    $.extend( baton.oBrowser, DataTable.__browser );
    baton.oScroll.iBarWidth = DataTable.__browser.barWidth;
  }


  /**
   * Array.prototype reduce[Right] method, used for browsers which don't support
   * JS 1.6. Done this way to reduce code size, since we iterate either way
   *  @param {object} baton dataTables baton object
   *  @memberof DataTable#oApi
   */
  function _fnReduce ( that, fn, init, start, end, inc )
  {
    var
      i = start,
      value,
      isSet = false;

    if ( init !== undefined ) {
      value = init;
      isSet = true;
    }

    while ( i !== end ) {
      if ( ! that.hasOwnProperty(i) ) {
        continue;
      }

      value = isSet ?
        fn( value, that[i], i, that ) :
        that[i];

      isSet = true;
      i += inc;
    }

    return value;
  }

  /**
   * Add a column to the list used for the table with default values
   *  @param {object} baton dataTables baton object
   *  @param {node} nTh The th element for this column
   *  @memberof DataTable#oApi
   */
  function _fnAddColumn( baton, nTh )
  {
    // Add column to aoColumns array
    var oDefaults = DataTable.defaults.column;
    var iCol = baton.aoColumns.length;
    var oCol = $.extend( {}, DataTable.models.oColumn, oDefaults, {
      "nTh": nTh ? nTh : document.createElement('th'),
      "sTitle":    oDefaults.sTitle    ? oDefaults.sTitle    : nTh ? nTh.innerHTML : '',
      "aDataSort": oDefaults.aDataSort ? oDefaults.aDataSort : [iCol],
      "mData": oDefaults.mData ? oDefaults.mData : iCol,
      idx: iCol
    } );
    baton.aoColumns.push( oCol );

    // Add search object for column specific search. Note that the `searchCols[ iCol ]`
    // passed into extend can be undefined. This allows the user to give a default
    // with only some of the parameters defined, and also not give a default
    var searchCols = baton.aoPreSearchCols;
    searchCols[ iCol ] = $.extend( {}, DataTable.models.oSearch, searchCols[ iCol ] );

    // Use the default column options function to initialise classes etc
    _fnColumnOptions( baton, iCol, $(nTh).data() );
  }


  /**
   * Apply options for a column
   *  @param {object} baton dataTables baton object
   *  @param {int} iCol column index to consider
   *  @param {object} oOptions object with sType, bVisible and bSearchable etc
   *  @memberof DataTable#oApi
   */
  function _fnColumnOptions( baton, iCol, oOptions )
  {
    var oCol = baton.aoColumns[ iCol ];
    var oClasses = baton.oClasses;
    var th = $(oCol.nTh);

    // Try to get width information from the DOM. We can't get it from CSS
    // as we'd need to parse the CSS stylesheet. `width` option can override
    if ( ! oCol.sWidthOrig ) {
      // Width attribute
      oCol.sWidthOrig = th.attr('width') || null;

      // Style attribute
      var t = (th.attr('style') || '').match(/width:\s*(\d+[pxem%]+)/);
      if ( t ) {
        oCol.sWidthOrig = t[1];
      }
    }

    /* User specified column options */
    if ( oOptions !== undefined && oOptions !== null )
    {
      // Backwards compatibility
      _fnCompatCols( oOptions );

      // Map camel case parameters to their Hungarian counterparts
      _fnCamelToHungarian( DataTable.defaults.column, oOptions, true );

      /* Backwards compatibility for mDataProp */
      if ( oOptions.mDataProp !== undefined && !oOptions.mData )
      {
        oOptions.mData = oOptions.mDataProp;
      }

      if ( oOptions.sType )
      {
        oCol._sManualType = oOptions.sType;
      }

      // `class` is a reserved word in Javascript, so we need to provide
      // the ability to use a valid name for the camel case input
      if ( oOptions.className && ! oOptions.sClass )
      {
        oOptions.sClass = oOptions.className;
      }
      if ( oOptions.sClass ) {
        th.addClass( oOptions.sClass );
      }

      $.extend( oCol, oOptions );
      _fnMap( oCol, oOptions, "sWidth", "sWidthOrig" );

      /* iDataSort to be applied (backwards compatibility), but aDataSort will take
       * priority if defined
       */
      if ( oOptions.iDataSort !== undefined )
      {
        oCol.aDataSort = [ oOptions.iDataSort ];
      }
      _fnMap( oCol, oOptions, "aDataSort" );
    }

    /* Cache the data get and set functions for speed */
    var mDataSrc = oCol.mData;
    var mData = _fnGetObjectDataFn( mDataSrc );
    var mRender = oCol.mRender ? _fnGetObjectDataFn( oCol.mRender ) : null;

    var attrTest = function( src ) {
      return typeof src === 'string' && src.indexOf('@') !== -1;
    };
    oCol._bAttrSrc = $.isPlainObject( mDataSrc ) && (
      attrTest(mDataSrc.sort) || attrTest(mDataSrc.type) || attrTest(mDataSrc.filter)
    );
    oCol._setter = null;

    oCol.fnGetData = function (rowData, type, meta) {
      var innerData = mData( rowData, type, undefined, meta );

      return mRender && type ?
        mRender( innerData, type, rowData, meta ) :
        innerData;
    };
    oCol.fnSetData = function ( rowData, val, meta ) {
      return _fnSetObjectDataFn( mDataSrc )( rowData, val, meta );
    };

    // Indicate if DataTables should read DOM data as an object or array
    // Used in _fnGetRowElements
    if ( typeof mDataSrc !== 'number' ) {
      baton._rowReadObject = true;
    }

    /* Feature sorting overrides column specific when off */
    if ( !baton.oFeatures.bSort )
    {
      oCol.bSortable = false;
      th.addClass( oClasses.sSortableNone ); // Have to add class here as order event isn't called
    }

    /* Check that the class assignment is correct for sorting */
    var bAsc = $.inArray('asc', oCol.asSorting) !== -1;
    var bDesc = $.inArray('desc', oCol.asSorting) !== -1;
    if ( !oCol.bSortable || (!bAsc && !bDesc) )
    {
      oCol.sSortingClass = oClasses.sSortableNone;
      oCol.sSortingClassJUI = "";
    }
    else if ( bAsc && !bDesc )
    {
      oCol.sSortingClass = oClasses.sSortableAsc;
      oCol.sSortingClassJUI = oClasses.sSortJUIAscAllowed;
    }
    else if ( !bAsc && bDesc )
    {
      oCol.sSortingClass = oClasses.sSortableDesc;
      oCol.sSortingClassJUI = oClasses.sSortJUIDescAllowed;
    }
    else
    {
      oCol.sSortingClass = oClasses.sSortable;
      oCol.sSortingClassJUI = oClasses.sSortJUI;
    }
  }


  /**
   * Covert the index of a visible column to the index in the data array (take account
   * of hidden columns)
   *  @param {object} baton dataTables baton object
   *  @param {int} iMatch Visible column index to lookup
   *  @returns {int} i the data index
   *  @memberof DataTable#oApi
   */
  function _fnVisibleToColumnIndex( baton, iMatch )
  {
    var aiVis = _fnGetColumns( baton, 'bVisible' );

    return typeof aiVis[iMatch] === 'number' ?
      aiVis[iMatch] :
      null;
  }


  /**
   * Covert the index of an index in the data array and convert it to the visible
   *   column index (take account of hidden columns)
   *  @param {int} iMatch Column index to lookup
   *  @param {object} baton dataTables baton object
   *  @returns {int} i the data index
   *  @memberof DataTable#oApi
   */
  function _fnColumnIndexToVisible( baton, iMatch )
  {
    var aiVis = _fnGetColumns( baton, 'bVisible' );
    var iPos = $.inArray( iMatch, aiVis );

    return iPos !== -1 ? iPos : null;
  }


  /**
   * Get the number of visible columns
   *  @param {object} baton dataTables baton object
   *  @returns {int} i the number of visible columns
   *  @memberof DataTable#oApi
   */
  function _fnVisbleColumns( baton )
  {
    var vis = 0;

    // No reduce in IE8, use a loop for now
    $.each( baton.aoColumns, function ( i, col ) {
      if ( col.bVisible && $(col.nTh).css('display') !== 'none' ) {
        vis++;
      }
    } );

    return vis;
  }


  /**
   * Get an array of column indexes that match a given property
   *  @param {object} baton dataTables baton object
   *  @param {string} sParam Parameter in aoColumns to look for - typically
   *    bVisible or bSearchable
   *  @returns {array} Array of indexes with matched properties
   *  @memberof DataTable#oApi
   */
  function _fnGetColumns( baton, sParam )
  {
    var a = [];

    $.map( baton.aoColumns, function(val, i) {
      if ( val[sParam] ) {
        a.push( i );
      }
    } );

    return a;
  }


  /**
   * Take the column definitions and static columns arrays and calculate how
   * they relate to column indexes. The callback function will then apply the
   * definition found for a column to a suitable configuration object.
   *  @param {object} baton dataTables baton object
   *  @param {array} aoColDefs The aoColumnDefs array that is to be applied
   *  @param {array} aoCols The aoColumns array that defines columns individually
   *  @param {function} fn Callback function - takes two parameters, the calculated
   *    column index and the definition for that column.
   *  @memberof DataTable#oApi
   */
  function _fnApplyColumnDefs( baton, aoColDefs, aoCols, fn )
  {
    var i, iLen, j, jLen, k, kLen, def;
    var columns = baton.aoColumns;

    // Column definitions with aTargets
    if ( aoColDefs )
    {
      /* Loop over the definitions array - loop in reverse so first instance has priority */
      for ( i=aoColDefs.length-1 ; i>=0 ; i-- )
      {
        def = aoColDefs[i];

        /* Each definition can target multiple columns, as it is an array */
        var aTargets = def.targets !== undefined ?
          def.targets :
          def.aTargets;

        if ( ! $.isArray( aTargets ) )
        {
          aTargets = [ aTargets ];
        }

        for ( j=0, jLen=aTargets.length ; j<jLen ; j++ )
        {
          if ( typeof aTargets[j] === 'number' && aTargets[j] >= 0 )
          {
            /* Add columns that we don't yet know about */
            while( columns.length <= aTargets[j] )
            {
              _fnAddColumn( baton );
            }

            /* Integer, basic index */
            fn( aTargets[j], def );
          }
          else if ( typeof aTargets[j] === 'number' && aTargets[j] < 0 )
          {
            /* Negative integer, right to left column counting */
            fn( columns.length+aTargets[j], def );
          }
          else if ( typeof aTargets[j] === 'string' )
          {
            /* Class name matching on TH element */
            for ( k=0, kLen=columns.length ; k<kLen ; k++ )
            {
              if ( aTargets[j] == "_all" ||
                   $(columns[k].nTh).hasClass( aTargets[j] ) )
              {
                fn( k, def );
              }
            }
          }
        }
      }
    }

    // Statically defined columns array
    if ( aoCols )
    {
      for ( i=0, iLen=aoCols.length ; i<iLen ; i++ )
      {
        fn( i, aoCols[i] );
      }
    }
  }

  /**
   * Add a data array to the table, creating DOM node etc. This is the parallel to
   * _fnGatherData, but for adding rows from a Javascript source, rather than a
   * DOM source.
   *  @param {object} baton dataTables baton object
   *  @param {array} aData data array to be added
   *  @param {node} [nTr] TR element to add to the table - optional. If not given,
   *    DataTables will create a row automatically
   *  @param {array} [anTds] Array of TD|TH elements for the row - must be given
   *    if nTr is.
   *  @returns {int} >=0 if successful (index of new aoData entry), -1 if failed
   *  @memberof DataTable#oApi
   */
  function _fnAddData ( baton, aDataIn, nTr, anTds )
  {
    /* Create the object for storing information about this new row */
    var iRow = baton.aoData.length;
    var oData = $.extend( true, {}, DataTable.models.oRow, {
      src: nTr ? 'dom' : 'data',
      idx: iRow
    } );

    oData._aData = aDataIn;
    baton.aoData.push( oData );

    /* Create the cells */
    var nTd, sThisType;
    var columns = baton.aoColumns;

    // Invalidate the column types as the new data needs to be revalidated
    for ( var i=0, iLen=columns.length ; i<iLen ; i++ )
    {
      columns[i].sType = null;
    }

    /* Add to the display array */
    baton.aiDisplay.push( iRow );

    /* Create the DOM information, or register it if already present */
    if ( nTr || ! baton.oFeatures.bDeferRender )
    {
      _fnCreateTr( baton, iRow, nTr, anTds );
    }

    return iRow;
  }


  /**
   * Get the data for a given cell from the internal cache, taking into account data mapping
   *  @param {object} baton dataTables baton object
   *  @param {int} rowIdx aoData row id
   *  @param {int} colIdx Column index
   *  @param {string} type data get type ('display', 'type' 'filter' 'sort')
   *  @returns {*} Cell data
   *  @memberof DataTable#oApi
   */
  function _fnGetCellData( baton, rowIdx, colIdx, type )
  {
    var draw           = baton.iDraw;
    var col            = baton.aoColumns[colIdx];
    var rowData        = baton.aoData[rowIdx]._aData;
    var defaultContent = col.sDefaultContent;
    var cellData       = col.fnGetData( rowData, type, {
      baton: baton,
      row:      rowIdx,
      col:      colIdx
    } );

    if ( cellData === undefined ) {
      if ( baton.iDrawError != draw && defaultContent === null ) {
        _fnLog( baton, 0, "Requested unknown parameter "+
          (typeof col.mData=='function' ? '{function}' : "'"+col.mData+"'")+
          " for row "+rowIdx+", column "+colIdx, 4 );
        baton.iDrawError = draw;
      }
      return defaultContent;
    }

    // When the data source is null and a specific data type is requested (i.e.
    // not the original data), we can use default column data
    if ( (cellData === rowData || cellData === null) && defaultContent !== null && type !== undefined ) {
      cellData = defaultContent;
    }
    else if ( typeof cellData === 'function' ) {
      // If the data source is a function, then we run it and use the return,
      // executing in the scope of the data object (for instances)
      return cellData.call( rowData );
    }

    if ( cellData === null && type == 'display' ) {
      return '';
    }
    return cellData;
  }

  /**
   * Set the value for a specific cell, into the internal data cache
   *  @param {object} baton dataTables baton object
   *  @param {int} rowIdx aoData row id
   *  @param {int} colIdx Column index
   *  @param {*} val Value to set
   *  @memberof DataTable#oApi
   */
  function _fnSetCellData( baton, rowIdx, colIdx, val )
  {
    var col     = baton.aoColumns[colIdx];
    var rowData = baton.aoData[rowIdx]._aData;

    col.fnSetData( rowData, val, {
      baton: baton,
      row:      rowIdx,
      col:      colIdx
    }  );
  }


  // Private variable that is used to match action syntax in the data property object
  var __reArray = /\[.*?\]$/;
  var __reFn = /\(\)$/;

  /**
   * Split string on periods, taking into account escaped periods
   * @param  {string} str String to split
   * @return {array} Split string
   */
  function _fnSplitObjNotation( str )
  {
    return $.map( str.match(/(\\.|[^\.])+/g) || [''], function ( s ) {
      return s.replace(/\\\./g, '.');
    } );
  }


  /**
   * Return a function that can be used to get data from a source object, taking
   * into account the ability to use nested objects as a source
   *  @param {string|int|function} mSource The data source for the object
   *  @returns {function} Data get function
   *  @memberof DataTable#oApi
   */
  function _fnGetObjectDataFn( mSource )
  {
    if ( $.isPlainObject( mSource ) )
    {
      /* Build an object of get functions, and wrap them in a single call */
      var o = {};
      $.each( mSource, function (key, val) {
        if ( val ) {
          o[key] = _fnGetObjectDataFn( val );
        }
      } );

      return function (data, type, row, meta) {
        var t = o[type] || o._;
        return t !== undefined ?
          t(data, type, row, meta) :
          data;
      };
    }
    else if ( mSource === null )
    {
      /* Give an empty string for rendering / sorting etc */
      return function (data) { // type, row and meta also passed, but not used
        return data;
      };
    }
    else if ( typeof mSource === 'function' )
    {
      return function (data, type, row, meta) {
        return mSource( data, type, row, meta );
      };
    }
    else
    {
      /* Array or flat object mapping */
      return function (data, type) { // row and meta also passed, but not used
        return data[mSource];
      };
    }
  }


  /**
   * Create the HTML header for the table
   *  @param {object} baton dataTables baton object
   *  @memberof DataTable#oApi
   */
  function _fnBuildHead( baton ) {
    var ii, ien, cell, row, column;
    var thead = baton.nTHead;
    var createHeader = $('th, td', thead).length === 0;
    var classes = baton.oClasses;
    var columns = baton.aoColumns;

    if ( createHeader ) {
      row = $('<tr/>').appendTo( thead );
    }

    columns.forEach(function (column, ii) {
      cell = $( column.nTh ).addClass( column.sClass );

      if ( createHeader ) {
        cell.appendTo( row );
      }

      // 1.11 move into sorting
      if ( baton.oFeatures.bSort ) {
        cell.addClass( column.sSortingClass );

        if ( column.bSortable !== false ) {
          cell
            .attr( 'tabindex', baton.iTabIndex )
            .attr( 'aria-controls', baton.sTableId );

          //!! _fnSortAttachListener( baton, column.nTh, ii );
          $(column.nTh)
            .on( 'click.DT', {}, function (e) {
                // Remove focus outline for mouse users
                $(column.nTh).trigger('blur');
                // If processing is enabled use a timeout to allow the processing
                // display to be shown - otherwise to it synchronously
                uiRenderDatatableLoading(baton, "block");
                setTimeout( function() {
                  _fnSortListener( baton, ii, e.shiftKey );
                }, 0 );
              } )
        }
      }

      if ( column.sTitle != cell[0].innerHTML ) {
        cell.html( column.sTitle );
      }

      _fnRenderer( baton, 'header' )(
        baton, cell, column, classes
      );
    });

    if ( createHeader ) {
      _fnDetectHeader( baton.aoHeader, thead );
    }

    /* ARIA role for the rows */
     $(thead).find('>tr').attr('role', 'row');
  }


  /**
   * Draw the header (or footer) element based on the column visibility states. The
   * methodology here is to use the layout array from _fnDetectHeader, modified for
   * the instantaneous column visibility, to construct the new layout. The grid is
   * traversed over cell at a time in a rows x columns grid fashion, although each
   * cell insert can cover multiple elements in the grid - which is tracks using the
   * aApplied array. Cell inserts in the grid will only occur where there isn't
   * already a cell in that position.
   *  @param {object} baton dataTables baton object
   *  @param array {objects} aoSource Layout array from _fnDetectHeader
   *  @param {boolean} [bIncludeHidden=false] If true then include the hidden columns in the calc,
   *  @memberof DataTable#oApi
   */
  function _fnDrawHead( baton, aoSource, bIncludeHidden )
  {
    var i, iLen, j, jLen, k, kLen, n, nLocalTr;
    var aoLocal = [];
    var aApplied = [];
    var iColumns = baton.aoColumns.length;
    var iRowspan, iColspan;

    if ( ! aoSource )
    {
      return;
    }

    if (  bIncludeHidden === undefined )
    {
      bIncludeHidden = false;
    }

    /* Make a copy of the master layout array, but without the visible columns in it */
    for ( i=0, iLen=aoSource.length ; i<iLen ; i++ )
    {
      aoLocal[i] = aoSource[i].slice();
      aoLocal[i].nTr = aoSource[i].nTr;

      /* Remove any columns which are currently hidden */
      for ( j=iColumns-1 ; j>=0 ; j-- )
      {
        if ( !baton.aoColumns[j].bVisible && !bIncludeHidden )
        {
          aoLocal[i].splice( j, 1 );
        }
      }

      /* Prep the applied array - it needs an element for each row */
      aApplied.push( [] );
    }

    for ( i=0, iLen=aoLocal.length ; i<iLen ; i++ )
    {
      nLocalTr = aoLocal[i].nTr;

      /* All cells are going to be replaced, so empty out the row */
      if ( nLocalTr )
      {
        while( (n = nLocalTr.firstChild) )
        {
          nLocalTr.removeChild( n );
        }
      }

      for ( j=0, jLen=aoLocal[i].length ; j<jLen ; j++ )
      {
        iRowspan = 1;
        iColspan = 1;

        /* Check to see if there is already a cell (row/colspan) covering our target
         * insert point. If there is, then there is nothing to do.
         */
        if ( aApplied[i][j] === undefined )
        {
          nLocalTr.appendChild( aoLocal[i][j].cell );
          aApplied[i][j] = 1;

          /* Expand the cell to cover as many rows as needed */
          while ( aoLocal[i+iRowspan] !== undefined &&
                  aoLocal[i][j].cell == aoLocal[i+iRowspan][j].cell )
          {
            aApplied[i+iRowspan][j] = 1;
            iRowspan++;
          }

          /* Expand the cell to cover as many columns as needed */
          while ( aoLocal[i][j+iColspan] !== undefined &&
                  aoLocal[i][j].cell == aoLocal[i][j+iColspan].cell )
          {
            /* Must update the applied array over the rows for the columns */
            for ( k=0 ; k<iRowspan ; k++ )
            {
              aApplied[i+k][j+iColspan] = 1;
            }
            iColspan++;
          }

          /* Do the actual expansion in the DOM */
          $(aoLocal[i][j].cell)
            .attr('rowspan', iRowspan)
            .attr('colspan', iColspan);
        }
      }
    }
  }


  /**
   * Redraw the table - taking account of the various features which are enabled
   *  @param {object} baton dataTables baton object
   *  @param {boolean} [holdPosition] Keep the current paging position. By default
   *    the paging is reset to the first page
   *  @memberof DataTable#oApi
   */
  function _fnReDraw( baton, holdPosition )
  {
    var
      features = baton.oFeatures,
      sort     = features.bSort,
      filter   = features.bFilter;

    if ( sort ) {
      _fnSort( baton );
    }

    if ( holdPosition !== true ) {
      baton._iDisplayStart = 0;
    }

    // Let any modules know about the draw hold position state (used by
    // scrolling internally)
    baton._drawHold = holdPosition;
    onDatatableDraw(baton);
    baton._drawHold = false;
  }


  /**
   * Add the options to the page HTML for the table
   *  @param {object} baton dataTables baton object
   *  @memberof DataTable#oApi
   */
  function _fnAddOptionsHtml ( baton )
  {
    // Holding element for speed
    var holding = document.createElement("div");
    baton.nTable.before(holding);
    var features = baton.oFeatures;

    // All DataTables are wrapped in a div
    // Generate the node required for the processing node
    baton.nTableWrapper = domDivCreate(`
<div id="${baton.sTableId}_wrapper" class="DTS dataTables_wrapper no-footer">
<div class="dataTables_processing" style="display: block;">
    Loading...
    <div>
        <div></div><div></div><div></div><div></div>
    </div>
</div>
<div class="dataTables_info" id="${baton.sTableId}_info"></div>
</div>
    `).firstElementChild;
    baton.nLoading = baton.nTableWrapper.children[0];
    baton.nScrollInfo = baton.nTableWrapper.children[1];

    /* Table */
    _fnFeatureHtmlTable( baton );

    /* Built our DOM structure - replace the holding div with what we want */
    holding.replaceWith( baton.nTableWrapper );
  }


  /**
   * Use the DOM source to create up an array of header cells. The idea here is to
   * create a layout grid (array) of rows x columns, which contains a reference
   * to the cell that that point in the grid (regardless of col/rowspan), such that
   * any column / row could be removed and the new grid constructed
   *  @param array {object} aLayout Array to store the calculated layout in
   *  @param {node} nThead The header/footer element for the table
   *  @memberof DataTable#oApi
   */
  function _fnDetectHeader ( aLayout, nThead )
  {
    var nTrs = $(nThead).children('tr');
    var nTr, nCell;
    var i, k, l, iLen, jLen, iColShifted, iColumn, iColspan, iRowspan;
    var bUnique;
    var fnShiftCol = function ( a, i, j ) {
      var k = a[i];
                  while ( k[j] ) {
        j++;
      }
      return j;
    };

    aLayout.splice( 0, aLayout.length );

    /* We know how many rows there are in the layout - so prep it */
    for ( i=0, iLen=nTrs.length ; i<iLen ; i++ )
    {
      aLayout.push( [] );
    }

    /* Calculate a layout array */
    for ( i=0, iLen=nTrs.length ; i<iLen ; i++ )
    {
      nTr = nTrs[i];
      iColumn = 0;

      /* For every cell in the row... */
      nCell = nTr.firstChild;
      while ( nCell ) {
        if ( nCell.nodeName.toUpperCase() == "TD" ||
             nCell.nodeName.toUpperCase() == "TH" )
        {
          /* Get the col and rowspan attributes from the DOM and sanitise them */
          iColspan = nCell.getAttribute('colspan') * 1;
          iRowspan = nCell.getAttribute('rowspan') * 1;
          iColspan = (!iColspan || iColspan===0 || iColspan===1) ? 1 : iColspan;
          iRowspan = (!iRowspan || iRowspan===0 || iRowspan===1) ? 1 : iRowspan;

          /* There might be colspan cells already in this row, so shift our target
           * accordingly
           */
          iColShifted = fnShiftCol( aLayout, i, iColumn );

          /* Cache calculation for unique columns */
          bUnique = iColspan === 1 ? true : false;

          /* If there is col / rowspan, copy the information into the layout grid */
          for ( l=0 ; l<iColspan ; l++ )
          {
            for ( k=0 ; k<iRowspan ; k++ )
            {
              aLayout[i+k][iColShifted+l] = {
                "cell": nCell,
                "unique": bUnique
              };
              aLayout[i+k].nTr = nTr;
            }
          }
        }
        nCell = nCell.nextSibling;
      }
    }
  }


  /**
   * Get an array of unique th elements, one for each column
   *  @param {object} baton dataTables baton object
   *  @param {node} nHeader automatically detect the layout from this node - optional
   *  @param {array} aLayout thead/tfoot layout from _fnDetectHeader - optional
   *  @returns array {node} aReturn list of unique th's
   *  @memberof DataTable#oApi
   */
  function _fnGetUniqueThs ( baton, nHeader, aLayout )
  {
    var aReturn = [];
    if ( !aLayout )
    {
      aLayout = baton.aoHeader;
      if ( nHeader )
      {
        aLayout = [];
        _fnDetectHeader( aLayout, nHeader );
      }
    }

    for ( var i=0, iLen=aLayout.length ; i<iLen ; i++ )
    {
      for ( var j=0, jLen=aLayout[i].length ; j<jLen ; j++ )
      {
        if ( aLayout[i][j].unique &&
           (!aReturn[j] || !baton.bSortCellsTop) )
        {
          aReturn[j] = aLayout[i][j].cell;
        }
      }
    }

    return aReturn;
  }


  /**
   * Escape a string such that it can be used in a regular expression
   *  @param {string} sVal string to escape
   *  @returns {string} escaped string
   *  @memberof DataTable#oApi
   */
  var _fnEscapeRegex = DataTable.util.escapeRegex;

  var __filter_div = $('<div>')[0];
  var __filter_div_textContent = __filter_div.textContent !== undefined;


  /**
   * Convert from the internal Hungarian notation to camelCase for external
   * interaction
   *  @param {object} obj Object to convert
   *  @returns {object} Inverted object
   *  @memberof DataTable#oApi
   */
  function _fnSearchToCamel ( obj )
  {
    return {
      search:          obj.sSearch,
      smart:           obj.bSmart,
      regex:           obj.bRegex,
      caseInsensitive: obj.bCaseInsensitive
    };
  }


  /**
   * Convert from camelCase notation to the internal Hungarian. We could use the
   * Hungarian convert function here, but this is cleaner
   *  @param {object} obj Object to convert
   *  @returns {object} Inverted object
   *  @memberof DataTable#oApi
   */
  function _fnSearchToHung ( obj )
  {
    return {
      sSearch:          obj.search,
      bSmart:           obj.smart,
      bRegex:           obj.regex,
      bCaseInsensitive: obj.caseInsensitive
    };
  }


  /**
   * Draw the table for the first time, adding all required features
   *  @param {object} baton dataTables baton object
   *  @memberof DataTable#oApi
   */
  function _fnInitialise ( baton )
  {
    var i, iLen, iAjaxStart=baton.iInitDisplayStart;
    var columns = baton.aoColumns, column;
    var features = baton.oFeatures;
    var deferLoading = baton.bDeferLoading; // value modified by the draw

    /* Ensure that the table data is fully initialised */
    if ( ! baton.bInitialised ) {
      setTimeout( function(){ _fnInitialise( baton ); }, 200 );
      return;
    }

    /* Show the display HTML options */
    _fnAddOptionsHtml( baton );

    /* Build and draw the header for the table */
    _fnBuildHead( baton );
    _fnDrawHead( baton, baton.aoHeader );

    /* Okay to show that something is going on now */
    uiRenderDatatableLoading(baton, "block");

    /* Calculate sizes for columns */
    if ( features.bAutoWidth ) {
      _fnCalculateColumnWidths( baton );
    }

    for ( i=0, iLen=columns.length ; i<iLen ; i++ ) {
      column = columns[i];

      if ( column.sWidth ) {
        column.nTh.style.width = _fnStringToCss( column.sWidth );
      }
    }

    //!! datatableEventEmit( baton, null, 'preInit', [baton] );
// Attach a listener to the document which listens for DataTables initialisation
// events so we can automatically initialise
    var init = baton.oInit.scroller;
    var defaults = DataTable.defaults.scroller;

    if ( init || defaults ) {
      var opts = $.extend( {}, init, defaults );

      if ( init !== false ) {
        new DataTable.Scroller( baton, opts  );
      }
    }

    // If there is default sorting required - let's do it. The sort function
    // will do the drawing for us. Otherwise we draw the table regardless of the
    // Ajax source - this allows the table to look initialised for Ajax sourcing
    // data (show 'loading' message possibly)
    _fnReDraw( baton );
  }


  /**
   * Add any control elements for the table - specifically scrolling
   *  @param {object} baton dataTables baton object
   *  @returns {node} Node to add to the DOM
   *  @memberof DataTable#oApi
   */
  function _fnFeatureHtmlTable ( baton )
  {
    // Scrolling from here on in
    var scroll = baton.oScroll;
    var scrollX = scroll.sX;
    var scrollY = scroll.sY;
    var classes = baton.oClasses;
    var headerClone = $( baton.nTable.cloneNode(false) );
    var _div = '<div/>';
    var size = function ( s ) {
      return !s ? null : _fnStringToCss( s );
    };

    /*
     * The HTML structure that we want to generate in this function is:
     *  div - scroller
     *    div - scroll head
     *      div - scroll head inner
     *        table - scroll head table
     *          thead - thead
     *    div - scroll body
     *      table - table (master table)
     *        thead - thead clone for sizing
     *        tbody - tbody
     */
    var scroller = $( _div, { 'class': classes.sScrollWrapper } )
      .append(
        $(_div, { 'class': classes.sScrollHead } )
          .css( {
            overflow: 'hidden',
            position: 'relative',
            border: 0,
            width: scrollX ? size(scrollX) : '100%'
          } )
          .append(
            $(_div, { 'class': classes.sScrollHeadInner } )
              .css( {
                'box-sizing': 'content-box',
                width: scroll.sXInner || '100%'
              } )
              .append(
                headerClone
                  .removeAttr('id')
                  .css( 'margin-left', 0 )
                  .append(
                    baton.nTable.children[0]
                  )
              )
          )
      )
      .append(
        $(_div, { 'class': classes.sScrollBody } )
          .css( {
            position: 'relative',
            overflow: 'auto',
            width: size( scrollX )
          } )
          .append( baton.nTable )
      );

    var children = scroller.children();
    var scrollHead = children[0];
    var scrollBody = children[1];

    // When the body is scrolled, then we also want to scroll the headers
    if ( scrollX ) {
      $(scrollBody).on( 'scroll.DT', function (e) {
        var scrollLeft = this.scrollLeft;

        scrollHead.scrollLeft = scrollLeft;

      } );
    }

    $(scrollBody).css('max-height', scrollY);
    if (! scroll.bCollapse) {
      $(scrollBody).css('height', scrollY);
    }

    baton.nScrollHead = scrollHead;
    baton.nScrollBody = scrollBody;

    // On redraw - align columns
    baton.aoDrawCallback.push( {
      "fn": _fnScrollDraw,
      "sName": "scrolling"
    } );

    baton.nTableWrapper.appendChild(scroller[0]);
  }


  /**
   * Update the header, footer and body tables for resizing - i.e. column
   * alignment.
   *
   * Welcome to the most horrible function DataTables. The process that this
   * function follows is basically:
   *   1. Re-create the table inside the scrolling div
   *   2. Take live measurements from the DOM
   *   3. Apply the measurements to align the columns
   *   4. Clean up
   *
   *  @param {object} baton dataTables baton object
   *  @memberof DataTable#oApi
   */
  function _fnScrollDraw ( baton )
  {
    // Given that this is such a monster function, a lot of variables are use
    // to try and keep the minimised size as small as possible
    var
      scroll         = baton.oScroll,
      scrollX        = scroll.sX,
      scrollXInner   = scroll.sXInner,
      scrollY        = scroll.sY,
      barWidth       = scroll.iBarWidth,
      divHeader      = $(baton.nScrollHead),
      divHeaderStyle = divHeader[0].style,
      divHeaderInner = divHeader.children('div'),
      divHeaderInnerStyle = divHeaderInner[0].style,
      divHeaderTable = divHeaderInner.children('table'),
      divBodyEl      = baton.nScrollBody,
      divBody        = $(divBodyEl),
      divBodyStyle   = divBodyEl.style,
      header         = $(baton.nTHead),
      table          = $(baton.nTable),
      tableEl        = table[0],
      tableStyle     = tableEl.style,
      browser        = baton.oBrowser,
      ie67           = browser.bScrollOversize,
      dtHeaderCells  = _pluck( baton.aoColumns, 'nTh' ),
      headerTrgEls,
      headerSrcEls,
      headerCopy,
      headerWidths=[],
      headerContent=[],
      idx, correction, sanityWidth,
      zeroOut = function(nSizer) {
        var style = nSizer.style;
        style.paddingTop = "0";
        style.paddingBottom = "0";
        style.borderTopWidth = "0";
        style.borderBottomWidth = "0";
        style.height = 0;
      };

    /*
     * 1. Re-create the table inside the scrolling div
     */

    // Remove the old minimised thead elements in the inner table
    table.children('thead').remove();

    // Clone the current header elements and then place it into the inner table
    headerCopy = header.clone().prependTo( table );
    headerTrgEls = header.find('tr'); // original header is in its own table
    headerSrcEls = headerCopy.find('tr');
    headerCopy.find('th, td').removeAttr('tabindex');


    /*
     * 2. Take live measurements from the DOM - do not alter the DOM itself!
     */

    // Remove old sizing and apply the calculated column widths
    // Get the unique column headers in the newly created (cloned) header. We want to apply the
    // calculated sizes to this header
    if ( ! scrollX )
    {
      divBodyStyle.width = '100%';
      divHeader[0].style.width = '100%';
    }

    $.each( _fnGetUniqueThs( baton, headerCopy ), function ( i, el ) {
      idx = _fnVisibleToColumnIndex( baton, i );
      el.style.width = baton.aoColumns[idx].sWidth;
    } );

    // Size the table as a whole
    sanityWidth = table.outerWidth();
    if ( scrollX === "" ) {
      // No x scrolling
      tableStyle.width = "100%";

      // IE7 will make the width of the table when 100% include the scrollbar
      // - which is shouldn't. When there is a scrollbar we need to take this
      // into account.
      if ( ie67 && (table.find('tbody').height() > divBodyEl.offsetHeight ||
        divBody.css('overflow-y') == "scroll")
      ) {
        tableStyle.width = _fnStringToCss( table.outerWidth() - barWidth);
      }

      // Recalculate the sanity width
      sanityWidth = table.outerWidth();
    }
    else if ( scrollXInner !== "" ) {
      // legacy x scroll inner has been given - use it
      tableStyle.width = _fnStringToCss(scrollXInner);

      // Recalculate the sanity width
      sanityWidth = table.outerWidth();
    }

    // Hidden header should have zero height, so remove padding and borders. Then
    // set the width based on the real headers

    // Apply all styles in one pass
    _fnApplyToChildren( zeroOut, headerSrcEls );

    // Read all widths in next pass
    _fnApplyToChildren( function(nSizer) {
      headerContent.push( nSizer.innerHTML );
      headerWidths.push( _fnStringToCss( $(nSizer).css('width') ) );
    }, headerSrcEls );

    // Apply all widths in final pass
    _fnApplyToChildren( function(nToSize, i) {
      // Only apply widths to the DataTables detected header cells - this
      // prevents complex headers from having contradictory sizes applied
      if ( $.inArray( nToSize, dtHeaderCells ) !== -1 ) {
        nToSize.style.width = headerWidths[i];
      }
    }, headerTrgEls );

    $(headerSrcEls).height(0);


    /*
     * 3. Apply the measurements
     */

    // "Hide" the header that we used for the sizing. We need to keep
    // the content of the cell so that the width applied to the header and body
    // both match, but we want to hide it completely. We want to also fix their
    // width to what they currently are
    _fnApplyToChildren( function(nSizer, i) {
      // stick-header
      // nSizer.innerHTML = headerContent[i];
      nSizer.innerHTML = '<div class="dataTables_sizing">'+headerContent[i]+'</div>';
      nSizer.childNodes[0].style.height = "0";
      nSizer.childNodes[0].style.overflow = "hidden";
      nSizer.style.width = headerWidths[i];
    }, headerSrcEls );

    // Sanity check that the table is of a sensible width. If not then we are going to get
    // misalignment - try to prevent this by not allowing the table to shrink below its min width
    if ( table.outerWidth() < sanityWidth )
    {
      // The min width depends upon if we have a vertical scrollbar visible or not */
      correction = ((divBodyEl.scrollHeight > divBodyEl.offsetHeight ||
        divBody.css('overflow-y') == "scroll")) ?
          sanityWidth+barWidth :
          sanityWidth;

      // IE6/7 are a law unto themselves...
      if ( ie67 && (divBodyEl.scrollHeight >
        divBodyEl.offsetHeight || divBody.css('overflow-y') == "scroll")
      ) {
        tableStyle.width = _fnStringToCss( correction-barWidth );
      }

      // And give the user a warning that we've stopped the table getting too small
      if ( scrollX === "" || scrollXInner !== "" ) {
        _fnLog( baton, 1, 'Possible column misalignment', 6 );
      }
    }
    else
    {
      correction = '100%';
    }

    // Apply to the container elements
    divBodyStyle.width = _fnStringToCss( correction );
    divHeaderStyle.width = _fnStringToCss( correction );


    /*
     * 4. Clean up
     */
    if ( ! scrollY ) {
      /* IE7< puts a vertical scrollbar in place (when it shouldn't be) due to subtracting
       * the scrollbar height from the visible display, rather than adding it on. We need to
       * set the height in order to sort this. Don't want to do it in any other browsers.
       */
      if ( ie67 ) {
        divBodyStyle.height = _fnStringToCss( tableEl.offsetHeight+barWidth );
      }
    }

    /* Finally set the width's of the header tables */
    var iOuterWidth = table.outerWidth();
    divHeaderTable[0].style.width = _fnStringToCss( iOuterWidth );
    divHeaderInnerStyle.width = _fnStringToCss( iOuterWidth );

    // Figure out if there are scrollbar present - if so then we need a the header to
    // provide a bit more space to allow "overflow" scrolling (i.e. past the scrollbar)
    var bScrolling = table.height() > divBodyEl.clientHeight || divBody.css('overflow-y') == "scroll";
    var padding = 'padding' + (browser.bScrollbarLeft ? 'Left' : 'Right' );
    divHeaderInnerStyle[ padding ] = bScrolling ? barWidth+"px" : "0px";

    // Correct DOM ordering for colgroup - comes before the thead
    table.children('colgroup').insertBefore( table.children('thead') );

    /* Adjust the position of the header in case we loose the y-scrollbar */
    divBody.trigger('scroll');

    // If sorting or filtering has occurred, jump the scrolling back to the top
    // only if we aren't holding the position
    if ( (baton.bSorted || baton.bFiltered) && ! baton._drawHold ) {
      divBodyEl.scrollTop = 0;
    }
  }


  /**
   * Apply a given function to the display child nodes of an element array (typically
   * TD children of TR rows
   *  @param {function} fn Method to apply to the objects
   *  @param array {nodes} an1 List of elements to look through for display children
   *  @param array {nodes} an2 Another list (identical structure to the first) - optional
   *  @memberof DataTable#oApi
   */
  function _fnApplyToChildren( fn, an1, an2 )
  {
    var index=0, i=0, iLen=an1.length;
    var nNode1, nNode2;

    while ( i < iLen ) {
      nNode1 = an1[i].firstChild;
      nNode2 = an2 ? an2[i].firstChild : null;

      while ( nNode1 ) {
        if ( nNode1.nodeType === 1 ) {
          if ( an2 ) {
            fn( nNode1, nNode2, index );
          }
          else {
            fn( nNode1, index );
          }

          index++;
        }

        nNode1 = nNode1.nextSibling;
        nNode2 = an2 ? nNode2.nextSibling : null;
      }

      i++;
    }
  }


  var __re_html_remove = /<.*?>/g;


  /**
   * Calculate the width of columns for the table
   *  @param {object} baton dataTables baton object
   *  @memberof DataTable#oApi
   */
  function _fnCalculateColumnWidths ( baton )
  {
    var
      table = baton.nTable,
      columns = baton.aoColumns,
      scroll = baton.oScroll,
      scrollY = scroll.sY,
      scrollX = scroll.sX,
      scrollXInner = scroll.sXInner,
      columnCount = columns.length,
      visibleColumns = _fnGetColumns( baton, 'bVisible' ),
      headerCells = $('th', baton.nTHead),
      tableWidthAttr = table.getAttribute('width'), // from DOM element
      tableContainer = table.parentNode,
      userInputs = false,
      i, column, columnIdx, width, outerWidth,
      browser = baton.oBrowser,
      ie67 = browser.bScrollOversize;

    var styleWidth = table.style.width;
    if ( styleWidth && styleWidth.indexOf('%') !== -1 ) {
      tableWidthAttr = styleWidth;
    }

    /* Convert any user input sizes into pixel sizes */
    for ( i=0 ; i<visibleColumns.length ; i++ ) {
      column = columns[ visibleColumns[i] ];

      if ( column.sWidth !== null ) {
        column.sWidth = _fnConvertToWidth( column.sWidthOrig, tableContainer );

        userInputs = true;
      }
    }

    /* If the number of columns in the DOM equals the number that we have to
     * process in DataTables, then we can use the offsets that are created by
     * the web- browser. No custom sizes can be set in order for this to happen,
     * nor scrolling used
     */
    if ( ie67 || ! userInputs && ! scrollX && ! scrollY &&
         columnCount == _fnVisbleColumns( baton ) &&
         columnCount == headerCells.length
    ) {
      for ( i=0 ; i<columnCount ; i++ ) {
        var colIdx = _fnVisibleToColumnIndex( baton, i );

        if ( colIdx !== null ) {
          columns[ colIdx ].sWidth = _fnStringToCss( headerCells.eq(i).width() );
        }
      }
    }
    else
    {
      // Otherwise construct a single row, worst case, table with the widest
      // node in the data, assign any user defined widths, then insert it into
      // the DOM and allow the browser to do all the hard work of calculating
      // table widths
      var tmpTable = $(table).clone() // don't use cloneNode - IE8 will remove events on the main table
        .css( 'visibility', 'hidden' )
        .removeAttr( 'id' );

      // Clean up the table body
      tmpTable.find('tbody tr').remove();
      var tr = $('<tr/>').appendTo( tmpTable.find('tbody') );

      // Clone the table header and footer - we can't use the header / footer
      // from the cloned table, since if scrolling is active, the table's
      // real header and footer are contained in different table tags
      tmpTable.find('thead').remove();
      tmpTable
        .append( $(baton.nTHead).clone() )

      // Apply custom sizing to the cloned header
      headerCells = _fnGetUniqueThs( baton, tmpTable.find('thead')[0] );

      for ( i=0 ; i<visibleColumns.length ; i++ ) {
        column = columns[ visibleColumns[i] ];

        headerCells[i].style.width = column.sWidthOrig !== null && column.sWidthOrig !== '' ?
          _fnStringToCss( column.sWidthOrig ) :
          '';

        // For scrollX we need to force the column width otherwise the
        // browser will collapse it. If this width is smaller than the
        // width the column requires, then it will have no effect
        if ( column.sWidthOrig && scrollX ) {
          $( headerCells[i] ).append( $('<div/>').css( {
            width: column.sWidthOrig,
            margin: 0,
            padding: 0,
            border: 0,
            height: 1
          } ) );
        }
      }

      // Find the widest cell for each column and put it into the table
      if ( baton.aoData.length ) {
        for ( i=0 ; i<visibleColumns.length ; i++ ) {
          columnIdx = visibleColumns[i];
          column = columns[ columnIdx ];

          $( _fnGetWidestNode( baton, columnIdx ) )
            .clone( false )
            .append( column.sContentPadding )
            .appendTo( tr );
        }
      }

      // Tidy the temporary table - remove name attributes so there aren't
      // duplicated in the dom (radio elements for example)
      $('[name]', tmpTable).removeAttr('name');

      // Table has been built, attach to the document so we can work with it.
      // A holding element is used, positioned at the top of the container
      // with minimal height, so it has no effect on if the container scrolls
      // or not. Otherwise it might trigger scrolling when it actually isn't
      // needed
      var holder = $('<div/>').css( scrollX || scrollY ?
          {
            position: 'absolute',
            top: 0,
            left: 0,
            height: 1,
            right: 0,
            overflow: 'hidden'
          } :
          {}
        )
        .append( tmpTable )
        .appendTo( tableContainer );

      // When scrolling (X or Y) we want to set the width of the table as
      // appropriate. However, when not scrolling leave the table width as it
      // is. This results in slightly different, but I think correct behaviour
      if ( scrollX && scrollXInner ) {
        tmpTable.width( scrollXInner );
      }
      else if ( scrollX ) {
        tmpTable.css( 'width', 'auto' );
        tmpTable.removeAttr('width');

        // If there is no width attribute or style, then allow the table to
        // collapse
        if ( tmpTable.width() < tableContainer.clientWidth && tableWidthAttr ) {
          tmpTable.width( tableContainer.clientWidth );
        }
      }
      else if ( scrollY ) {
        tmpTable.width( tableContainer.clientWidth );
      }
      else if ( tableWidthAttr ) {
        tmpTable.width( tableWidthAttr );
      }

      // Get the width of each column in the constructed table - we need to
      // know the inner width (so it can be assigned to the other table's
      // cells) and the outer width so we can calculate the full width of the
      // table. This is safe since DataTables requires a unique cell for each
      // column, but if ever a header can span multiple columns, this will
      // need to be modified.
      var total = 0;
      for ( i=0 ; i<visibleColumns.length ; i++ ) {
        var cell = $(headerCells[i]);
        var border = cell.outerWidth() - cell.width();

        // Use getBounding... where possible (not IE8-) because it can give
        // sub-pixel accuracy, which we then want to round up!
        var bounding = browser.bBounding ?
          Math.ceil( headerCells[i].getBoundingClientRect().width ) :
          cell.outerWidth();

        // Total is tracked to remove any sub-pixel errors as the outerWidth
        // of the table might not equal the total given here (IE!).
        total += bounding;

        // Width for each column to use
        columns[ visibleColumns[i] ].sWidth = _fnStringToCss( bounding - border );
      }

      table.style.width = _fnStringToCss( total );

      // Finished with the table - ditch it
      holder.remove();
    }

    // If there is a width attr, we want to attach an event listener which
    // allows the table sizing to automatically adjust when the window is
    // resized. Use the width attr rather than CSS, since we can't know if the
    // CSS is a relative value or absolute - DOM read is always px.
    if ( tableWidthAttr ) {
      table.style.width = _fnStringToCss( tableWidthAttr );
    }
  }


  /**
   * Throttle the calls to a function. Arguments and context are maintained for
   * the throttled function
   *  @param {function} fn Function to be called
   *  @param {int} [freq=200] call frequency in mS
   *  @returns {function} wrapped function
   *  @memberof DataTable#oApi
   */
  var _fnThrottle = DataTable.util.throttle;


  /**
   * Convert a CSS unit width to pixels (e.g. 2em)
   *  @param {string} width width to be converted
   *  @param {node} parent parent to get the with for (required for relative widths) - optional
   *  @returns {int} width in pixels
   *  @memberof DataTable#oApi
   */
  function _fnConvertToWidth ( width, parent )
  {
    if ( ! width ) {
      return 0;
    }

    var n = $('<div/>')
      .css( 'width', _fnStringToCss( width ) )
      .appendTo( parent || document.body );

    var val = n[0].offsetWidth;
    n.remove();

    return val;
  }


  /**
   * Get the widest node
   *  @param {object} baton dataTables baton object
   *  @param {int} colIdx column of interest
   *  @returns {node} widest table node
   *  @memberof DataTable#oApi
   */
  function _fnGetWidestNode( baton, colIdx )
  {
    var idx = _fnGetMaxLenString( baton, colIdx );
    if ( idx < 0 ) {
      return null;
    }

    var data = baton.aoData[ idx ];
    return ! data.nTr ? // Might not have been created when deferred rendering
      $('<td/>').html( _fnGetCellData( baton, idx, colIdx, 'display' ) )[0] :
      data.anCells[ colIdx ];
  }


  /**
   * Get the maximum strlen for each data column
   *  @param {object} baton dataTables baton object
   *  @param {int} colIdx column of interest
   *  @returns {string} max string length for each column
   *  @memberof DataTable#oApi
   */
  function _fnGetMaxLenString( baton, colIdx )
  {
    var s, max=-1, maxIdx = -1;

    for ( var i=0, ien=baton.aoData.length ; i<ien ; i++ ) {
      s = _fnGetCellData( baton, i, colIdx, 'display' )+'';
      s = s.replace( __re_html_remove, '' );
      s = s.replace( /&nbsp;/g, ' ' );

      if ( s.length > max ) {
        max = s.length;
        maxIdx = i;
      }
    }

    return maxIdx;
  }


  /**
   * Append a CSS unit (only if required) to a string
   *  @param {string} value to css-ify
   *  @returns {string} value with css unit
   *  @memberof DataTable#oApi
   */
  function _fnStringToCss( s )
  {
    if ( s === null ) {
      return '0px';
    }

    if ( typeof s == 'number' ) {
      return s < 0 ?
        '0px' :
        s+'px';
    }

    // Check it has a unit character already
    return s.match(/\d$/) ?
      s+'px' :
      s;
  }


  function _fnSortFlatten ( baton )
  {
    var
      i, iLen, k, kLen,
      aSort = [],
      aiOrig = [],
      aoColumns = baton.aoColumns,
      aDataSort, iCol, sType, srcCol,
      nestedSort = [],
      add = function ( a ) {
        if ( a.length && ! $.isArray( a[0] ) ) {
          // 1D array
          nestedSort.push( a );
        }
        else {
          // 2D array
          $.merge( nestedSort, a );
        }
      };
    add( baton.aaSorting );

    for ( i=0 ; i<nestedSort.length ; i++ )
    {
      srcCol = nestedSort[i][0];
      aDataSort = aoColumns[ srcCol ].aDataSort;

      for ( k=0, kLen=aDataSort.length ; k<kLen ; k++ )
      {
        iCol = aDataSort[k];
        sType = aoColumns[ iCol ].sType || 'string';

        if ( nestedSort[i]._idx === undefined ) {
          nestedSort[i]._idx = $.inArray( nestedSort[i][1], aoColumns[iCol].asSorting );
        }

        aSort.push( {
          src:       srcCol,
          col:       iCol,
          dir:       nestedSort[i][1],
          index:     nestedSort[i]._idx,
          type:      sType,
          formatter: DataTable.ext.type.order[ sType+"-pre" ]
        } );
      }
    }

    return aSort;
  }

  /**
   * Change the order of the table
   *  @param {object} baton dataTables baton object
   *  @memberof DataTable#oApi
   *  @todo This really needs split up!
   */
  function _fnSort ( baton )
  {
    var
      i, ien, iLen, j, jLen, k, kLen,
      sDataType, nTh,
      aiOrig = [],
      oExtSort = DataTable.ext.type.order,
      aoData = baton.aoData,
      aoColumns = baton.aoColumns,
      aDataSort, data, iCol, sType, oSort,
      formatters = 0,
      sortCol,
      displayMaster = baton.aiDisplay,
      aSort;

    aSort = _fnSortFlatten( baton );

    for ( i=0, ien=aSort.length ; i<ien ; i++ ) {
      sortCol = aSort[i];

      // Track if we can use the fast sort algorithm
      if ( sortCol.formatter ) {
        formatters++;
      }

      // Load the data needed for the sort, for each cell
      _fnSortData( baton, sortCol.col );
    }

    /* Tell the draw function that we have sorted the data */
    baton.bSorted = true;
  }


  function _fnSortAria ( baton )
  {
    var label;
    var nextSort;
    var columns = baton.aoColumns;
    var aSort = _fnSortFlatten( baton );

    // ARIA attributes - need to loop all columns, to update all (removing old
    // attributes as needed)
    for ( var i=0, iLen=columns.length ; i<iLen ; i++ )
    {
      var col = columns[i];
      var asSorting = col.asSorting;
      var sTitle = col.sTitle.replace( /<.*?>/g, "" );
      var th = col.nTh;

      // IE7 is throwing an error when setting these properties with jQuery's
      // attr() and removeAttr() methods...
      th.removeAttribute('aria-sort');

      /* In ARIA only the first sorting column can be marked as sorting - no multi-sort option */
      if ( col.bSortable ) {
        if ( aSort.length > 0 && aSort[0].col == i ) {
          th.setAttribute('aria-sort', aSort[0].dir=="asc" ? "ascending" : "descending" );
          nextSort = asSorting[ aSort[0].index+1 ] || asSorting[0];
        }
        else {
          nextSort = asSorting[0];
        }

        label = sTitle + ( nextSort === "asc" ?
          ": activate to sort column ascending" :
          ": activate to sort column descending"
        );
      }
      else {
        label = sTitle;
      }

      th.setAttribute('aria-label', label);
    }
  }


  /**
   * Function to run on user sort request
   *  @param {object} baton dataTables baton object
   *  @param {node} attachTo node to attach the handler to
   *  @param {int} colIdx column sorting index
   *  @param {boolean} [append=false] Append the requested sort to the existing
   *    sort if true (i.e. multi-column sort)
   *  @param {function} [callback] callback function
   *  @memberof DataTable#oApi
   */
  function _fnSortListener ( baton, colIdx, append )
  {
    var col = baton.aoColumns[ colIdx ];
    var sorting = baton.aaSorting;
    var asSorting = col.asSorting;
    var nextSortIdx;
    var next = function ( a, overflow ) {
      var idx = a._idx;
      if ( idx === undefined ) {
        idx = $.inArray( a[1], asSorting );
      }

      return idx+1 < asSorting.length ?
        idx+1 :
        overflow ?
          null :
          0;
    };

    // Convert to 2D array if needed
    if ( typeof sorting[0] === 'number' ) {
      sorting = baton.aaSorting = [ sorting ];
    }

    // If appending the sort then we are multi-column sorting
    if ( append && baton.oFeatures.bSortMulti ) {
      // Are we already doing some kind of sort on this column?
      var sortIdx = $.inArray( colIdx, _pluck(sorting, '0') );

      if ( sortIdx !== -1 ) {
        // Yes, modify the sort
        nextSortIdx = next( sorting[sortIdx], true );

        if ( nextSortIdx === null && sorting.length === 1 ) {
          nextSortIdx = 0; // can't remove sorting completely
        }

        if ( nextSortIdx === null ) {
          sorting.splice( sortIdx, 1 );
        }
        else {
          sorting[sortIdx][1] = asSorting[ nextSortIdx ];
          sorting[sortIdx]._idx = nextSortIdx;
        }
      }
      else {
        // No sort on this column yet
        sorting.push( [ colIdx, asSorting[0], 0 ] );
        sorting[sorting.length-1]._idx = 0;
      }
    }
    else if ( sorting.length && sorting[0][0] == colIdx ) {
      // Single column - already sorting on this column, modify the sort
      nextSortIdx = next( sorting[0] );

      sorting.length = 1;
      sorting[0][1] = asSorting[ nextSortIdx ];
      sorting[0]._idx = nextSortIdx;
    }
    else {
      // Single column - sort only on this column
      sorting.length = 0;
      sorting.push( [ colIdx, asSorting[0] ] );
      sorting[0]._idx = 0;
    }

    // Run the sort by calling a full redraw
    _fnReDraw( baton );
  }


  // Get the data to sort a column, be it from cache, fresh (populating the
  // cache), or from a sort formatter
  function _fnSortData( baton, idx )
  {
    // Custom sorting function - provided by the sort data type
    var column = baton.aoColumns[ idx ];
    var customSort = DataTable.ext.order[ column.sSortDataType ];
    var customData;

    if ( customSort ) {
      customData = customSort.call( baton.oInstance, baton, idx,
        _fnColumnIndexToVisible( baton, idx )
      );
    }

    // Use / populate cache
    var row, cellData;
    var formatter = DataTable.ext.type.order[ column.sType+"-pre" ];

    for ( var i=0, ien=baton.aoData.length ; i<ien ; i++ ) {
      row = baton.aoData[i];

      if ( ! row._aSortData ) {
        row._aSortData = [];
      }

      if ( ! row._aSortData[idx] || customSort ) {
        cellData = customSort ?
          customData[i] : // If there was a custom sort function, use data from there
          _fnGetCellData( baton, i, idx, 'sort' );

        row._aSortData[ idx ] = formatter ?
          formatter( cellData ) :
          cellData;
      }
    }
  }


  /**
   * Return the baton object for a particular table
   *  @param {node} table table we are using as a dataTable
   *  @returns {object} baton object - or null if not found
   *  @memberof DataTable#oApi
   */
  function _fnSettingsFromNode ( table )
  {
    var baton = DataTable.baton;
    var idx = $.inArray( table, _pluck( baton, 'nTable' ) );

    return idx !== -1 ?
      baton[ idx ] :
      null;
  }


  /**
   * See if a property is defined on one object, if so assign it to the other object
   *  @param {object} ret target object
   *  @param {object} src source object
   *  @param {string} name property
   *  @param {string} [mappedName] name to map too - optional, name used if not given
   *  @memberof DataTable#oApi
   */
  function _fnMap( ret, src, name, mappedName )
  {
    if ( $.isArray( name ) ) {
      $.each( name, function (i, val) {
        if ( $.isArray( val ) ) {
          _fnMap( ret, src, val[0], val[1] );
        }
        else {
          _fnMap( ret, src, val );
        }
      } );

      return;
    }

    if ( mappedName === undefined ) {
      mappedName = name;
    }

    if ( src[name] !== undefined ) {
      ret[mappedName] = src[name];
    }
  }


  /**
   * Extend objects - very similar to jQuery.extend, but deep copy objects, and
   * shallow copy arrays. The reason we need to do this, is that we don't want to
   * deep copy array init values (such as aaSorting) since the dev wouldn't be
   * able to override them, but we do want to deep copy arrays.
   *  @param {object} out Object to extend
   *  @param {object} extender Object from which the properties will be applied to
   *      out
   *  @param {boolean} breakRefs If true, then arrays will be sliced to take an
   *      independent copy with the exception of the `data` or `aaData` parameters
   *      if they are present. This is so you can pass in a collection to
   *      DataTables and have that used as your data source without breaking the
   *      references
   *  @returns {object} out Reference, just for convenience - out === the return.
   *  @memberof DataTable#oApi
   *  @todo This doesn't take account of arrays inside the deep copied objects.
   */
  function _fnExtend( out, extender, breakRefs )
  {
    var val;

    for ( var prop in extender ) {
      if ( extender.hasOwnProperty(prop) ) {
        val = extender[prop];

        if ( $.isPlainObject( val ) ) {
          if ( ! $.isPlainObject( out[prop] ) ) {
            out[prop] = {};
          }
          $.extend( true, out[prop], val );
        }
        else if ( breakRefs && prop !== 'data' && prop !== 'aaData' && $.isArray(val) ) {
          out[prop] = val.slice();
        }
        else {
          out[prop] = val;
        }
      }
    }

    return out;
  }


  /**
   * Register a callback function. Easily allows a callback function to be added to
   * an array store of callback functions that can then all be called together.
   *  @param {object} baton dataTables baton object
   *  @param {string} sStore Name of the array storage for the callbacks in baton
   *  @param {function} fn Function to be called back
   *  @param {string} sName Identifying name for the callback (i.e. a label)
   *  @memberof DataTable#oApi
   */
  function _fnCallbackReg( baton, sStore, fn, sName )
  {
    if ( fn )
    {
      baton[sStore].push( {
        "fn": fn,
        "sName": sName
      } );
    }
  }


  function _fnRenderer( baton, type )
  {
    var renderer = baton.renderer;
    var host = DataTable.ext.renderer[type];

    if ( $.isPlainObject( renderer ) && renderer[type] ) {
      // Specific renderer for this type. If available use it, otherwise use
      // the default.
      return host[renderer[type]] || host._;
    }
    else if ( typeof renderer === 'string' ) {
      // Common renderer - if there is one available for this type use it,
      // otherwise use the default
      return host[renderer] || host._;
    }

    // Use the default
    return host._;
  }


  /**
   * Computed structure of the DataTables API, defined by the options passed to
   * `DataTable.Api.register()` when building the API.
   *
   * The structure is built in order to speed creation and extension of the Api
   * objects since the extensions are effectively pre-parsed.
   *
   * The array is an array of objects with the following structure, where this
   * base array represents the Api prototype base:
   *
   *     [
   *       {
   *         name:      'data'                -- string   - Property name
   *         val:       function () {},       -- function - Api method (or undefined if just an object
   *         methodExt: [ ... ],              -- array    - Array of Api object definitions to extend the method result
   *         propExt:   [ ... ]               -- array    - Array of Api object definitions to extend the property
   *       },
   *       {
   *         name:     'row'
   *         val:       {},
   *         methodExt: [ ... ],
   *         propExt:   [
   *           {
   *             name:      'data'
   *             val:       function () {},
   *             methodExt: [ ... ],
   *             propExt:   [ ... ]
   *           },
   *           ...
   *         ]
   *       }
   *     ]
   *
   * @type {Array}
   * @ignore
   */
  var __apiStruct = [];


  /**
   * `Array.prototype` reference.
   *
   * @type object
   * @ignore
   */
  var __arrayProto = Array.prototype;


  /**
   * Abstraction for `context` parameter of the `Api` constructor to allow it to
   * take several different forms for ease of use.
   *
   * Each of the input parameter types will be converted to a DataTables baton
   * object where possible.
   *
   * @param  {string|node|jQuery|object} mixed DataTable identifier. Can be one
   *   of:
   *
   *   * `string` - jQuery selector. Any DataTables' matching the given selector
   *     with be found and used.
   *   * `node` - `TABLE` node which has already been formed into a DataTable.
   *   * `jQuery` - A jQuery object of `TABLE` nodes.
   *   * `object` - DataTables baton object
   *   * `DataTables.Api` - API instance
   * @return {array|null} Matching DataTables baton objects. `null` or
   *   `undefined` is returned if no matching DataTable is found.
   * @ignore
   */
  var _toSettings = function ( mixed )
  {
    var idx, jq;
    var baton = DataTable.baton;
    var tables = $.map( baton, function (el, i) {
      return el.nTable;
    } );

    if ( ! mixed ) {
      return [];
    }
    else if ( mixed.nTable && mixed.oApi ) {
      // DataTables baton object
      return [ mixed ];
    }
    else if ( mixed.nodeName && mixed.nodeName.toLowerCase() === 'table' ) {
      // Table node
      idx = $.inArray( mixed, tables );
      return idx !== -1 ? [ baton[idx] ] : null;
    }
    else if ( typeof mixed === 'string' ) {
      // jQuery selector
      jq = $(mixed);
    }
    else if ( mixed instanceof $ ) {
      // jQuery object (also DataTables instance)
      jq = mixed;
    }

    if ( jq ) {
      return jq.map( function(i) {
        idx = $.inArray( this, tables );
        return idx !== -1 ? baton[idx] : null;
      } ).toArray();
    }
  };


  /**
   * DataTables API class - used to control and interface with  one or more
   * DataTables enhanced tables.
   *
   * The API class is heavily based on jQuery, presenting a chainable interface
   * that you can use to interact with tables. Each instance of the API class has
   * a "context" - i.e. the tables that it will operate on. This could be a single
   * table, all tables on a page or a sub-set thereof.
   *
   * Additionally the API is designed to allow you to easily work with the data in
   * the tables, retrieving and manipulating it as required. This is done by
   * presenting the API class as an array like interface. The contents of the
   * array depend upon the actions requested by each method (for example
   * `rows().nodes()` will return an array of nodes, while `rows().data()` will
   * return an array of objects or arrays depending upon your table's
   * configuration). The API object has a number of array like methods (`push`,
   * `pop`, `reverse` etc) as well as additional helper methods (`each`, `pluck`,
   * `unique` etc) to assist your working with the data held in a table.
   *
   * Most methods (those which return an Api instance) are chainable, which means
   * the return from a method call also has all of the methods available that the
   * top level object had. For example, these two calls are equivalent:
   *
   *     // Not chained
   *     api.row.add( {...} );
   *     api.draw();
   *
   *     // Chained
   *     api.row.add( {...} ).draw();
   *
   * @class DataTable.Api
   * @param {array|object|string|jQuery} context DataTable identifier. This is
   *   used to define which DataTables enhanced tables this API will operate on.
   *   Can be one of:
   *
   *   * `string` - jQuery selector. Any DataTables' matching the given selector
   *     with be found and used.
   *   * `node` - `TABLE` node which has already been formed into a DataTable.
   *   * `jQuery` - A jQuery object of `TABLE` nodes.
   *   * `object` - DataTables baton object
   * @param {array} [data] Data to initialise the Api instance with.
   *
   * @example
   *   // Direct initialisation during DataTables construction
   *   var api = $('#example').DataTable();
   *
   * @example
   *   // Initialisation using a DataTables jQuery object
   *   var api = $('#example').dataTable().api();
   *
   * @example
   *   // Initialisation as a constructor
   *   var api = new DataTable.Api( 'table.dataTable' );
   */
  DataTable.Api = function ( context, data )
  {
    var baton = [];
    var ctxSettings = function ( o ) {
      var a = _toSettings( o );
      if ( a ) {
        baton.push.apply( baton, a );
      }
    };

    if ( $.isArray( context ) ) {
      for ( var i=0, ien=context.length ; i<ien ; i++ ) {
        ctxSettings( context[i] );
      }
    }
    else {
      ctxSettings( context );
    }

    // Remove duplicates
    this.context = _unique( baton );

    // Initial data
    if ( data ) {
      $.merge( this, data );
    }

    // selector
    this.selector = {
      rows: null,
      cols: null,
      opts: null
    };

    DataTable.Api.extend( this, this, __apiStruct );
  };

  // Don't destroy the existing prototype, just extend it. Required for jQuery 2's
  // isPlainObject.
  $.extend( DataTable.Api.prototype, {
    any: function ()
    {
      return this.count() !== 0;
    },


    concat:  __arrayProto.concat,


    context: [], // array of table baton objects


    count: function ()
    {
      return this.flatten().length;
    },


    each: function ( fn )
    {
      for ( var i=0, ien=this.length ; i<ien; i++ ) {
        fn.call( this, this[i], i, this );
      }

      return this;
    },


    eq: function ( idx )
    {
      var ctx = this.context;

      return ctx.length > idx ?
        new DataTable.Api( ctx[idx], this[idx] ) :
        null;
    },


    filter: function ( fn )
    {
      var a = [];

      if ( __arrayProto.filter ) {
        a = __arrayProto.filter.call( this, fn, this );
      }
      else {
        // Compatibility for browsers without EMCA-252-5 (JS 1.6)
        for ( var i=0, ien=this.length ; i<ien ; i++ ) {
          if ( fn.call( this, this[i], i, this ) ) {
            a.push( this[i] );
          }
        }
      }

      return new DataTable.Api( this.context, a );
    },


    flatten: function ()
    {
      var a = [];
      return new DataTable.Api( this.context, a.concat.apply( a, this.toArray() ) );
    },


    join:    __arrayProto.join,


    indexOf: __arrayProto.indexOf || function (obj, start)
    {
      for ( var i=(start || 0), ien=this.length ; i<ien ; i++ ) {
        if ( this[i] === obj ) {
          return i;
        }
      }
      return -1;
    },

    iterator: function ( flatten, type, fn, alwaysNew ) {
      var
        a = [], ret,
        i, ien, j, jen,
        context = this.context,
        rows, items, item,
        selector = this.selector;

      // Argument shifting
      if ( typeof flatten === 'string' ) {
        alwaysNew = fn;
        fn = type;
        type = flatten;
        flatten = false;
      }

      for ( i=0, ien=context.length ; i<ien ; i++ ) {
        var apiInst = new DataTable.Api( context[i] );

        if ( type === 'table' ) {
          ret = fn.call( apiInst, context[i], i );

          if ( ret !== undefined ) {
            a.push( ret );
          }
        }
        else if ( type === 'columns' || type === 'rows' ) {
          // this has same length as context - one entry for each table
          ret = fn.call( apiInst, context[i], this[i], i );

          if ( ret !== undefined ) {
            a.push( ret );
          }
        }
        else if ( type === 'column' || type === 'column-rows' || type === 'row' || type === 'cell' ) {
          // columns and rows share the same structure.
          // 'this' is an array of column indexes for each context
          items = this[i];

          if ( type === 'column-rows' ) {
            rows = _selector_row_indexes( context[i], selector.opts );
          }

          for ( j=0, jen=items.length ; j<jen ; j++ ) {
            item = items[j];

            if ( type === 'cell' ) {
              ret = fn.call( apiInst, context[i], item.row, item.column, i, j );
            }
            else {
              ret = fn.call( apiInst, context[i], item, i, j, rows );
            }

            if ( ret !== undefined ) {
              a.push( ret );
            }
          }
        }
      }

      if ( a.length || alwaysNew ) {
        var api = new DataTable.Api( context, flatten ? a.concat.apply( [], a ) : a );
        var apiSelector = api.selector;
        apiSelector.rows = selector.rows;
        apiSelector.cols = selector.cols;
        apiSelector.opts = selector.opts;
        return api;
      }
      return this;
    },


    lastIndexOf: __arrayProto.lastIndexOf || function (obj, start)
    {
      // Bit cheeky...
      return this.indexOf.apply( this.toArray.reverse(), arguments );
    },


    length:  0,


    map: function ( fn )
    {
      var a = [];

      if ( __arrayProto.map ) {
        a = __arrayProto.map.call( this, fn, this );
      }
      else {
        // Compatibility for browsers without EMCA-252-5 (JS 1.6)
        for ( var i=0, ien=this.length ; i<ien ; i++ ) {
          a.push( fn.call( this, this[i], i ) );
        }
      }

      return new DataTable.Api( this.context, a );
    },


    pluck: function ( prop )
    {
      return this.map( function ( el ) {
        return el[ prop ];
      } );
    },

    pop:     __arrayProto.pop,


    push:    __arrayProto.push,


    // Does not return an API instance
    reduce: __arrayProto.reduce || function ( fn, init )
    {
      return _fnReduce( this, fn, init, 0, this.length, 1 );
    },


    reduceRight: __arrayProto.reduceRight || function ( fn, init )
    {
      return _fnReduce( this, fn, init, this.length-1, -1, -1 );
    },


    reverse: __arrayProto.reverse,


    // Object with rows, columns and opts
    selector: null,


    shift:   __arrayProto.shift,


    slice: function () {
      return new DataTable.Api( this.context, this );
    },


    sort:    __arrayProto.sort, // ? name - order?


    splice:  __arrayProto.splice,


    toArray: function ()
    {
      return __arrayProto.slice.call( this );
    },


    to$: function ()
    {
      return $( this );
    },


    toJQuery: function ()
    {
      return $( this );
    },


    unique: function ()
    {
      return new DataTable.Api( this.context, _unique(this) );
    },


    unshift: __arrayProto.unshift
  } );


  DataTable.Api.extend = function ( scope, obj, ext )
  {
    // Only extend API instances and static properties of the API
    if ( ! ext.length || ! obj || ( ! (obj instanceof DataTable.Api) && ! obj.__dt_wrapper ) ) {
      return;
    }

    var
      i, ien,
      struct,
      methodScoping = function ( scope, fn, struc ) {
        return function () {
          var ret = fn.apply( scope, arguments );

          // Method extension
          DataTable.Api.extend( ret, ret, struc.methodExt );
          return ret;
        };
      };

    for ( i=0, ien=ext.length ; i<ien ; i++ ) {
      struct = ext[i];

      // Value
      obj[ struct.name ] = struct.type === 'function' ?
        methodScoping( scope, struct.val, struct ) :
        struct.type === 'object' ?
          {} :
          struct.val;

      obj[ struct.name ].__dt_wrapper = true;

      // Property extension
      DataTable.Api.extend( scope, obj[ struct.name ], struct.propExt );
    }
  };


  // @todo - Is there need for an augment function?
  // DataTable.Api.augment = function ( inst, name )
  // {
  //   // Find src object in the structure from the name
  //   var parts = name.split('.');

  //   DataTable.Api.extend( inst, obj );
  // };


  //     [
  //       {
  //         name:      'data'                -- string   - Property name
  //         val:       function () {},       -- function - Api method (or undefined if just an object
  //         methodExt: [ ... ],              -- array    - Array of Api object definitions to extend the method result
  //         propExt:   [ ... ]               -- array    - Array of Api object definitions to extend the property
  //       },
  //       {
  //         name:     'row'
  //         val:       {},
  //         methodExt: [ ... ],
  //         propExt:   [
  //           {
  //             name:      'data'
  //             val:       function () {},
  //             methodExt: [ ... ],
  //             propExt:   [ ... ]
  //           },
  //           ...
  //         ]
  //       }
  //     ]

  DataTable.Api.register = _api_register = function ( name, val )
  {
    if ( $.isArray( name ) ) {
      for ( var j=0, jen=name.length ; j<jen ; j++ ) {
        DataTable.Api.register( name[j], val );
      }
      return;
    }

    var
      i, ien,
      heir = name.split('.'),
      struct = __apiStruct,
      key, method;

    var find = function ( src, name ) {
      for ( var i=0, ien=src.length ; i<ien ; i++ ) {
        if ( src[i].name === name ) {
          return src[i];
        }
      }
      return null;
    };

    for ( i=0, ien=heir.length ; i<ien ; i++ ) {
      method = heir[i].indexOf('()') !== -1;
      key = method ?
        heir[i].replace('()', '') :
        heir[i];

      var src = find( struct, key );
      if ( ! src ) {
        src = {
          name:      key,
          val:       {},
          methodExt: [],
          propExt:   [],
          type:      'object'
        };
        struct.push( src );
      }

      if ( i === ien-1 ) {
        src.val = val;
        src.type = typeof val === 'function' ?
          'function' :
          $.isPlainObject( val ) ?
            'object' :
            'other';
      }
      else {
        struct = method ?
          src.methodExt :
          src.propExt;
      }
    }
  };

  DataTable.Api.registerPlural = _api_registerPlural = function ( pluralName, singularName, val ) {
    DataTable.Api.register( pluralName, val );

    DataTable.Api.register( singularName, function () {
      var ret = val.apply( this, arguments );

      if ( ret === this ) {
        // Returned item is the API instance that was passed in, return it
        return this;
      }
      else if ( ret instanceof DataTable.Api ) {
        // New API instance returned, want the value from the first item
        // in the returned array for the singular result.
        return ret.length ?
          $.isArray( ret[0] ) ?
            new DataTable.Api( ret.context, ret[0] ) : // Array results are 'enhanced'
            ret[0] :
          undefined;
      }

      // Non-API return - just fire it back
      return ret;
    } );
  };


  /**
   * Context selector for the API's context (i.e. the tables the API instance
   * refers to.
   *
   * @name    DataTable.Api#tables
   * @param {string|integer} [selector] Selector to pick which tables the iterator
   *   should operate on. If not given, all tables in the current context are
   *   used. This can be given as a jQuery selector (for example `':gt(0)'`) to
   *   select multiple tables or as an integer to select a single table.
   * @returns {DataTable.Api} Returns a new API instance if a selector is given.
   */
  _api_register( 'tables()', function ( selector ) {
    // A new instance is created if there was a selector specified
    return selector !== undefined && selector !== null ?
      new DataTable.Api( __table_selector( selector, this.context ) ) :
      this;
  } );


  _api_register( 'table()', function ( selector ) {
    var tables = this.tables( selector );
    var ctx = tables.context;

    // Truncate to the first matched table
    return ctx.length ?
      new DataTable.Api( ctx[0] ) :
      tables;
  } );


  _api_registerPlural( 'tables().nodes()', 'table().node()' , function () {
    return this.iterator( 'table', function ( ctx ) {
      return ctx.nTable;
    }, 1 );
  } );


  _api_registerPlural( 'tables().body()', 'table().body()' , function () {
    return this.iterator( 'table', function ( ctx ) {
      return ctx.nTBody;
    }, 1 );
  } );


  _api_registerPlural( 'tables().header()', 'table().header()' , function () {
    return this.iterator( 'table', function ( ctx ) {
      return ctx.nTHead;
    }, 1 );
  } );


  /**
   * Redraw the tables in the current context.
   */

  // Strings for the method names to help minification
  var _emp = '';
  var _child_obj = _emp+'row().child';
  var _child_mth = _child_obj+'()';


  /**
   * Convert from camel case parameters to Hungarian notation. This is made public
   * for the extensions to provide the same ability as DataTables core to accept
   * either the 1.9 style Hungarian notation, or the 1.10+ style camelCase
   * parameters.
   *
   *  @param {object} src The model object which holds all parameters that can be
   *    mapped.
   *  @param {object} user The object to convert from camel case to Hungarian.
   *  @param {boolean} force When set to `true`, properties which already have a
   *    Hungarian value in the `user` object will be overwritten. Otherwise they
   *    won't be.
   */
  DataTable.camelToHungarian = _fnCamelToHungarian;


  // jQuery functions to operate on the tables
  $.each( [ 'on', 'one', 'off' ], function (i, key) {
    _api_register( key+'()', function ( /* event, handler */ ) {
      var args = Array.prototype.slice.call(arguments);

      // Add the `dt` namespace automatically if it isn't already present
      args[0] = $.map( args[0].split( /\s/ ), function ( e ) {
        return ! e.match(/\.dt\b/) ?
          e+'.dt' :
          e;
        } ).join( ' ' );

      var inst = $( this.tables().nodes() );
      inst[key].apply( inst, args );
      return this;
    } );
  } );


  _api_register( 'clear()', function () {
    return this.iterator( 'table', function ( baton ) {
      _fnClearTable( baton );
    } );
  } );


  _api_register( 'data()', function () {
    return this.iterator( 'table', function ( baton ) {
      return _pluck( baton.aoData, '_aData' );
    } ).flatten();
  } );


  _api_register( 'destroy()', function ( remove ) {
    remove = remove || false;

    return this.iterator( 'table', function ( baton ) {
      var orig      = baton.nTableWrapper.parentNode;
      var classes   = baton.oClasses;
      var table     = baton.nTable;
      var tbody     = baton.nTBody;
      var thead     = baton.nTHead;
      var jqTable   = $(table);
      var jqTbody   = $(tbody);
      var jqWrapper = $(baton.nTableWrapper);
      var rows      = $.map( baton.aoData, function (r) { return r.nTr; } );
      var i, ien;

      // Flag to note that the table is currently being destroyed - no action
      // should be taken
      baton.bDestroying = true;

      // Fire off the destroy callbacks for plug-ins etc
      datatableEventEmit( baton, "aoDestroyCallback", "destroy", [baton] );

      // If not being removed from the document, make all columns visible
      if ( ! remove ) {
        new DataTable.Api( baton ).columns().visible( true );
      }

      // Blitz all `DT` namespaced events (these are internal events, the
      // lowercase, `dt` events are user subscribed and they are responsible
      // for removing them
      jqWrapper.off('.DT').find(':not(tbody *)').off('.DT');
      $(window).off('.DT-'+baton.sInstance);

      // When scrolling we had to break the table up - restore it
      if ( table != thead.parentNode ) {
        jqTable.children('thead').detach();
        jqTable.append( thead );
      }

      //!! baton.aaSorting = [];
      //!! _fnSortingClasses( baton );

      $( rows ).removeClass( baton.asStripeClasses.join(' ') );

      $('th, td', thead).removeClass( classes.sSortable+' '+
        classes.sSortableAsc+' '+classes.sSortableDesc+' '+classes.sSortableNone
      );

      // Add the TR elements back into the table in their original order
      jqTbody.children().detach();
      jqTbody.append( rows );

      // Remove the DataTables generated nodes, events and classes
      var removedMethod = remove ? 'remove' : 'detach';
      jqTable[ removedMethod ]();
      jqWrapper[ removedMethod ]();

      /* Remove the baton object from the baton array */
      var idx = $.inArray( baton, DataTable.baton );
      if ( idx !== -1 ) {
        DataTable.baton.splice( idx, 1 );
      }
    } );
  } );


  // Add the `every()` method for rows, columns and cells in a compact form
  $.each( [ 'column', 'row', 'cell' ], function ( i, type ) {
    _api_register( type+'s().every()', function ( fn ) {
      var opts = this.selector.opts;
      var api = this;

      return this.iterator( type, function ( baton, arg1, arg2, arg3, arg4 ) {
        // Rows and columns:
        //  arg1 - index
        //  arg2 - table counter
        //  arg3 - loop counter
        //  arg4 - undefined
        // Cells:
        //  arg1 - row index
        //  arg2 - column index
        //  arg3 - table counter
        //  arg4 - loop counter
        fn.call(
          api[ type ](
            arg1,
            type==='cell' ? arg2 : opts,
            type==='cell' ? opts : undefined
          ),
          arg1, arg2, arg3, arg4
        );
      } );
    } );
  } );


  /**
   * Version string for plug-ins to check compatibility. Allowed format is
   * `a.b.c-d` where: a:int, b:int, c:int, d:string(dev|beta|alpha). `d` is used
   * only for non-release builds. See http://semver.org/ for more information.
   *  @member
   *  @type string
   *  @default Version number
   */
  DataTable.version = "1.10.21";

  /**
   * Private data store, containing all of the baton objects that are
   * created for the tables on a given page.
   *
   * Note that the `DataTable.baton` object is aliased to
   * `jQuery.fn.dataTableExt` through which it may be accessed and
   * manipulated, or `jQuery.fn.dataTable.baton`.
   *  @member
   *  @type array
   *  @default []
   *  @private
   */
  DataTable.baton = [];

  /**
   * Object models container, for the various models that DataTables has
   * available to it. These models define the objects that are used to hold
   * the active state and configuration of the table.
   *  @namespace
   */
  DataTable.models = {};


  /**
   * Template object for the way in which DataTables holds information about
   * search information for the global filter and individual column filters.
   *  @namespace
   */
  DataTable.models.oSearch = {
    /**
     * Flag to indicate if the filtering should be case insensitive or not
     *  @type boolean
     *  @default true
     */
    "bCaseInsensitive": true,

    /**
     * Applied search term
     *  @type string
     *  @default <i>Empty string</i>
     */
    "sSearch": "",

    /**
     * Flag to indicate if the search term should be interpreted as a
     * regular expression (true) or not (false) and therefore and special
     * regex characters escaped.
     *  @type boolean
     *  @default false
     */
    "bRegex": false,

    /**
     * Flag to indicate if DataTables is to use its smart filtering or not.
     *  @type boolean
     *  @default true
     */
    "bSmart": true
  };


  /**
   * Template object for the way in which DataTables holds information about
   * each individual row. This is the object format used for the baton
   * aoData array.
   *  @namespace
   */
  DataTable.models.oRow = {
    /**
     * Data object from the original data source for the row. This is either
     * an array if using the traditional form of DataTables, or an object if
     * using mData options. The exact type will depend on the passed in
     * data from the data source, or will be an array if using DOM a data
     * source.
     *  @type array|object
     *  @default []
     */
    "_aData": [],

    /**
     * Sorting data cache - this array is ostensibly the same length as the
     * number of columns (although each index is generated only as it is
     * needed), and holds the data that is used for sorting each column in the
     * row. We do this cache generation at the start of the sort in order that
     * the formatting of the sort data need be done only once for each cell
     * per sort. This array should not be read from or written to by anything
     * other than the master sorting methods.
     *  @type array
     *  @default null
     *  @private
     */
    "_aSortData": null,

    /**
     * Per cell filtering data cache. As per the sort data cache, used to
     * increase the performance of the filtering in DataTables
     *  @type array
     *  @default null
     *  @private
     */
    "_aFilterData": null,

    /**
     * Filtering data cache. This is the same as the cell filtering cache, but
     * in this case a string rather than an array. This is easily computed with
     * a join on `_aFilterData`, but is provided as a cache so the join isn't
     * needed on every search (memory traded for performance)
     *  @type array
     *  @default null
     *  @private
     */
    "_sFilterRow": null,

    /**
     * Cache of the class name that DataTables has applied to the row, so we
     * can quickly look at this variable rather than needing to do a DOM check
     * on className for the nTr property.
     *  @type string
     *  @default <i>Empty string</i>
     *  @private
     */
    "_sRowStripe": "",

    /**
     * Denote if the original data source was from the DOM, or the data source
     * object. This is used for invalidating data, so DataTables can
     * automatically read data from the original source, unless uninstructed
     * otherwise.
     *  @type string
     *  @default null
     *  @private
     */
    "src": null,

    /**
     * Index in the aoData array. This saves an indexOf lookup when we have the
     * object, but want to know the index
     *  @type integer
     *  @default -1
     *  @private
     */
    "idx": -1
  };


  /**
   * Template object for the column information object in DataTables. This object
   * is held in the baton aoColumns array and contains all the information that
   * DataTables needs about each individual column.
   *
   * Note that this object is related to {@link DataTable.defaults.column}
   * but this one is the internal data store for DataTables's cache of columns.
   * It should NOT be manipulated outside of DataTables. Any configuration should
   * be done through the initialisation options.
   *  @namespace
   */
  DataTable.models.oColumn = {
    /**
     * Column index. This could be worked out on-the-fly with $.inArray, but it
     * is faster to just hold it as a variable
     *  @type integer
     *  @default null
     */
    "idx": null,

    /**
     * A list of the columns that sorting should occur on when this column
     * is sorted. That this property is an array allows multi-column sorting
     * to be defined for a column (for example first name / last name columns
     * would benefit from this). The values are integers pointing to the
     * columns to be sorted on (typically it will be a single integer pointing
     * at itself, but that doesn't need to be the case).
     *  @type array
     */
    "aDataSort": null,

    /**
     * Define the sorting directions that are applied to the column, in sequence
     * as the column is repeatedly sorted upon - i.e. the first value is used
     * as the sorting direction when the column if first sorted (clicked on).
     * Sort it again (click again) and it will move on to the next index.
     * Repeat until loop.
     *  @type array
     */
    "asSorting": null,

    /**
     * Flag to indicate if the column is searchable, and thus should be included
     * in the filtering or not.
     *  @type boolean
     */
    "bSearchable": null,

    /**
     * Flag to indicate if the column is sortable or not.
     *  @type boolean
     */
    "bSortable": null,

    /**
     * Flag to indicate if the column is currently visible in the table or not
     *  @type boolean
     */
    "bVisible": null,

    /**
     * Store for manual type assignment using the `column.type` option. This
     * is held in store so we can manipulate the column's `sType` property.
     *  @type string
     *  @default null
     *  @private
     */
    "_sManualType": null,

    /**
     * Flag to indicate if HTML5 data attributes should be used as the data
     * source for filtering or sorting. True is either are.
     *  @type boolean
     *  @default false
     *  @private
     */
    "_bAttrSrc": false,

    /**
     * Developer definable function that is called whenever a cell is created (Ajax source,
     * etc) or processed for input (DOM source). This can be used as a compliment to mRender
     * allowing you to modify the DOM element (add background colour for example) when the
     * element is available.
     *  @type function
     *  @param {element} nTd The TD node that has been created
     *  @param {*} sData The Data for the cell
     *  @param {array|object} oData The data for the whole row
     *  @param {int} iRow The row index for the aoData data store
     *  @default null
     */
    "fnCreatedCell": null,

    /**
     * Function to get data from a cell in a column. You should <b>never</b>
     * access data directly through _aData internally in DataTables - always use
     * the method attached to this property. It allows mData to function as
     * required. This function is automatically assigned by the column
     * initialisation method
     *  @type function
     *  @param {array|object} oData The data array/object for the array
     *    (i.e. aoData[]._aData)
     *  @param {string} sSpecific The specific data type you want to get -
     *    'display', 'type' 'filter' 'sort'
     *  @returns {*} The data for the cell from the given row's data
     *  @default null
     */
    "fnGetData": null,

    /**
     * Function to set data for a cell in the column. You should <b>never</b>
     * set the data directly to _aData internally in DataTables - always use
     * this method. It allows mData to function as required. This function
     * is automatically assigned by the column initialisation method
     *  @type function
     *  @param {array|object} oData The data array/object for the array
     *    (i.e. aoData[]._aData)
     *  @param {*} sValue Value to set
     *  @default null
     */
    "fnSetData": null,

    /**
     * Property to read the value for the cells in the column from the data
     * source array / object. If null, then the default content is used, if a
     * function is given then the return from the function is used.
     *  @type function|int|string|null
     *  @default null
     */
    "mData": null,

    /**
     * Partner property to mData which is used (only when defined) to get
     * the data - i.e. it is basically the same as mData, but without the
     * 'set' option, and also the data fed to it is the result from mData.
     * This is the rendering method to match the data method of mData.
     *  @type function|int|string|null
     *  @default null
     */
    "mRender": null,

    /**
     * Unique header TH/TD element for this column - this is what the sorting
     * listener is attached to (if sorting is enabled.)
     *  @type node
     *  @default null
     */
    "nTh": null,

    /**
     * Unique footer TH/TD element for this column (if there is one). Not used
     * in DataTables as such, but can be used for plug-ins to reference the
     * footer for each column.
     *  @type node
     *  @default null
     */
    "nTf": null,

    /**
     * The class to apply to all TD elements in the table's TBODY for the column
     *  @type string
     *  @default null
     */
    "sClass": null,

    /**
     * When DataTables calculates the column widths to assign to each column,
     * it finds the longest string in each column and then constructs a
     * temporary table and reads the widths from that. The problem with this
     * is that "mmm" is much wider then "iiii", but the latter is a longer
     * string - thus the calculation can go wrong (doing it properly and putting
     * it into an DOM object and measuring that is horribly(!) slow). Thus as
     * a "work around" we provide this option. It will append its value to the
     * text that is found to be the longest string for the column - i.e. padding.
     *  @type string
     */
    "sContentPadding": null,

    /**
     * Allows a default value to be given for a column's data, and will be used
     * whenever a null data source is encountered (this can be because mData
     * is set to null, or because the data source itself is null).
     *  @type string
     *  @default null
     */
    "sDefaultContent": null,

    /**
     * Name for the column, allowing reference to the column by name as well as
     * by index (needs a lookup to work by name).
     *  @type string
     */
    "sName": null,

    /**
     * Custom sorting data type - defines which of the available plug-ins in
     * afnSortData the custom sorting will use - if any is defined.
     *  @type string
     *  @default std
     */
    "sSortDataType": 'std',

    /**
     * Class to be applied to the header element when sorting on this column
     *  @type string
     *  @default null
     */
    "sSortingClass": null,

    /**
     * Class to be applied to the header element when sorting on this column -
     * when jQuery UI theming is used.
     *  @type string
     *  @default null
     */
    "sSortingClassJUI": null,

    /**
     * Title of the column - what is seen in the TH element (nTh).
     *  @type string
     */
    "sTitle": null,

    /**
     * Column sorting and filtering type
     *  @type string
     *  @default null
     */
    "sType": null,

    /**
     * Width of the column
     *  @type string
     *  @default null
     */
    "sWidth": null,

    /**
     * Width of the column when it was first "encountered"
     *  @type string
     *  @default null
     */
    "sWidthOrig": null
  };


  /*
   * Developer note: The properties of the object below are given in Hungarian
   * notation, that was used as the interface for DataTables prior to v1.10, however
   * from v1.10 onwards the primary interface is camel case. In order to avoid
   * breaking backwards compatibility utterly with this change, the Hungarian
   * version is still, internally the primary interface, but is is not documented
   * - hence the @name tags in each doc comment. This allows a Javascript function
   * to create a map from Hungarian notation to camel case (going the other direction
   * would require each property to be listed, which would at around 3K to the size
   * of DataTables, while this method is about a 0.5K hit.
   *
   * Ultimately this does pave the way for Hungarian notation to be dropped
   * completely, but that is a massive amount of work and will break current
   * installs (therefore is on-hold until v2).
   */

  /**
   * Initialisation options that can be given to DataTables at initialisation
   * time.
   *  @namespace
   */
  DataTable.defaults = {
    /**
     * An array of data to use for the table, passed in at initialisation which
     * will be used in preference to any data which is already in the DOM. This is
     * particularly useful for constructing tables purely in Javascript, for
     * example with a custom Ajax call.
     *  @type array
     *  @default null
     *
     *  @dtopt Option
     *  @name DataTable.defaults.data
     *
     *  @example
     *    // Using a 2D array data source
     *    $(document).ready( function () {
     *      $('#example').dataTable( {
     *        "data": [
     *          ['Trident', 'Internet Explorer 4.0', 'Win 95+', 4, 'X'],
     *          ['Trident', 'Internet Explorer 5.0', 'Win 95+', 5, 'C'],
     *        ],
     *        "columns": [
     *          { "title": "Engine" },
     *          { "title": "Browser" },
     *          { "title": "Platform" },
     *          { "title": "Version" },
     *          { "title": "Grade" }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using an array of objects as a data source (`data`)
     *    $(document).ready( function () {
     *      $('#example').dataTable( {
     *        "data": [
     *          {
     *            "engine":   "Trident",
     *            "browser":  "Internet Explorer 4.0",
     *            "platform": "Win 95+",
     *            "version":  4,
     *            "grade":    "X"
     *          },
     *          {
     *            "engine":   "Trident",
     *            "browser":  "Internet Explorer 5.0",
     *            "platform": "Win 95+",
     *            "version":  5,
     *            "grade":    "C"
     *          }
     *        ],
     *        "columns": [
     *          { "title": "Engine",   "data": "engine" },
     *          { "title": "Browser",  "data": "browser" },
     *          { "title": "Platform", "data": "platform" },
     *          { "title": "Version",  "data": "version" },
     *          { "title": "Grade",    "data": "grade" }
     *        ]
     *      } );
     *    } );
     */
    "aaData": null,


    /**
     * If ordering is enabled, then DataTables will perform a first pass sort on
     * initialisation. You can define which column(s) the sort is performed
     * upon, and the sorting direction, with this variable. The `sorting` array
     * should contain an array for each column to be sorted initially containing
     * the column's index and a direction string ('asc' or 'desc').
     *  @type array
     *  @default [[0,'asc']]
     *
     *  @dtopt Option
     *  @name DataTable.defaults.order
     *
     *  @example
     *    // Sort by 3rd column first, and then 4th column
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "order": [[2,'asc'], [3,'desc']]
     *      } );
     *    } );
     *
     *    // No initial sorting
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "order": []
     *      } );
     *    } );
     */
    "aaSorting": [[0,'asc']],


    /**
     * DataTables can be instructed to load data to display in the table from a
     * Ajax source. This option defines how that Ajax call is made and where to.
     *
     * The `ajax` property has three different modes of operation, depending on
     * how it is defined. These are:
     *
     * * `string` - Set the URL from where the data should be loaded from.
     * * `object` - Define properties for `jQuery.ajax`.
     * * `function` - Custom data get function
     *
     * `string`
     * --------
     *
     * As a string, the `ajax` property simply defines the URL from which
     * DataTables will load data.
     *
     * `object`
     * --------
     *
     * As an object, the parameters in the object are passed to
     * [jQuery.ajax](http://api.jquery.com/jQuery.ajax/) allowing fine control
     * of the Ajax request. DataTables has a number of default parameters which
     * you can override using this option. Please refer to the jQuery
     * documentation for a full description of the options available, although
     * the following parameters provide additional options in DataTables or
     * require special consideration:
     *
     * * `data` - As with jQuery, `data` can be provided as an object, but it
     *   can also be used as a function to manipulate the data DataTables sends
     *   to the server. The function takes a single parameter, an object of
     *   parameters with the values that DataTables has readied for sending. An
     *   object may be returned which will be merged into the DataTables
     *   defaults, or you can add the items to the object that was passed in and
     *   not return anything from the function. This supersedes `fnServerParams`
     *   from DataTables 1.9-.
     *
     * * `dataSrc` - By default DataTables will look for the property `data` (or
     *   `aaData` for compatibility with DataTables 1.9-) when obtaining data
     *   from an Ajax source or for server-side processing - this parameter
     *   allows that property to be changed. You can use Javascript dotted
     *   object notation to get a data source for multiple levels of nesting, or
     *   it my be used as a function. As a function it takes a single parameter,
     *   the JSON returned from the server, which can be manipulated as
     *   required, with the returned value being that used by DataTables as the
     *   data source for the table.
     *
     * * `success` - Should not be overridden it is used internally in
     *   DataTables. To manipulate / transform the data returned by the server
     *   use `ajax.dataSrc`, or use `ajax` as a function (see below).
     *
     * `function`
     * ----------
     *
     * As a function, making the Ajax call is left up to yourself allowing
     * complete control of the Ajax request. Indeed, if desired, a method other
     * than Ajax could be used to obtain the required data, such as Web storage
     * or an AIR database.
     *
     * The function is given four parameters and no return is required. The
     * parameters are:
     *
     * 1. _object_ - Data to send to the server
     * 2. _function_ - Callback function that must be executed when the required
     *    data has been obtained. That data should be passed into the callback
     *    as the only parameter
     * 3. _object_ - DataTables baton object for the table
     *
     * Note that this supersedes `fnServerData` from DataTables 1.9-.
     *
     *  @type string|object|function
     *  @default null
     *
     *  @dtopt Option
     *  @name DataTable.defaults.ajax
     *  @since 1.10.0
     *
     * @example
     *   // Get JSON data from a file via Ajax.
     *   // Note DataTables expects data in the form `{ data: [ ...data... ] }` by default).
     *   $('#example').dataTable( {
     *     "ajax": "data.json"
     *   } );
     *
     * @example
     *   // Get JSON data from a file via Ajax, using `dataSrc` to change
     *   // `data` to `tableData` (i.e. `{ tableData: [ ...data... ] }`)
     *   $('#example').dataTable( {
     *     "ajax": {
     *       "url": "data.json",
     *       "dataSrc": "tableData"
     *     }
     *   } );
     *
     * @example
     *   // Get JSON data from a file via Ajax, using `dataSrc` to read data
     *   // from a plain array rather than an array in an object
     *   $('#example').dataTable( {
     *     "ajax": {
     *       "url": "data.json",
     *       "dataSrc": ""
     *     }
     *   } );
     *
     * @example
     *   // Manipulate the data returned from the server - add a link to data
     *   // (note this can, should, be done using `render` for the column - this
     *   // is just a simple example of how the data can be manipulated).
     *   $('#example').dataTable( {
     *     "ajax": {
     *       "url": "data.json",
     *       "dataSrc": function ( json ) {
     *         for ( var i=0, ien=json.length ; i<ien ; i++ ) {
     *           json[i][0] = '<a href="/message/'+json[i][0]+'>View message</a>';
     *         }
     *         return json;
     *       }
     *     }
     *   } );
     *
     * @example
     *   // Add data to the request
     *   $('#example').dataTable( {
     *     "ajax": {
     *       "url": "data.json",
     *       "data": function ( d ) {
     *         return {
     *           "extra_search": $('#extra').val()
     *         };
     *       }
     *     }
     *   } );
     *
     * @example
     *   // Send request as POST
     *   $('#example').dataTable( {
     *     "ajax": {
     *       "url": "data.json",
     *       "type": "POST"
     *     }
     *   } );
     *
     * @example
     *   // Get the data from localStorage (could interface with a form for
     *   // adding, editing and removing rows).
     *   $('#example').dataTable( {
     *     "ajax": function (data, callback, baton) {
     *       callback(
     *         JSON.parse( localStorage.getItem('dataTablesData') )
     *       );
     *     }
     *   } );
     */
    "ajax": null,


    /**
     * This parameter allows you to readily specify the entries in the length drop
     * down menu that DataTables shows when pagination is enabled. It can be
     * either a 1D array of options which will be used for both the displayed
     * option and the value, or a 2D array which will use the array in the first
     * position as the value, and the array in the second position as the
     * displayed options (useful for language strings such as 'All').
     *
     * Note that the `pageLength` property will be automatically set to the
     * first value given in this array, unless `pageLength` is also provided.
     *  @type array
     *  @default [ 10, 25, 50, 100 ]
     *
     *  @dtopt Option
     *  @name DataTable.defaults.lengthMenu
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "lengthMenu": [[10, 25, 50, -1], [10, 25, 50, "All"]]
     *      } );
     *    } );
     */
    "aLengthMenu": [ 10, 25, 50, 100 ],


    /**
     * The `columns` option in the initialisation parameter allows you to define
     * details about the way individual columns behave. For a full list of
     * column options that can be set, please see
     * {@link DataTable.defaults.column}. Note that if you use `columns` to
     * define your columns, you must have an entry in the array for every single
     * column that you have in your table (these can be null if you don't which
     * to specify any options).
     *  @member
     *
     *  @name DataTable.defaults.column
     */
    "aoColumns": null,

    /**
     * Very similar to `columns`, `columnDefs` allows you to target a specific
     * column, multiple columns, or all columns, using the `targets` property of
     * each object in the array. This allows great flexibility when creating
     * tables, as the `columnDefs` arrays can be of any length, targeting the
     * columns you specifically want. `columnDefs` may use any of the column
     * options available: {@link DataTable.defaults.column}, but it _must_
     * have `targets` defined in each object in the array. Values in the `targets`
     * array may be:
     *   <ul>
     *     <li>a string - class name will be matched on the TH for the column</li>
     *     <li>0 or a positive integer - column index counting from the left</li>
     *     <li>a negative integer - column index counting from the right</li>
     *     <li>the string "_all" - all columns (i.e. assign a default)</li>
     *   </ul>
     *  @member
     *
     *  @name DataTable.defaults.columnDefs
     */
    "aoColumnDefs": null,


    /**
     * Basically the same as `search`, this parameter defines the individual column
     * filtering state at initialisation time. The array must be of the same size
     * as the number of columns, and each element be an object with the parameters
     * `search` and `escapeRegex` (the latter is optional). 'null' is also
     * accepted and the default will be used.
     *  @type array
     *  @default []
     *
     *  @dtopt Option
     *  @name DataTable.defaults.searchCols
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "searchCols": [
     *          null,
     *          { "search": "My filter" },
     *          null,
     *          { "search": "^[0-9]", "escapeRegex": false }
     *        ]
     *      } );
     *    } )
     */
    "aoSearchCols": [],


    /**
     * An array of CSS classes that should be applied to displayed rows. This
     * array may be of any length, and DataTables will apply each class
     * sequentially, looping when required.
     *  @type array
     *  @default null <i>Will take the values determined by the `oClasses.stripe*`
     *    options</i>
     *
     *  @dtopt Option
     *  @name DataTable.defaults.stripeClasses
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "stripeClasses": [ 'strip1', 'strip2', 'strip3' ]
     *      } );
     *    } )
     */
    "asStripeClasses": null,


    /**
     * Enable or disable automatic column width calculation. This can be disabled
     * as an optimisation (it takes some time to calculate the widths) if the
     * tables widths are passed in using `columns`.
     *  @type boolean
     *  @default true
     *
     *  @dtopt Features
     *  @name DataTable.defaults.autoWidth
     *
     *  @example
     *    $(document).ready( function () {
     *      $('#example').dataTable( {
     *        "autoWidth": false
     *      } );
     *    } );
     */
    "bAutoWidth": true,


    /**
     * Deferred rendering can provide DataTables with a huge speed boost when you
     * are using an Ajax or JS data source for the table. This option, when set to
     * true, will cause DataTables to defer the creation of the table elements for
     * each row until they are needed for a draw - saving a significant amount of
     * time.
     *  @type boolean
     *  @default false
     *
     *  @dtopt Features
     *  @name DataTable.defaults.deferRender
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "ajax": "sources/arrays.txt",
     *        "deferRender": true
     *      } );
     *    } );
     */
    "bDeferRender": false,


    /**
     * Replace a DataTable which matches the given selector and replace it with
     * one which has the properties of the new initialisation object passed. If no
     * table matches the selector, then the new DataTable will be constructed as
     * per normal.
     *  @type boolean
     *  @default false
     *
     *  @dtopt Options
     *  @name DataTable.defaults.destroy
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "srollY": "200px",
     *        "paginate": false
     *      } );
     *
     *      // Some time later....
     *      $('#example').dataTable( {
     *        "filter": false,
     *        "destroy": true
     *      } );
     *    } );
     */
    "bDestroy": false,


    /**
     * Enable or disable filtering of data. Filtering in DataTables is "smart" in
     * that it allows the end user to input multiple words (space separated) and
     * will match a row containing those words, even if not in the order that was
     * specified (this allow matching across multiple columns). Note that if you
     * wish to use filtering in DataTables this must remain 'true' - to remove the
     * default filtering input box and retain filtering abilities, please use
     * {@link DataTable.defaults.dom}.
     *  @type boolean
     *  @default true
     *
     *  @dtopt Features
     *  @name DataTable.defaults.searching
     *
     *  @example
     *    $(document).ready( function () {
     *      $('#example').dataTable( {
     *        "searching": false
     *      } );
     *    } );
     */
    "bFilter": true,


    /**
     * Enable or disable the table information display. This shows information
     * about the data that is currently visible on the page, including information
     * about filtered data if that action is being performed.
     *  @type boolean
     *  @default true
     *
     *  @dtopt Features
     *  @name DataTable.defaults.info
     *
     *  @example
     *    $(document).ready( function () {
     *      $('#example').dataTable( {
     *        "info": false
     *      } );
     *    } );
     */
    "bInfo": true,


    /**
     * Allows the end user to select the size of a formatted page from a select
     * menu (sizes are 10, 25, 50 and 100). Requires pagination (`paginate`).
     *  @type boolean
     *  @default true
     *
     *  @dtopt Features
     *  @name DataTable.defaults.lengthChange
     *
     *  @example
     *    $(document).ready( function () {
     *      $('#example').dataTable( {
     *        "lengthChange": false
     *      } );
     *    } );
     */
    "bLengthChange": true,


    /**
     * Enable or disable the display of a 'processing' indicator when the table is
     * being processed (e.g. a sort). This is particularly useful for tables with
     * large amounts of data where it can take a noticeable amount of time to sort
     * the entries.
     *  @type boolean
     *  @default false
     *
     *  @dtopt Features
     *  @name DataTable.defaults.processing
     *
     *  @example
     *    $(document).ready( function () {
     *      $('#example').dataTable( {
     *        "processing": true
     *      } );
     *    } );
     */
    "bProcessing": false,


    /**
     * Retrieve the DataTables object for the given selector. Note that if the
     * table has already been initialised, this parameter will cause DataTables
     * to simply return the object that has already been set up - it will not take
     * account of any changes you might have made to the initialisation object
     * passed to DataTables (setting this parameter to true is an acknowledgement
     * that you understand this). `destroy` can be used to reinitialise a table if
     * you need.
     *  @type boolean
     *  @default false
     *
     *  @dtopt Options
     *  @name DataTable.defaults.retrieve
     *
     *  @example
     *    $(document).ready( function() {
     *      initTable();
     *      tableActions();
     *    } );
     *
     *    function initTable ()
     *    {
     *      return $('#example').dataTable( {
     *        "scrollY": "200px",
     *        "paginate": false,
     *        "retrieve": true
     *      } );
     *    }
     *
     *    function tableActions ()
     *    {
     *      var table = initTable();
     *      // perform API operations with oTable
     *    }
     */
    "bRetrieve": false,


    /**
     * When vertical (y) scrolling is enabled, DataTables will force the height of
     * the table's viewport to the given height at all times (useful for layout).
     * However, this can look odd when filtering data down to a small data set,
     * and the footer is left "floating" further down. This parameter (when
     * enabled) will cause DataTables to collapse the table's viewport down when
     * the result set will fit within the given Y height.
     *  @type boolean
     *  @default false
     *
     *  @dtopt Options
     *  @name DataTable.defaults.scrollCollapse
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "scrollY": "200",
     *        "scrollCollapse": true
     *      } );
     *    } );
     */
    "bScrollCollapse": false,


    /**
     * Configure DataTables to use server-side processing. Note that the
     * `ajax` parameter must also be given in order to give DataTables a
     * source to obtain the required data for each draw.
     *  @type boolean
     *  @default false
     *
     *  @dtopt Features
     *  @dtopt Server-side
     *  @name DataTable.defaults.serverSide
     *
     *  @example
     *    $(document).ready( function () {
     *      $('#example').dataTable( {
     *        "serverSide": true,
     *        "ajax": "xhr.php"
     *      } );
     *    } );
     */
    "bServerSide": false,


    /**
     * Enable or disable sorting of columns. Sorting of individual columns can be
     * disabled by the `sortable` option for each column.
     *  @type boolean
     *  @default true
     *
     *  @dtopt Features
     *  @name DataTable.defaults.ordering
     *
     *  @example
     *    $(document).ready( function () {
     *      $('#example').dataTable( {
     *        "ordering": false
     *      } );
     *    } );
     */
    "bSort": true,


    /**
     * Enable or display DataTables' ability to sort multiple columns at the
     * same time (activated by shift-click by the user).
     *  @type boolean
     *  @default true
     *
     *  @dtopt Options
     *  @name DataTable.defaults.orderMulti
     *
     *  @example
     *    // Disable multiple column sorting ability
     *    $(document).ready( function () {
     *      $('#example').dataTable( {
     *        "orderMulti": false
     *      } );
     *    } );
     */
    "bSortMulti": true,


    /**
     * Allows control over whether DataTables should use the top (true) unique
     * cell that is found for a single column, or the bottom (false - default).
     * This is useful when using complex headers.
     *  @type boolean
     *  @default false
     *
     *  @dtopt Options
     *  @name DataTable.defaults.orderCellsTop
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "orderCellsTop": true
     *      } );
     *    } );
     */
    "bSortCellsTop": false,


    /**
     * Enable or disable the addition of the classes `sorting\_1`, `sorting\_2` and
     * `sorting\_3` to the columns which are currently being sorted on. This is
     * presented as a feature switch as it can increase processing time (while
     * classes are removed and added) so for large data sets you might want to
     * turn this off.
     *  @type boolean
     *  @default true
     *
     *  @dtopt Features
     *  @name DataTable.defaults.orderClasses
     *
     *  @example
     *    $(document).ready( function () {
     *      $('#example').dataTable( {
     *        "orderClasses": false
     *      } );
     *    } );
     */
    "bSortClasses": true,


    /**
     * This function is called when a TR element is created (and all TD child
     * elements have been inserted), or registered if using a DOM source, allowing
     * manipulation of the TR element (adding classes etc).
     *  @type function
     *  @param {node} row "TR" element for the current row
     *  @param {array} data Raw data array for this row
     *  @param {int} dataIndex The index of this row in the internal aoData array
     *
     *  @dtopt Callbacks
     *  @name DataTable.defaults.createdRow
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "createdRow": function( row, data, dataIndex ) {
     *          // Bold the grade for all 'A' grade browsers
     *          if ( data[4] == "A" )
     *          {
     *            $('td:eq(4)', row).html( '<b>A</b>' );
     *          }
     *        }
     *      } );
     *    } );
     */
    "fnCreatedRow": null,


    /**
     * This function is called on every 'draw' event, and allows you to
     * dynamically modify any aspect you want about the created DOM.
     *  @type function
     *  @param {object} baton DataTables baton object
     *
     *  @dtopt Callbacks
     *  @name DataTable.defaults.drawCallback
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "drawCallback": function( baton ) {
     *          alert( 'DataTables has redrawn the table' );
     *        }
     *      } );
     *    } );
     */
    "fnDrawCallback": null,


    /**
     * When rendering large numbers in the information element for the table
     * (i.e. "Showing 1 to 10 of 57 entries") DataTables will render large numbers
     * to have a comma separator for the 'thousands' units (e.g. 1 million is
     * rendered as "1,000,000") to help readability for the end user. This
     * function will override the default method DataTables uses.
     *  @type function
     *  @member
     *  @param {int} toFormat number to be formatted
     *  @returns {string} formatted string for DataTables to show the number
     *
     *  @dtopt Callbacks
     *  @name DataTable.defaults.formatNumber
     *
     *  @example
     *    // Format a number using a single quote for the separator (note that
     *    // this can also be done with the language.thousands option)
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "formatNumber": function ( toFormat ) {
     *          return toFormat.toString().replace(
     *            /\B(?=(\d{3})+(?!\d))/g, "'"
     *          );
     *        };
     *      } );
     *    } );
     */
    "fnFormatNumber": function ( toFormat ) {
      return toFormat.toString().replace(
        /\B(?=(\d{3})+(?!\d))/g,
        ","
      );
    },


    /**
     * This function is called on every 'draw' event, and allows you to
     * dynamically modify the header row. This can be used to calculate and
     * display useful information about the table.
     *  @type function
     *  @param {node} head "TR" element for the header
     *  @param {array} data Full table data (as derived from the original HTML)
     *  @param {int} start Index for the current display starting point in the
     *    display array
     *  @param {int} end Index for the current display ending point in the
     *    display array
     *  @param {array int} display Index array to translate the visual position
     *    to the full data array
     *
     *  @dtopt Callbacks
     *  @name DataTable.defaults.headerCallback
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "fheaderCallback": function( head, data, start, end, display ) {
     *          head.getElementsByTagName('th')[0].innerHTML = "Displaying "+(end-start)+" records";
     *        }
     *      } );
     *    } )
     */
    "fnHeaderCallback": null,


    /**
     * Called at the very start of each table draw and can be used to cancel the
     * draw by returning false, any other return (including undefined) results in
     * the full draw occurring).
     *  @type function
     *  @param {object} baton DataTables baton object
     *  @returns {boolean} False will cancel the draw, anything else (including no
     *    return) will allow it to complete.
     *
     *  @dtopt Callbacks
     *  @name DataTable.defaults.preDrawCallback
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "preDrawCallback": function( baton ) {
     *          if ( $('#test').val() == 1 ) {
     *            return false;
     *          }
     *        }
     *      } );
     *    } );
     */
    "fnPreDrawCallback": null,


    /**
     * This function allows you to 'post process' each row after it have been
     * generated for each table draw, but before it is rendered on screen. This
     * function might be used for setting the row class name etc.
     *  @type function
     *  @param {node} row "TR" element for the current row
     *  @param {array} data Raw data array for this row
     *  @param {int} displayIndex The display index for the current table draw
     *  @param {int} displayIndexFull The index of the data in the full list of
     *    rows (after filtering)
     *
     *  @dtopt Callbacks
     *  @name DataTable.defaults.rowCallback
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "rowCallback": function( row, data, displayIndex, displayIndexFull ) {
     *          // Bold the grade for all 'A' grade browsers
     *          if ( data[4] == "A" ) {
     *            $('td:eq(4)', row).html( '<b>A</b>' );
     *          }
     *        }
     *      } );
     *    } );
     */
    "fnRowCallback": null,


    /**
     * __Deprecated__ The functionality provided by this parameter has now been
     * superseded by that provided through `ajax`, which should be used instead.
     *
     * This parameter allows you to override the default function which obtains
     * the data from the server so something more suitable for your application.
     * For example you could use POST data, or pull information from a Gears or
     * AIR database.
     *  @type function
     *  @member
     *  @param {string} source HTTP source to obtain the data from (`ajax`)
     *  @param {array} data A key/value pair object containing the data to send
     *    to the server
     *  @param {function} callback to be called on completion of the data get
     *    process that will draw the data on the page.
     *  @param {object} baton DataTables baton object
     *
     *  @dtopt Callbacks
     *  @dtopt Server-side
     *  @name DataTable.defaults.serverData
     *
     *  @deprecated 1.10. Please use `ajax` for this functionality now.
     */
    "fnServerData": null,


    /**
     * __Deprecated__ The functionality provided by this parameter has now been
     * superseded by that provided through `ajax`, which should be used instead.
     *
     *  It is often useful to send extra data to the server when making an Ajax
     * request - for example custom filtering information, and this callback
     * function makes it trivial to send extra information to the server. The
     * passed in parameter is the data set that has been constructed by
     * DataTables, and you can add to this or modify it as you require.
     *  @type function
     *  @param {array} data Data array (array of objects which are name/value
     *    pairs) that has been constructed by DataTables and will be sent to the
     *    server. In the case of Ajax sourced data with server-side processing
     *    this will be an empty array, for server-side processing there will be a
     *    significant number of parameters!
     *  @returns {undefined} Ensure that you modify the data array passed in,
     *    as this is passed by reference.
     *
     *  @dtopt Callbacks
     *  @dtopt Server-side
     *  @name DataTable.defaults.serverParams
     *
     *  @deprecated 1.10. Please use `ajax` for this functionality now.
     */
    "fnServerParams": null,


    /**
     * When enabled DataTables will not make a request to the server for the first
     * page draw - rather it will use the data already on the page (no sorting etc
     * will be applied to it), thus saving on an XHR at load time. `deferLoading`
     * is used to indicate that deferred loading is required, but it is also used
     * to tell DataTables how many records there are in the full table (allowing
     * the information element and pagination to be displayed correctly). In the case
     * where a filtering is applied to the table on initial load, this can be
     * indicated by giving the parameter as an array, where the first element is
     * the number of records available after filtering and the second element is the
     * number of records without filtering (allowing the table information element
     * to be shown correctly).
     *  @type int | array
     *  @default null
     *
     *  @dtopt Options
     *  @name DataTable.defaults.deferLoading
     *
     *  @example
     *    // 57 records available in the table, no filtering applied
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "serverSide": true,
     *        "ajax": "scripts/server_processing.php",
     *        "deferLoading": 57
     *      } );
     *    } );
     *
     *  @example
     *    // 57 records after filtering, 100 without filtering (an initial filter applied)
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "serverSide": true,
     *        "ajax": "scripts/server_processing.php",
     *        "deferLoading": [ 57, 100 ],
     *        "search": {
     *          "search": "my_filter"
     *        }
     *      } );
     *    } );
     */
    "iDeferLoading": null,


    /**
     * Number of rows to display on a single page when using pagination. If
     * feature enabled (`lengthChange`) then the end user will be able to override
     * this to a custom setting using a pop-up menu.
     *  @type int
     *  @default 10
     *
     *  @dtopt Options
     *  @name DataTable.defaults.pageLength
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "pageLength": 50
     *      } );
     *    } )
     */
    "iDisplayLength": 10,


    /**
     * Define the starting point for data display when using DataTables with
     * pagination. Note that this parameter is the number of records, rather than
     * the page number, so if you have 10 records per page and want to start on
     * the third page, it should be "20".
     *  @type int
     *  @default 0
     *
     *  @dtopt Options
     *  @name DataTable.defaults.displayStart
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "displayStart": 20
     *      } );
     *    } )
     */
    "iDisplayStart": 0,


    /**
     * By default DataTables allows keyboard navigation of the table (sorting, paging,
     * and filtering) by adding a `tabindex` attribute to the required elements. This
     * allows you to tab through the controls and press the enter key to activate them.
     * The tabindex is default 0, meaning that the tab follows the flow of the document.
     * You can overrule this using this parameter if you wish. Use a value of -1 to
     * disable built-in keyboard navigation.
     *  @type int
     *  @default 0
     *
     *  @dtopt Options
     *  @name DataTable.defaults.tabIndex
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "tabIndex": 1
     *      } );
     *    } );
     */
    "iTabIndex": 0,


    /**
     * Classes that DataTables assigns to the various components and features
     * that it adds to the HTML table. This allows classes to be configured
     * during initialisation in addition to through the static
     * {@link DataTable.ext.oStdClasses} object).
     *  @namespace
     *  @name DataTable.defaults.classes
     */
    "oClasses": {},

    /**
     * All strings that DataTables uses in the user interface that it creates
     * are defined in this object, allowing you to modified them individually or
     * completely replace them all as required.
     *  @namespace
     *  @name DataTable.defaults.language
     */
    "oLanguage": {
      /**
       * When using Ajax sourced data and during the first draw when DataTables is
       * gathering the data, this message is shown in an empty row in the table to
       * indicate to the end user the the data is being loaded. Note that this
       * parameter is not used when loading data by server-side processing, just
       * Ajax sourced data with client-side processing.
       *  @type string
       *  @default Loading...
       *
       *  @dtopt Language
       *  @name DataTable.defaults.language.loadingRecords
       *
       *  @example
       *    $(document).ready( function() {
       *      $('#example').dataTable( {
       *        "language": {
       *          "loadingRecords": "Please wait - loading..."
       *        }
       *      } );
       *    } );
       */
      "sLoadingRecords": "Loading...",


      /**
       * Text which is displayed when the table is processing a user action
       * (usually a sort command or similar).
       *  @type string
       *  @default Processing...
       *
       *  @dtopt Language
       *  @name DataTable.defaults.language.processing
       *
       *  @example
       *    $(document).ready( function() {
       *      $('#example').dataTable( {
       *        "language": {
       *          "processing": "DataTables is currently busy"
       *        }
       *      } );
       *    } );
       */
      "sProcessing": "Processing...",


      /**
       * Details the actions that will be taken when the user types into the
       * filtering input text box. The variable "_INPUT_", if used in the string,
       * is replaced with the HTML text box for the filtering input allowing
       * control over where it appears in the string. If "_INPUT_" is not given
       * then the input box is appended to the string automatically.
       *  @type string
       *  @default Search:
       *
       *  @dtopt Language
       *  @name DataTable.defaults.language.search
       *
       *  @example
       *    // Input text box will be appended at the end automatically
       *    $(document).ready( function() {
       *      $('#example').dataTable( {
       *        "language": {
       *          "search": "Filter records:"
       *        }
       *      } );
       *    } );
       *
       *  @example
       *    // Specify where the filter should appear
       *    $(document).ready( function() {
       *      $('#example').dataTable( {
       *        "language": {
       *          "search": "Apply filter _INPUT_ to table"
       *        }
       *      } );
       *    } );
       */
      "sSearch": "Search:",


      /**
       * Assign a `placeholder` attribute to the search `input` element
       *  @type string
       *  @default
       *
       *  @dtopt Language
       *  @name DataTable.defaults.language.searchPlaceholder
       */
      "sSearchPlaceholder": "",


      /**
       * Text shown inside the table records when the is no information to be
       * displayed after filtering. `emptyTable` is shown when there is simply no
       * information in the table at all (regardless of filtering).
       *  @type string
       *  @default No matching records found
       *
       *  @dtopt Language
       *  @name DataTable.defaults.language.zeroRecords
       *
       *  @example
       *    $(document).ready( function() {
       *      $('#example').dataTable( {
       *        "language": {
       *          "zeroRecords": "No records to display"
       *        }
       *      } );
       *    } );
       */
      "sZeroRecords": "No matching records found"
    },


    /**
     * This parameter allows you to have define the global filtering state at
     * initialisation time. As an object the `search` parameter must be
     * defined, but all other parameters are optional. When `regex` is true,
     * the search string will be treated as a regular expression, when false
     * (default) it will be treated as a straight string. When `smart`
     * DataTables will use it's smart filtering methods (to word match at
     * any point in the data), when false this will not be done.
     *  @namespace
     *  @extends DataTable.models.oSearch
     *
     *  @dtopt Options
     *  @name DataTable.defaults.search
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "search": {"search": "Initial search"}
     *      } );
     *    } )
     */
    "oSearch": $.extend( {}, DataTable.models.oSearch ),


    /**
     * This initialisation variable allows you to specify exactly where in the
     * DOM you want DataTables to inject the various controls it adds to the page
     * (for example you might want the pagination controls at the top of the
     * table). DIV elements (with or without a custom class) can also be added to
     * aid styling. The follow syntax is used:
     *   <ul>
     *     <li>The following options are allowed:
     *       <ul>
     *         <li>'l' - Length changing</li>
     *         <li>'f' - Filtering input</li>
     *         <li>'t' - The table!</li>
     *         <li>'i' - Information</li>
     *         <li>'p' - Pagination</li>
     *         <li>'r' - pRocessing</li>
     *       </ul>
     *     </li>
     *     <li>The following constants are allowed:
     *       <ul>
     *         <li>'H' - jQueryUI theme "header" classes ('fg-toolbar ui-widget-header ui-corner-tl ui-corner-tr ui-helper-clearfix')</li>
     *         <li>'F' - jQueryUI theme "footer" classes ('fg-toolbar ui-widget-header ui-corner-bl ui-corner-br ui-helper-clearfix')</li>
     *       </ul>
     *     </li>
     *     <li>The following syntax is expected:
     *       <ul>
     *         <li>'&lt;' and '&gt;' - div elements</li>
     *         <li>'&lt;"class" and '&gt;' - div with a class</li>
     *         <li>'&lt;"#id" and '&gt;' - div with an ID</li>
     *       </ul>
     *     </li>
     *     <li>Examples:
     *       <ul>
     *         <li>'&lt;"wrapper"flipt&gt;'</li>
     *         <li>'&lt;lf&lt;t&gt;ip&gt;'</li>
     *       </ul>
     *     </li>
     *   </ul>
     *  @type string
     *  @default lfrtip <i>(when `jQueryUI` is false)</i> <b>or</b>
     *    <"H"lfr>t<"F"ip> <i>(when `jQueryUI` is true)</i>
     *
     *  @dtopt Options
     *  @name DataTable.defaults.dom
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "dom": '&lt;"top"i&gt;rt&lt;"bottom"flp&gt;&lt;"clear"&gt;'
     *      } );
     *    } );
     */
    "sDom": "lfrtip",


    /**
     * Search delay option. This will throttle full table searches that use the
     * DataTables provided search input element (it does not effect calls to
     * `dt-api search()`, providing a delay before the search is made.
     *  @type integer
     *  @default 0
     *
     *  @dtopt Options
     *  @name DataTable.defaults.searchDelay
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "searchDelay": 200
     *      } );
     *    } )
     */
    "searchDelay": null,


    /**
     * DataTables features six different built-in options for the buttons to
     * display for pagination control:
     *
     * * `numbers` - Page number buttons only
     * * `simple` - 'Previous' and 'Next' buttons only
     * * 'simple_numbers` - 'Previous' and 'Next' buttons, plus page numbers
     * * `full` - 'First', 'Previous', 'Next' and 'Last' buttons
     * * `full_numbers` - 'First', 'Previous', 'Next' and 'Last' buttons, plus page numbers
     * * `first_last_numbers` - 'First' and 'Last' buttons, plus page numbers
     *
     * Further methods can be added using {@link DataTable.ext.oPagination}.
     *  @type string
     *  @default simple_numbers
     *
     *  @dtopt Options
     *  @name DataTable.defaults.pagingType
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "pagingType": "full_numbers"
     *      } );
     *    } )
     */
    "sPaginationType": "simple_numbers",


    /**
     * Enable horizontal scrolling. When a table is too wide to fit into a
     * certain layout, or you have a large number of columns in the table, you
     * can enable x-scrolling to show the table in a viewport, which can be
     * scrolled. This property can be `true` which will allow the table to
     * scroll horizontally when needed, or any CSS unit, or a number (in which
     * case it will be treated as a pixel measurement). Setting as simply `true`
     * is recommended.
     *  @type boolean|string
     *  @default <i>blank string - i.e. disabled</i>
     *
     *  @dtopt Features
     *  @name DataTable.defaults.scrollX
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "scrollX": true,
     *        "scrollCollapse": true
     *      } );
     *    } );
     */
    "sScrollX": "",


    /**
     * This property can be used to force a DataTable to use more width than it
     * might otherwise do when x-scrolling is enabled. For example if you have a
     * table which requires to be well spaced, this parameter is useful for
     * "over-sizing" the table, and thus forcing scrolling. This property can by
     * any CSS unit, or a number (in which case it will be treated as a pixel
     * measurement).
     *  @type string
     *  @default <i>blank string - i.e. disabled</i>
     *
     *  @dtopt Options
     *  @name DataTable.defaults.scrollXInner
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "scrollX": "100%",
     *        "scrollXInner": "110%"
     *      } );
     *    } );
     */
    "sScrollXInner": "",


    /**
     * Enable vertical scrolling. Vertical scrolling will constrain the DataTable
     * to the given height, and enable scrolling for any data which overflows the
     * current viewport. This can be used as an alternative to paging to display
     * a lot of data in a small area (although paging and scrolling can both be
     * enabled at the same time). This property can be any CSS unit, or a number
     * (in which case it will be treated as a pixel measurement).
     *  @type string
     *  @default <i>blank string - i.e. disabled</i>
     *
     *  @dtopt Features
     *  @name DataTable.defaults.scrollY
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "scrollY": "200px",
     *        "paginate": false
     *      } );
     *    } );
     */
    "sScrollY": "",


    /**
     * __Deprecated__ The functionality provided by this parameter has now been
     * superseded by that provided through `ajax`, which should be used instead.
     *
     * Set the HTTP method that is used to make the Ajax call for server-side
     * processing or Ajax sourced data.
     *  @type string
     *  @default GET
     *
     *  @dtopt Options
     *  @dtopt Server-side
     *  @name DataTable.defaults.serverMethod
     *
     *  @deprecated 1.10. Please use `ajax` for this functionality now.
     */
    "sServerMethod": "GET",


    /**
     * DataTables makes use of renderers when displaying HTML elements for
     * a table. These renderers can be added or modified by plug-ins to
     * generate suitable mark-up for a site. For example the Bootstrap
     * integration plug-in for DataTables uses a paging button renderer to
     * display pagination buttons in the mark-up required by Bootstrap.
     *
     * For further information about the renderers available see
     * DataTable.ext.renderer
     *  @type string|object
     *  @default null
     *
     *  @name DataTable.defaults.renderer
     *
     */
    "renderer": null,


    /**
     * Set the data property name that DataTables should use to get a row's id
     * to set as the `id` property in the node.
     *  @type string
     *  @default DT_RowId
     *
     *  @name DataTable.defaults.rowId
     */
    "rowId": "DT_RowId"
  };

  _fnHungarianMap( DataTable.defaults );


  /*
   * Developer note - See note in model.defaults.js about the use of Hungarian
   * notation and camel case.
   */

  /**
   * Column options that can be given to DataTables at initialisation time.
   *  @namespace
   */
  DataTable.defaults.column = {
    /**
     * Define which column(s) an order will occur on for this column. This
     * allows a column's ordering to take multiple columns into account when
     * doing a sort or use the data from a different column. For example first
     * name / last name columns make sense to do a multi-column sort over the
     * two columns.
     *  @type array|int
     *  @default null <i>Takes the value of the column index automatically</i>
     *
     *  @name DataTable.defaults.column.orderData
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columnDefs`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [
     *          { "orderData": [ 0, 1 ], "targets": [ 0 ] },
     *          { "orderData": [ 1, 0 ], "targets": [ 1 ] },
     *          { "orderData": 2, "targets": [ 2 ] }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          { "orderData": [ 0, 1 ] },
     *          { "orderData": [ 1, 0 ] },
     *          { "orderData": 2 },
     *          null,
     *          null
     *        ]
     *      } );
     *    } );
     */
    "aDataSort": null,
    "iDataSort": -1,


    /**
     * You can control the default ordering direction, and even alter the
     * behaviour of the sort handler (i.e. only allow ascending ordering etc)
     * using this parameter.
     *  @type array
     *  @default [ 'asc', 'desc' ]
     *
     *  @name DataTable.defaults.column.orderSequence
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columnDefs`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [
     *          { "orderSequence": [ "asc" ], "targets": [ 1 ] },
     *          { "orderSequence": [ "desc", "asc", "asc" ], "targets": [ 2 ] },
     *          { "orderSequence": [ "desc" ], "targets": [ 3 ] }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          null,
     *          { "orderSequence": [ "asc" ] },
     *          { "orderSequence": [ "desc", "asc", "asc" ] },
     *          { "orderSequence": [ "desc" ] },
     *          null
     *        ]
     *      } );
     *    } );
     */
    "asSorting": [ 'asc', 'desc' ],


    /**
     * Enable or disable filtering on the data in this column.
     *  @type boolean
     *  @default true
     *
     *  @name DataTable.defaults.column.searchable
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columnDefs`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [
     *          { "searchable": false, "targets": [ 0 ] }
     *        ] } );
     *    } );
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          { "searchable": false },
     *          null,
     *          null,
     *          null,
     *          null
     *        ] } );
     *    } );
     */
    "bSearchable": true,


    /**
     * Enable or disable ordering on this column.
     *  @type boolean
     *  @default true
     *
     *  @name DataTable.defaults.column.orderable
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columnDefs`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [
     *          { "orderable": false, "targets": [ 0 ] }
     *        ] } );
     *    } );
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          { "orderable": false },
     *          null,
     *          null,
     *          null,
     *          null
     *        ] } );
     *    } );
     */
    "bSortable": true,


    /**
     * Enable or disable the display of this column.
     *  @type boolean
     *  @default true
     *
     *  @name DataTable.defaults.column.visible
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columnDefs`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [
     *          { "visible": false, "targets": [ 0 ] }
     *        ] } );
     *    } );
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          { "visible": false },
     *          null,
     *          null,
     *          null,
     *          null
     *        ] } );
     *    } );
     */
    "bVisible": true,


    /**
     * Developer definable function that is called whenever a cell is created (Ajax source,
     * etc) or processed for input (DOM source). This can be used as a compliment to mRender
     * allowing you to modify the DOM element (add background colour for example) when the
     * element is available.
     *  @type function
     *  @param {element} td The TD node that has been created
     *  @param {*} cellData The Data for the cell
     *  @param {array|object} rowData The data for the whole row
     *  @param {int} row The row index for the aoData data store
     *  @param {int} col The column index for aoColumns
     *
     *  @name DataTable.defaults.column.createdCell
     *  @dtopt Columns
     *
     *  @example
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [ {
     *          "targets": [3],
     *          "createdCell": function (td, cellData, rowData, row, col) {
     *            if ( cellData == "1.7" ) {
     *              $(td).css('color', 'blue')
     *            }
     *          }
     *        } ]
     *      });
     *    } );
     */
    "fnCreatedCell": null,


    /**
     * This parameter has been replaced by `data` in DataTables to ensure naming
     * consistency. `dataProp` can still be used, as there is backwards
     * compatibility in DataTables for this option, but it is strongly
     * recommended that you use `data` in preference to `dataProp`.
     *  @name DataTable.defaults.column.dataProp
     */


    /**
     * This property can be used to read data from any data source property,
     * including deeply nested objects / properties. `data` can be given in a
     * number of different ways which effect its behaviour:
     *
     * * `integer` - treated as an array index for the data source. This is the
     *   default that DataTables uses (incrementally increased for each column).
     * * `string` - read an object property from the data source. There are
     *   three 'special' options that can be used in the string to alter how
     *   DataTables reads the data from the source object:
     *    * `.` - Dotted Javascript notation. Just as you use a `.` in
     *      Javascript to read from nested objects, so to can the options
     *      specified in `data`. For example: `browser.version` or
     *      `browser.name`. If your object parameter name contains a period, use
     *      `\\` to escape it - i.e. `first\\.name`.
     *    * `[]` - Array notation. DataTables can automatically combine data
     *      from and array source, joining the data with the characters provided
     *      between the two brackets. For example: `name[, ]` would provide a
     *      comma-space separated list from the source array. If no characters
     *      are provided between the brackets, the original array source is
     *      returned.
     *    * `()` - Function notation. Adding `()` to the end of a parameter will
     *      execute a function of the name given. For example: `browser()` for a
     *      simple function on the data source, `browser.version()` for a
     *      function in a nested property or even `browser().version` to get an
     *      object property if the function called returns an object. Note that
     *      function notation is recommended for use in `render` rather than
     *      `data` as it is much simpler to use as a renderer.
     * * `null` - use the original data source for the row rather than plucking
     *   data directly from it. This action has effects on two other
     *   initialisation options:
     *    * `defaultContent` - When null is given as the `data` option and
     *      `defaultContent` is specified for the column, the value defined by
     *      `defaultContent` will be used for the cell.
     *    * `render` - When null is used for the `data` option and the `render`
     *      option is specified for the column, the whole data source for the
     *      row is used for the renderer.
     * * `function` - the function given will be executed whenever DataTables
     *   needs to set or get the data for a cell in the column. The function
     *   takes three parameters:
     *    * Parameters:
     *      * `{array|object}` The data source for the row
     *      * `{string}` The type call data requested - this will be 'set' when
     *        setting data or 'filter', 'display', 'type', 'sort' or undefined
     *        when gathering data. Note that when `undefined` is given for the
     *        type DataTables expects to get the raw data for the object back<
     *      * `{*}` Data to set when the second parameter is 'set'.
     *    * Return:
     *      * The return value from the function is not required when 'set' is
     *        the type of call, but otherwise the return is what will be used
     *        for the data requested.
     *
     * Note that `data` is a getter and setter option. If you just require
     * formatting of data for output, you will likely want to use `render` which
     * is simply a getter and thus simpler to use.
     *
     * Note that prior to DataTables 1.9.2 `data` was called `mDataProp`. The
     * name change reflects the flexibility of this property and is consistent
     * with the naming of mRender. If 'mDataProp' is given, then it will still
     * be used by DataTables, as it automatically maps the old name to the new
     * if required.
     *
     *  @type string|int|function|null
     *  @default null <i>Use automatically calculated column index</i>
     *
     *  @name DataTable.defaults.column.data
     *  @dtopt Columns
     *
     *  @example
     *    // Read table data from objects
     *    // JSON structure for each row:
     *    //   {
     *    //      "engine": {value},
     *    //      "browser": {value},
     *    //      "platform": {value},
     *    //      "version": {value},
     *    //      "grade": {value}
     *    //   }
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "ajaxSource": "sources/objects.txt",
     *        "columns": [
     *          { "data": "engine" },
     *          { "data": "browser" },
     *          { "data": "platform" },
     *          { "data": "version" },
     *          { "data": "grade" }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Read information from deeply nested objects
     *    // JSON structure for each row:
     *    //   {
     *    //      "engine": {value},
     *    //      "browser": {value},
     *    //      "platform": {
     *    //         "inner": {value}
     *    //      },
     *    //      "details": [
     *    //         {value}, {value}
     *    //      ]
     *    //   }
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "ajaxSource": "sources/deep.txt",
     *        "columns": [
     *          { "data": "engine" },
     *          { "data": "browser" },
     *          { "data": "platform.inner" },
     *          { "data": "details.0" },
     *          { "data": "details.1" }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using `data` as a function to provide different information for
     *    // sorting, filtering and display. In this case, currency (price)
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [ {
     *          "targets": [ 0 ],
     *          "data": function ( source, type, val ) {
     *            if (type === 'set') {
     *              source.price = val;
     *              // Store the computed dislay and filter values for efficiency
     *              source.price_display = val=="" ? "" : "$"+numberFormat(val);
     *              source.price_filter  = val=="" ? "" : "$"+numberFormat(val)+" "+val;
     *              return;
     *            }
     *            else if (type === 'display') {
     *              return source.price_display;
     *            }
     *            else if (type === 'filter') {
     *              return source.price_filter;
     *            }
     *            // 'sort', 'type' and undefined all just use the integer
     *            return source.price;
     *          }
     *        } ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using default content
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [ {
     *          "targets": [ 0 ],
     *          "data": null,
     *          "defaultContent": "Click to edit"
     *        } ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using array notation - outputting a list from an array
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [ {
     *          "targets": [ 0 ],
     *          "data": "name[, ]"
     *        } ]
     *      } );
     *    } );
     *
     */
    "mData": null,


    /**
     * This property is the rendering partner to `data` and it is suggested that
     * when you want to manipulate data for display (including filtering,
     * sorting etc) without altering the underlying data for the table, use this
     * property. `render` can be considered to be the the read only companion to
     * `data` which is read / write (then as such more complex). Like `data`
     * this option can be given in a number of different ways to effect its
     * behaviour:
     *
     * * `integer` - treated as an array index for the data source. This is the
     *   default that DataTables uses (incrementally increased for each column).
     * * `string` - read an object property from the data source. There are
     *   three 'special' options that can be used in the string to alter how
     *   DataTables reads the data from the source object:
     *    * `.` - Dotted Javascript notation. Just as you use a `.` in
     *      Javascript to read from nested objects, so to can the options
     *      specified in `data`. For example: `browser.version` or
     *      `browser.name`. If your object parameter name contains a period, use
     *      `\\` to escape it - i.e. `first\\.name`.
     *    * `[]` - Array notation. DataTables can automatically combine data
     *      from and array source, joining the data with the characters provided
     *      between the two brackets. For example: `name[, ]` would provide a
     *      comma-space separated list from the source array. If no characters
     *      are provided between the brackets, the original array source is
     *      returned.
     *    * `()` - Function notation. Adding `()` to the end of a parameter will
     *      execute a function of the name given. For example: `browser()` for a
     *      simple function on the data source, `browser.version()` for a
     *      function in a nested property or even `browser().version` to get an
     *      object property if the function called returns an object.
     * * `object` - use different data for the different data types requested by
     *   DataTables ('filter', 'display', 'type' or 'sort'). The property names
     *   of the object is the data type the property refers to and the value can
     *   defined using an integer, string or function using the same rules as
     *   `render` normally does. Note that an `_` option _must_ be specified.
     *   This is the default value to use if you haven't specified a value for
     *   the data type requested by DataTables.
     * * `function` - the function given will be executed whenever DataTables
     *   needs to set or get the data for a cell in the column. The function
     *   takes three parameters:
     *    * Parameters:
     *      * {array|object} The data source for the row (based on `data`)
     *      * {string} The type call data requested - this will be 'filter',
     *        'display', 'type' or 'sort'.
     *      * {array|object} The full data source for the row (not based on
     *        `data`)
     *    * Return:
     *      * The return value from the function is what will be used for the
     *        data requested.
     *
     *  @type string|int|function|object|null
     *  @default null Use the data source value.
     *
     *  @name DataTable.defaults.column.render
     *  @dtopt Columns
     *
     *  @example
     *    // Create a comma separated list from an array of objects
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "ajaxSource": "sources/deep.txt",
     *        "columns": [
     *          { "data": "engine" },
     *          { "data": "browser" },
     *          {
     *            "data": "platform",
     *            "render": "[, ].name"
     *          }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Execute a function to obtain data
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [ {
     *          "targets": [ 0 ],
     *          "data": null, // Use the full data source object for the renderer's source
     *          "render": "browserName()"
     *        } ]
     *      } );
     *    } );
     *
     *  @example
     *    // As an object, extracting different data for the different types
     *    // This would be used with a data source such as:
     *    //   { "phone": 5552368, "phone_filter": "5552368 555-2368", "phone_display": "555-2368" }
     *    // Here the `phone` integer is used for sorting and type detection, while `phone_filter`
     *    // (which has both forms) is used for filtering for if a user inputs either format, while
     *    // the formatted phone number is the one that is shown in the table.
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [ {
     *          "targets": [ 0 ],
     *          "data": null, // Use the full data source object for the renderer's source
     *          "render": {
     *            "_": "phone",
     *            "filter": "phone_filter",
     *            "display": "phone_display"
     *          }
     *        } ]
     *      } );
     *    } );
     *
     *  @example
     *    // Use as a function to create a link from the data source
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [ {
     *          "targets": [ 0 ],
     *          "data": "download_link",
     *          "render": function ( data, type, full ) {
     *            return '<a href="'+data+'">Download</a>';
     *          }
     *        } ]
     *      } );
     *    } );
     */
    "mRender": null,


    /**
     * Change the cell type created for the column - either TD cells or TH cells. This
     * can be useful as TH cells have semantic meaning in the table body, allowing them
     * to act as a header for a row (you may wish to add scope='row' to the TH elements).
     *  @type string
     *  @default td
     *
     *  @name DataTable.defaults.column.cellType
     *  @dtopt Columns
     *
     *  @example
     *    // Make the first column use TH cells
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [ {
     *          "targets": [ 0 ],
     *          "cellType": "th"
     *        } ]
     *      } );
     *    } );
     */
    "sCellType": "td",


    /**
     * Class to give to each cell in this column.
     *  @type string
     *  @default <i>Empty string</i>
     *
     *  @name DataTable.defaults.column.class
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columnDefs`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [
     *          { "class": "my_class", "targets": [ 0 ] }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          { "class": "my_class" },
     *          null,
     *          null,
     *          null,
     *          null
     *        ]
     *      } );
     *    } );
     */
    "sClass": "",

    /**
     * When DataTables calculates the column widths to assign to each column,
     * it finds the longest string in each column and then constructs a
     * temporary table and reads the widths from that. The problem with this
     * is that "mmm" is much wider then "iiii", but the latter is a longer
     * string - thus the calculation can go wrong (doing it properly and putting
     * it into an DOM object and measuring that is horribly(!) slow). Thus as
     * a "work around" we provide this option. It will append its value to the
     * text that is found to be the longest string for the column - i.e. padding.
     * Generally you shouldn't need this!
     *  @type string
     *  @default <i>Empty string<i>
     *
     *  @name DataTable.defaults.column.contentPadding
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          null,
     *          null,
     *          null,
     *          {
     *            "contentPadding": "mmm"
     *          }
     *        ]
     *      } );
     *    } );
     */
    "sContentPadding": "",


    /**
     * Allows a default value to be given for a column's data, and will be used
     * whenever a null data source is encountered (this can be because `data`
     * is set to null, or because the data source itself is null).
     *  @type string
     *  @default null
     *
     *  @name DataTable.defaults.column.defaultContent
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columnDefs`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [
     *          {
     *            "data": null,
     *            "defaultContent": "Edit",
     *            "targets": [ -1 ]
     *          }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          null,
     *          null,
     *          null,
     *          {
     *            "data": null,
     *            "defaultContent": "Edit"
     *          }
     *        ]
     *      } );
     *    } );
     */
    "sDefaultContent": null,


    /**
     * This parameter is only used in DataTables' server-side processing. It can
     * be exceptionally useful to know what columns are being displayed on the
     * client side, and to map these to database fields. When defined, the names
     * also allow DataTables to reorder information from the server if it comes
     * back in an unexpected order (i.e. if you switch your columns around on the
     * client-side, your server-side code does not also need updating).
     *  @type string
     *  @default <i>Empty string</i>
     *
     *  @name DataTable.defaults.column.name
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columnDefs`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [
     *          { "name": "engine", "targets": [ 0 ] },
     *          { "name": "browser", "targets": [ 1 ] },
     *          { "name": "platform", "targets": [ 2 ] },
     *          { "name": "version", "targets": [ 3 ] },
     *          { "name": "grade", "targets": [ 4 ] }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          { "name": "engine" },
     *          { "name": "browser" },
     *          { "name": "platform" },
     *          { "name": "version" },
     *          { "name": "grade" }
     *        ]
     *      } );
     *    } );
     */
    "sName": "",


    /**
     * Defines a data source type for the ordering which can be used to read
     * real-time information from the table (updating the internally cached
     * version) prior to ordering. This allows ordering to occur on user
     * editable elements such as form inputs.
     *  @type string
     *  @default std
     *
     *  @name DataTable.defaults.column.orderDataType
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columnDefs`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [
     *          { "orderDataType": "dom-text", "targets": [ 2, 3 ] },
     *          { "type": "numeric", "targets": [ 3 ] },
     *          { "orderDataType": "dom-select", "targets": [ 4 ] },
     *          { "orderDataType": "dom-checkbox", "targets": [ 5 ] }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          null,
     *          null,
     *          { "orderDataType": "dom-text" },
     *          { "orderDataType": "dom-text", "type": "numeric" },
     *          { "orderDataType": "dom-select" },
     *          { "orderDataType": "dom-checkbox" }
     *        ]
     *      } );
     *    } );
     */
    "sSortDataType": "std",


    /**
     * The title of this column.
     *  @type string
     *  @default null <i>Derived from the 'TH' value for this column in the
     *    original HTML table.</i>
     *
     *  @name DataTable.defaults.column.title
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columnDefs`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [
     *          { "title": "My column title", "targets": [ 0 ] }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          { "title": "My column title" },
     *          null,
     *          null,
     *          null,
     *          null
     *        ]
     *      } );
     *    } );
     */
    "sTitle": null,


    /**
     * The type allows you to specify how the data for this column will be
     * ordered. Four types (string, numeric, date and html (which will strip
     * HTML tags before ordering)) are currently available. Note that only date
     * formats understood by Javascript's Date() object will be accepted as type
     * date. For example: "Mar 26, 2008 5:03 PM". May take the values: 'string',
     * 'numeric', 'date' or 'html' (by default). Further types can be adding
     * through plug-ins.
     *  @type string
     *  @default null <i>Auto-detected from raw data</i>
     *
     *  @name DataTable.defaults.column.type
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columnDefs`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [
     *          { "type": "html", "targets": [ 0 ] }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          { "type": "html" },
     *          null,
     *          null,
     *          null,
     *          null
     *        ]
     *      } );
     *    } );
     */
    "sType": null,


    /**
     * Defining the width of the column, this parameter may take any CSS value
     * (3em, 20px etc). DataTables applies 'smart' widths to columns which have not
     * been given a specific width through this interface ensuring that the table
     * remains readable.
     *  @type string
     *  @default null <i>Automatic</i>
     *
     *  @name DataTable.defaults.column.width
     *  @dtopt Columns
     *
     *  @example
     *    // Using `columnDefs`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columnDefs": [
     *          { "width": "20%", "targets": [ 0 ] }
     *        ]
     *      } );
     *    } );
     *
     *  @example
     *    // Using `columns`
     *    $(document).ready( function() {
     *      $('#example').dataTable( {
     *        "columns": [
     *          { "width": "20%" },
     *          null,
     *          null,
     *          null,
     *          null
     *        ]
     *      } );
     *    } );
     */
    "sWidth": null
  };

  _fnHungarianMap( DataTable.defaults.column );


  /**
   * DataTables baton object - this holds all the information needed for a
   * given table, including configuration, data and current application of the
   * table options. DataTables does not have a single instance for each DataTable
   * with the baton attached to that instance, but rather instances of the
   * DataTable "class" are created on-the-fly as needed (typically by a
   * $().dataTable() call) and the baton object is then applied to that
   * instance.
   *
   * Note that this object is related to {@link DataTable.defaults} but this
   * one is the internal data store for DataTables's cache of columns. It should
   * NOT be manipulated outside of DataTables. Any configuration should be done
   * through the initialisation options.
   *  @namespace
   *  @todo Really should attach the baton object to individual instances so we
   *    don't need to create new instances on each $().dataTable() call (if the
   *    table already exists). It would also save passing baton around and
   *    into every single function. However, this is a very significant
   *    architecture change for DataTables and will almost certainly break
   *    backwards compatibility with older installations. This is something that
   *    will be done in 2.0.
   */
  DataTable.models.baton = {
    /**
     * Primary features of DataTables and their enablement state.
     *  @namespace
     */
    "oFeatures": {

      /**
       * Flag to say if DataTables should automatically try to calculate the
       * optimum table and columns widths (true) or not (false).
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type boolean
       */
      "bAutoWidth": null,

      /**
       * Delay the creation of TR and TD elements until they are actually
       * needed by a driven page draw. This can give a significant speed
       * increase for Ajax source and Javascript source data, but makes no
       * difference at all fro DOM and server-side processing tables.
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type boolean
       */
      "bDeferRender": null,

      /**
       * Enable filtering on the table or not. Note that if this is disabled
       * then there is no filtering at all on the table, including fnFilter.
       * To just remove the filtering input use sDom and remove the 'f' option.
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type boolean
       */
      "bFilter": null,

      /**
       * Table information element (the 'Showing x of y records' div) enable
       * flag.
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type boolean
       */
      "bInfo": null,

      /**
       * Present a user control allowing the end user to change the page size
       * when pagination is enabled.
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type boolean
       */
      "bLengthChange": null,

      /**
       * Processing indicator enable flag whenever DataTables is enacting a
       * user request - typically an Ajax request for server-side processing.
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type boolean
       */
      "bProcessing": null,

      /**
       * Server-side processing enabled flag - when enabled DataTables will
       * get all data from the server for every draw - there is no filtering,
       * sorting or paging done on the client-side.
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type boolean
       */
      "bServerSide": null,

      /**
       * Sorting enablement flag.
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type boolean
       */
      "bSort": null,

      /**
       * Multi-column sorting
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type boolean
       */
      "bSortMulti": null,

      /**
       * Apply a class to the columns which are being sorted to provide a
       * visual highlight or not. This can slow things down when enabled since
       * there is a lot of DOM interaction.
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type boolean
       */
      "bSortClasses": null,
    },


    /**
     * Scrolling baton for a table.
     *  @namespace
     */
    "oScroll": {
      /**
       * When the table is shorter in height than sScrollY, collapse the
       * table container down to the height of the table (when true).
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type boolean
       */
      "bCollapse": null,

      /**
       * Width of the scrollbar for the web-browser's platform. Calculated
       * during table initialisation.
       *  @type int
       *  @default 0
       */
      "iBarWidth": 0,

      /**
       * Viewport width for horizontal scrolling. Horizontal scrolling is
       * disabled if an empty string.
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type string
       */
      "sX": null,

      /**
       * Width to expand the table to when using x-scrolling. Typically you
       * should not need to use this.
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type string
       *  @deprecated
       */
      "sXInner": null,

      /**
       * Viewport height for vertical scrolling. Vertical scrolling is disabled
       * if an empty string.
       * Note that this parameter will be set by the initialisation routine. To
       * set a default use {@link DataTable.defaults}.
       *  @type string
       */
      "sY": null
    },

    /**
     * Browser support parameters
     *  @namespace
     */
    "oBrowser": {
      /**
       * Indicate if the browser incorrectly calculates width:100% inside a
       * scrolling element (IE6/7)
       *  @type boolean
       *  @default false
       */
      "bScrollOversize": false,

      /**
       * Determine if the vertical scrollbar is on the right or left of the
       * scrolling container - needed for rtl language layout, although not
       * all browsers move the scrollbar (Safari).
       *  @type boolean
       *  @default false
       */
      "bScrollbarLeft": false,

      /**
       * Flag for if `getBoundingClientRect` is fully supported or not
       *  @type boolean
       *  @default false
       */
      "bBounding": false,

      /**
       * Browser scrollbar width
       *  @type integer
       *  @default 0
       */
      "barWidth": 0
    },


    "ajax": null,


    /**
     * Array referencing the nodes which are used for the features. The
     * parameters of this object match what is allowed by sDom - i.e.
     *   <ul>
     *     <li>'l' - Length changing</li>
     *     <li>'f' - Filtering input</li>
     *     <li>'t' - The table!</li>
     *     <li>'i' - Information</li>
     *     <li>'p' - Pagination</li>
     *     <li>'r' - pRocessing</li>
     *   </ul>
     *  @type array
     *  @default []
     */
    "aanFeatures": [],

    /**
     * Store data information - see {@link DataTable.models.oRow} for detailed
     * information.
     *  @type array
     *  @default []
     */
    "aoData": [],

    /**
     * Array of indexes which are in the current display (after filtering etc)
     *  @type array
     *  @default []
     */
    "aiDisplay": [],

    /**
     * Store information about each column that is in use
     *  @type array
     *  @default []
     */
    "aoColumns": [],

    /**
     * Store information about the table's header
     *  @type array
     *  @default []
     */
    "aoHeader": [],

    /**
     * Store the applied global search information in case we want to force a
     * research or compare the old search to a new one.
     * Note that this parameter will be set by the initialisation routine. To
     * set a default use {@link DataTable.defaults}.
     *  @namespace
     *  @extends DataTable.models.oSearch
     */
    "oPreviousSearch": {},

    /**
     * Store the applied search for each column - see
     * {@link DataTable.models.oSearch} for the format that is used for the
     * filtering information for each column.
     *  @type array
     *  @default []
     */
    "aoPreSearchCols": [],

    /**
     * Sorting that is applied to the table. Note that the inner arrays are
     * used in the following manner:
     * <ul>
     *   <li>Index 0 - column number</li>
     *   <li>Index 1 - current sorting direction</li>
     * </ul>
     * Note that this parameter will be set by the initialisation routine. To
     * set a default use {@link DataTable.defaults}.
     *  @type array
     *  @todo These inner arrays should really be objects
     */
    "aaSorting": null,

    /**
     * Classes to use for the striping of a table.
     * Note that this parameter will be set by the initialisation routine. To
     * set a default use {@link DataTable.defaults}.
     *  @type array
     *  @default []
     */
    "asStripeClasses": null,

    /**
     * If restoring a table - we should restore its striping classes as well
     *  @type array
     *  @default []
     */
    "asDestroyStripes": [],

    /**
     * If restoring a table - we should restore its width
     *  @type int
     *  @default 0
     */
    "sDestroyWidth": 0,

    /**
     * Callback functions array for every time a row is inserted (i.e. on a draw).
     *  @type array
     *  @default []
     */
    "aoRowCallback": [],

    /**
     * Callback functions for the header on each draw.
     *  @type array
     *  @default []
     */
    "aoHeaderCallback": [],

    /**
     * Array of callback functions for draw callback functions
     *  @type array
     *  @default []
     */
    "aoDrawCallback": [],

    /**
     * Array of callback functions for row created function
     *  @type array
     *  @default []
     */
    "aoRowCreatedCallback": [],

    /**
     * Callback functions for just before the table is redrawn. A return of
     * false will be used to cancel the draw.
     *  @type array
     *  @default []
     */
    "aoPreDrawCallback": [],

    /**
     * Callback functions for when the table has been initialised.
     *  @type array
     *  @default []
     */
    "aoInitComplete": [],
    /**
     * Cache the table ID for quick access
     *  @type string
     *  @default <i>Empty string</i>
     */
    "sTableId": "",

    /**
     * The TABLE node for the main table
     *  @type node
     *  @default null
     */
    "nTable": null,

    /**
     * Permanent ref to the thead element
     *  @type node
     *  @default null
     */
    "nTHead": null,

    /**
     * Permanent ref to the tbody element
     *  @type node
     *  @default null
     */
    "nTBody": null,

    /**
     * Cache the wrapper node (contains all DataTables controlled elements)
     *  @type node
     *  @default null
     */
    "nTableWrapper": null,

    /**
     * Indicate if when using server-side processing the loading of data
     * should be deferred until the second draw.
     * Note that this parameter will be set by the initialisation routine. To
     * set a default use {@link DataTable.defaults}.
     *  @type boolean
     *  @default false
     */
    "bDeferLoading": false,

    /**
     * Indicate if all required information has been read in
     *  @type boolean
     *  @default false
     */
    "bInitialised": false,

    /**
     * Information about open rows. Each object in the array has the parameters
     * 'nTr' and 'nParent'
     *  @type array
     *  @default []
     */
    "aoOpenRows": [],

    /**
     * Dictate the positioning of DataTables' control elements - see
     * {@link DataTable.model.oInit.sDom}.
     * Note that this parameter will be set by the initialisation routine. To
     * set a default use {@link DataTable.defaults}.
     *  @type string
     *  @default null
     */
    "sDom": null,

    /**
     * Search delay (in mS)
     *  @type integer
     *  @default null
     */
    "searchDelay": null,

    /**
     * Which type of pagination should be used.
     * Note that this parameter will be set by the initialisation routine. To
     * set a default use {@link DataTable.defaults}.
     *  @type string
     *  @default two_button
     */
    "sPaginationType": "two_button",

    /**
     * Note if draw should be blocked while getting data
     *  @type boolean
     *  @default true
     */
    "bAjaxDataGet": true,

    /**
     * JSON returned from the server in the last Ajax request
     *  @type object
     *  @default undefined
     */
    "json": undefined,

    /**
     * Function to get the server-side data.
     * Note that this parameter will be set by the initialisation routine. To
     * set a default use {@link DataTable.defaults}.
     *  @type function
     */
    "fnServerData": null,

    /**
     * Functions which are called prior to sending an Ajax request so extra
     * parameters can easily be sent to the server
     *  @type array
     *  @default []
     */
    "aoServerParams": [],

    /**
     * Send the XHR HTTP method - GET or POST (could be PUT or DELETE if
     * required).
     * Note that this parameter will be set by the initialisation routine. To
     * set a default use {@link DataTable.defaults}.
     *  @type string
     */
    "sServerMethod": null,

    /**
     * Format numbers for display.
     * Note that this parameter will be set by the initialisation routine. To
     * set a default use {@link DataTable.defaults}.
     *  @type function
     */
    "fnFormatNumber": null,

    /**
     * List of options that can be used for the user selectable length menu.
     * Note that this parameter will be set by the initialisation routine. To
     * set a default use {@link DataTable.defaults}.
     *  @type array
     *  @default []
     */
    "aLengthMenu": null,

    /**
     * Paging display length
     *  @type int
     *  @default 10
     */
    "_iDisplayLength": 10,

    /**
     * Paging start point - aiDisplay index
     *  @type int
     *  @default 0
     */
    "_iDisplayStart": 0,

    /**
     * The classes to use for the table
     *  @type object
     *  @default {}
     */
    "oClasses": {},

    /**
     * Flag attached to the baton object so you can check in the draw
     * callback if filtering has been done in the draw. Deprecated in favour of
     * events.
     *  @type boolean
     *  @default false
     *  @deprecated
     */
    "bFiltered": false,

    /**
     * Flag attached to the baton object so you can check in the draw
     * callback if sorting has been done in the draw. Deprecated in favour of
     * events.
     *  @type boolean
     *  @default false
     *  @deprecated
     */
    "bSorted": false,

    /**
     * Indicate that if multiple rows are in the header and there is more than
     * one unique cell per column, if the top one (true) or bottom one (false)
     * should be used for sorting / title by DataTables.
     * Note that this parameter will be set by the initialisation routine. To
     * set a default use {@link DataTable.defaults}.
     *  @type boolean
     */
    "bSortCellsTop": null,

    /**
     * Initialisation object that is used for the table
     *  @type object
     *  @default null
     */
    "oInit": null,

    /**
     * Destroy callback functions - for plug-ins to attach themselves to the
     * destroy so they can clean up markup and events.
     *  @type array
     *  @default []
     */
    "aoDestroyCallback": [],


    /**
     * Get the display end point - aiDisplay index
     *  @type function
     */
    "fnDisplayEnd": function ()
    {
      var
        len      = this._iDisplayLength,
        start    = this._iDisplayStart,
        calc     = start + len,
        records  = this.aiDisplay.length,
        features = this.oFeatures;

      if ( features.bServerSide ) {
        return Math.min( start+len, this.iRecordsCount );
      }
    },

    /**
     * The DataTables object for this table
     *  @type object
     *  @default null
     */
    "oInstance": null,

    /**
     * Unique identifier for each instance of the DataTables object. If there
     * is an ID on the table node, then it takes that value, otherwise an
     * incrementing internal counter is used.
     *  @type string
     *  @default null
     */
    "sInstance": null,

    /**
     * tabindex attribute value that is added to DataTables control elements, allowing
     * keyboard navigation of the table and its controls.
     */
    "iTabIndex": 0,

    /**
     * DIV container for the header scrolling table if scrolling
     */
    "nScrollHead": null,

    /**
     * Last applied sort
     *  @type array
     *  @default []
     */
    "aLastSort": [],

    /**
     * Stored plug-in instances
     *  @type object
     *  @default {}
     */
    "oPlugins": {},

    /**
     * Function used to get a row's id from the row's data
     *  @type function
     *  @default null
     */
    "rowIdFn": null,

    /**
     * Data location where to store a row's id
     *  @type string
     *  @default null
     */
    "rowId": null
  };

  /**
   * Extension object for DataTables that is used to provide all extension
   * options.
   *
   * Note that the `DataTable.ext` object is available through
   * `jQuery.fn.dataTable.ext` where it may be accessed and manipulated. It is
   * also aliased to `jQuery.fn.dataTableExt` for historic reasons.
   *  @namespace
   *  @extends DataTable.models.ext
   */


  /**
   * DataTables extensions
   *
   * This namespace acts as a collection area for plug-ins that can be used to
   * extend DataTables capabilities. Indeed many of the build in methods
   * use this method to provide their own capabilities (sorting methods for
   * example).
   *
   * Note that this namespace is aliased to `jQuery.fn.dataTableExt` for legacy
   * reasons
   *
   *  @namespace
   */
  DataTable.ext = _ext = {
    /**
     * Buttons. For use with the Buttons extension for DataTables. This is
     * defined here so other extensions can define buttons regardless of load
     * order. It is _not_ used by DataTables core.
     *
     *  @type object
     *  @default {}
     */
    buttons: {},


    /**
     * Element class names
     *
     *  @type object
     *  @default {}
     */
    classes: {},


    /**
     * DataTables build type (expanded by the download builder)
     *
     *  @type string
     */
    builder: "-source-",


    /**
     * Error reporting.
     *
     * How should DataTables report an error. Can take the value 'alert',
     * 'throw', 'none' or a function.
     *
     *  @type string|function
     *  @default alert
     */
    errMode: "alert",


    /**
     * Feature plug-ins.
     *
     * This is an array of objects which describe the feature plug-ins that are
     * available to DataTables. These feature plug-ins are then available for
     * use through the `dom` initialisation option.
     *
     * Each feature plug-in is described by an object which must have the
     * following properties:
     *
     * * `fnInit` - function that is used to initialise the plug-in,
     * * `cFeature` - a character so the feature can be enabled by the `dom`
     *   instillation option. This is case sensitive.
     *
     * The `fnInit` function has the following input parameters:
     *
     * 1. `{object}` DataTables baton object: see
     *    {@link DataTable.models.baton}
     *
     * And the following return is expected:
     *
     * * {node|null} The element which contains your feature. Note that the
     *   return may also be void if your plug-in does not require to inject any
     *   DOM elements into DataTables control (`dom`) - for example this might
     *   be useful when developing a plug-in which allows table control via
     *   keyboard entry
     *
     *  @type array
     *
     *  @example
     *    DataTable.ext.features.push( {
     *      "fnInit": function( baton ) {
     *        return new TableTools( { "oDTSettings": baton } );
     *      },
     *      "cFeature": "T"
     *    } );
     */
    feature: [],


    /**
     * Row searching.
     *
     * This method of searching is complimentary to the default type based
     * searching, and a lot more comprehensive as it allows you complete control
     * over the searching logic. Each element in this array is a function
     * (parameters described below) that is called for every row in the table,
     * and your logic decides if it should be included in the searching data set
     * or not.
     *
     * Searching functions have the following input parameters:
     *
     * 1. `{object}` DataTables baton object: see
     *    {@link DataTable.models.baton}
     * 2. `{array|object}` Data for the row to be processed (same as the
     *    original format that was passed in as the data source, or an array
     *    from a DOM data source
     * 3. `{int}` Row index ({@link DataTable.models.baton.aoData}), which
     *    can be useful to retrieve the `TR` element if you need DOM interaction.
     *
     * And the following return is expected:
     *
     * * {boolean} Include the row in the searched result set (true) or not
     *   (false)
     *
     * Note that as with the main search ability in DataTables, technically this
     * is "filtering", since it is subtractive. However, for consistency in
     * naming we call it searching here.
     *
     *  @type array
     *  @default []
     *
     *  @example
     *    // The following example shows custom search being applied to the
     *    // fourth column (i.e. the data[3] index) based on two input values
     *    // from the end-user, matching the data in a certain range.
     *    DataTable.ext.search.push(
     *      function( baton, data, dataIndex ) {
     *        var min = document.getElementById('min').value * 1;
     *        var max = document.getElementById('max').value * 1;
     *        var version = data[3] == "-" ? 0 : data[3]*1;
     *
     *        if ( min == "" && max == "" ) {
     *          return true;
     *        }
     *        else if ( min == "" && version < max ) {
     *          return true;
     *        }
     *        else if ( min < version && "" == max ) {
     *          return true;
     *        }
     *        else if ( min < version && version < max ) {
     *          return true;
     *        }
     *        return false;
     *      }
     *    );
     */
    search: [],


    /**
     * Selector extensions
     *
     * The `selector` option can be used to extend the options available for the
     * selector modifier options (`selector-modifier` object data type) that
     * each of the three built in selector types offer (row, column and cell +
     * their plural counterparts). For example the Select extension uses this
     * mechanism to provide an option to select only rows, columns and cells
     * that have been marked as selected by the end user (`{selected: true}`),
     * which can be used in conjunction with the existing built in selector
     * options.
     *
     * Each property is an array to which functions can be pushed. The functions
     * take three attributes:
     *
     * * baton object for the host table
     * * Options object (`selector-modifier` object type)
     * * Array of selected item indexes
     *
     * The return is an array of the resulting item indexes after the custom
     * selector has been applied.
     *
     *  @type object
     */
    selector: {
      cell: [],
      column: [],
      row: []
    },


    /**
     * Internal functions, exposed for used in plug-ins.
     *
     * Please note that you should not need to use the internal methods for
     * anything other than a plug-in (and even then, try to avoid if possible).
     * The internal function may change between releases.
     *
     *  @type object
     *  @default {}
     */
    internal: {},


    /**
     * Legacy configuration options. Enable and disable legacy options that
     * are available in DataTables.
     *
     *  @type object
     */
    legacy: {
      /**
       * Enable / disable DataTables 1.9 compatible server-side processing
       * requests
       *
       *  @type boolean
       *  @default null
       */
      ajax: null
    },


    /**
     * Pagination plug-in methods.
     *
     * Each entry in this object is a function and defines which buttons should
     * be shown by the pagination rendering method that is used for the table:
     * {@link DataTable.ext.renderer.pageButton}. The renderer addresses how the
     * buttons are displayed in the document, while the functions here tell it
     * what buttons to display. This is done by returning an array of button
     * descriptions (what each button will do).
     *
     * Pagination types (the four built in options and any additional plug-in
     * options defined here) can be used through the `paginationType`
     * initialisation parameter.
     *
     * The functions defined take two parameters:
     *
     * 1. `{int} page` The current page index
     * 2. `{int} pages` The number of pages in the table
     *
     * Each function is expected to return an array where each element of the
     * array can be one of:
     *
     * * `first` - Jump to first page when activated
     * * `last` - Jump to last page when activated
     * * `previous` - Show previous page when activated
     * * `next` - Show next page when activated
     * * `{int}` - Show page of the index given
     * * `{array}` - A nested array containing the above elements to add a
     *   containing 'DIV' element (might be useful for styling).
     *
     * Note that DataTables v1.9- used this object slightly differently whereby
     * an object with two functions would be defined for each plug-in. That
     * ability is still supported by DataTables 1.10+ to provide backwards
     * compatibility, but this option of use is now decremented and no longer
     * documented in DataTables 1.10+.
     *
     *  @type object
     *  @default {}
     *
     *  @example
     *    // Show previous, next and current page buttons only
     *    $.fn.dataTableExt.oPagination.current = function ( page, pages ) {
     *      return [ 'previous', page, 'next' ];
     *    };
     */
    pager: {},


    renderer: {
      pageButton: {},
      header: {}
    },


    /**
     * Ordering plug-ins - custom data source
     *
     * The extension options for ordering of data available here is complimentary
     * to the default type based ordering that DataTables typically uses. It
     * allows much greater control over the the data that is being used to
     * order a column, but is necessarily therefore more complex.
     *
     * This type of ordering is useful if you want to do ordering based on data
     * live from the DOM (for example the contents of an 'input' element) rather
     * than just the static string that DataTables knows of.
     *
     * The way these plug-ins work is that you create an array of the values you
     * wish to be ordering for the column in question and then return that
     * array. The data in the array much be in the index order of the rows in
     * the table (not the currently ordering order!). Which order data gathering
     * function is run here depends on the `dt-init columns.orderDataType`
     * parameter that is used for the column (if any).
     *
     * The functions defined take two parameters:
     *
     * 1. `{object}` DataTables baton object: see
     *    {@link DataTable.models.baton}
     * 2. `{int}` Target column index
     *
     * Each function is expected to return an array:
     *
     * * `{array}` Data for the column to be ordering upon
     *
     *  @type array
     *
     *  @example
     *    // Ordering using `input` node values
     *    DataTable.ext.order['dom-text'] = function  ( baton, col )
     *    {
     *      return this.api().column( col, {order:'index'} ).nodes().map( function ( td, i ) {
     *        return $('input', td).val();
     *      } );
     *    }
     */
    order: {},


    /**
     * Type based plug-ins.
     *
     * Each column in DataTables has a type assigned to it, either by automatic
     * detection or by direct assignment using the `type` option for the column.
     * The type of a column will effect how it is ordering and search (plug-ins
     * can also make use of the column type if required).
     *
     * @namespace
     */
    type: {
      /**
       * Type detection functions.
       *
       * The functions defined in this object are used to automatically detect
       * a column's type, making initialisation of DataTables super easy, even
       * when complex data is in the table.
       *
       * The functions defined take two parameters:
       *
         *  1. `{*}` Data from the column cell to be analysed
         *  2. `{baton}` DataTables baton object. This can be used to
         *     perform context specific type detection - for example detection
         *     based on language baton such as using a comma for a decimal
         *     place. Generally speaking the options from the baton will not
         *     be required
       *
       * Each function is expected to return:
       *
       * * `{string|null}` Data type detected, or null if unknown (and thus
       *   pass it on to the other type detection functions.
       *
       *  @type array
       *
       *  @example
       *    // Currency type detection plug-in:
       *    DataTable.ext.type.detect.push(
       *      function ( data, baton ) {
       *        // Check the numeric part
       *        if ( ! data.substring(1).match(/[0-9]/) ) {
       *          return null;
       *        }
       *
       *        // Check prefixed by currency
       *        if ( data.charAt(0) == '$' || data.charAt(0) == '&pound;' ) {
       *          return 'currency';
       *        }
       *        return null;
       *      }
       *    );
       */
      detect: [],


      /**
       * Type based search formatting.
       *
       * The type based searching functions can be used to pre-format the
       * data to be search on. For example, it can be used to strip HTML
       * tags or to de-format telephone numbers for numeric only searching.
       *
       * Note that is a search is not defined for a column of a given type,
       * no search formatting will be performed.
       *
       * Pre-processing of searching data plug-ins - When you assign the sType
       * for a column (or have it automatically detected for you by DataTables
       * or a type detection plug-in), you will typically be using this for
       * custom sorting, but it can also be used to provide custom searching
       * by allowing you to pre-processing the data and returning the data in
       * the format that should be searched upon. This is done by adding
       * functions this object with a parameter name which matches the sType
       * for that target column. This is the corollary of <i>afnSortData</i>
       * for searching data.
       *
       * The functions defined take a single parameter:
       *
         *  1. `{*}` Data from the column cell to be prepared for searching
       *
       * Each function is expected to return:
       *
       * * `{string|null}` Formatted string that will be used for the searching.
       *
       *  @type object
       *  @default {}
       *
       *  @example
       *    DataTable.ext.type.search['title-numeric'] = function ( d ) {
       *      return d.replace(/\n/g," ").replace( /<.*?>/g, "" );
       *    }
       */
      search: {},


      /**
       * Type based ordering.
       *
       * The column type tells DataTables what ordering to apply to the table
       * when a column is sorted upon. The order for each type that is defined,
       * is defined by the functions available in this object.
       *
       * Each ordering option can be described by three properties added to
       * this object:
       *
       * * `{type}-pre` - Pre-formatting function
       * * `{type}-asc` - Ascending order function
       * * `{type}-desc` - Descending order function
       *
       * All three can be used together, only `{type}-pre` or only
       * `{type}-asc` and `{type}-desc` together. It is generally recommended
       * that only `{type}-pre` is used, as this provides the optimal
       * implementation in terms of speed, although the others are provided
       * for compatibility with existing Javascript sort functions.
       *
       * `{type}-pre`: Functions defined take a single parameter:
       *
         *  1. `{*}` Data from the column cell to be prepared for ordering
       *
       * And return:
       *
       * * `{*}` Data to be sorted upon
       *
       * `{type}-asc` and `{type}-desc`: Functions are typical Javascript sort
       * functions, taking two parameters:
       *
         *  1. `{*}` Data to compare to the second parameter
         *  2. `{*}` Data to compare to the first parameter
       *
       * And returning:
       *
       * * `{*}` Ordering match: <0 if first parameter should be sorted lower
       *   than the second parameter, ===0 if the two parameters are equal and
       *   >0 if the first parameter should be sorted height than the second
       *   parameter.
       *
       *  @type object
       *  @default {}
       *
       *  @example
       *    // Numeric ordering of formatted numbers with a pre-formatter
       *    $.extend( DataTable.ext.type.order, {
       *      "string-pre": function(x) {
       *        a = (a === "-" || a === "") ? 0 : a.replace( /[^\d\-\.]/g, "" );
       *        return parseFloat( a );
       *      }
       *    } );
       *
       *  @example
       *    // Case-sensitive string ordering, with no pre-formatting method
       *    $.extend( DataTable.ext.order, {
       *      "string-case-asc": function(x,y) {
       *        return ((x < y) ? -1 : ((x > y) ? 1 : 0));
       *      },
       *      "string-case-desc": function(x,y) {
       *        return ((x < y) ? 1 : ((x > y) ? -1 : 0));
       *      }
       *    } );
       */
      order: {}
    },

    /**
     * Unique DataTables instance counter
     *
     * @type int
     * @private
     */
    _unique: 0,


    //
    // Depreciated
    // The following properties are retained for backwards compatiblity only.
    // The should not be used in new projects and will be removed in a future
    // version
    //

    /**
     * Version check function.
     *  @type function
     *  @depreciated Since 1.10
     */
    fnVersionCheck: DataTable.fnVersionCheck,
  };
;


  $.extend( DataTable.ext.classes, {
    "sTable": "dataTable",
    "sNoFooter": "no-footer",

    /* Paging buttons */
    "sPageButton": "paginate_button",
    "sPageButtonActive": "current",
    "sPageButtonDisabled": "disabled",

    /* Striping classes */
    "sStripeOdd": "odd",
    "sStripeEven": "even",

    /* Empty row */
    "sRowEmpty": "dataTables_empty",

    /* Features */
    "sFilter": "dataTables_filter",
    "sPaging": "dataTables_paginate paging_", /* Note that the type is postfixed */
    "sLength": "dataTables_length",
    "sProcessing": "dataTables_processing",

    /* Sorting */
    "sSortAsc": "sorting_asc",
    "sSortDesc": "sorting_desc",
    "sSortable": "sorting", /* Sortable in both directions */
    "sSortableAsc": "sorting_asc_disabled",
    "sSortableDesc": "sorting_desc_disabled",
    "sSortableNone": "sorting_disabled",
    "sSortColumn": "sorting_", /* Note that an int is postfixed for the sorting order */

    /* Filtering */
    "sFilterInput": "",

    /* Page length */
    "sLengthSelect": "",

    /* Scrolling */
    "sScrollWrapper": "dataTables_scroll",
    "sScrollHead": "dataTables_scrollHead",
    "sScrollHeadInner": "dataTables_scrollHeadInner",
    "sScrollBody": "dataTables_scrollBody",
  } );


  var extPagination = DataTable.ext.pager;


  // Built in type detection. See model.ext.aTypes for information about
  // what is required from this methods.
  $.extend( DataTable.ext.type.detect, [
    // Plain numbers - first since V8 detects some plain numbers as dates
    // e.g. Date.parse('55') (but not all, e.g. Date.parse('22')...).
    function ( d )
    {
      return !isNaN( parseFloat(d) ) && isFinite( d );
    },

    // Dates (only those recognised by the browser's Date.parse)
    function ( d, baton )
    {
      // V8 tries _very_ hard to make a string passed into `Date.parse()`
      // valid, so we need to use a regex to restrict date formats. Use a
      // plug-in for anything other than ISO8601 style strings
      if ( d && !(d instanceof Date) && ! _re_date.test(d) ) {
        return null;
      }
      var parsed = Date.parse(d);
      return (parsed !== null && !isNaN(parsed)) || _empty(d) ? 'date' : null;
    },

    // Formatted numbers
    function ( d, baton )
    {
      return _isNumber( d, "", true ) ? 'num-fmt'+"" : null;
    },

    // HTML numeric
    function ( d, baton )
    {
      return _htmlNumeric( d, "" ) ? 'html-num'+"" : null;
    },

    // HTML numeric, formatted
    function ( d, baton )
    {
      return _htmlNumeric( d, "", true ) ? 'html-num-fmt'+"" : null;
    },

    // HTML (this is strict checking - there must be html)
    function ( d, baton )
    {
      return _empty( d ) || (typeof d === 'string' && d.indexOf('<') !== -1) ?
        'html' : null;
    }
  ] );


  // Filter formatting functions. See model.ext.ofnSearch for information about
  // what is required from these methods.
  //
  // Note that additional search methods are added for the html numbers and
  // html formatted numbers by `_addNumericSort()` when we know what the decimal
  // place is


  $.extend( DataTable.ext.type.search, {
    html: function ( data ) {
      return _empty(data) ?
        data :
        typeof data === 'string' ?
          data
            .replace( _re_new_lines, " " )
            .replace( _re_html, "" ) :
          '';
    },

    string: function ( data ) {
      return _empty(data) ?
        data :
        typeof data === 'string' ?
          data.replace( _re_new_lines, " " ) :
          data;
    }
  } );


  var __numericReplace = function ( d, decimalPlace, re1, re2 ) {
    if ( d !== 0 && (!d || d === '-') ) {
      return -Infinity;
    }

    if ( d.replace ) {
      if ( re1 ) {
        d = d.replace( re1, '' );
      }

      if ( re2 ) {
        d = d.replace( re2, '' );
      }
    }

    return d * 1;
  };


  // Add the numeric 'deformatting' functions for sorting and search. This is done
  // in a function to provide an easy ability for the language options to add
  // additional methods if a non-period decimal place is used.
  function _addNumericSort ( decimalPlace ) {
    $.each(
      {
        // Plain numbers
        "num": function ( d ) {
          return __numericReplace( d, decimalPlace );
        },

        // Formatted numbers
        "num-fmt": function ( d ) {
          return __numericReplace( d, decimalPlace, _re_formatted_numeric );
        },

        // HTML numeric
        "html-num": function ( d ) {
          return __numericReplace( d, decimalPlace, _re_html );
        },

        // HTML numeric, formatted
        "html-num-fmt": function ( d ) {
          return __numericReplace( d, decimalPlace, _re_html, _re_formatted_numeric );
        }
      },
      function ( key, fn ) {
        // Add the ordering method
        _ext.type.order[ key+decimalPlace+'-pre' ] = fn;

        // For HTML types add a search formatter that will strip the HTML
        if ( key.match(/^html\-/) ) {
          _ext.type.search[ key+decimalPlace ] = _ext.type.search.html;
        }
      }
    );
  }


  // Numeric sorting types - order doesn't matter here
  _addNumericSort( '' );


  $.extend( true, DataTable.ext.renderer, {
    header: {
      _: function ( baton, cell, column, classes ) {
        // No additional mark-up required
        // Attach a sort listener to update on sort - note that using the
        // `DT` namespace will allow the event to be removed automatically
        // on destroy, while the `dt` namespaced event is the one we are
        // listening for
        $(baton.nTable).on( 'order.dt.DT', function ( e, ctx, sorting, columns ) {
          if ( baton !== ctx ) { // need to check this this is the host
            return;               // table, not a nested one
          }

          var colIdx = column.idx;

          cell
            .removeClass(
              column.sSortingClass +' '+
              classes.sSortAsc +' '+
              classes.sSortDesc
            )
            .addClass( columns[ colIdx ] == 'asc' ?
              classes.sSortAsc : columns[ colIdx ] == 'desc' ?
                classes.sSortDesc :
                column.sSortingClass
            );
        } );
      }
    }
  } );

  /*
   * Public helper functions. These aren't used internally by DataTables, or
   * called by any of the options passed into DataTables, but they can be used
   * externally by developers working with DataTables. They are helper functions
   * to make working with DataTables a little bit easier.
   */

  var __htmlEscapeEntities = function ( d ) {
    return typeof d === 'string' ?
      d
        .replace(/&/g, '&amp;')
        .replace(/</g, '&lt;')
        .replace(/>/g, '&gt;')
        .replace(/"/g, '&quot;') :
      d;
  };


  /*
   * This is really a good bit rubbish this method of exposing the internal methods
   * publicly... - To be fixed in 2.0 using methods on the prototype
   */


  /**
   * Create a wrapper function for exporting an internal functions to an external API.
   *  @param {string} fn API function name
   *  @returns {function} wrapped function
   *  @memberof DataTable#internal
   */
  function _fnExternApiFunc (fn)
  {
    return function() {
      var args = [_fnSettingsFromNode( this[DataTable.ext.iApiIndex] )].concat(
        Array.prototype.slice.call(arguments)
      );
      return DataTable.ext.internal[fn].apply( this, args );
    };
  }


  /**
   * Reference to internal functions for use by plug-in developers. Note that
   * these methods are references to internal functions and are considered to be
   * private. If you use these methods, be aware that they are liable to change
   * between versions.
   *  @namespace
   */
  $.extend( DataTable.ext.internal, {
    _fnExternApiFunc: _fnExternApiFunc,
    _fnAddColumn: _fnAddColumn,
    _fnColumnOptions: _fnColumnOptions,
    _fnVisibleToColumnIndex: _fnVisibleToColumnIndex,
    _fnColumnIndexToVisible: _fnColumnIndexToVisible,
    _fnVisbleColumns: _fnVisbleColumns,
    _fnGetColumns: _fnGetColumns,
    _fnApplyColumnDefs: _fnApplyColumnDefs,
    _fnHungarianMap: _fnHungarianMap,
    _fnCamelToHungarian: _fnCamelToHungarian,
    _fnBrowserDetect: _fnBrowserDetect,
    _fnAddData: _fnAddData,
    _fnSetCellData: _fnSetCellData,
    _fnSplitObjNotation: _fnSplitObjNotation,
    _fnGetObjectDataFn: _fnGetObjectDataFn,
    _fnBuildHead: _fnBuildHead,
    _fnDrawHead: _fnDrawHead,
    _fnAddOptionsHtml: _fnAddOptionsHtml,
    _fnDetectHeader: _fnDetectHeader,
    _fnGetUniqueThs: _fnGetUniqueThs,
    _fnEscapeRegex: _fnEscapeRegex,
    _fnInitialise: _fnInitialise,
    _fnScrollDraw: _fnScrollDraw,
    _fnApplyToChildren: _fnApplyToChildren,
    _fnCalculateColumnWidths: _fnCalculateColumnWidths,
    _fnThrottle: _fnThrottle,
    _fnConvertToWidth: _fnConvertToWidth,
    _fnGetWidestNode: _fnGetWidestNode,
    _fnGetMaxLenString: _fnGetMaxLenString,
    _fnStringToCss: _fnStringToCss,
    _fnSortFlatten: _fnSortFlatten,
    _fnSort: _fnSort,
    _fnSortAria: _fnSortAria,
    _fnSortData: _fnSortData,
    _fnMap: _fnMap,
    _fnCallbackReg: _fnCallbackReg,
    _fnRenderer: _fnRenderer,
    _fnExtend: _fnExtend,
    _fnCalculateEnd: function () {} // Used by a lot of plug-ins, but redundant
                                    // in 1.10, so this dead-end function is
                                    // added to prevent errors
  } );


  // jQuery access
  $.fn.dataTable = DataTable;

  // Provide access to the host jQuery object (circular reference)
  DataTable.$ = $;

  // Legacy aliases
  $.fn.dataTableSettings = DataTable.baton;
  $.fn.dataTableExt = DataTable.ext;

  // With a capital `D` we return a DataTables API instance rather than a
  // jQuery object
  $.fn.DataTable = function ( opts ) {
    return $(this).dataTable( opts ).api();
  };

  // All properties that are available to DataTable should also be
  // available on $.fn.DataTable
  $.each( DataTable, function ( prop, val ) {
    $.fn.DataTable[ prop ] = val;
  } );


  // Information about events fired by DataTables - for documentation.
  /**
   * Draw event, fired whenever the table is redrawn on the page, at the same
   * point as fnDrawCallback. This may be useful for binding events or
   * performing calculations when the table is altered at all.
   *  @name DataTable#draw.dt
   *  @event
   *  @param {event} e jQuery event object
   *  @param {object} o DataTables baton object {@link DataTable.models.baton}
   */

  /**
   * Search event, fired when the searching applied to the table (using the
   * built-in global search, or column filters) is altered.
   *  @name DataTable#search.dt
   *  @event
   *  @param {event} e jQuery event object
   *  @param {object} o DataTables baton object {@link DataTable.models.baton}
   */

  /**
   * Page change event, fired when the paging of the table is altered.
   *  @name DataTable#page.dt
   *  @event
   *  @param {event} e jQuery event object
   *  @param {object} o DataTables baton object {@link DataTable.models.baton}
   */

  /**
   * Order event, fired when the ordering applied to the table is altered.
   *  @name DataTable#order.dt
   *  @event
   *  @param {event} e jQuery event object
   *  @param {object} o DataTables baton object {@link DataTable.models.baton}
   */

  /**
   * DataTables initialisation complete event, fired when the table is fully
   * drawn, including Ajax data loaded, if Ajax data is required.
   *  @name DataTable#init.dt
   *  @event
   *  @param {event} e jQuery event object
   *  @param {object} baton DataTables baton object
   *  @param {object} json The JSON object request from the server - only
   *    present if client-side Ajax sourced data is used</li></ol>
   */

  /**
   * Processing event, fired when DataTables is doing some kind of processing
   * (be it, order, search or anything else). It can be used to indicate to
   * the end user that there is something happening, or that something has
   * finished.
   *  @name DataTable#processing.dt
   *  @event
   *  @param {event} e jQuery event object
   *  @param {object} baton DataTables baton object
   *  @param {boolean} bShow Flag for if DataTables is doing processing or not
   */

  /**
   * Ajax (XHR) event, fired whenever an Ajax request is completed from a
   * request to made to the server for new data. This event is called before
   * DataTables processed the returned data, so it can also be used to pre-
   * process the data returned from the server, if needed.
   *
   * Note that this trigger is called in `fnServerData`, if you override
   * `fnServerData` and which to use this event, you need to trigger it in you
   * success function.
   *  @name DataTable#xhr.dt
   *  @event
   *  @param {event} e jQuery event object
   *  @param {object} o DataTables baton object {@link DataTable.models.baton}
   *  @param {object} json JSON returned from the server
   *
   *  @example
   *     // Use a custom property returned from the server in another DOM element
   *     $('#table').dataTable().on('xhr.dt', function (e, baton, json) {
   *       $('#status').html( json.status );
   *     } );
   *
   *  @example
   *     // Pre-process the data returned from the server
   *     $('#table').dataTable().on('xhr.dt', function (e, baton, json) {
   *       for ( var i=0, ien=json.aaData.length ; i<ien ; i++ ) {
   *         json.aaData[i].sum = json.aaData[i].one + json.aaData[i].two;
   *       }
   *       // Note no return - manipulate the data directly in the JSON object.
   *     } );
   */

  /**
   * Destroy event, fired when the DataTable is destroyed by calling fnDestroy
   * or passing the bDestroy:true parameter in the initialisation object. This
   * can be used to remove bound events, added DOM nodes, etc.
   *  @name DataTable#destroy.dt
   *  @event
   *  @param {event} e jQuery event object
   *  @param {object} o DataTables baton object {@link DataTable.models.baton}
   */

  /**
   * Page length change event, fired when number of records to show on each
   * page (the length) is changed.
   *  @name DataTable#length.dt
   *  @event
   *  @param {event} e jQuery event object
   *  @param {object} o DataTables baton object {@link DataTable.models.baton}
   *  @param {integer} len New length
   */

  /**
   * Column sizing has changed.
   *  @name DataTable#column-sizing.dt
   *  @event
   *  @param {event} e jQuery event object
   *  @param {object} o DataTables baton object {@link DataTable.models.baton}
   */

  /**
   * Column visibility has changed.
   *  @name DataTable#column-visibility.dt
   *  @event
   *  @param {event} e jQuery event object
   *  @param {object} o DataTables baton object {@link DataTable.models.baton}
   *  @param {int} column Column index
   *  @param {bool} vis `false` if column now hidden, or `true` if visible
   */

  return DataTable;
}));


/*
repo https://github.com/DataTables/Scroller/tree/1.5.1
committed 2018-06-22T15:19:44Z
*/


/*
file https://github.com/DataTables/Scroller/blob/1.5.1/js/scroller.dataTables.js
*/
/*! Scroller 1.5.1
 * ©2011-2018 SpryMedia Ltd - datatables.net/license
 */
/**
 * @summary     Scroller
 * @description Virtual rendering for DataTables
 * @version     1.5.1
 * @file        dataTables.scroller.js
 * @author      SpryMedia Ltd (www.sprymedia.co.uk)
 * @contact     www.sprymedia.co.uk/contact
 * @copyright   Copyright 2011-2018 SpryMedia Ltd.
 *
 * This source file is free software, available under the following license:
 *   MIT license - http://datatables.net/license/mit
 *
 * This source file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the license files for details.
 *
 * For details please refer to: http://www.datatables.net
 */
(function( factory ){
  if ( typeof define === 'function' && define.amd ) {
    // AMD
    define( ['jquery', 'datatables.net'], function ( $ ) {
      return factory( $, window, document );
    } );
  }
  else if ( typeof exports === 'object' ) {
    // CommonJS
    module.exports = function (root, $) {
      if ( ! root ) {
        root = window;
      }
      if ( ! $ || ! DataTable ) {
        $ = require('datatables.net')(root, $).$;
      }
      return factory( $, root, root.document );
    };
  }
  else {
    // Browser
    factory( jQuery, window, document );
  }
}(function( $, window, document, undefined ) {
'use strict';
var DataTable = $.fn.dataTable;
/**
 * Scroller is a virtual rendering plug-in for DataTables which allows large
 * datasets to be drawn on screen every quickly. What the virtual rendering means
 * is that only the visible portion of the table (and a bit to either side to make
 * the scrolling smooth) is drawn, while the scrolling container gives the
 * visual impression that the whole table is visible. This is done by making use
 * of the pagination abilities of DataTables and moving the table around in the
 * scrolling container DataTables adds to the page. The scrolling container is
 * forced to the height it would be for the full table display using an extra
 * element.
 *
 * Note that rows in the table MUST all be the same height. Information in a cell
 * which expands on to multiple lines will cause some odd behaviour in the scrolling.
 *
 * Scroller is initialised by simply including the letter 'S' in the sDom for the
 * table you want to have this feature enabled on. Note that the 'S' must come
 * AFTER the 't' parameter in `dom`.
 *
 * Key features include:
 *   <ul class="limit_length">
 *     <li>Speed! The aim of Scroller for DataTables is to make rendering large data sets fast</li>
 *     <li>Full compatibility with deferred rendering in DataTables for maximum speed</li>
 *     <li>Display millions of rows</li>
 *     <li>Integration with state saving in DataTables (scrolling position is saved)</li>
 *     <li>Easy to use</li>
 *   </ul>
 *
 *  @class
 *  @constructor
 *  @global
 *  @param {object} baton DataTables baton object or API instance
 *  @param {object} [opts={}] Configuration object for FixedColumns. Options
 *    are defined by {@link Scroller.defaults}
 *
 *  @requires jQuery 1.7+
 *  @requires DataTables 1.10.0+
 *
 *  @example
 *    $(document).ready(function() {
 *        $('#example').DataTable( {
 *            "scrollY": "200px",
 *            "ajax": "media/dataset/large.txt",
 *            "dom": "frtiS",
 *            "deferRender": true
 *        } );
 *    } );
 */
var Scroller = function ( baton) {
  // hack-baton
  this.baton = baton;
  /* Sanity check - you just know it will happen */
  if ( ! (this instanceof Scroller) ) {
    alert( "Scroller warning: Scroller must be initialised with the 'new' keyword." );
    return;
  }
  var dtApi = new DataTable.Api( baton );
  /**
   * baton object which contains customisable information for the Scroller instance
   * @namespace
   * @private
   * @extends Scroller.defaults
   */
  Object.assign(baton, {
    /**
     * DataTables baton object
     *  @type     object
     *  @default  Passed in as first parameter to constructor
     */
    "dt": new DataTable.Api(dtApi.context, dtApi.context)[0],
    /**
     * DataTables API instance
     *  @type     DataTable.Api
     */
    "dtApi": dtApi,
    /**
     * Pixel location of the bottom of the drawn table in the viewport
     *  @type     int
     *  @default  0
     */
    "tableBottom": 0,
    /**
     * Pixel location of the boundary for when the next data set should be loaded and drawn
     * when scrolling up the way.
     *  @type     int
     *  @default  0
     *  @private
     */
    "redrawTop": 0,
    /**
     * Pixel location of the boundary for when the next data set should be loaded and drawn
     * when scrolling down the way. Note that this is actually calculated as the offset from
     * the top.
     *  @type     int
     *  @default  0
     *  @private
     */
    "redrawBottom": 0,
    /**
     * Number of rows calculated as visible in the visible viewport
     *  @type     int
     *  @default  0
     */
    "viewportRows": 0,
    /**
     * setTimeout reference for the redraw, used when server-side processing is enabled in the
     * DataTables in order to prevent DoSing the server
     *  @type     int
     *  @default  null
     */
    "drawTO": null,
    topRowFloat: 0,
    scrollDrawDiff: null,
    loaderVisible: false
  });
  // @todo The defaults should extend a `c` property and the internal baton
  // only held in the `s` property. At the moment they are mixed
  baton = $.extend( baton, Scroller.defaults);
  /**
   * DOM elements used by the class instance
   * @private
   * @namespace
   *
   */
  baton.nScrollerForce = document.createElement('div');
  baton.nScroller = undefined;
  baton.nScrollerTable = undefined;
  // Attach the instance to the DataTables instance so it can be accessed in
  // future. Don't initialise Scroller twice on the same table
  if ( baton.oScroller ) {
    return;
  }
  baton.oScroller = this;
  /* Let's do it */
  this._fnConstruct();
};
$.extend( Scroller.prototype, {
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * Public methods
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  /**
   * Calculate and store information about how many rows are to be displayed
   * in the scrolling viewport, based on current dimensions in the browser's
   * rendering. This can be particularly useful if the table is initially
   * drawn in a hidden element - for example in a tab.
   *  @param {bool} [bRedraw=true] Redraw the table automatically after the recalculation, with
   *    the new dimensions forming the basis for the draw.
   *  @returns {void}
   *  @example
   *    $(document).ready(function() {
   *      // Make the example container hidden to throw off the browser's sizing
   *      document.getElementById('container').style.display = "none";
   *      var oTable = $('#example').dataTable( {
   *        "sScrollY": "200px",
   *        "sAjaxSource": "media/dataset/large.txt",
   *        "sDom": "frtiS",
   *        "bDeferRender": true,
   *        "fnInitComplete": function (o) {
   *          // Immediately scroll to row 1000
   *          o.oScroller.fnScrollToRow( 1000 );
   *        }
   *      } );
   *
   *      setTimeout( function () {
   *        // Make the example container visible and recalculate the scroller sizes
   *        document.getElementById('container').style.display = "block";
   *        oTable.fnSettings().oScroller.fnMeasure();
   *      }, 3000 );
   */
  "fnMeasure": function ( bRedraw )
  {
    let baton = this.baton;
    baton.viewport = baton.nScroller.clientHeight;
    baton.viewportRows = parseInt( baton.viewport / DATATABLE_ROW_HEIGHT, 10 )+1;
    baton._iDisplayLength = baton.viewportRows * DATATABLE_ROW_BUFFER_SCALE;
  },
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * Private methods (they are of course public in JS, but recommended as private)
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  /**
   * Initialisation for Scroller
   *  @returns {void}
   *  @private
   */
  "_fnConstruct": function ()
  {
    let baton = this.baton;
    var that = this;
    let dt = baton.dtApi;
    /* Insert a div element that we can use to force the DT scrolling container to
     * the height that would be required if the whole table was being displayed
     */
    baton.nScrollerForce.style.position = "relative";
    baton.nScrollerForce.style.top = "0px";
    baton.nScrollerForce.style.left = "0px";
    baton.nScrollerForce.style.width = "1px";
    baton.nScroller = $('div.'+baton.oClasses.sScrollBody, baton.nTableWrapper)[0];
    baton.nScroller.appendChild( baton.nScrollerForce );
    baton.nScroller.style.position = "relative";
    baton.nScrollerTable = $('>table', baton.nScroller)[0];
    baton.nScrollerTable.style.position = "absolute";
    baton.nScrollerTable.style.top = "0px";
    baton.nScrollerTable.style.left = "0px";
    this.fnMeasure( false );
    // Scrolling callback to see if a page change is needed - use a throttled
    // function for the save save callback so we aren't hitting it on every
    // scroll
    baton.ingnoreScroll = true;
    $(baton.nScroller).on( 'scroll.dt-scroller', function (e) {
      onDatatableScroll(baton);
    } );
    // In iOS we catch the touchstart event in case the user tries to scroll
    // while the display is already scrolling
    $(baton.nScroller).on('touchstart.dt-scroller', function () {
      onDatatableScroll(baton);
    } );
    dt.on( 'init.scroller', function () {
      that.fnMeasure( false );
      that._fnDrawCallback();
      // Update the scroller when the DataTable is redrawn
      dt.on( 'draw.scroller', function () {
        that._fnDrawCallback();
      });
    } );
    // Set height before the draw happens, allowing everything else to update
    // on draw complete without worry for roder.
    dt.on( 'preDraw.dt.scroller', function () {
      that._fnScrollForce(baton);
    } );
  },
  /**
   * Draw callback function which is fired when the DataTable is redrawn. The main function of
   * this method is to position the drawn table correctly the scrolling container for the rows
   * that is displays as a result of the scrolling position.
   *  @returns {void}
   *  @private
   */
  "_fnDrawCallback": function () {
    let baton = this.baton;
    var
      that = this,
      iScrollTop = baton.nScroller.scrollTop,
      iActualScrollTop = iScrollTop,
      iScrollBottom = iScrollTop + baton.viewport,
      iTableHeight = $(baton.nTable).height(),
      displayStart = baton._iDisplayStart,
      displayLen = baton._iDisplayLength,
      displayEnd = baton.iRecordsCount;
    // Disable the scroll event listener while we are updating the DOM
    baton.skip = true;
    // If paging is reset
    if ( (baton.bSorted || baton.bFiltered) && displayStart === 0 ) {
      baton.topRowFloat = 0;
    }
    // Reposition the scrolling for the updated virtual position if needed
    if ( displayStart === 0 ) {
      // Linear calculation at the top of the table
      iScrollTop = baton.topRowFloat * DATATABLE_ROW_HEIGHT;
    }
    else if ( displayStart + displayLen >= displayEnd ) {
      // Linear calculation that the bottom as well
      iScrollTop = baton.scrollerHeightScroll - ((displayEnd - baton.topRowFloat) * DATATABLE_ROW_HEIGHT);
    }
    else {
      // Domain scaled in the middle
      iScrollTop = scrollerDomainConvert(baton, undefined, baton.topRowFloat * DATATABLE_ROW_HEIGHT );
    }
    baton.nScroller.scrollTop = iScrollTop;
    // Store positional information so positional calculations can be based
    // upon the current table draw position
    baton.baseScrollTop = iScrollTop;
    baton.baseRowTop = baton.topRowFloat;
    /**
     * Pixel location of the top of the drawn table in the viewport
     *  @type     int
     *  @default  0
     */
    // Position the table in the virtual scroller
    var tableTop = iScrollTop - ((baton.topRowFloat - displayStart) * DATATABLE_ROW_HEIGHT);
    if ( displayStart === 0 ) {
      tableTop = 0;
    }
    else if ( displayStart + displayLen >= displayEnd ) {
      tableTop = baton.scrollerHeightScroll - iTableHeight;
    }
    baton.nScrollerTable.style.top = tableTop+'px';
    baton.tableBottom = iTableHeight + tableTop;
    // Calculate the boundaries for where a redraw will be triggered by the
    // scroll event listener
    var boundaryPx = (iScrollTop - tableTop) * DATATABLE_SCROLLER_BOUNDARY_SCALE;
    baton.redrawTop = iScrollTop - boundaryPx;
    baton.redrawBottom = iScrollTop + boundaryPx > baton.scrollerHeightScroll - baton.viewport - DATATABLE_ROW_HEIGHT ?
      baton.scrollerHeightScroll - baton.viewport - DATATABLE_ROW_HEIGHT :
      iScrollTop + boundaryPx;
    baton.skip = false;
    baton.ingnoreScroll = false;
    // Because of the order of the DT callbacks, the info update will
    // take precedence over the one we want here. So a 'thread' break is
    // needed.  Only add the thread break if bInfo is set
    if ( baton.oFeatures.bInfo ) {
      setTimeout( function () {
        uiRenderScrollerInfo(baton);
      }, 0 );
    }
    // Hide the loading indicator
    uiRenderDatatableLoading(baton, "none");
  },
  /**
   * Force the scrolling container to have height beyond that of just the
   * table that has been drawn so the user can scroll the whole data set.
   *
   * Note that if the calculated required scrolling height exceeds a maximum
   * value (1 million pixels - hard-coded) the forcing element will be set
   * only to that maximum value and virtual / physical domain transforms will
   * be used to allow Scroller to display tables of any number of records.
   *  @returns {void}
   *  @private
   */
  _fnScrollForce: function (baton)
  {
    var max = 1_000_000;
    baton.scrollerHeightVirtual = DATATABLE_ROW_HEIGHT * baton.iRecordsCount;
    baton.scrollerHeightScroll = baton.scrollerHeightVirtual;
    if ( baton.scrollerHeightScroll > max ) {
      baton.scrollerHeightScroll = max;
    }
    // Minimum height so there is always a row visible (the 'no rows found'
    // if reduced to zero filtering)
    baton.nScrollerForce.style.height = baton.scrollerHeightScroll > DATATABLE_ROW_HEIGHT ?
      baton.scrollerHeightScroll+'px' :
      DATATABLE_ROW_HEIGHT + 'px';
  }
} );
// Attach Scroller to DataTables so it can be accessed as an 'extra'
DataTable.Scroller = Scroller;
}));
/*
file none
*/
/*jslint-enable*/
