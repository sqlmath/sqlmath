"""
test_lgbm.py.

python test_lgbm.py
"""

import ctypes
import filecmp
import pathlib
from platform import system

import numpy as np
from scipy import sparse

if system() in ("Windows", "Microsoft"):
    lib_file = "./lib_lightgbm.dll"
else:
    lib_file = "./lib_lightgbm.so"
LIB = ctypes.cdll.LoadLibrary(lib_file)

LIB.LGBM_GetLastError.restype = ctypes.c_char_p

dtype_float32 = 0
dtype_float64 = 1
dtype_int32 = 2
dtype_int64 = 3

FILE_BINARY_TEST = "test_lgbm_binary.test"
FILE_BINARY_TRAIN = "test_lgbm_binary.train"

def c_str(string):
    return ctypes.c_char_p(string.encode("utf-8"))


def load_from_file(filename, reference):
    ref = None
    if reference is not None:
        ref = reference
    handle = ctypes.c_void_p()
    LIB.LGBM_DatasetCreateFromFile(
        c_str(str(filename)),
        c_str("max_bin=15"),
        ref,
        ctypes.byref(handle))
    print(LIB.LGBM_GetLastError())
    num_data = ctypes.c_int(0)
    LIB.LGBM_DatasetGetNumData(handle, ctypes.byref(num_data))
    num_feature = ctypes.c_int(0)
    LIB.LGBM_DatasetGetNumFeature(handle, ctypes.byref(num_feature))
    print(f"#data: {num_data.value} #feature: {num_feature.value}")
    return handle


def save_to_binary(handle, filename):
    LIB.LGBM_DatasetSaveBinary(handle, c_str(filename))


def load_from_csr(filename, reference):
    data = np.loadtxt(str(filename), dtype=np.float64)
    csr = sparse.csr_matrix(data[:, 1:])
    label = data[:, 0].astype(np.float32)
    handle = ctypes.c_void_p()
    ref = None
    if reference is not None:
        ref = reference

    LIB.LGBM_DatasetCreateFromCSR(
        csr.indptr.ctypes.data_as(ctypes.POINTER(ctypes.c_int32)),
        ctypes.c_int(dtype_int32),
        csr.indices.ctypes.data_as(ctypes.POINTER(ctypes.c_int32)),
        csr.data.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        ctypes.c_int(dtype_float64),
        ctypes.c_int64(len(csr.indptr)),
        ctypes.c_int64(len(csr.data)),
        ctypes.c_int64(csr.shape[1]),
        c_str("max_bin=15"),
        ref,
        ctypes.byref(handle))
    num_data = ctypes.c_int(0)
    LIB.LGBM_DatasetGetNumData(handle, ctypes.byref(num_data))
    num_feature = ctypes.c_int(0)
    LIB.LGBM_DatasetGetNumFeature(handle, ctypes.byref(num_feature))
    LIB.LGBM_DatasetSetField(
        handle,
        c_str("label"),
        label.ctypes.data_as(ctypes.POINTER(ctypes.c_float)),
        ctypes.c_int(len(label)),
        ctypes.c_int(dtype_float32))
    print(f"#data: {num_data.value} #feature: {num_feature.value}")
    return handle


def load_from_csc(filename, reference):
    data = np.loadtxt(str(filename), dtype=np.float64)
    csc = sparse.csc_matrix(data[:, 1:])
    label = data[:, 0].astype(np.float32)
    handle = ctypes.c_void_p()
    ref = None
    if reference is not None:
        ref = reference

    LIB.LGBM_DatasetCreateFromCSC(
        csc.indptr.ctypes.data_as(ctypes.POINTER(ctypes.c_int32)),
        ctypes.c_int(dtype_int32),
        csc.indices.ctypes.data_as(ctypes.POINTER(ctypes.c_int32)),
        csc.data.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        ctypes.c_int(dtype_float64),
        ctypes.c_int64(len(csc.indptr)),
        ctypes.c_int64(len(csc.data)),
        ctypes.c_int64(csc.shape[0]),
        c_str("max_bin=15"),
        ref,
        ctypes.byref(handle))
    num_data = ctypes.c_int(0)
    LIB.LGBM_DatasetGetNumData(handle, ctypes.byref(num_data))
    num_feature = ctypes.c_int(0)
    LIB.LGBM_DatasetGetNumFeature(handle, ctypes.byref(num_feature))
    LIB.LGBM_DatasetSetField(
        handle,
        c_str("label"),
        label.ctypes.data_as(ctypes.POINTER(ctypes.c_float)),
        ctypes.c_int(len(label)),
        ctypes.c_int(dtype_float32))
    print(f"#data: {num_data.value} #feature: {num_feature.value}")
    return handle


