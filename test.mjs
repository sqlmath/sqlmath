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


/*jslint beta, node*/
import jslint from "./jslint.mjs";
import {
    SQLITE_WIN_SLR_ELEM_SIZE,
    assertJsonEqual,
    assertNumericalEqual,
    assertOrThrow,
    childProcessSpawn2,
    ciBuildExt,
    dbCloseAsync,
    dbExecAndReturnLastBlobAsync,
    dbExecAndReturnLastJsonAsync,
    dbExecAsync,
    dbFileExportAsync,
    dbFileImportAsync,
    dbNoopAsync,
    dbOpenAsync,
    debugInline,
    fsCopyFileUnlessTest,
    fsExistsUnlessTest,
    fsReadFileUnlessTest,
    fsWriteFileUnlessTest,
    jsbatonValueString,
    noop,
    sqlmathWebworkerInit,
    version
} from "./sqlmath.mjs";
let {
    assertErrorThrownAsync,
    jstestDescribe,
    jstestIt
} = jslint;
let testDataSpx = (function () {
    let data = String(`
##
Date Open High Low Close Volume
2019-12-31 3215.179932 3231.719971 3212.030029 3230.780029 2894760000
2020-01-02 3244.669922 3258.139893 3235.530029 3257.850098 3459930000
2020-01-03 3226.360107 3246.149902 3222.340088 3234.850098 3484700000
2020-01-06 3217.550049 3246.840088 3214.639893 3246.280029 3702460000
2020-01-07 3241.860107 3244.909912 3232.429932 3237.179932 3435910000
2020-01-08 3238.590088 3267.070068 3236.669922 3253.050049 3726840000
2020-01-09 3266.030029 3275.580078 3263.669922 3274.699951 3641230000
2020-01-10 3281.810059 3282.98999 3260.860107 3265.350098 3214580000
2020-01-13 3271.129883 3288.129883 3268.429932 3288.129883 3459390000
2020-01-14 3285.350098 3294.25 3277.189941 3283.149902 3687620000
2020-01-15 3282.27002 3298.659912 3280.689941 3289.290039 3721490000
2020-01-16 3302.969971 3317.110107 3302.820068 3316.810059 3540580000
2020-01-17 3323.659912 3329.879883 3318.860107 3329.620117 3726220000
2020-01-21 3321.030029 3329.790039 3316.610107 3320.790039 4104840000
2020-01-22 3330.02002 3337.77002 3320.040039 3321.75 3623780000
2020-01-23 3315.77002 3326.879883 3301.870117 3325.540039 3766710000
2020-01-24 3333.100098 3333.179932 3281.530029 3295.469971 3708780000
2020-01-27 3247.159912 3258.850098 3234.5 3243.629883 3831050000
2020-01-28 3255.350098 3285.780029 3253.219971 3276.23999 3531570000
2020-01-29 3289.459961 3293.469971 3271.889893 3273.399902 3600250000
2020-01-30 3256.449951 3285.909912 3242.800049 3283.659912 3790350000
2020-01-31 3282.330078 3282.330078 3214.679932 3225.52002 4529700000
2020-02-03 3235.659912 3268.439941 3235.659912 3248.919922 3760460000
2020-02-04 3280.610107 3306.919922 3280.610107 3297.590088 3996900000
2020-02-05 3324.909912 3337.580078 3313.75 3334.689941 4121480000
2020-02-06 3344.919922 3347.959961 3334.389893 3345.780029 3887250000
2020-02-07 3335.540039 3341.419922 3322.120117 3327.709961 3733920000
2020-02-10 3318.280029 3352.26001 3317.77002 3352.090088 3462730000
2020-02-11 3365.870117 3375.629883 3352.719971 3357.75 3762940000
2020-02-12 3370.5 3381.469971 3369.719971 3379.449951 3930910000
2020-02-13 3365.899902 3385.090088 3360.52002 3373.939941 3500890000
2020-02-14 3378.080078 3380.689941 3366.149902 3380.159912 3419700000
2020-02-18 3369.040039 3375.01001 3355.610107 3370.290039 3750400000
2020-02-19 3380.389893 3393.52002 3378.830078 3386.149902 3614200000
2020-02-20 3380.449951 3389.149902 3341.02002 3373.22998 4019180000
2020-02-21 3360.5 3360.76001 3328.449951 3337.75 3908780000
2020-02-24 3257.610107 3259.810059 3214.649902 3225.889893 4851160000
2020-02-25 3238.939941 3246.98999 3118.77002 3128.209961 5596760000
2020-02-26 3139.899902 3182.51001 3108.98999 3116.389893 5484650000
2020-02-27 3062.540039 3097.070068 2977.389893 2978.76001 7064710000
2020-02-28 2916.899902 2959.719971 2855.840088 2954.219971 8569570000
2020-03-02 2974.280029 3090.959961 2945.189941 3090.22998 6381330000
2020-03-03 3096.459961 3136.719971 2976.629883 3003.370117 6376510000
2020-03-04 3045.75 3130.969971 3034.379883 3130.120117 5073020000
2020-03-05 3075.699951 3083.040039 2999.830078 3023.939941 5579290000
2020-03-06 2954.199951 2985.929932 2901.540039 2972.370117 6555240000
2020-03-09 2863.889893 2863.889893 2734.429932 2746.560059 8441290000
2020-03-10 2813.47998 2882.590088 2734 2882.22998 7642040000
2020-03-11 2825.600098 2825.600098 2707.219971 2741.379883 7431200000
2020-03-12 2630.860107 2660.949951 2478.860107 2480.639893 8850810000
2020-03-13 2569.98999 2711.330078 2492.370117 2711.02002 8299070000
2020-03-16 2508.590088 2562.97998 2380.939941 2386.129883 7805450000
2020-03-17 2425.659912 2553.929932 2367.040039 2529.189941 8370250000
2020-03-18 2436.5 2453.570068 2280.52002 2398.100098 8799300000
2020-03-19 2393.47998 2466.969971 2319.780029 2409.389893 7956100000
2020-03-20 2431.939941 2453.01001 2295.560059 2304.919922 9053950000
2020-03-23 2290.709961 2300.72998 2191.860107 2237.399902 7411380000
2020-03-24 2344.439941 2449.709961 2344.439941 2447.330078 7563150000
2020-03-25 2457.77002 2571.419922 2407.530029 2475.560059 8300010000
2020-03-26 2501.290039 2637.01001 2500.719971 2630.070068 7766990000
2020-03-27 2555.870117 2615.909912 2520.02002 2541.469971 6201670000
2020-03-30 2558.97998 2631.800049 2545.280029 2626.649902 5751120000
2020-03-31 2614.689941 2641.389893 2571.149902 2584.590088 6576210000
2020-04-01 2498.080078 2522.75 2447.48999 2470.5 5964000000
2020-04-02 2458.540039 2533.219971 2455.790039 2526.899902 6464190000
2020-04-03 2514.919922 2538.179932 2459.959961 2488.649902 6096970000
2020-04-06 2578.280029 2676.850098 2574.570068 2663.679932 6403840000
2020-04-07 2738.649902 2756.889893 2657.669922 2659.409912 7050410000
2020-04-08 2685 2760.75 2663.300049 2749.97998 5875710000
2020-04-09 2776.98999 2818.570068 2762.360107 2789.820068 7899550000
2020-04-13 2782.459961 2782.459961 2721.169922 2761.629883 5319530000
2020-04-14 2805.100098 2851.850098 2805.100098 2846.060059 5615730000
2020-04-15 2795.639893 2801.879883 2761.540039 2783.360107 5208000000
2020-04-16 2799.340088 2806.51001 2764.320068 2799.550049 5228810000
2020-04-17 2842.429932 2879.219971 2830.879883 2874.560059 5804810000
2020-04-20 2845.620117 2868.97998 2820.429932 2823.159912 5228630000
2020-04-21 2784.810059 2785.540039 2727.100098 2736.560059 5121010000
2020-04-22 2787.889893 2815.100098 2775.949951 2799.310059 5084920000
2020-04-23 2810.419922 2844.899902 2794.26001 2797.800049 5773530000
2020-04-24 2812.639893 2842.709961 2791.76001 2836.73999 5382810000
2020-04-27 2854.649902 2887.719971 2852.889893 2878.47998 5203930000
2020-04-28 2909.959961 2921.149902 2860.709961 2863.389893 5691160000
2020-04-29 2918.459961 2954.860107 2912.159912 2939.51001 6645640000
2020-04-30 2930.909912 2930.909912 2892.469971 2912.429932 6544980000
2020-05-01 2869.090088 2869.090088 2821.610107 2830.709961 4759810000
2020-05-04 2815.01001 2844.23999 2797.850098 2842.73999 4735930000
2020-05-05 2868.879883 2898.22998 2863.550049 2868.439941 5140290000
2020-05-06 2883.139893 2891.110107 2847.649902 2848.419922 4892570000
2020-05-07 2878.26001 2901.919922 2876.47998 2881.189941 5178790000
2020-05-08 2908.830078 2932.159912 2902.879883 2929.800049 4876030000
2020-05-11 2915.459961 2944.25 2903.439941 2930.189941 4819730000
2020-05-12 2939.5 2945.820068 2869.590088 2870.120117 5119630000
2020-05-13 2865.860107 2874.139893 2793.149902 2820 6151650000
2020-05-14 2794.540039 2852.800049 2766.639893 2852.5 5651130000
2020-05-15 2829.949951 2865.01001 2816.780029 2863.699951 5507700000
2020-05-18 2913.860107 2968.090088 2913.860107 2953.909912 6373670000
2020-05-19 2948.590088 2964.209961 2922.350098 2922.939941 4984330000
2020-05-20 2953.629883 2980.290039 2953.629883 2971.610107 5005380000
2020-05-21 2969.949951 2978.5 2938.570068 2948.51001 4976620000
2020-05-22 2948.050049 2956.76001 2933.590088 2955.449951 3970860000
2020-05-26 3004.080078 3021.719971 2988.169922 2991.77002 5848500000
2020-05-27 3015.649902 3036.25 2969.75 3036.129883 6398000000
2020-05-28 3046.610107 3068.669922 3023.399902 3029.72998 5447020000
2020-05-29 3025.169922 3049.169922 2998.610107 3044.310059 7297550000
2020-06-01 3038.780029 3062.179932 3031.540039 3055.72998 4684170000
2020-06-02 3064.780029 3081.070068 3051.639893 3080.820068 5197150000
2020-06-03 3098.899902 3130.939941 3098.899902 3122.870117 6005560000
2020-06-04 3111.560059 3128.909912 3090.409912 3112.350098 6451290000
2020-06-05 3163.840088 3211.719971 3163.840088 3193.929932 8639300000
2020-06-08 3199.919922 3233.129883 3196 3232.389893 8498300000
2020-06-09 3213.320068 3222.709961 3193.110107 3207.179932 6410930000
2020-06-10 3213.419922 3223.27002 3181.48999 3190.139893 6598870000
2020-06-11 3123.530029 3123.530029 2999.48999 3002.100098 7037320000
2020-06-12 3071.040039 3088.419922 2984.469971 3041.310059 5845330000
2020-06-15 2993.76001 3079.76001 2965.659912 3066.590088 5757480000
2020-06-16 3131 3153.449951 3076.060059 3124.73999 5845810000
2020-06-17 3136.129883 3141.159912 3108.030029 3113.48999 4560450000
2020-06-18 3101.639893 3120 3093.51001 3115.340088 4448690000
2020-06-19 3140.290039 3155.530029 3083.110107 3097.73999 8346650000
2020-06-22 3094.419922 3120.919922 3079.389893 3117.860107 4686690000
2020-06-23 3138.699951 3154.899902 3127.120117 3131.290039 4711850000
2020-06-24 3114.399902 3115.01001 3032.129883 3050.330078 5597680000
2020-06-25 3046.600098 3086.25 3024.01001 3083.76001 4847690000
2020-06-26 3073.199951 3073.72998 3004.629883 3009.050049 8107850000
2020-06-29 3018.590088 3053.889893 2999.73999 3053.23999 4473970000
2020-06-30 3050.199951 3111.51001 3047.830078 3100.290039 4705850000
2020-07-01 3105.919922 3128.439941 3101.169922 3115.860107 4449230000
2020-07-02 3143.639893 3165.810059 3124.52002 3130.01001 4197720000
2020-07-06 3155.290039 3182.590088 3155.290039 3179.719971 4759160000
2020-07-07 3166.439941 3184.149902 3142.929932 3145.320068 4571330000
2020-07-08 3153.070068 3171.800049 3136.530029 3169.939941 4937310000
2020-07-09 3176.169922 3179.780029 3115.699951 3152.050049 4843650000
2020-07-10 3152.469971 3186.820068 3136.219971 3185.040039 4524190000
2020-07-13 3205.080078 3235.320068 3149.429932 3155.219971 4902440000
2020-07-14 3141.110107 3200.949951 3127.659912 3197.52002 4507140000
2020-07-15 3225.97998 3238.280029 3200.76001 3226.560059 4686830000
2020-07-16 3208.360107 3220.389893 3198.590088 3215.570068 3972860000
2020-07-17 3224.209961 3233.52002 3205.649902 3224.72998 4012850000
2020-07-20 3224.290039 3258.610107 3215.159912 3251.840088 3979340000
2020-07-21 3268.52002 3277.290039 3247.77002 3257.300049 4556610000
2020-07-22 3254.860107 3279.320068 3253.100098 3276.02002 4267160000
2020-07-23 3271.639893 3279.98999 3222.659912 3235.659912 4296760000
2020-07-24 3218.580078 3227.26001 3200.050049 3215.629883 3689410000
2020-07-27 3219.840088 3241.429932 3214.25 3239.409912 3970310000
2020-07-28 3234.27002 3243.719971 3216.169922 3218.439941 4053180000
2020-07-29 3227.219971 3264.73999 3227.219971 3258.439941 4478200000
2020-07-30 3231.76001 3250.919922 3204.129883 3246.219971 4264920000
2020-07-31 3270.449951 3272.169922 3220.26001 3271.120117 5007530000
2020-08-03 3288.26001 3302.72998 3284.530029 3294.610107 4438940000
2020-08-04 3289.919922 3306.840088 3286.370117 3306.51001 4414380000
2020-08-05 3317.370117 3330.77002 3317.370117 3327.77002 4535060000
2020-08-06 3323.169922 3351.030029 3318.139893 3349.159912 4278930000
2020-08-07 3340.050049 3352.540039 3328.719971 3351.280029 4110030000
2020-08-10 3356.040039 3363.290039 3335.439941 3360.469971 4327920000
2020-08-11 3370.340088 3381.01001 3326.439941 3333.689941 4880540000
2020-08-12 3355.459961 3387.889893 3355.459961 3380.350098 3772250000
2020-08-13 3372.949951 3387.23999 3363.350098 3373.429932 3438730000
2020-08-14 3368.659912 3378.51001 3361.639893 3372.850098 3200060000
2020-08-17 3380.860107 3387.590088 3379.219971 3381.98999 3460330000
2020-08-18 3387.040039 3395.060059 3370.149902 3389.780029 3669550000
2020-08-19 3392.51001 3399.540039 3369.659912 3374.850098 3679480000
2020-08-20 3360.47998 3390.800049 3354.689941 3385.51001 3431040000
2020-08-21 3386.01001 3399.959961 3379.310059 3397.159912 3505010000
2020-08-24 3418.090088 3432.090088 3413.129883 3431.280029 3743410000
2020-08-25 3435.949951 3444.209961 3425.840088 3443.620117 3627650000
2020-08-26 3449.969971 3481.070068 3444.149902 3478.72998 3780530000
2020-08-27 3485.139893 3501.379883 3468.350098 3484.550049 3955890000
2020-08-28 3494.689941 3509.22998 3484.320068 3508.01001 3868510000
2020-08-31 3509.72998 3514.77002 3493.25 3500.310059 4348280000
2020-09-01 3507.439941 3528.030029 3494.600098 3526.649902 4101490000
2020-09-02 3543.76001 3588.110107 3535.22998 3580.840088 4295640000
2020-09-03 3564.73999 3564.850098 3427.409912 3455.060059 4911830000
2020-09-04 3453.600098 3479.149902 3349.629883 3426.959961 4438720000
2020-09-08 3371.879883 3379.969971 3329.27002 3331.840088 4694330000
2020-09-09 3369.820068 3424.77002 3366.840088 3398.959961 3932520000
2020-09-10 3412.560059 3425.550049 3329.25 3339.189941 4208630000
2020-09-11 3352.699951 3368.949951 3310.469971 3340.969971 3710090000
2020-09-14 3363.560059 3402.929932 3363.560059 3383.540039 3839580000
2020-09-15 3407.72998 3419.47998 3389.25 3401.199951 4057190000
2020-09-16 3411.22998 3428.919922 3384.449951 3385.48999 4718900000
2020-09-17 3346.860107 3375.169922 3328.820068 3357.01001 4379840000
2020-09-18 3357.379883 3362.27002 3292.399902 3319.469971 7076400000
2020-09-21 3285.570068 3285.570068 3229.100098 3281.060059 4836070000
2020-09-22 3295.75 3320.310059 3270.949951 3315.570068 3974510000
2020-09-23 3320.110107 3323.350098 3232.570068 3236.919922 4378650000
2020-09-24 3226.139893 3278.699951 3209.449951 3246.590088 4601920000
2020-09-25 3236.659912 3306.879883 3228.439941 3298.459961 3803330000
2020-09-28 3333.899902 3360.73999 3332.909912 3351.600098 3950910000
2020-09-29 3350.919922 3357.919922 3327.540039 3335.469971 3661590000
2020-09-30 3341.209961 3393.560059 3340.469971 3363 4738640000
2020-10-01 3385.870117 3397.179932 3361.389893 3380.800049 4076340000
2020-10-02 3338.939941 3369.100098 3323.689941 3348.419922 3975180000
2020-10-05 3367.27002 3409.570068 3367.27002 3408.600098 3692720000
2020-10-06 3408.73999 3431.560059 3354.540039 3360.969971 4453390000
2020-10-07 3384.560059 3426.26001 3384.560059 3419.439941 3814750000
2020-10-08 3434.280029 3447.280029 3428.149902 3446.830078 3867640000
2020-10-09 3459.669922 3482.340088 3458.070068 3477.139893 3944090000
2020-10-12 3500.02002 3549.850098 3499.610107 3534.219971 3435760000
2020-10-13 3534.01001 3534.01001 3500.860107 3511.929932 3611100000
2020-10-14 3515.469971 3527.939941 3480.550049 3488.669922 3858510000
2020-10-15 3453.719971 3489.080078 3440.889893 3483.340088 3724550000
2020-10-16 3493.5 3515.76001 3480.449951 3483.810059 4688030000
2020-10-19 3493.659912 3502.419922 3419.929932 3426.919922 4091080000
2020-10-20 3439.379883 3476.929932 3435.649902 3443.120117 3917850000
2020-10-21 3439.909912 3464.860107 3433.060059 3435.560059 4103960000
2020-10-22 3438.5 3460.530029 3415.340088 3453.48999 4172060000
2020-10-23 3464.899902 3466.459961 3440.449951 3465.389893 3651600000
2020-10-26 3441.419922 3441.419922 3364.860107 3400.969971 4002210000
2020-10-27 3403.149902 3409.51001 3388.709961 3390.679932 3962400000
2020-10-28 3342.47998 3342.47998 3268.889893 3271.030029 5139970000
2020-10-29 3277.169922 3341.050049 3259.820068 3310.110107 4911860000
2020-10-30 3293.590088 3304.929932 3233.939941 3269.959961 4843930000
2020-11-02 3296.199951 3330.139893 3279.73999 3310.23999 4314690000
2020-11-03 3336.25 3389.48999 3336.25 3369.159912 4241380000
2020-11-04 3406.459961 3486.25 3405.169922 3443.439941 4790400000
2020-11-05 3485.73999 3529.050049 3485.73999 3510.449951 4858150000
2020-11-06 3508.340088 3521.580078 3484.340088 3509.439941 4842460000
2020-11-09 3583.040039 3645.98999 3547.47998 3550.5 8570510000
2020-11-10 3543.26001 3557.219971 3511.909912 3545.530029 6037470000
2020-11-11 3563.219971 3581.159912 3557 3572.659912 4635560000
2020-11-12 3562.669922 3569.02002 3518.580078 3537.01001 4909660000
2020-11-13 3552.570068 3593.659912 3552.570068 3585.149902 4719580000
2020-11-16 3600.159912 3628.51001 3600.159912 3626.909912 5295510000
2020-11-17 3610.310059 3623.110107 3588.679932 3609.530029 4812180000
2020-11-18 3612.090088 3619.090088 3567.330078 3567.790039 5284810000
2020-11-19 3559.409912 3585.219971 3543.840088 3581.870117 4363370000
2020-11-20 3579.310059 3581.22998 3556.850098 3557.540039 4236370000
2020-11-23 3566.820068 3589.810059 3552.77002 3577.590088 5057550000
2020-11-24 3594.52002 3642.310059 3594.52002 3635.409912 6280290000
2020-11-25 3635.5 3635.5 3617.76001 3629.649902 4910440000
2020-11-27 3638.550049 3644.310059 3629.330078 3638.350098 2778390000
2020-11-30 3634.179932 3634.179932 3594.389893 3621.629883 6308410000
2020-12-01 3645.870117 3678.449951 3645.870117 3662.449951 5418480000
2020-12-02 3653.780029 3670.959961 3644.840088 3669.01001 5041250000
2020-12-03 3668.280029 3682.72998 3657.169922 3666.719971 5065340000
2020-12-04 3670.939941 3699.199951 3670.939941 3699.120117 5099620000
2020-12-07 3694.72998 3697.409912 3678.879883 3691.959961 4804500000
2020-12-08 3683.050049 3708.449951 3678.830078 3702.25 4584390000
2020-12-09 3705.97998 3712.389893 3660.540039 3672.820068 5232800000
2020-12-10 3659.129883 3678.48999 3645.179932 3668.100098 4658480000
2020-12-11 3656.080078 3665.909912 3633.399902 3663.459961 4375470000
2020-12-14 3675.27002 3697.610107 3645.840088 3647.48999 4623850000
2020-12-15 3666.409912 3695.290039 3659.620117 3694.620117 4387080000
2020-12-16 3696.25 3711.27002 3688.570068 3701.169922 4067040000
2020-12-17 3713.649902 3725.120117 3710.870117 3722.47998 4192810000
2020-12-18 3722.389893 3726.699951 3685.840088 3709.409912 7097100000
2020-12-21 3684.280029 3702.899902 3636.47998 3694.919922 4748580000
2020-12-22 3698.080078 3698.26001 3676.159912 3687.26001 4053310000
2020-12-23 3693.419922 3711.23999 3689.280029 3690.01001 3779160000
2020-12-24 3694.030029 3703.820068 3689.320068 3703.060059 1883780000
2020-12-28 3723.030029 3740.51001 3723.030029 3735.360107 3535460000
2020-12-29 3750.01001 3756.120117 3723.310059 3727.040039 3393290000
2020-12-30 3736.189941 3744.629883 3730.209961 3732.040039 3154850000
2020-12-31 3733.27002 3760.199951 3726.879883 3756.070068 3179040000
2021-01-04 3764.610107 3769.98999 3662.709961 3700.649902 5015000000
##
    `);
    data = data.trim().split("\n").slice(2, -1).map(function (elem, ii) {
        elem = elem.split(" ");
        return {
            date: elem[0],
            ii,
            priceClose: Number(elem[4]),
            priceHigh: Number(elem[2]),
            priceLow: Number(elem[3]),
            priceOpen: Number(elem[1]),
            symbol: "spx",
            volume: Number(elem[5])
        };
    });
    return data;
}());
noop(debugInline);

