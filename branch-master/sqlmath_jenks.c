// MIT License
//
// Copyright (c) 2021 Kai Zhu
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


// This code is derived from Maarten Hilferink's CalcNaturalBreaks,
// under GNU GPL v3.0 license at https://www.geodms.nl/CalcNaturalBreaks


/*
run standalone test:
g++ -Wall sqlmath_jenks.c -o .aa.exe && ./.aa.exe

This page contains C++ code of a O(k×n×log(nn)) method for the classification
of an array of numeric values such that the variance per class is minimal,
known as a Fisher's Natural Breaks Classification. This algorithm is an
improvement of Jenks' Natural Breaks Optimization method, which is an O(k×n^2)
algorithm. See:
Fisher's Natural Breaks Classification complexity proof

This code is derived from clc/dll/src1/CalcClassBreaks.cpp in our Subversion
Repository which implements the ClassifyJenksFisher operation in the [GeoDms].
It is independently compilable and runnable and finds the JenksFisher
class-breaks for an array of random numbers and made that available here.
*/

// LINT_C_FILE


/*
file sqlmath_h - start
*/
#ifndef SQLMATH_H
#define SQLMATH_API
#define SQLMATH_FNC
#define SQLMATH_H
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define ALLOCF free
#define ALLOCM malloc
#define MAX(aa, bb) (((aa) < (bb)) ? (bb) : (aa))
#define MIN(aa, bb) (((aa) > (bb)) ? (bb) : (aa))
SQLMATH_API int doubleSortCompare(
    const void *aa,
    const void *bb
) {
// this function will compare aa with bb
    const double cc = *(double *) aa - *(double *) bb;
    return cc == 0 ? 0 : cc > 0 ? 1 : -1;
}
#endif                          // SQLMATH_H
/*
file sqlmath_h - end
*/


/*
file sqlmath_jenks - start
*/
typedef struct JenksHistogramElem {
    double cvw;                 // cumulative value * weight
    double val;                 // value
    int cnt;                    // count of value
    int cww;                    // cumulative weight = sum of cnt
} JenksHistogramElem;

typedef struct JenksObject {
    double rows;
    double kk;
    double *resultBreaks;
    double *resultCounts;
    JenksHistogramElem *histogram;      // nn * sizeof(JenksHistogramElem)
    double *ssmNow;             // nn * 8
    double *ssmPrv;             // nn * 8
    double *tmp;                // nn * 8
    int *classBreaks;           // nn * (kk - 1) * sizeof(int)
    int byteLength;
    int classBreaksIndex;
    int completedRows;
} JenksObject;

inline static double jenksGetSsm(
    JenksObject * self,
    int iiBeg,
    int iiEnd
);

inline static int jenksFindMaxBreakIndex(
    JenksObject * self,
    int ii,
    int ppBeg,
    int ppEnd
);

inline static void jenksCalcRange(
    JenksObject * self,
    int iiBeg,
    int iiEnd,
    int ppBeg,
    int ppEnd
) {
// find classBreaks[ii+completedRows]
// for all ii>=iiBeg and ii<iiEnd given that
// the results are at least ppBeg+(completedRows-1)
// and less than            ppEnd+(completedRows-1)
// Complexity:
// O(log(iiEnd-iiBeg)*Max((iiEnd-iiBeg),(ppEnd-ppBeg))) <= O(mm*log(mm))
    assert(iiBeg <= iiEnd);
    assert(ppBeg <= iiBeg);
    assert(ppEnd <= iiEnd);
    if (iiBeg == iiEnd) {
        return;
    }
    assert(ppBeg < ppEnd);
    int iiMid = (iiBeg + iiEnd) / 2;
    int ppMid =
        jenksFindMaxBreakIndex(self, iiMid, ppBeg, MIN(ppEnd, iiMid + 1));
    assert(ppBeg <= ppMid);
    assert(ppMid < ppEnd);
    assert(ppMid <= iiMid);
    // Recurse - solve first half of the sub-problems with lower 'half' of
    // possible outcomes.
    jenksCalcRange(self, iiBeg, iiMid, ppBeg, MIN(iiMid, ppMid + 1));
    // store result for the middle element.
    self->classBreaks[self->classBreaksIndex + iiMid] = ppMid;
    // Recurse - solve second half of the sub-problems with upper 'half' of
    // possible outcomes.
    jenksCalcRange(self, iiMid + 1, iiEnd, ppMid, ppEnd);
}

