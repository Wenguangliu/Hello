#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>
#include <math.h>


#include "ManufacturingStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "AmsNdxtTestStates.h"
#include "Shmem.h"
#include "LogErr.h"


#define MAX_NDXT_REPORT_STR_LENGTH     10240 
#define MIN_BAND_BLOCK_SIGNAL          0.5
#define FLASH                          0

/* Dac trim wavelength index table */
unsigned char dacWaveLengthTable [] =
        {
           W850, W630, W515, W600,      // group 0
           W850, W550, W467, W500,      // group 1
           W850, W467, W405, W340       // group 2 
        };

/* Ndxt wavelength index table */
unsigned char ndxtWaveLengthTable [] =
        {
            W850, W630, W515, W600,     // group 0
            W850, W550, W467, W500,     // group 1
            W850, W405, W340, W340      // group 2
        };

NdxtFinalErrorReport_t  ndxtFinalErrorReport [] = 
                                                {
                                                    NO_ERROR_REPORT,                "No Error",
                                                    DAC_SETTING_MIN_LEVEL_ERR,      "DAC Setting Min Level Error",
                                                    DAC_SETTING_MAX_LEVEL_ERR,      "DAC Setting Max Level Error",
                                                    AVG_MIN_OFFSET_ERR,             "Average Min Offset Error",
                                                    AVG_MAX_OFFSET_ERR,             "Average Max Offset Error",
                                                    MAX_OFFSET_SD_ERR,              "Max Offset SD Error",
                                                    MAX_CV_ERR,                     "Max CV Error",
                                                    MIN_SCALED_COUNTS_ERR,          "Min Scaled Counts Error",
                                                    ABS_VALUE_LARGE_ERR,            "Absorbances Value Lager Error",
                                                    MAX_NORM_CV_ERR,                "Max Normalized CV Error",
                                                    BAND_PASS_LIMIT_ERR,            "Band Pass Limit Error",
                                                    BAND_BLOCK_LIMIT_ERR,           "Band Block Limit Error",
                                                    BAND_BLOCK_MIN_SIGNAL_ERR,      "Band Block Min Signal Error",
                                                    MAX_REPORT_ERR,                 " "
                                                };

static int  trimmedDataIndex = 0;                // for optical group, trimmedDataIndex refer to gruop number (cuvette is fixed at 29) 
                                                 // for main group,  trimmedDataIndex is cuvette based, totallly 90 = 3 group x 30 cvuette numbers   

int                 rawOffsets [PASSNUM][CHANNUM][FLASHNUM];
int                 rawOpticals [PASSNUM][CHANNUM][FLASHNUM];
unsigned short      wavelengthAdcOffset [10];

#if 0
double  nd05_expected_abs[9] = {0.448,0.474,0.489,0.493,0.494,0.495,0.495,0.493,0.480};
double  nd10_expected_abs[9] = {1.071,1.085,1.079,1.071,1.067,1.056,1.040,1.031,0.986};
double  nd20_expected_abs[9] = {1.774,1.742,1.688,1.650,1.631,1.591,1.543,1.516,1.402};
double  band_block_limits[9] = {.003,.003,.003,.005,.005,.009,.005,.005,.015};
double  nd05_diff_limit = 6.0; 
double  nd10_diff_limit = 6.0;
double  nd20_diff_limit = 6.0;
#else
double  *nd05_expected_abs;
double  *nd10_expected_abs;
double  *nd20_expected_abs;
double  *band_block_limits;
double  nd05_diff_limit; 
double  nd10_diff_limit;
double  nd20_diff_limit;
#endif
const  int int2rep[10] = { 8,6,4,5,1,3,2,0,9,9 };
const  int dac2rep[10] = { 8,7,6,5,2,4,3,1,0,9 };
//Following is in internal order: 340,405,467,500,515,550,600,630,850
const int   band_pass_cuvs[WAVENUM] = { 19,12,13,14,15,16,17,18,11 };
const int band_block_cuvs[WAVENUM] = { 3,4,5,6,7,8,25,26,27 };
const int   pass_list[WAVENUM] =    { 2,2,1,1,0,1,0,0,0 };
const int   channel_list[WAVENUM] = { 3,1,2,3,2,1,3,1,0 };

double  dac_averages[CHANNUM];
double  dac_stds[CHANNUM];
double  dac_ranges[CHANNUM];
double  dac_cvs[CHANNUM];
double  average_offsets[10];
double  average_opticals[10];
double  optical_cvs[10];
double  offset_sds[10];
double  dac_settings[NUM_DACS];
double  refd_cvs[10];
double  refd_ranges[10];
double  nd05_absorbances[10];
double  nd10_absorbances[10];
double  nd20_absorbances[10];
double  nd05_diff[10];
double  nd10_diff[10];
double  nd20_diff[10];
double  nd05_abs[10];
double  nd10_abs[10];
double  nd20_abs[10];
double  band_pass[WAVENUM][WAVENUM];
double  band_block[WAVENUM][WAVENUM];


#define MIN_DAC_SETTING 10
#define MAX_DAC_SETTING 200
#define MIN_OFFSET          -100
//#define MAX_OFFSET          100
//#define MAX_OFFSET          950 
#define MAX_CV                  2.5
#define MAX_OFFSET_SD       20
#define MIN_SCALED_COUNTS   20000.0
#define MAX_NORM_CV         1.0
#define ND_DIFF_LIMIT       5.0         // 5.0 %

/*-----------------05-24-95 09:43am-----------------
 Wavelength definitions
--------------------------------------------------*/
#define REPORTED_340        0
#define REPORTED_405        1
#define REPORTED_467        2
#define REPORTED_500        3
#define REPORTED_515        4
#define REPORTED_550        5
#define REPORTED_600        6
#define REPORTED_630        7
#define REPORTED_850        8

/*-----------------05-24-95 09:43am-----------------
 Labels
--------------------------------------------------*/
const char  FLAGGED_FIELD[] = {"*"};
const char  NO_FLAG[] =             {" "};

const char  W340_LABEL[] =      {"340  "};
const char  W405_LABEL[] =      {"405  "};
const char  W467_LABEL[] =      {"467  "};
const char  W500_LABEL[] =      {"500  "};
const char  W515_LABEL[] =      {"515  "};
const char  W550_LABEL[] =      {"550  "};
const char  W600_LABEL[] =      {"600  "};
const char  W630_LABEL[] =      {"630  "};
const char  W850_LABEL[] =      {"850  "};

#if 0
const   char    REF340_LABEL[] =        {"  | 340/850  "};
const char  REF405_LABEL[] =        {"  | 405/850  "};
const char  REF467_LABEL[] =        {"  | 467/850  "};
const char  REF500_LABEL[] =        {"  | 500/850  "};
const char  REF515_LABEL[] =        {"  | 515/850  "};
const char  REF550_LABEL[] =        {"  | 550/850  "};
const char  REF600_LABEL[] =        {"  | 600/850  "};
const char  REF630_LABEL[] =        {"  | 630/850  "};
#else
const char  REF340_LABEL[] =        {"   340/850  "};
const char  REF405_LABEL[] =        {"   405/850  "};
const char  REF467_LABEL[] =        {"   467/850  "};
const char  REF500_LABEL[] =        {"   500/850  "};
const char  REF515_LABEL[] =        {"   515/850  "};
const char  REF550_LABEL[] =        {"   550/850  "};
const char  REF600_LABEL[] =        {"   600/850  "};
const char  REF630_LABEL[] =        {"   630/850  "};
#endif


const char  ND05_LABEL[] =          {"0.5 OD "};
const char  ND10_LABEL[] =          {"1.0 OD "};
const char  ND20_LABEL[] =          {"2.0 OD "};

const char *wavelength_labels[] = { W340_LABEL,
                                                                W405_LABEL,
                                                                W467_LABEL,
                                                                W500_LABEL,
                                                                W515_LABEL,
                                                                W550_LABEL,
                                                                W600_LABEL,
                                                                W630_LABEL,
                                                                W850_LABEL,
                                                                };

const char *refd_wavelength_labels[] = {    REF340_LABEL,
                                                                REF405_LABEL,
                                                                REF467_LABEL,
                                                                REF500_LABEL,
                                                                REF515_LABEL,
                                                                REF550_LABEL,
                                                                REF600_LABEL,
                                                                REF630_LABEL };