jstestDescribe((
    "test_apidoc"
), function test_apidoc() {
    jstestIt((
        "test apidoc handling-behavior"
    ), function () {
        jslint.jslint_apidoc({
            example_list: [
                "README.md",
                "test.mjs",
                "sqlmath.mjs"
            ],
            github_repo: "https://github.com/sqlmath/sqlmath",
            module_list: [
                {
                    pathname: "./sqlmath.mjs"
                }
            ],
            package_name: "sqlmath",
            pathname: ".artifact/apidoc.html",
            version
        });
    });
});

jstestDescribe((
    "test_assertXxx"
), function test_assertXxx() {
    jstestIt((
        "test assertXxx handling-behavior"
    ), function () {
        assertErrorThrownAsync(function () {
            assertNumericalEqual(0, 0);
        }, "value cannot be 0 or falsy");
        assertErrorThrownAsync(function () {
            assertNumericalEqual(1, 2);
        }, "1 != 2");
        assertErrorThrownAsync(function () {
            assertNumericalEqual(1, 2, "aa");
        }, "aa");
        assertNumericalEqual(1, 1);
    });
});

jstestDescribe((
    "test_ccallAsync"
), function test_ccallAsync() {
    jstestIt((
        "test cCallAsync handling-behavior"
    ), async function () {
        // test datatype handling-behavior
        await Promise.all([
            ["", ""],
            ["\u0000", ""],
            ["aa", "aa"],
            [-0, 0],
            [-0.5, undefined],
            [-0n, -0],
            [-0x8000000000000000n, -0x8000000000000000n],
            [-0x8000000000000001n, 0x7fffffffffffffffn],
            [-1 / 0, undefined],
            [-1e-999, 0],
            [-1e999, undefined],
            [-1n, -1],
            [-2, -2],
            [-2n, -2],
            [-Infinity, undefined],
            [-NaN, undefined],
            [0, 0],
            [0.5, undefined],
            [0n, 0],
            [0x7fffffffffffffffn, 0x7fffffffffffffffn],
            [0x8000000000000000n, -0x8000000000000000n],
            [1 / 0, undefined],
            [1e-999, 0],
            [1e999, undefined],
            [1n, 1],
            [2, 2],
            [2n, 2],
            [Infinity, undefined],
            [NaN, undefined],
            [Symbol(), 0],
            [false, 0],
            [noop, 0],
            [null, 0],
            [true, 1],
            [undefined, 0],
            [{}, 0]
        ].map(async function ([
            valInput, valExpected
        ]) {
            let baton;
            let valActual;
            if (valExpected === undefined) {
                assertErrorThrownAsync(function () {
                    return dbNoopAsync(undefined, valInput, undefined);
                });
                return;
            }
            baton = await dbNoopAsync(undefined, valInput, undefined);
            baton = baton[0];
            valActual = (
                typeof valInput === "string"
                ? jsbatonValueString({
                    argi: 1,
                    baton
                })
                : String(baton.getBigInt64(4 + 4 + 8, true))
            );
            valExpected = String(valExpected);
            if (typeof valInput === "bigint") {
                valInput = String(valInput);
            }
            assertJsonEqual(valActual, valExpected, {
                valActual,
                valExpected,
                valInput
            });
        }));
    });
});

