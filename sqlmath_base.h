#define LIGHTGBM_C_EXPORT typedef

// *INDENT-OFF*
/*jslint-disable*/
/*
shRollupFetch
{
    "fetchList": [
        {
            "replaceList": [
                {
                    "aa": "\nLIGHTGBM_C_EXPORT ([^\\(]*?) (\\w*?)(\\([\\S\\s]*?\\));\n",
                    "bb": "\nLIGHTGBM_C_EXPORT $1 (*$2_t) $3;\nstatic $2_t $2 = NULL;\n",
                    "flags": "g",
                    "substr": ""
                }
            ],
            "url": "https://github.com/microsoft/LightGBM/blob/v3.3.5/include/LightGBM/c_api.h"
        }
    ],
    "replaceList": []
}
-#define INLINE_FUNCTION inline
+// hack-lightgbm - fix warning
+#define INLINE_FUNCTION static inline

-#include <LightGBM/export.h>
+// hack-lightgbm - inline header
+// #include <LightGBM/export.h>
*/


/*
repo https://github.com/microsoft/LightGBM/tree/v3.3.5
committed 2023-01-07T05:42:37Z
*/


/*
file https://github.com/microsoft/LightGBM/blob/v3.3.5/include/LightGBM/c_api.h
*/
/*!
 * \file c_api.h
 * \copyright Copyright (c) 2016 Microsoft Corporation. All rights reserved.
 *            Licensed under the MIT License. See LICENSE file in the project root for license information.
 * \note
 * To avoid type conversion on large data, the most of our exposed interface supports both float32 and float64,
 * except the following:
 * 1. gradient and Hessian;
 * 2. current score for training and validation data.
 * .
 * The reason is that they are called frequently, and the type conversion on them may be time-cost.
 */
#ifndef LIGHTGBM_C_API_H_
#define LIGHTGBM_C_API_H_

// hack-lightgbm - inline header
// #include <LightGBM/export.h>

#ifdef __cplusplus
#include <cstdint>
#include <cstdio>
#include <cstring>
#else
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#endif


typedef void* DatasetHandle;  /*!< \brief Handle of dataset. */
typedef void* BoosterHandle;  /*!< \brief Handle of booster. */
typedef void* FastConfigHandle; /*!< \brief Handle of FastConfig. */

#define C_API_DTYPE_FLOAT32 (0)  /*!< \brief float32 (single precision float). */
#define C_API_DTYPE_FLOAT64 (1)  /*!< \brief float64 (double precision float). */
#define C_API_DTYPE_INT32   (2)  /*!< \brief int32. */
#define C_API_DTYPE_INT64   (3)  /*!< \brief int64. */

#define C_API_PREDICT_NORMAL     (0)  /*!< \brief Normal prediction, with transform (if needed). */
#define C_API_PREDICT_RAW_SCORE  (1)  /*!< \brief Predict raw score. */
#define C_API_PREDICT_LEAF_INDEX (2)  /*!< \brief Predict leaf index. */
#define C_API_PREDICT_CONTRIB    (3)  /*!< \brief Predict feature contributions (SHAP values). */

#define C_API_MATRIX_TYPE_CSR (0)  /*!< \brief CSR sparse matrix type. */
#define C_API_MATRIX_TYPE_CSC (1)  /*!< \brief CSC sparse matrix type. */

#define C_API_FEATURE_IMPORTANCE_SPLIT (0)  /*!< \brief Split type of feature importance. */
#define C_API_FEATURE_IMPORTANCE_GAIN  (1)  /*!< \brief Gain type of feature importance. */

/*!
 * \brief Get string message of the last error.
 * \return Error information
 */
LIGHTGBM_C_EXPORT const char* (*LGBM_GetLastError_t) ();
static LGBM_GetLastError_t LGBM_GetLastError = NULL;

/*!
 * \brief Register a callback function for log redirecting.
 * \param callback The callback function to register
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_RegisterLogCallback_t) (void (*callback)(const char*));
static LGBM_RegisterLogCallback_t LGBM_RegisterLogCallback = NULL;

/*!
 * \brief Get number of samples based on parameters and total number of rows of data.
 * \param num_total_row Number of total rows
 * \param parameters Additional parameters, namely, ``bin_construct_sample_cnt`` is used to calculate returned value
 * \param[out] out Number of samples. This value is used to pre-allocate memory to hold sample indices when calling ``LGBM_SampleIndices``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_GetSampleCount_t) (int32_t num_total_row,
                                          const char* parameters,
                                          int* out);
static LGBM_GetSampleCount_t LGBM_GetSampleCount = NULL;

/*!
 * \brief Create sample indices for total number of rows.
 * \note
 * You should pre-allocate memory for ``out``, you can get its length by ``LGBM_GetSampleCount``.
 * \param num_total_row Number of total rows
 * \param parameters Additional parameters, namely, ``bin_construct_sample_cnt`` and ``data_random_seed`` are used to produce the output
 * \param[out] out Created indices, type is int32_t
 * \param[out] out_len Number of indices
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_SampleIndices_t) (int32_t num_total_row,
                                         const char* parameters,
                                         void* out,
                                         int32_t* out_len);
static LGBM_SampleIndices_t LGBM_SampleIndices = NULL;

/* --- start Dataset interface */

