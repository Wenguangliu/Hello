// ********************************************************************
//
// FILENAME:  
// 
// 		CalcBfdData.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Constants used in BFD calculations
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>


#include "CalcBfdData.h"


const BfdFactorIndices_t bfdFactorIndices[ MAX_BFD_CALS ] = 
{
	{ NADH_CURVE_IDX,   WL_340_FILTER_IDX, NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 0:  NADH    340
	{ PNP_CURVE_IDX,    WL_405_FILTER_IDX, NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 1:  PNP     405
	{ ALB_B_CURVE_IDX,  WL_600_FILTER_IDX, ALB_A_CURVE_IDX,  WL_550_FILTER_IDX }, // BFD factor 2:  ALB_BCP 600/550
	{ ALB_B_CURVE_IDX,  WL_600_FILTER_IDX, ALB_C_CURVE_IDX,  WL_630_FILTER_IDX }, // BFD factor 3:  ALB_BCP 600/630
	{ ALB_F_CURVE_IDX,  WL_630_FILTER_IDX, ALB_D_CURVE_IDX,  WL_405_FILTER_IDX }, // BFD factor 4:  ALB_BCG 630/405
	{ TP_A_CURVE_IDX,   WL_550_FILTER_IDX, TP_B_CURVE_IDX,   WL_850_FILTER_IDX }, // BFD factor 5:  TP      550/850
	{ HDL_A_CURVE_IDX,  WL_550_FILTER_IDX, HDL_B_CURVE_IDX,  WL_630_FILTER_IDX }, // BFD factor 6:  HDL     550/630
	{ TRIG_CURVE_IDX,   WL_500_FILTER_IDX, NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 7:  TRIG    500
	{ ALB_F_CURVE_IDX,  WL_630_FILTER_IDX, ALB_E_CURVE_IDX,  WL_500_FILTER_IDX }, // BFD factor 8:  ALB_BCG 630/500
	{ NA_CURVE_IDX,     WL_405_FILTER_IDX, NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 9:  NA      405
	{ BA_A_CURVE_IDX,   WL_405_FILTER_IDX, BA_B_CURVE_IDX,   WL_500_FILTER_IDX }, // BFD factor 10: BA      405/500
	{ NO_DYE_IDX,       NO_FILTER_IDX,     NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 11: SPARE - NOT USED
	{ NO_DYE_IDX,       NO_FILTER_IDX,     NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 12: SPARE - NOT USED
	{ TBIL_A_CURVE_IDX, WL_467_FILTER_IDX, TBIL_B_CURVE_IDX, WL_550_FILTER_IDX }, // BFD factor 13: TBIL    467/550
	{ TBIL_A_CURVE_IDX, WL_467_FILTER_IDX, NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 14: TBIL    467 (for HDL)
	{ NO_DYE_IDX,       NO_FILTER_IDX,     NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 15: SPARE - NOT USED
	{ NO_DYE_IDX,       NO_FILTER_IDX,     NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 16: SPARE - NOT USED
	{ CRE_A_CURVE_IDX,  WL_550_FILTER_IDX, CRE_B_CURVE_IDX,  WL_600_FILTER_IDX }, // BFD factor 17: CRE     550/600
	{ NO_DYE_IDX,       NO_FILTER_IDX,     NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 18: SPARE - NOT USED
	{ NO_DYE_IDX,       NO_FILTER_IDX,     NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 19: SPARE - NOT USED
	{ NO_DYE_IDX,       NO_FILTER_IDX,     NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 20: SPARE - NOT USED
	{ AMY_A_CURVE_IDX,  WL_405_FILTER_IDX, AMY_B_CURVE_IDX,  WL_500_FILTER_IDX }, // BFD factor 21: AMY     405/500
	{ LDH_A_CURVE_IDX,  WL_500_FILTER_IDX, LDH_B_CURVE_IDX,  WL_630_FILTER_IDX }, // BFD factor 22: LDH     500/630
	{ CA_C_CURVE_IDX,   WL_600_FILTER_IDX, NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 23: CA      600
	{ UA_A_CURVE_IDX,   WL_515_FILTER_IDX, UA_B_CURVE_IDX,   WL_600_FILTER_IDX }, // BFD factor 24: UA      515/600
	{ NO_DYE_IDX,       NO_FILTER_IDX,     NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 25: SPARE - NOT USED
	{ NO_DYE_IDX,       NO_FILTER_IDX,     NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 26: SPARE - NOT USED
	{ GGT_A_CURVE_IDX,  WL_405_FILTER_IDX, GGT_B_CURVE_IDX,  WL_500_FILTER_IDX }, // BFD factor 27: GGT     405/500
	{ CA_A_CURVE_IDX,   WL_405_FILTER_IDX, NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 28: CA      405
	{ CA_B_CURVE_IDX,   WL_467_FILTER_IDX, NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 29: CA      467
	{ NO_DYE_IDX,       NO_FILTER_IDX,     NO_DYE_IDX,       NO_FILTER_IDX     }, // BFD factor 30: SPARE - NOT USED
	{ NO_DYE_IDX,       NO_FILTER_IDX,     NO_DYE_IDX,       NO_FILTER_IDX     }  // BFD factor 31: SPARE - NOT USED
};

const float	bfdDyeCurves[ MAX_DYE_CURVES ][ MAX_BFD_POINTS ] =
{
	// -10nm   , -9nm    , -8nm    , -7nm    , -6nm    , -5nm    , -4nm    , -3nm    , -2nm    , -1nm    , CENTER  , +1nm    , +2nm    , +3nm    , +4nm    ,  +5nm   , +6nm    , +7nm    , +8nm    , +9nm    , +10nm       ( [idx] dye curve values around CENTER wavelength )   		

	{ 0.06544  ,0.06306  ,0.06076  ,0.05905  ,0.05738  ,0.05616  ,0.05497  ,0.05439  ,0.05381  ,0.05377  ,0.05372  ,0.05414  ,0.05456  ,0.05546  ,0.05638  ,0.05804  ,0.05975  ,0.06178  ,0.06388  ,0.06673  ,0.06972   }, // [0]  340nm: NADH_CURVE_IDX

	{ 0.09410  ,0.09254  ,0.09135  ,0.09018  ,0.08954  ,0.08889  ,0.08879  ,0.08869  ,0.08903  ,0.08937  ,0.09028  ,0.09120  ,0.09289  ,0.09462  ,0.09695  ,0.09933  ,0.10238  ,0.10552  ,0.10916  ,0.11292  ,0.11761   }, // [1]  405nm: PNP_CURVE_IDX
		
	{ 0.33675  ,0.33127  ,0.32588  ,0.32083  ,0.31585  ,0.30926  ,0.30281  ,0.29694  ,0.29119  ,0.28612  ,0.28113  ,0.27572  ,0.27041  ,0.26480  ,0.25930  ,0.25423  ,0.24927  ,0.24455  ,0.23993  ,0.23559  ,0.23133   }, // [2]  550nm: ALB_A_CURVE_IDX (BCP)

	{ 0.09239  ,0.08794  ,0.08370  ,0.08115  ,0.07868  ,0.07674  ,0.07486  ,0.07329  ,0.07176  ,0.07126  ,0.07077  ,0.07149  ,0.07223  ,0.07399  ,0.07579  ,0.07906  ,0.08246  ,0.08749  ,0.09282  ,0.09880  ,0.10516   }, // [3]  600nm: ALB_B_CURVE_IDX (BCP)

	{ 0.25013  ,0.27478  ,0.30187  ,0.32608  ,0.35223  ,0.38025  ,0.41050  ,0.43674  ,0.46465  ,0.49241  ,0.52183  ,0.55104  ,0.58188  ,0.60617  ,0.63147  ,0.65416  ,0.67766  ,0.69854  ,0.72007  ,0.74216  ,0.76493   }, // [4]  630nm: ALB_C_CURVE_IDX (BCP)

	{ 0.36813  ,0.36602  ,0.36392  ,0.36141  ,0.35892  ,0.35604  ,0.35318  ,0.35156  ,0.34995  ,0.34754  ,0.34514  ,0.34316  ,0.34119  ,0.33923  ,0.33729  ,0.33497  ,0.33266  ,0.33113  ,0.32961  ,0.32734  ,0.32509   }, // [5]  550nm: TP_A_CURVE_IDX

	{ 0.72946  ,0.73367  ,0.73790  ,0.73875  ,0.73961  ,0.74302  ,0.74645  ,0.74903  ,0.75162  ,0.75422  ,0.75683  ,0.76033  ,0.76384  ,0.76648  ,0.76913  ,0.77090  ,0.77268  ,0.77535  ,0.77804  ,0.78073  ,0.78343   }, // [6]  850nm: TP_B_CURVE_IDX

	{ 0.02211  ,0.02181  ,0.02187  ,0.02194  ,0.02205  ,0.02216  ,0.02214  ,0.02213  ,0.02252  ,0.02293  ,0.02350  ,0.02408  ,0.02517  ,0.02631  ,0.02743  ,0.02860  ,0.03002  ,0.03152  ,0.03354  ,0.03569  ,0.03854   }, // [7]  467nm: TBIL_A_CURVE_IDX

	{ 0.86298  ,0.86542  ,0.86786  ,0.87421  ,0.88060  ,0.87808  ,0.87557  ,0.87509  ,0.87462  ,0.88101  ,0.88744  ,0.89050  ,0.89357  ,0.89165  ,0.88973  ,0.89068  ,0.89162  ,0.89385  ,0.89609  ,0.89957  ,0.90307   }, // [8]  550nm: TBIL_B_CURVE_IDX

	{ 0.42801  ,0.43651  ,0.44519  ,0.45424  ,0.46348  ,0.47332  ,0.48337  ,0.49318  ,0.50319  ,0.51354  ,0.52409  ,0.53526  ,0.54666  ,0.55696  ,0.56746  ,0.57800  ,0.58873  ,0.59946  ,0.61040  ,0.62086  ,0.63149   }, // [9]  550nm: CRE_A_CURVE_IDX

	{ 0.83721  ,0.84235  ,0.84751  ,0.85090  ,0.85430  ,0.85747  ,0.86066  ,0.86484  ,0.86904  ,0.87244  ,0.87585  ,0.87924  ,0.88264  ,0.88500  ,0.88737  ,0.89051  ,0.89366  ,0.89518  ,0.89671  ,0.90161  ,0.90653   }, // [10] 600nm: CRE_B_CURVE_IDX

	{ 0.17526  ,0.17557  ,0.17593  ,0.17630  ,0.17745  ,0.17860  ,0.18008  ,0.18157  ,0.18342  ,0.18528  ,0.18797  ,0.19069  ,0.19431  ,0.19799  ,0.20240  ,0.20691  ,0.21232  ,0.21786  ,0.22451  ,0.23136  ,0.23920   }, // [11] 405nm: AMY_A_CURVE_IDX

	{ 0.97230  ,0.97208  ,0.97185  ,0.97403  ,0.97611  ,0.97478  ,0.97342  ,0.97307  ,0.97275  ,0.97435  ,0.97611  ,0.97588  ,0.97566  ,0.97601  ,0.97634  ,0.97674  ,0.97724  ,0.97687  ,0.97656  ,0.97579  ,0.97499   }, // [12] 500nm: AMY_B_CURVE_IDX

	{ 0.13474  ,0.13456  ,0.13434  ,0.13412  ,0.13412  ,0.13412  ,0.13434  ,0.13456  ,0.13493  ,0.13533  ,0.13589  ,0.13649  ,0.13712  ,0.13775  ,0.13820  ,0.13861  ,0.13896  ,0.13932  ,0.13941  ,0.13948  ,0.13954   }, // [13] 405nm: CA_A_CURVE_IDX

	{ 0.13951  ,0.13763  ,0.13580  ,0.13400  ,0.13195  ,0.12993  ,0.12811  ,0.12633  ,0.12462  ,0.12291  ,0.12123  ,0.11956  ,0.11792  ,0.11631  ,0.11458  ,0.11290  ,0.11133  ,0.10980  ,0.10812  ,0.10644  ,0.10459   }, // [14] 467nm: CA_B_CURVE_IDX

	{ 0.01165  ,0.01161  ,0.01157  ,0.01157  ,0.01158  ,0.01172  ,0.01187  ,0.01217  ,0.01249  ,0.01298  ,0.01349  ,0.01428  ,0.01510  ,0.01624  ,0.01747  ,0.01924  ,0.02119  ,0.02364  ,0.02638  ,0.02940  ,0.03278   }, // [15] 600nm: CA_C_CURVE_IDX

	{ 0.10476  ,0.10349  ,0.10252  ,0.10155  ,0.10097  ,0.10037  ,0.10007  ,0.09977  ,0.09988  ,0.10000  ,0.10044  ,0.10090  ,0.10170  ,0.10249  ,0.10373  ,0.10500  ,0.10664  ,0.10829  ,0.11026  ,0.11228  ,0.11474   }, // [16] 515nm: UA_A_CURVE_IDX

	{ 0.57610  ,0.58439  ,0.59265  ,0.59924  ,0.60576  ,0.61320  ,0.62073  ,0.62864  ,0.63665  ,0.64343  ,0.65028  ,0.65766  ,0.66512  ,0.67189  ,0.67873  ,0.68517  ,0.69167  ,0.69888  ,0.70632  ,0.71285  ,0.71945   }, // [17] 600nm: UA_B_CURVE_IDX

	{ 0.716514 ,0.714403 ,0.712548 ,0.710698 ,0.710136 ,0.709574 ,0.709849 ,0.710123 ,0.710460 ,0.710797 ,0.710435 ,0.710073 ,0.710997 ,0.711922 ,0.712835 ,0.713750 ,0.715244 ,0.716741 ,0.718468 ,0.720199 ,0.724539  }, // [18] 405nm: GGT_A_CURVE_IDX
		
	{ 0.899929 ,0.903002 ,0.906084 ,0.907980 ,0.909881 ,0.913612 ,0.917360 ,0.919586 ,0.921818 ,0.923261 ,0.924705 ,0.926917 ,0.929135 ,0.930817 ,0.932503 ,0.934799 ,0.937101 ,0.939046 ,0.940994 ,0.942351 ,0.943709  }, // [19] 500nm: GGT_B_CURVE_IDX

	{ 0.30128  ,0.29858  ,0.29587  ,0.29341  ,0.29095  ,0.28883  ,0.28670  ,0.28465  ,0.28260  ,0.28137  ,0.28014  ,0.27892  ,0.27769  ,0.27728  ,0.27687  ,0.27698  ,0.27710  ,0.27817  ,0.27924	,0.28052  ,0.28180 	 }, //	[20] 550nm: HDL_A_CURVE_IDX			

	{ 0.65849  ,0.66410  ,0.66970  ,0.67452  ,0.67934  ,0.68447  ,0.68960  ,0.69457  ,0.69953  ,0.70429  ,0.70905  ,0.71399  ,0.71892  ,0.72371  ,0.72850  ,0.73356  ,0.73862  ,0.74319  ,0.74776	,0.75196  ,0.75615 	 }, // [21] 630nm: HDL_B_CURVE_IDX				

	{ 0.064565 ,0.064434 ,0.064303 ,0.064257 ,0.064212 ,0.064060 ,0.063908 ,0.064219 ,0.064531 ,0.064831 ,0.065132 ,0.065355 ,0.065578 ,0.066166 ,0.066759 ,0.067233 ,0.067711 ,0.068293 ,0.068879 ,0.069686 ,0.070503	 }, // [22] 500nm: TRIG_CURVE_IDX				

	{ 0.36761  ,0.36465  ,0.36203  ,0.35943  ,0.35710  ,0.35478  ,0.35304  ,0.35131  ,0.34998  ,0.34865  ,0.34770  ,0.34674  ,0.34629  ,0.34584  ,0.34592  ,0.34599  ,0.34650  ,0.34701  ,0.34781	,0.34861  ,0.35014	  }, // [23] 405nm: ALB_D_CURVE_IDX (BCG)

	{ 0.65825  ,0.66001  ,0.66177  ,0.66344  ,0.66511  ,0.66644  ,0.66778  ,0.66853  ,0.66928  ,0.66994  ,0.67060  ,0.67070  ,0.67081  ,0.67042  ,0.67003  ,0.66954  ,0.66904  ,0.66796  ,0.66688	,0.66560  ,0.66431	  }, // [24] 500nm: ALB_E_CURVE_IDX (BCG)

	{ 0.20794  ,0.20679  ,0.20566  ,0.20487  ,0.20409  ,0.20394  ,0.20380  ,0.20398  ,0.20415  ,0.20485  ,0.20555  ,0.20703  ,0.20852  ,0.21036  ,0.21222  ,0.21462  ,0.21705  ,0.22002  ,0.22304	,0.22680  ,0.23062	  }, // [25] 630nm: ALB_F_CURVE_IDX (BCG)

	{ 0.11680  ,0.11371  ,0.11088  ,0.10812  ,0.10542  ,0.10279  ,0.10051  ,0.09828  ,0.09636  ,0.09447  ,0.09269  ,0.09095  ,0.08956  ,0.08820  ,0.08709  ,0.08600  ,0.08527  ,0.08454  ,0.08397  ,0.08339  ,0.08322	  }, // [26] 405nm: NA_CURVE_IDX

	{ 0.0499303,0.0493781,0.0488321,0.0485609,0.0482913,0.0478631,0.0474387,0.0473471,0.0472556,0.0471702,0.0470849,0.0472166,0.0473487,0.0474595,0.0475705,0.0479565,0.0483456,0.0486480,0.0489523,0.0495407,0.0501360	}, // [27] 500nm: LDH_A_CURVE_IDX				

	{ 0.713224 ,0.723407 ,0.733736 ,0.741979 ,0.750316 ,0.758773 ,0.767325 ,0.775224 ,0.783204 ,0.790572 ,0.798009 ,0.805219 ,0.812495 ,0.818886 ,0.825327 ,0.832345 ,0.839423 ,0.845149 ,0.850914	,0.856283	,0.861685		}, // [28] 630nm: LDH_B_CURVE_IDX			

	{ 0.370447 ,0.367452 ,0.365335 ,0.363229 ,0.362706 ,0.362184 ,0.362808 ,0.363433 ,0.364930 ,0.366434 ,0.369024 ,0.371633 ,0.375630 ,0.379670 ,0.384544 ,0.389479 ,0.395436 ,0.401484 ,0.408319	,0.415271	,0.423679		}, // [29] 405nm: BA_A_CURVE_IDX			

	{ 0.973309 ,0.973548 ,0.973788 ,0.974370 ,0.974952 ,0.974969 ,0.974986 ,0.975277 ,0.975569 ,0.975911 ,0.976254 ,0.976392 ,0.976529 ,0.976992 ,0.977456 ,0.977662 ,0.977868 ,0.978040 ,0.978211	,0.978263	,0.978315		}  // [30] 500nm: BA_B_CURVE_IDX			
};