jstestDescribe((
    "test_childProcessSpawn2"
), function test_childProcessSpawn2() {
    jstestIt((
        "test childProcessSpawn2 handling-behavior"
    ), async function () {
        await Promise.all([
            childProcessSpawn2(
                "aa",
                [],
                {modeCapture: "utf8", modeDebug: true, stdio: []}
            )
        ]);
    });
});

jstestDescribe((
    "test_ciBuildExtXxx"
), function test_ciBuildExtXxx() {
    jstestIt((
        "test ciBuildExt handling-behavior"
    ), async function () {
        await Promise.all([
            ciBuildExt({modeSkip: true, process: {}}),
            ciBuildExt({process: {arch: "arm", env: {}, platform: "win32"}}),
            ciBuildExt({process: {arch: "arm64", env: {}, platform: "win32"}}),
            ciBuildExt({process: {arch: "ia32", env: {}, platform: "win32"}}),
            ciBuildExt({process: {env: {}, platform: "darwin"}}),
            ciBuildExt({process: {env: {}, platform: "win32"}}),
            ciBuildExt({process: {}})
        ]);
    });
});

jstestDescribe((
    "test_dbBind"
), function test_dbBind() {
    jstestIt((
        "test db-bind-value handling-behavior"
    ), async function test_dbBindValue() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        async function test_dbExecAndReturnLastBlobAsync(val) {
            return await dbExecAndReturnLastBlobAsync({
                bindList: [
                    val
                ],
                db,
                sql: "SELECT 1, 2, 3; SELECT 1, 2, ?"
            });
        }
        // test bigint-error handling-behavior
        noop([
            -(2n ** 63n + 1n),
            2n ** 63n
        ]).forEach(function (val) {
            assertErrorThrownAsync(
                test_dbExecAndReturnLastBlobAsync.bind(undefined, val)
            );
        });
        // test datatype handling-behavior
        await Promise.all([
            // 1. bigint
            [-0n, -0],
            [-0x8000000000000000n, "-9223372036854775808"],
            [-1n, -1],
            [-2n, -2],
            [0n, 0],
            [0x7fffffffffffffffn, "9223372036854775807"],
            [1n, 1],
            [2n, 2],
            // 2. boolean
            [false, 0],
            [true, 1],
            // 3. function
            [noop, null],
            // 4. number
            [-0, 0],
            [-0.5, -0.5],
            [-1 / 0, null],
            [-1e-999, 0],
            [-1e999, null],
            [-2, -2],
            [-Infinity, null],
            [-NaN, 0],
            [0, 0],
            [0.5, 0.5],
            [1 / 0, null],
            [1e-999, 0],
            [1e999, null],
            [2, 2],
            [Infinity, null],
            [NaN, 0],
            // 5. object
            [[], "[]"],
            [new Date(0), "1970-01-01T00:00:00.000Z"],
            [new RegExp(), "{}"],
            [new SharedArrayBuffer(0), null],
            [new TextEncoder().encode(""), null],
            [new TextEncoder().encode("\u0000"), null],
            [new TextEncoder().encode("\u0000\u{1f600}\u0000"), null],
            [new Uint8Array(0), null],
            [null, null],
            [{}, "{}"],
            // 6. string
            ["", ""],
            ["0", "0"],
            ["1", "1"],
            ["2", "2"],
            ["\u0000", "\u0000"],
            ["\u0000\u{1f600}\u0000", "\u0000\u{1f600}\u0000"],
            ["a".repeat(9999), "a".repeat(9999)],
            // 7. symbol
            [Symbol(), null],
            // 8. undefined
            [undefined, null]
        ].map(async function ([
            valInput, valExpected
        ], ii) {
            // test dbExecAndReturnLastBlobAsync's bind handling-behavior
            await Promise.all([
                valInput
            ].map(async function (valInput) {
                let bufActual = new TextDecoder().decode(
                    await test_dbExecAndReturnLastBlobAsync(valInput)
                );
                let bufExpected = String(valExpected);
                switch (typeof(valInput)) {
                case "bigint":
                    valInput = Number(valInput);
                    break;
                case "function":
                case "symbol":
                case "undefined":
                    bufExpected = "";
                    break;
                case "number":
                    switch (valInput) {
                    case -2:
                        bufExpected = "-2.0";
                        break;
                    case -Infinity:
                        bufExpected = "-Inf";
                        break;
                    case 2:
                        bufExpected = "2.0";
                        break;
                    case Infinity:
                        bufExpected = "Inf";
                        break;
                    }
                    break;
                case "object":
                    if (ArrayBuffer.isView(valInput)) {
                        bufExpected = new TextDecoder().decode(valInput);
                        break;
                    }
                    if (
                        valInput === null
                        || valInput.constructor === SharedArrayBuffer
                    ) {
                        bufExpected = "";
                    }
                    break;
                }
                assertJsonEqual(bufActual, bufExpected, {
                    bufActual,
                    bufExpected,
                    ii,
                    valExpected,
                    valInput
                });
            }));
            // test dbExecAsync's responseType handling-behavior
            await Promise.all([
                "arraybuffer",
                "list",
                undefined
            ].map(async function (responseType) {
                let bufActual = await dbExecAsync({
                    bindList: [
                        valInput
                    ],
                    db,
                    responseType,
                    sql: "SELECT ? AS val"
                });
                switch (responseType) {
                case "arraybuffer":
                    bufActual = JSON.parse(
                        new TextDecoder().decode(bufActual)
                    )[0][1][0];
                    break;
                case "list":
                    bufActual = bufActual[0][1][0];
                    break;
                default:
                    bufActual = bufActual[0][0].val;
                }
                assertJsonEqual(bufActual, valExpected, {
                    bufActual,
                    ii,
                    responseType,
                    valExpected,
                    valInput
                });
            }));
            // test dbExecAsync's bind handling-behavior
            await Promise.all([
                [
                    [
                        valExpected, valExpected, 0
                    ],
                    (
                        "SELECT 0;"
                        + " SELECT ? AS c1, ? AS c2, ? AS c3, ? AS c4"
                        + " UNION ALL SELECT ?1, ?2, ?3, ?4"
                        + " UNION ALL SELECT ?1, ?2, ?3, ?4"
                    )
                ],
                [
                    {
                        k1: valExpected,
                        k2: valExpected,
                        k3: 0
                    },
                    (
                        "SELECT 0;"
                        + " SELECT $k1 AS c1, $k2 AS c2, $k3 AS c3, $k4 AS c4"
                        + " UNION ALL SELECT :k1, :k2, :k3, :k4"
                        + " UNION ALL SELECT @k1, @k2, @k3, @k4"
                    )
                ]
            ].map(async function ([
                bindList, sql
            ]) {
                let valActual = await dbExecAsync({
                    bindList,
                    db,
                    responseType: "list",
                    sql
                });
                assertJsonEqual(
                    [
                        [
                            ["0"],
                            [0]
                        ],
                        [
                            ["c1", "c2", "c3", "c4"],
                            [valExpected, valExpected, 0, undefined],
                            [valExpected, valExpected, 0, undefined],
                            [valExpected, valExpected, 0, undefined]
                        ]
                    ],
                    valActual,
                    {
                        ii,
                        valActual,
                        valExpected,
                        valInput
                    }
                );
            }));
        }));
    });
});

