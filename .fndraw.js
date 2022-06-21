// _fnDraw - start
/*jslint browser, name*/
/*global
    DataTable
    _fnCallbackFire
    assertOrThrow
    stringHtmlSafe
*/
function _fnAjaxUpdateDraw2(settings, {
    data,
    iRecordsDisplay,
    iRecordsTotal
}) {
// Data the data from the server (nuking the old) and redraw the table
//  @param {object} settings dataTables settings object
//  @param {object} json json data return from the server.
//  @param {string} sEcho Tracking flag for DataTables to match requests
//  @param {int} iTotalRecords Number of records in the data set,
//      not accounting for filtering
//  @param {int} iTotalDisplayRecords Number of records in the data set,
//      accounting for filtering
//  @param {array} aaData The data to display on this page
//  @param {string} [sColumns] Column ordering (sName, comma separated)
//  @memberof DataTable#oApi
    // _fnClearTable(settings); - start
    settings.aoData.length = 0;
    settings.aiDisplay.length = 0;
    // _fnClearTable(settings); - end
    settings._iRecordsDisplay = iRecordsDisplay;
    settings._iRecordsTotal = iRecordsTotal;
    data.forEach(function (aDataIn) {
// Add a data array to the table, creating DOM node etc. This is the parallel to
// _fnGatherData, but for adding rows from a Javascript source, rather than a
// DOM source.
        /* Create the object for storing information about this new row */
        let iRow = settings.aoData.length;
        let oData = Object.assign({}, DataTable.models.oRow, {
            idx: iRow,
            src: "data"
        });
        oData._aData = aDataIn;
        settings.aoData.push(oData);
        /* Create the cells */
        // Invalidate the column types as the new data needs to be revalidated
        settings.aoColumns.forEach(function (col) {
            col.sType = null;
        });
        /* Add to the display array */
        settings.aiDisplay.push(iRow);
    });
    settings.bAjaxDataGet = false;
    _fnDraw(settings);
    if (!settings._bInitComplete) {
        settings._bInitComplete = true;
        // When data was added after the initialisation (data or Ajax)
        // we need to calculate the column sizing
        // hack-datatables - optimization - disable slow-onresize-handler
        _fnCallbackFire(settings, "aoInitComplete", "init", [settings, {
            data,
            iRecordsDisplay,
            iRecordsTotal
        }]);
    }
    settings.bAjaxDataGet = true;
    _fnProcessingDisplay(settings, false);
}
function _fnDraw(settings) {
// Insert the required TR nodes into the table for display
//  @param {object} settings dataTables settings object
//  @memberof DataTable#oApi
    let html = "";
    _fnProcessingDisplay(settings, true);
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
    if (
        !settings.bDeferLoading
        && !settings.bDestroying
        && settings.bAjaxDataGet
    ) {
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
    // Server-side processing draw intercept
    if (settings.bDeferLoading) {
        settings.bDeferLoading = false;
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
}
function _fnProcessingDisplay(settings, show) {
// Display or hide the processing indicator
//  @param {object} settings dataTables settings object
//  @param {bool} show Show the processing indicator (true) or not (false)
//  @memberof DataTable#oApi
    settings.aanFeatures.r[0].style.display = (
        show
        ? "block"
        : "none"
    );
}
// _fnDraw - end
