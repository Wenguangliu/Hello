/* Define header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

/* Internal Module Links */
extern const int ABI_winCriticalStop;
#define ABM_winCriticalStop                  &AbInternalLinks[ABI_winCriticalStop]
extern const int ABI_winMessage;
#define ABM_winMessage                       &AbInternalLinks[ABI_winMessage]
extern const int ABI_winLcd;
#define ABM_winLcd                           &AbInternalLinks[ABI_winLcd]
extern const int ABI_winBlack;
#define ABM_winBlack                         &AbInternalLinks[ABI_winBlack]

/* 'winCriticalStop' Window link */
extern const int ABN_winCriticalStop;
#define ABW_winCriticalStop                  AbGetABW( ABN_winCriticalStop )
extern const int ABN_ptRawCriticalStopTopBackground;
#define ABW_ptRawCriticalStopTopBackground   AbGetABW( ABN_ptRawCriticalStopTopBackground )
extern const int ABN_ptLabelCriticalStopTopLabel;
#define ABW_ptLabelCriticalStopTopLabel      AbGetABW( ABN_ptLabelCriticalStopTopLabel )
extern const int ABN_ptLabelCriticalStopMessage;
#define ABW_ptLabelCriticalStopMessage       AbGetABW( ABN_ptLabelCriticalStopMessage )
extern const int ABN_ptButtonCriticalStopInfo;
#define ABW_ptButtonCriticalStopInfo         AbGetABW( ABN_ptButtonCriticalStopInfo )
extern const int ABN_ptButtonCriticalStop;
#define ABW_ptButtonCriticalStop             AbGetABW( ABN_ptButtonCriticalStop )

/* 'winMessage' Window link */
extern const int ABN_winMessage;
#define ABW_winMessage                       AbGetABW( ABN_winMessage )
extern const int ABN_ptBkgdStartupMessage;
#define ABW_ptBkgdStartupMessage             AbGetABW( ABN_ptBkgdStartupMessage )
extern const int ABN_ptLabelMessage;
#define ABW_ptLabelMessage                   AbGetABW( ABN_ptLabelMessage )

/* 'winBlack' Window link */
extern const int ABN_winBlack;
#define ABW_winBlack                         AbGetABW( ABN_winBlack )

/* 'winLcd' Window link */
extern const int ABN_winLcd;
#define ABW_winLcd                           AbGetABW( ABN_winLcd )
extern const int ABN_ptBkgdStartup;
#define ABW_ptBkgdStartup                    AbGetABW( ABN_ptBkgdStartup )
extern const int ABN_ptLabelTitle;
#define ABW_ptLabelTitle                     AbGetABW( ABN_ptLabelTitle )
extern const int ABN_ptProgBar;
#define ABW_ptProgBar                        AbGetABW( ABN_ptProgBar )
extern const int ABN_ptTextProgBar;
#define ABW_ptTextProgBar                    AbGetABW( ABN_ptTextProgBar )

/* 'winErrorInfo' Window link */
extern const int ABN_winErrorInfo;
#define ABW_winErrorInfo                     AbGetABW( ABN_winErrorInfo )
extern const int ABN_ptBkgdStartupMessage0;
#define ABW_ptBkgdStartupMessage0            AbGetABW( ABN_ptBkgdStartupMessage0 )
extern const int ABN_ptButtonErrorInfo;
#define ABW_ptButtonErrorInfo                AbGetABW( ABN_ptButtonErrorInfo )
extern const int ABN_ptLabelErrorInfo;
#define ABW_ptLabelErrorInfo                 AbGetABW( ABN_ptLabelErrorInfo )
extern const int ABN_ptMultiTextErrorInfo;
#define ABW_ptMultiTextErrorInfo             AbGetABW( ABN_ptMultiTextErrorInfo )
extern const int ABN_ptButtonErrorInfoUp;
#define ABW_ptButtonErrorInfoUp              AbGetABW( ABN_ptButtonErrorInfoUp )
extern const int ABN_ptButtonErrorInfoDown;
#define ABW_ptButtonErrorInfoDown            AbGetABW( ABN_ptButtonErrorInfoDown )

/* 'winViewResults' Window link */
extern const int ABN_winViewResults;
#define ABW_winViewResults                   AbGetABW( ABN_winViewResults )
extern const int ABN_ptLabelViewResultsBkgd;
#define ABW_ptLabelViewResultsBkgd           AbGetABW( ABN_ptLabelViewResultsBkgd )
extern const int ABN_ptMultiTextViewResults;
#define ABW_ptMultiTextViewResults           AbGetABW( ABN_ptMultiTextViewResults )
extern const int ABN_ptButtonViewResultsDown;
#define ABW_ptButtonViewResultsDown          AbGetABW( ABN_ptButtonViewResultsDown )
extern const int ABN_ptButtonViewResultsUp;
#define ABW_ptButtonViewResultsUp            AbGetABW( ABN_ptButtonViewResultsUp )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

#define AB_OPTIONS "s:x:y:h:w:S:"

#if defined(__cplusplus)
}
#endif