jstestDescribe((
    "test_dbXxxAsync"
), function test_dbXxxAsync() {
    jstestIt((
        "test dbCloseAsync handling-behavior"
    ), async function test_dbCloseAsync() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbCloseAsync({});
        }, "invalid or closed db");
        // test close handling-behavior
        dbCloseAsync({
            db
        });
    });
    jstestIt((
        "test dbExecAsync handling-behavior"
    ), async function test_dbExecAsync() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbExecAsync({
                db,
                sql: undefined
            });
        }, "near \"undefined\": syntax error");
        // test race-condition handling-behavior
        Array.from(new Array(4)).forEach(async function () {
            let result;
            try {
                result = JSON.stringify(
                    await dbExecAsync({
                        bindList: [
                            new TextEncoder().encode("foob"),
                            new TextEncoder().encode("fooba"),
                            new TextEncoder().encode("foobar")
                        ],
                        db,
                        responseType: "list",
                        sql: (`
CREATE TABLE testDbExecAsync1 AS
SELECT 101 AS c101, 102 AS c102
--
UNION ALL
VALUES
    (201, 202),
    (301, NULL);
CREATE TABLE testDbExecAsync2 AS
SELECT 401 AS c401, 402 AS c402, 403 AS c403
--
UNION ALL
VALUES
    (501, 502.0123, 5030123456789),
    (601, '602', '603_\"\x01\x08\x09\x0a\x0b\x0c\x0d\x0e'),
    (?1, ?2, ?3),
    (CAST(?1 AS TEXT), CAST(?2 AS TEXT), CAST(?3 AS TEXT)),
    (
        CAST(uncompress(compress(?1)) AS TEXT),
        CAST(uncompress(compress(?2)) AS TEXT),
        CAST(uncompress(compress(?3)) AS TEXT)
    );
SELECT * FROM testDbExecAsync1;
SELECT * FROM testDbExecAsync2;
                        `)
                    })
                );
                assertJsonEqual(result, JSON.stringify([
                    [
                        ["c101", "c102"],
                        [101, 102],
                        [201, 202],
                        [301, null]
                    ],
                    [
                        ["c401", "c402", "c403"],
                        [401, 402, 403],
                        [501, 502.0123, 5030123456789],
                        [601, "602", "603_\"\u0001\b\t\n\u000b\f\r\u000e"],
                        [null, null, null],
                        ["foob", "fooba", "foobar"],
                        ["foob", "fooba", "foobar"]
                    ]
                ]));
            } catch (err) {
                assertOrThrow(
                    err.message.indexOf(
                        "table testDbExecAsync1 already exists"
                    ) >= 0,
                    err
                );
            }
        });
        // test close-while-busy handling-behavior
        assertErrorThrownAsync(function () {
            return dbCloseAsync({
                db
            });
        }, (
            /db cannot close with \d+? actions pending/
        ));
        // test retry handling-behavior
        assertErrorThrownAsync(function () {
            return dbExecAsync({
                modeRetry: 1
            });
        }, "invalid or closed db");
    });
    jstestIt((
        "test dbFileXxx handling-behavior"
    ), async function test_dbFileXxx() {
        let data;
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test null-case handling-behavior
        dbFileExportAsync({
            modeNoop: true
        });
        assertErrorThrownAsync(function () {
            return dbFileImportAsync({
                db
            });
        }, "invalid filename undefined");
        assertErrorThrownAsync(function () {
            return dbFileExportAsync({
                db
            });
        }, "invalid filename undefined");
        await dbExecAsync({
            db,
            sql: "CREATE TABLE t01 AS SELECT 1 AS c01"
        });
        await dbFileExportAsync({
            db,
            filename: ".testDbFileXxx.sqlite"
        });
        db = await dbOpenAsync({
            filename: ":memory:"
        });
        await dbFileImportAsync({
            db,
            filename: ".testDbFileXxx.sqlite"
        });
        data = await dbExecAsync({
            db,
            sql: "SELECT * FROM t01"
        });
        assertJsonEqual(data, [
            [
                {
                    c01: 1
                }
            ]
        ]);
    });
    jstestIt((
        "test dbOpenAsync handling-behavior"
    ), async function test_dbOpenAsync() {
        // test auto-finalization handling-behavior
        await new Promise(function (resolve) {
            dbOpenAsync({
                afterFinalization: resolve,
                filename: ":memory:"
            });
        });
        // test null-case handling-behavior
        assertErrorThrownAsync(function () {
            return dbOpenAsync({});
        }, "invalid filename undefined");
    });
});

jstestDescribe((
    "test_fsXxx"
), function test_fsXxx() {
    jstestIt((
        "test fsXxx handling-behavior"
    ), async function () {
        await Promise.all([
            fsCopyFileUnlessTest("", ""),
            fsExistsUnlessTest(""),
            fsReadFileUnlessTest("", ""),
            fsWriteFileUnlessTest("", ""),
            //
            fsCopyFileUnlessTest(
                "package.json",
                ".tmp/test_fsCopyFileUnlessTest_force",
                "force"
            ),
            fsExistsUnlessTest("", "force"),
            fsExistsUnlessTest("package.json", "force"),
            fsReadFileUnlessTest("package.json", "force"),
            fsWriteFileUnlessTest(
                ".tmp/test_fsWriteFileUnlessTest_force",
                "",
                "force"
            )
        ]);
    });
});

jstestDescribe((
    "test_misc"
), function test_misc() {
    jstestIt((
        "test misc handling-behavior"
    ), async function () {
        // test assertErrorThrownAsync error handling-behavior
        await assertErrorThrownAsync(function () {
            return assertErrorThrownAsync(noop);
        });
        // test assertJsonEqual error handling-behavior
        await assertErrorThrownAsync(function () {
            assertJsonEqual(1, 2);
        });
        await assertErrorThrownAsync(function () {
            assertJsonEqual(1, 2, "undefined");
        });
        await assertErrorThrownAsync(function () {
            assertJsonEqual(1, 2, {});
        });
        // test assertOrThrow error handling-behavior
        await assertErrorThrownAsync(function () {
            assertOrThrow(undefined, "undefined");
        });
        await assertErrorThrownAsync(function () {
            assertOrThrow(undefined, new Error());
        });
    });
});