const char  *nd_diff_labels[] = {   ND05_LABEL, ND10_LABEL, ND20_LABEL };

char                    test_failed;
NdxtFinalReportError_t      finalReportError;




// save for trimmed data calculation values
static TrimmedData_t trimmedData [MAX_PACKETS];

/*
 *  CalcStatistics()
 *
 *  Calculates the average, standard deviation,
 *  % range and % cv for the data set passed.
 *
 */

AmsNdxtTestErrorCode_t
CalcStatistics (int asize, unsigned int rData[][FLASHNUM], double *pavg, double *pstd, double *prange, double *pcv)
{
    int     i,j;
    double   min,max,darr, sum, sumsqr, set_size;


    // asize is flash numbers
    if (asize < 3) {
        LogError ("DATA_VALUE_ERROR: asize =%d", asize);
        return DATA_VALUE_ERROR;
    }

    for (j = 0; j < CHANNUM; j++) {
        // initialize the min and max values...
        min = (double) rData[j][0];
        max = (double) rData[j][0];

        // Generate the sum and square terms
        sum = 0.0;
        sumsqr = 0.0;
        for (i = 0; i < asize; i++) {
            darr = (double)rData[j][i];
            sum += darr;
            sumsqr += (darr*darr);
            if (darr < min) {
                min = darr;
            } 
            if (darr > max) {
                max = darr;
            }
        }

        // Now generate the averages, standard deviations and ranges
        sumsqr = sumsqr - max*max - min*min;
        sum = sum - min - max;
        set_size = (double)(asize-2);
        pavg[j] = sum / set_size;
        pstd[j] = sqrt((set_size * sumsqr - sum * sum) / (set_size * set_size));

        if (pavg[j] == 0.0) {
            pcv[j] = 0.0;
        } else {
            pcv[j] = pstd[j] / pavg[j] * 100.0;
        }
        if (sum == 0) {
            prange[j] = 0.0;
        } else {
            prange[j] = ((max-min) / (sum/asize)) * 100.0;
        }
    }
    return NO_ERRORS;
}




// calculate optical dac trim values 
// notice : the optical DAC is divied to three group
// each group consist of 4 wave length 
// this function needs to be called first when start calculation
void
GetOpticalDacGroupData (RawData_t  *rawData)
{
    int                     flashNum, group, wave, i, j;               ;
    unsigned short          *rawFlashWaveRead; 
    unsigned int            rData [CHANNUM][FLASHNUM];
    double                  dacAverages [CHANNUM];
    double                  dacStds [CHANNUM];
    double                  dacRanges [CHANNUM];
    double                  dacCvs [CHANNUM];
    
    wave = 0;

    //each group get 4 wavelength
    // ZZZ for group 2, it may need to zero all the data ???
    for (group = 0; group < 3; group++) {
        for (flashNum = 1;  flashNum < NUM_OPTICAL_DACS_FLASHES +1; flashNum++) {
            rawFlashWaveRead  = rawData->rotorRawReadings.opticalDacs [flashNum].rawFlashReadings;
            // get raw data 
            for (i = 0; i < CHANNUM; i++) {
                if ((group == 2) && (i == 1)) {
                    // group 2 channel 1 is set 0
                     rData [i][flashNum -1] = FLASH;
                } else {
                    rData [i][flashNum -1] = rawFlashWaveRead  [dacWaveLengthTable [wave + i]];
                }
            }
        }
        // now calculate the statistics based on group
        CalcStatistics (NUM_OPTICAL_DACS_FLASHES, rData, dacAverages, dacStds, dacRanges, dacCvs);
        // save the data to array, trimmedDataIndex = 0 -2
        for (j = 0; j != CHANNUM; ++j) {
            trimmedData[trimmedDataIndex].averages[j]   = dacAverages[j];
            trimmedData[trimmedDataIndex].stds[j]       = dacStds[j];
            trimmedData[trimmedDataIndex].ranges[j]     = dacRanges[j];
            trimmedData[trimmedDataIndex].cvs[j]        = dacCvs[j];
       }
       //increase package number
       trimmedDataIndex++;
        // next group wavelength
       wave += 4;
    }
}


// calculate main group sampling data
// it sampling cuvette from 0 to 29 and each flash 21 times
// the sampling data is divided to 3 groups based on 
// difference wavelength   
AmsNdxtTestErrorCode_t
GetMainGroupData (RawData_t  *rawData)
{
    int                     flashNum, cuvNum, group, wave, i, j, k;
    unsigned short			*rawFlashWaveRead;
    unsigned int            rData [CHANNUM][FLASHNUM];
    double                   dacAverages [CHANNUM];
    double                   dacStds [CHANNUM];
    double                   dacRanges [CHANNUM];
    double                   dacCvs [CHANNUM];


    wave = 0;
    for (group = 0; group < 3; group++) {
        for (cuvNum = 0; cuvNum < 30; cuvNum++) {
            for (flashNum = 1; flashNum < MAX_NDXT_SAMPLE_FLASHES + 1; flashNum++) {
                rawFlashWaveRead =  rawData->rotorRawReadings.ndxtSamples [cuvNum][flashNum].rawFlashReadings;
                // get raw data
                for (i = 0; i < CHANNUM; i++) {
                   rData [i][flashNum -1] = rawFlashWaveRead  [ndxtWaveLengthTable [wave + i]];
                }
            }
            if (cuvNum == BLACK_CUVETTE) {
                //copy the raw values in raw array into the raw_offsets array for futur use
                for (j = 0; j != CHANNUM; ++j) {
                    for (k = 0; k < FLASHNUM; k++) {
                      rawOffsets [group][j][k] = rData[j][k];
                    }
                }
            }
            if (cuvNum == OPTICAL_CUVETTE) {
                // copy the raw values in raw array into the raw_opticals array
                for (j = 0; j < CHANNUM; j++) {
                    for (k = 0; k < FLASHNUM; k++) {
                      rawOpticals [group][j][k] = rData[j][k];
                    }
                }

            }
            // now calculate the statistics based on group
            CalcStatistics (MAX_NDXT_SAMPLE_FLASHES, rData, dacAverages, dacStds, dacRanges, dacCvs);
            // save the data to array, 
            // note: j =0 is always W850 data
            // trimmedDataIndex  = 3 - 92  on cuvette based
            // example : cuvette 0, should be saved on 
            // trimmedDataIndex = 3, 33, 63
            //  cuvette 29 should be saved on
            // trimmedDataIndex = 32, 62, 92 
            for (j = 0; j != CHANNUM; ++j) {
                trimmedData[trimmedDataIndex].averages[j]   = dacAverages[j];
                trimmedData[trimmedDataIndex].stds[j]       = dacStds[j];
                trimmedData[trimmedDataIndex].ranges[j]     = dacRanges[j];
                trimmedData[trimmedDataIndex].cvs[j]        = dacCvs[j];
            }
            trimmedDataIndex++;
        }
        // update wavelength for new group
        wave += 4;

    }
    return NO_ERRORS;
}

// special order put into dac arrary 
//last call to get dac trim value
//calibrationData->wavelengthDacTrims[i]
void
GetTrimDacValue (unsigned char *DacTrim)
{
    int i;

    for (i = 0; i < 9; i++) {
        dac_settings [i] = (double)(DacTrim [i]);
    }
}

 // calculate black offset values
void
CalWavelengthOffsets (RawData_t  *rawData)
{
    unsigned char   waveLength;
    unsigned        flashNumber;
    unsigned long   offsetSum;
    unsigned short  rData;
    unsigned short *rawFlashWaveRead;
    
    // compute average of black offset readings for each wavelength

    for (waveLength = 0; waveLength < NUM_WAVELENGTHS; waveLength++) {
        offsetSum = 0;
        // sum raw readings for number of flashes (skip flash 0)
        for (flashNumber = 1; flashNumber <= NUM_BLACK_OFFSETS_FLASHES; flashNumber++) {
            rawFlashWaveRead = rawData->rotorRawReadings.blackOffsets [flashNumber].rawFlashReadings;
            rData = rawFlashWaveRead [waveLength];
            offsetSum += rData;
        }
        wavelengthAdcOffset [waveLength]  = (unsigned short)(offsetSum / NUM_BLACK_OFFSETS_FLASHES);
    }
}


 
 //Calculates the % range and %cv for the referenced data set passed.
