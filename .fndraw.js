// _fnDraw - start
/*jslint browser, name*/
/*global
    DataTable
    _fnCallbackFire
    assertOrThrow
    jQuery
    stringHtmlSafe
*/
function _fnAjaxUpdateDraw2(settings, json) {
// Data the data from the server (nuking the old) and redraw the table
//  @param {object} settings dataTables settings object
//  @param {object} json json data return from the server.
//  @param {string} json.sEcho Tracking flag for DataTables to match requests
//  @param {int} json.iTotalRecords Number of records in the data set,
//      not accounting for filtering
//  @param {int} json.iTotalDisplayRecords Number of records in the data set,
//      accounting for filtering
//  @param {array} json.aaData The data to display on this page
//  @param {string} [json.sColumns] Column ordering (sName, comma separated)
//  @memberof DataTable#oApi
    // Protect against out of sequence returns
    if (json.draw < settings.iDraw) {
        return;
    }
    settings.iDraw = json.draw || settings.iDraw;
    // _fnClearTable(settings); - start
    settings.aoData.length = 0;
    settings.aiDisplayMaster.length = 0;
    settings.aiDisplay.length = 0;
    settings.aIds = {};
    // _fnClearTable(settings); - end
    settings._iRecordsTotal = json.recordsTotal;
    settings._iRecordsDisplay = json.recordsFiltered;
    json.data.forEach(function (elem) {
        _fnAddData2(settings, elem);
    });
    settings.aiDisplay = settings.aiDisplayMaster.slice();
    settings.bAjaxDataGet = false;
    _fnDraw(settings);
    if (!settings._bInitComplete) {
        // _fnInitComplete(settings, json);
        settings._bInitComplete = true;
        // When data was added after the initialisation (data or Ajax)
        // we need to calculate the column sizing
        // hack-datatables - optimization - disable slow-onresize-handler
        // _fnAdjustColumnSizing(settings);
        _fnCallbackFire(settings, null, "plugin-init", [settings, json]);
        _fnCallbackFire(settings, "aoInitComplete", "init", [settings, json]);
    }
    settings.bAjaxDataGet = true;
    // _fnProcessingDisplay(settings, false);
    settings.aanFeatures.r[0].style.display = "none";
}
function _fnDraw(settings) {
// Insert the required TR nodes into the table for display
//  @param {object} settings dataTables settings object
//  @memberof DataTable#oApi
    let html = "";
    // _fnProcessingDisplay(settings, true);
    settings.aanFeatures.r[0].style.display = "block";
    // Provide a pre-callback function which can be used to cancel the draw
    // is false is returned
    _fnCallbackFire(
        settings,
        "aoPreDrawCallback",
        "preDraw",
        [
            settings
        ]
    );
    settings.bDrawing = true;
    // Server-side processing draw intercept
    if (settings.bDeferLoading) {
        settings.bDeferLoading = false;
        settings.iDraw += 1;
    } else if (!settings.bDestroying && settings.bAjaxDataGet) {
        settings.iDraw += 1;
        // Create an Ajax call based on the table's settings
        settings.ajax(
            settings,
            function (json) {
                _fnAjaxUpdateDraw2(settings, json);
            },
            settings
        );
        return;
    }
    settings.aiDisplay.forEach(function (rowIi) {
        // Create a new TR element (and it's TD children) for a row
        html += `<tr>`;
        // Process each column
        settings.aoColumns.forEach(function (ignore, colIi) {
            let val = settings.aoData[rowIi]._aData[colIi];
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
    settings.nTBody.innerHTML = html;
    Array.from(settings.nTBody.children).forEach(function (elemTr, ii) {
        let rowIi = settings.aiDisplay[ii];
        let row = settings.aoData[rowIi];
        row.nTr = elemTr;
        // Use a private property on the node to allow reserve mapping
        // from the node
        // to the aoData array for fast look up
        elemTr._DT_RowIndex = rowIi;
        row.anCells = Array.from(elemTr.children).map(function (elemTd, colIi) {
            elemTd._DT_CellIndex = {
                column: colIi,
                row: rowIi
            };
            return elemTd;
        });
    });
    // Table is empty - create a row with an empty message in it
    if (settings.aiDisplay.length === 0) {
        settings.nTBody.innerHTML = (
            `<tr><td colspan="${settings.aoColumns.length}">`
            + `No data available in table`
            + `</td></tr>`
        );
    }
    // Call all required callback functions for the end of a draw
    _fnCallbackFire(settings, "aoDrawCallback", "draw", [settings]);
    // Draw is complete, sorting and filtering must be as well
    settings.bSorted = false;
    settings.bFiltered = false;
    settings.bDrawing = false;
}
// Add a data array to the table, creating DOM node etc. This is the parallel to
// _fnGatherData, but for adding rows from a Javascript source, rather than a
// DOM source.
//  @param {object} settings dataTables settings object
//  @param {array} aData data array to be added
//  @param {node} [nTr] TR element to add to the table - optional. If not given,
//    DataTables will create a row automatically
//  @param {array} [anTds] Array of TD|TH elements for the row - must be given
//    if nTr is.
//  @returns {int} >=0 if successful (index of new aoData entry), -1 if failed
//  @memberof DataTable#oApi
function _fnAddData2(settings, aDataIn) {
    /* Create the object for storing information about this new row */
    let iRow = settings.aoData.length;
    let oData = Object.assign({}, DataTable.models.oRow, {
        idx: iRow,
        src: "data"
    });
    oData._aData = aDataIn;
    settings.aoData.push(oData);
    /* Create the cells */
    let columns = settings.aoColumns;
    // Invalidate the column types as the new data needs to be revalidated
    columns.forEach(function (col) {
        col.sType = null;
    });
    /* Add to the display array */
    settings.aiDisplayMaster.push(iRow);
    return iRow;
}
// _fnDraw - end