jstestDescribe((
    "test_sqlite"
), function test_sqlite() {
    jstestIt((
        "test sqlite-error handling-behavior"
    ), async function test_sqliteError() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        assertJsonEqual(
            "not an error",
            noop(
                await dbExecAsync({
                    db,
                    sql: `SELECT throwerror(0) AS val`
                })
            )[0][0].val
        );
        await Promise.all([
            [1, "SQL logic error"],
            [2, "unknown error"],
            [3, "access permission denied"],
            [4, "query aborted"],
            [5, "database is locked"],
            [6, "database table is locked"],
            [7, "out of memory"],
            [8, "attempt to write a readonly database"],
            [9, "interrupted"],
            [10, "disk I/O error"],
            [11, "database disk image is malformed"],
            [12, "unknown operation"],
            [13, "database or disk is full"],
            [14, "unable to open database file"],
            [15, "locking protocol"],
            [16, "unknown error"],
            [17, "database schema has changed"],
            [18, "string or blob too big"],
            [19, "constraint failed"],
            [20, "datatype mismatch"],
            [21, "bad parameter or other API misuse"],
            [22, "unknown error"],
            [23, "authorization denied"],
            [24, "unknown error"],
            [25, "column index out of range"],
            [26, "file is not a database"],
            [27, "notification message"],
            [28, "warning message"],
            [100, "unknown error"],
            [101, "unknown error"]
        ].map(async function ([
            errno, errmsg
        ]) {
            await assertErrorThrownAsync(function () {
                return dbExecAsync({
                    db,
                    sql: `SELECT throwerror(${errno})`
                });
            }, errmsg);
        }));
    });
    jstestIt((
        "test sqlite-extension-doublearray_xxx handling-behavior"
    ), async function test_sqlite_extension_doublearray_xxx() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        await Promise.all([
            [" [ , 1 ] ", "error"],
            [" [ , ] ", "error"],
            [" [ 1 , ] ", "error"],
            [" [ ] ", "[]"],
            [" [ null ] ", "[0.0]"],
            ["", "error"],
            ["1,2]", "error"],
            ["[,1]", "error"],
            ["[,]", "error"],
            ["[0]", "[0.0]"],
            ["[1,2", "error"],
            ["[1,2,a]", "[1.0,2.0,0.0]"],
            ["[1,]", "error"],
            ["[1,a,3]", "[1.0,0.0,3.0]"],
            ["[1]", "[1.0]"],
            ["[]", "[]"],
            ["[a,2,3]", "[0.0,2.0,3.0]"],
            [0, "error"],
            [1, "error"],
            [`[${"1".repeat(100)}]`, `[1.11111111111111e+99]`],
            [null, "error"],
            [undefined, "error"],
            [{}, "error"]
        ].map(async function ([
            valInput, valExpected
        ], ii) {
            let valActual;
            try {
                valActual = noop(
                    await dbExecAsync({
                        bindList: {
                            valInput
                        },
                        db,
                        sql: (`
SELECT doublearray_jsonto(doublearray_jsonfrom($valInput)) AS result;
                        `)
                    })
                )[0][0].result;
            } catch (ignore) {
                assertOrThrow(valExpected === "error", JSON.stringify({
                    ii,
                    valActual,
                    valExpected,
                    valInput
                }, undefined, 4));
                return;
            }
            assertJsonEqual(valActual, valExpected, {
                ii,
                valActual,
                valExpected,
                valInput
            });
        }));
    });
    jstestIt((
        "test sqlite-extension-fill_forward handling-behavior"
    ), async function test_sqlite_extension_fill_forward() {
        let db = await dbOpenAsync({filename: ":memory:"});
        let result = await dbExecAsync({
            db,
            sql: (`
SELECT
    fill_forward(val) OVER (ORDER BY id ASC) AS val
    FROM (
        SELECT 10 AS id, NULL AS val
        UNION ALL SELECT 9 AS id, 9 AS val
        UNION ALL SELECT 8 AS id, 8 AS val
        UNION ALL SELECT 7 AS id, NULL AS val
        UNION ALL SELECT 6 AS id, 6 AS val
        UNION ALL SELECT 5 AS id, 5 AS val
        UNION ALL SELECT 4 AS id, NULL AS val
        UNION ALL SELECT 3 AS id, 3 AS val
        UNION ALL SELECT 2 AS id, NULL AS val
        UNION ALL SELECT 1 AS id, NULL AS val
    );
            `)
        });
        result = result[0].map(function ({val}) {
            return val;
        });
        assertJsonEqual(result, [null, null, 3, 3, 5, 6, 6, 8, 9, 9]);
    });
    jstestIt((
        "test sqlite-extension-math handling-behavior"
    ), async function test_sqlite_extension_math() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        // test sqlmath-defined-func handling-behavior
        Object.entries({
            "''": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "",
                "copyblob": ""
            },
            "'-0.5'": {
                "castrealornull": -0.5,
                "castrealorzero": -0.5,
                "casttextorempty": "-0.5",
                "copyblob": "-0.5"
            },
            "'-1'": {
                "castrealornull": -1,
                "castrealorzero": -1,
                "casttextorempty": "-1",
                "copyblob": "-1",
                "cot": -0.642092615934331,
                "coth": -1.31303528549933,
                "sign": -1
            },
            "'0'": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "0",
                "copyblob": "0",
                "cot": null,
                "coth": null,
                "sign": 0
            },
            "'0.5'": {
                "castrealornull": 0.5,
                "castrealorzero": 0.5,
                "casttextorempty": "0.5",
                "copyblob": "0.5"
            },
            "'1'": {
                "castrealornull": 1,
                "castrealorzero": 1,
                "casttextorempty": "1",
                "copyblob": "1",
                "cot": 0.642092615934331,
                "coth": 1.31303528549933,
                "sign": 1
            },
            "'aa'": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "aa",
                "copyblob": "aa"
            },
            "'hello'": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "hello",
                "copyblob": "hello"
            },
            "-0.5": {
                "castrealornull": -0.5,
                "castrealorzero": -0.5,
                "casttextorempty": "-0.5",
                "copyblob": -0.5
            },
            "-0x7fffffffffffffff": {
                "sign": -1
            },
            "-1": {
                "castrealornull": -1,
                "castrealorzero": -1,
                "casttextorempty": "-1",
                "copyblob": -1,
                "cot": -0.642092615934331,
                "coth": -1.31303528549933,
                "sign": -1
            },
            "-1e999": {
                "sign": -1
            },
            "0": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "0",
                "copyblob": 0,
                "cot": null,
                "coth": null,
                "sign": 0
            },
            "0.5": {
                "castrealornull": 0.5,
                "castrealorzero": 0.5,
                "casttextorempty": "0.5",
                "copyblob": 0.5
            },
            "0.5, 0.5": {
                "roundorzero": 1
            },
            "0.5, 1": {
                "roundorzero": 0.5
            },
            "0.5, NULL": {
                "roundorzero": 1
            },
            "0x7fffffffffffffff": {
                "sign": 1
            },
            "0x8000000000000000": {
                "sign": -1
            },
            "0xffffffffffffffff": {
                "sign": -1
            },
            "1": {
                "castrealornull": 1,
                "castrealorzero": 1,
                "casttextorempty": "1",
                "copyblob": 1,
                "cot": 0.642092615934331,
                "coth": 1.31303528549933,
                "sign": 1
            },
            "1e999": {
                "castrealornull": null,
                "castrealorzero": 0,
                "sign": 1
            },
            "null": {
                "castrealornull": null,
                "castrealorzero": 0,
                "casttextorempty": "",
                "copyblob": null,
                "cot": null,
                "coth": null,
                "sign": null
            },
            "null, 0": {
                "roundorzero": 0
            },
            "null, 0.5": {
                "roundorzero": 0
            },
            "null, null": {
                "roundorzero": 0
            },
            "zeroblob(0)": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "",
                "copyblob": null
            },
            "zeroblob(1)": {
                "castrealornull": 0,
                "castrealorzero": 0,
                "casttextorempty": "",
                "copyblob": null
            }
        }).forEach(function ([
            arg, funcDict
        ]) {
            Object.entries(funcDict).forEach(async function ([
                func, valExpected
            ]) {
                let sql = `SELECT ${func}(${arg}) AS val`;
                let valActual = noop(
                    await dbExecAsync({
                        db,
                        sql
                    })
                )[0][0].val;
                assertJsonEqual(valActual, valExpected, {
                    sql,
                    valActual,
                    valExpected
                });
            });
        });
    });
    jstestIt((
        "test sqlite-extension-quantile handling-behavior"
    ), async function test_sqlite_extension_quantile() {
        let db = await dbOpenAsync({
            filename: ":memory:"
        });
        await (async function () {
            let valActual = noop(
                await dbExecAsync({
                    db,
                    sql: (`
-- test null-case handling-behavior
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT
        1 AS id,
        median(val) AS mdn,
        quantile(val, 0.5) AS qnt,
        stdev(val) AS std
    FROM __tmp1;
                    `)
                })
            )[0];
            assertJsonEqual(
                valActual,
                [{id: 1, mdn: null, qnt: null, std: null}]
            );
        }());
        await Promise.all([
            [
                [[], -99, undefined],
                [[], 0, undefined],
                [[], 0.5, undefined],
                [[], 1, undefined],
                [[], 99, undefined],
                [[undefined, undefined, 1, 1, 2, 3, 4], 0.5, 2],
                [[undefined, undefined, 1, 2, 3, 4], 0.5, 2.5],
                [[undefined], 0.5, undefined]
            ],
            [
                [[], -99, 1],
                [[], 0, 1],
                [[], 0.125, 1.875],
                [[], 0.250, 2.75],
                [[], 0.375, 3.625],
                [[], 0.500, 4.5],
                [[], 0.625, 5.375],
                [[], 0.750, 6.25],
                [[], 0.875, 7.125],
                [[], 1, 8],
                [[], 99, 8],
                [[0.1], 0, 0.1],
                [[1.1], 0.125, 1.1],
                [[2.1], 0.250, 2.1],
                [[3.1], 0.375, 3.1],
                [[4.1], 0.500, 4.1],
                [[4.1], 0.625, 5],
                [[5.1], 0.750, 6],
                [[6.1], 0.875, 7],
                [[7.1], 1, 8],
                [[0], 0, 0],
                [[1], 0.125, 1],
                [[2], 0.250, 2],
                [[3], 0.375, 3],
                [[4], 0.500, 4],
                [[5], 0.625, 5],
                [[6], 0.750, 6],
                [[7], 0.875, 7],
                [[8], 1, 8],
                [[], 0, 1]
            ].map(function ([
                data, kk, valExpected
            ]) {
                return [
                    data.concat([
                        undefined, -Infinity, Infinity, NaN,
                        "8", 7, 6, "5", "4", 3, 2, "1",
                        undefined
                    ]),
                    kk,
                    valExpected
                ];
            })
        ].flat().map(async function ([
            data, kk, valExpected
        ]) {
            let avg = 0;
            let data2;
            let valExpectedMdn;
            let valExpectedStd = 0;
            data2 = data.map(function (elem) {
                return Number(elem);
            }).filter(function (elem) {
                return Number.isFinite(elem);
            }).sort();
            valExpectedMdn = (
                data2.length % 2 === 0
                ? 0.5 * (
                    data2[0.5 * data2.length - 1] + data2[0.5 * data2.length]
                )
                : data2[0.5 * (data2.length - 1)]
            );
            data2.forEach(function (elem) {
                avg += elem;
            });
            avg *= 1 / data2.length;
            data2.forEach(function (elem) {
                valExpectedStd += (elem - avg) ** 2;
            });
            valExpectedStd = (
                data2.length <= 0
                ? null
                // : data2.length === 1
                // ? 0
                : Number(Math.sqrt(
                    valExpectedStd / (data2.length - 1)
                ).toFixed(8))
            );
            await Promise.all([
                data,
                Array.from(data).reverse()
            ].map(async function (data) {
                let valActual;
                valActual = noop(
                    await dbExecAsync({
                        bindList: {
                            tmp1: JSON.stringify(data)
                        },
                        db,
                        sql: (`
SELECT
        median(value) AS mdn,
        quantile(value, ${kk}) AS qnt,
        ROUND(stdev(value), 8) AS std
    FROM JSON_EACH($tmp1);
-- test null-case handling-behavior
SELECT quantile(value, ${kk}) AS qnt FROM JSON_EACH($tmp1) WHERE 0;
                        `)
                    })
                )[0][0];
                assertJsonEqual(
                    valActual,
                    {
                        mdn: valExpectedMdn,
                        qnt: valExpected ?? null,
                        std: valExpectedStd
                    },
                    {
                        data,
                        kk,
                        valActual,
                        valExpected,
                        valExpectedMdn,
                        valExpectedStd
                    }
                );
            }));
        }));
    });
    jstestIt((
        "test sqlite-extension-win_emax handling-behavior"
    ), async function test_sqlite_extension_win_emax() {
        let db = await dbOpenAsync({filename: ":memory:"});
        let valIn;
        async function test_win_emax_aggregate({
            aa,
            bb,
            valExpected,
            valExpected2
        }) {
            let alpha = 2 * 1.0 / (4 + 1);
            let sqlBetween = "";
            let valActual;
            if (aa !== undefined) {
                sqlBetween = (
                    `ROWS BETWEEN ${aa - 1} PRECEDING AND ${bb} FOLLOWING`
                );
            }
            // test win_ema1-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn: JSON.stringify(valIn)
                },
                db,
                sql: (`
SELECT
        win_ema1(value->>1, ${alpha}) OVER (
            ORDER BY value->>0 ASC
            ${sqlBetween}
        ) AS val
    FROM JSON_EAcH($valIn);
                `)
            });
            valActual = valActual[0].map(function ({val}) {
                return Number(val.toFixed(4));
            });
            assertJsonEqual(valActual, valExpected);
            // test win_ema2-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn: JSON.stringify(valIn)
                },
                db,
                sql: (`
SELECT
        id,
        win_ema2(
            value->>1,
            IIF(id = 1, -1, value->>1),
            ${alpha}
        ) OVER (
            ORDER BY value->>0 ASC
            ${sqlBetween}
        ) AS val
    FROM JSON_EAcH($valIn);
                `)
            });
            valActual = valActual[0].map(function ({val}, ii, list) {
                val = JSON.parse(val).map(function (elem) {
                    return Number(elem.toFixed(4));
                });
                if (ii + (bb || 0) + 1 >= list.length) {
                    assertJsonEqual(val[1], valExpected2, valActual);
                } else {
                    assertJsonEqual(val[1], val[0], valActual);
                }
                return val[0];
            });
            assertJsonEqual(valActual, valExpected);
        }
        valIn = [
            [11, NaN],
            [10, "10"],
            [9, 9],
            [8, "8"],
            [7, 7],
            [6, 6],
            [5, Infinity],
            [4, "4"],
            [3, 3],
            [2, 2],
            [1, "1"],
            [0, undefined]
        ];
        await Promise.all([
            (async function () {
                let valActual;
                // test win_ema2-error handling-behavior
                await assertErrorThrownAsync(function () {
                    return dbExecAsync({
                        db,
                        sql: (`
SELECT win_ema2(1) FROM (SELECT 1);
                        `)
                    });
                }, "wrong number of arguments");
                await assertErrorThrownAsync(function () {
                    return dbExecAsync({
                        db,
                        sql: (`
SELECT win_ema2(1, NULL) FROM (SELECT 1);
                        `)
                    });
                }, "invalid argument 'alpha'");
                // test win_ema1-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT win_ema1(1, 1) FROM __tmp1;
                    `)
                });
                valActual = valActual[0].map(function ({val}) {
                    return val;
                });
                assertJsonEqual(valActual, [null]);
                // test win_ema2-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT win_ema2(1, 2, 3) FROM __tmp1;
                    `)
                });
                valActual = valActual[0].map(function ({val}) {
                    return val;
                });
                assertJsonEqual(valActual, [null]);
            }()),
            // test win_emax-aggregate-normal handling-behavior
            test_win_emax_aggregate({
                valExpected: [
                    0.0000, 0.4000, 1.0400, 1.8240,
                    2.6944, 3.2166, 4.3300, 5.3980,
                    6.4388, 7.4633, 8.4780, 9.0868
                ],
                valExpected2: 4.6868
            }),
            // test win_emax-aggregate-window handling-behavior
            test_win_emax_aggregate({
                aa: 1,
                bb: 3,
                valExpected: [
                    1.824, 2.824, 3.424, 4.584,
                    5.680, 6.608, 7.824, 8.824,
                    9.424, 9.424, 9.424, 9.424
                ],
                valExpected2: 5.024
            }),
            test_win_emax_aggregate({
                aa: 3,
                bb: 1,
                valExpected: [
                    0.400, 1.040, 1.824, 2.824,
                    3.424, 4.584, 5.680, 6.608,
                    7.824, 8.824, 9.424, 9.424
                ],
                valExpected2: 5.024
            }),
            test_win_emax_aggregate({
                aa: 4,
                bb: 0,
                valExpected: [
                    0.000, 0.400, 1.040, 1.824,
                    2.824, 3.424, 4.584, 5.680,
                    6.608, 7.824, 8.824, 9.424
                ],
                valExpected2: 5.024
            })
        ]);
    });
    jstestIt((
        "test sqlite-extension-win_quantilex handling-behavior"
    ), async function test_sqlite_extension_win_quantilex() {
        let db = await dbOpenAsync({filename: ":memory:"});
        let valIn;
        async function test_win_quantilex_aggregate({
            aa,
            bb,
            quantile,
            valExpected,
            valExpected2
        }) {
            let sqlBetween = "";
            let valActual;
            if (aa !== undefined) {
                sqlBetween = (
                    `ROWS BETWEEN ${aa - 1} PRECEDING AND ${bb} FOLLOWING`
                );
            }
            // test win_quantile1-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn: JSON.stringify(valIn)
                },
                db,
                sql: (`
SELECT
        win_quantile1(value->>1, ${quantile}) OVER (
            ORDER BY value->>0 ASC
            ${sqlBetween}
        ) AS val
    FROM JSON_EAcH($valIn);
                `)
            });
            valActual = valActual[0].map(function ({val}) {
                return Number(val.toFixed(4));
            });
            assertJsonEqual(valActual, valExpected);
            // test win_quantile2-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn: JSON.stringify(valIn)
                },
                db,
                sql: (`
SELECT
        id,
        win_quantile2(
            value->>1,
            IIF(id = 34, -1, value->>1),
            ${quantile}
        ) OVER (
            ORDER BY value->>0 ASC
            ${sqlBetween}
        ) AS val
    FROM JSON_EAcH($valIn);
                `)
            });
            valActual = valActual[0].map(function ({val}, ii) {
                val = JSON.parse(val).map(function (elem) {
                    return Number(elem.toFixed(4));
                });
                if (ii === 11) {
                    assertJsonEqual(val[1], valExpected2, valActual);
                } else {
                    assertJsonEqual(val[1], val[0], valActual);
                }
                return val[0];
            });
            assertJsonEqual(valActual, valExpected);
        }
        valIn = [
            [1, undefined],
            [2, "1"],
            [3, "2"],
            [4, 3],
            [5, 4],
            [6, "abcd"],
            [7, 6],
            [8, NaN],
            [9, 8],
            [10, 9],
            [11, 10],
            [12, 11]
        ];
        await Promise.all([
            (async function () {
                let valActual;
                // test win_quantile2-error handling-behavior
                await assertErrorThrownAsync(function () {
                    return dbExecAsync({
                        db,
                        sql: (`
SELECT win_quantile2(1) FROM (SELECT 1);
                        `)
                    });
                }, "wrong number of arguments");
                await assertErrorThrownAsync(function () {
                    return dbExecAsync({
                        db,
                        sql: (`
SELECT win_quantile2(1, NULL) FROM (SELECT 1);
                        `)
                    });
                }, "invalid argument 'quantile'");
                // test win_quantile1-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT win_quantile1(1, 1) FROM __tmp1;
                    `)
                });
                valActual = valActual[0].map(function ({val}) {
                    return val;
                });
                assertJsonEqual(valActual, [null]);
                // test win_quantile2-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT win_quantile2(1, 2, 3) FROM __tmp1;
                    `)
                });
                valActual = valActual[0].map(function ({val}) {
                    return val;
                });
                assertJsonEqual(valActual, [null]);
            }()),
            // test win_quantilex-aggregate-normal handling-behavior
            test_win_quantilex_aggregate({
                quantile: 0,
                valExpected: [
                    0.0000, 0.0000, 0.0000, 0.0000,
                    0.0000, 0.0000, 0.0000, 0.0000,
                    0.0000, 0.0000, 0.0000, 0.0000
                ],
                valExpected2: -1
            }),
            test_win_quantilex_aggregate({
                quantile: 0.25,
                valExpected: [
                    0.0000, 0.2500, 0.5000, 0.7500,
                    1.0000, 0.2500, 0.5000, 0.7500,
                    1.0000, 1.2500, 1.5000, 1.7500
                ],
                valExpected2: 0.7500
            }),
            test_win_quantilex_aggregate({
                quantile: 0.33333333,
                valExpected: [
                    0.0000, 0.3333, 0.6667, 1.0000,
                    1.3333, 0.6667, 1.0000, 1.3333,
                    1.6667, 2.0000, 2.3333, 2.6667
                ],
                valExpected2: 1.6667
            }),
            test_win_quantilex_aggregate({
                quantile: 0.5,
                valExpected: [
                    0.0000, 0.5000, 1.0000, 1.5000,
                    2.0000, 1.5000, 2.0000, 2.5000,
                    3.0000, 3.5000, 4.0000, 5.0000
                ],
                valExpected2: 3.5000
            }),
            test_win_quantilex_aggregate({
                quantile: 0.66666667,
                valExpected: [
                    0.0000, 0.6667, 1.3333, 2.0000,
                    2.6667, 2.3333, 3.0000, 3.6667,
                    4.6667, 6.0000, 6.0000, 6.6667
                ],
                valExpected2: 6.0000
            }),
            test_win_quantilex_aggregate({
                quantile: 0.75,
                valExpected: [
                    0.0000, 0.7500, 1.5000, 2.2500,
                    3.0000, 2.7500, 3.5000, 4.5000,
                    6.0000, 6.0000, 7.0000, 8.2500
                ],
                valExpected2: 6.5000
            }),
            test_win_quantilex_aggregate({
                quantile: 1,
                valExpected: [
                    0.0000, 1.0000, 2.0000, 3.0000,
                    4.0000, 4.0000, 6.0000, 6.0000,
                    8.0000, 9.0000, 10.0000, 11.0000
                ],
                valExpected2: 10.0000
            }),
            // test win_quantilex-aggregate-window handling-behavior
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0,
                valExpected: [
                    0.0000, 0.0000, 0.0000, 0.0000,
                    0.0000, 0.0000, 0.0000, 0.0000,
                    0.0000, 0.0000, 0.0000, 0.0000
                ],
                valExpected2: -1
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.25,
                valExpected: [
                    0.0000, 0.2500, 0.5000, 0.7500,
                    1.0000, 0.2500, 0.5000, 0.7500,
                    1.7500, 2.7500, 3.7500, 5.5000
                ],
                valExpected2: 3
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.33333333,
                valExpected: [
                    0.0000, 0.3333, 0.6667, 1.0000,
                    1.3333, 0.6667, 1.0000, 1.3333,
                    2.3333, 3.3333, 4.6667, 6.0000
                ],
                valExpected2: 4.6667
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.5000,
                valExpected: [
                    0.0000, 0.5000, 1.0000, 1.5000,
                    2.0000, 1.5000, 2.0000, 2.5000,
                    3.5000, 5.0000, 6.0000, 7.0000
                ],
                valExpected2: 6.0000
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.66666667,
                valExpected: [
                    0.0000, 0.6667, 1.3333, 2.0000,
                    2.6667, 2.3333, 3.0000, 3.6667,
                    5.3333, 6.0000, 7.3333, 8.6667
                ],
                valExpected2: 7.3333
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 0.75,
                valExpected: [
                    0.0000, 0.7500, 1.5000, 2.2500,
                    3.0000, 2.7500, 3.5000, 4.5000,
                    6.0000, 6.5000, 8.2500, 9.2500
                ],
                valExpected2: 8.2500
            }),
            test_win_quantilex_aggregate({
                aa: 8,
                bb: 0,
                quantile: 1.0000,
                valExpected: [
                    0.0000, 1.0000, 2.0000, 3.0000,
                    4.0000, 4.0000, 6.0000, 6.0000,
                    8.0000, 9.0000, 10.0000, 11.0000
                ],
                valExpected2: 10.0000
            })
        ]);
    });
    jstestIt((
        "test sqlite-extension-win_slrcos2 handling-behavior"
    ), async function test_sqlite_extension_win_slrcos2() {
        let db = await dbOpenAsync({filename: ":memory:"});
        let valExpected0;
        let valIn;
        async function test_win_slrcos2_aggregate({
            aa,
            bb,
            valExpected,
            valExpected2,
            valExpected3
        }) {
            let idLast = 25;
            let sqlBetween = "";
            let valActual;
            if (aa !== undefined) {
                sqlBetween = (
                    `ROWS BETWEEN ${aa - 1} PRECEDING AND ${bb} FOLLOWING`
                );
            }
            // test win_slrcos2-aggregate handling-behavior
            valActual = await dbExecAsync({
                bindList: {
                    valIn1: JSON.stringify(valIn.slice(0, -1)),
                    valIn2: JSON.stringify(valIn.slice(-1))
                },
                db,
                sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 AS
    SELECT
        id,
        __slr
    FROM (
        SELECT
            id,
            doublearray_jsonto(win_slrcos2(
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, value->>1,
                value->>0, IIF(id = ${idLast}, -1, value->>1)
            ) OVER (
                ORDER BY NULL ASC
                ${sqlBetween}
            )) AS __slr
        FROM JSON_EAcH($valIn1)
    );
INSERT INTO __tmp1
    SELECT
        ${idLast + 3},
        __slr
    FROM (
        SELECT
            doublearray_jsonto(win_slr2_step(
                doublearray_jsonfrom(__slr),
                ${aa + bb},
                $valIn2->>0->>0, $valIn2->>0->>1,
                $valIn2->>0->>0, $valIn2->>0->>1,
                $valIn2->>0->>0, $valIn2->>0->>1,
                $valIn2->>0->>0, $valIn2->>0->>1,
                $valIn2->>0->>0, $valIn2->>0->>1,
                $valIn2->>0->>0, $valIn2->>0->>1,
                $valIn2->>0->>0, $valIn2->>0->>1,
                $valIn2->>0->>0, $valIn2->>0->>1,
                $valIn2->>0->>0, $valIn2->>0->>1,
                $valIn2->>0->>0, $valIn2->>0->>1
            )) AS __slr
        FROM JSON_EAcH($valIn2)
        JOIN __tmp1 ON __tmp1.id = ${idLast}
    );
SELECT
        id,
        --
        ROUND(__slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 0), 8) AS nnn1,
        ROUND(__slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 1), 8) AS mxx1,
        ROUND(__slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 2), 8) AS myy1,
        ROUND(__slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 3), 8) AS exx1,
        ROUND(__slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 4), 8) AS eyy1,
        ROUND(__slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 5), 8) AS laa1,
        ROUND(__slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 6), 8) AS lbb1,
        ROUND(__slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 7), 8) AS lrr1,
        ROUND(__slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 8), 8) AS lyy1,
        ROUND(__slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 9), 8) AS lee1,
        ROUND(__slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 10), 8) AS xx01,
        ROUND(__slr->>(0 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 11), 8) AS yy01,
        --
        ROUND(__slr->>(8 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 0), 8) AS nnn2,
        ROUND(__slr->>(8 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 1), 8) AS mxx2,
        ROUND(__slr->>(8 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 2), 8) AS myy2,
        ROUND(__slr->>(8 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 3), 8) AS exx2,
        ROUND(__slr->>(8 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 4), 8) AS eyy2,
        ROUND(__slr->>(8 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 5), 8) AS laa2,
        ROUND(__slr->>(8 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 6), 8) AS lbb2,
        ROUND(__slr->>(8 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 7), 8) AS lrr2,
        ROUND(__slr->>(8 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 8), 8) AS lyy2,
        ROUND(__slr->>(8 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 9), 8) AS lee2,
        ROUND(__slr->>(8 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 10), 8) AS xx02,
        ROUND(__slr->>(8 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 11), 8) AS yy02,
        --
        ROUND(__slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 0), 8) AS nnn3,
        ROUND(__slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 1), 8) AS mxx3,
        ROUND(__slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 2), 8) AS myy3,
        ROUND(__slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 3), 8) AS exx3,
        ROUND(__slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 4), 8) AS eyy3,
        ROUND(__slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 5), 8) AS laa3,
        ROUND(__slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 6), 8) AS lbb3,
        ROUND(__slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 7), 8) AS lrr3,
        ROUND(__slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 8), 8) AS lyy3,
        ROUND(__slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 9), 8) AS lee3,
        ROUND(__slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 10), 8) AS xx03,
        ROUND(__slr->>(9 * ${SQLITE_WIN_SLR_ELEM_SIZE} + 11), 8) AS yy03