AmsNdxtTestErrorCode_t
CalcRefdStats (int asize, double arr[][FLASHNUM], double pcv[CHANNUM], double prange[CHANNUM])
{
    int i,j;
    double darr, min, max, sum, sumsqr, set_size, pavg, pstd;


    prange[0] = 0.0;
    pcv[0] = 0.0;

    if (asize < 3) {
        LogError ("DATA_VALUE_ERROR: asize=%d", asize);
        return DATA_VALUE_ERROR;
    }

    for (j = 1; j < CHANNUM; j++) {

        //initialize the min and max values...
        min = arr[j][0];
        max = arr[j][0];

        // Generate the sum and square terms
        sum = 0.0;
        sumsqr = 0.0;

        for (i = 0; i < asize; i++) {
            darr = arr[j][i];
            sum += darr;
            sumsqr += darr*darr;
            if (darr < min) {
                min = darr;
            }
            if (darr > max) {
                max = darr;
            }
        }

       //  Now trim and generate the averages, standard deviations and ranges
        sumsqr = sumsqr - max*max - min*min;
        sum = sum - min - max;
        set_size = (double)(asize-2);
        pavg = sum / set_size;
        pstd = sqrt((set_size * sumsqr - sum * sum) / (set_size * set_size));

        if (pavg == 0) {
            LogError ("DIVIDE_BY_ZERO_2_ERROR"); 
            return DIVIDE_BY_ZERO_2_ERROR;
        }

        pcv[j] = pstd / pavg * 100.0;

        if (sum == 0) {
            LogError ("DIVIDE_BY_ZERO_3_ERROR");
            return DIVIDE_BY_ZERO_3_ERROR;
        }

        prange[j] = ((max-min) / (sum/asize)) * 100;
    }
    return NO_ERRORS;
}




// Completes the calculations on the data
AmsNdxtTestErrorCode_t
CrunchData (void)
{

    int i,j,k, current_index, trindex1, trindex2, trindex3;
    AmsNdxtTestErrorCode_t err_val;
    double offset_850_avg=0.0, full_scale_850_avg = 0.0, ref_cv[CHANNUM],ref_range[CHANNUM];
    double offset_850_sd = 0.0, optical_850_cv = 0.0, ref_value, refd_data[CHANNUM][FLASHNUM];
    double  nd05_xmit[10], nd10_xmit[10], nd20_xmit[10];
    double  band_pass_avgs[WAVENUM][WAVENUM], band_block_avgs[WAVENUM][WAVENUM];

    // First step is to generate the black (offset)   
    // full scale (optical) values for report. 850nm averages all 3 sets.

    // note: for trimmedData array, 0 -2 is optical group sampling data calculation
    //       from 3 - 92 are main loop sampling data calculation 
    // get main 3 group calculation data. 850 is always on the first group element  
    for (i = 0; i < 3; i++) {
        // Reference proper set of data, MAIN_DATA_INDEX = 3, skip optical data group data
        // j = 0 is always W850 channel data
        // for black offset
        trimmedDataIndex = MAIN_DATA_INDEX + BLACK_CUVETTE + (i * CUVNUM);
        //Do 850nm separately
        offset_850_avg  += trimmedData[trimmedDataIndex].averages[0];
        offset_850_sd   += trimmedData[trimmedDataIndex].stds[0];

        // get other wavelength calculated data
        for (j = 0; j < 3; j++) {
            average_offsets[j + (i * 3)] = trimmedData[trimmedDataIndex].averages[j+1];
            offset_sds[j + (i * 3)] = trimmedData[trimmedDataIndex].stds[j+1];
        }

        //for the full scale readings (opticals)
        trimmedDataIndex = MAIN_DATA_INDEX + OPTICAL_CUVETTE + (i * CUVNUM);

        /*-----------------05-12-95 10:05am-----------------
         Do 850nm separately
        --------------------------------------------------*/
        full_scale_850_avg += trimmedData[trimmedDataIndex].averages[0];
        optical_850_cv += trimmedData[trimmedDataIndex].cvs[0];

        for (j = 0; j < 3; j++) {
            average_opticals[j + (i * 3)] = trimmedData[trimmedDataIndex].averages[j+1];
            optical_cvs[j + (i * 3)] = trimmedData[trimmedDataIndex].cvs[j+1];
        }
    }

    //put 850nm offsets and opticals into averages
    average_offsets[9] = offset_850_avg /= 3.0;
    average_opticals[9] = full_scale_850_avg /= 3.0;
    offset_sds[9] = offset_850_sd /= 3.0;
    optical_cvs[9] = optical_850_cv /= 3.0;

    // Generate the referenced cv's and ranges for the optical readings.
    // Note that the raw values for all the opticals and offsets were
    // saved separately.  Offsets must be subtracted before referencing.

    for (i = 0; i < PASSNUM; i++) {
        for (j = 1; j < CHANNUM; j++) {
            //current_index = wavelength 0 - 8, 9 is W850
            current_index = i * 3 + (j - 1);
            for (k = 0; k < FLASHNUM; k++) {
                ref_value = (double)rawOpticals[i][0][k] - average_offsets[9]; /* 850nm term */
                if (ref_value == 0) {
                    LogError ("DIVIDE_BY_ZERO_4_ERROR");
                    return DIVIDE_BY_ZERO_4_ERROR;
                }
                refd_data[j][k] = ((double)rawOpticals[i][j][k] - average_offsets[current_index]) / ref_value;

            }
        }
        if ((err_val = CalcRefdStats (FLASHNUM, refd_data, ref_cv, ref_range)) != NO_ERRORS) {
            LogError ("CalcRefdStats: errValue = %d", err_val);
            return err_val;
        }
        for (j = 0; j < CHANNUM - 1; j++) {
            refd_cvs[i * 3 + j] = ref_cv [j+1];
            refd_ranges[i * 3 + j] = ref_range [j+1];
        }
    }

    // Calculate absorbances for neutral density filter test
    for (i = 0; i < PASSNUM; i++) {
        /*-----------------05-12-95 10:05am-----------------
         Reference proper set of data
        --------------------------------------------------*/
        trindex1 = MAIN_DATA_INDEX + ND05_CUVETTE + (i * CUVNUM);
        trindex2 = MAIN_DATA_INDEX + ND10_CUVETTE + (i * CUVNUM);
        trindex3 = MAIN_DATA_INDEX + ND20_CUVETTE + (i * CUVNUM);

        for (j = 0; j < 3; j++) {
            current_index = j + (i * 3);
            ref_value = average_opticals[current_index] - average_offsets[current_index];
            if (ref_value == 0.0) {
                LogError ("DIVIDE_BY_ZERO_5_ERROR");
                return DIVIDE_BY_ZERO_5_ERROR;
            }
            nd05_xmit[current_index] = (trimmedData[trindex1].averages[j+1] - average_offsets[current_index]) / ref_value;
            nd10_xmit[current_index] = (trimmedData[trindex2].averages[j+1] - average_offsets[current_index]) / ref_value;
            nd20_xmit[current_index] = (trimmedData[trindex3].averages[j+1] - average_offsets[current_index]) / ref_value;

            if (nd05_xmit[current_index] <= 0.0 || nd10_xmit[current_index] <= 0.0 || nd20_xmit[current_index] <= 0.0) {
                LogError ("ILLEGAL_LOG_PARAM_ERROR"); 
                return ILLEGAL_LOG_PARAM_ERROR;
            }

            nd05_abs[current_index] = -log10(nd05_xmit[current_index]);
            nd10_abs[current_index] = -log10(nd10_xmit[current_index]);
            nd20_abs[current_index] = -log10(nd20_xmit[current_index]);

#if 1 
            // ZZZ
            nd05_diff[current_index] = (nd05_abs[current_index] - nd05_expected_abs[int2rep[current_index]]) * 100.0 / nd05_expected_abs[int2rep[current_index]];
            nd10_diff[current_index] = (nd10_abs[current_index] - nd10_expected_abs[int2rep[current_index]]) * 100.0 / nd05_expected_abs[int2rep[current_index]];
            nd20_diff[current_index] = (nd20_abs[current_index] - nd20_expected_abs[int2rep[current_index]]) * 100.0 / nd05_expected_abs[int2rep[current_index]];
#else
            nd05_diff[current_index] = (nd05_abs[current_index] - nd05_expected_abs[int2rep[current_index]]) * 100.0 / nd05_expected_abs[int2rep[current_index]];
            nd10_diff[current_index] = (nd10_abs[current_index] - nd10_expected_abs[int2rep[current_index]]) * 100.0 / nd10_expected_abs[int2rep[current_index]];
            nd20_diff[current_index] = (nd20_abs[current_index] - nd20_expected_abs[int2rep[current_index]]) * 100.0 / nd20_expected_abs[int2rep[current_index]];
#endif
        }
    }
    // finish up with the first set of 850nm
    current_index = 9;
    ref_value = average_opticals[current_index] - average_offsets[current_index];
    if (ref_value == 0.0) {
        LogError ("DIVIDE_BY_ZERO_6_ERROR");
        return DIVIDE_BY_ZERO_6_ERROR;
    }
    nd05_xmit[current_index] = (trimmedData[MAIN_DATA_INDEX + ND05_CUVETTE].averages[0] - average_offsets[current_index]) / ref_value;
    nd10_xmit[current_index] = (trimmedData[MAIN_DATA_INDEX + ND10_CUVETTE].averages[0] - average_offsets[current_index]) / ref_value;
    nd20_xmit[current_index] = (trimmedData[MAIN_DATA_INDEX + ND20_CUVETTE].averages[0] - average_offsets[current_index]) / ref_value;

    if (nd05_xmit[current_index] <= 0.0 || nd10_xmit[current_index] <= 0.0 || nd20_xmit[current_index] <= 0.0) {
        LogError ("ILLEGAL_LOG_PARAM_ERROR");
        return ILLEGAL_LOG_PARAM_ERROR;
    }

    nd05_abs[current_index] = -log10(nd05_xmit[current_index]);
    nd10_abs[current_index] = -log10(nd10_xmit[current_index]);
    nd20_abs[current_index] = -log10(nd20_xmit[current_index]);

#if 1
    // ZZZ
    nd05_diff[current_index] = (nd05_abs[current_index] - nd05_expected_abs[9]) * 100.0 / nd05_expected_abs[9];
    nd10_diff[current_index] = (nd10_abs[current_index] - nd10_expected_abs[9]) * 100.0 / nd05_expected_abs[9];
    nd20_diff[current_index] = (nd20_abs[current_index] - nd20_expected_abs[9]) * 100.0 / nd05_expected_abs[9];
#else
    nd05_diff[current_index] = (nd05_abs[current_index] - nd05_expected_abs[9]) * 100.0 / nd05_expected_abs[9];
    nd10_diff[current_index] = (nd10_abs[current_index] - nd10_expected_abs[9]) * 100.0 / nd10_expected_abs[9];
    nd20_diff[current_index] = (nd20_abs[current_index] - nd20_expected_abs[9]) * 100.0 / nd20_expected_abs[9];
#endif

    // Now calculate band pass and band block % crosstalk
    // First step is to load in averages used to calculated these values
    // into the final result arrays before processing.
    for (i = 0; i <  WAVENUM; i++)  {
        /* filter used */
        for (j = 0; j < WAVENUM; j++)  {
            /* wavelength read */
            current_index = MAIN_DATA_INDEX + band_pass_cuvs[i] + (pass_list[j] * CUVNUM);
            band_pass_avgs[i][j] = trimmedData[current_index].averages[channel_list[j]] - average_offsets[int2rep[j]];

            current_index = MAIN_DATA_INDEX + band_block_cuvs[i] + (pass_list[j] * CUVNUM);
            band_block_avgs[i][j] = trimmedData[current_index].averages[channel_list[j]] - average_offsets[int2rep[j]];
        }
    }

    for (i = 0; i < WAVENUM; i++) {
            /* filter used */
        if (band_pass_avgs[i][i] == 0.0) {
            LogError ("DIVIDE_BY_ZERO_7_ERROR");
            return DIVIDE_BY_ZERO_7_ERROR;
        }
        for (j = 0; j < WAVENUM; j++) {
            /* wavelength read */
            if (average_opticals[int2rep[j]] == 0) {
                LogError ("DIVIDE_BY_ZERO_8_ERROR");
                return DIVIDE_BY_ZERO_8_ERROR;
            }
            band_pass[i][j] = (band_pass_avgs[i][j] / average_opticals[int2rep[j]]) / (band_pass_avgs[i][i] / average_opticals[int2rep[i]]);
            band_block[i][j] = band_block_avgs[i][j] / average_opticals[int2rep[j]];
        }
    }

    return NO_ERRORS;
}


 //  right_justify()
 //  Right justifies the string passed by padding blanks on the left
 //  until length of string matches passed parameter.
