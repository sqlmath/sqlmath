/*jslint beta, devel*/
(function () {
    function assertOrThrow(passed, msg) {

// This function will throw <msg> if <passed> is falsy.

        if (!passed) {
            throw (
                (msg && msg.stack)
                ? msg
                : new Error(msg)
            );
        }
    }

    function simpleLinearRegressionCreate(size = 8192) {

// This function will create an object for calculating simple-linear-regression
// statistics.
// https://en.wikipedia.org/wiki/Simple_linear_regression

        let nn = 0;
        let sxx = 0;
        let sxy = 0;
        let syy = 0;
        let xavg = 0;
        let xx2 = 0;
        let xyList = new Float64Array(size * 2);
        let yavg = 0;

        function pass1(xx, yy) {

// This function will calculate simple-linear-regression in one pass using
// Wellford's algorithm.
// For a new value xx, compute the new nn, new xavg, the new sxx.
// xavg accumulates the xavg of the entire dataset
// sxx aggregates the squared distance from the xavg
// nn aggregates the number of samples seen so far

            let dx;
            let dy;
            xyList[2 * nn] = xx;
            xyList[2 * nn + 1] = yy;
            nn += 1;
            xx2 += xx * xx;

// calculate sxx

            dx = xx - xavg;
            xavg += dx / nn;
            sxx += dx * (xx - xavg);

// calculate syy

            dy = yy - yavg;
            yavg += dy / nn;
            syy += dy * (yy - yavg);

// calculate sxy

            sxy += dx * (yy - yavg);
        }

        function pass2(xyList2) {

// This function will calculate simple-linear-regression in two passes.

            let ii;
            let nn2;
            let xx;
            let yy;
            nn = xyList2.length / 2;
            nn2 = xyList2.length;

// pass #1

            ii = 0;
            while (ii < nn2) {
                xx = xyList2[ii];
                yy = xyList2[ii + 1];
                xyList[ii] = xx;
                xyList[ii + 1] = yy;
                xavg += xx;
                yavg += yy;
                xx2 += xx * xx;
                ii += 2;
            }
            xavg = xavg / nn;
            yavg = yavg / nn;

// pass #2

            ii = 0;
            while (ii < nn2) {
                xx = xyList[ii] - xavg;
                yy = xyList[ii + 1] - yavg;
                sxx += xx * xx;
                syy += yy * yy;
                sxy += xx * yy;
                ii += 2;
            }
        }

        function result() {
            let alpha;
            let beta;
            let ii = 0;
            let nn2 = nn * 2;
            let salpha2;
            let sbeta2;
            let see = 0;
            beta = sxy / sxx;
            alpha = yavg - beta * xavg;
            while (ii < nn2) {
                see += (alpha + beta * xyList[ii] - xyList[ii + 1]) ** 2;
                ii += 2;
            }
            see = see / (nn - 2);
            sbeta2 = see / sxx;
            salpha2 = sbeta2 * xx2 / nn;
            return {
                alpha,
                beta,
                correlation: sxy / Math.sqrt(sxx * syy),
                nn,
                salpha2,
                sbeta2,
                see,
                sxx,
                sxy,
                syy,
                xavg,
                yavg
            };
        }

        return {
            pass1,
            pass2,
            result
        };
    }

// test

    (function () {
        let slr1;
        let xxList;
        let yyList;

        function round(num, decimal) {
            return Math.round(num / decimal) * decimal;
        }

        function validate() {
            let state = slr1.result();
            console.error(state);
            assertOrThrow(round(state.beta, 0.001) === 61.272, state.beta);
            assertOrThrow(round(state.alpha, 0.001) === -39.062, state.alpha);
            assertOrThrow(round(state.see, 0.0001) === 0.5762, state.see);
            assertOrThrow(round(state.sbeta2, 0.0001) === 3.1539, state.sbeta2);
            assertOrThrow(
                round(state.salpha2, 0.00001) === 8.63185,
                state.salpha2
            );
            assertOrThrow(
                round(state.correlation, 0.0001) === 0.9946,
                state.correlation
            );
        }

        xxList = [
            1.47, 1.50, 1.52, 1.55, 1.57, 1.60, 1.63, 1.65,
            1.68, 1.70, 1.73, 1.75, 1.78, 1.80, 1.83
        ];
        yyList = [
            52.21, 53.12, 54.48, 55.84, 57.20, 58.57, 59.93, 61.29,
            63.11, 64.47, 66.28, 68.10, 69.92, 72.19, 74.46
        ];

// validate pass1

        slr1 = simpleLinearRegressionCreate(xxList.length);
        xxList.forEach(function (xx, ii) {
            slr1.pass1(xx, yyList[ii]);
        });
        validate();

// validate pass2

        slr1 = simpleLinearRegressionCreate(xxList.length);
        slr1.pass2(xxList.flatMap(function (xx, ii) {
            return [
                xx, yyList[ii]
            ];
        }));
        validate();

// validate pass1 and pass2

        slr1 = simpleLinearRegressionCreate(xxList.length);
        slr1.pass2(xxList.slice(2).flatMap(function (xx, ii) {
            return [
                xx, yyList[ii + 2]
            ];
        }));
        xxList.slice(0, 2).forEach(function (xx, ii) {
            slr1.pass1(xx, yyList[ii]);
        });
        validate();
    }());
}());