FROM __tmp1;
                `)
            });
            valActual = valActual[0].map(function ({
                exx1,
                exx2,
                exx3,
                eyy1,
                eyy2,
                eyy3,
                id,
                laa1,
                laa2,
                laa3,
                lbb1,
                lbb2,
                lbb3,
                lee1,
                lee2,
                lee3,
                lrr1,
                lrr2,
                lrr3,
                lyy1,
                lyy2,
                lyy3,
                mxx1,
                mxx2,
                mxx3,
                myy1,
                myy2,
                myy3,
                nnn1,
                nnn2,
                nnn3,
                xx01,
                xx02,
                xx03,
                yy01,
                yy02,
                yy03
            }, ii, list) {
                let obj1;
                let obj2;
                let obj3;
                obj1 = {
                    exx: exx1,
                    eyy: eyy1,
                    id,
                    laa: laa1,
                    lbb: lbb1,
                    lee: lee1,
                    lrr: lrr1,
                    lyy: lyy1,
                    mxx: mxx1,
                    myy: myy1,
                    nnn: nnn1,
                    xx0: xx01,
                    yy0: yy01
                };
                obj2 = {
                    exx: exx2,
                    eyy: eyy2,
                    id,
                    laa: laa2,
                    lbb: lbb2,
                    lee: lee2,
                    lrr: lrr2,
                    lyy: lyy2,
                    mxx: mxx2,
                    myy: myy2,
                    nnn: nnn2,
                    xx0: xx02,
                    yy0: yy02
                };
                obj3 = {
                    exx: exx3,
                    eyy: eyy3,
                    id,
                    laa: laa3,
                    lbb: lbb3,
                    lee: lee3,
                    lrr: lrr3,
                    lyy: lyy3,
                    mxx: mxx3,
                    myy: myy3,
                    nnn: nnn3,
                    xx0: xx03,
                    yy0: yy03
                };
                switch (list.length - ii) {
                case 1:
                    assertJsonEqual(obj2, obj1, valActual);
                    assertJsonEqual(obj3, valExpected3, valActual);
                    break;
                case 2:
                    assertJsonEqual(obj2, obj1, valActual);
                    assertJsonEqual(obj3, valExpected2, valActual);
                    break;
                default:
                    assertJsonEqual(obj2, obj1, valActual);
                    assertJsonEqual(obj3, obj1, valActual);
                }
                return obj1;
            });
            assertJsonEqual(valActual, valExpected);
        }
        valExpected0 = [
            {
                "exx": null,
                "eyy": null,
                "id": 1,
                "laa": null,
                "lbb": null,
                "lee": null,
                "lrr": null,
                "lyy": null,
                "mxx": 2,
                "myy": 0,
                "nnn": 1,
                "xx0": 2,
                "yy0": 0
            },
            {
                "exx": 0,
                "eyy": 0.70710678,
                "id": 4,
                "laa": null,
                "lbb": null,
                "lee": null,
                "lrr": null,
                "lyy": null,
                "mxx": 2,
                "myy": 0.5,
                "nnn": 2,
                "xx0": 2,
                "yy0": 0
            },
            {
                "exx": 0.57735027,
                "eyy": 1.52752523,
                "id": 7,
                "laa": -4.5,
                "lbb": 2.5,
                "lee": 0.70710678,
                "lrr": 0.94491118,
                "lyy": 3,
                "mxx": 2.33333333,
                "myy": 1.33333333,
                "nnn": 3,
                "xx0": 2,
                "yy0": 0
            },
            {
                "exx": 0.95742711,
                "eyy": 1.82574186,
                "id": 10,
                "laa": -3,
                "lbb": 1.81818182,
                "lee": 0.67419986,
                "lrr": 0.95346259,
                "lyy": 4.27272727,
                "mxx": 2.75,
                "myy": 2,
                "nnn": 4,
                "xx0": 2,
                "yy0": 0
            },
            {
                "exx": 1.30384048,
                "eyy": 2.07364414,
                "id": 13,
                "laa": -2.29411765,
                "lbb": 1.52941176,
                "lee": 0.65678958,
                "lrr": 0.96164474,
                "lyy": 5.35294118,
                "mxx": 3.2,
                "myy": 2.6,
                "nnn": 5,
                "xx0": 2,
                "yy0": 0
            },
            {
                "exx": 1.37840488,
                "eyy": 2.31660671,
                "id": 16,
                "laa": -2.54385965,
                "lbb": 1.63157895,
                "lee": 0.62126074,
                "lrr": 0.97080629,
                "lyy": 5.61403509,
                "mxx": 3.5,
                "myy": 3.16666667,
                "nnn": 6,
                "xx0": 2,
                "yy0": 0
            },
            {
                "exx": 1.38013112,
                "eyy": 2.37045304,
                "id": 19,
                "laa": -2.65,
                "lbb": 1.675,
                "lee": 0.57445626,
                "lrr": 0.9752227,
                "lyy": 5.725,
                "mxx": 3.71428571,
                "myy": 3.57142857,
                "nnn": 7,
                "xx0": 2,
                "yy0": 0
            },
            {
                "exx": 1.51185789,
                "eyy": 2.50713268,
                "id": 22,
                "laa": -2.5,
                "lbb": 1.625,
                "lee": 0.54006172,
                "lrr": 0.97991187,
                "lyy": 7.25,
                "mxx": 4,
                "myy": 4,
                "nnn": 8,
                "xx0": 2,
                "yy0": 0
            },
            {
                "exx": 2.39045722,
                "eyy": 2.26778684,
                "id": 25,
                "laa": 0.75,
                "lbb": 0.85,
                "lee": 1.08781126,
                "lrr": 0.89597867,
                "lyy": 9.25,
                "mxx": 5,
                "myy": 5,
                "nnn": 8,
                "xx0": 2,
                "yy0": 1
            },
            {
                "exx": 2.39045722,
                "eyy": 1.60356745,
                "id": 28,
                "laa": 2.75,
                "lbb": 0.55,
                "lee": 0.99163165,
                "lrr": 0.81989159,
                "lyy": 3.85,
                "mxx": 5,
                "myy": 5.5,
                "nnn": 8,
                "xx0": 0,
                "yy0": 0
            }
        ];
        valIn = [
            [2, "abcd"],
            [NaN, 1],
            [3, 3],
            [4, 4],
            [5, 5],
            [5, 6],
            [5, undefined],
            [6, 7],
            //
            [10, 8],
            [2, 5]
        ];
        await Promise.all([
            (async function () {
                let valActual;
                // test win_slrcos2-error handling-behavior
                await assertErrorThrownAsync(function () {
                    return dbExecAsync({
                        db,
                        sql: (`