void 
right_justify (char *target_string, unsigned char field_size)
{
    char build_string[20], pad, i, *b_ptr;

    b_ptr = build_string;
    if (strlen(target_string) < field_size)
    {
        pad = field_size - strlen(target_string);
        for (i = 0; i != pad; ++i) {
            *b_ptr++ = ' ';
        }
        *b_ptr = '\0';
        strcat (build_string,target_string);
        strcpy (target_string,build_string);
    }

}


// generate_report()
// Generates the report for the file, sets test_failed if it did.
AmsNdxtTestErrorCode_t
StateAmsNdxtTestResultsCal::
GenerateReport (void)
{
    int i, j, k;
    double   scaled_counts,dac_level,xt_limit;
    double   abs_val, signed_val, *pexpect, *pmeas, limit;
    const   char *label_ptr;
    char    field_string[20], line_string[100], *ls_ptr;
    char    *reportStr, *pBuff ;
    int     strLen;
	FILE	*fp;

    // alloc report string memory
    reportStr = (char *)malloc (MAX_NDXT_REPORT_STR_LENGTH);
    if (reportStr == NULL) {
        LogError ("Fail to allocate NDXT memory");
        return MEM_ALLOC_ERROR;
    }
    pBuff = reportStr;

    // Title for first section (DAC/BLACK reading statistics)
    sprintf (pBuff ,"WAVE DAC   OFFSET  STD    CV     OPT    SCALED  | NORMALIZED:  CV    RANGE\r\n");
    pBuff += strlen (pBuff);
    // Note: the C formatters don't right justify properly, so each value
    // is converted into a string, then left-padded to fit a fixed field
    // width, then appended to the string to be output to the log file.
    for (i = 0; i < 9; i++) {
        label_ptr = wavelength_labels[i];
        dac_level = dac_settings[i];

        ls_ptr = line_string;
        line_string[0] = '\0';

        sprintf (ls_ptr,"%s",label_ptr);
        sprintf (field_string,"%3.0f",dac_level);
        right_justify (field_string, 3);
        strcat(line_string,field_string);
        if (dac_level < MIN_DAC_SETTING || dac_level > MAX_DAC_SETTING) {
            if (dac_level < MIN_DAC_SETTING) {
                LogError ("MIN_DAC_SETTING:dac_level=%d", dac_level);
                finalReportError = DAC_SETTING_MIN_LEVEL_ERR;
            }  else {
                LogError ("MAX_DAC_SETTING:dac_level=%d", dac_level);
                finalReportError = DAC_SETTING_MAX_LEVEL_ERR;
            }
            strcat(line_string,FLAGGED_FIELD);
            test_failed = TRUE;
        } else {
            strcat(line_string,NO_FLAG);
        }

        sprintf (field_string,"%4.1f",average_offsets[int2rep[i]]);
        right_justify (field_string,7);
        strcat(line_string,field_string);
        if (average_offsets[int2rep[i]] < MIN_OFFSET || average_offsets[int2rep[i]] > manufacturingTestSet.maxBlackOffset) {
            if (average_offsets[int2rep[i]] < MIN_OFFSET) {
                finalReportError = AVG_MIN_OFFSET_ERR;
                //LogError ("MIN_OFFSET_ERR: average_offsets=%f", average_offsets[int2rep[i]]);
            } else {
                finalReportError = AVG_MAX_OFFSET_ERR;
                //LogError ("MAX_OFFSET_ERR: average_offsets=%f", average_offsets[int2rep[i]]);
                
            }
            test_failed = TRUE;
            strcat(line_string,FLAGGED_FIELD);
        } else {
            strcat(line_string,NO_FLAG);
        }

        sprintf(field_string,"%2.1f",offset_sds[int2rep[i]]);
        right_justify (field_string, 5);
        strcat (line_string,field_string);
        if (offset_sds[int2rep[i]] > MAX_OFFSET_SD) {
            finalReportError = MAX_OFFSET_SD_ERR;
            //LogError ("MAX_OFFSET_SD_ERR:offset_sds=%f",offset_sds[int2rep[i]]);
            test_failed = TRUE;
            strcat(line_string,FLAGGED_FIELD);
        } else {
            strcat(line_string,NO_FLAG);
        }

        sprintf(field_string,"%2.2f%%",optical_cvs[int2rep[i]]);
        right_justify (field_string, 7);
        strcat(line_string,field_string);
        if (optical_cvs[int2rep[i]] > MAX_CV) {
            finalReportError = MAX_CV_ERR;
            //LogError ("MAX_CV_ERR:optical_cvs=%f", optical_cvs[int2rep[i]]);
            test_failed = TRUE;
            strcat(line_string,FLAGGED_FIELD);
        } else {
            strcat(line_string,NO_FLAG);
        }

        sprintf(field_string,"%5.0f",average_opticals[int2rep[i]]);
        right_justify(field_string, 6);
        strcat(line_string,field_string);

        scaled_counts = (256.0 / (double) dac_settings[i]) * average_opticals[int2rep[i]];


        sprintf(field_string,"%6.0f",scaled_counts);
        right_justify (field_string, 8);
        strcat (line_string,field_string);
        if (scaled_counts < MIN_SCALED_COUNTS) {
            finalReportError = MIN_SCALED_COUNTS_ERR;
            LogError ("MIN_SCALED_COUNTS_ERR:scaled_counts=%f", scaled_counts);
            test_failed = TRUE;
            strcat(line_string,FLAGGED_FIELD);
        } else {
            strcat(line_string,NO_FLAG);
        }
        // Print normalized (referenced) values
        if (i != 8) {
            /* Don't ref 850 to 850! */
            label_ptr = refd_wavelength_labels[i];
            strcat (line_string,label_ptr);
            sprintf(field_string,"%2.2f%%",refd_cvs[int2rep[i]]);
            right_justify (field_string, 7);
            strcat (line_string,field_string);
            if (refd_cvs[int2rep[i]] > MAX_NORM_CV) {
                finalReportError = MAX_NORM_CV_ERR;
                LogError ("MAX_NORM_CV_ERR:refd_cvs=%f", refd_cvs[int2rep[i]]);
                test_failed = TRUE;
                strcat(line_string,FLAGGED_FIELD);
            } else {
                strcat(line_string,NO_FLAG);
            }

            sprintf(field_string,"%2.2f%%",refd_ranges[int2rep[i]]);
            right_justify(field_string, 7);
            strcat(line_string,field_string);

            sprintf(pBuff,"%s\r\n",line_string);
            pBuff += strlen (pBuff);
        } else {
            sprintf(pBuff,"%s\r\n\r\n",line_string);  /* Next section */
            pBuff += strlen (pBuff);
        }    
    }


    // for the absolute measured values of the ND filters
    sprintf(pBuff ,"NEUTRAL DENSITY FILTER: MEASURED VALUES\r\n");
    pBuff += strlen (pBuff);
    sprintf(pBuff,"FILTER   340      405      467      500      515      550      600      630      850\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff,"%s",nd_diff_labels[0]);
    pBuff += strlen (pBuff);
    for (i = 0; i < 9; i++) {
        sprintf(pBuff,"% 7.4f  ",nd05_abs[int2rep[i]]);
        pBuff += strlen (pBuff);
    }
    sprintf (pBuff,"\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff ,"%s",nd_diff_labels[1]);
    pBuff += strlen (pBuff);
    for (i = 0; i != 9; ++i) {
        sprintf(pBuff,"% 7.4f  ",nd10_abs[int2rep[i]]);
        pBuff += strlen (pBuff);
    }
    sprintf(pBuff,"\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff,"%s",nd_diff_labels[2]);
    pBuff += strlen (pBuff);
    for (i = 0; i < 9; i++) {
        sprintf(pBuff,"% 7.4f  ",nd20_abs[int2rep[i]]);
        pBuff += strlen (pBuff);
    }
    sprintf(pBuff,"\r\n\r\n");
    pBuff += strlen (pBuff);

    // Now for the % difference in absorbance for ND filters
    sprintf(pBuff,"NEUTRAL DENSITY FILTER % DIFFERENCE FROM EXPECTED ABSORBANCES:\n\r");
    pBuff += strlen (pBuff);
    sprintf(pBuff,"FILTER   340      405      467      500      515      550      600      630      850\r\n");
    pBuff += strlen (pBuff);

    for(k = 0; k < 3; k++) {
        switch (k) {
            case 0: 
                pexpect = nd05_expected_abs; 
                pmeas = nd05_abs; 
                limit = nd05_diff_limit; 
            break;

            case 1: 
                pexpect = nd10_expected_abs; 
                pmeas = nd10_abs; 
                limit = nd10_diff_limit; 
            break;

            case 2: 
                pexpect = nd20_expected_abs; 
                pmeas = nd20_abs; 
                limit = nd20_diff_limit; 
            break;

            default:
            break;
        }
        sprintf(pBuff,"%s",nd_diff_labels[k]);
        pBuff += strlen (pBuff);

        for (i = 0; i < 9; i++) { 
            signed_val  = (pmeas[int2rep[i]] - pexpect[i] ) * 100. / pexpect[i];
            abs_val = (signed_val<0.) ? -1. * signed_val : signed_val;
            if (abs_val >= limit) {
                finalReportError = ABS_VALUE_LARGE_ERR;
                //LogError ("ABS_VALUE_LARGE_ERR:abs_val =%f", abs_val);
                test_failed = TRUE;
                sprintf(pBuff,"% 7.2f%%*",signed_val);
                pBuff += strlen (pBuff);
            } else {
                sprintf(pBuff,"% 7.2f%% ",signed_val);
                pBuff += strlen (pBuff);
            }
        }
        sprintf(pBuff,"\r\n");
        pBuff += strlen (pBuff);
    }
    sprintf(pBuff,"\r\n");
    pBuff += strlen (pBuff);

    //Percent Crosstalk (Band Pass)
    sprintf (pBuff,"PERCENT CROSSTALK:  BAND PASS\r\n");
    pBuff += strlen (pBuff);
    sprintf (pBuff,"         340     405     467     500     515     550     600     630     850\r\n");
    pBuff += strlen (pBuff);

    for (i = 0; i < 9; i++) {
        label_ptr = wavelength_labels[i];
        sprintf (pBuff,"%s ",label_ptr);
        pBuff += strlen (pBuff);

        for (j = 0; j < 9; j++) {
            xt_limit = 0.05; /* default */
            if (j == 6 || j == 8)
            xt_limit = 0.1;  /* higher limit for 600 and 850 channel) */
            if (((j == 3) && (i == 4)) || ((i == 4) && (j == 3)))
            xt_limit = 1.5;  /* higher limit for 500x515 crosstalk */

            if ((i != j) && (band_pass[i][j] > xt_limit || band_pass[i][j] < -xt_limit)) {
                sprintf(pBuff ,"% 3.4f*",band_pass[i][j]);
                pBuff += strlen (pBuff);
                finalReportError = BAND_PASS_LIMIT_ERR;
                //LogError ("BAND_PASS_LIMIT_ERR:band_pass=%f", band_pass[i][j]);
                test_failed = TRUE;
            } else {
                sprintf(pBuff,"% 3.4f ",band_pass[i][j]);
                pBuff += strlen (pBuff);
            }
        }
        sprintf(pBuff,"\r\n");
        pBuff += strlen (pBuff);

    }
    sprintf(pBuff,"\r\n");
    pBuff += strlen (pBuff);

    //Percent Crosstalk (Band Block)
    sprintf(pBuff,"PERCENT CROSSTALK:  BAND BLOCK\n");
    pBuff += strlen (pBuff);
    sprintf(pBuff,"         340     405     467     500     515     550     600     630     850\r\n");
    pBuff += strlen (pBuff);

    for (i = 0; i < 9; i++) {
        label_ptr = wavelength_labels[i];
        sprintf(pBuff,"%s ",label_ptr);
        pBuff += strlen (pBuff);

        for (j = 0; j < 9; j++) {
            if ((i == j) && (band_block[i][j] > band_block_limits[i] || band_block[i][j] < -band_block_limits[i])) {
                sprintf(pBuff,"% 3.4f*",band_block[i][j]);
                pBuff += strlen (pBuff);
                finalReportError = BAND_BLOCK_LIMIT_ERR; 
                //LogError ("BAND_BLOCK_LIMIT_ERR, band_block=%f", band_block[i][j]);
                test_failed = TRUE;
            } else if (i == 0 && j == 8) {
                if (band_block[i][j] < MIN_BAND_BLOCK_SIGNAL) {
                    finalReportError = BAND_BLOCK_MIN_SIGNAL_ERR;
                    //LogError ("BAND_BLOCK_MIN_ERR: band_block=%f", band_block[i][j]);
                    sprintf(pBuff,"% 3.4f*",band_block[i][j]);
                    pBuff += strlen (pBuff);
                    test_failed = TRUE;
                }
                 // ZZZ
                else {
                    sprintf(pBuff,"% 3.4f ",band_block[i][j]);
                    pBuff += strlen (pBuff);
                }

            } else {
                sprintf(pBuff,"% 3.4f ",band_block[i][j]);
                pBuff += strlen (pBuff);
            }

        }
        sprintf(pBuff,"\r\n");
        pBuff += strlen (pBuff);
    }
    sprintf(pBuff,"\r\n");
    pBuff += strlen (pBuff);
    *pBuff = '\0';

    fp = fopen (AMS_NDXT_REPORT_FILE, "wb");
    if (fp == NULL) {
        LogError ("Fail to open file %s", AMS_NDXT_REPORT_FILE);
    } else {
        strLen =  fwrite (reportStr, strlen (reportStr), 1, fp);
        fclose (fp);
       // LogDebug ("NDXT report length =%d", strlen (reportStr));
    }
    // free memory
    free (reportStr);
    return NO_ERRORS;
}

// generate_report()
// Generates the report for the file, sets test_failed if it did.
AmsNdxtTestErrorCode_t
StateAmsNdxtTestResultsCal::
GenerateDisplayReport (void)
{
    int                 i, j, k;
    double              scaled_counts,dac_level,xt_limit;
    double              abs_val, signed_val, *pexpect, *pmeas, limit;
    const               char *label_ptr;
    char                field_string[20], line_string[100], *ls_ptr;
    char                *reportStr, *pBuff, *rBuff;
    wchar_t             *dBuff;
    int                 strLen;
	FILE	            *fp;
    SYSTEMTIME          time;

    // alloc report string memory
    reportStr = (char *)malloc (MAX_NDXT_REPORT_STR_LENGTH);
    if (reportStr == NULL) {
        LogError ("Fail to allocate NDXT memory");
        return MEM_ALLOC_ERROR;
    }
    pBuff = reportStr;
    dBuff = displayText;

    // get analyzer serial number
    sprintf (pBuff,"Analyzer Serial Number:%s\r\n", factoryData->instrumentSerialNumber);
    pBuff += strlen (pBuff);

    // get current time
    GetLocalTime(&time);
    sprintf (pBuff, "Test Date: %u/%u/%u Time: %u:%u:%u\r\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);

    pBuff += strlen (pBuff);

    // set default test results to be pass
    rBuff = pBuff;
    sprintf (pBuff, "NDXT Test Result: Pass\r\n\r\n");
    // record test results buffer point
    pBuff += strlen (pBuff);

    wsprintf (dBuff, L"Test Time: %u/%u/%u %u:%u:%u\r\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);

    LogDebug ("%s", WstrToCstr(dBuff));
    dBuff += wcslen (dBuff);



    // Title for first section (DAC/BLACK reading statistics)
    sprintf (pBuff ,"WAVE DAC   OFFSET  STD    CV     OPT    SCALED\r\n");
    pBuff += strlen (pBuff);

    pBuff += strlen (pBuff);
    for (i = 0; i < 9; i++) {
        label_ptr = wavelength_labels[i];
        dac_level = dac_settings[i];
        ls_ptr = line_string;
        line_string[0] = '\0';
        sprintf (ls_ptr,"%s", label_ptr);
        sprintf (field_string,"%3.0f",dac_level);
        right_justify (field_string, 3);
        strcat(line_string, field_string);
        if (dac_level < MIN_DAC_SETTING || dac_level > MAX_DAC_SETTING) {
            if (dac_level < MIN_DAC_SETTING) {
                //LogError ("MIN_DAC_SETTING:dac_level=%d", dac_level);
                finalReportError = DAC_SETTING_MIN_LEVEL_ERR;
            }  else {
                //LogError ("MAX_DAC_SETTING:dac_level=%d", dac_level);
                finalReportError = DAC_SETTING_MAX_LEVEL_ERR;
            }
            strcat(line_string,FLAGGED_FIELD);
            test_failed = TRUE;
        } else {
            strcat(line_string,NO_FLAG);
        }

        sprintf (field_string,"%4.1f",average_offsets[int2rep[i]]);
        right_justify (field_string, 7);
        strcat(line_string,field_string);
        if (average_offsets[int2rep[i]] < MIN_OFFSET || average_offsets[int2rep[i]] > manufacturingTestSet.maxBlackOffset) {
            if (average_offsets[int2rep[i]] < MIN_OFFSET) {
                finalReportError = AVG_MIN_OFFSET_ERR;
                //LogError ("MIN_OFFSET_ERR: average_offsets=%f", average_offsets[int2rep[i]]);
            } else {
                finalReportError = AVG_MAX_OFFSET_ERR;
                //LogError ("MAX_OFFSET_ERR: average_offsets=%f", average_offsets[int2rep[i]]);
                
            }
            test_failed = TRUE;
            strcat(line_string,FLAGGED_FIELD);
        } else {
            strcat(line_string,NO_FLAG);
        }

        sprintf(field_string,"%2.1f",offset_sds[int2rep[i]]);
        right_justify (field_string, 5);
        strcat (line_string,field_string);
        if (offset_sds[int2rep[i]] > MAX_OFFSET_SD) {
            finalReportError = MAX_OFFSET_SD_ERR;
            //LogError ("MAX_OFFSET_SD_ERR:offset_sds=%f",offset_sds[int2rep[i]]);
            test_failed = TRUE;
            strcat(line_string,FLAGGED_FIELD);
        } else {
            strcat(line_string,NO_FLAG);
        }

        sprintf(field_string,"%2.2f%%",optical_cvs[int2rep[i]]);
        right_justify (field_string, 7);
        strcat(line_string,field_string);
        if (optical_cvs[int2rep[i]] > MAX_CV) {
            finalReportError = MAX_CV_ERR;
            //LogError ("MAX_CV_ERR:optical_cvs=%f", optical_cvs[int2rep[i]]);
            test_failed = TRUE;
            strcat(line_string,FLAGGED_FIELD);
        } else {
            strcat(line_string,NO_FLAG);
        }

        sprintf(field_string,"%5.0f",average_opticals[int2rep[i]]);
        right_justify(field_string, 6);
        strcat(line_string,field_string);

        scaled_counts = (256.0 / (double) dac_settings[i]) * average_opticals[int2rep[i]];


        sprintf(field_string,"%6.0f",scaled_counts);
        right_justify (field_string, 8);
        strcat (line_string,field_string);
        if (scaled_counts < MIN_SCALED_COUNTS) {
            finalReportError = MIN_SCALED_COUNTS_ERR;
            //LogError ("MIN_SCALED_COUNTS_ERR:scaled_counts=%f", scaled_counts);
            test_failed = TRUE;
            strcat(line_string,FLAGGED_FIELD);
        } else {
            strcat(line_string,NO_FLAG);
        }
        sprintf(pBuff,"%s\r\n",line_string);  /* Next section */
        pBuff += strlen (pBuff);

    }
    sprintf (pBuff,"\r\n");
    pBuff += strlen (pBuff);
    sprintf (pBuff ,"WAVE   NORMALIZED:  CV    RANGE\r\n");
    pBuff += strlen (pBuff);
    for (i = 0; i < 9; i++) {
        label_ptr = wavelength_labels[i];
        ls_ptr = line_string;
        line_string[0] = '\0';
        sprintf (ls_ptr,"%s", label_ptr);
        if (i != 8) {
            label_ptr = refd_wavelength_labels[i];
            strcat (line_string,label_ptr);
             sprintf(field_string,"%2.2f%%",refd_cvs[int2rep[i]]);
            right_justify (field_string, 7);
            strcat (line_string,field_string);
            if (refd_cvs[int2rep[i]] > MAX_NORM_CV) {
                finalReportError = MAX_NORM_CV_ERR;
                //LogError ("MAX_NORM_CV_ERR:refd_cvs=%f", refd_cvs[int2rep[i]]);
                test_failed = TRUE;
                strcat(line_string,FLAGGED_FIELD);
            } else {
                strcat(line_string,NO_FLAG);
            }

            sprintf(field_string,"%2.2f%%",refd_ranges[int2rep[i]]);
            right_justify(field_string, 7);
            strcat(line_string,field_string);
            sprintf(pBuff,"%s\r\n",line_string);
            pBuff += strlen (pBuff);


        }
    }
    sprintf (pBuff,"\r\n");
    pBuff += strlen (pBuff);
    // for the absolute measured values of the ND filters
    sprintf(pBuff ,"NEUTRAL DENSITY FILTER: MEASURED VALUES\r\n");
    pBuff += strlen (pBuff);


    sprintf(pBuff,"FILTER    340      405      467      500      515\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff,"%s",nd_diff_labels[0]);
    pBuff += strlen (pBuff);
    for (i = 0; i < 5; i++) {
        sprintf(pBuff,"% 7.4f  ",nd05_abs[int2rep[i]]);
        pBuff += strlen (pBuff);
    }
    sprintf (pBuff,"\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff ,"%s",nd_diff_labels[1]);
    pBuff += strlen (pBuff);
    for (i = 0; i < 5; ++i) {
        sprintf(pBuff,"% 7.4f  ",nd10_abs[int2rep[i]]);
        pBuff += strlen (pBuff);
    }
    sprintf(pBuff,"\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff,"%s",nd_diff_labels[2]);
    pBuff += strlen (pBuff);
    for (i = 0; i < 5; i++) {
        sprintf(pBuff,"% 7.4f  ",nd20_abs[int2rep[i]]);
        pBuff += strlen (pBuff);
    }
    sprintf(pBuff,"\r\n\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff,"FILTER    550      600      630      850\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff,"%s",nd_diff_labels[0]);
    pBuff += strlen (pBuff);
    for (i = 5; i < 9; i++) {
        sprintf(pBuff,"% 7.4f  ",nd05_abs[int2rep[i]]);
        pBuff += strlen (pBuff);
    }
    sprintf (pBuff,"\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff ,"%s",nd_diff_labels[1]);
    pBuff += strlen (pBuff);
    for (i = 5; i < 9; ++i) {
        sprintf(pBuff,"% 7.4f  ",nd10_abs[int2rep[i]]);
        pBuff += strlen (pBuff);
    }
    sprintf(pBuff,"\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff,"%s",nd_diff_labels[2]);
    pBuff += strlen (pBuff);
    for (i = 5; i < 9; i++) {
        sprintf(pBuff,"% 7.4f  ",nd20_abs[int2rep[i]]);
        pBuff += strlen (pBuff);
    }
    sprintf(pBuff,"\r\n\r\n");
    pBuff += strlen (pBuff);

    // Now for the % difference in absorbance for ND filters
    sprintf(pBuff,"NEUTRAL DENSITY FILTER DIFFERENCE FROM EXPECTED ABSORBANCES\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff,"FILTER     340      405      467      500      515\r\n");
    pBuff += strlen (pBuff);

    for(k = 0; k < 3; k++) {
        switch (k) {
            case 0: 
                pexpect = nd05_expected_abs; 
                pmeas = nd05_abs; 
                limit = nd05_diff_limit; 
            break;

            case 1: 
                pexpect = nd10_expected_abs; 
                pmeas = nd10_abs; 
                limit = nd10_diff_limit; 
            break;

            case 2: 
                pexpect = nd20_expected_abs; 
                pmeas = nd20_abs; 
                limit = nd20_diff_limit; 
            break;

            default:
            break;
        }
        sprintf(pBuff,"%s",nd_diff_labels[k]);
        pBuff += strlen (pBuff);

        for (i = 0; i < 5; i++) { 
            signed_val  = (pmeas[int2rep[i]] - pexpect[i] ) * 100. / pexpect[i];
            abs_val = (signed_val<0.) ? -1. * signed_val : signed_val;
            if (abs_val >= limit) {
                finalReportError = ABS_VALUE_LARGE_ERR;
                //LogError ("ABS_VALUE_LARGE_ERR:abs_val =%f", abs_val);
                test_failed = TRUE;
                sprintf(pBuff,"% 7.2f%%*",signed_val);
                pBuff += strlen (pBuff);
            } else {
                sprintf(pBuff,"% 7.2f%% ",signed_val);
                pBuff += strlen (pBuff);
            }
        }
        sprintf(pBuff,"\r\n");
        pBuff += strlen (pBuff);
    }
    sprintf(pBuff,"\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff,"FILTER     550      600      630      850\r\n");
    pBuff += strlen (pBuff);

    for(k = 0; k < 3; k++) {
        switch (k) {
            case 0: 
                pexpect = nd05_expected_abs; 
                pmeas = nd05_abs; 
                limit = nd05_diff_limit; 
            break;

            case 1: 
                pexpect = nd10_expected_abs; 
                pmeas = nd10_abs; 
                limit = nd10_diff_limit; 
            break;

            case 2: 
                pexpect = nd20_expected_abs; 
                pmeas = nd20_abs; 
                limit = nd20_diff_limit; 
            break;

            default:
            break;
        }
        sprintf(pBuff,"%s",nd_diff_labels[k]);
        pBuff += strlen (pBuff);

        for (i = 5; i < 9; i++) { 
            signed_val  = (pmeas[int2rep[i]] - pexpect[i] ) * 100. / pexpect[i];
            abs_val = (signed_val<0.) ? -1. * signed_val : signed_val;
            if (abs_val >= limit) {
                finalReportError = ABS_VALUE_LARGE_ERR;
                //LogError ("ABS_VALUE_LARGE_ERR:abs_val =%f", abs_val);
                test_failed = TRUE;
                sprintf(pBuff,"% 7.2f%%*",signed_val);
                pBuff += strlen (pBuff);
            } else {
                sprintf(pBuff,"% 7.2f%% ",signed_val);
                pBuff += strlen (pBuff);
            }
        }
        sprintf(pBuff,"\r\n");
        pBuff += strlen (pBuff);
    }
    sprintf(pBuff,"\r\n");
    pBuff += strlen (pBuff);

    //Percent Crosstalk (Band Pass)
    sprintf (pBuff,"PERCENT CROSSTALK:  BAND PASS\r\n");
    pBuff += strlen (pBuff);

    sprintf (pBuff,"         340     405     467     500     515\r\n");
    pBuff += strlen (pBuff);

    for (i = 0; i < 9; i++) {
        label_ptr = wavelength_labels[i];
        sprintf (pBuff,"%s ",label_ptr);
        pBuff += strlen (pBuff);

        for (j = 0; j < 5; j++) {
            xt_limit = 0.05; /* default */
            if (j == 6 || j == 8)
            xt_limit = 0.1;  /* higher limit for 600 and 850 channel) */
            if (((j == 3) && (i == 4)) || ((i == 4) && (j == 3)))
            xt_limit = 1.5;  /* higher limit for 500x515 crosstalk */

            if ((i != j) && (band_pass[i][j] > xt_limit || band_pass[i][j] < -xt_limit)) {
                sprintf(pBuff ,"% 3.4f*",band_pass[i][j]);
                pBuff += strlen (pBuff);
                finalReportError = BAND_PASS_LIMIT_ERR;
                //LogError ("BAND_PASS_LIMIT_ERR:band_pass=%f", band_pass[i][j]);
                test_failed = TRUE;
            } else {
                sprintf(pBuff,"% 3.4f ",band_pass[i][j]);
                pBuff += strlen (pBuff);
            }
        }
        sprintf(pBuff,"\r\n");
        pBuff += strlen (pBuff);

    }
    sprintf(pBuff,"\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff,"         550     600     630     850\r\n");
    pBuff += strlen (pBuff);

    for (i = 0; i < 9; i++) {
        label_ptr = wavelength_labels[i];
        sprintf (pBuff,"%s ",label_ptr);
        pBuff += strlen (pBuff);

        for (j = 5; j < 9; j++) {
            xt_limit = 0.05; /* default */
            if (j == 6 || j == 8)
            xt_limit = 0.1;  /* higher limit for 600 and 850 channel) */
            if (((j == 3) && (i == 4)) || ((i == 4) && (j == 3)))
            xt_limit = 1.5;  /* higher limit for 500x515 crosstalk */

            if ((i != j) && (band_pass[i][j] > xt_limit || band_pass[i][j] < -xt_limit)) {
                sprintf(pBuff ,"% 3.4f*",band_pass[i][j]);
                pBuff += strlen (pBuff);
                finalReportError = BAND_PASS_LIMIT_ERR;
                //LogError ("BAND_PASS_LIMIT_ERR:band_pass=%f", band_pass[i][j]);
                test_failed = TRUE;
            } else {
                sprintf(pBuff,"% 3.4f ",band_pass[i][j]);
                pBuff += strlen (pBuff);
            }
        }
        sprintf(pBuff,"\r\n");
        pBuff += strlen (pBuff);

    }
    sprintf(pBuff,"\r\n");
    pBuff += strlen (pBuff);

    //Percent Crosstalk (Band Block)
    sprintf(pBuff,"PERCENT CROSSTALK:  BAND BLOCK\r\n");
    pBuff += strlen (pBuff);

    sprintf(pBuff,"         340     405     467     500     515\r\n");
    pBuff += strlen (pBuff);

    for (i = 0; i < 9; i++) {
        label_ptr = wavelength_labels[i];
        sprintf(pBuff,"%s ",label_ptr);
        pBuff += strlen (pBuff);

        for (j = 0; j < 5; j++) {
            if ((i == j) && (band_block[i][j] > band_block_limits[i] || band_block[i][j] < -band_block_limits[i])) {
                sprintf(pBuff,"% 3.4f*",band_block[i][j]);
                pBuff += strlen (pBuff);
                finalReportError = BAND_BLOCK_LIMIT_ERR; 
                //LogError ("BAND_BLOCK_LIMIT_ERR, band_block=%f", band_block[i][j]);
                test_failed = TRUE;
            } else if (i == 0 && j == 8) {
                if (band_block[i][j] < MIN_BAND_BLOCK_SIGNAL) {
                    finalReportError = BAND_BLOCK_MIN_SIGNAL_ERR;
                    //LogError ("BAND_BLOCK_MIN_ERR: band_block=%f", band_block[i][j]);
                    sprintf(pBuff,"% 3.4f*",band_block[i][j]);
                    pBuff += strlen (pBuff);
                    test_failed = TRUE;
                }
            } else {
                sprintf(pBuff,"% 3.4f ",band_block[i][j]);
                pBuff += strlen (pBuff);
            }

        }
        sprintf(pBuff,"\r\n");
        pBuff += strlen (pBuff);
    }
    sprintf (pBuff,"\r\n");
    pBuff += strlen (pBuff);
    sprintf(pBuff,"         550     600     630     850\r\n");
    pBuff += strlen (pBuff);

    for (i = 0; i < 9; i++) {
        label_ptr = wavelength_labels[i];
        sprintf(pBuff,"%s ",label_ptr);
        pBuff += strlen (pBuff);

        for (j = 5; j < 9; j++) {
            if ((i == j) && (band_block[i][j] > band_block_limits[i] || band_block[i][j] < -band_block_limits[i])) {
                sprintf(pBuff,"% 3.4f*",band_block[i][j]);
                pBuff += strlen (pBuff);
                finalReportError = BAND_BLOCK_LIMIT_ERR; 
                //LogError ("BAND_BLOCK_LIMIT_ERR, band_block=%f", band_block[i][j]);
                test_failed = TRUE;
            } else if (i == 0 && j == 8) {
                if (band_block[i][j] < MIN_BAND_BLOCK_SIGNAL) {
                    finalReportError = BAND_BLOCK_MIN_SIGNAL_ERR;
                    //LogError ("BAND_BLOCK_MIN_ERR: band_block=%f", band_block[i][j]);
                    sprintf(pBuff,"% 3.4f*",band_block[i][j]);
                    pBuff += strlen (pBuff);
                    test_failed = TRUE;
                } 
                // ZZZ
                else {
                    sprintf(pBuff,"% 3.4f ",band_block[i][j]);
                    pBuff += strlen (pBuff);
                }
            } else {
                sprintf(pBuff,"% 3.4f ",band_block[i][j]);
                pBuff += strlen (pBuff);
            }

        }
        sprintf(pBuff,"\r\n");
        pBuff += strlen (pBuff);
    }
    sprintf(pBuff,"\r\n");
    pBuff += strlen (pBuff);
    *pBuff = '\0';


    // format test results display
    if (test_failed == FALSE) {
        swprintf (dBuff, L"NDXT Test Result: Pass");
        LogDebug ("%s", WstrToCstr(dBuff));
        dBuff += wcslen (dBuff);
    } else {
        wsprintf (dBuff, L"NDXT Test Result: Fail");
        LogDebug ("%s", WstrToCstr(dBuff));
        dBuff += wcslen (dBuff);
        wsprintf (dBuff, L"\r\n\r\n");
        dBuff += wcslen (dBuff);
        if (finalReportError < MAX_REPORT_ERR) {
            wsprintf (dBuff,  CstrToWstr (ndxtFinalErrorReport [finalReportError].str));
            LogDebug ("%s", WstrToCstr(dBuff));
            dBuff += wcslen (dBuff);
            // insert test results based on above calculation
            sprintf (rBuff, "NDXT Test Result: Fail");
            rBuff +=  strlen (rBuff);
            *rBuff ='\r\n';
        }
    }
    // save the file to USB port
    SaveTestResultsToUsb (AMS_NDXT_TEST_REPORT_FILE, reportStr);

    fp = fopen (AMS_NDXT_DISP_REPORT_FILE, "wb");
    if (fp == NULL) {
        LogError ("Fail to open file %s", AMS_NDXT_REPORT_FILE);
    } else {
        strLen =  fwrite (reportStr, strlen (reportStr), 1, fp);
        //LogDebug ("strlen =%d", strlen (reportStr));
        fclose (fp);
        //LogDebug ("NDXT display report length =%d", strlen (reportStr));
    }

    // free memory
    free (reportStr);

    return NO_ERRORS;
}

// calculate and check NDXT test data 
AmsNdxtTestErrorCode_t
StateAmsNdxtTestResultsCal::
NdxtTestDataCheck (RawData_t *rawData)
{
    // reset test fail flag
    test_failed = 0;
    // set index = 0;
    trimmedDataIndex = 0;

    nd05_expected_abs = manufacturingTestSet.nd05ExpectedAbs;
    nd10_expected_abs = manufacturingTestSet.nd10ExpectedAbs; 
    nd20_expected_abs = manufacturingTestSet.nd20ExpectedAbs;
    band_block_limits = manufacturingTestSet.bandBlockLimits;
    nd05_diff_limit    = manufacturingTestSet.nd05DiffLimit;
    nd10_diff_limit    = manufacturingTestSet.nd10DiffLimit;   
    nd20_diff_limit    = manufacturingTestSet.nd20DiffLimit;


    finalReportError = NO_ERROR_REPORT;
    // first get optical check data 
    GetOpticalDacGroupData (rawData);
    // second get main group data 
    GetMainGroupData (rawData);
    // final get trim dac values
    GetTrimDacValue (&calibrationData->wavelengthDacTrims[0]);
    // calculation
    CrunchData ();

    return NO_ERRORS;
    
}

void
StateAmsNdxtTestResultsCal::
FormatTestReport (void)
{
    // format report 
    GenerateReport ();
    GenerateDisplayReport ();
}