SQLMATH_API JenksObject *jenksCreate(
    size_t nn,
    size_t kk
) {
// This function will allocate memory for and initialize new Jenks object.
#define jenksDestroy ALLOCF
    // declare var
    JenksObject *self;
    char *pTmp = NULL;
    size_t byteLength = 0;
    // Allocate self.
    nn = MAX(nn, kk);
    byteLength = (16 * 8        //
        + kk * 8                // resultBreaks
        + kk * 8                // resultCounts
        + nn * sizeof(JenksHistogramElem)       // histogram
        + nn * (kk - 1) * sizeof(int)   // classBreaks
        + nn * 8                // ssmNow
        + nn * 8                // ssmPrv
        + nn * 8                // tmp
        + 0);
    self = (JenksObject *) ALLOCM(byteLength);
    if (self == NULL) {
        return NULL;
    }
    // Initialize self.
    self->rows = 2;
    pTmp = (char *) self + 16 * 8;
    self->resultBreaks = (double *) pTmp;
    pTmp += kk * 8;
    self->resultCounts = (double *) pTmp;
    pTmp += kk * 8;
    self->histogram = (JenksHistogramElem *) pTmp;
    pTmp += nn * sizeof(JenksHistogramElem);
    self->classBreaks = (int *) pTmp;
    pTmp += nn * (kk - 1) * sizeof(int);
    self->ssmNow = (double *) pTmp;
    pTmp += nn * 8;
    self->ssmPrv = (double *) pTmp;
    pTmp += nn * 8;
    self->tmp = (double *) pTmp;
    pTmp += nn * 8;
    assert((size_t) (pTmp - (char *) self) == byteLength);
    return self;
}

inline static int jenksFindMaxBreakIndex(
    JenksObject * self,
    int ii,
    int ppBeg,
    int ppEnd
) {
// finds classBreaks[ii+completedRows] given that
// the result is at least ppBeg+(completedRows-1)
// and less than          ppEnd+(completedRows-1)
// Complexity: O(ppEnd-ppBeg) <= O(mm)
    int foundP = ppBeg;
    double ssmMin = self->ssmPrv[ppBeg] + jenksGetSsm(self, ppBeg, ii);
    while (1) {
        ppBeg += 1;
        if (ppBeg >= ppEnd) {
            break;
        }
        double currSSM = self->ssmPrv[ppBeg] + jenksGetSsm(self, ppBeg, ii);
        if (currSSM > ssmMin) {
            ssmMin = currSSM;
            foundP = ppBeg;
        }
    }
    self->ssmNow[ii] = ssmMin;
    return foundP;
}

inline static double jenksGetSsm(
    JenksObject * self,
    int iiBeg,
    int iiEnd
) {
// Get the Squared Mean for elements iiBeg..iiEnd, multiplied by weight.
// Note that nn*mean^2 = sum^2/nn when mean := sum/nn
    iiBeg += self->completedRows - 1;
    iiEnd += self->completedRows;
    return pow(self->histogram[iiEnd].cvw - self->histogram[iiBeg].cvw, 2)
        / (self->histogram[iiEnd].cww - self->histogram[iiBeg].cww);
}