SELECT doublearray_jsonto(win_slrcos2(1)) FROM (SELECT 1);
                        `)
                    });
                }, "wrong number of arguments");
                // test win_slrcos2-null-case handling-behavior
                valActual = await dbExecAsync({
                    db,
                    sql: (`
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 (val REAL);
SELECT doublearray_jsonto(win_slrcos2(1, 1)) FROM __tmp1;
                    `)
                });
                valActual = valActual[0].map(function ({val}) {
                    return val;
                });
                assertJsonEqual(valActual, [null]);
            }()),
            // test win_slrcos2-aggregate-normal handling-behavior
            (async function () {
                let valActual;
                valActual = await dbExecAndReturnLastJsonAsync({
                    bindList: {
                        valIn1: JSON.stringify(valIn.slice(0, -1)),
                        valIn2: JSON.stringify(valIn.slice(-1))
                    },
                    db,
                    sql: (`
SELECT
        doublearray_jsonto(win_slr2_step(
            __slr,
            ${valIn.length},
            $valIn2->>0->>0,
            $valIn2->>0->>1
        ))
    FROM (
        SELECT
            win_slrcos2(value->>0, value->>1) AS __slr
        FROM JSON_EACH($valIn1)
    );
                    `)
                });
                valActual = valActual.map(function (xx) {
                    return Number(xx.toFixed(8));
                }).slice(0, 12);
                assertJsonEqual(
                    valActual,
                    [
                        10, // nnn
                        4.4, // mxx
                        4.5, // myy
                        2.45854519, // exx
                        2.54950976, // eyy
                        0.77941176, // laa
                        0.84558824, // lbb
                        0.81541829, // lrr
                        2.47058824, // lyy
                        1.56536502, // lee
                        0, // xx0
                        0 // yy0
                    ]
                );
            }()),
            // test win_slrcos2-aggregate-window handling-behavior
            (async function () {
                let valActual;
                let valExpected;
                valActual = {
                    "exx": 29.003448070876,
                    "eyy": 4.89897948556636,
                    "laa": -0.820256776034237,
                    "lbb": 0.146219686162625,
                    "lee": 2.74202903932406,
                    "lrr": 0.865664999629448,
                    "lyy": 6.63694721825963,
                    "mxx": 74,
                    "myy": 10,
                    "nnn": 6
                };
                valExpected = noop(
                    await dbExecAsync({
                        db,
                        sql: (`
SELECT
        doublearray_extract(__slr, 0) AS nnn,
        doublearray_extract(__slr, 1) AS mxx,
        doublearray_extract(__slr, 2) AS myy,
        doublearray_extract(__slr, 3) AS exx,
        doublearray_extract(__slr, 4) AS eyy,
        doublearray_extract(__slr, 5) AS laa,
        doublearray_extract(__slr, 6) AS lbb,
        doublearray_extract(__slr, 7) AS lrr,
        doublearray_extract(__slr, 8) AS lyy,
        doublearray_extract(__slr, 9) AS lee
    FROM (
        SELECT
            win_slrcos2(xx, yy) AS __slr
        FROM (
            SELECT 34 AS xx, 5 AS yy
            UNION ALL SELECT 108, 17
            UNION ALL SELECT 64, 11
            UNION ALL SELECT 88, 8
            UNION ALL SELECT 99, 14
            UNION ALL SELECT 51, 5
        )
    )
                        `)
                    })
                )[0][0];
                assertJsonEqual(valActual, valExpected);
            }()),
            // test win_slrcos2-aggregate-window handling-behavior
            test_win_slrcos2_aggregate({
                aa: 8,
                bb: 0,
                valExpected: valExpected0,
                valExpected2: {
                    "exx": 2.39045722,
                    "eyy": 2.74837614,
                    "id": 25,
                    "laa": 5.25,
                    "lbb": -0.275,
                    "lee": 2.88241797,
                    "lrr": -0.23918696,
                    "lyy": 2.5,
                    "mxx": 5,
                    "myy": 3.875,
                    "nnn": 8,
                    "xx0": 2,
                    "yy0": 1
                },
                valExpected3: {
                    "exx": 2.39045722,
                    "eyy": 2.50356888,
                    "id": 28,
                    "laa": 7.25,
                    "lbb": -0.575,
                    "lee": 2.26016224,
                    "lrr": -0.5490214,
                    "lyy": 6.1,
                    "mxx": 5,
                    "myy": 4.375,
                    "nnn": 8,
                    "xx0": 0,
                    "yy0": 0
                }
            }),
            // test win_slrcos2-spx handling-behavior
            (async function () {
                let valActual;
                let valExpected;
                valExpected = String(`