def load_from_mat(filename, reference):
    mat = np.loadtxt(str(filename), dtype=np.float64)
    label = mat[:, 0].astype(np.float32)
    mat = mat[:, 1:]
    data = np.array(mat.reshape(mat.size), dtype=np.float64, copy=False)
    handle = ctypes.c_void_p()
    ref = None
    if reference is not None:
        ref = reference

    LIB.LGBM_DatasetCreateFromMat(
        data.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        ctypes.c_int(dtype_float64),
        ctypes.c_int32(mat.shape[0]),
        ctypes.c_int32(mat.shape[1]),
        ctypes.c_int(1),
        c_str("max_bin=15"),
        ref,
        ctypes.byref(handle))
    num_data = ctypes.c_int(0)
    LIB.LGBM_DatasetGetNumData(handle, ctypes.byref(num_data))
    num_feature = ctypes.c_int(0)
    LIB.LGBM_DatasetGetNumFeature(handle, ctypes.byref(num_feature))
    LIB.LGBM_DatasetSetField(
        handle,
        c_str("label"),
        label.ctypes.data_as(ctypes.POINTER(ctypes.c_float)),
        ctypes.c_int(len(label)),
        ctypes.c_int(dtype_float32))
    print(f"#data: {num_data.value} #feature: {num_feature.value}")
    return handle


def free_dataset(handle):
    LIB.LGBM_DatasetFree(handle)


def test_dataset():
    train = load_from_file(FILE_BINARY_TRAIN, None)
    test = load_from_mat(FILE_BINARY_TEST, train)
    free_dataset(test)
    test = load_from_csr(FILE_BINARY_TEST, train)
    free_dataset(test)
    test = load_from_csc(FILE_BINARY_TEST, train)
    free_dataset(test)
    pathlib.Path(".tmp/test_lgbm_train.binary.bin").unlink()
    save_to_binary(train, ".tmp/test_lgbm_train.binary.bin")
    free_dataset(train)
    train = load_from_file(".tmp/test_lgbm_train.binary.bin", None)
    free_dataset(train)


def test_booster():
    train = load_from_mat(FILE_BINARY_TRAIN, None)
    test = load_from_mat(FILE_BINARY_TEST, train)
    booster = ctypes.c_void_p()
    LIB.LGBM_BoosterCreate(
        train,
        c_str("app=binary metric=auc num_leaves=31 verbose=0"),
        ctypes.byref(booster))
    LIB.LGBM_BoosterAddValidData(booster, test)
    is_finished = ctypes.c_int(0)
    for i in range(1, 51):
        LIB.LGBM_BoosterUpdateOneIter(booster, ctypes.byref(is_finished))
        result = np.array([0.0], dtype=np.float64)
        out_len = ctypes.c_int(0)
        LIB.LGBM_BoosterGetEval(
            booster,
            ctypes.c_int(0),
            ctypes.byref(out_len),
            result.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
        if i % 10 == 0:
            print(f"{i} iteration test AUC {result[0]:.6f}")
    LIB.LGBM_BoosterSaveModel(
        booster,
        ctypes.c_int(0),
        ctypes.c_int(-1),
        ctypes.c_int(0),
        c_str(".tmp/test_lgbm_model.txt"))
    LIB.LGBM_BoosterFree(booster)
    free_dataset(train)
    free_dataset(test)
    booster2 = ctypes.c_void_p()
    num_total_model = ctypes.c_int(0)
    LIB.LGBM_BoosterCreateFromModelfile(
        c_str(".tmp/test_lgbm_model.txt"),
        ctypes.byref(num_total_model),
        ctypes.byref(booster2))
    data = np.loadtxt(FILE_BINARY_TEST, dtype=np.float64)
    mat = data[:, 1:]
    preb = np.empty(mat.shape[0], dtype=np.float64)
    num_preb = ctypes.c_int64(0)
    data = np.array(mat.reshape(mat.size), dtype=np.float64, copy=False)
    LIB.LGBM_BoosterPredictForMat(
        booster2,
        data.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        ctypes.c_int(dtype_float64),
        ctypes.c_int32(mat.shape[0]),
        ctypes.c_int32(mat.shape[1]),
        ctypes.c_int(1),
        ctypes.c_int(1),
        ctypes.c_int(0),
        ctypes.c_int(25),
        c_str(""),
        ctypes.byref(num_preb),
        preb.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
    LIB.LGBM_BoosterPredictForFile(
        booster2,
        c_str(FILE_BINARY_TEST),
        ctypes.c_int(0),
        ctypes.c_int(0),
        ctypes.c_int(0),
        ctypes.c_int(25),
        c_str(""),
        c_str(".tmp/test_lgbm_preb.txt"))
    LIB.LGBM_BoosterPredictForFile(
        booster2,
        c_str(FILE_BINARY_TEST),
        ctypes.c_int(0),
        ctypes.c_int(0),
        ctypes.c_int(10),
        ctypes.c_int(25),
        c_str(""),
        c_str(".tmp/test_lgbm_preb.txt"))
    LIB.LGBM_BoosterFree(booster2)
    assert filecmp.cmp(
        ".tmp/test_lgbm_preb.txt",
        "test_lgbm_preb.txt",
        shallow=False)

test_dataset()
test_booster()