SQLMATH_FNC void jenks(
    JenksObject * self,
    const int nn,
    int kk,
    const double *values
) {
    // declare var
    int bufSize = 0;
    int ii = 0;
    int lastClassBreakIndex;
    int mm = 0;
    // init var
    kk = MIN(kk, nn);
    self->kk = (double) kk;
    // handle null-case
    if (kk == 0) {
        return;
    }
    // handle if kk == 1, then return min-value of values
    if (kk == 1) {
        ii = nn - 1;
        self->resultBreaks[0] = values[ii];
        self->resultCounts[0] = nn;
        while (ii > 0) {
            ii -= 1;
            if (self->resultBreaks[0] > values[ii]) {
                self->resultBreaks[0] = values[ii];
            }
        }
        return;
    }
    // create pair of value->number of occurences (weight) which is input for
    // the JF- algorithm
    // create list with weighted unique values, sorted ASC by values.
    // @returns [{val:value as int,count:weight as int},...]
    {
        double cvw = 0;
        double val = 0;
        int cnt = 1;
        int cww = 0;
        // sort values
        ii = nn;
        while (ii > 0) {
            ii -= 1;
            self->tmp[ii] = values[ii];
        }
        qsort(self->tmp, nn, 8, doubleSortCompare);
        // init histogram
        val = self->tmp[0];
        ii = 1;
        while (1) {
            if (ii < nn && self->tmp[ii] == val) {
                cnt += 1;
                ii += 1;
                continue;
            }
            cww += cnt;
            cvw += val * cnt;
            self->histogram[mm].cnt = cnt;
            self->histogram[mm].cvw = cvw;
            self->histogram[mm].cww = cww;
            self->histogram[mm].val = val;
            // prepare SSM for first class. Last (kk-1) values are omitted
            self->ssmPrv[mm] =
                pow(self->histogram[mm].cvw, 2) / self->histogram[mm].cww;
            mm += 1;
            if (ii >= nn) {
                break;
            }
            val = self->tmp[ii];
            cnt = 1;
            ii += 1;
        }
    }
    // handle if kk >= mm, then return sorted array of unique-values of input
    kk = MIN(kk, mm);
    self->kk = (double) kk;
    if (kk >= mm) {
        ii = mm;
        while (ii > 0) {
            ii -= 1;
            self->resultBreaks[ii] = self->histogram[ii].val;
            self->resultCounts[ii] = self->histogram[ii].cnt;
        }
        return;
    }
    // Starting point of calculation of breaks.
    // complexity: O(mm*log(mm)*kk)
    self->classBreaksIndex = 0;
    self->completedRows = 1;
    bufSize = mm - (kk - 1);
    while (self->completedRows < kk - 1) {
        // complexity: O(mm*log(mm))
        jenksCalcRange(self, 0, bufSize, 0, bufSize);
        // swap arrays ssmNow and ssmPrv.
        memcpy(self->tmp, self->ssmPrv, (size_t) bufSize * 8);
        memcpy(self->ssmPrv, self->ssmNow, (size_t) bufSize * 8);
        memcpy(self->ssmNow, self->tmp, (size_t) bufSize * 8);
        // update classBreaksIndex.
        self->classBreaksIndex += bufSize;
        self->completedRows += 1;
    }
    lastClassBreakIndex =
        jenksFindMaxBreakIndex(self, bufSize - 1, 0, bufSize);
    ii = kk;
    while (ii > 0) {
        ii -= 1;
        // assign the break values to the result
        self->resultBreaks[ii] =
            self->histogram[lastClassBreakIndex + ii].val;
        // assign the bucket count to the result
        if (ii > 0) {
            self->resultCounts[ii - 1] =
                self->histogram[lastClassBreakIndex + ii - 1].cww;
        }
        if (ii > 1) {
            self->classBreaksIndex -= bufSize;
            lastClassBreakIndex =
                self->classBreaks[self->classBreaksIndex +
                lastClassBreakIndex];
        }
    }
    // break for the first class is the minimum of the dataset.
    self->resultBreaks[0] = self->histogram[0].val;
    self->resultCounts[kk - 1] = nn;
    ii = kk;
    while (ii > 1) {
        ii -= 1;
        self->resultCounts[ii] -= self->resultCounts[ii - 1];
    }
}

/*
file sqlmath_jenks - start
*/