/*!
 * \brief Load dataset from file (like LightGBM CLI version does).
 * \param filename The name of the file
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out A loaded dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromFile_t) (const char* filename,
                                                 const char* parameters,
                                                 const DatasetHandle reference,
                                                 DatasetHandle* out);
static LGBM_DatasetCreateFromFile_t LGBM_DatasetCreateFromFile = NULL;

/*!
 * \brief Allocate the space for dataset and bucket feature bins according to sampled data.
 * \param sample_data Sampled data, grouped by the column
 * \param sample_indices Indices of sampled data
 * \param ncol Number of columns
 * \param num_per_col Size of each sampling column
 * \param num_sample_row Number of sampled rows
 * \param num_total_row Number of total rows
 * \param parameters Additional parameters
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromSampledColumn_t) (double** sample_data,
                                                          int** sample_indices,
                                                          int32_t ncol,
                                                          const int* num_per_col,
                                                          int32_t num_sample_row,
                                                          int32_t num_total_row,
                                                          const char* parameters,
                                                          DatasetHandle* out);
static LGBM_DatasetCreateFromSampledColumn_t LGBM_DatasetCreateFromSampledColumn = NULL;

/*!
 * \brief Allocate the space for dataset and bucket feature bins according to reference dataset.
 * \param reference Used to align bin mapper with other dataset
 * \param num_total_row Number of total rows
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateByReference_t) (const DatasetHandle reference,
                                                    int64_t num_total_row,
                                                    DatasetHandle* out);
static LGBM_DatasetCreateByReference_t LGBM_DatasetCreateByReference = NULL;

/*!
 * \brief Push data to existing dataset, if ``nrow + start_row == num_total_row``, will call ``dataset->FinishLoad``.
 * \param dataset Handle of dataset
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of columns
 * \param start_row Row start index
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetPushRows_t) (DatasetHandle dataset,
                                           const void* data,
                                           int data_type,
                                           int32_t nrow,
                                           int32_t ncol,
                                           int32_t start_row);
static LGBM_DatasetPushRows_t LGBM_DatasetPushRows = NULL;

/*!
 * \brief Push data to existing dataset, if ``nrow + start_row == num_total_row``, will call ``dataset->FinishLoad``.
 * \param dataset Handle of dataset
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col Number of columns
 * \param start_row Row start index
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetPushRowsByCSR_t) (DatasetHandle dataset,
                                                const void* indptr,
                                                int indptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t nindptr,
                                                int64_t nelem,
                                                int64_t num_col,
                                                int64_t start_row);
static LGBM_DatasetPushRowsByCSR_t LGBM_DatasetPushRowsByCSR = NULL;

/*!
 * \brief Create a dataset from CSR format.
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col Number of columns
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromCSR_t) (const void* indptr,
                                                int indptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t nindptr,
                                                int64_t nelem,
                                                int64_t num_col,
                                                const char* parameters,
                                                const DatasetHandle reference,
                                                DatasetHandle* out);
static LGBM_DatasetCreateFromCSR_t LGBM_DatasetCreateFromCSR = NULL;

/*!
 * \brief Create a dataset from CSR format through callbacks.
 * \param get_row_funptr Pointer to ``std::function<void(int idx, std::vector<std::pair<int, double>>& ret)>``
 *                       (called for every row and expected to clear and fill ``ret``)
 * \param num_rows Number of rows
 * \param num_col Number of columns
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromCSRFunc_t) (void* get_row_funptr,
                                                    int num_rows,
                                                    int64_t num_col,
                                                    const char* parameters,
                                                    const DatasetHandle reference,
                                                    DatasetHandle* out);
static LGBM_DatasetCreateFromCSRFunc_t LGBM_DatasetCreateFromCSRFunc = NULL;

/*!
 * \brief Create a dataset from CSC format.
 * \param col_ptr Pointer to column headers
 * \param col_ptr_type Type of ``col_ptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to row indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param ncol_ptr Number of columns in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_row Number of rows
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromCSC_t) (const void* col_ptr,
                                                int col_ptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t ncol_ptr,
                                                int64_t nelem,
                                                int64_t num_row,
                                                const char* parameters,
                                                const DatasetHandle reference,
                                                DatasetHandle* out);
static LGBM_DatasetCreateFromCSC_t LGBM_DatasetCreateFromCSC = NULL;

/*!
 * \brief Create dataset from dense matrix.
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of columns
 * \param is_row_major 1 for row-major, 0 for column-major
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromMat_t) (const void* data,
                                                int data_type,
                                                int32_t nrow,
                                                int32_t ncol,
                                                int is_row_major,
                                                const char* parameters,
                                                const DatasetHandle reference,
                                                DatasetHandle* out);
static LGBM_DatasetCreateFromMat_t LGBM_DatasetCreateFromMat = NULL;

/*!
 * \brief Create dataset from array of dense matrices.
 * \param nmat Number of dense matrices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of columns
 * \param is_row_major 1 for row-major, 0 for column-major
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromMats_t) (int32_t nmat,
                                                 const void** data,
                                                 int data_type,
                                                 int32_t* nrow,
                                                 int32_t ncol,
                                                 int is_row_major,
                                                 const char* parameters,
                                                 const DatasetHandle reference,
                                                 DatasetHandle* out);
static LGBM_DatasetCreateFromMats_t LGBM_DatasetCreateFromMats = NULL;

/*!
 * \brief Create subset of a data.
 * \param handle Handle of full dataset
 * \param used_row_indices Indices used in subset
 * \param num_used_row_indices Length of ``used_row_indices``
 * \param parameters Additional parameters
 * \param[out] out Subset of data
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetSubset_t) (const DatasetHandle handle,
                                            const int32_t* used_row_indices,
                                            int32_t num_used_row_indices,
                                            const char* parameters,
                                            DatasetHandle* out);
static LGBM_DatasetGetSubset_t LGBM_DatasetGetSubset = NULL;

/*!
 * \brief Save feature names to dataset.
 * \param handle Handle of dataset
 * \param feature_names Feature names
 * \param num_feature_names Number of feature names
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSetFeatureNames_t) (DatasetHandle handle,
                                                  const char** feature_names,
                                                  int num_feature_names);
static LGBM_DatasetSetFeatureNames_t LGBM_DatasetSetFeatureNames = NULL;

/*!
 * \brief Get feature names of dataset.
 * \param handle Handle of dataset
 * \param len Number of ``char*`` pointers stored at ``out_strs``.
 *            If smaller than the max size, only this many strings are copied
 * \param[out] num_feature_names Number of feature names
 * \param buffer_len Size of pre-allocated strings.
 *                   Content is copied up to ``buffer_len - 1`` and null-terminated
 * \param[out] out_buffer_len String sizes required to do the full string copies
 * \param[out] feature_names Feature names, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetFeatureNames_t) (DatasetHandle handle,
                                                  const int len,
                                                  int* num_feature_names,
                                                  const size_t buffer_len,
                                                  size_t* out_buffer_len,
                                                  char** feature_names);
static LGBM_DatasetGetFeatureNames_t LGBM_DatasetGetFeatureNames = NULL;

/*!
 * \brief Free space for dataset.
 * \param handle Handle of dataset to be freed
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetFree_t) (DatasetHandle handle);
static LGBM_DatasetFree_t LGBM_DatasetFree = NULL;

/*!
 * \brief Save dataset to binary file.
 * \param handle Handle of dataset
 * \param filename The name of the file
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSaveBinary_t) (DatasetHandle handle,
                                             const char* filename);
static LGBM_DatasetSaveBinary_t LGBM_DatasetSaveBinary = NULL;

/*!
 * \brief Save dataset to text file, intended for debugging use only.
 * \param handle Handle of dataset
 * \param filename The name of the file
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetDumpText_t) (DatasetHandle handle,
                                           const char* filename);
static LGBM_DatasetDumpText_t LGBM_DatasetDumpText = NULL;

/*!
 * \brief Set vector to a content in info.
 * \note
 * - \a group only works for ``C_API_DTYPE_INT32``;
 * - \a label and \a weight only work for ``C_API_DTYPE_FLOAT32``;
 * - \a init_score only works for ``C_API_DTYPE_FLOAT64``.
 * \param handle Handle of dataset
 * \param field_name Field name, can be \a label, \a weight, \a init_score, \a group
 * \param field_data Pointer to data vector
 * \param num_element Number of elements in ``field_data``
 * \param type Type of ``field_data`` pointer, can be ``C_API_DTYPE_INT32``, ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSetField_t) (DatasetHandle handle,
                                           const char* field_name,
                                           const void* field_data,
                                           int num_element,
                                           int type);
static LGBM_DatasetSetField_t LGBM_DatasetSetField = NULL;

/*!
 * \brief Get info vector from dataset.
 * \param handle Handle of dataset
 * \param field_name Field name
 * \param[out] out_len Used to set result length
 * \param[out] out_ptr Pointer to the result
 * \param[out] out_type Type of result pointer, can be ``C_API_DTYPE_INT32``, ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetField_t) (DatasetHandle handle,
                                           const char* field_name,
                                           int* out_len,
                                           const void** out_ptr,
                                           int* out_type);
static LGBM_DatasetGetField_t LGBM_DatasetGetField = NULL;

/*!
 * \brief Raise errors for attempts to update dataset parameters.
 * \param old_parameters Current dataset parameters
 * \param new_parameters New dataset parameters
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetUpdateParamChecking_t) (const char* old_parameters,
                                                      const char* new_parameters);
static LGBM_DatasetUpdateParamChecking_t LGBM_DatasetUpdateParamChecking = NULL;

/*!
 * \brief Get number of data points.
 * \param handle Handle of dataset
 * \param[out] out The address to hold number of data points
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetNumData_t) (DatasetHandle handle,
                                             int* out);
static LGBM_DatasetGetNumData_t LGBM_DatasetGetNumData = NULL;

/*!
 * \brief Get number of features.
 * \param handle Handle of dataset
 * \param[out] out The address to hold number of features
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetNumFeature_t) (DatasetHandle handle,
                                                int* out);
static LGBM_DatasetGetNumFeature_t LGBM_DatasetGetNumFeature = NULL;

/*!
 * \brief Add features from ``source`` to ``target``.
 * \param target The handle of the dataset to add features to
 * \param source The handle of the dataset to take features from
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetAddFeaturesFrom_t) (DatasetHandle target,
                                                  DatasetHandle source);
static LGBM_DatasetAddFeaturesFrom_t LGBM_DatasetAddFeaturesFrom = NULL;

/* --- start Booster interfaces */

