/*jslint browser, name*/
/*global
    _fnAddData
    _fnAjaxParameters
    _fnBuildAjax
    _fnCallbackFire
    assertOrThrow
    stringHtmlSafe
*/
function _fnAjaxUpdateDraw2(settings, json) {
// Data the data from the server (nuking the old) and redraw the table
//  @param {object} oSettings dataTables settings object
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
        _fnAddData(settings, elem);
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
        _fnBuildAjax(
            settings,
            _fnAjaxParameters(settings),
            function (json) {
                _fnAjaxUpdateDraw2(settings, json);
            }
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
                ? `<NULL>`
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