/*
file sqlmath_test - start
*/
#ifndef SQLITE3_EXT_C2
// *INDENT-OFF*
#include <algorithm>
#include <iostream>
typedef std::size_t                  SizeT;
typedef SizeT                        CountType;
typedef std::pair<double, CountType> ValueCountPair;
typedef std::vector<ValueCountPair>  ValueCountPairContainer;
typedef std::vector<double>          LimitsContainer;
// helper funcs
template <typename T> T Min(T a, T b) { return (a < b) ? a : b; }
SizeT GetTotalCount(const  ValueCountPairContainer& vcpc) {
    SizeT sum = 0;
    ValueCountPairContainer::const_iterator
        i = vcpc.begin(),
        e = vcpc.end();
    for (sum = 0; i != e; ++i)
        sum += (*i).second;
    return sum;
}
// helper struct JenksFisher
// captures the intermediate data and methods for the calculation of Natural
// Class Breaks.
struct JenksFisher {
    JenksFisher(const ValueCountPairContainer& vcpc, SizeT k)
        :    m_M(vcpc.size())
        ,    m_K(k)
        ,    m_BufSize(vcpc.size()-(k-1))
        ,    m_PrevSSM(m_BufSize)
        ,    m_CurrSSM(m_BufSize)
        ,    m_CB(m_BufSize * (m_K-1))
        ,    m_CBPtr() {
        m_CumulValues.reserve(vcpc.size());
        double cwv = 0;
        CountType cw = 0, w;
        for (SizeT i=0; i != m_M; ++i) {
            // PRECONDITION: the value sequence must be strictly increasing
            assert(!i || vcpc[i].first > vcpc[i-1].first);
            w   = vcpc[i].second;
            assert(w > 0); // PRECONDITION: all weights must be positive
            cw  += w;
            assert(cw > w || !i); // No overflow? No loss of precision?
            cwv += w * vcpc[i].first;
            m_CumulValues.push_back(ValueCountPair(cwv, cw));
            if (i < m_BufSize)
                // prepare SSM for first class. Last (k-1) values are omitted
                m_PrevSSM[i] = cwv * cwv / cw;
        }
    }
    // Gets sum of weighs for elements b..e.
    double GetW(SizeT b, SizeT e) {
        // First element always belongs to class 0, thus queries should never
        // include it.
        assert(b);
        assert(b <= e);
        assert(e < m_M);
        double res = m_CumulValues[e].second;
        res -= m_CumulValues[b-1].second;
        return res;
    }
    // Gets sum of weighed values for elements b..e
    double GetWV(SizeT b, SizeT e) {
        assert(b);
        assert(b <= e);
        assert(e < m_M);
        double res = m_CumulValues[e].first;
        res -= m_CumulValues[b-1].first;
        return res;
    }
    // Gets the Squared Mean for elements b..e, multiplied by weight.
    // Note that n*mean^2 = sum^2/n when mean := sum/n
    double GetSSM(SizeT b, SizeT e) {
        double res = GetWV(b, e);
        return res * res / GetW(b, e);
    }
    // finds CB[i+m_NrCompletedRows] given that
    // the result is at least bp+(m_NrCompletedRows-1)
    // and less than          ep+(m_NrCompletedRows-1)
    // Complexity: O(ep-bp) <= O(m)
    SizeT FindMaxBreakIndex(SizeT i, SizeT bp, SizeT ep) {
        assert(bp < ep);
        assert(bp <= i);
        assert(ep <= i+1);
        assert(i  <  m_BufSize);
        assert(ep <= m_BufSize);
        double minSSM = m_PrevSSM[bp]
            + GetSSM(bp+m_NrCompletedRows, i+m_NrCompletedRows);
        SizeT foundP = bp;
        while (++bp < ep) {
            double currSSM = m_PrevSSM[bp]
                + GetSSM(bp+m_NrCompletedRows, i+m_NrCompletedRows);
            if (currSSM > minSSM) {
                minSSM = currSSM;
                foundP = bp;
            }
        }
        m_CurrSSM[i] = minSSM;
        return foundP;
    }
    // find CB[i+m_NrCompletedRows]
    // for all i>=bi and i<ei given that
    // the results are at least bp+(m_NrCompletedRows-1)
    // and less than            ep+(m_NrCompletedRows-1)
    // Complexity: O(log(ei-bi)*Max((ei-bi),(ep-bp))) <= O(m*log(m))
    void CalcRange(SizeT bi, SizeT ei, SizeT bp, SizeT ep) {
        assert(bi <= ei);
        assert(ep <= ei);
        assert(bp <= bi);
        if (bi == ei)
            return;
        assert(bp < ep);
        SizeT mi = (bi + ei)/2;
        SizeT mp = FindMaxBreakIndex(mi, bp, Min<SizeT>(ep, mi+1));
        assert(bp <= mp);
        assert(mp <  ep);
        assert(mp <= mi);
        // solve first half of the sub-problems with lower 'half' of possible
        // outcomes
        CalcRange(bi, mi, bp, Min<SizeT>(mi, mp+1));
        m_CBPtr[ mi ] = mp; // store result for the middle element.
        // solve second half of the sub-problems with upper 'half' of possible
        // outcomes
        CalcRange(mi+1, ei, mp, ep);
    }
    // complexity: O(m*log(m)*k)
    void CalcAll() {
        if (m_K >= 2) {
            m_CBPtr = m_CB.begin();
            for (
                m_NrCompletedRows = 1;
                m_NrCompletedRows < m_K-1;
                ++m_NrCompletedRows
            ) {
                // complexity: O(m*log(m))
                CalcRange(0, m_BufSize, 0, m_BufSize);
                m_PrevSSM.swap(m_CurrSSM);
                m_CBPtr += m_BufSize;
            }
        }
    }
    SizeT                   m_M, m_K, m_BufSize;
    ValueCountPairContainer m_CumulValues;
    std::vector<double> m_PrevSSM;
    std::vector<double> m_CurrSSM;
    std::vector<SizeT>               m_CB;
    std::vector<SizeT>::iterator     m_CBPtr;
    SizeT                  m_NrCompletedRows;
};
// GetValueCountPairs
//
// GetValueCountPairs sorts chunks of values and then merges them in order to
// minimize extra memory and work when many values are equal.
// This is done recursively while retaining used intermediary buffers in order
// to minimize heap allocations.
const SizeT BUFFER_SIZE = 1024;
void GetCountsDirect(
    ValueCountPairContainer& vcpc, // NOLINT
    const double* values,
    SizeT size
) {
    assert(size <= BUFFER_SIZE);
    assert(size > 0);
    assert(vcpc.empty());
    double buffer[BUFFER_SIZE];
    std::copy(values, values+size, buffer);
    std::sort(buffer, buffer+size);
    double currValue = buffer[0];
    SizeT     currCount = 1;
    for (SizeT index = 1; index != size; ++index) {
        if (currValue < buffer[index]) {
            vcpc.push_back(ValueCountPair(currValue, currCount));
            currValue = buffer[index];
            currCount = 1;
        } else {
            ++currCount;
        }
    }
    vcpc.push_back(ValueCountPair(currValue, currCount));
}
struct CompareFirst {
    bool operator () (const ValueCountPair& lhs, const ValueCountPair& rhs) {
        return lhs.first < rhs.first;
    }
};
void MergeToLeft(
    ValueCountPairContainer& vcpcLeft, // NOLINT
    const ValueCountPairContainer& vcpcRight,
    ValueCountPairContainer& vcpcDummy
) {
    assert(vcpcDummy.empty());
    vcpcDummy.swap(vcpcLeft);
    vcpcLeft.resize(vcpcRight.size() + vcpcDummy.size());
    std::merge(
        vcpcRight.begin(),
        vcpcRight.end(),
        vcpcDummy.begin(),
        vcpcDummy.end(),
        vcpcLeft.begin(),
        CompareFirst());
    ValueCountPairContainer::iterator
        currPair = vcpcLeft.begin(),
        lastPair = vcpcLeft.end();
    ValueCountPairContainer::iterator index = currPair+1;
    while (index != lastPair && currPair->first < index->first) {
        currPair = index;
        ++index;
    }
    double currValue = currPair->first;
    SizeT     currCount = currPair->second;
    for (; index != lastPair; ++index) {
        if (currValue < index->first) {
            *currPair++ = ValueCountPair(currValue, currCount);
            currValue = index->first;
            currCount = index->second;
        } else {
            currCount += index->second;
        }
    }
    *currPair++ = ValueCountPair(currValue, currCount);
    vcpcLeft.erase(currPair, lastPair);
    vcpcDummy.clear();
}
struct ValueCountPairContainerArray : std::vector<ValueCountPairContainer> {
    void resize(SizeT k) {
        assert(capacity() >= k);
        while (size() < k) {
            push_back(ValueCountPairContainer());
            back().reserve(BUFFER_SIZE);
        }
    }
    void GetValueCountPairs(
        ValueCountPairContainer& vcpc, // NOLINT
        const double* values,
        SizeT size,
        unsigned int nrUsedContainers
    ) {
        assert(vcpc.empty());
        if (size <= BUFFER_SIZE) {
            GetCountsDirect(vcpc, values, size);
        } else {
            resize(nrUsedContainers+2);
            unsigned int m = size/2;
            GetValueCountPairs(vcpc, values, m, nrUsedContainers);
            GetValueCountPairs(
                begin()[nrUsedContainers],
                values + m,
                size - m,
                nrUsedContainers+1);
            MergeToLeft(
                vcpc,
                begin()[nrUsedContainers],
                begin()[nrUsedContainers+1]);
            begin()[nrUsedContainers].clear();
        }
        assert(GetTotalCount(vcpc) == size);
    }
};
void GetValueCountPairs(
    ValueCountPairContainer& vcpc, // NOLINT
    const double* values,
    SizeT n
) {
    vcpc.clear();
    if (n) {
        ValueCountPairContainerArray vcpca;
        // max nr halving is log2(max cardinality / BUFFER_SIZE);
        // max cardinality is SizeT(-1)
        vcpca.reserve(3+8*sizeof(SizeT)-10);
        vcpca.GetValueCountPairs(vcpc, values, n, 0);
        assert(vcpc.size());
    }
}
void ClassifyJenksFisherFromValueCountPairs(
    LimitsContainer& breaksArray, // NOLINT
    SizeT k,
    const ValueCountPairContainer& vcpc
) {
    breaksArray.resize(k);
    SizeT m  = vcpc.size();
    assert(k <= m); // PRECONDITION
    if (!k)
        return;
    JenksFisher jf(vcpc,  k);
    if (k > 1) {
        jf.CalcAll();
        SizeT lastClassBreakIndex = jf.FindMaxBreakIndex(
            jf.m_BufSize-1,
            0,
            jf.m_BufSize);
        while (--k) {
            breaksArray[k] = vcpc[lastClassBreakIndex+k].first;
            assert(lastClassBreakIndex < jf.m_BufSize);
            if (k > 1) {
                jf.m_CBPtr -= jf.m_BufSize;
                lastClassBreakIndex = jf.m_CBPtr[lastClassBreakIndex];
            }
        }
        assert(jf.m_CBPtr == jf.m_CB.begin());
    }
    assert(k == 0);
    // break for the first class is the minimum of the dataset.
    breaksArray[0] = vcpc[0].first;
}
// test code
// #include "boost/random.hpp"
void test(
    const double *arr,
    const int nn,
    const int kk,
    const double aa,
    const double bb,
    const double cc,
    const double dd,
    const double ee,
    const double ff,
    const double gg,
    const double hh,
    JenksObject *self
) {
    // declare var
    LimitsContainer resultingbreaksArray;
    ValueCountPairContainer sortedUniqueValueCounts;
    int ii = 0;
    std::vector<double> values(arr, arr + nn);
    // print values
    std::cout << "values = [";
    ii = 0;
    while (ii < nn && ii < 100) {
        std::cout << (ii == 0 ? "" : ", ") << values[ii];
        ii += 1;
    }
    std::cout << "]" << std::endl;
    // jenks1
    GetValueCountPairs(sortedUniqueValueCounts, &values[0], nn);
    ClassifyJenksFisherFromValueCountPairs(
        resultingbreaksArray,
        kk,
        sortedUniqueValueCounts);
    // jenks2
    jenks(self, nn, kk, arr);
    // print result
    std::cout << "breaks = [";
    ii = 0;
    for (double breakValue : resultingbreaksArray) {
        std::cout << (ii == 0 ? "" : ", ") << breakValue;
        ii += 1;
    }
    std::cout << "]" << std::endl;
    // test
    ii = 0;
    while (ii < kk) {
        // debug
// printf(
// "nn=%d kk=%d ii=%d jenks1[ii]=%f jenks2[ii].val=%f jenks2[ii].cnt=%.0f\n",
// nn, kk, ii, resultingbreaksArray[ii], self->resultBreaks[ii],
// self->resultCounts[ii]);
        switch (ii) {
        case 0:
            assert(resultingbreaksArray[ii] == aa);
            assert(self->resultBreaks[ii] == aa);
            assert(self->resultCounts[ii] == ee);
            break;
        case 1:
            assert(resultingbreaksArray[ii] == bb);
            assert(self->resultBreaks[ii] == bb);
            assert(self->resultCounts[ii] == ff);
            break;
        case 2:
            assert(resultingbreaksArray[ii] == cc);
            assert(self->resultBreaks[ii] == cc);
            assert(self->resultCounts[ii] == gg);
            break;
        case 3:
            assert(resultingbreaksArray[ii] == dd);
            assert(self->resultBreaks[ii] == dd);
            assert(self->resultCounts[ii] == hh);
            break;
        }
        ii += 1;
    }
    std::cout << std::endl << std::endl;
}
int main(int c, char** argv) {
/*
    const double rangeMin = 0.0;
    const double rangeMax = 10.0;
    typedef boost::uniform_real<double> NumberDistribution;
    typedef boost::mt19937 RandomNumberGenerator;
    typedef boost::variate_generator<
        RandomNumberGenerator&,
        NumberDistribution
    > Generator;
    NumberDistribution distribution(rangeMin, rangeMax);
    RandomNumberGenerator generator;
    generator.seed(0); // seed with the current time
    Generator numberGenerator(generator, distribution);
    const int n = 1000000;
    const int k = 10;
    std::cout << "Generating random numbers..." << std::endl;
    std::vector<double> values;
    values.reserve(n);
    for (int i=0; i!=n; ++i)
    {
        double v = numberGenerator();
        // populate a distribuiton slightly more interesting than uniform, with
        // a lower density at higher values.
        values.push_back(v*v);
    }
    assert(values.size() == n);
    std::cout << "Generating sortedUniqueValueCounts ..." << std::endl;
    ValueCountPairContainer sortedUniqueValueCounts;
    GetValueCountPairs(sortedUniqueValueCounts, &values[0], n);
    std::cout << "Finding Jenks ClassBreaks..." << std::endl;
    LimitsContainer resultingbreaksArray;
    ClassifyJenksFisherFromValueCountPairs(
        resultingbreaksArray,
        k,
        sortedUniqueValueCounts);
    std::cout << "Reporting results..." << std::endl;
    for (double breakValue: resultingbreaksArray)
        std::cout << breakValue << std::endl << std::endl;
    std::cout << "Press a char and <enter> to terminate" << std::endl;
    char ch;
    std::cin >> ch; // wait for user to enter a key
*/
    JenksObject *self = jenksCreate(1 << 20, 4);
    assert(self != NULL);
    double aa_3_2[] = { 2, 1, 0 };
    test(aa_3_2, 3, 2,
        0, 1, 0, 0,
        1, 2, 0, 0,
        self);
    // values = [2, 1, 0]
    // breaks = [0, 1]
    double aa_4_2[] = { 13.7, 0.1, 0.2, -0.4 };
    test(aa_4_2, 4, 3,
        -0.4, 0.1, 13.7, 0,
        1, 2, 1, 0,
        self);
    // values = [13.7, 0.1, 0.2, -0.4]
    // breaks = [-0.4, 0.1, 13.7]
    double aa_5_4[] = { 13.7, 0.1, 0.2, -0.4, 0.5 };
    test(aa_5_4, 5, 4,
        -0.4, 0.1, 0.5, 13.7,
        1, 2, 1, 1,
        self);
    // values = [13.7, 0.1, 0.2, -0.4, 0.5]
    // breaks = [-0.4, 0.1, 0.5, 13.7]
    double aa_6_4[] = { 13.7, 0.1, 0.2, -0.4, 0.5, 0.5 };
    test(aa_6_4, 6, 4,
        -0.4, 0.1, 0.5, 13.7,
        1, 2, 2, 1,
        self);
    // values = [13.7, 0.1, 0.2, -0.4, 0.5, 0.5]
    // breaks = [-0.4, 0.1, 0.5, 13.7]
    double aa_7_4[] = { 13.7, 0.1, 0.2, -0.4, 0.5, 0.5, 12.5 };
    test(aa_7_4, 7, 4,
        -0.4, 0.1, 12.5, 13.7,
        1, 4, 1, 1,
        self);
    // values = [13.7, 0.1, 0.2, -0.4, 0.5, 0.5, 12.5]
    // breaks = [-0.4, 0.1, 12.5, 13.7]
    double aa_8_4[] = { 13.7, 0.1, 0.2, -0.4, 0.5, 0.5, 12.5, 0 };
    test(aa_8_4, 8, 4,
        -0.4, 0.2, 12.5, 13.7,
        3, 3, 1, 1,
        self);
    // values = [13.7, 0.1, 0.2, -0.4, 0.5, 0.5, 12.5, -0.5]
    // breaks = [-0.4, 0.2, 12.5, 13.7]
    double aa_11_4[] = { 7.1, 3.1, 3, 2.8, 2.7, 2.5, 2.2, 1.6, 1.2, 1.1, 0.1 };
    test(aa_11_4, 11, 4,
        0.1, 1.1, 2.2, 7.1,
        1, 3, 6, 1,
        self);
    // values = [7.1, 3.1, 3, 2.8, 2.7, 2.5, 2.2, 1.6, 1.2, 1.1, 0.1]
    // breaks = [0.1, 1.1, 2.2, 7.1]
    double aa_104_4[] = {
        -9, -6, -3, -3, -1, 1, 2, 2, 2, 3, 3, 4, 5, 5, 6, 6,
        7, 8, 8, 9, 10, 11, 11, 12, 12, 12, 12, 13, 15, 15, 15, 15,
        16, 16, 17, 18, 18, 19, 19, 19, 21, 22, 23, 23, 23, 23, 23, 26,
        28, 28, 30, 31, 31, 32, 32, 33, 34, 34, 34, 34, 35, 35, 35, 36,
        36, 41, 41, 41, 42, 43, 43, 43, 44, 44, 45, 45, 46, 46, 47, 48,
        51, 51, 52, 53, 53, 54, 54, 56, 56, 58, 60, 61, 62, 64, 67, 70,
        73, 74, 78, 78, 78, 79, 83, 93
    };
    test(aa_104_4, 104, 4,
        -9, 18, 41, 64,
        35, 30, 28, 11,
        self);
    // breaks = [-9, 18, 41, 64]
    // test aa_big_4
    {
        size_t nn = 1 << 20;
        double *aa_big_4 = (double *) ALLOCM((size_t) nn * 8);
        size_t ii = nn;
        while (ii > 0) {
            ii -= 1;
            aa_big_4[ii] = ii;
        }
        test(aa_big_4, nn, 4,
            0, 262144, 524288, 786432,
            262144, 262144, 262144, 262144,
            self);
        ALLOCF(aa_big_4);
    }
    jenksDestroy(self);
} // main
// *INDENT-ON*
#endif                          // SQLITE3_EXT_C2
/*
file sqlmath_test - start
*/