/*!
* \brief Get boolean representing whether booster is fitting linear trees.
* \param handle Handle of booster
* \param[out] out The address to hold linear trees indicator
* \return 0 when succeed, -1 when failure happens
*/
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetLinear_t) (BoosterHandle handle, bool* out);
static LGBM_BoosterGetLinear_t LGBM_BoosterGetLinear = NULL;

/*!
 * \brief Create a new boosting learner.
 * \param train_data Training dataset
 * \param parameters Parameters in format 'key1=value1 key2=value2'
 * \param[out] out Handle of created booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterCreate_t) (const DatasetHandle train_data,
                                         const char* parameters,
                                         BoosterHandle* out);
static LGBM_BoosterCreate_t LGBM_BoosterCreate = NULL;

/*!
 * \brief Load an existing booster from model file.
 * \param filename Filename of model
 * \param[out] out_num_iterations Number of iterations of this booster
 * \param[out] out Handle of created booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterCreateFromModelfile_t) (const char* filename,
                                                      int* out_num_iterations,
                                                      BoosterHandle* out);
static LGBM_BoosterCreateFromModelfile_t LGBM_BoosterCreateFromModelfile = NULL;

/*!
 * \brief Load an existing booster from string.
 * \param model_str Model string
 * \param[out] out_num_iterations Number of iterations of this booster
 * \param[out] out Handle of created booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterLoadModelFromString_t) (const char* model_str,
                                                      int* out_num_iterations,
                                                      BoosterHandle* out);
static LGBM_BoosterLoadModelFromString_t LGBM_BoosterLoadModelFromString = NULL;

/*!
 * \brief Free space for booster.
 * \param handle Handle of booster to be freed
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterFree_t) (BoosterHandle handle);
static LGBM_BoosterFree_t LGBM_BoosterFree = NULL;

/*!
 * \brief Shuffle models.
 * \param handle Handle of booster
 * \param start_iter The first iteration that will be shuffled
 * \param end_iter The last iteration that will be shuffled
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterShuffleModels_t) (BoosterHandle handle,
                                                int start_iter,
                                                int end_iter);
static LGBM_BoosterShuffleModels_t LGBM_BoosterShuffleModels = NULL;

/*!
 * \brief Merge model from ``other_handle`` into ``handle``.
 * \param handle Handle of booster, will merge another booster into this one
 * \param other_handle Other handle of booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterMerge_t) (BoosterHandle handle,
                                        BoosterHandle other_handle);
static LGBM_BoosterMerge_t LGBM_BoosterMerge = NULL;

/*!
 * \brief Add new validation data to booster.
 * \param handle Handle of booster
 * \param valid_data Validation dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterAddValidData_t) (BoosterHandle handle,
                                               const DatasetHandle valid_data);
static LGBM_BoosterAddValidData_t LGBM_BoosterAddValidData = NULL;

/*!
 * \brief Reset training data for booster.
 * \param handle Handle of booster
 * \param train_data Training dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterResetTrainingData_t) (BoosterHandle handle,
                                                    const DatasetHandle train_data);
static LGBM_BoosterResetTrainingData_t LGBM_BoosterResetTrainingData = NULL;

/*!
 * \brief Reset config for booster.
 * \param handle Handle of booster
 * \param parameters Parameters in format 'key1=value1 key2=value2'
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterResetParameter_t) (BoosterHandle handle,
                                                 const char* parameters);
static LGBM_BoosterResetParameter_t LGBM_BoosterResetParameter = NULL;

/*!
 * \brief Get number of classes.
 * \param handle Handle of booster
 * \param[out] out_len Number of classes
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetNumClasses_t) (BoosterHandle handle,
                                                int* out_len);
static LGBM_BoosterGetNumClasses_t LGBM_BoosterGetNumClasses = NULL;

/*!
 * \brief Update the model for one iteration.
 * \param handle Handle of booster
 * \param[out] is_finished 1 means the update was successfully finished (cannot split any more), 0 indicates failure
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterUpdateOneIter_t) (BoosterHandle handle,
                                                int* is_finished);
static LGBM_BoosterUpdateOneIter_t LGBM_BoosterUpdateOneIter = NULL;

/*!
 * \brief Refit the tree model using the new data (online learning).
 * \param handle Handle of booster
 * \param leaf_preds Pointer to predicted leaf indices
 * \param nrow Number of rows of ``leaf_preds``
 * \param ncol Number of columns of ``leaf_preds``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterRefit_t) (BoosterHandle handle,
                                        const int32_t* leaf_preds,
                                        int32_t nrow,
                                        int32_t ncol);
static LGBM_BoosterRefit_t LGBM_BoosterRefit = NULL;

/*!
 * \brief Update the model by specifying gradient and Hessian directly
 *        (this can be used to support customized loss functions).
 * \param handle Handle of booster
 * \param grad The first order derivative (gradient) statistics
 * \param hess The second order derivative (Hessian) statistics
 * \param[out] is_finished 1 means the update was successfully finished (cannot split any more), 0 indicates failure
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterUpdateOneIterCustom_t) (BoosterHandle handle,
                                                      const float* grad,
                                                      const float* hess,
                                                      int* is_finished);
static LGBM_BoosterUpdateOneIterCustom_t LGBM_BoosterUpdateOneIterCustom = NULL;

/*!
 * \brief Rollback one iteration.
 * \param handle Handle of booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterRollbackOneIter_t) (BoosterHandle handle);
static LGBM_BoosterRollbackOneIter_t LGBM_BoosterRollbackOneIter = NULL;

/*!
 * \brief Get index of the current boosting iteration.
 * \param handle Handle of booster
 * \param[out] out_iteration Index of the current boosting iteration
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetCurrentIteration_t) (BoosterHandle handle,
                                                      int* out_iteration);
static LGBM_BoosterGetCurrentIteration_t LGBM_BoosterGetCurrentIteration = NULL;

/*!
 * \brief Get number of trees per iteration.
 * \param handle Handle of booster
 * \param[out] out_tree_per_iteration Number of trees per iteration
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterNumModelPerIteration_t) (BoosterHandle handle,
                                                       int* out_tree_per_iteration);
static LGBM_BoosterNumModelPerIteration_t LGBM_BoosterNumModelPerIteration = NULL;

/*!
 * \brief Get number of weak sub-models.
 * \param handle Handle of booster
 * \param[out] out_models Number of weak sub-models
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterNumberOfTotalModel_t) (BoosterHandle handle,
                                                     int* out_models);
static LGBM_BoosterNumberOfTotalModel_t LGBM_BoosterNumberOfTotalModel = NULL;

/*!
 * \brief Get number of evaluation metrics.
 * \param handle Handle of booster
 * \param[out] out_len Total number of evaluation metrics
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetEvalCounts_t) (BoosterHandle handle,
                                                int* out_len);
static LGBM_BoosterGetEvalCounts_t LGBM_BoosterGetEvalCounts = NULL;

/*!
 * \brief Get names of evaluation metrics.
 * \param handle Handle of booster
 * \param len Number of ``char*`` pointers stored at ``out_strs``.
 *            If smaller than the max size, only this many strings are copied
 * \param[out] out_len Total number of evaluation metrics
 * \param buffer_len Size of pre-allocated strings.
 *                   Content is copied up to ``buffer_len - 1`` and null-terminated
 * \param[out] out_buffer_len String sizes required to do the full string copies
 * \param[out] out_strs Names of evaluation metrics, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetEvalNames_t) (BoosterHandle handle,
                                               const int len,
                                               int* out_len,
                                               const size_t buffer_len,
                                               size_t* out_buffer_len,
                                               char** out_strs);
static LGBM_BoosterGetEvalNames_t LGBM_BoosterGetEvalNames = NULL;

/*!
 * \brief Get names of features.
 * \param handle Handle of booster
 * \param len Number of ``char*`` pointers stored at ``out_strs``.
 *            If smaller than the max size, only this many strings are copied
 * \param[out] out_len Total number of features
 * \param buffer_len Size of pre-allocated strings.
 *                   Content is copied up to ``buffer_len - 1`` and null-terminated
 * \param[out] out_buffer_len String sizes required to do the full string copies
 * \param[out] out_strs Names of features, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetFeatureNames_t) (BoosterHandle handle,
                                                  const int len,
                                                  int* out_len,
                                                  const size_t buffer_len,
                                                  size_t* out_buffer_len,
                                                  char** out_strs);
static LGBM_BoosterGetFeatureNames_t LGBM_BoosterGetFeatureNames = NULL;

/*!
 * \brief Get number of features.
 * \param handle Handle of booster
 * \param[out] out_len Total number of features
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetNumFeature_t) (BoosterHandle handle,
                                                int* out_len);
static LGBM_BoosterGetNumFeature_t LGBM_BoosterGetNumFeature = NULL;

/*!
 * \brief Get evaluation for training data and validation data.
 * \note
 *   1. You should call ``LGBM_BoosterGetEvalNames`` first to get the names of evaluation metrics.
 *   2. You should pre-allocate memory for ``out_results``, you can get its length by ``LGBM_BoosterGetEvalCounts``.
 * \param handle Handle of booster
 * \param data_idx Index of data, 0: training data, 1: 1st validation data, 2: 2nd validation data and so on
 * \param[out] out_len Length of output result
 * \param[out] out_results Array with evaluation results
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetEval_t) (BoosterHandle handle,
                                          int data_idx,
                                          int* out_len,
                                          double* out_results);
static LGBM_BoosterGetEval_t LGBM_BoosterGetEval = NULL;

/*!
 * \brief Get number of predictions for training data and validation data
 *        (this can be used to support customized evaluation functions).
 * \param handle Handle of booster
 * \param data_idx Index of data, 0: training data, 1: 1st validation data, 2: 2nd validation data and so on
 * \param[out] out_len Number of predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetNumPredict_t) (BoosterHandle handle,
                                                int data_idx,
                                                int64_t* out_len);
static LGBM_BoosterGetNumPredict_t LGBM_BoosterGetNumPredict = NULL;

/*!
 * \brief Get prediction for training data and validation data.
 * \note
 * You should pre-allocate memory for ``out_result``, its length is equal to ``num_class * num_data``.
 * \param handle Handle of booster
 * \param data_idx Index of data, 0: training data, 1: 1st validation data, 2: 2nd validation data and so on
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetPredict_t) (BoosterHandle handle,
                                             int data_idx,
                                             int64_t* out_len,
                                             double* out_result);
static LGBM_BoosterGetPredict_t LGBM_BoosterGetPredict = NULL;

/*!
 * \brief Make prediction for file.
 * \param handle Handle of booster
 * \param data_filename Filename of file with data
 * \param data_has_header Whether file has header or not
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param result_filename Filename of result file in which predictions will be written
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForFile_t) (BoosterHandle handle,
                                                 const char* data_filename,
                                                 int data_has_header,
                                                 int predict_type,
                                                 int start_iteration,
                                                 int num_iteration,
                                                 const char* parameter,
                                                 const char* result_filename);
static LGBM_BoosterPredictForFile_t LGBM_BoosterPredictForFile = NULL;

/*!
 * \brief Get number of predictions.
 * \param handle Handle of booster
 * \param num_row Number of rows
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param[out] out_len Length of prediction
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterCalcNumPredict_t) (BoosterHandle handle,
                                                 int num_row,
                                                 int predict_type,
                                                 int start_iteration,
                                                 int num_iteration,
                                                 int64_t* out_len);
static LGBM_BoosterCalcNumPredict_t LGBM_BoosterCalcNumPredict = NULL;

/*!
 * \brief Release FastConfig object.
 *
 * \param fastConfig Handle to the FastConfig object acquired with a ``*FastInit()`` method.
 * \return 0 when it succeeds, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_FastConfigFree_t) (FastConfigHandle fastConfig);
static LGBM_FastConfigFree_t LGBM_FastConfigFree = NULL;

/*!
 * \brief Make prediction for a new dataset in CSR format.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col Number of columns
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSR_t) (BoosterHandle handle,
                                                const void* indptr,
                                                int indptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t nindptr,
                                                int64_t nelem,
                                                int64_t num_col,
                                                int predict_type,
                                                int start_iteration,
                                                int num_iteration,
                                                const char* parameter,
                                                int64_t* out_len,
                                                double* out_result);
static LGBM_BoosterPredictForCSR_t LGBM_BoosterPredictForCSR = NULL;

/*!
 * \brief Make sparse prediction for a new dataset in CSR or CSC format. Currently only used for feature contributions.
 * \note
 * The outputs are pre-allocated, as they can vary for each invocation, but the shape should be the same:
 *   - for feature contributions, the shape of sparse matrix will be ``num_class * num_data * (num_feature + 1)``.
 * The output indptr_type for the sparse matrix will be the same as the given input indptr_type.
 * Call ``LGBM_BoosterFreePredictSparse`` to deallocate resources.
 * \param handle Handle of booster
 * \param indptr Pointer to row headers for CSR or column headers for CSC
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices for CSR or row indices for CSC
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col_or_row Number of columns for CSR or number of rows for CSC
 * \param predict_type What should be predicted, only feature contributions supported currently
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param matrix_type Type of matrix input and output, can be ``C_API_MATRIX_TYPE_CSR`` or ``C_API_MATRIX_TYPE_CSC``
 * \param[out] out_len Length of output indices and data
 * \param[out] out_indptr Pointer to output row headers for CSR or column headers for CSC
 * \param[out] out_indices Pointer to sparse column indices for CSR or row indices for CSC
 * \param[out] out_data Pointer to sparse data space
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictSparseOutput_t) (BoosterHandle handle,
                                                      const void* indptr,
                                                      int indptr_type,
                                                      const int32_t* indices,
                                                      const void* data,
                                                      int data_type,
                                                      int64_t nindptr,
                                                      int64_t nelem,
                                                      int64_t num_col_or_row,
                                                      int predict_type,
                                                      int start_iteration,
                                                      int num_iteration,
                                                      const char* parameter,
                                                      int matrix_type,
                                                      int64_t* out_len,
                                                      void** out_indptr,
                                                      int32_t** out_indices,
                                                      void** out_data);
static LGBM_BoosterPredictSparseOutput_t LGBM_BoosterPredictSparseOutput = NULL;

/*!
 * \brief Method corresponding to ``LGBM_BoosterPredictSparseOutput`` to free the allocated data.
 * \param indptr Pointer to output row headers or column headers to be deallocated
 * \param indices Pointer to sparse indices to be deallocated
 * \param data Pointer to sparse data space to be deallocated
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterFreePredictSparse_t) (void* indptr, int32_t* indices, void* data, int indptr_type, int data_type);
static LGBM_BoosterFreePredictSparse_t LGBM_BoosterFreePredictSparse = NULL;

/*!
 * \brief Make prediction for a new dataset in CSR format. This method re-uses the internal predictor structure
 *        from previous calls and is optimized for single row invocation.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col Number of columns
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSRSingleRow_t) (BoosterHandle handle,
                                                         const void* indptr,
                                                         int indptr_type,
                                                         const int32_t* indices,
                                                         const void* data,
                                                         int data_type,
                                                         int64_t nindptr,
                                                         int64_t nelem,
                                                         int64_t num_col,
                                                         int predict_type,
                                                         int start_iteration,
                                                         int num_iteration,
                                                         const char* parameter,
                                                         int64_t* out_len,
                                                         double* out_result);
static LGBM_BoosterPredictForCSRSingleRow_t LGBM_BoosterPredictForCSRSingleRow = NULL;

/*!
 * \brief Initialize and return a ``FastConfigHandle`` for use with ``LGBM_BoosterPredictForCSRSingleRowFast``.
 *
 * Release the ``FastConfig`` by passing its handle to ``LGBM_FastConfigFree`` when no longer needed.
 *
 * \param handle Booster handle
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param num_col Number of columns
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_fastConfig FastConfig object with which you can call ``LGBM_BoosterPredictForCSRSingleRowFast``
 * \return 0 when it succeeds, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSRSingleRowFastInit_t) (BoosterHandle handle,
                                                                 const int predict_type,
                                                                 const int start_iteration,
                                                                 const int num_iteration,
                                                                 const int data_type,
                                                                 const int64_t num_col,
                                                                 const char* parameter,
                                                                 FastConfigHandle *out_fastConfig);
static LGBM_BoosterPredictForCSRSingleRowFastInit_t LGBM_BoosterPredictForCSRSingleRowFastInit = NULL;

/*!
 * \brief Faster variant of ``LGBM_BoosterPredictForCSRSingleRow``.
 *
 * Score single rows after setup with ``LGBM_BoosterPredictForCSRSingleRowFastInit``.
 *
 * By removing the setup steps from this call extra optimizations can be made like
 * initializing the config only once, instead of once per call.
 *
 * \note
 *   Setting up the number of threads is only done once at ``LGBM_BoosterPredictForCSRSingleRowFastInit``
 *   instead of at each prediction.
 *   If you use a different number of threads in other calls, you need to start the setup process over,
 *   or that number of threads will be used for these calls as well.
 *
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 *
 * \param fastConfig_handle FastConfig object handle returned by ``LGBM_BoosterPredictForCSRSingleRowFastInit``
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSRSingleRowFast_t) (FastConfigHandle fastConfig_handle,
                                                             const void* indptr,
                                                             const int indptr_type,
                                                             const int32_t* indices,
                                                             const void* data,
                                                             const int64_t nindptr,
                                                             const int64_t nelem,
                                                             int64_t* out_len,
                                                             double* out_result);
static LGBM_BoosterPredictForCSRSingleRowFast_t LGBM_BoosterPredictForCSRSingleRowFast = NULL;

/*!
 * \brief Make prediction for a new dataset in CSC format.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param col_ptr Pointer to column headers
 * \param col_ptr_type Type of ``col_ptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to row indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param ncol_ptr Number of columns in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_row Number of rows
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSC_t) (BoosterHandle handle,
                                                const void* col_ptr,
                                                int col_ptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t ncol_ptr,
                                                int64_t nelem,
                                                int64_t num_row,
                                                int predict_type,
                                                int start_iteration,
                                                int num_iteration,
                                                const char* parameter,
                                                int64_t* out_len,
                                                double* out_result);
static LGBM_BoosterPredictForCSC_t LGBM_BoosterPredictForCSC = NULL;

/*!
 * \brief Make prediction for a new dataset.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of columns
 * \param is_row_major 1 for row-major, 0 for column-major
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMat_t) (BoosterHandle handle,
                                                const void* data,
                                                int data_type,
                                                int32_t nrow,
                                                int32_t ncol,
                                                int is_row_major,
                                                int predict_type,
                                                int start_iteration,
                                                int num_iteration,
                                                const char* parameter,
                                                int64_t* out_len,
                                                double* out_result);
static LGBM_BoosterPredictForMat_t LGBM_BoosterPredictForMat = NULL;

/*!
 * \brief Make prediction for a new dataset. This method re-uses the internal predictor structure
 *        from previous calls and is optimized for single row invocation.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param ncol Number columns
 * \param is_row_major 1 for row-major, 0 for column-major
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMatSingleRow_t) (BoosterHandle handle,
                                                         const void* data,
                                                         int data_type,
                                                         int ncol,
                                                         int is_row_major,
                                                         int predict_type,
                                                         int start_iteration,
                                                         int num_iteration,
                                                         const char* parameter,
                                                         int64_t* out_len,
                                                         double* out_result);
static LGBM_BoosterPredictForMatSingleRow_t LGBM_BoosterPredictForMatSingleRow = NULL;

/*!
 * \brief Initialize and return a ``FastConfigHandle`` for use with ``LGBM_BoosterPredictForMatSingleRowFast``.
 *
 * Release the ``FastConfig`` by passing its handle to ``LGBM_FastConfigFree`` when no longer needed.
 *
 * \param handle Booster handle
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param ncol Number of columns
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_fastConfig FastConfig object with which you can call ``LGBM_BoosterPredictForMatSingleRowFast``
 * \return 0 when it succeeds, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMatSingleRowFastInit_t) (BoosterHandle handle,
                                                                 const int predict_type,
                                                                 const int start_iteration,
                                                                 const int num_iteration,
                                                                 const int data_type,
                                                                 const int32_t ncol,
                                                                 const char* parameter,
                                                                 FastConfigHandle *out_fastConfig);
static LGBM_BoosterPredictForMatSingleRowFastInit_t LGBM_BoosterPredictForMatSingleRowFastInit = NULL;

/*!
 * \brief Faster variant of ``LGBM_BoosterPredictForMatSingleRow``.
 *
 * Score a single row after setup with ``LGBM_BoosterPredictForMatSingleRowFastInit``.
 *
 * By removing the setup steps from this call extra optimizations can be made like
 * initializing the config only once, instead of once per call.
 *
 * \note
 *   Setting up the number of threads is only done once at ``LGBM_BoosterPredictForMatSingleRowFastInit``
 *   instead of at each prediction.
 *   If you use a different number of threads in other calls, you need to start the setup process over,
 *   or that number of threads will be used for these calls as well.
 *
 * \param fastConfig_handle FastConfig object handle returned by ``LGBM_BoosterPredictForMatSingleRowFastInit``
 * \param data Single-row array data (no other way than row-major form).
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when it succeeds, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMatSingleRowFast_t) (FastConfigHandle fastConfig_handle,
                                                             const void* data,
                                                             int64_t* out_len,
                                                             double* out_result);
static LGBM_BoosterPredictForMatSingleRowFast_t LGBM_BoosterPredictForMatSingleRowFast = NULL;

/*!
 * \brief Make prediction for a new dataset presented in a form of array of pointers to rows.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number columns
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMats_t) (BoosterHandle handle,
                                                 const void** data,
                                                 int data_type,
                                                 int32_t nrow,
                                                 int32_t ncol,
                                                 int predict_type,
                                                 int start_iteration,
                                                 int num_iteration,
                                                 const char* parameter,
                                                 int64_t* out_len,
                                                 double* out_result);
static LGBM_BoosterPredictForMats_t LGBM_BoosterPredictForMats = NULL;

/*!
 * \brief Save model into file.
 * \param handle Handle of booster
 * \param start_iteration Start index of the iteration that should be saved
 * \param num_iteration Index of the iteration that should be saved, <= 0 means save all
 * \param feature_importance_type Type of feature importance, can be ``C_API_FEATURE_IMPORTANCE_SPLIT`` or ``C_API_FEATURE_IMPORTANCE_GAIN``
 * \param filename The name of the file
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterSaveModel_t) (BoosterHandle handle,
                                            int start_iteration,
                                            int num_iteration,
                                            int feature_importance_type,
                                            const char* filename);
static LGBM_BoosterSaveModel_t LGBM_BoosterSaveModel = NULL;

/*!
 * \brief Save model to string.
 * \param handle Handle of booster
 * \param start_iteration Start index of the iteration that should be saved
 * \param num_iteration Index of the iteration that should be saved, <= 0 means save all
 * \param feature_importance_type Type of feature importance, can be ``C_API_FEATURE_IMPORTANCE_SPLIT`` or ``C_API_FEATURE_IMPORTANCE_GAIN``
 * \param buffer_len String buffer length, if ``buffer_len < out_len``, you should re-allocate buffer
 * \param[out] out_len Actual output length
 * \param[out] out_str String of model, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterSaveModelToString_t) (BoosterHandle handle,
                                                    int start_iteration,
                                                    int num_iteration,
                                                    int feature_importance_type,
                                                    int64_t buffer_len,
                                                    int64_t* out_len,
                                                    char* out_str);
static LGBM_BoosterSaveModelToString_t LGBM_BoosterSaveModelToString = NULL;

/*!
 * \brief Dump model to JSON.
 * \param handle Handle of booster
 * \param start_iteration Start index of the iteration that should be dumped
 * \param num_iteration Index of the iteration that should be dumped, <= 0 means dump all
 * \param feature_importance_type Type of feature importance, can be ``C_API_FEATURE_IMPORTANCE_SPLIT`` or ``C_API_FEATURE_IMPORTANCE_GAIN``
 * \param buffer_len String buffer length, if ``buffer_len < out_len``, you should re-allocate buffer
 * \param[out] out_len Actual output length
 * \param[out] out_str JSON format string of model, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterDumpModel_t) (BoosterHandle handle,
                                            int start_iteration,
                                            int num_iteration,
                                            int feature_importance_type,
                                            int64_t buffer_len,
                                            int64_t* out_len,
                                            char* out_str);
static LGBM_BoosterDumpModel_t LGBM_BoosterDumpModel = NULL;

/*!
 * \brief Get leaf value.
 * \param handle Handle of booster
 * \param tree_idx Index of tree
 * \param leaf_idx Index of leaf
 * \param[out] out_val Output result from the specified leaf
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetLeafValue_t) (BoosterHandle handle,
                                               int tree_idx,
                                               int leaf_idx,
                                               double* out_val);
static LGBM_BoosterGetLeafValue_t LGBM_BoosterGetLeafValue = NULL;

/*!
 * \brief Set leaf value.
 * \param handle Handle of booster
 * \param tree_idx Index of tree
 * \param leaf_idx Index of leaf
 * \param val Leaf value
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterSetLeafValue_t) (BoosterHandle handle,
                                               int tree_idx,
                                               int leaf_idx,
                                               double val);
static LGBM_BoosterSetLeafValue_t LGBM_BoosterSetLeafValue = NULL;

/*!
 * \brief Get model feature importance.
 * \param handle Handle of booster
 * \param num_iteration Number of iterations for which feature importance is calculated, <= 0 means use all
 * \param importance_type Method of importance calculation:
 *   - ``C_API_FEATURE_IMPORTANCE_SPLIT``: result contains numbers of times the feature is used in a model;
 *   - ``C_API_FEATURE_IMPORTANCE_GAIN``: result contains total gains of splits which use the feature
 * \param[out] out_results Result array with feature importance
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterFeatureImportance_t) (BoosterHandle handle,
                                                    int num_iteration,
                                                    int importance_type,
                                                    double* out_results);
static LGBM_BoosterFeatureImportance_t LGBM_BoosterFeatureImportance = NULL;

/*!
 * \brief Get model upper bound value.
 * \param handle Handle of booster
 * \param[out] out_results Result pointing to max value
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetUpperBoundValue_t) (BoosterHandle handle,
                                                     double* out_results);
static LGBM_BoosterGetUpperBoundValue_t LGBM_BoosterGetUpperBoundValue = NULL;

/*!
 * \brief Get model lower bound value.
 * \param handle Handle of booster
 * \param[out] out_results Result pointing to min value
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetLowerBoundValue_t) (BoosterHandle handle,
                                                     double* out_results);
static LGBM_BoosterGetLowerBoundValue_t LGBM_BoosterGetLowerBoundValue = NULL;

/*!
 * \brief Initialize the network.
 * \param machines List of machines in format 'ip1:port1,ip2:port2'
 * \param local_listen_port TCP listen port for local machines
 * \param listen_time_out Socket time-out in minutes
 * \param num_machines Total number of machines
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_NetworkInit_t) (const char* machines,
                                       int local_listen_port,
                                       int listen_time_out,
                                       int num_machines);
static LGBM_NetworkInit_t LGBM_NetworkInit = NULL;

/*!
 * \brief Finalize the network.
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_NetworkFree_t) ();
static LGBM_NetworkFree_t LGBM_NetworkFree = NULL;

/*!
 * \brief Initialize the network with external collective functions.
 * \param num_machines Total number of machines
 * \param rank Rank of local machine
 * \param reduce_scatter_ext_fun The external reduce-scatter function
 * \param allgather_ext_fun The external allgather function
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_NetworkInitWithFunctions_t) (int num_machines,
                                                    int rank,
                                                    void* reduce_scatter_ext_fun,
                                                    void* allgather_ext_fun);
static LGBM_NetworkInitWithFunctions_t LGBM_NetworkInitWithFunctions = NULL;

#if !defined(__cplusplus) && (!defined(__STDC__) || (__STDC_VERSION__ < 199901L))
/*! \brief Inline specifier no-op in C using standards before C99. */
#define INLINE_FUNCTION
#else
/*! \brief Inline specifier. */
// hack-lightgbm - fix warning
#define INLINE_FUNCTION static inline
#endif

#if !defined(__cplusplus) && (!defined(__STDC__) || (__STDC_VERSION__ < 201112L))
/*! \brief Thread local specifier no-op in C using standards before C11. */
#define THREAD_LOCAL
#elif !defined(__cplusplus)
/*! \brief Thread local specifier. */
#define THREAD_LOCAL _Thread_local
#elif defined(_MSC_VER)
/*! \brief Thread local specifier. */
#define THREAD_LOCAL __declspec(thread)
#else
/*! \brief Thread local specifier. */
#define THREAD_LOCAL thread_local
#endif

/*!
 * \brief Handle of error message.
 * \return Error message
 */
static char* LastErrorMsg() { static THREAD_LOCAL char err_msg[512] = "Everything is fine"; return err_msg; }

#ifdef _MSC_VER
  #pragma warning(disable : 4996)
#endif
/*!
 * \brief Set string message of the last error.
 * \param msg Error message
 */
INLINE_FUNCTION void LGBM_SetLastError(const char* msg) {
  const int err_buf_len = 512;
  snprintf(LastErrorMsg(), err_buf_len, "%s", msg);
}

#endif  /* LIGHTGBM_C_API_H_ */


/*
file none
*/
/*jslint-enable*/


// *INDENT-ON*