##
caa cpp cww
0 0 0
0 0 0
11.80162844 0 0
10.22455353 2.4895005 3.85598789
9.67225352 3.13823881 3.43890867
9.38520882 3.05755264 3.37526868
11.15579422 1.67761426 3.82563076
10.43550311 2.2113198 3.71279007
10.94194012 2.13910515 3.69467822
10.38203105 1.94656038 3.76634934
9.90157092 2.08293992 3.73271653
10.92881058 1.75950221 3.80323052
11.72017621 1.59618572 3.84616339
11.29389859 1.4967185 3.88520253
11.00715398 1.63825985 3.86227002
10.81067912 1.68093078 3.85050268
14.03240329 1.43582 3.90436153
23.66195495 1.85723292 3.83435662
24.82818326 1.86394308 3.82754239
25.70137847 2.10938377 3.78349959
25.68038126 2.1899139 3.76437758
29.91349083 2.46349955 3.72166938
30.65662977 2.69140766 3.688013
30.04134638 2.81022418 3.67000203
30.42547793 2.92634758 3.65344588
30.92191243 3.0327673 3.63895998
30.51546171 3.02878829 3.63769386
30.68796503 3.06483286 3.63228076
30.77987366 2.88721701 3.6497531
31.43668848 2.97511155 3.63936172
31.47848618 2.98196635 3.63759004
31.47761576 3.08701607 3.62674109
31.10907836 3.1852698 3.61717997
30.94567193 3.25606384 3.61050582
30.51992802 3.30150012 3.60619724
30.54317585 3.37213961 3.59980578
37.65290494 3.06146091 3.62696707
51.37803242 3.27653295 3.6119397
60.85732073 3.2722773 3.61432556
78.71754176 3.45318028 3.6027943
91.95051659 3.62828188 3.59105743
93.91990229 3.7134328 3.58495764
98.7806608 3.82672026 3.57611856
98.46418618 3.83309517 3.57509828
100.94965761 3.71194721 3.57594745
103.68642909 3.55821362 3.58459275
117.38844377 3.62055074 3.57585938
119.34481792 3.83703118 3.56750985
126.13463306 4.26585668 3.54871054
146.62029061 4.15262236 3.55396389
148.09651512 4.31544836 3.54785071
164.17323306 4.07439365 3.55397083
167.19994363 4.01900697 3.55710631
173.10400788 3.92107795 3.55790648
175.99915343 3.8364972 3.56329894
180.6413271 4.1490884 3.55300356
184.60339929 4.16011777 3.55410809
181.10755568 4.38054437 3.54822015
177.22626809 4.14884626 3.55277026
174.36675016 4.32030405 3.54833428
168.39619693 3.77292287 3.56170234
166.41741668 3.98910019 3.55641658
164.08240212 3.71076777 3.56437269
160.49990407 3.99877082 3.55767806
156.67944352 4.16308126 3.55475713
153.79035921 4.01062381 3.55661075
160.79029727 4.07962364 3.55463429
168.84301085 3.50096163 3.56726078
181.61722513 3.53943285 3.56525259
195.17165245 2.84070709 3.58149667
205.7974472 3.01715364 3.57679898
218.81426821 3.14442165 3.57493685
226.4185739 3.44248496 3.56905123
232.70713576 3.42913269 3.56843159
239.3421892 3.19970208 3.57362621
241.35499828 2.61295207 3.58347868
237.74542774 2.45388947 3.58769528
233.14529127 2.19543653 3.59096936
226.49581765 2.78222153 3.58140625
222.16497773 3.28349155 3.5727835
219.76675649 3.38848478 3.57008623
214.93584545 3.56453023 3.56748017
214.33246799 3.15244964 3.5730981
212.06775885 3.1025012 3.57506247
201.14120339 2.48665956 3.58404995
191.94493002 3.12429736 3.57503454
171.54997616 3.38552457 3.57066601
153.36833963 3.1033655 3.5737161
133.95297942 3.70479978 3.56524062
126.43681782 2.67852318 3.57894894
106.29127136 4.18902782 3.55985888
94.88193891 2.8712456 3.57825989
97.41102901 3.71439037 3.56678618
88.80466709 3.51469058 3.56895571
90.85103756 3.70414929 3.56554472
90.06550501 3.9862641 3.56097038
90.63194347 4.1092685 3.55888928
90.3201687 3.73693619 3.56295426
85.8440406 4.51824042 3.55453675
73.06431673 4.02348905 3.56033163
70.68755941 4.20234458 3.55826273
68.6035428 4.38687986 3.55589586
68.49537876 4.56141971 3.55319685
67.62256789 5.02374461 3.54713154
67.6687234 5.29820125 3.54320558
67.3486889 0.01087314 3.53130659
61.90965401 5.85223616 3.53584163
57.90378111 6.17850094 3.53195866
51.5150017 5.36057832 3.54259912
55.48257879 5.53235156 3.54020823
56.7823217 5.45454666 3.54123166
57.66214381 5.85245513 3.53648716
60.31739178 6.02516793 3.53347253
61.78056074 0.59986562 3.52306874
61.10145177 1.29590276 3.51415037
61.06349496 2.45250355 3.50013673
61.12805708 3.61442097 3.48578294
59.50690522 3.75046599 3.48120881
59.98874471 4.08419218 3.47495709
59.91214852 4.63553866 3.4670527
60.21382624 4.83376926 3.46298834
62.91100199 4.86822515 3.46131217
64.24889899 5.01273969 3.45895941
68.38377931 5.08774712 3.45740848
70.38239102 4.95853294 3.45864361
70.0205693 4.73682302 3.46068442
70.0989148 3.94072892 3.4688249
70.08231923 3.57857029 3.472374
69.79126725 3.52079716 3.47278415
69.8513625 3.49059238 3.47235465
69.29959931 3.74351303 3.46945336
69.22423722 3.59542374 3.46981176
69.22337449 3.43254096 3.47036563
69.43428675 3.25202755 3.47126108
68.51199609 4.09275799 3.46388795
65.3215267 4.01501231 3.46415071
62.64033705 4.7098814 3.45860645
59.69365027 5.15108067 3.45506419
59.71146767 4.36327275 3.46219061
58.65913768 4.68163439 3.45964245
58.94300413 3.83601402 3.46659407
57.73804758 4.34069144 3.46241495
56.3048158 3.74869985 3.46658716
55.65933065 4.29098153 3.46199331
55.57452758 4.93900387 3.4566588
55.52101811 0.01157386 3.44613103
55.4292923 1.27764833 3.4361599
55.58744269 1.36608837 3.43537815
56.08068284 1.17042908 3.43656863
56.15234708 0.48890632 3.44221944
56.59412405 6.19431848 3.4464081
54.18259566 0.93287833 3.43947008
47.66749708 6.17978294 3.44653904
41.14878117 2.89149366 3.42478595
33.62757546 3.14158221 3.4225538
33.66611103 3.27495654 3.42125529
33.78874763 3.35870255 3.42015752
33.70843072 2.91712561 3.42275198
31.82535641 3.51680661 3.4182377
30.44462058 1.79901048 3.42996688
29.18710477 2.1945944 3.42720535
28.72495319 0.1193393 3.44195888
27.61382738 1.0617403 3.43578537
25.82690679 0.11084163 3.44196838
25.65346428 5.22613917 3.44937481
26.21631213 5.35940267 3.44745688
23.51768526 2.22200236 3.46710737
23.29498336 1.32730738 3.4720321
23.60175884 0.58704673 3.47585977
24.49499144 5.70922939 3.48324685
27.57921234 1.0257097 3.47176743
27.86482278 3.02962145 3.45702633
30.74858219 1.92478429 3.46422831
40.86229799 4.85570748 3.44380259
43.77725993 3.80148203 3.45069379
50.06126381 5.24635927 3.44028284
54.52280915 3.34956993 3.45284768
56.53063807 3.3445969 3.45221607
57.81118116 2.18622929 3.45921118
59.39119712 1.9394567 3.45980368
61.8032236 1.36676673 3.4628443
65.59625357 1.12693231 3.46365395
70.5777457 1.49944479 3.46084222
73.14066409 1.26071564 3.46220011
78.07336736 3.41489545 3.44889833
80.32210376 3.40104715 3.449485
80.19636698 3.19662906 3.45086563
77.58488339 3.00368228 3.4524693
75.97243984 1.97940161 3.45832848
73.11540581 2.88028999 3.45296322
70.7810171 1.96262023 3.45787125
68.86042077 3.15739392 3.45074299
67.58057939 3.20433517 3.45022944
66.2602667 5.11555812 3.43933603
66.28117179 0.91284564 3.42758139
67.0433405 1.5486217 3.42413503
68.92241083 2.52804252 3.41856567
72.42732119 5.88545468 3.43539882
75.15195455 4.74987155 3.44186665
76.5795457 1.90197797 3.45803015
77.63688999 1.88559298 3.45792495
78.62868561 1.28813333 3.46102436
78.66814636 1.59526936 3.45921634
78.72082689 1.97135 3.45709552
78.76324941 2.83031727 3.45266907
79.04989394 3.80891831 3.44761724
79.39271831 3.1432605 3.45163194
79.20443422 1.27085538 3.46202443
78.40723555 1.06606386 3.46330873
80.24583071 4.96267588 3.47598862
79.71400586 5.57652742 3.47284914
80.43897901 4.13141347 3.48016379
78.5254457 4.56495104 3.47841531
71.76423338 1.588077 3.49390572
70.34388381 1.83076595 3.49286282
70.5904264 0.94827327 3.49770066
71.55327341 2.17077402 3.49139752
72.73943857 2.67421541 3.48887168
73.95258579 2.76222104 3.4882643
75.56568387 2.49746142 3.48938975
75.50615275 6.14370592 3.50246875
75.69744412 5.83138125 3.50401951
76.83095495 2.36468497 3.52137188
77.39428154 2.66139711 3.51978825
77.46914123 3.52377654 3.51546842
77.47997098 5.18447664 3.50727573
77.47920574 0.82308794 3.4978681
76.28109206 2.43707564 3.48996194
75.71043602 2.00269081 3.49165631
75.81557792 1.07500385 3.4960594
76.29041877 0.03791493 3.50096151
76.35259799 5.19406645 3.50650901
76.9316934 6.12417832 3.50211779
77.37705305 1.02728739 3.49668124
77.61008626 3.31889567 3.4859508
78.0250086 4.63058282 3.47978624
78.31443009 5.54567321 3.47529463
78.33125703 0.24436895 3.47055569
77.59150572 5.89082944 3.47312707
76.04594186 0.99509994 3.46685558
71.9977583 6.26785741 3.47130523
69.00404458 0.00481422 3.47170679
66.37481386 5.87809054 3.47392517
63.82046907 5.08991427 3.47774147
60.98187185 5.99887666 3.47382788
60.43674811 4.92293437 3.47845832
59.99883742 5.88197317 3.47416716
60.28517964 4.85446267 3.47840472
60.3234432 5.02754825 3.47789502
59.94849295 4.03889442 3.48221181
60.29967072 2.60343069 3.48881978
60.89671424 3.95982174 3.48314573
57.07411404 4.27760296 3.48197272
53.52091645 3.93009875 3.48325614
45.69906281 6.1373053 3.47405014
##
                `).trim().split("\n").slice(2, -1).map(function (elem) {
                    elem = elem.split(" ");
                    return {
                        caa: Number(Number(elem[0]).toFixed(8)),
                        cpp: Number(Number(elem[1]).toFixed(8)),
                        cww: Number(Number(elem[2]).toFixed(8))
                    };
                });
                valActual = noop(
                    await dbExecAsync({
                        bindList: {
                            testDataSpx
                        },
                        db,
                        sql: (`
SELECT
        -- doublearray_extract(__slr, 0) AS nnn,
        -- doublearray_extract(__slr, 3) AS exx,
        --
        -- doublearray_extract(__slr, 5) AS laa,
        -- doublearray_extract(__slr, 6) AS lbb,
        -- doublearray_extract(__slr, 8) AS lyy,
        -- doublearray_extract(__slr, 9) AS lee,
        --
        doublearray_extract(__slr, 12) AS caa,
        doublearray_extract(__slr, 13) AS cpp,
        doublearray_extract(__slr, 14) AS cww
        --
        -- doublearray_extract(__slr, 15) AS cyy,
        -- doublearray_extract(__slr, 16) AS cee
    FROM (
        SELECT
            win_slrcos2(value->>'ii', value->>'priceClose') OVER (
                ORDER BY NULL ASC
                ROWS BETWEEN 42 PRECEDING AND 0 FOLLOWING
            ) AS __slr,
            value->'priceClose' AS yy
        FROM JSON_EAcH($testDataSpx)
    );
                        `)
                    })
                )[0].map(function ({caa, cpp, cww}) {
                    return {
                        caa: Number(Number(caa).toFixed(8)),
                        cpp: Number(Number(cpp).toFixed(8)),
                        cww: Number(Number(cww).toFixed(8))
                    };
                });
                assertJsonEqual(
                    valActual,
                    valExpected,
                    (
                        "\n\ncaa cpp cww\n"
                        + valActual.map(function (elem) {
                            return `${elem.caa} ${elem.cpp} ${elem.cww}\n`;
                        }).join("")
                    )
                );
            }())
        ]);
    });
});

jstestDescribe((
    "test_sqlmathWebworkerInit"
), function test_sqlmathWebworkerInit() {
    jstestIt((
        "test sqlmathWebworkerInit handling-behavior"
    ), async function () {
        let db = await dbOpenAsync({
            dbData: new ArrayBuffer(),
            filename: ":memory:"
        });
        sqlmathWebworkerInit({
            db,
            modeTest: true
        });
    });
});
