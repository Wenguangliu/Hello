#include <string.h>

#include "AStrings.h"
#include "LogErr.h"
#include "Shmem.h"

typedef struct
{
	StringId_t		id;
	const char *	str[ NUM_LANGUAGES ];
} StringArr_t;

StringArr_t	HUMAN_STRINGS[] = {
//											ENGLISH,								FRENCH,									GERMAN,									SPANISH,								ITALIAN, 								PORTUGUESE,								PORTUGUESE (BRAZIL)																								PORTUGUESE (BRAZIL)
	{ STR_BLANK						,	{	"",										"",										"",										"",										"",										"",										""	}	},

	{ STR_PRODUCT_NAME				,	{	"piccolo xpress",						"piccolo xpress",					 	"piccolo xpress",					 	"piccolo xpress",					 	"piccolo xpress",						"piccolo xpress",						"piccolo xpress"	}	},

// Top (Stings > 35 Characters Unlikely to Display Correctly) 
//											          11111111112222222222233333	          11111111112222222222233333	          11111111112222222222233333	          11111111112222222222233333	          11111111112222222222233333	          11111111112222222222233333	          11111111112222222222233333	
//											 12345678901234567890123456789012345 	 12345678901234567890123456789012345	 12345678901234567890123456789012345	 12345678901234567890123456789012345	 12345678901234567890123456789012345	 12345678901234567890123456789012345	 12345678901234567890123456789012345	
	{ TSTR_ADD_SPECIES				,	{	"Add Demographic",						"Ajouter �l�ments d�mogr.",				"Demographie hinzuf�gen",				"A�adir demograf�a",					"Aggiungi demografia",					"Adicionar demografia",					"Adicionar dados demogr�ficos"	}	},
	{ TSTR_ADMINISTRATOR			,	{	"Administrator",						"Administrateur",						"Administrator",						"Administrador",						"Amministratore",						"Administrador",						"Administrador"	}	},
	{ TSTR_ADMINISTRATOR_ID			,	{	"Administrator ID",						"ID administrateur",					"Administrator-ID",						"Id. del administrador",				"ID amministratore",					"ID do administrador",					"ID do administrador"	}	},
	{ TSTR_ADVANCED_SETTINGS		,	{	"Advanced Settings",					"Param�tres avanc�s",					"Erweiterte Einstellungen",				"Configuraci�n avanzada",				"Impostazioni avanzate",				"Configura��es avan�adas",				"Configura��es avan�adas"	}	},
	{ TSTR_ALERT_VOLUME				,	{	"Alert Volume",							"Alerte volume",						"Mengenalarm",							"Volumen de alerta",					"Volume allarme",						"Volume de alerta",						"Volume do alerta"	}	},
	{ TSTR_ALL_RESULTS_BY_DATE		,	{	"All Results Sorted by Date",			"Tous r�sultats tri�s par date",		"Alle Ergebnisse nach Datum",			"Resultados por fecha",					"Tutti i risultati ordinati per data",	"Todos os resultados por data",			"Todos resultados classif. por data"	}	},
	{ TSTR_ALL_RESULTS_BY_ID		,	{	"All Results Sorted by ID",				"Tous r�sultats tri�s par ID",			"Alle Ergebnisse nach ID",				"Resultados por Id.",					"Tutti i risultati ordinati per ID",	"Todos os resultados por ID",			"Todos resultados classif. por ID"	}	},
	{ TSTR_ALL_UNITS				,	{	"All Units",							"Toutes unit�s",						"Alle Einheiten",						"Todas la unidades",					"Tutte le unit�",						"Todas as unidades",					"Todas as unidades"	}	},
	{ TSTR_ALTERNATE_ID				,	{	"Alternate ID",							"Autre ID",								"ID �ndern",							"Id. suplente",							"ID alternativo",						"ID suplente",							"ID alternativo"	}	},
	{ TSTR_ANALYTE_OPTIONS			,	{	"Analyte Options", 						"Options � analyser",					"Analytoptionen",						"Opciones de analito", 					"Opzioni analita"						"Op��es de analito",					"Op��es de analito"	}	},
	{ TSTR_ANALYTES					,	{	"Analytes",								"Substances � analyser",				"Analyte",								"Analitos",								"Analiti",								"Analitos",								"Analitos"	}	},
	{ TSTR_ANALYZER_INFORMATION		,	{	"Analyzer Information",					"Information analyseur",				"Analyseger�t-Angaben",					"Informaci�n del analizador",			"Informazioni analizzatore",			"Informa��o do analisador",				"Informa��es do analisador"	}	},
	{ TSTR_ANALYZER_SETTINGS		,	{	"Analyzer Settings",					"Param�tres analyseur",					"Analyseger�t-Einstellungen",			"Configuraci�n del analizador",			"Impostazioni analizzatore",			"Configura��es do analisador",			"Configura��es do analisador"	}	},
	{ TSTR_ARCHIVE_REF_RANGES		,	{	"Archive Reference Ranges",				"Fourchettes de r�f. archives",			"Archiv-Referenzbereiche",				"Guardar intervalos de referencia",		"Range di riferimento in archivio",		"Guardar intervalos de refer�ncia",		"Arquivar intervalos de refer�ncia"	}	},
	{ TSTR_ASTM_ORDER_METHOD		,	{	"ASTM Order Method",					"M�thode d'ordre ASTM",					"ASTM: Anforderungsmethode",			"M�todo de pedido de ASTM",				"Metodo ordine ASTM",					"M�todo de pedido de ASTM",				"M�todo de solicita��o de ASTM"	}	},
	{ TSTR_ASTM_PHYSICAL_LAYER		,	{	"ASTM Physical Layer",					"Couche physique ASTM",					"ASTM: Physikalische Ebene",			"Nivel f�sico ASTM",					"Livello fisico ASTM",					"N�vel f�sico de ASTM",					"Camada f�sica de ASTM"	}	},
	{ TSTR_ASTM_TCPIP_SETTINGS		,	{	"ASTM TCP/IP Settings", 				"Param�tres TCP/IP ASTM",				"ASTM: TCP/IP-Einstellungen",			"Configuraci�n TCP/IP ASTM",			"Impostazioni TCP/IP ASTM",				"Configura��es de TCP/IP de ASTM",		"Configura��es de TCP/IP do ASTM"	}	},
	{ TSTR_BACKLIGHT_BRIGHTNESS		,	{	"Backlight Brightness",					"Lumi�re r�tro�clairage",				"Helligkeit Hintergrundbeleuchtung",	"Brillo de la luz de fondo",			"Luminosit� retroilluminazione",		"Brilho da luz de fundo",				"Brilho da luz de fundo"	}	},
	{ TSTR_CALIBRATE_SCREEN			,	{	"Calibrate Screen",						"Calibrer �cran",						"Bildschirm kalibrieren",				"Calibrar pantalla",					"Calibra schermo",						"Calibrar ecr�",						"Calibrar tela"	}	},
	{ TSTR_CDROM_UPDATE				,	{	"CD ROM Update",						"Mise � jour CD ROM",					"CD-ROM-Aktualisierung",				"Actualizaci�n CD-ROM",					"Aggiorna CD ROM",						"Atualiza��o do CD-ROM",				"Atualiza��o em CD ROM"	}	},
	{ TSTR_CHECK_TCPIP_SETTINGS		,	{	"Check TCP/IP Settings", 				"V�rifier les param�tres TCP/IP",		"TCP/IP-Einstellungen �berpr�fen",		"Comprobar configuraci�n TCP/IP",		"Verifica impostazioni TCP/IP",			"Verificar configura��es de TCP/IP",	"Verificar configura��es TCP/IP"	}	},
	{ TSTR_CLICK_VOLUME				,	{	"Click Volume",							"Volume clic",							"Menge anklicken",						"Pulsar volumen",						"Fai clic su Volume",					"Clique em Volume",						"Clicar em Volume"	}	},
	{ TSTR_CLINICAL_ARCHIVE			,	{	"Clinical Archive",						"Archive clinique",						"Klinisches Archiv",					"Archivo cl�nico",						"Archivio clinico",						"Ficheiro cl�nico",						"Arquivo cl�nico"	}	},
	{ TSTR_CONFIGURE_PRINTER		,	{	"Configure Printer",					"Configurer Imprimante",				"Drucker konfigurieren",				"Configurar impresora",					"Configura stampante",					"Configurar impressora",				"Configurar impressora"	}	},
	{ TSTR_CONFIRM					,	{	"Confirm",								"Confirmer",							"Best�tigen",							"Confirmar",							"Conferma",								"Confirmar",							"Confirmar"	}	},
	{ TSTR_CONFIRM_ORDER			,	{	"Confirm Order",						"Confirmer ordre",						"Anforderung best�tigen",				"Confirmar pedido",						"Conferma ordine",						"Confirmar pedido",						"Confirmar solicita��o"	}	},
	{ TSTR_CONFIRM_PASSWORD			,	{	"Confirm Password",						"Confirmer mot de passe",				"Passwort best�tigen",					"Confirmar contrase�a",					"Conferma password",					"Confirmar palavra-passe",				"Confirmar senha"	}	},
	{ TSTR_CONTROL					,	{	"Control:",								"Contr�le :",							"Kontrolle:",							"Control:",								"Controllo:",							"Controlo:",							"Controle:"	}	},
	{ TSTR_CONTROL_BY_DATE			,	{	"Control Results Sorted by Date",		"Contr�ler r�sultats tri�s par date",	"Ergebnisse nach Dat. �berpr�fen",		"Resultados control por fecha",			"Controlla risult. ordinati per data",	"Resultados de controlo por data",		"Result. controle classif. por data"	}	},
	{ TSTR_CONTROL_EXPIRATION_DATE	,	{	"Control Expiration Date",				"Contr�ler date de p�remption",			"Ablaufdatum �berpr�fen",				"Fecha de vencimiento de control",		"Controlla data di scadenza",			"Prazo de validade do controlo",		"Data de expira��o do controle"	}	},
	{ TSTR_CONTROL_UPDATE			,	{	"Control Update",						"Contr�ler mise � jour",				"Update �berpr�fen",					"Actualizaci�n de control",				"Controlla aggiornamento",				"Atualiza��o do controlo",				"Atualiza��o do controle"	}	},
	{ TSTR_CRITICAL_STOP			,	{	"CRITICAL STOP",						"ARR�T CRITIQUE",						"KRITISCHER ABBRUCH",					"PARADA CR�TICA",						"INTERRUZIONE CRITICA",					"PARAGEM CR�TICA",						"PARADA CR�TICA"	}	},
	{ TSTR_DATA_ENTRY_OPTIONS		,	{	"Data Entry Options",					"Options de saisie des donn�es",		"Dateneingabeoptionen",					"Opciones de entrada de datos",			"Opzioni immissione dati",				"Op��es de entrada de dados",			"Op��es de inser��o de dados"	}	},		
	{ TSTR_DISPLAY_SETTINGS			,	{	"Display Settings",						"Affichage param�tres",					"Anzeigeeinstellungen",					"Configuraci�n de pantalla",			"Impostazioni display",					"Configura��es do visor",				"Configura��es do display"	}	},
	{ TSTR_ELECTROLYTES_UNITS		,	{	"Electrolytes Units",					"Unit�s �lectrolytes",					"Elektrolyteinheiten",					"Unidades de electrolitos",				"Unit� elettroliti",					"Unidades de eletr�litos",				"Unidades de eletr�litos"	}	},
	{ TSTR_ENTER_ADMISSION_ID		,	{	"Enter Admission ID",					"Saisir ID admission",					"Zugangs-ID eingeben",					"Introducir Id. Admisi�n",				"Inserisci ID ammissione",				"Introduzir ID de admiss�o",			"Inserir ID de admiss�o"	}	},
	{ TSTR_ENTER_AGE				,	{	"Enter Age",							"Saisir l'�ge",							"Alter eingeben",						"Introducir edad",						"Inserisci et�",						"Introduzir idade",						"Inserir idade"	}	},
	{ TSTR_ENTER_AGE_RANGE			,	{	"Enter Age Range",						"Saisir fourchette d'�ge",				"Altersbereich eingeben",				"Introducir intervalo de edad",			"Inserisci fascia d'et�",				"Introduzir intervalo de idades",		"Inserir faixa et�ria"	}	},
	{ TSTR_ENTER_ALTERNATE_ID		,	{	"Enter Alternate ID",					"Saisir autre ID",						"Alternative ID eingeben",				"Introducir Id. suplente",				"Inserisci ID alternativo",				"Introduzir ID suplente",				"Inserir ID alternativo"	}	},
	{ TSTR_ENTER_ANALYZER_IP_ADDRESS,	{	"Enter Analyzer IP Address",			"Saisir l'adresse IP de l'analyseur",	"Analyseger�te-IP-Adresse eingeben",	"Introducir direcci�n IP analizador",	"Immettere indir. IP analizzatore",		"Introduzir endere�o IP analisador",	"Inserir endere�o IP do analisador"	}	},
	{ TSTR_ENTER_CONTROL_ID			,	{	"Enter Control ID",						"Entrer ID contr�le",					"Kontrollen-ID eingeben",				"Introducir Id. de control",			"Inserisci ID controllo",				"Introduzir ID do controlo",			"Inserir ID de controle"	}	},
	{ TSTR_ENTER_DATE				,	{	"Enter Date",							"Saisir date",							"Datum eingeben",						"Introducir fecha",						"Inserisci data",						"Introduzir data",						"Inserir data"	}	},
	{ TSTR_ENTER_DATE_OF_BIRTH		,	{	"Enter Date of Birth",					"Saisir date de naissance",				"Geburtsdatum eingeben",				"Introducir fecha de nacimiento",		"Inserisci data di nascita",			"Introduzir data de nascimento",		"Inserir data de nascimento"	}	},
	{ TSTR_ENTER_DOCTOR_ID			,	{	"Enter Doctor ID",						"Saisir ID m�decin",					"Arzt-ID eingeben",						"Introducir Id. del m�dico",			"Inserisci ID medico",					"Introduzir ID do m�dico",				"Inserir ID do m�dico"	}	},
	{ TSTR_ENTER_END_DATE			,	{	"Enter End Date",						"Saisir la date de fin",				"Enddatum eingeben",					"Escriba fecha final",					"Inserisci data fine",					"Introduzir data final",				"Inserir data final"	}	},
	{ TSTR_ENTER_GATEWAY_IP_ADDRESS	,	{	"Enter Gateway IP Address",				"Saisir adresse IP de la passerelle",	"Gateway-IP-Adresse eingeben",			"Introd. direcci�n IP puerta enlace",	"Immettere indir. IP gateway",			"Introduzir endere�o IP da porta",		"Inserir endere�o IP do gateway"	}	},
	{ TSTR_ENTER_ID					,	{	"Enter ID",								"Saisir ID",							"ID eingeben",							"Introducir Id.",					 	"Inserisci ID",							"Introduzir ID",						"Inserir ID"	}	},
	{ TSTR_ENTER_ID_TO_CANCEL		,	{	"Enter ID to Cancel",					"Saisir ID pour annuler",				"ID zum Abbrechen eingeben",			"Introducir ID para cancelar",		 	"Inserisci ID per annullare",			"Introduzir ID para cancelar",		  	"Inserir ID para cancelar"}},
	{ TSTR_ENTER_IP_MASK			,	{	"Enter Analyzer IP Mask", 				"Saisir le masque IP de l'analyseur",	"IP-Maske des Analyseger�ts eingeben",	"Introducir m�scara IP analizador",	 	"Inserisci ID per annullare",			"Introduzir m�scara IP do analisador",	"Inserir m�scara de IP do analisador"	}	},
	{ TSTR_ENTER_KENNEL_NUMBER		,	{	"Enter Kennel Number",					"Saisir n� chenil",						"Zwingernr. eingeben",					"Introducir n�mero de caseta",		 	"Inserisci numero Kennel",				"Introduzir n�mero de Kennel",		  	"Inserir n�mero do canil"	}	},
	{ TSTR_ENTER_LAST_VACCINATION	,	{	"Enter Last Vaccination",				"Saisir derni�re vaccination",			"Letzte Impfung eingeben",				"Introducir �ltima vacuna",			 	"inserisci ultima vaccinazione",		"Introduzir �ltima vacina��o",		  	"Inserir �ltima vacina��o"	}	},
	{ TSTR_ENTER_LOCATION			,	{	"Enter Location",						"Entrer emplacement",					"Ort eingeben",							"Introducir ubicaci�n",				 	"Inserisci centro",						"Introduzir localiza��o",			  	"Inserir local"	}	},
	{ TSTR_ENTER_OPERATOR_ID		,	{	"Enter Operator ID",					"Saisir ID op�rateur",					"Bediener-ID eingeben",					"Introducir Id. del operario",		 	"Inserisci ID operatore",				"Introduzir ID do operador",		  	"Inserir ID do operador"	}	},
	{ TSTR_ENTER_OWNER_ID			,	{	"Enter Owner ID",						"Saisir ID propri�taire",				"Besitzer-ID eingeben",					"Introducir id. del due�o",			 	"Inserisci ID proprietario",			"Introduzir ID do propriet�rio",	  	"Inserir ID do propriet�rio"	}	},
	{ TSTR_ENTER_PASSWORD			,	{	"Enter Password",						"Saisir mot de passe",					"Passwort eingeben",					"Introducir contrase�a",			 	"Inserisci password",					"Introduzir palavra-passe",			  	"Inserir senha"	}	},
	{ TSTR_ENTER_PATIENT_ID			,	{	"Enter Patient ID",						"Saisir ID patient",					"Patienten-ID eingeben",				"Introducir Id. Paciente",			 	"Inserisci ID paziente",				"Introduzir ID de doente",			  	"Inserir ID do paciente"	}	},
	{ TSTR_ENTER_PHONE_NUMBER		,	{	"Enter Phone Number",					"Saisir n� de t�l",						"Telefonnummer eingeben",				"Introducir n�mero de tel�fono",	 	"Inserisci numero di telefono",			"Introduzir n�mero de telefone",	  	"Inserir n�mero do telefone"	}	},
	{ TSTR_ENTER_SAMPLE_ID			,	{	"Enter Sample ID",						"Saisir ID �chantillon",				"Proben-ID eingeben",					"Introducir Id. de la muestra",		 	"Inserisci ID campione",				"Introduzir ID da amostra",			  	"Inserir ID da amostra"	}	},
	{ TSTR_ENTER_SERVER_IP_ADDRESS	,	{	"Enter Server IP Address",				"Saisir l'adresse IP du serveur",		"Server-IP-Adresse eingeben",		  	"Introducir direcci�n IP servidor",	 	"Immettere l'indirizzo IP del server",	"Introduzir endere�o IP do servidor", 	"Inserir endere�o IP do servidor"	}	},
	{ TSTR_ENTER_SERVER_SOCKET		,	{	"Enter Server Socket",					"Saisir le socket du serveur",			"Server-Socket eingeben",			  	"Introducir socket del servidor",	 	"Immettere il socket del server",		"Introduzir tomada do servidor",	  	"Inserir soquete do servidor"	}	},
	{ TSTR_ENTER_START_DATE			,	{	"Enter Start Date",						"Saisir la date de d�but",				"Anfangsdatum eingeben",			  	"Escriba fecha inicial",			 	"Inserisci data inizio",				"Introduzir data inicial",			  	"Inserir data de in�cio"	}	},
	{ TSTR_ENZYMES_UNITS			,	{	"Enzymes Units",						"Unit�s enzymes",						"Enzymeinheiten",					  	"Unidades de enzimas",				 	"Unit� enzimi",							"Unidades de enzimas",				  	"Unidades de enzimas"	}	},
	{ TSTR_ERROR					,	{	"ERROR",								"ERREUR",								"FEHLER",							  	"ERROR",							 	"ERRORE",								"ERRO",								  	"ERRO"	}	},
	{ TSTR_FACTORY_DEFAULT_SPECIES	,	{	"Factory Default Demographics",			"Param�tres par d�faut �lts d�mogr.",	"Voreingestellte Demographien",		  	"Demograf�a predeterminada",		 	"Demografia predefinita di fabbrica",	"Valores de demografia de f�brica",	  	"Dados demog. padr�o de f�brica"	}	},
	{ TSTR_FACTORY_DEFAULTS			,	{	"Factory Defaults",						"Param�tres par d�faut en usine",		"Werkseinstellungen",				  	"Valores predeterminados",			 	"Impostazioni di fabbrica",				"Valores de f�brica",				  	"Padr�es de f�brica"	}	},
	{ TSTR_GROUP_UNITS				,	{	"Group Units",							"Unit�s groupe",						"Gruppeneinheiten",					  	"Unidades de grupo",				 	"Unit� gruppo",							"Unidades de grupo",				  	"Unidades do grupo"	}	},
	{ TSTR_HOME						,	{	"Home",									"Page d'accueil",						"Startseite",						  	"Inicio",							 	"Home",									"In�cio",							  	"In�cio"	}	},
	{ TSTR_INCLUDE_REF_RANGES		,	{	"Include Reference Ranges",				"Inclure fourchettes de r�f�rence",		"Referenzbereiche angeben",			  	"Incluir intervalos de referencia",	 	"Includi range di riferimento",			"Incluir intervalos de refer�ncia",	  	"Incluir intervalos de refer�ncia"	}	},
	{ TSTR_INFORMATION				,	{	"Information",							"Information",							"Information",						  	"Informaci�n",						 	"Informazioni",							"Informa��o",						  	"Informa��es"	}	},
	{ TSTR_INSERT_ROTOR				,	{	"Insert Disc",							"Ins�rer disque",						"Scheibe einsetzen",				  	"Insertar disco",					 	"Inserisci disco",						"Inserir disco",					  	"Inserir disco"	}	},
	{ TSTR_INSERT_SAMPLE			,	{	"Insert Sample",						"Ins�rer �chantillon",					"Probe einsetzen",					  	"Insertar muestra",					 	"Inserisci campione",					"Inserir amostra",					  	"Inserir amostra"	}	},
	{ TSTR_INTERNAL_ARCHIVES		,	{	"Internal Archives",					"Archives internes",					"Interne Archive",					  	"Archivos internos",				 	"Archivi interni",						"Ficheiros internos",				  	"Arquivos internos"	}	},
	{ TSTR_LIPIDS_UNITS				,	{	"Lipids Units",							"Unit�s lipides",						"Lipideinheiten",					  	"Unidades de l�pidos",				 	"Unit� lipidi",							"Unidades de l�pidos",				  	"Unidades de lip�dios"	}	},
	{ TSTR_LOAD_CD					,	{	"Load CD",								"Charger CD",							"CD laden",							  	"Cargar CD",						 	"Carica CD",							"Carregar CD",						  	"Carregar CD"	}	},
	{ TSTR_LOAD_CONTROLS_CD			,	{	"Load Controls CD",						"Charger CD commandes",					"CD-Steuerung laden",				  	"Cargar controles CD",				 	"Carica CD di controllo",				"Carregar CD de controlos",			  	"Carregar CD de controles"	}	},
	{ TSTR_LOAD_ORDER				,	{	"Load Order",							"Charger ordre",						"Anforderung laden",				  	"Cargar pedido",					 	"Carica ordine",						"Carregar pedido",					  	"Carregar solicita��o"	}	},
	{ TSTR_LOAD_ROTOR				,	{	"Load Disc",							"Charger disque",						"Scheibe laden",					  	"Cargar disco",						 	"Carica disco",							"Carregar disco",					  	"Carregar disco"	}	},
	{ TSTR_LOCKOUT_SETTINGS			,	{	"Lockout Settings",						"Param�tres verrouillage",				"Lookout-Eimstellungen",			  	"Confguraci�n de bloqueo",			 	"Impostazioni del blocco",				"Configura��es de bloqueio",		  	"Configura��es de bloqueio"	}	},
	{ TSTR_MANAGE_ADMINISTRATORS	,	{	"Manage Administrators",				"Gestion administrateurs",				"Administratoren verwalten",		  	"Gestionar administradores",		 	"Gestisci amministratori",				"Gerir administradores",			  	"Gerenciar administradores"	}	},
	{ TSTR_MANAGE_OPERATORS			,	{	"Manage Operators",						"Gestion op�rateurs",					"Bediener verwalten",				  	"Gestionar operarios",				 	"Gestisci operatori",					"Gerir operadores",					  	"Gerenciar operadores"	}	},
	{ TSTR_MINERALS_UNITS			,	{	"Minerals Units",						"Unit�s min�rales",						"Mineralieneinheiten",				  	"Unidades de minerales",			 	"Unit� minerali",						"Unidades de minerais",				  	"Unidades de minerais"	}	},
	{ TSTR_MODIFY_REF_RANGES		,	{	"Modify Reference Range",				"Modifier fourchette de r�f�rence",		"Referenzbereich �ndern",			  	"Modificar intervalo de referencia", 	"Modifica range di riferimento",		"Alterar intervalo de refer�ncia",	  	"Modificar intervalo de refer�ncia"	}	},
	{ TSTR_NEW_ADMINISTRATOR_ID		,	{	"New Administrator ID",					"Nouvel ID administrateur",				"Neue Administratoren-ID",			  	"Nueva Id. del administrador",		 	"Nuova ID amministratore",				"Nova ID do administrador",			  	"Novo ID do administrador"	}	},
	{ TSTR_NEW_OPERATOR_ID			,	{	"New Operator ID",						"Nouvel ID op�rateur",					"Neue Bediener-ID",					  	"Nueva Id. del operario",				"Nuova ID operatore",					"Nova ID do operador",				  	"ID do novo operador"	}	},
	{ TSTR_NO_RESULTS				,	{	"No Results",							"Aucun r�sultat",						"Keine Ergebnisse",					  	"No hay resultados",					"Nessun risultato",						"Sem resultados",					  	"Nenhum resultado"	}	},
	{ TSTR_NUM_ERROR_COPIES			,	{	"# Error Copies",						"Nbre copies erron�es",					"# Fehler-Kopien",					  	"N� de copias de error",				"N. Copie errore",						"N.� de c�pias de erro",			  	"No. de c�pias de erro"	}	},
	{ TSTR_NUM_IQC_COPIES			,	{	"# iQC Copies",							"Nbre copies iQC",						"# iQC-Kopien",						  	"N� de copias iQC",						"N. Copie iQC",							"N.� de c�pias do iQC",				  	"No. de c�pias de iQC"	}	},
	{ TSTR_NUM_RESULTS_COPIES		,	{	"# Results Copies",						"Nbre copie r�sultats",					"# Ergebniskopien",					  	"N� de copias de resultados",			"N. Copie risultati",					"N.� de c�pias de resultados",		  	"No. de c�pias dos resultados"	}	},
	{ TSTR_OPERATOR					,	{	"Operator",								"Op�rateur",							"Bediener",							  	"Operario",								"Operatore",							"Operador",							  	"Operador"	}	},
	{ TSTR_OPERATOR_EXPIRATION		,	{	"Operator Expiration",					"Expiration op�rateur",					"Ung�ltiger Bediener",				  	"Vencimiento del operario",				"Scadenza operatore",					"Prazo de validade do operador",	  	"Expira��o do operador"	}	},
	{ TSTR_OPERATOR_ID				,	{	"Operator ID",							"ID op�rateur",							"Bediener-ID",						  	"Id. del operario",						"ID operatore",							"ID do operador",					  	"ID do operador"	}	},
	{ TSTR_PATIENT					,	{	"Patient:",								"Patient :",							"Patient:",							  	"Paciente:",							"Paziente:",							"Doente:",							  	"Paciente:"	}	},
	{ TSTR_PATIENT_BY_DATE			,	{	"Patient Results Sorted by Date",		"R�sultats patients tri�s par date",	"Patientenergebnisse nach Datum",	  	"Resultados paciente por fecha",		"Risultati paz. ordinati per data",		"Resultados do doente por data",	  	"Result. paciente classif. por data"	}	},
	{ TSTR_PENDING_ORDERS			,	{	"Pending Orders",						"Ordres en cours",					  	"Ausstehende Anforderungen",		  	"Pedidos pendientes",					"Ordini in sospeso",					"Pedidos pendentes",				  	"Solicita��es pendentes"	}	},
	{ TSTR_POWER_SAVE_WAIT			,	{	"Power Save Wait",						"Attente �conomiseur alim.",		  	"Energiesparmodus, warten",			  	"Espera de ahorro de energ�a",			"Attendere Modo risparmio",				"Espera de poupan�a de energia",	  	"Espera de economia de energia"	}	},
	{ TSTR_PRINTER_SETUP			,	{	"Printer Setup",						"Configuration imprimante",			  	"Druckereinrichtung",				  	"Configuraci�n impresora",				"Impostazioni stampante",				"Configura��o da impressora",		  	"Configura��o da impressora"	}	},
	{ TSTR_PROTEINS_UNITS			,	{	"Proteins Units",						"Unit�s prot�ines",					  	"Proteineinheiten",					  	"Unidades de prote�nas",				"Unit� proteine",						"Unidades de prote�nas",			  	"Unidades de prote�nas"	}	},
	{ TSTR_RANGES					,	{	"Ranges",								"Fourchettes",						  	"Bereiche",							  	"Intervalos",							"Range",								"Intervalos",						  	"Intervalos"	}	},
	{ TSTR_READ_RESULTS				,	{	"Read Results",							"Lecture r�sultats",				  	"Ergebnisse auslesen",				  	"Leer resultados",						"Leggi risultati",						"Ler resultados",					  	"Ler resultados"	}	},
	{ TSTR_RECALL					,	{	"Recall",								"Rappel",							  	"Wiederaufruf",						  	"Recordar",								"Richiama",								"Recordar",							  	"Recuperar"	}	},
	{ TSTR_REMOVE_ROTOR				,	{	"Remove Disc",							"Retirer disque",					  	"Scheibe entfernen",				  	"Retirar disco",						"Rimuovi disco",						"Retirar disco",					  	"Remover disco"	}	},
	{ TSTR_REMOVE_SPECIES			,	{	"Remove Demographic",					"Retirer �l�ments d�mographiques",	  	"Demographien entfernen",			  	"Eliminar demograf�a",					"Rimuovi demografia",					"Eliminar demografia",				  	"Remover dados demogr�ficos"	}	},
	{ TSTR_RETRIEVE_REF_RANGES		,	{	"Retrieve Reference Ranges",			"R�cup�rer fourchettes de r�f�rence", 	"Referenzbereiche empfangen",		  	"Recuperar intervalos de referencia",	"Recupera range di riferimento",		"Recuperar intervalos de refer�ncia", 	"Recuperar intervalos de refer�ncia"	}	},
//	{ TSTR_SAMPLE					,	{	"Sample",								"�chantillon",						  	"Probe",							  	"Muestra",								"Campione",								"Amostra",							  	"Amostra"	}	},
	{ TSTR_SCREEN_SAVER_WAIT		,	{	"Screen Saver Wait",					"Attente �conomiseur d'�cran",		  	"Bildschirmschoner, warten",		  	"Espera del salvapantalla",				"Attendere salvaschermo",				"Espera de poupan�a do ecr�",		  	"Espera de prote��o de tela"	}	},
	{ TSTR_SEARCH					,	{	"Search",								"Rechercher",						  	"Suchen",							  	"B�squeda",								"Cerca",								"Pesquisar",						  	"Pesquisar"	}	},
	{ TSTR_SEARCH_ADMINISTRATORS	,	{	"Search Administrators",				"Rechercher administrateurs",		  	"Administratoren suchen",			  	"Buscar administradores",				"Cerca amministratori",					"Pesquisar administradores",		  	"Pesquisar administradores"	}	},
	{ TSTR_SEARCH_OPERATORS			,	{	"Search Operators",						"Rechercher op�rateurs",			  	"Bediener suchen",					  	"Buscar operarios",						"Cerca operatori",						"Pesquisar operadores",				  	"Pesquisar operadores"	}	},
	{ TSTR_SECURITY_SETTINGS		,	{	"Security Settings",					"Param�tres de s�curit�",			  	"Sicherheitseinstellungen",			  	"Configuraci�n de seguridad",			"Impostazioni sicurezza",				"Configura��es de seguran�a",		  	"Configura��es de seguran�a"	}	},
	{ TSTR_SELECT_DISPLAY_LANG		,	{	"Select Display Language",				"S�lectionner affich. langue",		  	"Sprache ausw�hlen",				  	"Seleccionar idioma",					"Seleziona lingua display",				"Selecionar idioma do visor",		  	"Selecionar idioma de exibi��o"	}	},
	{ TSTR_SELECT_GENDER			,	{	"Select Gender",						"S�lectionner le sexe",				  	"Geschlecht ausw�hlen",				  	"Seleccionar sexo",						"Seleziona sesso",						"Selecionar sexo",					  	"Selecionar sexo"	}	},
	{ TSTR_SELECT_KEYBOARD			,	{	"Select Keyboard Type",					"S�lectionner type de clavier",		  	"Tastatur ausw�hlen",				  	"Seleccionar tipo de teclado",			"Seleziona tipo tastiera",				"Selecionar tipo de teclado",		  	"Selecionar tipo de teclado"	}	},
	{ TSTR_SELECT_PRINTER			,	{	"Select Printer",						"S�lectionner imprimante",			  	"Drucker ausw�hlen",				  	"Seleccionar impresora",				"Seleziona stampante",					"Selecionar impressora",			  	"Selecionar impressora"	}	},
	{ TSTR_SELECT_PROTOCOL			,	{	"Select Protocol",						"S�lectionner un protocole",		  	"Protokoll ausw�hlen",				  	"Seleccionar protocolo",				"Selezionare protocollo",				"Selecionar protocolo",				  	"Selecionar protocolo"	}	},
	{ TSTR_SELECT_RACE				,	{	"Select Race",							"Choisir la race",					  	"Abstammung ausw�hlen",				  	"Seleccione raza",						"Seleziona razza",						"Selecionar corrida",				  	"Selecionar ra�a"	}	},
	{ TSTR_SELECT_REPORT			,	{	"Select Report",						"S�lectionner rapport",				  	"Bericht ausw�hlen",				  	"Seleccionar informe",					"Select rapporto",						"Selecionar relat�rio",				  	"Selecionar relat�rio"	}	},
	{ TSTR_SELECT_REPORTS			,	{	"Select Reports",						"S�lectionner rapports",			  	"Berichte ausw�hlen",				  	"Seleccionar informes",					"Seleziona rapporti",					"Selecionar relat�rios",			  	"Selecionar relat�rios"	}	},
	{ TSTR_SELECT_SPECIES			,	{	"Select Demographic",					"S�lectionner �l�ments d�mogr.",	  	"Demographie ausw�hlen",			  	"Seleccionar demograf�a",				"Seleziona demografia",					"Selecionar demografia",			  	"Selecionar dados demogr�ficos"	}	},
	{ TSTR_SELECT_TYPE				,	{	"Select Type",							"S�lectionner type",				  	"Typ ausw�hlen",					  	"Seleccionar tipo",						"Seleziona tipo",						"Selecionar tipo",					  	"Selecionar tipo"	}	},
	{ TSTR_SEND_REF_RANGES			,	{	"Send Reference Ranges",				"Envoyer fourchettes de r�f.",		  	"Referenzbereiche senden",			  	"Enviar intervalos de referencia",		"Imposta range di riferimento",			"Enviar intervalos de refer�ncia",	  	"Enviar intervalos de refer�ncia"	}	},
	{ TSTR_SERVICE_ARCHIVE			,	{	"Service Archive",						"Archive service",					  	"Service-Archiv",					  	"Archivo de servicio",					"Archivio di servizio",					"Ficheiro do servi�o",				  	"Arquivo de servi�o"	}	},
	{ TSTR_SET_DATE					,	{	"Set Date",								"D�finir date",						  	"Datum einstellen",					  	"Establecer fecha",						"Imposta data",							"Configurar data",					  	"Configurar data"	}	},
	{ TSTR_SET_DEFAULT				,	{	"Set Default",							"D�finir param�tres par d�faut",	  	"Standard einstellen",				  	"Establecer predeterminados",			"Imposta come predefinito",				"Configurar valores predeterminados", 	"Configurar padr�o"	}	},
	{ TSTR_SET_LANGUAGE				,	{	"Set Language",							"D�finir la langue",				  	"Sprache einstellen",				  	"Establecer idioma",					"Imposta lingua",						"Configurar idioma",				  	"Configurar idioma"	}	},
	{ TSTR_SET_PROTOCOL				,	{	"Set Protocol",							"D�finir le protocole",				  	"Protokoll einstellen",				  	"Establecer protocolo",					"Imposta protocollo",					"Configurar protocolo",				  	"Configurar protocolo"	}	},
	{ TSTR_SET_TIME					,	{	"Set Time",								"D�finir l'heure",					  	"Zeit einstellen",					  	"Establecer hora",						"Imposta tempo",						"Configurar hora",					  	"Configurar hora"	}	},
	{ TSTR_SET_UNITS				,	{	"Set Units",							"D�finir les unit�s",				  	"Einheiten einstellen",				  	"Establecer unidades",					"Imposta unit�",						"Configurar unidades",				  	"Configurar unidades"	}	},
	{ TSTR_SINGLE_ANALYTE			,	{	"Single Analyte",						"Une seule substance � analyser",	  	"Einzelnes Analyt",					  	"Analito �nico",						"Analita singolo",						"Analito �nico",					  	"Analito �nico"	}	},
	{ TSTR_SOFTWARE_UPDATE			,	{	"Software Update",						"Mise � jour logiciel",				  	"Software-Update",					  	"Actualizaci�n del software",			"Aggiornamento software",				"Atualiza��o do software",			  	"Atualiza��o de software"	}	},
	{ TSTR_SOUND_SETTINGS			,	{	"Sound Settings",						"Param�tres sonores",				  	"Sound-Einstellungen",				  	"Configuraci�n de sonido",				"Impostazioni audio",					"Configura��es do som",				  	"Configura��es do som"	}	},
	{ TSTR_SPECIES					,	{	"Demographic",							"�l�ments d�mographiques",			  	"Demographie",						  	"Demograf�a",							"Demografia",							"Demografia",						  	"Dados demogr�ficos"	}	},
	{ TSTR_SPECIES_AVAILABLE		,	{	"Demographics Available",				"�l�ments d�mographiques disponibles",	"Verf�gbare Demographien",			  	"Demograf�as disponibles",				"Demografia disponibile",				"Demografia dispon�vel",			  	"Dados demogr�ficos dispon�veis"	}	},
	{ TSTR_STATUS_VOLUME			,	{	"Status Volume",						"�tat volume",						  	"Mengenstatus",						  	"Volumen de estado",					"Volume di stato",						"Volume de estado",					  	"Volume do status"	}	},
	{ TSTR_TCPIP_SETTINGS			,	{	"TCP/IP Settings",						"Param�tres TCP/IP",				  	"TCP/IP-Einstellungen",				  	"Configuraci�n TCP/IP",					"Impostazioni TCP/IP",					"Configura��es de TCP/IP",			  	"Configura��es de TCP/IP"	}	},
	{ TSTR_TRANSMIT_ALL				,	{	"Transmit All",							"Transmettre tout",					  	"Alle �bertragen",					  	"Transmitir todo",						"Trasmetti tutti",						"Transmitir tudo",					  	"Transmitir todos"	}	},
	{ TSTR_TRANSMIT_DATE_RANGE		,	{	"Transmit Date Range",					"Transmettre la plage de dates",	  	"Datumsbereich �bertragen",			  	"Transmitir intervalo de fechas",		"Trasmetti range di date",				"Transmitir intervalo de data",		  	"Transmitir intervalo de datas"	}	},
	{ TSTR_UNITS					,	{	"Units",								"Unit�s",							  	"Einheiten",						  	"Unidades",								"Unit�",								"Unidades",							  	"Unidades"	}	},
	{ TSTR_VIEW_RESULT				,	{	"View Result",							"Afficher r�sultat",				  	"Ergebnis ansehen",					  	"Ver resultado",						"Visualizza risultati",					"Visualizar resultado",				  	"Visualizar resultado"	}	},
	{ TSTR_WARNING					,	{	"WARNING",								"MISE EN GARDE",					  	"ACHTUNG",							  	"AVISO",								"ATTENZIONE",							"ADVERT�NCIA",						  	"AVISO"	}	},

// Middle (Long Button Stings > 35 Characters Unlikely to Display Correctly)
//											ENGLISH,																			FRENCH,																											GERMAN,																												SPANISH,																								ITALIAN, 																											PORTUGUESE,																									PORTUGUESE (BRAZIL)
//											          11111111112222222222233333												          11111111112222222222233333																			          11111111112222222222233333																				          11111111112222222222233333																	          11111111112222222222233333																				          11111111112222222222233333																		          11111111112222222222233333																				          	
//											 12345678901234567890123456789012345												 12345678901234567890123456789012345																			 12345678901234567890123456789012345																				 12345678901234567890123456789012345																	 12345678901234567890123456789012345																				 12345678901234567890123456789012345																		 12345678901234567890123456789012345	
	{ MSTR_ADD_ADMINISTRATOR		,	{	"Add Administrator",																"Ajouter administrateur",																						"Admin. hinzuf�gen",																								"A�adir administrador",																					"Agg. amministratore",																								"Adicionar administrador",																					"Adicionar administrador"	}	},
	{ MSTR_ADD_CONTROL_VALUES		,	{	"Add Control Values",																"Ajouter valeurs de contr�le",																					"Steuerungswerte hinzuf�gen",																						"A�adir valores de control",																			"Aggiungi valori di controllo",																						"Adicionar valores de controlo",																			"Adicionar valores de controle"	}	},
	{ MSTR_ADD_OPERATOR				,	{	"Add Operator",																		"Ajouter op�rateur",																							"Bediener hinzuf�gen",																								"A�adir operario",																						"Aggiungi operatore",																								"Adicionar operator",																						"Adicionar operador"	}	},
	{ MSTR_ADD_SPECIES				,	{	"Add Demographic",																	"Ajouter �l�ments d�mographiques",																				"Demographie hinzuf�gen",																							"A�adir demograf�a",																					"Aggiungi demografia",																								"Adicionar demografia",																						"Adicionar dados demogr�ficos"	}	},
	{ MSTR_ADMISSION_ID				,	{	"Admission ID",																		"ID admission",																									"Zugangs-ID",																										"Id. admisi�n",																							"ID ammissione",																									"ID de Admiss�o",																							"ID de admiss�o"	}	},
	{ MSTR_ADMINISTRATOR_ID			,	{	"Administrator ID",																	"ID administrateur",																							"Administrator-ID",																									"Id. del administrador",																				"ID amministratore",																								"ID do administrador",																						"ID do administrador"	}	},
	{ MSTR_ADMINISTRATORS			,	{	"Administrators",																	"Administrateurs",																								"Administratoren",																									"Administradores",																						"Amministratori",																									"Administradores",																							"Administradores"	}	},
	{ MSTR_AGE						,	{	"Age",																				"Age",																											"Alter",																											"Edad",																									"Et�",																												"Idade",																									"Idade"	}	},
	{ MSTR_ALERTS					,	{	"Alerts",																			"Avertisseurs",																									"Warnungen",																										"Alertas",																								"Allarmi",																											"Alertas",																									"Alertas"	}	},
	{ MSTR_ALL						,	{	"All",																				"Tous",																											"Alle",																												"Todo",																									"Tutti",																											"Tudo",																										"Todos"	}	},
	{ MSTR_ALL_ANALYTES				,	{	"All Analytes",																		"Toutes subs. � analyser",																						"Alle Analyte",																										"Todos los analitos",																					"Tutti gli analiti",																								"Todos os analitos",																						"Todos os analitos"	}	},
	{ MSTR_ALTERNATE_ID				,	{	"Alternate ID",																		"Autre ID",																										"ID �ndern",																										"Id. suplente",																							"ID alternativo",																									"ID suplente",																								"ID alternativo"	}	},
	{ MSTR_ALWAYS					,	{	"Always",																			"Toujours",																										"Immer",																											"Siempre",																								"Sempre",																											"Sempre",																									"Sempre"	}	},
	{ MSTR_ANALYSIS_COMPLETE		,	{	"Analysis Complete",																"Analyse termin�e",																								"Analyse abgeschlossen",																							"An�lisis completado",																					"Analisi completa",																									"An�lise conclu�da",																						"An�lise conclu�da"	}	},
	{ MSTR_ANALYTE					,	{	"Analyte",																			"Substance � analyser",																							"Analyt",																											"Analito",																								"Analita",																											"Analito",																									"Analito"	}	},
	{ MSTR_ANALYTE_GROUPS			,	{	"Analyte Groups",																	"Gr. subst. � analyser",																						"Analytgruppen",																									"Grupos analito",																						"Gruppi analita",																									"Grupos de analito",																						"Grupos de analitos"	}	},
	{ MSTR_ANALYTE_OPTIONS			,	{	"Analyte Options",																	"Options � analyser",																							"Analytoptionen",																									"Opciones de analito",																					"Opzioni analita",																									"Op��es de analito",																						"Op��es de analito"	}	},
	{ MSTR_ANALYZE					,	{	"Analyze",																			"Analyser",																										"Analysieren",																										"Analizar",																								"Analizzare",																										"Analisar",																									"Analisar"	}	},
	{ MSTR_ANALYZER_IP				,	{	"Analyzer IP",																		"IP de l'analyseur",																							"Analyseger�t-IP",																									"IP del analizador",																					"IP analizzatore",																									"IP do analisador",																							"IP do analisador"	}	},
	{ MSTR_ANALYZER_PRINTER			,	{	"Analyzer Printer",																	"Imprimte analyseur",																							"Drucker-Analysege.",																								"Impres. Analiz.",																						"Stamp. Analizzatore",																								"Impressora do analisador",																					"Impressora do analisador"	}	},
	{ MSTR_ANALYZING_SAMPLE			,	{	"Analyzing Sample...",																"Analyse �ch. en cours...",																						"Probe wird analysiert ...",																						"Analizando muestra...",																				"Esame camp. in corso",																								"A analisar amostra...",																					"Analisando amostra..."	}	},
	{ MSTR_ASCII_TEXT				,	{	"ASCII Text",																		"Texte ASCII",																									"ASCII-Text",																										"Texto ASCII",																							"Testo ASCII",																										"Texto ASCII",																								"Texto ASCII"	}	},
	{ MSTR_ASIAN_PACIFIC_ISLANDER	,	{	"Asian/Pacific Islander",															"Asiat./orig. �les Pac.",																						"Asiatisch/Pazifisch",																								"Asi�tico/Isl. Pac�fico",																				"Isolano Asia/Pacifico",																							"Ilhas Asi�ticas/do Pac�fico",																				"Asi�tico/ilhas do Pac�fico"	}	},
	{ MSTR_ASTM_1394_7				,	{	"ASTM 1394-97",																		"ASTM 1394-97",																									"ASTM 1394-97",																										"ASTM 1394-97",																							"ASTM 1394-97",																										"ASTM 1394-97",																								"ASTM 1394-97"	}	},
	{ MSTR_ASTM_ORDERS				,	{	"ASTM Orders",																		"Ordres ASTM",																									"ASTM Anforderungen",																								"Pedidos ASTM",																							"Ordini ASTM",																										"Pedidos ASTM",																								"Solicita��es ASTM"	}	},
	{ MSTR_AUTO						,	{	"Auto",																				"Auto",																											"Automat.",																											"Auto",																									"Auto",																												"Auto",																										"Auto"	}	},
	{ MSTR_BACKLIGHT				,	{	"Backlight",																		"R�tro�clairage", 																								"Hintergrundbeleucht.",																								"Luz",																									"Retroilluminazione",																								"Luz de fundo",																								"Luz de fundo"	}	},
	{ MSTR_BASE_TEST				,	{	"Base Test",																		"Test de base",																									"Basistest",																										"Prueba base",																							"Test base",																										"Teste base",																								"Teste de base"	}	},
	{ MSTR_BLACK					,	{	"Black",																			"Noire",																										"Schwarz",																											"Negro",																								"Nero",																												"Negro",																									"Preto"	}	},
	{ MSTR_BOTH						,	{	"Both",																				"Les deux", 																									"Beides",																											"Ambos",																								"Entrambe",																											"Ambos",																									"Ambos"	}	},
	{ MSTR_BROWSE					,	{	"Browse",																			"Naviguer", 																									"Durchsuchen",																										"Examinar",																								"Sfoglia",																											"Navegar",																									"Procurar"	}	},
	{ MSTR_CALIBRATE				,	{	"Calibrate",																		"Calibrer", 																									"Kalibrieren",																										"Calibrar",																								"Calibra",																											"Calibrar",																									"Calibrar"	}	},
	{ MSTR_CAM_CYCLES				,	{	"Cam Cycles",																		"Cycles cam", 																									"Nockenzyklen",																										"Ciclos de la leva",																					"Cicli camma",																										"Ciclos Cam",																								"Ciclos da cam"	}	},
	{ MSTR_CANCEL_ANALYSIS			,	{	"Cancel Analysis?",																	"Annuler analyse ?", 																							"Analyse abbrechen?",																								"�Cancelar an�lisis?",																					"Annullare analisi?",																								"Cancelar an�lise?",																						"Cancelar an�lise?"	}	},
	{ MSTR_CANCEL_ARCHIVE			,	{	"Cancel Archive?",																	"Annuler archive ?", 																							"Archiv stornieren?",																								"�Cancelar archivo?",																					"Annulla archivio?",																								"Cancelar arquivo?",																						"Cancelar arquivo?"	}	},
	{ MSTR_CANCEL_ORDER				,	{	"Cancel Order",																		"Annuler l'ordre",																								"Anforderung abbrechen",																							"Cancelar pedido",																						"Annulla ordine",																									"Cancelar pedido",																							"Cancelar solicita��o"	}	},
	{ MSTR_CANCEL_TRANSMISSION		,	{	"Cancel Transmission?",																"Annuler transmission ?", 																						"�bertragung abbrechen?",																							"�Cancelar transmisi�n?",																				"Annullare trasmissione?",																							"Cancelar transmiss�o?",																					"Cancelar transmiss�o?"	}	},
	{ MSTR_CANCELED					,	{	"Canceled",																			"Annul�", 																										"Abgebrochen",																										"Cancelado",																							"Annullato",																										"Cancelado",																								"Cancelado"	}	},
	{ MSTR_CANCELING_ANALYSIS		,	{	"Canceling analysis, please wait",													"Annulation analyse en cours, veuillez patienter",																"Analyse wird abgebrochen, bitte warten",																			"Cancelando an�lisis. Espere, por favor.",																"Annullamento analisi, attendere",																					"A cancelar an�lise, espere por favor",																		"Cancelando a an�lise, aguarde"	}	},
	{ MSTR_CHANGE_EXPIRATION		,	{	"Change Expiration",																"Modifier expiration",																							"Ablaufdatum �ndern",																								"Cambiar vencimiento",																					"Modifica scadenza",																								"Alterar prazo de validade",																				"Alterar expira��o"	}	},
	{ MSTR_CHANGE_PASSWORD			,	{	"Change Password",																	"Modifier mot de passe",																						"Passwort �ndern",																									"Cambiar contrase�a",																					"Modifica password",																								"Alterar palavra-passe",																					"Alterar senha"	}	},
	{ MSTR_CHANGE_SETTINGS			,	{	"Change Settings",																	"Modifier les param�tres",																						"Einstellungen �ndern",																								"Cambiar configuraci�n",																				"Modifica impostazioni",																							"Alterar configura��es",																					"Alterar configura��es"	}	},
	{ MSTR_CHECK_SETTINGS			,	{	"Check Settings",																	"V�rifier les param�tres",																						"Einstell. �perpr�fen",																								"Comprobar config.",																					"Verifica impostazioni",																							"Verificar configura��es",																					"Verificar configura��es"	}	},
	{ MSTR_CLEAR					,	{	"Clear",																			"Supprimer",																									"Freigeben",																										"Borrar",																								"Pulisci",																											"Eliminar",																									"Limpar"	}	},
	{ MSTR_CLINICAL_ARCHIVE			,	{	"Clinical Archive",																	"Archive clinique",																								"Klinisches Archiv",																								"Archivo cl�nico",																						"Archivio clinico",																									"Ficheiro cl�nico",																							"Arquivo cl�nico"	}	},
	{ MSTR_CLINICAL_ARCHIVE_ELLIPSIS,	{	"Clinical Archive...",																"Archive clinique...",																							"Klinisches Archiv...",																								"Archivo cl�nico..",																					"Archivio clinico...",																								"Ficheiro cl�nico...",																						"Arquivo cl�nico..."	}	},
	{ MSTR_CLOSE_DRAWER_TO_ANALYZE	,	{	"Close drawer to analyze a sample",													"Fermer tiroir pour analyser un �chantillon",																	"Zur Probenanalyse Fach schlie�en",																					"Cerrar el caj�n para analizar una muestra",															"Chiudi cassetto per analizzare campione",																			"Fechar a gaveta para analisar uma amostra",																"Feche a gaveta para analisar uma amostra"	}	},
	{ MSTR_CLOSING_DRAWER			,	{	"Closing Drawer...",																"Fermeture tiroir en cours...",																					"Fach wird geschlossen ...",																						"Cerrando caj�n...",																					"Chiusura cassetto...",																								"A fechar a gaveta...",																						"Fechando gaveta..."	}	},
	{ MSTR_CONFIGURE				,	{	"Configure",																		"Configurer",																									"Konfigurieren",																									"Config.",																								"Configura",																										"Configurar",																								"Configurar"	}	},
	{ MSTR_CONFIRM_TRANSMIT_ALL		,	{	"Press Yes to transmit all results saved in the analyzer",							"App. Oui pour trans. tous r�sultats enreg. ds analyseur",														"\"Ja\", um alle im Analyseger�t gesp. Erg. zu �bertrag.",															"Pulse S� para transmitir todos los resultados guardados en el analizador",								"Premere S� per trasmettere tutti i risultati savalti nell'analizzatore",											"Prima Sim para transmitir todos os resultados guardados no analisador",									"Pressione Sim para transmitir todos os resultados salvos no analisador"	}	},
	{ MSTR_CONNECTION_FAILED		,	{	"Connection failed",																"�chec de la connexion",																						"Verbindung nicht hergestellt",																						"Fall� conexi�n",																						"Connessione non riuscita",																							"Falha de liga��o",																							"Falha na conex�o"	}	},
	{ MSTR_CONNECTION_SUCCESSFUL	,	{	"Connection successful",															"Connexion �tablie",																							"Verbindung hergestellt",																							"Conexi�n establecida",																					"Connessione riuscita",																								"Liga��o estabelecida",																						"Conex�o bem-sucedida"	}	},
	{ MSTR_CONTROLS					,	{	"Controls",																			"Contr�les",																									"Steuerungen",																										"Controles",																							"Controlli",																										"Controlos",																								"Controles"	}	},
	{ MSTR_CURRENT					,	{	"Current",																			"Actuel",																										"Laufend",																											"Actual",																								"Attuale",																											"Atual",																									"Atual"	}	},
	{ MSTR_DATA_ENTRY_OPTIONS		,	{	"Data Entry Options",																"Saisie des donn�es",																							"Dateneingabeoptionen",																								"Opc. entrada datos",																					"Opzioni imm. dati",																								"Op��es de entrada de dados",																				"Op��es de inser��o de dados"	}	},
	{ MSTR_DATE						,	{	"Date",																				"Date",																											"Datum",																											"Fecha",																								"Data",																												"Data",																										"Data"	}	},
	{ MSTR_DATE_OF_BIRTH			,	{	"Date of Birth (DOB)",																"Date de naissance (DOB)",																						"Geburtsdatum (GD)",																								"Fecha de nacimiento",																					"Data di nascita (DN)",																								"Data de nascimento (DDN)",																					"Data de nascimento (DN)"	}	},
	{ MSTR_DATE_OF_VACCINATION		,	{	"Last Vaccination (DOV)",															"Dernier vaccin (DOV)",																							"Letzte Impfung (LI)",																								"�ltima vacuna",																						"Ultima vaccinazione (DV)",																							"�ltima vacina��o (DDV)",																					"�ltima vacina��o (Data de vacin.))"	}	},
	{ MSTR_DAY						,	{	"Day",																				"Jour",																											"Tag",																												"D�a",																									"Giorno",																											"Dia",																										"Dia"	}	},
	{ MSTR_DAYS						,	{	"Days",																				"Jours",																										"Tage",																												"D�as",																									"Giorni",																											"Dias",																										"Dias"	}	},
	{ MSTR_DEFAULT					,	{	"Default",																			"V/d�ft",																										"Standard",																											"Predeterm.",																							"Predef.",																											"Predeterminado",																							"Padr�o"	}	},
	{ MSTR_DELETE_ADMINISTRATOR		,	{	"Delete Administrator",																"Suppr. administrateur",																						"Administrator l�schen",																							"Eliminar administrador",																				"Cancella amministratore",																							"Eliminar administrador",																					"Excluir administrador"	}	},
	{ MSTR_DELETE_OPERATOR			,	{	"Delete Operator",																	"Suppr. op�rateur", 																							"Bediener l�schen",																									"Eliminar operario",																					"Cancella operatore",																								"Eliminar operador",																						"Excluir operador"	}	},
	{ MSTR_DETERMINING_ROTOR_TYPE	,	{	"Determining disc type",															"D�termination type de disque",																					"Scheibentyp festlegen",																							"Determinando el tipo de disco",																		"Determinazione tipo del disco",																					"A determinar o tipo de disco",																				"Determinando o tipo de disco"	}	},
	{ MSTR_DISABLED					,	{	"Disabled",																			"D�sactiv�",																									"Deaktiviert",																										"Inactivo",																								"Disattivato",																										"Inativo",																									"Desativado"	}	},
	{ MSTR_DOB						,	{	"DOB",																				"DOB",																											"GD",																												"F. nac.",																								"DN",																												"DDN",																										"DN"	}	},
	{ MSTR_DOCTOR_ID				,	{	"Doctor ID",																		"ID m�decin",																									"Arzt-ID",																											"Id. m�dico",																							"ID medico",																										"ID do m�dico",																								"ID do m�dico"	}	},
	{ MSTR_DOV						,	{	"DOV",																				"DOV",																											"LI",																												"�l. vac.",																								"DV",																												"DDV",																										"Data de vacina��o"	}	},
	{ MSTR_DRAWER_CYCLES			,	{	"Drawer Cycles",																	"Cycles tiroir",																								"Fachzyklen",																										"Ciclos del caj�n",																						"Cicli cassetto",																									"Ciclos da gaveta",																							"Ciclos da gaveta"	}	},
	{ MSTR_EGFR						,	{	"eGFR", 																			"eGFR",																											"eGFR",																												"eGFR",																									"eGFR",																												"TFGe",																										"TFGe"	}	},
	{ MSTR_ELECTROLYTES				,	{	"Electrolytes",																		"�lectrolytes",																									"Elektrolyte",																										"Electrolitos",																							"Elettroliti",																										"Eletr�litos",																								"Eletr�litos"	}	},
	{ MSTR_ENABLED					,	{	"Enabled",																			"Activ�",																										"Aktiviert",																										"Activado",																								"Attivato",																											"Ativado",																									"Ativado"	}	},
	{ MSTR_ENTER_AGE_RANGE			,	{	"Enter Age Range",																	"Saisir fourchette d'�ge",																						"Altersbereich eingeben",																							"Introducir intervalo de edad",																			"Inserisci fascia d'et�",																							"Introduzir intervalo de idades",																			"Inserir faixa et�ria"	}	},
	{ MSTR_ENTER_PATIENT_ID			,	{	"Enter Patient ID",																	"Entrer ID  patient",																							"Patienten-ID eingeben",																							"Introducir Id. del paciente",																			"Inserisci ID paziente",																							"Introduzir ID de doente",																					"Inserir ID do paciente"	}	},
	{ MSTR_ENZYMES					,	{	"Enzymes",																			"Enzymes",																										"Enzyme",																											"Encimas",																								"Enzimi",																											"Enzimas",																									"Enzimas"	}	},
	{ MSTR_ERROR_READING_CD			,	{	"Error reading CD",																	"Erreur lecture CD",																							"Fehler beim Lesen der CD",																							"Error al leer el CD",																					"Errore di lettura CD",																								"Erro de leitura do CD",																					"Erro ao ler o CD"	}	},
	{ MSTR_ERROR_REPORT				,	{	"Error Report",																		"Rapport erreur",																								"Fehlerbericht",																									"Informe de errores",																					"Rapporto di errore",																								"Relat�rio de erros",																						"Relat�rio de erros"	}	},
	{ MSTR_ERRORS					,	{	"Errors",																			"Erreurs",																										"Fehler",																											"Errores",																								"Errori",																											"Erros",																									"Erros"	}	},
	{ MSTR_EXPIRATION_DATE			,	{	"Expiration Date",																	"Date de p�remption",																							"Ablaufdatum",																										"Fecha de vencimiento",																					"Data di scadenza",																									"Prazo de validade",																						"Data de expira��o"	}	},
	{ MSTR_EXTERNAL_ARCHIVE			,	{	"External Archive",																	"Archive externe",																								"Externes Archiv",																									"Archivo externo",																						"Archivio esterno",																									"Ficheiro externo",																							"Arquivo externo"	}	},
	{ MSTR_EXTERNAL_PRINTER			,	{	"External Printer",																	"Imprimante externe",																							"Externer Drucker",																									"Impresora externa",																					"Stampante esterna",																								"Impressora externa",																						"Impressora externa"	}	},
	{ MSTR_FACTORY_DEFAULT_SPECIES	,	{	"Factory Default Demographics",														"Param./d�faut �lmts d�mogr.",																					"Voreingestellte Demographien",																						"Demograf�a predeterminada",																			"Demografia predefinita di fabbrica",																				"Valores de demografia de f�brica",																			"Dados demogr. padr�o de f�brica"	}	},
	{ MSTR_FEMALE					,	{	"Female",																			"F�minin",																										"Weiblich",																											"Mujer",																								"Uomo",																												"Feminino",																									"Sexo feminino"	}	},
	{ MSTR_FLASHES					,	{	"Flashes",																			"Flashs",																										"Blinkzeichen",																										"Flashes",																								"Flash luminosi",					  																				"Flashes",																									"Flashes"	}	},
	{ MSTR_FRACTION					,	{	"Fraction",																			"Fraction",																										"Anteil",																											"Fracci�n",																								"Frazione",																											"Fra��o",																									"Fra��o"	}	},
	{ MSTR_FULL						,	{	"Full",																				"Complet",																										"Voll",																												"Lleno",																								"Completo",																											"Completo",																									"Cheio"	}	},
	{ MSTR_GATEWAY_IP				,	{	"Gateway IP",																		"IP de passerelle",																								"Gateway-IP",																										"IP puerta enlace",																						"IP gateway",																										"IP da porta",																								"IP do gateway"	}	},
	{ MSTR_GENDER					,	{	"Gender",																			"Sexe",																											"Geschlecht",																										"Sexo",																									"Sesso",																											"Sexo",																										"G�nero"	}	},
	{ MSTR_HISPANIC					,	{	"Hispanic",																			"Hispanique",																									"Hispanische Abst.",																								"Hispano",																								"Ispanico",																											"Hisp�nico",																								"Hisp�nico"	}	},
	{ MSTR_HOUR						,	{	"Hour",																				"Heure",																										"Stunde",																											"Hora",																									"Ora",																												"Hora",																										"Hora"	}	},
	{ MSTR_HR						,	{	"hr",																				"h",																											"Std.",																												"hr",																									"Or",																												"h",																										"h"	}	},
	{ MSTR_INFANT					,	{	"Infant",																			"Nourrisson",																									"S�ugling",																											"Ni�o",																									"Cucciolo",																											"Crian�a",																									"Lactente"	}	},
	{ MSTR_INSERT_CD				,	{	"Insert CD, close drawer, and press Continue",										"Ins�rer CD, fermer tiroir et appuyer sur Continuer",															"CD einlegen, Fach schlie�en und Weiter dr�cken",																	"Insertar CD, cerrar bandeja y pulsar Continuar",														"Inserisci CD, chiudi cassetto e premi Continua",																	"Insira o CD, feche a gaveta e prima Continuar",															"Insira o CD, feche a gaveta e pressione Continuar"	}	},
	{ MSTR_INTERNAL_ARCHIVE			,	{	"Internal Archive",																	"Archive interne",																								"Internes Archiv",																									"Archivo interno",																						"Archivio interno",																									"Ficheiro interno",																							"Arquivo interno"	}	},
	{ MSTR_IP_MASK					,	{	"Subnet Mask",																		"Masq. sous-r�s",																								"Subnetzmaske",																										"M�sc. de subred",																						"Masch. sottorete",																									"M�scara de subrede",																						"M�scara de sub-rede"	}	},
	{ MSTR_IQC						,	{	"iQC",																				"iQC",																											"iQC",																												"iQC",																									"iQC",																												"iQC",																										"iQC"	}	},
	{ MSTR_IS_NOT_VALID				,	{	"is not valid",																		"incorrect",																									"ung�ltig",																											"no es v�lido",																							"non valido",																										"inv�lido",																									"n�o � v�lido"	}	},
	{ MSTR_KENNEL_NUMBER			,	{	"Kennel Number",																	"Num�ro chenil",																								"Zwingernr.",																										"N�mero de caseta",																						"Numero Kennel",																									"N�mero de Kennel",																							"N�mero do canil"	}	},
	{ MSTR_KEYBOARD					,	{	"Keyboard",																			"Clavier",																										"Eingabeger�t",																										"Teclado",																								"Tastiera",																											"Teclado",																									"Teclado"	}	},
	{ MSTR_LAST_ROTOR				,	{	"Last Disc",																		"Dernier disque",																								"Letzte Scheibe",																									"�ltimo disco",																							"Ultimo disco",																										"�ltimo disco",																								"�ltimo disco"	}	},
	{ MSTR_LAST_MODIFIED			,	{	"Last Modified",																	"Derni�re modif.",																								"Zuletzt ge�ndert",																									"�ltimo cambio",																						"Ultima modifica",																									"�ltima altera��o",																							"�ltima modifica��o"	}	},
	{ MSTR_LIPIDS					,	{	"Lipids",																			"Lipides",																										"Lipide",																											"L�pidos",																								"Lipidi",																											"L�pidos",																									"Lip�dios"	}	},
	{ MSTR_LOAD_FUSE_SETTINGS		,	{	"Load Fuse Settings", 																"Charger les param�tres de fusible",																			"Sicherungseinstellungen laden",																					"Cargar configuraci�n del fusible",																		"Carica impostazioni fusibile",																						"Carregar configura��es de fus�veis",																		"Carregar configura��es de fus�vel"	}	},
	{ MSTR_LOAD_ROTOR_TO_ANALYZE	,	{	"To analyze additional sample, load new disc",										"Pour analyser un �chantillon suppl�mentaire, charger un nouveau disque",										"Zur Analyse einer weiteren Probe neue Scheibe laden",																"Para analizar otra muestra, cargue un disco nuevo",													"Per esaminare ulteriori campioni, caricare nuovo disco",															"Para analisar outra amostra, carregue um novo disco",														"Para analisar uma amostra adicional, carregue um novo disco"	}	},
	{ MSTR_LOCATION					,	{	"Location",																			"Emplacement",																									"Ort",																												"Ubicaci�n",																							"Sede",																												"Localiza��o",																								"Local"	}	},
	{ MSTR_LOCKOUT					,	{	"Lockout",																			"Verrouillage",																									"Verriegelung",																										"Bloqueo",																								"Serrata",																											"Bloqueio",																									"Bloquear"	}	},
	{ MSTR_LOWER					,	{	"Lower",																			"Inf�rieur",																									"Niedriger",																										"Inferior",																								"Basso",																											"Inferior",																									"Inferior"	}	},
	{ MSTR_MALE						,	{	"Male",																				"Masculin",																										"M�nnlich",																											"Hombre",																								"Donna",																											"Masculino",																								"Sexo masculino"	}	},
	{ MSTR_MANUAL_SETTINGS			,	{	"Manual Settings", 																	"Param�tres manuels",																							"Manuelle Einstellungen",																							"Configuraci�n manual",																					"Impostazioni manuali",																								"Configura��es manuais",																					"Configura��es manuais"	}	},
	{ MSTR_MIN						,	{	"min",																				"min",																											"min.",																												"min",																									"min",																												"min",																										"min"	}	},
	{ MSTR_MINERALS					,	{	"Minerals",																			"Min�raux",																										"Mineralien",																										"Minerales",																							"Minerali",																											"Minerais",																									"Minerais"	}	},
	{ MSTR_MINUTE					,	{	"Minute",																			"Minute",																										"Minute",																											"Minuto",																								"Minuto",																											"Minuto",																									"Minuto"	}	},
	{ MSTR_MONTH					,	{	"Month",																			"Mois",																											"Monat",																											"Mes",																									"Mese",																												"M�s",																										"M�s"	}	},
	{ MSTR_MOS						,	{	"Mos.",																				"Mois",																											"Monate",																											"Meses",																								"Meses",																											"Meses",																									"Meses"	}	},
	{ MSTR_NAME						,	{	"Name:",																			"Nom :",																										"Name:",																											"Nombre:",																								"Nome:",																											"Nome:",																									"Nome:"	}	},
	{ MSTR_NATIVE_AMERICAN			,	{	"Native American",																	"Am�rindienne",																									"Indianische Abst.",																								"Americano nativo",																						"Nativo americano",																									"Americano nativo",																							"Americano nativo"	}	},
	{ MSTR_NEONATE					,	{	"Neonate",																			"Nouveau-n�",																									"Neugeborenes",																										"Neonato",																								"Neonato",																											"Rec�m-nascido",																							"Neonato"	}	},
	{ MSTR_NEUTERED					,	{	"Neutered",																			"Ch�tr�",																										"Kastriert",																										"Castrado",																								"Castrato",																											"Castrado",																									"Castrado"	}	},
	{ MSTR_NEVER					,	{	"Never",																			"Jamais",																										"Nie",																												"Nunca",																								"Mai",																												"Nunca",																									"Nunca"	}	},
	{ MSTR_NEW						,	{	"New",																				"Nouveau",																										"Neu",																												"Nuevo",																								"Nuovo",																											"Novo",																										"Novo"	}	},
	{ MSTR_NO_RESULTS				,	{	"No saved results to view. Please run a disc prior to recalling results.",			"Aucun r�sultat enregistr� � afficher. Ex�cuter un disque avant de rappeler les r�sultats.",					"Keine gespeicherten Ergebnisse zur Anzeige. Vor Wiederaufrufen der Ergebnisse eine Scheibe starten.",				"No hay resultados guardados para ver. Utilice un disco antes de buscar resultados.",					"Nessun risultato salvato da visualizzare. Avviare un disco prima di richiamare i risultati.",						"No existem resultados guardados para visualizar. Utilize um disco antes de recuperar resultados.",			"Nenhum resultado salvo para visualiza��o. Execute um disco antes de recuperar os resultados."	}	},
	{ MSTR_NON_SI					,	{	"Non SI",																			"Non SI",																										"Kein SI",																											"No SI",																								"Non SI",																											"N�o SI",																									"N�o SI"	}	},
	{ MSTR_NONE						,	{	"None",																				"Aucun",																										"Kein",																												"Ninguno",																								"Nessuno",																											"Nenhum",																									"Nenhum"	}	},
	{ MSTR_OFF						,	{	"Off",																				"Off",																											"Aus",																												"Apagado",																								"Off",																												"Desligado",																								"Desligado"	}	},
	{ MSTR_OPENING_DRAWER			,	{	"Opening Drawer...",																"Ouverture tiroir en cours...",																					"Fach �ffnen ...",																									"Abriendo bandeja...",																					"Apertura cassetto in corso...",																					"A abrir a gaveta...",																						"Abrindo gaveta..."	}	},
	{ MSTR_OPERATOR_ID				,	{	"Operator ID",																		"ID op�rateur",																									"Bediener-ID",																										"Id. operario:",																						"ID operatore",																										"ID do operador",																							"ID do operador"	}	},
	{ MSTR_OPERATORS				,	{	"Operators",																		"Op�rateurs",																									"Bediener",																											"Operarios",																							"Operatori",																										"Operadores",																								"Operadores"	}	},
	{ MSTR_OWNER_ID					,	{	"Owner ID",																			"ID propri�taire",																								"Besitzer-ID",																										"Id. due�o",																							"ID propr.",																										"ID do propriet�rio",																						"ID do propriet�rio"	}	},
	{ MSTR_PATIENT_CONTROL_ID		,	{	"Patient/Control ID",																"ID patient/contr�le",																							"Patienten-/Kontrollen-ID",																							"Id. paciente/control",																					"ID paziente/controllo",																							"ID do doente/controlo",																					"ID do paciente/controle"	}	},
	{ MSTR_PATIENT_ID				,	{	"Patient ID",																		"ID patient",																									"Patienten-ID",																										"Id. paciente",																							"ID paziente",																										"ID do doente",																								"ID do paciente"	}	},
	{ MSTR_PATIENT_TYPE				,	{	"Patient Type",																		"Type de patient",																								"Patiententyp",																										"Tipo de paciente",																						"Tipo di paziente",																									"Tipo de doente",																							"Tipo de paciente"	}	},
	{ MSTR_PATIENTS					,	{	"Patients",																			"Patients",																										"Patienten",																										"Pacientes",																							"Pazienti",																											"Doentes",																									"Pacientes"	}	},
	{ MSTR_PENDING_ORDER			,	{	"Pending Order",																	"Ordre en cours",																								"Ausstehende Anforderung",																							"Pedido pendiente",																						"Ordine in sospeso",																								"Pedido pendente",																							"Solicita��o pendente"	}	},
	{ MSTR_PHONE_NUMBER				,	{	"Phone Number",																		"N� t�l",																										"Telefonnummer",																									"N�mero de tel�fono",																					"Num. telefono",																									"N�mero de telefone",																						"N�mero de telefone"	}	},
	{ MSTR_PLEASE_REMOVE_ROTOR		,	{	"Please remove the disc from the analyzer",											"Retirer le disque de l'analyseur",																				"Bitte die Scheibe aus dem Analyseger�t entfernen",																	"Retire el disco del analizador",																		"Rimuovere il disco dall'analizzatore",																				"Retire o disco do analisador",																				"Remova o disco do analisador"	}	},
	{ MSTR_PLEASE_WAIT_CDUPDATE		,	{	"Please wait. Do not remove power. Analyzer will restart when update is complete.", "Veuillez patienter. Ne pas couper l'alimentation. L'analyseur red�marre une fois la mise � jour termin�e.",	"Bitte warten. Bitte Stromversorgung nicht abschalten. Analyseger�t wird nach vollst�ndigem Update neu gestartet.",	"Espere, por favor. No apague el aparato. El analizador se reiniciar� cuando acabe la actualizaci�n.",	"Si prega di attendere. Non togliere corrente. L'analizzatore si riavvier� al completamento dell'aggiornamento.",	"Espere, por favor. N�o desligue. O analisador ser� reiniciado quando terminar a atualiza��o.",				"Aguarde. N�o remova a energia. O analisador ser� reiniciado quando a atualiza��o concluir."	}	},
	{ MSTR_POTC_1A					,	{	"POTC-1A",																			"POTC-1A",																										"POTC-1A",																											"POTC-1A",																								"POTC-1A",																											"POTC-1A",																									"POTC-1A"	}	},
	{ MSTR_POWER_SAVE				,	{	"Power Save",																		"�cono. alim.",																									"Energiesparmodus",																									"Ahorro de energ�a",																					"Modo risparmio",																									"Poupan�a de energia",																						"Economia de energia"	}	},
	{ MSTR_PREMATURE				,	{	"Premature",																		"Pr�matur�",																									"Vorzeitig",																										"Prematuro",																							"Prematuro",																										"Prematuro",																								"Prematuro"	}	},
	{ MSTR_PRESS_YES_DEFAULTS		,	{	"Press Yes to return all settings to factory defaults",								"Appuyer sur Oui pour r�tablir tous les param�tres par d�faut",													"Dr�cken Sie Ja, um die Werkseinstellungen wieder herzustellen",													"Pulse S� para reestablecer la configuraci�n predeterminada original",									"Premere S� per ripristinare tutte le impostazioni di fabbrica",													"Prima Sim para reestabelecer a configura��o para os valores predeterminados de f�brica",					"Pressione Sim para retornar todas as configura��es aos padr�es de f�brica"	}	},
	{ MSTR_PRESS_YES_REF_RANGES		,	{	"Press Yes to return the active demographics list to the factory default",			"Appuyer sur Oui pour r�tablir la liste d'�l�ments d�mographiques actifs par d�faut",							"Dr�cken Sie Ja, um die Werkseinstellungen f�r die Liste der aktiven Demographien wieder herzustellen",				"Pulse S� para reestablecer los valores predeterminados originales de la lista de demograf�as activas",	"Premere S� per mandare l'elenco demografie attive nelle impostazioni di fabbrica",									"Prima Sim para reestabelecer a lista de demografias ativa para os valores predeterminados de f�brica",		"Pressione Sim para retornar a lista de dados demogr�ficos ativos ao padr�o de f�brica"	}	},
	{ MSTR_PRESS_YES_TO_START		,	{	"Press Yes to start",																"Appuyer sur Oui pour red�marrer",																				"Dr�cken Sie Ja f�r Start",																							"Pulse S� para empezar",																				"Premere S� per avviare",																							"Prima Sim para iniciar",																					"Pressione Sim para come�ar"	}	},
	{ MSTR_PRINT					,	{	"Print",																			"Impr.",																										"Drucken",																											"Imprimir",																								"Stampa",																											"Imprimir",																									"Imprimir"	}	},
	{ MSTR_PRINTED					,	{	"Printed",																			"Imprim�",																										"Gedruckt",																											"Impreso",																								"Stampato",																											"Impresso",																									"Impresso"	}	},
	{ MSTR_PRINTER					,	{	"Printer",																			"Imprimante",																									"Drucker",																											"Impr.",																								"Stampante",																										"Impressora",																								"Impressora"	}	},
	{ MSTR_PROTEINS					,	{	"Proteins",																			"Prot�ines",																									"Proteine",																											"Prote�nas",																							"Proteine",																											"Prote�nas",																								"Prote�nas"	}	},
	{ MSTR_RACE						,	{	"Race",																				"Race",																											"Rasse",																											"Raza",																									"Razza",																											"Corrida",																									"Ra�a"	}	},
	{ MSTR_READING_CD				,	{	"Reading CD...",																	"Lecture du CD...",																								"CD wird gelesen...",																								"Leyendo CD...",																						"Lettura CD...",																									"A ler o CD...",																							"Lendo CD..."	}	},
	{ MSTR_RECALL_RESULTS			,	{	"Recall Results", 																	"Rappeler r�sultats",																							"Ergebnisse neu abrufen",																							"Buscar resultados",																					"Recupera i risultati",																								"Recuperar resultados",																						"Recuperar resultados"	}	},
	{ MSTR_REMOVE_SPECIES			,	{	"Remove Demographic",																"Retirer �l�ments d�mographiques",																				"Demographien entfernen",																							"Eliminar demograf�a",																					"Rimuovi demografia",																								"Eliminar demografia",																						"Remover dado demogr�fico"	}	},
	{ MSTR_RESULTS					,	{	"Results",																			"R�sultats",																									"Ergebnisse",																										"Result.",																								"Risultati",																										"Resultados",																								"Resultados"	}	},
	{ MSTR_RESULTS_QUEUED			,	{	"Results Queued",																	"R�sultats en file d'attente",																					"Zu �bertragende Ergebnisse",																						"Resultados en cola",																					"Risultati accodati",																								"Resultados em fila",																						"Resultados em fila"	}	},
	{ MSTR_RETRIEVE					,	{	"Retrieve",																			"R�cup�rer",																									"Abrufen",																											"Recuperar",																							"Recupera",																											"Recuperar",																								"Recuperar"	}	},
	{ MSTR_ROTORS_RUN				,	{	"Discs Run",																		"Ex�cution disques",																							"Scheiben in Betrieb",																								"Usar discos",																							"Disco in funzione",																								"Utilizar discos",																							"Execu��o de discos"	}	},
	{ MSTR_SAMPLE_ID				,	{	"Sample ID",																		"ID �chantillon",																								"Proben-ID",																										"Id. de muestra",																						"ID campione",																										"ID da amostra",																							"ID da amostra"	}	},
	{ MSTR_SAVING_RESULTS			,	{	"Saving Results...",																"Enregistrement des r�sultats en cours...",																		"Ergebnisse werden gespeichert...",																					"Guardando resultados...",																				"Salvataggio risultati...",																							"A guardar os resultados...",																					"Salvando resultados..."	}	},
	{ MSTR_SCREEN_CLICKS			,	{	"Screen Clicks",																	"Clics �cran",																									"Bildschirmklicks",																									"Clics de pantalla",																					"Clic schermo",																										"Cliques no ecr�",																							"Cliques de tela"	}	},
	{ MSTR_SCREEN_SAVER				,	{	"Screen Saver",																		"�conomiseur d'�cran",																							"Bildschirmschoner",																								"Salvapantalla",																						"Screensaver",																										"Prote��o do ecr�",																							"Protetor de tela"	}	},
	{ MSTR_SEARCH					,	{	"Search",																			"Rechercher",																									"Suchen",																											"B�squeda",																								"Cerca",																											"Pesquisar",																								"Pesquisar"	}	},
	{ MSTR_SELECT					,	{	"Select",																			"S�lectionner",																									"Ausw�hlen",																										"Seleccionar",																							"Seleziona",																										"Selecionar",																								"Selecionar"	}	},
	{ MSTR_SELECT_REPORTS			,	{	"Select Reports",																	"S�lectionner rapports",																						"Berichte ausw�hlen",																								"Seleccionar informes",																					"Seleziona rapporti",																								"Selecionar relat�rios",																					"Selecionar relat�rios"	}	},
	{ MSTR_SEND						,	{	"Send",																				"Envoyer",																										"Senden",																											"Enviar",																								"Invia",																											"Enviar",																									"Enviar"	}	},
	{ MSTR_SENDING					,	{	"Sending...",																		"Envoi en cours...",																							"Sendet ...",																										"Enviando...",																							"Invio in corso...",																								"A enviar...",																								"Enviando..."	}	},
	{ MSTR_SENDING_REPORT			,	{	"Sending Report...",																"Envoi de rapport en cours...",																					"Sende Bericht ...",																								"Enviando informe...",																					"Invio rapporto in corso...",																						"A enviar relat�rio...",																					"Enviando relat�rio..."	}	},
	{ MSTR_SERIAL_NO				,	{	"Serial No.:",																		"N� de s�rie :",																								"Seriennr.:",																										"N� de serie:",																							"Numero di serie:",																									"N.� de s�rie:",																							"No. de s�rie:"	}	},
	{ MSTR_SERVER_IP				,	{	"Server IP",																		"IP du serveur",																								"Server-IP",																										"IP de servidor",																						"IP server",																										"IP do servidor",																							"IP do servidor"	}	},
	{ MSTR_SERVER_SOCKET			,	{	"Server Socket",																	"Socket serveur",																								"Server-Socket",																									"Socket servidor",																						"Socket server",																									"Tomada do servidor",																						"Soquete do servidor"	}	},
	{ MSTR_SERVICE_ARCHIVE			,	{	"Service Archive",																	"Archive service",																								"Service-Archiv",																									"Archivo de servicio",																					"Archivio di servizio",																								"Ficheiro do servi�o",																						"Arquivo de servi�o"	}	},
	{ MSTR_SERVICE_ARCHIVE_ELLIPSIS	,	{	"Service Archive...",																"Archive service...",																							"Service-Archiv ...",																								"Archivo de servicio...",																				"Archivio di servizio...",																							"Ficheiro do servi�o...",																					"Arquivo de servi�o..."	}	},
	{ MSTR_SET_DEFAULT				,	{	"Set Default",																		"D�f. param./d�faut",																							"Standard einstellen",																								"Establ. predeterm.",																					"Imposta predef.",																									"Configurar valores predeterminados",																		"Configurar padr�o"	}	},
	{ MSTR_SETTINGS					,	{	"Settings",																			"Param�tres",																									"Einstellungen",																									"Configuraci�n",																						"Impostazioni",																										"Configura��es",																							"Configura��es"	}	},	
	{ MSTR_SI						,	{	"SI",																				"SI",																											"SI",																												"SI",																									"SI",																												"SI",																										"SI"	}	},
	{ MSTR_SINGLE_ANALYTE			,	{	"Single Analyte",																	"1 seule subs. � analyser",																						"Einzelnes Analyt",																									"Analito �nico",																						"Analita singolo",																									"Analito �nico",																							"Analito �nico"	}	},
	{ MSTR_SPAYED					,	{	"Spayed",																			"Ch�tr�e",																										"Sterilisiert",																										"Castrada",																								"Sterilizzato",																										"Esterilizado",																								"Esterilizado"	}	},
	{ MSTR_SPECIAL					,	{	"Special",																			"Sp�cial",																										"Spezial",																											"Especial",																								"Speciale",																											"Especial",																									"Especial"	}	},
	{ MSTR_SPECIES					,	{	"Demographic",																		"�lmts d�mogr.",																								"Demographien",																										"Demograf�a",																							"Demografia",																										"Demografia",																								"Dado demogr�fico"	}	},
	{ MSTR_STATUS					,	{	"Status",																			"�tat",																											"Status",																											"Estado",																								"Stato",																											"Estado",																									"Status"	}	},
	{ MSTR_TCPIP					,	{	"TCP/IP",																			"TCP/IP",																										"TCP/IP",																											"TCP/IP",																								"TCP/IP",																											"TCP/IP",																									"TCP/IP"	}	},
	{ MSTR_TEST						,	{	"Test",																				"Test",																											"Test",																												"Prueba",																								"Prova",																											"Teste",																									"Teste"	}	},
	{ MSTR_TESTING_CONNECTION		,	{	"Testing connection...",															"Test de la connexion en cours...",																				"Verbindung wird gepr�ft...",																						"Probando conexi�n...",																					"Test della connessione...",																						"A testar liga��o...",																						"Testando conex�o..."	}	},
	{ MSTR_THIS_ANALYZER			,	{	"This Analyzer",																	"Cet analyseur",																								"Dieses Analyseger�t",																								"Este analizador",																						"Questo analizzatore",																								"Este analisador",																							"Este analisador"	}	},
	{ MSTR_TODDLER					,	{	"Toddler",																			"B�b�",																											"Kleinkind",																										"P�rvulo",																								"Bambino ai primi passi",																							"Beb�",																										"Crian�a"	}	},
	{ MSTR_TOUCH_SCREEN_TO_CALIB	,	{	"Touch screen to calibrate or wait to cancel.",										"Toucher l'�cran pour calibrer ou attendre pour annuler.",														"Zur Kalibrierung Bildschirm ber�hren oder Abbruch abwarten.",														"Toque la pantalla para calibrar o espere para cancelar.",												"Toccare lo schermo per calibrare, oppure attendere per annullare.",												"Toque no ecr� para calibrar ou espere para cancelar.",														"Toque na tela para calibrar ou aguarde para cancelar."	}	},
	{ MSTR_TRANSMIT_ALL				,	{	"Transmit All",																		"Transmettre tout",																								"Alle �bertragen",																									"Transmitir todo",																						"Trasmetti tutti",																									"Transmitir tudo",																							"Transmitir todos"	}	},
	{ MSTR_TRANSMIT_DATE_RANGE		,	{	"Transmit Date Range",																"Transm. plage de dates",																						"Datumsbereich �bertr.",																							"Trans. intervalo/fechas",																				"Trasmetti range di date",																							"Transmitir intervalo de data",																				"Transmitir intervalo de datas"	}	},
	{ MSTR_TRANSMITTING_ELLIPSIS	,	{	"Transmitting...",																	"Transmission...",																								"�bertragung...",																									"Transmitiendo...",																						"Trasmissione in corso...",																							"A transmitir...",																							"Transmitindo..."	}	},
	{ MSTR_TRANSMITTING_ALL			,	{	"Transmitting All...",																"Trans. en cours...",																							"Alle werden �bertr. ...",																							"Transmitiendo todo...",																				"Trasm. tutti in corso...",																							"A transmitir tudo...",																						"Transmitindo todos..."	}	},
	{ MSTR_UNITS					,	{	"Units",																			"Unit�s",																										"Einheiten",																										"Unidades",																								"Unit�",																											"Unidades",																									"Unidades"	}	},
	{ MSTR_UNKNOWN					,	{	"Unknown",																			"Inconnu",																										"Unbekannt",																										"Desconocido",																							"Sconosciuto",																										"Desconhecido",																								"Desconhecido"	}	},
	{ MSTR_UPDATE_SOFTWARE			,	{	"Update Software",																	"Mise � jour logiciel",																							"Software aktualisieren",																							"Actualizar programa",																					"Aggiorna software",																								"Atualizar o Software",																						"Atualizar software"	}	},
	{ MSTR_UPDATING_CONTROLS		,	{	"Updating Controls...",																"Actualisation des commandes en cours...",																		"Steuerungen werden aktualisiert ...",																				"Actualizando controles...",																			"Aggiornamento controlli in corso...",																				"A atualizar controlos...",																					"Atualizando controles..."	}	},
	{ MSTR_UPDATING_SOFTWARE		,	{	"Updating Software...",																"Mise � jour logiciel...",																						"Softw. wird aktual. ...",																							"Actualizando programa",																				"Aggior. softw...",																									"A atualizar o Software...",																				"Atualizando software..."	}	},
	{ MSTR_UPPER					,	{	"Upper",																			"Haut",																											"Oben",																												"Superior",																								"Superiore",																										"Superior",																									"Superior"	}	},
	{ MSTR_USB						,	{	"USB (Serial)",																		"USB (s�rie)",																									"USB (ser. Anschl.)",																								"USB (serie)",																							"USB (seriale)",																									"USB (s�rie)",																								"USB (serial)"	}	},
	{ MSTR_VERSION					,	{	"Version:",																			"Version :",																									"Version:",																											"Versi�n:",																								"Versione:",																										"Vers�o:",																									"Vers�o:"	}	},
	{ MSTR_WARMING					,	{	"Warming",																			"R�chauffement",																								"Aufw�rmen",																										"Calentando",																							"Riscaldamento",																									"A aquecer",																								"Aquecendo"	}	},
	{ MSTR_WHITE					,	{	"White",																			"Blanc",																										"Weiss",																											"Blanco",																								"Bianco",																											"Branco",																									"Branco"	}	},
	{ MSTR_WKS						,	{	"Wks.",																				"Sem.",																											"Wks.",																												"Sem.",																									"Sett.",																											"Sem.",																										"Semanas"	}	},
	{ MSTR_WORK_LIST				,	{	"Work List",																		"Liste de t�ches",																								"Auftragsliste",																									"Lista de trabajo",																						"Elenco di lavoro",																									"Lista de trabalho",																						"Lista de trabalho"	}	},
	{ MSTR_XMIT						,	{	"Xmit",																				"Trans",																										"Xmit",																												"Transm.",																								"Xmit",																												"Transm.",																									"Transmit"	}	},
	{ MSTR_XML						,	{	"XML",																				"XML",																											"XML",																												"XML",																									"XML",																												"XML",																										"XML"	}	},
	{ MSTR_YEAR						,	{	"Year",																				"Ann�e",																										"Jahr",																												"A�o",																									"Anno",																												"Ano",																										"Ano"	}	},
	{ MSTR_YRS						,	{	"Yrs.",																				"Ans.",																											"Jahre",																											"A�os",																									"Anni",																												"Anos",																										"Anos"	}	},
	{ MSTR_ZERO_SEC					,	{	"Zero Sec",																			"Z�ro sec",																										"Null Sek.",																										"Cero seg",																								"Zero Sec",																											"Zero seg",																									"Zero seg"	}	},

// Bottom									ENGLISH,				FRENCH,							GERMAN,						SPANISH,				ITALIAN, 					PORTUGUESE,				PORTUGUESE (BRAZIL)
	{ BSTR_ADD						,	{	"Add",					"Ajouter",						"Hinzuf�gen",				"A�adir",				"Aggiungi",					"Adicionar",			"Adicionar"	}	},
	{ BSTR_BACK						,	{	"Back",					"Retour",						"Zur�ck",					"Volver",				"Indietro",					"Voltar",				"Voltar"	}	},
	{ BSTR_CANCEL					,	{	"Cancel",				"Annuler",						"Abbrechen",				"Cancelar",				"Annulla",					"Cancelar",				"Cancelar"	}	},
	{ BSTR_CLOSE 					,	{	"Close",				"Fermer",						"Schlie�en",				"Cerrar",				"Chiudi",					"Fechar",				"Fechar"	}	},
	{ BSTR_CONFIRM					,	{	"Confirm",				"Confirmer",					"Best�tigen",				"Confirmar",			"Conferma",					"Confirmar",			"Confirmar"	}	},
	{ BSTR_CONTINUE					,	{	"Continue",				"Continuer",					"Fortsetzen",				"Continuar",			"Continua",					"Continuar",			"Continuar"	}	},
	{ BSTR_DATE						,	{	"Date",					"Date",							"Datum",					"Fecha",				"Data",						"Data",					"Data"		}	},
	{ BSTR_DONE						,	{	"Done",					"Termin�",						"Erledigt",					"Terminado",			"Fatto",					"Terminar",				"Conclu�do"	}	},
	{ BSTR_ENTER					,	{	"Enter",				"Entr�e",						"Eingabe",					"Entrar",				"Invio",					"Introduzir",			"Entrar"	}	},
	{ BSTR_EXIT						,	{	"Exit",					"Quit",							"Exit",						"Salir",				"Esci",						"Sair",					"Sair"		}	},
	{ BSTR_HOME						,	{	"Home ",				"Accueil",						"Home",						"Inicio",				"Home",						"In�cio",				"In�cio"	}	},
	{ BSTR_NEXT						,	{	"Next",					"Suivant",						"Weiter",					"Sig.",					"Successivo",				"Seguinte",				"Pr�ximo"	}	},
	{ BSTR_NO						,	{	"No",					"Non",							"Nein",						"No",					"No",						"N�o",					"N�o"		}	},
	{ BSTR_OPEN						,	{	"Open",					"Ouvrir",						"�ffnen",					"Abrir",				"Aperto",					"Abrir",				"Abrir"		}	},
	{ BSTR_PERFORMING_IQC			,	{	"Performing iQC...",	"Ex�cution iQC en cours...",	"iQC wird ausgef�hrt...",	"Realizando iQC...",	"Esecuz. iQC in corso...",	"A executar iQC...",	"Executando iQC..."	}	},
	{ BSTR_PRINT					,	{	"Print",				"Impr.",						"Drucken",					"Impr.",				"Stampa",					"Imprimir",				"Imprimir"	}	},
	{ BSTR_RESTART					,	{	"Restart",				"Red�marrer",					"Erneut starten",			"Reiniciar",			"Riavvia",					"Reiniciar",			"Reiniciar"	}	},
	{ BSTR_SAVE						,	{	"Save",					"Enreg.",						"Save",						"Guard.",				"Salva",					"Guardar",				"Salvar"	}	},
	{ BSTR_SEARCH					,	{	"Search",				"Rechercher",					"Suchen",					"B�squeda",				"Cerca",					"Pesquisar",			"Pesquisar"	}	},
	{ BSTR_YES						,	{	"Yes",					"Oui",							"Ja",						"S�",					"S�",						"Sim",					"Sim"		}	},

// Info text
	{ ISTR_LOAD_ROTOR_1				,	{	"Remove reagent disc from its foil pouch and transfer 100ul of whole blood, plasma or serum into the disc.\n",					// English
											"Retirer le disque de r�actif de sa pochette alu et transf�rer 100 ul de sang entier, de plasma ou de s�rum dans le rotor.\n",	// French
											"Reagenzscheibe aus dem Plastikbeutel entfernen und 100ul Vollblut, Plasma oder Serum in die Scheibe einf�llen.\n",				// German
											"Retire el disco de reactivo de su bolsa metalizada y transfiera 100 ul de sangre, plasma o suero al disco.\n",					// Spanish
											"Rimuovere il disco reagente dalla custodia metallizzata e trasferire 100ul di sangue intero, plasma o siero nel disco.\n",		// Italian
											"Retire o disco de reagente da sua bolsa metalizada e transfira 100 ul de sangue total, plasma ou soro para o disco.\n",		// Portuguese
											"Remova o disco de reagente da bolsa de alum�nio e transfira 100 ul de sangue total, plasma ou soro para o disco.\n"			// Portuguese (Brazil)
										} 
	},
	{ ISTR_LOAD_ROTOR_2				,	{	"Insert the reagent disc into the analyzer, close the drawer and follow the on-screen prompts.\n",								// English
											"Ins�rer le disque de r�actif dans l'analyseur, fermer le tiroir et suivre les instructions qui s'affichent � l'�cran.\n",		// French
											"Reagenzscheibe in das Analyseger�t einsetzen, Fach schlie�en und Bildschirmanweisungen befolgen.\n",							// German
											"Inserte el disco de reactivo en el analizador, cierre el caj�n y siga las instrucciones de pantalla.\n",						// Spanish
											"Inserire il disco reagente nell'analizzatore, chiudere il cassetto e seguire le istruzioni a schermo.\n",						// Italian
											"Insira o disco de reagente no analisador, feche a gaveta e siga as instru��es do ecr�.\n",										// Portuguese
											"Insira o disco de reagente no analisador, feche a gaveta e siga as instru��es na tela.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_LOAD_ROTOR_3				,	{	"View, read or download results.\n",				// English
											"Afficher, lire et t�l�charger les r�sultats.\n",	// French
											"Ergebnisse ansehen, lesen oder herunterladen.\n",	// German
											"Visualice, lea o descargue resultados.\n",			// Spanish
											"Visualizzare, leggere o scaricare i risultati.\n",	// Italian
											"Visualize, leia ou descarregue os resultados.\n",	// Portuguese
											"Visualizar, ler ou baixar resultados.\n"			// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_RESTORE			,	{	"Restores reference ranges to a particular date and time. The restored reference ranges replace the current reference ranges. All changes after the date reference ranges were archived will be lost. It is recommended that an archive of the current reference ranges be made prior to restoring an earlier set.\n",																				// English
											"R�tablit les fourchettes de r�f�rence sur une date et une heure donn�es. Les fourchettes de r�f�rence r�tablies remplacent les fourchettes de r�f�rence actuelles. Tous les changements effectu�s apr�s l'archivage des fourchettes de date de r�f�rences seront perdus. Il est conseill� de cr�er une archive des fourchettes de r�f�rence actuelles avant de r�tablir un ensemble ant�rieur.\n",	// French
											"Setzt die Referenzbereiche auf ein bestimmtes Datum und Zeit zur�ck. Die wiederhergestellten Referenzbereiche ersetzen die aktuellen Referenzbereiche. S�mtliche nach der Archivierung der Datumsreferenzbereiche vorgenommenen �nderungen gehen verloren. Es wird empfohlen, die aktuellen Referenzbereiche zu archivieren, vor Wiederherstellung eines fr�heren Satzes.\n",						// German
											"Recupera los intervalos de referencia de una fecha y hora espec�ficos. Los intervalos de referencia reestablecidos reemplazan a los intervalos de referencia actuales. Se perder�n todos los cambios tras la fecha en la que se archivaron los intervalos de referencia. Se recomienda hacer un archivo de los intervalos de referencia actuales antes de reestablecer un grupo anterior.\n",		// Spanish
											"Ripristina i range di riferimento ad una particolare data e ora. I range di riferimento ripristinati sostituiscono i range di riferimento attuali. Tutte le modifiche successive alla data di archiviazione dei range di riferimento andranno perse. Si raccomanda di creare un archivio degli attuali range di riferimento prima di ripristinare un set precedente.\n",							// Italian
											"Restaura os intervalos de refer�ncia de una data e hora espec�ficas. Os intervalos de refer�ncia restaurados substituem os intervalos de refer�ncia atuais. Ser�o perdidas todas as altera��es ap�s a data do arquivo dos intervalos de refer�ncia. � recomendado realizar um arquivo dos intervalos de refer�ncia atuais antes de recuperar um conjunto anterior.\n",								// Portuguese
											"Restaura os intervalos de refer�ncia para uma data e hora espec�ficas. Os intervalos de refer�ncia restaurados substituem os intervalos de refer�ncia atuais. Todas as altera��es ap�s o arquivamento dos intervalos de refer�ncia de datas ser�o perdidas. � recomend�vel fazer um arquivo dos intervalos de refer�ncia atuais antes de restaurar um conjunto anterior.\n"						// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_SELECT			,	{	"Send: Archive reference ranges for future reference.\n\n"
											"Retrieve: Restore an internal reference range archive.\n",											// English
											"Envoyer : archiver les fourchettes de r�f�rence afin de pouvoir s'y reporter ult�rieurement.\n\n"
											"R�cup�rer : restaurer une archive de fourchette de r�f�rence interne.\n",							// French
											"Senden: Referenzbereiche f�r zuk�nftige Verwendung archivieren.\n\n"
											"Empfangen: Internes Archiv f�r Referenzbereiche wiederherstellen.\n",								// German
											"Enviar: archivar intervalos de referencia para usos futuros.\n\n"
											"Recuperar: reestablecer un archivo de intervalos de referencia interno.\n",						// Spanish
											"Invia: archivia range di riferimento per utilizzo futuro.\n\n"
											"Recupera: ripristina un archivio di range di riferimento interno.\n",								// Italian
											"Enviar: Guardar intervalos de refer�ncia para utiliza��o futura.\n\n"
											"Recuperar: Restaura um ficheiro de intervalos de refer�ncia interno.\n",							// Portuguese
											"Enviar: Arquive os intervalos de refer�ncia para futura refer�ncia.\n\n"
											"Recuperar: Restaure um arquivo de intervalo de refer�ncia interno.\n"								// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_SEND				,	{	"Printer: Print a copy of a demographic reference range.\n\n"
											"Internal Archive: Save a copy of all active reference ranges internal to the analyzer for future recall.\n\n"
											"External Archive: Transmit an ASCII dump of a demographic reference range to an external system.\n",																	// English
											"Imprimante : imprimer un exemplaire de fourchette de r�f�rence d�mographique.\n\n"
											"Archive interne : enregistrer un exemplaire de toutes les fourchettes de r�f�rence actives internes � l'analyseur afin de pouvoir les r�cup�rer ult�rieurement.\n\n"
											"Archive externe : transmettre un vidage de m�moire ASCII d'une fourchette de r�f�rence d�mographique � un syst�me externe.\n",											// French
											"Drucker: Kopie eines Demographie-Referenzbereichs drucken.\n\n"
											"Internes Archiv: Eine Kopie aller aktiven Referenzbereiche f�r sp�tere Verwendung intern im Analyseger�t speichern.\n\n"
											"Externes Archiv: ASCII-Auszug eines Demographie-Referenzbereichs an ein externes System �bertragen.\n",																// German
											"Impresora: imprimir una copia de un intervalo de referencia de demograf�a.\n\n"
											"Archivo interno: guardar una copia de todos los intervalos de referencia activos internos para el analizador para usos futuros.\n\n"
											"Archivo externo: hacer un volcado de datos ASCII de un intervalo de referencia de demograf�a a un sistema externo.\n",													// Spanish
											"Stampante: stampa una copia di un range di riferimento per demografia.\n\n"
											"Archivio interno: salva una copia di tutti i range di riferimento attivi interni all'analizzatore per utilizzo futuro.\n\n"
											"Archivio esterno: trasmette una serie ASCII di range di riferimento per demografia ad un sistema esterno.\n",															// Italian
											"Impressora: Imprime uma c�pia de um intervalo de refer�ncia de demografia.\n\n"
											"Ficheiro interno: Guardar uma c�pia de todos os intervalos de refer�ncia ativos internos no analisador para utiliza��o futura.\n\n"
											"Ficheiro externo: Transmitir um dump de ASCII de um intervalo de refer�ncia de demografia para um sistema externo.\n",													// Portuguese
											"Impressora: Imprima uma c�pia de um intervalo de refer�ncia de dados demogr�ficos.\n\n"
											"Arquivo interno: Salve uma c�pia de todos os intervalos de refer�ncia ativos internos no analisador, para futura recupera��o.\n\n"
											"Arquivo externo: Transmita um despejo ASCII de um intervalo de refer�ncia de dados demogr�ficos para um sistema externo.\n"											// Portuguese (Brazil)
										} 
	},
	{ ISTR_ASTM_ORDER_METHOD		,	{	"Work List: Select the order from a list.\n\n"
											"Enter Patient ID: Select the order from a list then confirm order based on entered patient ID.\n\n"
											"Disabled: ASTM orders are disabled.\n",																												// English
											"Liste de t�ches : S�lectionner l'ordre dans une liste.\n\n"
											"Entrer ID patient : S�lectionner l'ordre dans une liste, puis confirmer l'ordre en function de l'ID patient entr�e.\n\n"
											"D�sactiv� : Les orders ASTM sont  d�sactiv�s.",																										// French
											"Auftragsliste: Die Anforderung aus einer Liste ausw�hlen.\n\n"
											"Patienten-ID eingeben: Die Anforderung aus einer Liste ausw�hlen und Anforderung anhand der Patienten-ID best�tigen.\n\n"
											"Deaktiviert: ASTM-Anforderungen sind deaktiviert.",																									// German
											"Lista de trabajo: Seleccione el idioma en una lista.\n\n"
											"Introducir Id. del paciente: Seleccione el pedido en una lista y, a continuaci�n, confirme el pedido en funci�n del Id. del paciente introducido.\n\n"
											"Desactivado: Los pedidos ASTM est�n desactivados.",																									// Spanish
											"Elenco di lavoro: Selezionare l'ordine da un elenco.\n\n"
											"Inserisci ID paziente: Selezionare l'ordine da un elenco, quindi confermarlo in base all'ID paziente inserito.\n\n"
											"Disattivato: Gli ordini ASTM sono disattivati.",																										// Italian
											"Lista de trabalho: Seleciona o idioma de uma lista\n\n"
											"Introduzir ID de doente: Seleciona o pedido de uma lista e, em seguida, confirma o pedido baseado na ID do doente introduzido.\n\n"
											"Inativo: Os pedidos de ASTM est�o desativados.\n",																										// Portuguese
											"Lista de trabalho: Selecione a solicita��o em uma lista\n\n"
											"Inserir ID do paciente: Selecione a solicita��o em uma lista e confirme-a com base no ID do paciente inserido.\n\n"
											"Desativado: As solicita��es de ASTM est�o desativadas.\n"																								// Portuguese (Brazil)
										}
	},
	{ ISTR_ASTM_PHYSICAL_LAYER		,	{	"USB (Serial): The analyzer is connected from the USB \"B\" port to a local computer using a USB cable.\n\n"
											"TCP/IP: The analyzer is connected using a USB-to-Ethernet adapter to a network.\n",																	// English
											"USB (s�rie) : l'analyseur est connect� du port USB \"B\" � un ordinateur local par un c�ble USB.\n\n"
											"TCP/IP: l'analyseur est connect� � un r�seau par le biais d'un adaptateur USB-Ethernet.\n",															// French
											"USB (serieller Anschluss): Das Analysesystem ist �ber den USB-Anschluss \"B\" und ein USB-Kabel an einen lokalen Computer angeschlossen.\n\n"
											"TCP/IP: Das Analyseger�t ist �ber einen USB- Ethernet-Adapter an ein Netzwerk angeschlossen.\n",														// German
											"USB (serie): El analizador est� conectado desde un puerto USB \"B\" a un ordenador local mediante un cable USB.\n\n"
											"TCP/IP: El analizador est� conectado a la red mediante un adaptador USB a Ethernet.\n",																// Spanish
											"USB (seriale): L'analizzatore � connesso dalla porta USB \"B\" a un computer locale tramite un cavo USB.\n\n"
											"TCP/IP: L'analizzatore � connesso alla rete tramite un adattatore USB-Ethernet.\n",																	// Italian
											"USB (s�rie): O analisador est� ligado a partir da porta USB \"B\" de um computador local atrav�s de um cabo USB.\n\n"
											"TCP/IP: O analisador est� ligado a uma rede atrav�s de um adaptador USB para Ethernet.\n",																// Portuguese
											"USB (serial): O analisador � conectado da porta USB \"B\" a um computador local usando um cabo USB.\n\n"
											"TCP/IP: O analisador � conectado usando um adaptador USB para Ethernet em uma rede.\n"																	// Portuguese (Brazil)
										}
	},	
	{ ISTR_CD_UPDATE				,	{	"The CD ROM drive is located on the rear of the analyzer. Press the small rectangular button to open the drawer. Insert the CD onto the spindle and make sure it clicks into place. Close the CD ROM drive. Press Continue. After the analyzer has restarted, open the CD ROM drive and remove the CD. Store the CD in a safe place.\n",														// English
											"Le lecteur de CD ROM se trouve � l'arri�re de l'analyseur. Appuyer sur le petit bouton rectangulaire pour ouvrir le tiroir. Ins�rer le CD sur l'axe et v�rifier qu'il est bien en place. Fermer le lecteur de CD ROM. Appuyer sur Continuer. Une fois que l'analyseur a red�marr�, ouvrir le lecteur de CD ROM et retirer le CD. Ranger le CD en lieu s�r.\n",									// French
											"Das CD-ROM-Laufwerk befindet sich auf der R�ckseite des Analyseger�ts. Zur �ffnung des Fachs die kleine rechteckige Taste dr�cken. Die CD in die Spindel einsetzen und einrasten lassen. Schlie�en Sie das CD-ROM-Laufwerk. Dr�cken Sie auf Weiter. Nach dem Neustart des Analyseger�ts �ffnen Sie das CD-ROM-Laufwerk und entnehmen die CD. Verwahren Sie die CD an einem sicheren Ort.\n",	// German
											"La unidad de CD-ROM est� en la parte trasera del analizador. Pulse el bot�n rectangular peque�o para abrir la bandeja. Inserte el CD en el eje y aseg�rese de que hace clic cuando se coloca en su sitio. Cierre la unidad de CD-ROM. Pulse Continuar. Despu�s de reiniciar el analizador, abra la unidad de CD-ROM y retire el CD. Guarde el CD en un lugar seguro.\n",						// Spanish
											"Il drive del CD ROM si trova sul retro dell'analizzatore. Per aprire il cassetto premere il piccolo pulsante rettangolare. Inserire il CD sul supporto e assicurarsi che sia ben fissato. Chiudere il drive del CD ROM. Premere Continua. Una volta riavviato l'analizzatore, aprire il drive del CD ROM e rimuovere il CD. Conservare il CD in un luogo sicuro.\n",							// Italian
											"A unidade de CD-ROM est� localizada na parte posterior do analisador. Prima o bot�o retangular pequeno para abrir a gaveta. Insira o CD no eixo e certifique-se de que ouve um clique quando � colocado no lugar. Feche a unidade de CD-ROM. Prima Continuar. Depois de o analisador ter reiniciado, abra a unidade de CD-ROM e retire o CD. Guarde o CD num local seguro.\n",					// Portuguese
											"A unidade de CD-ROM est� localizada na parte traseira do analisador. Pressione o pequeno bot�o retangular para abrir a gaveta. Insira o CD no eixo e verifique se ele se encaixa no lugar. Feche a unidade de CD-ROM. Pressione Continuar. Depois que o analisador reiniciar, abra a unidade de CD-ROM e remova o CD. Guarde o CD em um local seguro.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_COMM_PROTOCOL			,	{	"None: No transmission of information is possible.\n\n"
											"ASCII Text: ASCII text dump of information unidirectional from the analyzer.\n\n"
											"ASTM: ASTM 1394 E 97 over ASTM E 1381 02 Standard Specification for Transferring Information Between Clinical Instruments and Computer Systems.\n\n"
											"XML: Extensible Markup Language output in proprietary Abaxis format.\n",																							// English
											"Aucune : aucune transmission d'information n'est possible.\n\n"
											"Texte ASCII : vidage de texte ASCII d'information unidirectionnelle � partir de l'analyseur.\n\n"
											"ASTM : Sp�cification des normes ASTM 1394 E 97/ASTM E 1381 02 pour la transmission de donn�es entre des instruments cliniques et des syst�mes informatiques.\n\n"
											"XML : Sortie en langage XML (Extensible Markup Language) dans le format propri�taire d'Abaxis.\n",																	// French
											"Keine: Keine Daten�bertragung m�glich.\n\n"
											"ASCII-Text: ASCII-Textauszug einseitig vom Analyseger�t.\n\n"
											"ASTM: ASTM 1394 E 97 �ber ASTM E 1381 02 Standardspezifikation f�r die �bertragung von Informationen zwischen klinischen Ger�ten und Computersystemen.\n\n"
											"XML: Ausgabe als Extensible Language in propriet�rem Abaxis-Format.\n",																							// German
											"Ninguna: no es posible realizar la transmisi�n de la informaci�n.\n\n"
											"Texto ASCII: volcado de datos en texto ASCII unidireccional desde el analizador.\n\n"
											"ASTM 1394 E 97 sobre ASTM E 1381 02 Especificaci�n est�ndar sobre transferencia de informaci�n entre instrumentos cl�nicos y sistemas inform�ticos.\n\n"
											"XML: Salida en lenguaje de marcado extensible en formato propiedad de Abaxis.\n",																					// Spanish
											"Nessuno: non � possibile alcuna trasmissione di informazioni.\n\n"
											"Testo ASCII: serie unidirezionale di informazioni in testo ASCII dall'analizzatore.\n\n"
											"ASTM: ASTM 1394 E 97 sulla specifica standard ASTM E 1381 02 per il trasferimento di informazioni tra strumenti clinici e computer.\n\n"
											"XML: Output di tipo Extensible Markup Language in formato proprietario Abaxis.\n",																					// Italian
											"Nenhum: N�o � poss�vel realizar a transmiss�o de informa��o.\n\n"
											"Texto ASCII: Dump de informa��es de texto ASCII, unidirecional do analisador.\n\n"
											"ASTM: ASTM 1394 E 97 sobre ASTM E 1381 02 Especifica��o padr�o sobre transfer�ncia de informa��o entre instrumentos cl�nicos e sistemas inform�ticos.\n\n"
											"XML: Sa�da em linguagem de marca��o extens�vel em formato propriedade da Abaxis.\n",																				// Portuguese
											"Nenhum: Nenhuma transmiss�o de informa��es � poss�vel.\n\n"
											"Texto ASCII: Despejo em texto ASCII de informa��es unidirecionais do analisador.\n\n"
											"ASTM: ASTM 1394 E 97 via ASTM E 1381 02 Especifica��o padr�o para transfer�ncia de informa��es entre instrumentos cl�nicos e sistemas de computadores.\n\n"
											"XML: Sa�da da linguagem de marca��o extens�vel no formato patenteado da Abaxis.\n"																					// Portuguese (Brazil)
										} 
	},
	{ ISTR_DATE_INVALID				,	{	"Only valid calendar days may be entered. Please set the date to today's date.\n",														// English
											"Saisir uniquement des jours civils valides. D�finir la date d'aujourd'hui comme date du jour.\n",										// French
											"Nur g�ltige Kalendertage eingeben. Datumsanzeige auf das heutige Datum einstellen.\n",													// German
											"Solo se pueden introducir d�as naturales correctos. Cambie la fecha al d�a de hoy.\n",													// Spanish
											"Possono essere inseriti solo giorni validi. Si prega di impostare la data odierna.\n",													// Italian
											"Apenas podem ser introduzidos dias de calend�rio v�lidos. Defina a data para a data do dia de hoje.\n",								// Portuguese
											"Apenas dias corridos v�lidos podem ser inseridos. Configure a data como a data de hoje.\n"												// Portuguese (Brazil)
										} 
	},
	{ ISTR_DELETE_ADMINISTRATOR		,	{	"Removing an administrator from the administrator list will permanently remove the administrator, and all information for that administrator will be lost.\n",					// English
											"La suppression d'un administrateur dans la liste d'administrateurs a pour effet de le supprimer et toutes les informations associ�es seront perdues.\n",						// French
											"Die Entfernung eines Administrators aus der Administratorliste entfernt den Administrator dauerhaft, und s�mtliche Informationen zu diesem Administrator gehen verloren.\n",	// German
											"Al eliminar un administrador de la lista de administradores se le elimina permanentemente. Se perder� toda la informaci�n de ese administrador.\n",							// Spanish
											"La rimozione di un amministratore dall'elenco amministratori sar� permanente e tutte le informazioni per tale amministratore saranno perse.\n",								// Italian
											"Ao eliminar um administrador da lista de administradores ir� eliminar permanentemente o administrador, e toda a informa��o desse administrador ser� perdida.\n",				// Portuguese
											"A remo��o de um administrador da lista de administradores o remover� permanentemente e todas as informa��es desse administrador ser�o perdidas.\n"								// Portuguese (Brazil)
										} 
	},
	{ ISTR_DELETE_OPERATOR			,	{	"Removing an operator from the operator list will permanently remove the operator, and all information for that operator will be lost.\n",										// English
											"La suppression d'un op�rateur dans la liste d'op�rateurs a pour effet de le supprimer et toutes les informations associ�es seront perdues.\n",									// French
											"Die Entfernung eines Bedieners aus der Bedienerliste entfernt den Bediener dauerhaft, und s�mtliche Informationen zu diesem Bediener gehen verloren.\n",						// German
											"Al eliminar un operario de la lista de operarios se le elimina permanentemente. Se perder� toda la informaci�n de ese operario.\n",											// Spanish
											"La rimozione di un operatore dall'elenco operatori sar� permanente e tutte le informazioni per tale operatore saranno perse.\n",												// Italian
											"Ao eliminar um operador da lista de operadores ir� eliminar permanentemente o operador, e toda a informa��o desse operador ser� perdida.\n",									// Portuguese
											"A remo��o de um operador da lista de operadores o remover� permanentemente e todas as informa��es desse operador ser�o perdidas.\n"											// Portuguese (Brazil)
										} 
	},
	{ ISTR_DISPLAY_SETTINGS			,	{	"Backlight: Controls the backlight level for the display.\n\n"
											"Screen Saver: Sets the time from the last screen press until the screen saver starts.\n\n"
											"Power Save: Sets the time from the last screen press until the display is turned off.\n\n"
											"Calibrate: Aligns the touch screen to the LCD display.\n",																				// English
											"R�tro�clairage : commande le niveau de r�tro�clairage de l'�cran.\n\n"
											"�conomiseur d'�cran : d�finit la dur�e entre la derni�re activation de l'�cran et la mise en marche de l'�conomiseur d'�cran.\n\n"
											"�conomiseur d'alimentation : d�finit la dur�e entre la derni�re activation de l'�cran et le moment o� l'�cran est arr�t�.\n\n"
											"Calibrage : alignement de l'�cran tactile avec l'�cran � cristaux liquides.\n",														// French
											"Hintergrundbeleuchtung: Steuert die Hintergrundbeleuchtung des Bildschirms.\n\n"
											"Bildschirmschoner: Zur Einstellung des Zeitintervalls von der letzten Bildschirmber�hrung bis zum Start des Bildschirmschoners.\n\n"
											"Energiesparmodus: Zur Einstellung der Zeit von der letzten Bildschirmber�hrung bis zur Abschaltung des Bildschirms.\n\n"
											"Kalibrieren: Richtet den Touch-Screen am LCD-Bildschirm aus.\n",																		// German
											"Luz de fondo: controla el nivel de la luz de fondo de la pantalla.\n\n"
											"Salvapantalla: marca el tiempo desde la �ltima presi�n sobre la pantalla hasta la activaci�n del salvapantalla.\n\n"
											"Ahorro de energ�a: marca el tiempo desde la �ltima presi�n de pantalla hasta que la pantalla se apaga.\n\n"
											"Calibrar: alinea la pantalla t�ctil a la pantalla de cristal l�quido.\n",																// Spanish
											"Retroilluminazione: controlla il livello di retroilluminazione per il display.\n\n"
											"Screensaver: imposta il tempo tra l'ultima pressione sullo schermo e l'avvio dello screensaver.\n\n"
											"Modo risparmio: imposta il tempo tra l'ultima pressione sullo schermo allo spegnimento del display.\n\n"
											"Calibra: allinea il touchscreen al display LCD.\n",																					// Italian
											"Luz de fundo: Controla o n�vel da luz de fundo do visor.\n\n"
											"Prote��o do ecr�: Define o tempo desde a �ltima vez que o ecr� � premido at� ao in�cio da prote��o do ecr�.\n\n"
											"Poupan�a de energia: Define o tempo desde a �ltima vez que o ecr� � premido at� que o visor � desligado.\n\n"
											"Calibrar: Alinha o ecr� t�til com o visor LCD.\n",																						// Portuguese
											"Luz de fundo: Controla o n�vel de luz de fundo do visor.\n\n"
											"Protetor de tela: Configura o tempo desde a �ltima press�o de tela at� a prote��o de tela iniciar.\n\n"
											"Economia de energia: Configura o tempo desde a �ltima press�o de tela at� o display desligar.\n\n"
											"Calibrar: Alinha a tela sens�vel ao toque com o display LCD.\n"																		// Portuguese (Brazil)
										} 
	},
	{ ISTR_FACTORY_DEFAULTS			,	{	"Restoring the analyzer to factory defaults resets the analyzer to the same conditions as when the analyzer left the factory. It is recommended that the reference ranges be archived prior to resetting the analyzer to factory defaults. Saved results and internal reference range archives will still be available after the settings are reset to factory defaults.\n",																						// English
											"Le fait de r�tablir les valeurs par d�faut de l'analyseur remet l'analyseur en �tat sortie d'usine. Il est conseill� d'archiver les fourchettes de r�f�rence avant de r�tablir les valeurs par d�faut de l'analyseur. Les r�sultats enregistr�s et les archives des fourchettes de r�f�rence internes seront toujours disponibles une fois les param�tres par d�faut r�tablis.\n",																					// French
											"Die Wiederherstellung der Werkseinstellungen des Analyseger�ts stellt die Einstellungen des Analyseger�ts auf den Auslieferungszustand zur�ck. Es wird empfohlen, die Referenzbereiche vor dem Zur�cksetzen der Referenzbereiche auf die Werkseinstellungen zu archivieren. Gespeicherte Ergebnisse und interne Referenzbereichsarchive sind auch nach Wiederherstellung der Werkseinstellungen verf�gbar.\n",														// German
											"La restauraci�n del analizador a los valores predeterminados originales reestablece las mismas condiciones de f�brica del analizador. Se recomienda archivar los intervalos de referencia antes de reestablecer los valores predeterminados de f�brica del analizador. Los archivos de resultados y los intervalos de referencia internos estar�n disponibles despu�s de reestablecer los valores predeterminados originales del analizador.\n",					// Spanish
											"Se si ripristinano le impostazioni predefinite, l'analizzatore ritorna alle condizioni in cui ha lasciato la fabbrica. Si raccomanda di archiviare i range di riferimento prima di resettare l'analizzatore sulle impostazioni di fabbrica. I risultati salvati e gli archivi dei range di riferimento interni saranno disponibili anche dopo aver ripristinato le impostazioni di fabbrica.\n",																	// Italian
											"O restauro do analisador para os valores predeterminados de f�brica restaura o analisador para as mesmas condi��es do analisador quando saiu da f�brica. � recomendado arquivar os intervalos de refer�ncia antes de restaurar o analisador para os valores predeterminados de f�brica. Os ficheiros de resultados e dos intervalos de refer�ncia internos continuar�o dispon�veis ap�s o restauro do analisador para os valores predeterminados de f�brica.\n",	// Portuguese
											"A restaura��o do analisador para os padr�es de f�brica o redefine para as mesmas condi��es de quando ele saiu da f�brica. � recomend�vel que os intervalos de refer�ncia sejam arquivados antes de redefinir o analisador para os padr�es de f�brica. Os resultados salvos e os arquivos do intervalo de refer�ncia interno ainda estar�o dispon�veis depois que as configura��es forem redefinidas para os padr�es de f�brica.\n"									// Portuguese (Brazil)
										} 
	},
	{ ISTR_LANGUAGE_SET				,	{	"Both: Both the display and the external keyboard will be set to the same language at the same time.\n\n"
											"Display: Only the language used on the display will be changed.\n\n"
											"Keyboard: Only the keyboard type will be changed. The keyboard selected must match the keyboard used.\n",										// English
											"Les deux : l'�cran et le clavier externe seront dans la m�me langue en m�me temps.\n\n"
											"�cran : seule la langue utilis�e � l'�cran sera modifi�e.\n\n"
											"Clavier : seule le type de clavier sera modifi�. Le clavier s�lectionn� doit correspondre au clavier utilis�.\n",								// French
											"Beide: Anzeige und externes Eingabeger�t werden gleichzeitig auf dieselbe Sprache eingestellt.\n\n"
											"Anzeige: Nur die Sprache der Anzeige wird ver�ndert.\n\n"
											"Eingabeger�t: Nur der Eingabeger�tetyp wird ver�ndert. Das ausgew�hlte Eingabeger�t muss mit dem verwendeten Eingabeger�t �bereinstimmen.\n",	// German
											"Ambos: se establecer�n simult�neamente tanto la pantalla como el teclado externo con el mismo idioma.\n\n"
											"Pantalla: solo se cambiar� el idioma de la pantalla.\n\n"
											"Teclado: solo cambiar� el tipo de teclado. El teclado seleccionado deber� coincidir con el teclado usado.\n",									// Spanish
											"Entrambi: sia il display che la tastiera esterna saranno impostati sulla stessa lingua nello stesso momento.\n\n"
											"Display: viene modificata solo la lingua utilizzata dal display.\n\n"
											"Tastiera: viene modificato solo il tipo di tastiera. La tastiera selezionata deve coincidere con la tastiera utilizzata.\n",					// Italian
											"Ambos: O visor e o teclado externo ser�o definidos simultaneamente com o mesmo idioma.\n\n"
											"Visor: Apenas ser� alterado o idioma utilizado no visor.\n\n"
											"Teclado: Apenas ser� alterado o tipo de teclado. O teclado selecionado dever� corresponder ao teclado utilizado.\n",							// Portuguese
											"Ambos: O display e o teclado externo ser�o configurados para o mesmo idioma ao mesmo tempo.\n\n"
											"Display: Apenas o idioma usado no display ser� alterado.\n\n"
											"Teclado: Apenas o tipo de teclado ser� alterado. O teclado selecionado deve corresponder ao teclado usado.\n"									// Portuguese (Brazil)
										} 
	},
	{ ISTR_PRINTER_CONFIGURE		,	{	"Set Default: Select the printer to use for printing.\n\n"
											"Select Reports: Select the type and number of reports.\n",									// English
											"Imprimante par d�faut : s�lectionner l'imprimante � utiliser pour l'impression.\n\n"
											"S�lectionner des rapports : s�lectionner le type et le nombre de rapports.\n",				// French
											"Standard einstellen: Drucker ausw�hlen.\n\n"
											"Berichte ausw�hlen: Typ und Zahl der Berichte ausw�hlen.\n",								// German
											"Establecer valores predeterminados: seleccione la impresora que se va a utilizar.\n\n"
											"Seleccionar informes: seleccione el tipo y n�mero de informes.\n",							// Spanish
											"Imposta come predefinito: seleziona la stampante da utilizzare per la stampa.\n\n"
											"Seleziona rapporti: seleziona il tipo e il numero di rapporti.\n",							// Italian
											"Configurar valores predeterminados: Selecione a impressora a utilizar para imprimir.\n\n"
											"Selecionar relat�rios: Selecione o tipo e o n�mero de relat�rios.\n",						// Portuguese
											"Configurar padr�o: Selecione a impressora a ser usada para a impress�o.\n\n"
											"Selecionar relat�rios: Selecione o tipo e o n�mero de relat�rios.\n"						// Portuguese (Brazil)
										} 
	},
	{ ISTR_PRINTER_SELECT_REPORTS	,	{	"Results: Select the number of results reports to print at the end of each analysis.\n\n"
											"iQC: Select the number of iQC reports to print at the end of each analysis.\n\n"
											"Error: Select the number of error reports to print at the end of each analysis. The reports can be printed automatically when there is an error, or may be printed for every analysis.\n",								// English
											"R�sultats : s�lectionner le nombre de rapports de r�sultats � imprimer � l'issue de chaque analyse.\n\n"
											"iQC : s�lectionner le nombre de rapports iQC � imprimer � l'issue de chaque analyse.\n\n"
											"Erreur : s�lectionner le nombre de rapports d'erreurs � imprimer � l'issue de chaque analyse. Des rapports peuvent s'imprimer automatiquement en cas d'erreur ou bien pour chaque analyse.\n",							// French
											"Ergebnisse: Anzahl der nach Abschluss jeder Analyse zu druckenden Ergebnisberichte ausw�hlen.\n\n"
											"iQC: Anzahl der nach Abschluss jeder Analyse zu druckenden iQC-Berichte ausw�hlen.\n\n"
											"Fehler: Anzahl der nach Abschluss jeder Analyse zu druckenden Fehlerberichte ausw�hlen. Bei Vorliegen eines Fehlers k�nnen die Berichte automatisch oder f�r jede Analyse gedruckt werden.\n",							// German
											"Resultados: seleccione el n�mero de informes de resultados para imprimir al final de cada an�lisis.\n\n"
											"iQC: seleccione el n�mero de informes de iQC para imprimir al final de cada an�lisis.\n\n"
											"Error: seleccione el n�mero de informes de error para imprimir al final de cada an�lisis. Los informes se pueden imprimir autom�ticamente cuando hay un error, o se pueden imprimir con cada an�lisis.\n",				// Spanish
											"Risultati: seleziona il numero di rapporti di risultati da stampare al termine di ogni analisi.\n\n"
											"iQC: seleziona il numero di rapporti di iQC da stampare al termine di ogni analisi.\n\n"
											"Errore: seleziona il numero di rapporti di errore da stampare al termine di ogni analisi. I rapporti possono essere stampati automaticamente in caso di errore, oppure essere stampati per ogni analisi.\n",			// Italian
											"Resultados: Seleccone o n�mero de relat�rios de resultados a imprimir no final de cada an�lise.\n\n"
											"iQC: Selecione o n�mero de relat�rios de iQC a imprimir no final de cada an�lise.\n\n"
											"Erro: Selecione o n�mero de relat�rios de erros a imprimir no final de cada an�lise. Os relat�rios podem ser impressos automaticamente quando existir um erro, ou podem ser impressos para cada an�lise.\n",			// Portuguese
											"Resultados: Selecione o n�mero de relat�rios de resultados a serem impressos no final de cada an�lise.\n\n"
											"iQC: Selecione o n�mero de relat�rios de iQC a serem impressos no final de cada an�lise.\n\n"
											"Erro: Selecione o n�mero de relat�rios de erro a serem impressos no final de cada an�lise. Os relat�rios podem ser impressos automaticamente quando houver um erro ou podem ser impressos para todas as an�lises.\n"	// Portuguese (Brazil)
										} 
	},
	{ ISTR_PRINTER_SETUP			,	{	"Configure: Select the printer to use and the number and type of reports to print.\n\n"
											"Test: Print a test page on the printer.\n",																			// English
											"Configuration : s�lectionner l'imprimante � utiliser, le nombre et le type de rapports � imprimer.\n\n"
											"Test : imprimer une page de test sur l'imprimante.\n",																	// French
											"Konfigurieren: Verwendeten Drucker sowie Zahl und Typ der zu druckenden Berichte ausw�hlen.\n\n"
											"Test: Testseite drucken.\n",																							// German
											"Configurar: seleccione la impresora que se va a utilizar y el n�mero y tipo de informes que se van a imprimir.\n\n"
											"Test: imprima una p�gina de prueba en la impresora.\n",																// Spanish
											"Configura: seleziona la stampante da usare, nonch� il numero e il tipo di rapporti da stampare.\n\n"
											"Prova: stampa una pagina di prova.\n",																					// Italian
											"Configurar: Selecione a impressora a utilizar e o n�mero e tipo de relat�rios a imprimir.\n\n"
											"Teste: Imprima uma p�gina de teste na impressora.\n",																	// Portuguese
											"Configurar: Selecione a impressora a ser usada e o n�mero e tipo de relat�rios a serem impressos.\n\n"
											"Teste: Imprima uma p�gina de teste na impressora.\n"																	// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_ADD			,	{	"Special n: The next available generic demographic label between 1 and 99.\n\n"
											"Control n: The next available control label between 1 and 99.\n\n",													// English
											"Sp�cial n : la prochaine �tiquette d�mographique g�n�rique disponible comprise entre 1 et 99.\n\n"
											"Contr�le n : la prochaine �tiquette de contr�le disponible comprise entre 1 et 99.\n",									// French
											"Spezielles n: Das n�chste verf�gbare Demographie-Etikett zwischen 1 und 99.\n\n"
											"Steuerung n: Das n�chste verf�gbare Steuerungsetikett zwischen 1 und 99.\n",											// German
											"Especial n: la pr�xima etiqueta de demograf�a gen�rica disponible entre 1 y 99.\n\n"
											"Control n: la pr�xima etiqueta de control disponible entre 1 y 99.\n",													// Spanish
											"Speciale n: la successiva etichetta di demografia generica disponibile tra 1 e 99.\n\n"
											"Controllo n: la successiva etichetta di controllo disponibile tra 1 e 99.\n",											// Italian
											"Especial n: A pr�xima etiqueta de demografia gen�rica dispon�vel entre 1 e 99.\n\n"
											"Controlo n: A pr�xima etiqueta de controlo dispon�vel entre 1 e 99.\n",												// Portuguese
											"N especial: O pr�ximo r�tulo de dado demogr�fico gen�rico dispon�vel entre 1 e 99.\n\n"
											"N controle: O pr�ximo r�tulo de controle dispon�vel entre 1 e 99.\n"													// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_INCLUDE		,	{	"Last Modified XXX: The last reference range modified.\n\n"
											"All: Allows the user to select from a list of all reference ranges known to the analyzer.\n",														// English
											"Derni�re modification XXX : la derni�re fourchette de r�f�rence modifi�e.\n\n"
											"Tous : permet � l'utilisateur d'effectuer une s�lection � partir de la liste de toutes les fourchettes de r�f�rences connues de l'analyseur.\n",	// French
											"Letzte �nderung XXX: Letzter ge�nderter Referenzbereich.\n\n"
											"Alle: Erm�glicht dem Benutzer die Auswahl aus einer Liste aller im Analyseger�t verf�gbaren Referenzbereiche.\n",									// German
											"�ltima modificaci�n XXX: el �ltimo intervalo de referencia modificado.\n\n"
											"Todo: permite al usuario elegir de una lista de todos los intervalos de referencia conocidos por el analizador.\n",								// Spanish
											"Ultima modifica XXX: l'ultimo range di riferimento modificato.\n\n"
											"Tutto: consente all'utente di selezionare da un elenco tutti i range di riferimento noti all'analizzatore.\n",										// Italian
											"�ltima modifica��o XXX: O �ltimo intervalo de refer�ncia modificado.\n\n"
											"Tudo: Permite ao utilizador selecionar a partir de uma lista com todos os intervalos de refer�ncia conhecidos para o analisador.\n",				// Portuguese
											"�ltima modifica��o de XXX: O �ltimo intervalo de refer�ncia modificado.\n\n"
											"Todos: Permite ao usu�rio selecionar em uma lista de todos os intervalos de refer�ncia conhecidos pelo analisador.\n"								// Portuguese (Brazil)
										}
	},
	{ ISTR_REF_RANGES_MODIFY		,	{	"Analyte: Provides a list of all active reference ranges for a particular analyte.\n\n"
											"Demographic: Provides a list of all analytes for all reference ranges.\n",																			// English
											"Substance � analyser : fournit une liste de toutes les fourchettes de r�f�rence actives pour une substance � analyser donn�e.\n\n"
											"�l�ment d�mographique : fournit une liste de toutes les substances � analyser pour toutes les fourchettes de r�f�rence.\n",						// French
											"Analyt: Anzeige einer Liste aller aktiven Referenzbereiche f�r ein bestimmtes Analyt.\n\n"
											"Demographie: Anzeige einer Liste aller Analyte f�r alle Referenzbereiche.\n",																		// German
											"Analito: proporciona una lista de todos los intervalos de referencia activos para un analito particular.\n\n"
											"Demograf�a: proporciona una lista de todos los analitos para todos los intervalos de referencia.\n",												// Spanish
											"Analita: fornisce un elenco di tutti i range di riferimento attivi per un particolare analita.\n\n"
											"Demografia: fornisce un elenco di tutti gli analiti per ogni range di riferimento.\n",																// Italian
											"Analito: Fornece uma lista de todos os intervalos de referencia ativos para um analito espec�fico.\n\n"
											"Demografia: Fornece uma lista de todos os analitos para todos os intervalos de refer�ncia.\n",														// Portuguese
											"Analito: Fornece uma lista de todos os intervalos de refer�ncia ativos para um determinado analito.\n\n"
											"Dados demogr�ficos: Fornece uma lista de todos os analitos para todos os intervalos de refer�ncia.\n"												// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_REMOVE		,	{	"Removing a demographic from the sample type list will remove the item from the list of demographics shown when running a disc, and any values entered will be permanently lost.\n",																						// English
											"La suppression d'un �l�ment d�mographique dans la liste de type d'�chantillons a pour effet de supprimer cet �l�ment dans la liste d'�l�ments d�mographiques affich�s lors de l'ex�cution d'un disque, et toutes les valeurs saisies seront irr�m�diablement perdues.\n",	// French
											"Die Entfernung einer Demographie aus der Probentypliste entfernt das Objekt aus der Demographieliste, die bei laufendem Rotor angezeigt wird, und s�mtliche eingegebene Werte gehen dauerhaft verloren.\n",																// German
											"Al eliminar una demograf�a de la lista de tipo de muestra se eliminar� la entrada de la lista de demograf�as mostrada al usar el disco y cualquier valor introducido se perder� permanentemente.\n",																		// Spanish
											"Rimuovendo una demografia dall'elenco dei tipi si rimuover� quella voce dall'elenco di demografie proposto quando un disco � in funzione e tutti i valori immessi saranno persi in modo permanente.\n",																	// Italian
											"Ao eliminar uma demografia da lista de tipo de amostra, ser� eliminado o item da lista de demografias exibida quando se utiliza um disco, e qualquer valor introduzido ser� permanentemente perdido.\n",																	// Portuguese
											"A remo��o de um dado demogr�fico da lista de tipos de amostra remover� o item da lista de dados demogr�ficos mostrada durante a execu��o de um disco e quaisquer valores inseridos ser�o permanentemente perdidos.\n"														// Portuguese (Brazil)
										} 
	},
	{ ISTR_SECURITY_LOCKOUT			,	{	"Analyze: When checked, only a authorized operator can open the drawer at the start or the end of an analysis.\n\n"
											"Recall Results: When checked, only a authorized operator can recall results.\n\n"
											"Settings: When checked, only a authorized operator can change settings.\n",																// English
											"Analyse : lors de la v�rification, seul un op�rateur autoris� peut ouvrir le tiroir au d�but ou � la fin d'une analyse.\n\n"
											"Rappeler r�sultats : lors de la v�rification, seul un op�rateur autoris� peut rappeler les r�sultats.\n\n"
											"Param�tres : lors de la v�rification, seul un op�rateur autoris� peut modifier les param�tres.\n",											// French
											"Analysieren: Wenn aktiviert, kann nur ein autorisierter Bediener das Fach bei Start oder Ende einer Analyse �ffnen. \n\n"
											"Ergebnisse erneut abrufen: Wenn aktiviert, kann nur ein autorisierter Bediener Ergebnisse erneut abrufen.\n\n"
											"Einstellungen: Wenn aktiviert, kann nur ein autorisierter Bediener Einstellungen �ndern.\n",												// German
											"Analizar: cuando se activa, solo un operario autorizado puede abrir el caj�n al comienzo o al final de un an�lisis.\n\n"				
											"Buscar resultados: cuando se activa, solo un operario autorizado puede buscar los resultados.\n\n"
											"Configuraci�n: cuando se activa, solo un operario autorizado puede cambiar la configuraci�n.\n",											// Spanish
											"Analizzare: quando spuntato, esclusivamente un operatore autorizzato pu� aprire il cassetto all'inizio o alla fine di un'analisi.\n\n"
											"Recupera i risultati: quando spuntato, esclusivamente un operatore autorizzato pu� recuperare i risultati.\n\n"
											"Impostazioni; quando spuntato, esclusivamente un operatore autorizzato pu� modificare le impostazioni.\n",									// Italian
											"Analisar: Quando selecionado, apenas um operador autorizado pode abrir a gaveta no in�cio ou no final de uma an�lise.\n\n"
											"Recuperar resultados: Quando selecionado, apenas um operador autorizado pode recuperar os resultados.\n\n"
											"Configura��es: Quando selecionado, apenas um operador autorizado pode alterar as configura��es.\n",										// Portuguese
											"Analisar: Quando marcado, apenas um operador autorizado pode abrir a gaveta no in�cio ou no final de uma an�lise.\n\n"
											"Recuperar resultados: Quando marcado, somente um operador autorizado pode recuperar os resultados.\n\n"
											"Configura��es: Quando marcado, apenas um operador autorizado pode alterar as configura��es.\n"												// Portuguese (Brazil)
										}	
	},
	{ ISTR_SECURITY_SETTINGS		,	{	"Administrators: Can access the settings in the second settings screen. Security features take effect when the first administrator is added.  Security features are disabled when all administrators are deleted.\n\n"
											"Operators: Authorized operators for the analyzer. Adding an operator to the list will turn on the Operator ID prompt when analyzing a sample.\n\n"
											"Controls: The date that a new set of control samples must be run by. Analyzing a sample after this date will result in a warning when the sample is analyzed.\n",																																	// English
											"Administrateurs : ils peuvent acc�der aux param�tres sur le second �cran de param�tres. Les caract�ristiques de s�curit� entrent en vigueur lors de l'ajout du premier administrateur. Les caract�ristiques de s�curit� sont d�sactiv�es lorsque tous les administrateurs sont supprim�s.\n\n"
											"Op�rateurs : op�rateurs autoris�s pour l'analyseur. L'ajout d'un op�rateur � la liste a pour effet d'activer un message d'invitation � saisir l'ID op�rateur lors de l'analyse d'un �chantillon.\n\n"
											"Contr�les : date butoir � laquelle un nouvel ensemble d'�chantillons de contr�le doit avoir �t� ex�cut�. Le fait d'analyser un �chantillon apr�s cette date entra�ne un message d'avertissement lors de l'analyse de l'�chantillon.\n",															// French
											"Administrator: Haben Zugang zu den Einstellungen in dem Zweiteinstellungsbildschirm. Sicherheitsmerkmale werden bei Hinzuf�gung des ersten Administrators wirksam. Sicherheitsmerkmale werden bei L�schung aller Administratoren deaktiviert.\n\n"
											"Bediener: Befugte Bediener des Analyseger�ts. Die Hinzuf�gung eines Bedieners zur Liste aktiviert bei Analyse einer Probe die Aufforderung zur Eingabe der Bediener-ID.\n\n"
											"Steuerungen: Das Datum, zu dem ein neuer Satz von Kontrollproben vorgenommen werden muss. Die Analyse einer Probe nach diesem Datum f�hrt zu einem Warnhinweis bei Analyse der Probe.\n",																											// German
											"Administradores: pueden acceder a la configuraci�n en la segunda pantalla de configuraciones. Las medidas de seguridad empiezan a adoptarse despu�s de a�adir el primer administrador. Las medidas de seguridad est�n inactivas al eliminar a todos los administradores.\n\n"
											"Operarios: operarios autorizados para uso del analizador. Al a�adir un operario a la lista se activar� un mensaje de identificaci�n de operario al analizar la muestra.\n\n"
											"Controles: la fecha en la que se debe usar un nuevo grupo de muestras de control. El an�lisis de una muestra despu�s de esta fecha motivar� un mensaje de aviso durante el an�lisis.\n",																											// Spanish
											"Amministratori: possono accedere alle impostazioni nel secondo schermo di impostazioni. Le impostazioni di sicurezza entrano in vigore quando si aggiunge il primo amministratore. Le impostazioni di sicurezza sono disattivate quando tutti gli amministratori sono cancellati.\n\n"
											"Operatori: operatori autorizzati all'uso dell'analizzatore. Se si aggiunge un operatore all'elenco, durante l'analisi di un campione viene attivato il prompt \"ID operatore\".\n\n"
											"Controlli: la data in cui deve essere effettuata una nuova serie di controlli campione. Se l'analisi di un campione viene eseguita dopo tale data, verr� visualizzato un messaggio di attenzione.\n",																								// Italian
											"Administradores: Podem aceder �s configura��es no segundo ecr� de configura��es. Os recursos de seguran�a tornam-se efetivos quando o primeiro administrador � adicionado.  Os recursos de seguran�a ficam inativos quando todos os administradores s�o eliminados.\n\n"
											"Operadores: Operadores autorizados para utiliza��o do analisador. Adicionar um operador � lista ir� ativar um pedido de ID do operador ao analisar a amostra.\n\n"
											"Controlos: A data na qual se deve utilizar um grupo novo de amostras controlo. A an�lise de uma amostra ap�s esta data resultar� numa mensagem de aviso durante a an�lise.\n",																														// Portuguese
											"Administradores: Podem acessar as configura��es na segunda tela de configura��es. Os recursos de seguran�a entram em vigor quando o primeiro administrador � adicionado.  Os recursos de seguran�a s�o desativados quando todos os administradores s�o exclu�dos.\n\n"
											"Operadores: Operadores autorizados para o analisador. Adicionar um operador � lista ativar� o prompt do ID do operador ao analisar uma amostra.\n\n"
											"Controles: A data at� a qual um novo conjunto de amostras de controle deve ser executado. A an�lise de uma amostra ap�s essa data resultar� em um aviso quando a amostra for analisada.\n"																											// Portuguese (Brazil)
										} 
	},
	{ ISTR_SECURITY_SETTINGS_LOCKOUT,	{	"Administrators: Can access the settings in the second settings screen. Security features take effect when the first administrator is added.  Security features are disabled when all administrators are deleted.\n\n"
											"Operators: Authorized operators for the analyzer. Adding an operator to the list will turn on the Operator ID prompt when analyzing a sample.\n\n"
											"Controls: The date that a new set of control samples must be run by. Analyzing a sample after this date will result in a warning when the sample is analyzed.\n\n"
											"Lockout: Limits use of the analyzer functions to authorized operators.  Create authorized operators prior to enabling lockout.\n",																																									// English
											"Administrateurs : ils peuvent acc�der aux param�tres sur le second �cran de param�tres. Les caract�ristiques de s�curit� entrent en vigueur lors de l'ajout du premier administrateur. Les caract�ristiques de s�curit� sont d�sactiv�es lorsque tous les administrateurs sont supprim�s.\n\n"
											"Op�rateurs : op�rateurs autoris�s pour l'analyseur. L'ajout d'un op�rateur � la liste a pour effet d'activer un message d'invitation � saisir l'ID op�rateur lors de l'analyse d'un �chantillon.\n\n"
											"Contr�les : date butoir � laquelle un nouvel ensemble d'�chantillons de contr�le doit avoir �t� ex�cut�. Le fait d'analyser un �chantillon apr�s cette date entra�ne un message d'avertissement lors de l'analyse de l'�chantillon.\n\n"
											"Verrouillage : limite l'utilisation des fonctions de l'analyseur aux op�rateurs autoris�s. Les op�rateurs autoris�s doivent �tre cr��s avant d'autoriser le verrouillage.\n",																														// French
											"Administrator: Haben Zugang zu den Einstellungen in dem Zweiteinstellungsbildschirm. Sicherheitsmerkmale werden bei Hinzuf�gung des ersten Administrators wirksam. Sicherheitsmerkmale werden bei L�schung aller Administratoren deaktiviert.\n\n"
											"Bediener: Befugte Bediener des Analyseger�ts. Die Hinzuf�gung eines Bedieners zur Liste aktiviert bei Analyse einer Probe die Aufforderung zur Eingabe der Bediener-ID.\n\n"
											"Steuerungen: Das Datum, zu dem ein neuer Satz von Kontrollproben vorgenommen werden muss. Die Analyse einer Probe nach diesem Datum f�hrt zu einem Warnhinweis bei Analyse der Probe.\n\n"
											"Verriegelung: Beschr�nkt die Verwendung von Analyseger�tfunktionen auf autorisierte Bediener. Erstellen Sie autorisierte Bediener bevor Sie die Verriegelung aktivieren.\n",																														// German
											"Administradores: pueden acceder a la configuraci�n en la segunda pantalla de configuraci�n. Las caracter�sticas de seguridad se activan cuando se agrega el primer administrador y se desactivan si se eliminan todos los administradores.\n\n"
											"Operarios: operarios autorizados a utilizar el analizador. Al a�adir un operario a la lista, se preguntar� el ID del operario al analizar una muestra.\n\n"
											"Controles: fecha en la que se debe ejecutar un nuevo conjunto de muestras de control. Si se analiza una muestra despu�s de esta fecha, aparecer� una advertencia cuando se analice la muestra.\n\n"
											"Bloqueo: limita el uso de las funciones del analizador a los operarios autorizados. Debe crear operarios autorizados antes de activar el bloqueo.\n",																																				// Spanish
											"Amministratori: possono accedere alle impostazioni nel secondo schermo di impostazioni. Le impostazioni di sicurezza entrano in vigore quando si aggiunge il primo amministratore. Le impostazioni di sicurezza sono disattivate quando tutti gli amministratori sono cancellati.\n\n"
											"Operatori: operatori autorizzati all'uso dell'analizzatore. Se si aggiunge un operatore all'elenco, durante l'analisi di un campione viene attivato il prompt \"ID operatore\".\n\n"
											"Controlli: la data in cui deve essere effettuata una nuova serie di controlli campione. Se l'analisi di un campione viene eseguita dopo tale data, verr� visualizzato un messaggio di attenzione.\n\n",																							
											"Serrata: limita l'uso delle funzioni dell'analizzatore agli operatori autorizzati.  Creare operatori autorizzati prima di abilitare la serrata.\n"																																					// Italian
											"Administradores: Podem aceder �s configura��es no segundo ecr� de configura��es. Os recursos de seguran�a tornam-se efetivos quando o primeiro administrador � adicionado.  Os recursos de seguran�a ficam inativos quando todos os administradores s�o eliminados.\n\n"
											"Operadores: Operadores autorizados para utiliza��o do analisador. Adicionar um operador � lista ir� ativar um pedido de ID do operador ao analisar a amostra.\n\n"
											"Controlos: A data na qual se deve utilizar um grupo novo de amostras controlo. A an�lise de uma amostra ap�s esta data resultar� numa mensagem de aviso durante a an�lise.\n\n"
											"Bloqueio: Limita a utiliza��o das fun��es do analisador aos operadores autorizados.  Deve criar operadores autorizados antes de ativar o bloqueio.\n",																																				// Portuguese
											"Administradores: Podem acessar as configura��es na segunda tela de configura��es. Os recursos de seguran�a entram em vigor quando o primeiro administrador � adicionado.  Os recursos de seguran�a s�o desativados quando todos os administradores s�o exclu�dos.\n\n"
											"Operadores: Operadores autorizados para o analisador. Adicionar um operador � lista ativar� o prompt do ID do operador ao analisar uma amostra.\n\n"
											"Controles: A data at� a qual um novo conjunto de amostras de controle deve ser executado. A an�lise de uma amostra ap�s essa data resultar� em um aviso quando a amostra for analisada.\n\n"
											"Bloquear: Limita o uso das fun��es do analisador a operadores autorizados.  Crie operadores autorizados antes de ativar o bloqueio.\n"																																								// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_ELECTROLYTE_UNITS	,	{	"Selecting a unit on this screen will change the units for CL, K, NA and tCO2 simultaneously to the same units.\n",										// English
											"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � CL, K, NA et tCO2.\n",																// French
											"Mit der Auswahl einer Einheit in diesem Men� werden automatisch die Einheiten f�r CL, K, NA und tCO2 auf diese Einheit umgestellt.\n",					// German
											"Al seleccionar una unidad en esta pantalla, se cambiar� CL, K, NA y tCO2 simult�neamente a la misma unidad.\n",										// Spanish
											"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per CL, K, NA e tCO2 nelle stesse unit�.\n",							// Italian
											"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades CL, K, NA e tCO2 para as mesmas unidades.\n",						// Portuguese
											"A sele��o de uma unidade nessa tela mudar� as unidades para CL, K, NA e tCO2 simultaneamente para as mesmas unidades.\n"								// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_ENZYME_UNITS			,	{	"Selecting a unit on this screen will change the units for ALT, ALP, AMY, AST, CK, GGT and LD simultaneously to the same units.\n",						// English
											"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � ALT, ALP, AMY, AST, CK, GGT et LD.\n",												// French
											"Mit der Auswahl einer Einheit in diesem Men� werden automatisch die Einheiten f�r ALT, ALP, AMY, AST, CK, GGT und LD auf diese Einheit umgestellt.\n",	// German
											"Al seleccionar una unidad en esta pantalla, se cambiar� ALT, ALP, AMY, AST, CK, GGT y LD simult�neamente a la misma unidad.\n",						// Spanish
											"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per ALT, ALP, AMY, AST, CK, GGT ed LD nelle stesse unit�.\n",			// Italian
											"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades ALT, ALP, AMY, AST, CK, GGT e LD para as mesmas unidades.\n",		// Portuguese
											"A sele��o de uma unidade nessa tela mudar� as unidades para ALT, ALP, AMY, AST, CK, GGT e LD simultaneamente para as mesmas unidades.\n"				// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_LIPID_UNITS			,	{	"Selecting a unit on this screen will change the units for CHOL, HDL, LDL, TRIG and VLDL simultaneously to the same units.\n",							// English
											"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � CHOL, LHD, LBD, TRIG et LTBD.\n",													// French
											"Mit der Auswahl einer Einheit in diesem Men� werden automatisch die Einheiten f�r CHOL, HDL, LDL, TRIG und VLDL auf diese Einheit umgestellt.\n",		// German
											"Al seleccionar una unidad en esta pantalla, se cambiar� COL, HDL, LDL, TRIG y PMBD simult�neamente a la misma unidad.\n",								// Spanish
											"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per COL, HDL, LDL, TRIG e VLDL nelle stesse unit�.\n",					// Italian
											"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades CHOL, HDL, LDL, TRIG e VLDL para as mesmas unidades.\n",				// Portuguese
											"A sele��o de uma unidade nessa tela mudar� as unidades para CHOL, HDL, LDL, TRIG e VLDL simultaneamente para as mesmas unidades.\n"					// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_MINERAL_UNITS		,	{	"Selecting a unit on this screen will change the units for CA and MG simultaneously to the same units.\n",												// English
											"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � CA et MG.\n",																		// French
											"Mit der Auswahl einer Einheit in diesem Men� werden automatisch die Einheiten f�r CA und MG auf diese Einheit umgestellt.\n",							// German
											"Al seleccionar una unidad en esta pantalla, se cambiar� CA y MG simult�neamente a la misma unidad.\n",													// Spanish
											"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per CA ed MG nelle stesse unit�.\n",									// Italian
											"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades CA e MG para as mesmas unidades.\n",									// Portuguese
											"A sele��o de uma unidade nessa tela mudar� as unidades para CA e MG simultaneamente para as mesmas unidades.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_PROTEIN_UNITS		,	{	"Selecting a unit on this screen will change the units for ALB and TP simultaneously to the same units.\n",												// English
											"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � ALB et TP.\n",																		// French
											"Mit der Auswahl einer Einheit in diesem Men� werden automatisch die Einheiten f�r ALB und TP auf diese Einheit umgestellt.\n",							// German
											"Al seleccionar una unidad en esta pantalla, se cambiar� ALB y TP simult�neamente a la misma unidad.\n",												// Spanish
											"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per ALB e TP nelle stesse unit�.\n",									// Italian
											"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades ALB e TP para as mesmas unidades.\n",								// Portuguese
											"A sele��o de uma unidade nessa mudar� as unidades para ALB e TP simultaneamente para as mesmas unidades.\n"											// Portuguese (Brazil)
										} 
	},
	{ ISTR_SOUND_SETTINGS			,	{	"Screen Click: Sets the volume level of the sound made when touching a button on the display.\n\n"
											"Alerts: Sets the volume level when a warning or error screen is displayed.\n\n"
											"Status: Sets the volume level for status sounds.\n",																									// English
											"Clic �cran : cette fonction d�finit le volume sonore lors de l'activation d'un bouton � l'�cran.\n\n"
											"Avertisseurs sonores : cette fonction d�finit le volume lors de l'affichage d'un avertissement ou d'un �cran d'erreur.\n\n"
											"�tat : d�finit le volume sonore pour les indicateurs d'�tat.\n",																						// French
											"Bildschirmklick: Einstellung der Tonlautst�rke bei Ber�hrung einer Taste auf der Anzeige.\n\n"
											"Alarme: Einstellung der Tonlautst�rke bei Erscheinen der Warn- oder Fehleranzeige.\n\n"
											"Status: Einstellung der Lautst�rke f�r Statust�ne.\n",																									// German
											"Clic de pantalla: establece el volumen del sonido que se hace cuando se toca un bot�n en la pantalla.\n\n"
											"Alertas: establece el volumen del sonido que se hace cuando aparece una ventana de aviso o error.\n\n"
											"Estado: establece el volumen del sonido de estado.\n",																									// Spanish
											"Clic schermo: imposta il livello del volume del suono che si sente quando si tocca un pulsante sullo schermo.\n\n"
											"Allarmi: imposta il livello del volume dei messaggi di attenzione o errore visualizzati a schermo.\n\n"
											"Stato: imposta il livello del volume per i suoni di stato.\n",																							// Italian
											"Clique no ecr�: Define o n�vel de volume do som que se ouve quando se toca uma tecla no visor.\n\n"
											"Alertas: Define o n�vel de volume do som que se ouve quando � visualizado um ecr� de aviso ou erro.\n\n"
											"Estado: Define o n�vel de volume para os sons de estado.\n",																							// Portuguese
											"Clique na tela: Configura o n�vel de volume do som produzido ao tocar em um bot�o no visor.\n\n"
											"Alertas: Configura o n�vel do volume quando uma tela de aviso ou erro � exibida.\n\n"
											"Status: Configura o n�vel de volume para os sons de status.\n"																							// Portuguese (Brazil)
										} 
	},
	{ ISTR_TCPIP_SETTINGS			,	{	"Change Settings: Change the TCP/IP Settings where Analyzer IP is the Static IP Address for the analyzer, the Subnet Mask is the IP Address mask for the analyzer, the Gateway IP is the IP Address of the router for the analyzer to reach other networks, the Server IP is the IP address of the server that will host the ASTM connection, and the Server Socket is the Socket (Port) on the Server that will be used for the ASTM TCP/IP connection.\n\n"
											"Check Settings: The analyzer will attempt to make an ASTM connection using the settings under the Change Settings button.  The analyzer will display the result of the connection attempt.  Ensure the ASTM server is running and configured to accept a connection before checking the settings.\n",																																																																												// English																																																																																																																																																																																																																												// English                 					
											"Modifier les param�tres : modifier les param�tres TCP/IP si le champ Adresse IP de l'analyseur est renseign� par l'adresse IP statique de l'appareil, le champ Masque de sous-r�seau contient le masque d'adresse IP de l'analyseur, le champ Adresse IP de la passerelle d�signe l'adresse IP du routeur qui permet � l'analyseur de communiquer avec d'autres r�seaux. Le champ Adresse IP du serveur est renseign� par l'adresse IP du serveur qui h�bergera la connexion ASTM et enfin le champ Socket du serveur d�signe le Socket (Port) du serveur qui sera utilis� pour la connexion TCP/IP ASTM.\n\n"
											"V�rifier les param�tres : l'analyseur tentera d'�tablir une connexion ASTM sur la base des param�tres d�finis sous le bouton Modifier les param�tres.  L'analyseur affichera le r�sultat de la tentative de connexion.  V�rifier que le serveur ASTM est en marche et qu'il est configur� pour accepter une connexion avant de v�rifier les param�tres.\n",																																																														// French
											"Einstellungen �ndern: Die TCP/IP-Einstellungen so �ndern, dass die IP-Adresse des Analyseger�ts die statische IP-Adresse f�r das Analyseger�t ist, die Subnetzmaske die IP-Adressmaske f�r das Analyseger�t ist, die Gateway-IP-Adresse die IP-Adresse des Routers f�r das Analyseger�t ist, �ber den dieses andere Netzwerke erreichen kann, die Server-IP-Adresse die IP-Adresse des Servers ist, der als Host f�r den ASTM-Anschluss dient, und der Server-Socket der Socket (Anschluss ) des Servers ist, der f�r die ASTM-TCP/IP-Verbindung genutzt werden soll.\n\n"
											"Einstellungen �berpr�fen: Das Analyseger�t wird versuchen, eine ASTM-Verbindung herzustellen, und bedient sich dabei der �ber die Taste \"Einstellungen �ndern\" eingegebenen Einstellungen. Das Analyseger�t zeigt das Resultat des Verbindungsversuchs an. Vor dem �berpr�fen der Einstellungen sicherstellen, dass der ASTM-Server in Betrieb und f�r die Annahme einer Verbindung konfiguriert ist.\n",																																																		// German
											"Cambiar configuraci�n: Cambie la configuraci�n de TCP/IP, donde la direcci�n IP del analizador es la direcci�n IP est�tica del analizador, la m�scara de la subred es la m�scara de la direcci�n IP del analizador, la direcci�n IP de la puerta de enlace es la direcci�n IP del enrutador para conectividad entre el analizador y otras redes, la direcci�n IP del servidor es la direcci�n IP del servidor que alojar� la conexi�n ASTM y el socket del servidor es el socket (puerto) del servidor que se utilizar� para la conexi�n TCP/IP ASTM.\n\n"
											"Comprobar configuraci�n: El analizador intentar� realizar una conexi�n ASTM usando los par�metros que aparecen tras pulsar el bot�n \"Cambiar configuraci�n\".  El analizador mostrar� el resultado del intento de conexi�n.  Aseg�rese de que el servidor ASTM est� funcionando y configurado para aceptar una conexi�n antes de comprobar los par�metros de configuraci�n.\n",																																																									// Spanish
											"Modifica impostazioni: Modificare le impostazioni TCP/IP dove IP analizzatore � l'indirizzo IP statico per l'analizzatore, Maschera di sottorete � la maschera dell'indirizzo IP per l'analizzatore, IP gateway � l'indirizzo IP del router per consentire all'analizzatore di raggiungere altre reti, IP server � l'indirizzo IP del server per l'hosting della connessione ASTM, e Socket server � il Socket (porta) sul server che verr� utilizzato per la connessione TCP/IP ASTM.\n\n"
											"Verifica impostazioni: L'analizzatore tenter� di effettuare una connessione ASTM utilizzando le impostazioni sotto il pulsante Modifica impostazioni.  L'analizzatore visualizzer� il risultato del tentativo di connessione.  Prima di eseguire la verifica delle impostazioni, accertarsi che il server ASTM sia attivo e configurato per accettare una connessione.\n",																																																											// Italian
											"Alterar configura��es: Altera as configura��es de TCP/IP, nas quais o IP do analisador � o endere�o IP est�tico do analisador, a m�scara de subrede � a m�scara do endere�o IP do analisador, o IP do Portal � o endere�o IP do rotor para que o analisador se ligue a outras redes, o IP do servidor � o endere�o IP do servidor que alojar� a liga��o ASTM e a tomada do servidor � a tomada (Porta) no Servidor que ser� utilizada para a liga��o TCP/IP ASTM.\n\n"
											"Verificar configura��es: O analisador tentar� realizar uma liga��o ASTM utilizando as defini��es que surgem sob o bot�o Alterar configura��es.  O analisador apresentar� o resultado da tentativa de liga��o.  Certifique-se que o servidor ASTM est� a funcionar e que est� configurado para aceitar uma liga��o antes de verificar as configura��es.\n",																																																															// Portuguese
											"Alterar configura��es: Altere as configura��es de TCP/IP em que o IP do analisador � o endere�o IP est�tico do analisador, a m�scara de sub-rede � a m�scara do endere�o IP do analisador, o IP do gateway � o endere�o IP do roteador para que o analisador alcance outras redes, IP do servidor � o endere�o IP do servidor que hospedar� a conex�o ASTM e Soquete do servidor � o soquete (porta) do servidor que ser� usado para a conex�o TCP/IP de ASTM.\n\n"
											"Verificar configura��es: O analisador tentar� fazer uma conex�o ASTM usando as configura��es no bot�o Alterar configura��es.  O analisador exibir� o resultado da tentativa de conex�o.  Certifique-se de que o servidor ASTM esteja em execu��o e configurado para aceitar uma conex�o antes de verificar as configura��es.\n"																																																																					// Portuguese (Brazil)
										}
	},		
	{ ISTR_TOUCH_CALIBRATE			,	{	"Pressing Yes will align the touch screen to the LCD display. It is necessary to use a soft-pointed stylus to accurately align the touch screen. Using a hard-pointed object will damage the touch screen. Pressing the screen other than at the center of the target will misalign the touch screen. If you touch the screen in the wrong location, complete the alignment by touching the remaining targets and do not touch the final alignment button. If you have questions contact technical support before performing the alignment.\n",																											// English
											"Appuyer sur Oui pour aligner l'�cran tactile et l'�cran � cristaux liquides. Pour aligner correctement l'�cran tactile, utiliser un stylet � pointe souple. Toute utilisation d'un objet pointu risquerait d'endommager l'�cran tactile. Le fait d'appuyer sur l'�cran ailleurs qu'au centre de la cible entra�ne un d�faut d'alignement de l'�cran tactile. Si vous touchez l'�cran au mauvais endroit, terminer l'alignement en touchant les cibles restantes et ne pas toucher le bouton d'alignement final. Pour toute question, contacter l'assistance technique avant de proc�der � l'alignement.\n",											// French
											"Dr�cken auf Ja richtet den Touch-Screen auf das LCD-Display aus. Zur genauen Ausrichtung des Touch-Screens muss ein Eingabestift mit weicher Spitze verwendet werden. Die Verwendung eines Objekts mit harter Spitze besch�digt den Touch-Screen. Dr�cken der Anzeige au�erhalb der Zielmitte f�hrt zu einer falschen Ausrichtung des Touch-Screens. Wenn Sie den Screen an der falschen Stelle ber�hren, schlie�en Sie die Ausrichtung durch Ber�hrung der verbleibenden Ziele ab. Die endg�ltige Ausrichtungstaste nicht ber�hren. Bei Fragen vor der Ausrichtung den technischen Support kontaktieren.\n",											// German
											"Al pulsar S� se alinear� la pantalla t�ctil con la pantalla de cristal l�quido. Si fuese necesario, utilice un l�piz con punta blanda para alinear con precisi�n la pantalla de contacto. Si se utiliza un objeto con punta dura, la pantalla t�ctil resultar� da�ada. Si se presiona la pantalla en otro lugar distinto al centro del objetivo, la pantalla t�ctil resultar� desalineada. Si toca la pantalla en el lugar equivocado, complete la alineaci�n tocando los objetivos restantes y no toque el bot�n de alineaci�n final. Si tiene alguna duda o pregunta, llame al servicio de asistencia t�cnica antes de realizar la alineaci�n.\n",	// Spanish
											"Premendo S� il touchscreen verr� allineato al display LCD. Per allineare accuratamente il touchscreen � necessario utilizzare una penna stilo a punta morbida. Oggetti a punta rigida possono danneggiare il touchscreen. Se si preme lo schermo non al centro dell'obiettivo, il touchscreen verr� allineato male. Se si tocca lo schermo nel punto sbagliato, completare l'allineamento toccando gli altri obiettivi e non toccare il pulsante di allineamento finale. Per eventuali dubbi, prima di effettuare l'allineamento contattare l'assistenza tecnica.\n",																					// Italian
											"Ao premir Sim ir� alinhar o ecr� t�til com o visor LCD. � necess�rio utilizar um ponteiro de ponta suave para alinhar o ecr� t�til com precis�o. Se utilizar um objeto de ponta dura ir� provocar danos no ecr� t�til. Ao pressionar o ecr� noutro local que n�o seja o centro do alvo, ir� desalinhar o ecr� t�til. Se tocar no ecr� no local errado, conclua o alinhamento tocando nos alvos restantes e n�o toque na tecla final de alinhamento. Se tiver alguma d�vida, contacte a servi�o de assist�ncia t�cnica antes de efetuar o alinhamento.\n",																								// Portuguese
											"Pressionar Sim alinhar� a tela sens�vel ao toque com o display LCD. � necess�rio usar uma caneta de ponta macia para alinhar precisamente a tela sens�vel ao toque. O uso de um objeto pontiagudo danificar� a tela sens�vel ao toque. Pressionar a tela fora do centro do alvo desalinhar� a tela sens�vel ao toque. Se voc� tocar na tela no local errado, conclua o alinhamento tocando nos alvos restantes e n�o toque no bot�o de alinhamento final. Se tiver d�vidas, entre em contato com o suporte t�cnico antes de realizar o alinhamento.\n"																									// Portuguese (Brazil)
										} 
	},
	{ ISTR_UNITS_GLOBAL				,	{	"Non SI: Also known as Common Units. The units in which all results are calculated and the units commonly used in the United States.\n\n"
											"SI: Units identified by Abaxis as the commonly used SI units.",																																// English
											"Non SI : �galement d�sign�es \"Unit�s ordinaires\". Il s'agit des unit�s dans lesquelles tous les r�sultats sont calcul�s, ainsi que des unit�s ordinairement utilis�es aux �tats-Unis.\n\n"
											"SI : unit�s identifi�es par Abaxis comme �tant les unit�s SI ordinairement utilis�es.\n",																										// French
											"Kein SI: Auch als H�ufige Einheiten bekannt. Die Einheiten zur Ergebnisermittlung sind die in den USA gebr�uchlichen Einheiten.\n\n"
											"SI: Als Abaxis identifizierte Einheiten als h�ufig verwendete SI-Einheiten.\n",																												// German
											"No SI: tambi�n se le conoce como Unidades comunes. La unidades en las que se calculan todos los resultados y las unidades com�nmente usadas en los Estados Unidos.\n\n"
											"SI: unidades identificadas por Abaxis como las unidades del sistema internacional usadas normalmente.\n",																						// Spanish
											"Non SI: conosciuto anche come Unit� comuni. Le unit� in cui tutti i risultati sono calcolati e le unit� comunemente usate negli Stati Uniti.\n\n"
											"SI: unit� identificate come Abaxis come le unit� SI comunemente usate.\n",																														// Italian
											"N�o SI: Tamb�m conhecidas como Unidades comuns. As unidades nas quais todos os resultados s�o calculados e as unidades frequentemente utilizadas nos Estados Unidos.\n\n"
											"SI: Unidades identificadas pela Abaxis como as unidades do sistema internacional utilizadas frequentemente.\n",																				// Portuguese
											"N�o SI: Tamb�m conhecido como Unidades comuns. As unidades nas quais todos os resultados s�o calculados e as unidades comumente usadas nos Estados Unidos.\n\n"
											"SI: Unidades identificadas pela Abaxis como as unidades SI comumente usadas.\n"																												// Portuguese (Brazil)
										} 
	},
	

// Error screens							ENGLISH,							FRENCH,										GERMAN,									SPANISH,												ITALIAN, 								PORTUGUESE,											PORTUGUESE (BRAZIL)
	{ ESTR_ADMIN_ID_LIST_FULL		,	{	"Administrator ID List Full",		"Liste ID administrateur pleine",			"Vollst�ndige Administrator-ID-Liste",	"Lista de id. de administradores completa",				"Elenco ID Amministratore completo",	"Lista de ID de administradores completa",			"Lista de IDs de administrador completa"	}	},
	{ ESTR_AMBIENT_TEMP				,	{	"Ambient Temp. Error",				"Temp. ambiante Erreur",					"Umgebungstemp. Fehler",				"Temp. ambiente Error",									"Temp. ambiente Errore",				"Temperatura ambiente Erro",						"Temp. ambiente Erro"	}	},
	{ ESTR_ANALYSIS_CANCELED		,	{	"Analysis canceled by operator",	"Anal. annul�e par l'op�rateur",			"Anal. vom Bedien. abgebrochen",		"An�l. cancelado por oper.",							"Anal. annullata dall'operatore",		"An�lise cancelada pelo operador",					"An�lise cancelada pelo operador"	}	},
	{ ESTR_ANALYSIS_TIMEOUT			,	{	"Analysis Time-out",				"Temps d'analyse d�pass�",					"Analysezeit abgelaufen",				"Tiempo an�lisis agotado",								"Time-out analisi",						"Tempo de an�lise esgotado",						"Tempo limite da an�lise"	}	},
	{ ESTR_ANALYZER_TIMEOUT			,	{	"Analyzer Time-out",				"D�l. ex�c. analyseur exp.",				"Analyseger�t-Unterbr.",				"Tiempo inactiv. analizad.",							"Time-out analizzatore",				"Tempo inat. analisador",							"Tempo limite do analis."	}	},
	{ ESTR_ANALYZER_TOO_HOT			,	{	"Analyzer Too Hot",					"Analyseur trop chaud",						"Analyseger�t zu hei�",					"Analizador muy caliente",								"Analizzatore troppo caldo",			"Analisador muito quente",							"Analisador muito quente"	}	},
	{ ESTR_BARCODE_FORMAT			,	{	"Barcode Format Error",				"Erreur format code barres",				"Barcode Formatfehler",					"Error formato c�d. barras",							"Err. formato cod. a barre",			"Erro formato c�d. barras",							"Erro formato c�d. barras"	}	},
	{ ESTR_BARCODE_SENSOR			,	{	"Barcode Sensor Error",				"Erreur d�tect. c. barres",					"Barcode Sensorfehler",					"Error sensor c�d. barras",								"Err. sens. codice a barre",			"Erro sensor c�d. barras",							"Erro sensor c�d. barras"	}	},
	{ ESTR_BARCODE_TIMEOUT			,	{	"Barcode Time-out Error",			"D�l. ex�c. c. barres exp.",				"Barcode Unterbr.-Fehler",				"Err tiem inact c�d barr",								"Err. time-out cod. barre",				"Erro tempo inat. c�d bar",							"Erro temp lim. c�d. barra"	}	},
	{ ESTR_BEADS_MISSING			,	{	"Beads Missing Error",				"Erreur billes manquantes",					"Beads fehlen, Fehler",					"Err. p�rdida cuentas",									"Errore granuli mancanti",				"Erro de perda de esferas",							"Erro de esferas ausentes"	}	},
	{ ESTR_CAM_DOWN_JAM				,	{	"CAM Down Jam",						"Blocage CAM haut",							"CAM oben Stau",						"LEVA bloqueada abajo",									"Blocco CAMMA gi�",						"CAM bloqueada abaixo",								"Emperramento CAM p/ baixo"	}	},
	{ ESTR_CAM_UP_JAM				,	{	"CAM Up Jam",						"Blocage CAM bas",							"CAM unten Stau",						"LEVA bloqueada arriba",								"Blocco CAMMA su",						"CAM bloqueada acima",								"Emperramento CAM p/ cima"	}	},
	{ ESTR_CD_READ					,	{	"Error Reading CD ROM",				"Erreur lecture CD ROM",					"Fehler beim Lesen der CD-ROM",			"Error al leer el CD-ROM",								"Errore di lettura CD ROM",				"Erro de leitura do CD-ROM",						"Erro ao ler o CD ROM"	}	},
	{ ESTR_CORRUPTED_CALIBRATION	,	{	"Corrupted Calibration",			"Calibrage corrompu",						"Fehlerhafte Kalibrierung",				"Calibraci�n err�nea",									"Calibrazione corrotta",				"Calibra��o corrompida",							"Calibra��o corrompida"	}	},
	{ ESTR_CUVETTE_SENSOR			,	{	"Cuvette Sensor Error",				"Erreur d�tecteur cuvette",					"Cuvette Sensorfehler",					"Error sensor cubeta",									"Errore sensore cuvette",				"Erro do sensor de cuvetes",						"Erro do sensor da cuveta"	}	},
	{ ESTR_CUVETTE_TIMEOUT			,	{	"Cuvette Time-out Error",			"Err. d�lai ex�c. cuvette",					"Cuvette Unterbr.-Fehler",				"Err tiem inact cubeta",								"Errore time-out cuvette",				"Erro tempo inat. cuvete",							"Erro tempo limite cuveta"	}	},
	{ ESTR_DILUENT					,	{	"Diluent Error",					"Erreur diluant",							"Fehler des Verd�nners",				"Error del diluyente",									"Errore diluente",						"Erro de solvente",									"Erro de diluente"	}	},
	{ ESTR_DILUENT_ABSORBANCE		,	{	"Diluent Absorbance Error",			"Erreur absorbance diluant",				"Verd�nner Absorb.-Fehler",				"Err absorbancia diluyente",							"Err. assorb. diluente",				"Erro absorv�ncia solvente",						"Erro absor��o do diluente"	}	},
	{ ESTR_DILUTION					,	{	"Dilution Error",					"Erreur dilution",							"Verd�nnungsfehler",					"Error de diluci�n",									"Errore diluizione",					"Erro de dilui��o",									"Erro de dilui��o"	}	},
	{ ESTR_DISTRIBUTION				,	{	"Distribution Error",				"Erreur distribution",						"Verteilungsfehler",					"Error de distribuci�n",								"Errore distribuzione",					"Erro de distribui��o",								"Erro de distribui��o"	}	},
	{ ESTR_DRAWER_CLOSED_JAM		,	{	"Drawer Closed Jam",				"Blocage fermeture tiroir",					"Fach geschlossen Stau",				"Bloqueo de la bandeja cerrada",						"Blocco cassetto chiuso",				"Bloqueio da gaveta fechada",						"Emperramento de gaveta fechada"	}	},
	{ ESTR_DRAWER_OPEN_JAM			,	{	"Drawer Open Jam",					"Blocage tiroir ouvert",					"Fach offen Stau",						"Bloqueo de la bandeja abierta",						"Blocco cassetto aperto",				"Bloqueio da gaveta aberta",						"Emperramento de gaveta aberta"	}	},
	{ ESTR_ENGINE_RESET				,	{	"Asynchronous Reset",				"R�initialisation asynchr.",				"Asynchroner Resetvorgang",				"Restablecim. as�ncrono",								"Ripristino asincrono",					"Rein�cio ass�ncrono",								"Redefini��o ass�ncrona"	}	},
	{ ESTR_ILLEGAL_IP_ADDRESS		,	{	"Illegal IP Address",				"Adresse IP non valide",					"Falsche IP-Adresse",					"Direcci�n IP no v�lida",	   							"Indirizzo IP non valido",				"Endere�o IP inv�lido",								"Endere�o IP ilegal"	}	},
	{ ESTR_ILLEGAL_IP_MASK			,	{	"Illegal IP Mask",					"Masque IP non valide",						"Falsche IP-Maske",						"M�scara IP no v�lida",									"Maschera IP non valida",				"M�scara de IP inv�lida",							"M�scara IP ilegal"	}	},
	{ ESTR_ILLEGAL_ROTOR_TYPE		,	{	"Illegal Disc Type",				"Type disque non autoris�",					"Falscher Scheibentyp",					"Tipo de disco ilegal",									"Tipo disco illegale",					"Tipo de disco inv�lido",							"Tipo de disco ilegal"	}	},
	{ ESTR_ILLEGAL_SOCKET			,	{	"Illegal Socket Value",				"Valeur de socket non valide",				"Falscher Socket-Wert",					"Valor de socket no v�lido",							"Valore socket non valido",				"Valor da tomada inv�lido",							"Valor de soquete ilegal"	}	},
	{ ESTR_INITIALIZATION			,	{	"Initialization Error",				"Erreur d'initialisation",					"Initialisierungsfehler",				"Error de inicio",										"Errore inizializzazione",				"Erro de inicializa��o",							"Erro de inicializa��o"	}	},
	{ ESTR_INSUFF_SAMPLE			,	{	"Insufficient Sample Error",		"Erreur �ch. insuffisant",					"Unzureich. Probe, Fehler",				"Err. muestra insuficien",								"Err. campione insuff.",				"Erro amostra insuficiente",						"Erro amostra insuficiente"	}	},
	{ ESTR_INSUFF_STORAGE			,	{	"Insufficient Storage Space",		"Espace de stockage insuffisant",			"Ungen�gend Speicherplatz",				"Espacio insuficiente",									"Spazio archiviazione insufficiente",	"Espa�o de armazenamento insuficiente",				"Espa�o de armazenamento insuficiente"	}	},
	{ ESTR_INTERNAL_COMM			,	{	"Internal Comm. Error",				"Comm. interne Erreur",						"Interne Komm. Fehler",					"Com. interna Error",									"Comm. interno Errore",					"Com. interna Erro",								"Com. interna Erro"	}	},
	{ ESTR_INTERNAL_HEATER			,	{	"Internal Heater Error",			"Err. disp. chauf. interne",				"Interner Fehler Heizger�t",			"Err. calentador interno",								"Errore riscald. interno",				"Erro de aquecedor interno",						"Erro do aquecedor interno"	}	},
	{ ESTR_INTERNAL_LOGIC			,	{	"Internal Logic Error",				"Erreur logique interne",					"Interner Logic-Fehler",				"Error l�gico interno",									"Errore logica interna",				"Erro l�gico interno",								"Erro da l�gica interna"	}	},
	{ ESTR_INTERNAL_PS				,	{	"Internal Power Error",				"Erreur d'alim. interne",					"Interner Energiefehler",				"Error de energ�a interno",								"Errore corrente interna",				"Erro de energia interno",							"Erro de energia interna"	}	},
	{ ESTR_INTERNAL_SOFTWARE		,	{	"Internal Software Error",			"Erreur logiciel interne",					"Interner Softwarefehler",				"Error de software interno",							"Errore software interno",				"Erro de software interno",							"Erro de software interno"	}	},
	{ ESTR_IQC_BEAD					,	{	"iQC Bead Error",					"Erreur bille iQC",							"iQC-Bead-Fehler",						"Error de cuenta iQC",									"Errore granuli iQC",					"Erro de esfera iQC",								"Erro de esfera iQC"	}	},
	{ ESTR_IQC_BEAD_MIX				,	{	"iQC Bead Mix Error",				"Erreur mix billes iQC",					"iQC-Bead-Mischfehler",					"Error mezcla cuenta iQC",								"Err. miscela granuli iQC",				"Erro de mist. esfera iQC",							"Erro mistura esferas iQC"	}	},
	{ ESTR_IQC_PRECISION			,	{	"iQC Precision Error",				"Erreur pr�cision iQC",						"iQC-Pr�zisionsfehler",					"Error de precisi�n iQC",								"Errore precisione iQC",				"Erro de precis�o iQC",								"Erro de precis�o de iQC"	}	},
	{ ESTR_IQC_RATIO				,	{	"iQC Ratio Error",					"Erreur ratio iQC",							"iQC-Verh�ltnisfehler",					"Error de proporci�n iQC",								"Errore rapporto iQC",					"Erro de propor��o iQC",							"Erro de rela��o de iQC"	}	},
	{ ESTR_LAMP_LEVEL				,	{	"Flash Lamp Level Error",			"Erreur niveau lampe flash",				"Blinkleuchte-Stufenfehler",			"Error nivel l�mpara flash",							"Err. liv. lampada flash",				"Erro n�vel flash l�mpada",							"Erro n�vel l�mpada flash"	}	},
	{ ESTR_LAMP_NOISE				,	{	"Flash Lamp Noise Error",			"Erreur bruit lampe flash",					"Blinkleuchte-Ger�uschfeh.",			"Error ruido l�mpara flash",							"Err. rumore lamp. flash",				"Erro ru�do flash l�mpada",							"Erro ru�do l�mpada flash"	}	},
	{ ESTR_MEMORY_READ				,	{	"Memory Read Error",				"Erreur de lecture m�moire",				"Fehler bei Speicheraufruf",			"Error lectura memoria",								"Errore lettura memoria",				"Erro leitura mem�ria",								"Erro leitura mem�ria"	}	},
	{ ESTR_MEMORY_SAVE				,	{	"Memory Save Error",				"Erreur enregistr. m�moire",				"Fehler bei Speicherung",				"Err. guardar memoria",									"Errore salv. Memoria",					"Erro guardar na mem�ria",							"Erro salvamento mem�ria"	}	},
	{ ESTR_NEW_ROTOR_TYPE			,	{	"New Disc Type",					"Nouveau type de disque",					"Neuer Scheibentyp",					"Nuevo tipo de disco",									"Nuovo tipo disco",						"Tipo novo de disco",								"Novo tipo de disco"	}	},
	{ ESTR_NO_FILES_TO_ARCHIVE		,	{	"No Files to Archive",				"Aucun fichier � archiver",					"Keine zu archiv. Dateien",				"No archi para archivar",								"Nessun file da archiviare",			"Nenhum ficheiro arquivar",							"Sem arquivo para arquivar"	}	},
	{ ESTR_NO_FILES_TO_TRANSMIT		,	{	"No Files to Transmit",				"Aucun fich. � transmettre",				"Keine zu �bertr. Dateien",				"No archi para transmit",								"Nessun file da trasm.",				"Nenhum ficheiro transmit.",						"Sem arquivo p/ transmitir"	}	},
	{ ESTR_NO_RESULTS_TO_TRANSMIT	,	{	"No Results to Transmit",			"Aucun r�sulat � trans.",					"Keine Erg. zu �bertragen",				"No result para transmit",								"Nessun risult. da trasm.",				"Nenhum result. transmitir",						"Sem result. p/ transmitir"	}	},
	{ ESTR_OP_ID_LIST_FULL			,	{	"Operator ID List Full",			"Liste ID op�rateur pleine", 				"Vollst. Bediener-ID-Liste",			"List id. operari complet",								"Elenco ID Oper. completo",				"Lista ID operadores comp.",						"Lista IDs oper. completa"	}	},
	{ ESTR_ORDER_WAS_CANCELED		,	{	"Order Was Canceled",				"Commande a �t� annul�e",					"Bestellung storniert",					"El pedido se ha cancelado",							"Ordine cancellato", 					"O pedido foi cancelado",							"Solicita��o cancelada"	}	},
	{ ESTR_PATIENT_IDS_DONT_MATCH	,	{	"Patient IDs Do Not Match",			"ID patient ne corr. pas",					"Patienten-IDs unstimmig",				"No coinciden Id.  pacien",								"ID paziente non corrisp.",				"IDs doente n�o coincidem",							"IDs paciente n�o corresp."	}	},
	{ ESTR_PHOTOMETER				,	{	"Photometer Error",					"Erreur photom�tre",						"Fotometer-Fehler",						"Error del fot�metro",									"Errore fotometro",						"Erro do fot�metro",								"Erro de fot�metro"	}	},
	{ ESTR_PLEASE_REMOVE_ROTOR		,	{	"Please Remove Disc",				"Retirer le disque",						"Bitte Scheibe entfernen",				"Retire el disco",										"Rimuovere il disco",					"Retirar disco",									"Remova o disco"	}	},
	{ ESTR_PRINTER_NOT_FOUND		,	{	"Printer Not Found",				"Imprimante introuvable",					"Kein Drucker gefunden",				"No se encuentra impresora",							"Stampante non trovata",				"Impressora n�o localizada",						"Impressora n�o encontrada"	}	},
	{ ESTR_PRODUCT_CODE				,	{	"Illegal Product Code",				"Code produit non autoris�",				"Falscher Produktcode",					"C�digo de producto ilegal",							"Codice prodotto illegale",				"C�d. produto inv�lido",							"C�digo de produto ilegal"	}	},
	{ ESTR_RESULT_CORRUPTION		,	{	"Saved Results Error",				"Erreur dans les r�sultats enregistr�s",	"Fehler bei gespeicherten Ergebnissen",	"Error de resultados guardados",						"Errore risultati salvati",				"Erro de resultados guardados",						"Erro de resultados salvos"	}	},
	{ ESTR_ROTOR_DATE_ILLEGAL		,	{	"Disc Date Illegal",				"Date disque non autoris�e",				"Falsches Scheibendatum",				"Fecha de disco ilegal",								"Data disco illegale",					"Data do disco inv�lida",							"Data do disco ilegal"	}	},
	{ ESTR_ROTOR_DATE_IMPROPER		,	{	"Disc Date Improper",				"Date disque incorrecte",					"Unzul. Scheibendatum",					"Fecha disco no adecuada",								"Data disco impropria",					"Data do disco inadequada",							"Data do disco impr�pria"	}	},
	{ ESTR_ROTOR_EXPIRED			,	{	"Disc Expired",						"Expiration disque",						"Abgelaufene Scheibe",					"Disco caducado",										"Disco scaduto",						"Disco expirado",									"Disco expirado"	}	},
	{ ESTR_RQC						,	{	"RQC Error",						"Erreur RQC",								"RQC-Fehler",							"Error RQC",											"Errore RQC",							"Erro RQC",											"Erro de RQC"	}	},
	{ ESTR_SAMPLE_BLANK_BEAD		,	{	"Sample Blank Bead Error",			"Erreur bille vierge �ch.",					"Probe leer, Bead-Fehler",				"Err.  cuent. virg.  muest",							"Err. granuli camp. vuoti",				"Erro esf. branco amostra",							"Erro esf. vazia amostra"	}	},
	{ ESTR_SAMPLE_DISTRIBUTION		,	{	"Sample Distribution Error",		"Erreur distribution �ch.",					"Probenverteilungsfehler",				"Err. distrib. muestra",								"Err. distrib. campione",				"Erro de distrib. amostra",							"Erro distr. de amostra"	}	},
	{ ESTR_SAMPLE_MIX				,	{	"Sample Mix Error",					"Erreur mix �chantillon",					"Probenmischfehler",					"Error mezcla muestra",									"Errore miscela campione",				"Erro de mist. da amostra",							"Erro mistura de amostra"	}	},
	{ ESTR_SAMPLE_QUALITY			,	{	"Sample Quality Error",				"Erreur qualit� �chantil.",					"Fehler, Probenqualit�t",				"Error calidad muestra",								"Errore qualit� campione",				"Erro de qual. da amostra",							"Erro qualidade da amostra"	}	},
	{ ESTR_SPINDLE_MOTOR			,	{	"Spindle Motor Error",				"Erreur moteur de l'axe",					"Fehler, Spindelmotor",					"Error del motor del eje",								"Errore asse motore",					"Erro do eixo do motor",							"Erro de motor do fuso"	}	},
	{ ESTR_SPINDLE_MOTOR_STUCK		,	{	"Spindle Motor Error",				"Erreur moteur de l'axe",					"Fehler, Spindelmotor",					"Error del motor del eje",								"Errore asse motore",					"Erro do eixo do motor",							"Erro de motor do fuso"	}	},
	{ ESTR_TEMPERATURE				,	{	"Temperature Error",				"Erreur temp�rature",						"Temperaturfehler",						"Error de temperatura",									"Errore temperatura",					"Erro de temperatura",								"Erro de temperatura"	}	},
	{ ESTR_UPDATED_ROTOR_TYPE		,	{	"Updated Disc Type",				"Type de disque actualis�",					"Aktual. Scheibentyp",					"Tipo de disco actualizado",							"Tipo disco aggiornato",				"Tipo de disco atualizado",							"Tipo de disco atualizado"	}	},

// Warning screens							ENGLISH,																													FRENCH,																																		GERMAN,																																	SPANISH,																													ITALIAN, 																														PORTUGUESE,																															PORTUGUESE (BRAZIL)
	{ ESTR_ADMIN_NOT_FOUND			,	{	"Administrator ID not found",																								"ID admin. introuvable",																													"Administrator-ID nicht gefunden",																										"No se encontr� Id. administrador",																							"ID amministratore non trovato",																								"ID de administrador n�o encontrada",																								"ID de administrador n�o encontrado"	}	},
	{ ESTR_CANNOT_REMOVE_OP_ID		,	{	"All operators must be deleted before the Operator ID screen can be removed",												"Tous les op�rateurs doivent �tre supprim�s avant retrait de l'�cran ID op�rat.",															"Alle Bediener m�ssen gel�scht werden, bevor der Bediener-ID-Bildschirm entfernt werden kann",											"Debe eliminar todos los operarios antes de eliminar la pantalla Id. operario",												"Prima di rimuovere lo schermo ID operatore, annullare tutti gli operatori",													"Devem ser eliminados todos os operadores antes do ecr� de ID de operadores poder ser eliminado",									"Todos os operadores devem ser exclu�dos antes que a tela de ID de operador possa ser removida"	}	},
	{ ESTR_CONTROL_EXPIRED			,	{	"Control renewal required",																									"Renouvell. contr�le obligatoire",																											"Kontrollerneuerung erforderlich",																										"Debe renovar control",																										"Necessario rinnovo controllo",																									"� necess�rio renovar o controlo",																									"Renova��o do controle necess�ria"	}	},
	{ ESTR_DATA_LOSS				,	{	"Changing software will result in data loss.  Press CANCEL to archive data prior to upgrading software.",					"La mise � niveau va occasionner la perte de donn�es. Appuyer sur ANNULER pour archiver les donn�es avant de mettre � niveau le logiciel.",	"Bei einer Softwareaktualisierung werden Daten gel�scht. ABBRECHEN bet�tigen, um Daten vor der Softwareaktualisierung zu archivieren.",	"Al cambiar el software perder� sus datos. Pulse CANCELAR si desea archivar los datos antes de actualizar el software.",	"La modifica di software determina la perdita di dati. Premere ANNULLA per archiviare i dati prima di aggiornare il software.",	"Ao alterar o software ir� perder os seus dados. Prima CANCELAR se desejar arquivar os dados antes da atualiza��o do software.",	"A altera��o do software resultar� em perda de dados. Pressione CANCELAR para arquivar os dados antes de atualizar o software."	}	},
	{ ESTR_DUPLICATE_ADMIN_ID		,	{	"Duplicate Administrator ID entered",																						"ID admin. saisi en double",																												"Doppelte Administrator-ID eingegeben",																									"Esta Id. administrador ya existe",																							"Inserito duplicato ID amministratore",																							"A ID do administrador j� existe",																									"ID de administrador duplicado inserido"	}	},
	{ ESTR_DUPLICATE_OP_ID			,	{	"Duplicate Operator ID entered",																							"ID op�rateur saisi en double",																												"Doppelte Bediener-ID eingegeben",																										"Esta Id. operario ya existe",																								"Inserito duplicato ID operatore",																								"A ID do operador j� existe",																										"C�digo de operador duplicado digitado"	}	},
	{ ESTR_OP_RENEWAL_REQUIRED		,	{	"Operator renewal required",																								"Renouvell. op�rateur obligatoire",																											"Bediener-Erneuerung erforderlich",																										"Debe renovar operario",																									"Necessario rinnovo operatore",																									"� necess�rio renovar o operador",																									"Renova��o de operador necess�ria"	}	},
	{ ESTR_REMOVE_FROM_ADMIN_LIST	,	{	"will be removed from administrators list",																					"sera retir� de la liste d'administrateurs",																								"wird aus der Administratorenliste entfernt",																							"se eliminar� de la lista de administradores", 																				"sar� rimosso dall'elenco amministratori",																						"ser� eliminado da lista de administradores",																						"ser� removido da lista de administradores"	}	},
	{ ESTR_REMOVE_FROM_OP_LIST		,	{	"will be removed from operators list",																						"sera retir� de la liste d'op�rateurs",																										"wird aus der Bedienerliste entfernt",																									"se eliminar� de la lista de operarios",																					"sar� rimosso dall'elenco operatori",																							"ser� eliminado da lista de operadores",																							"ser� removido da lista de operadores"	}	},
	{ ESTR_REMOVE_FROM_TYPE_LIST	,	{	"will be removed from Type list",																							"sera retir� du type de liste",																												"wird aus der Typenliste entfernt",																										"se eliminar� de la lista de tipos",																						"sar� rimosso dall'elenco Tipi",																								"ser� eliminado da lista de tipos",																									"ser� removido da lista de Tipo"	}	},
	{ ESTR_RESTORING_REF_RANGES		,	{	"Restoring Reference Ranges to",																							"R�tablissement des fourchettes de r�f�rence sur",																							"Wiederherstellung der Referenzbereiche auf",																							"Reestablecer los intervalos de referencia a",																				"Riportare i range di riferimento a",																							"Restaurar os intervalos de refer�ncia para",																						"Restaurando intervalos de refer�ncia para"	}	},
	{ ESTR_ROTOR_DETECTED			,	{	"Disc detected during warming",																								"Disque d�tect� en cours de chauffage",																										"Scheibe beim Aufw�rmen vorgefunden",																									"Disco detectado durante el calentamiento inicial",																			"Disco individuato durante riscaldamento",																						"Disco detetado durante o aquecimento",																								"Disco detectado durante o aquecimento"	}	},
	{ ESTR_UNAUTHORIZED_OP			,	{	"Unauthorized operator",																									"Op�rateur non-autoris�",																													"Unautorisierter Bediener",																												"Operario no autorizado",																									"Operatore non autorizzato",																									"Operador n�o autorizado",																											"Operador n�o autorizado"	}	},

// Error info text
	{ EISTR_ADMIN_ID_LIST_FULL		,	{	"All the available space for Administrator ID's is used.  Please remove at least one Administrator ID before adding a new Administrator ID.\n",													// English
											"Tout l'espace disponible r�serv� aux ID administrateur est utilis�. Veuillez supprimer au moins un ID administrateur avant d'en ajouter un nouveau.\n",										// French
											"Der gesamte Speicherplatz f�r Administrator-IDs ist belegt. Bitte entfernen Sie mindestens eine Administrator-ID, bevor Sie eine neue hinzuf�gen.\n",											// German
											"Se ha utilizado todo el espacio disponible para n�meros de identificaci�n de administradores. Elimine al menos un n�mero de identificaci�n de administrador antes de a�adir otro nuevo.\n",	// Spanish
											"� stato utilizzato tutto lo spazio disponibile per l'ID amministratore. Rimuovere almeno un ID amministratore prima di aggiungere un nuovo ID amministratore.\n",								// Italian
											"Foi utilizado todo o espa�o dispon�vel para IDs de administradores.  Elimine pelo menos uma ID de administrador antes de adicionar uma ID de administrador nova.\n",							// Portuguese
											"Todo o espa�o dispon�vel para os IDs de administrador est� usado.  Remova pelo menos um ID de administrador antes de adicionar um novo ID de administrador.\n"									// Portuguese (Brazil)
										}
	},

	{ EISTR_ADMIN_NOT_FOUND			,	{	"The administrator ID entered is not in the system. Please re-enter the ID.\n",						// English
											"L'ID admin. n'est pas saisi sur le syst�me. Saisir � nouveau l'ID\n",								// French
											"Die eingegebene Administrator-ID existiert nicht im System. Bitte geben Sie die ID erneut ein.\n",	// German
											"Esta Id. administrador no est� en el sistema. Por favor, vuelva a escribirla.\n",					// Spanish
											"L'ID amministratore inserito non presente nel sistema. Pregasi inserire ID nuovamente.\n",			// Italian
											"A ID de administrador introduzida n�o est� no sistema. Introduza novamente a ID.\n",				// Portuguese
											"O ID de administrador inserido n�o est� no sistema. Insira novamente o ID.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_AMBIENT_TEMP			,	{	"The temperature outside the analyzer is either too hot or too cold to run a disc. Contact Technical Service for assistance.\n",															// English
											"La temp�rature � l'ext�rieur de l'analyseur est trop �lev�e ou trop faible pour permettre le fonctionnement d'un disque. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Die Temperatur au�erhalb des Analyseger�ts ist f�r den Scheibenbetrieb zu hei� oder zu kalt. Wenden Sie sich an den technischen Service.\n",												// German
											"La temperatura fuera del analizador es demasiado alta o demasiado baja para usar el disco. P�ngase en contacto con el servicio t�cnico.\n",												// Spanish
											"La temperatura esterna all'analizzatore � troppo alta o troppo bassa per permettere il funzionamento di un disco. Per assistenza contattare il servizio tecnico.\n",						// Italian
											"A temperatura fora do analisador � demasiado alta ou demasiado baixa para executar um disco. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",							// Portuguese
											"A temperatura externa do analisador est� muito alta ou muito baixa para executar um disco. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_ANALYSIS_CANCELED		,	{	"The analysis was cancelled by pressing the Cancel on the screen.\n",					// English
											"Annulation de l'analyse par pression sur la touche Annuler sur l'�cran\n",				// French
											"Die Analyse wurde mit der Schaltfl�che Abbrechen auf dem Bildschirm abgebrochen.\n",	// German
											"El an�lisis fue cancelado al pulsar Cancelar en la pantalla.\n",						// Spanish
											"L'analisi � stata annullata premendo il tasto Annulla sullo schermo.\n",				// Italian
											"A an�lise foi cancelada ao premir Cancelar no ecr�.\n",								// Portuguese
											"A an�lise foi cancelada pela press�o do bot�o Cancelar na tela.\n"						// Portuguese (Brazil)
										}
	},
	{ EISTR_ANALYSIS_TIMEOUT		,	{	"The analyzer is not completing the analysis within the time allotted. Contact Technical Service for assistance.\n",								// English
											"L'Analyseur n'a pas termin� l'analyse dans le temps imparti. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											"Das Analyseger�t f�hrt die Analyse nicht im vorgesehenen Zeitraum durch. Wenden Sie sich an den technischen Service.\n",						// German
											"El analizador no est� completando el an�lisis en el tiempo asignado. P�ngase en contacto con el servicio t�cnico.\n",							// Spanish
											"L'analizzatore non sta completando l'analisi entro il tempo assegnato. Per assistenza contattare il servizio tecnico\n",						// Italian
											"O analisador n�o est� a concluir a an�lise no tempo atribu�do.  Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia\n",			// Portuguese
											"O analisador n�o est� concluindo a an�lise dentro do tempo alocado.  Entre em contato com o Servi�o T�cnico para obter assist�ncia\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_ANALYZER_DATE			,	{	"Date in analyzer is earlier than that of the disc being run. Check dates in analyzer and rerun with new disc.\n",												// English
											"La date de l'analyseur est ant�rieure � celle du disque en cours d'utilisation. V�rifier les dates sur l'analyseur et r�ex�cuter le nouveau disque.\n",		// French
											"Das Datum im Analyseger�t liegt vor dem des Scheibenbetriebs. Pr�fen Sie das Datum im Analyseger�t und nehmen Sie den Betrieb mit einer neuen Scheibe auf.\n",	// German
											"La fecha del analizador es anterior a la fecha del disco que se va a usar. Compruebe las fechas del analizador y vuelva a usar el nuevo disco.\n",				// Spanish
											"La data dell'analizzatore � precedente a quella di attivazione del disco Controllare le date dell'analizzatore e riavviare con un nuovo disco.\n",				// Italian
											"A data do analisador � anterior � data do disco que foi iniciado. Verifique as datas do analisador e reinicie com um disco novo.\n",							// Portuguese
											"A data no analisador � anterior � do disco em execu��o. Verifique as datas no analisador e execute novamente com um novo disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_BACKGROUND_ABSORBANCE	,	{	"Background absorbance is too high. Contact Technical Service for assistance.\n",								// English
											"L'absorbance d'arri�re-plan est trop �lev�e. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Die Hintergrundabsorbierung ist zu hoch. Wenden Sie sich an den technischen Service.\n",						// German
											"La absorbancia de fondo es muy alta. P�ngase en contacto con el servicio t�cnico.\n",							// Spanish
											"L'assorbimento di fondo � troppo alto. Per assistenza contattare il servizio tecnico.\n",						// Italian
											"A absorv�ncia de fundo � muito elevada. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											"A absorb�ncia do fundo est� muito alta. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_BACKGROUND_NOISE		,	{	"The analyzer detected a high background signal. This may impact the accuracy of the analyzer's absorbance measurement. If symptom continues, contact Technical Service.\n",														// English
											"L'analyseur a d�tect� un signal d'arri�re-plan �lev�. Ceci peut affecter la pr�cision de la mesure d'absorbance par l'analyseur. En cas de persistance des sympt�mes, contacter le service technique.\n",							// French
											"Das Analyseger�t hat ein hohes Hintergrundsignal festgestellt. Dies kann die Genauigkeit der Absorbierungsmessung des Analyseger�ts beeintr�chtigen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",	// German
											"El analizador detecta una se�al de fondo alta. Esto puede afectar a la precisi�n de la medida de absorbancia del analizador. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",			// Spanish
											"L'analizzatore ha individuato un forte segnale di fondo. Esso potrebbe influire sulla precisione della misurazione di assorbenza dell'analizzatore. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											"O analisador detetou um sinal de fundo elevado. Isto pode afetar a precis�o da medi��o da absorv�ncia do analisador. Se o problema persistir, contacte o Servi�o de Assist�ncia T�cnica.\n",										// Portuguese
											"O analisador detectou um sinal de fundo alto. Isso pode afetar a precis�o da medi��o de absorb�ncia do analisador. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"												// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE					,	{	"The analyzer was unable to read the disc's barcode. If symptom continues, contact Technical Service.\n",												// English
											"L'analyseur n'a pas pu lire le code � barres du disque. En cas de persistance des sympt�mes, contacter le service technique.\n",						// French
											"Das Analyseger�t konnte den Barcode der Scheibe nicht lesen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",				// German
											"El analizador no pudo leer el c�digo de barras del disco. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"L'analizzatore non � riuscito a leggere il codice a barre del disco. Se i sintomi continuano contattare il servizio tecnico.\n",						// Italian
											"O analisador n�o consegue ler o c�digo de barras do disco. Se o problema persistir, contacte o Servi�o de Assist�ncia T�cnica.\n",						// Portuguese
											"O analisador n�o foi capaz de ler o c�digo de barras do disco. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE_FORMAT			,	{	"The analyzer did not recognize the disc. Contact Technical Service for further assistance.\n",							// English
											"L'analyseur n'a pas reconnu le disque. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											"Das Analyseger�t konnte die Scheibe nicht erkennen. Wenden Sie sich an den technischen Service.\n",					// German
											"El analizador no ha reconocido el disco. P�ngase en contacto con el servicio t�cnico.\n",								// Spanish
											"L'analizzatore non ha riconosciuto il disco. Per ulteriore assistenza contattare il servizio tecnico.\n",				// Italian
											"O analisador n�o consegue reconhecer o disco. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											"O analisador n�o reconheceu o disco. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MISSING			,	{	"The analyzer did not detect all the chemistry beads in the disc. Rerun sample using new disc.\n",										// English
											"L'analyseur n'a pas d�tect� toutes les billes chimiques dans le disque. R�ex�cuter l'�chantillon � l'aide d'un nouveau disque.\n",		// French
											"Das Analyseger�t konnte nicht alle chemischen Beads in der Scheibe erkennen. Wiederholen Sie die Probe mit einer neuen Scheibe.\n",	// German
											"El analizador no detect� todas las cuentas qu�micas en el disco. Vuelva a utilizar la muestra usando un nuevo disco.\n",				// Spanish
											"L'analizzatore non ha individuato tutti i granuli chimici nel disco. Riavviare il campione con un nuovo disco.\n",						// Italian
											"O analisador n�o detetou todas as esferas qu�micas no disco. Executar novamente a amostra utilizando um disco novo.\n",				// Portuguese
											"O analisador n�o detectou todas as esferas de qu�micas no disco. Execute novamente a amostra usando um novo disco.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MIX				,	{	"The analyzer will not be able to determine the exposure status if the QC beads have not mixed correctly. Rerun with a new disc.\n",									// English
											"L'analyseur ne parviendra pas � d�terminer l'�tat d'exposition si les billes QC ne sont pas correctement m�lang�es. R�ex�cuter avec un nouveau disque.\n",				// French
											"Das Analyseger�t kann den Status der Einwirkung nicht feststellen, wenn die QC-Beads nicht ordnungsgem�� vermischt wurden. Mit einem neuen Rotor wiederholen.\n",		// German
											"El analizador no ser� capaz de determinar el estado de exposici�n si las cuentas QC no se mezclan correctamente. Debe usarse con un nuevo disco.\n",					// Spanish
											"L'analizzatore non potr� determinare lo stato di esposizione se i granuli di QC non sono stati mescolati correttamente. Riavviare con un nuovo disco.\n",				// Italian
											"O analisador n�o conseguir� determinar o estado de exposi��o se as esferas de CQ n�o forem misturadas adequadamente. Executar novamente utilizando um disco novo.\n",	// Portuguese
											"O analisador n�o poder� determinar o status de exposi��o se as esferas de QC n�o tiverem sido misturadas corretamente. Execute novamente com um novo disco.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_BIAS					,	{	"The analyzer detected a high bias signal. This may impact the accuracy of the analyzer's measurement of absorbance. If symptom continues, contact Technical Service.\n",														// English
											"L'analyseur a d�tect� un signal de polarisation �lev�, ce qui peut affecter la pr�cision de la mesure d'absorbance par l'analyseur. En cas de persistance des sympt�mes, contacter le service technique.\n",					// French
											"Das Analyseger�t hat ein hohes St�rsignal festgestellt. Dies kann die Genauigkeit der Absorbierungsmessung des Analyseger�ts beeintr�chtigen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",		// German
											"El analizador detecta una se�al de polarizaci�n alta. Esto puede afectar a la precisi�n de la medida de absorbancia del analizador. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"L'analizzatore ha individuato un forte segnale di bias. Esso potrebbe influire sulla precisione della misurazione di assorbenza dell'analizzatore. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											"O analisador detetou um sinal de desvio elevado. Isto pode afetar a precis�o da medi��o da absorv�ncia do analisador. Se o problema persistir, contacte o Servi�o de Assist�ncia T�cnica.\n",									// Portuguese
											"O analisador detectou um sinal de polariza��o alto. Isso pode afetar a precis�o da medi��o de absorb�ncia do analisador. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_BOTTOM_HEATER_HIGH		,	{	"The bottom heater plate draws too much current when turned on at full power. Contact Technical Service for assistance.\n",												// English
											"La plaque chauffante inf�rieure consomme trop d'�lectricit� lorsqu'elle fonctionne � fond. Pour obtenir une assistance, contacter le service technique.\n",			// French
											"Die untere Heizplatte verbraucht bei Maximalbetrieb zu viel Strom. Wenden Sie sich an den technischen Service.\n",														// German
											"La placa del calentador inferior utiliza mucha corriente cuando se enciende a toda potencia. P�ngase en contacto con el servicio t�cnico.\n",							// Spanish
											"Il disco di riscaldamento inferiore prende troppa corrente quando � acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",					// Italian
											"A placa inferior do aquecedor utiliza demasiada de corrente quando ligada na pot�ncia m�xima. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											"A placa inferior do aquecedor consome muita corrente quando ligada na pot�ncia m�xima. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_BOTTOM_HEATER_LOW		,	{	"The bottom heater plate did not draw enough current when turned on at full power. Contact Technical Service for assistance.\n",										// English
											"La plaque chauffante inf�rieure n'a pas consomm� trop d'�lectricit� lorsqu'elle a fonctionn� � fond. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Die untere Heizplatte verbrauchte bei Maximalbetrieb zu wenig Strom. Wenden Sie sich an den technischen Service.\n",													// German
											"La placa del calentador inferior no utiliz� suficiente corriente cuando se puso a toda potencia. P�ngase en contacto con el servicio t�cnico.\n",						// Spanish
											"Il disco di riscaldamento inferiore non ha preso sufficiente corrente quando � acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",			// Italian
											"A placa inferior do aquecedor n�o utiliza corrente suficiente quando ligada na pot�ncia m�xima. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											"A placa inferior do aquecedor n�o atraiu corrente suficiente quando ligada na pot�ncia m�xima. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_CANNOT_REMOVE_OP_ID		,	{	"To remove the operator ID prompt it is necessary to remove all operator IDs first.  Please use the Operator Administration menu to remove all operators, and then return here to remove the operator prompt.\n",																// English
											"Pour supprimer l'invite ID op�rateur, il est n�cessaire de commencer par supprimer tous les ID op�rateur.  Supprimer tous les op�rateurs � partir du menu Administration op�rateur, puis revenir ici pour supprimer l'invite op�rateur.\n",									// French
											"Um die Bediener-ID-Eingabeaufforderung zu entfernen, m�ssen Sie zuerst alle Bediener-IDs l�schen. Bitte machen Sie dies �ber das Bediener-Administrationsmen� und gehen zur�ck, um die Bediener-Eingabeaufforderung zu entfernen.\n",											// German
											"Para eliminar el aviso de Id. operario debe eliminar todas las Id. operador. Para esto, vaya al men� Administraci�n de operarios para eliminar todos los operarios y despu�s vuelva aqu� para eliminar el aviso.\n",															// Spanish
											"Per rimuovere il prompt ID operatore si devono prima rimuovere tutti gli ID operatore. Pregasi utilizzare il menu Amministrazione operatore per rimuovere tutti gli operatori, quindi tornare a questo punto e rimuovere il prompt operatore.\n",								// Italian
											"Para eliminar o aviso de ID de operador � necess�rio eliminar primeiro todas as ID de operador.  Para remover todos os operadores utilize o men� Administra��o de operadores para eliminar todos os operadores e, em seguida, volte novamente aqui para eliminar o aviso.\n",	// Portuguese
											"Para remover o prompt do ID de operador, � necess�rio remover todos os IDs de operador antes.  Use o menu Administra��o do operador para remover todos os operadores e, em seguida, retorne aqui para remover o prompt do operador.\n"											// Portuguese (Brazil)
										} 
	},
	{ EISTR_CDROM					,	{	"The analyzer is unable to read information from the CD ROM. Contact Technical Service for assistance.\n",									// English
											"L'analyseur n'est pas en mesure de lire des informations sur le CD ROM. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Das Analyseger�t kann die CD-ROM nicht lesen. Wenden Sie sich an den technischen Service.\n",												// German
											"El analizador no puede leer la informaci�n del CD-ROM. P�ngase en contacto con el servicio t�cnico.\n",									// Spanish
											"L'analizzatore non � in grado di leggere le informazioni dal CD ROM. Per assistenza contattare il servizio tecnico.\n",					// Italian
											"O analisador n�o consegue ler a informa��o do CD-ROM. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",				// Portuguese
											"O analisador n�o � capaz de ler as informa��es do CD-ROM. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_CONTROL_EXPIRED			,	{	"The date set for analyzing control samples has passed.  Please contact technical support for more information.\n",										// English
											"La date pr�vue pour l'analyse des �chantillons de contr�le est d�pass�e. Pour plus d'informations, contacter l'assistance technique.\n",				// French
											"Das angegebene Datum, um die Kontrollproben zu analysieren, ist abgelaufen. Wenden Sie sich an den technischen Service f�r weitere Information.\n",	// German
											"La fecha para analizar las muestras de control ya ha pasado. P�ngase en contacto con el servicio t�cnico.\n",											// Spanish
											"La data impostata per l'analisi dei campioni di controllo � stata superata. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",		// Italian
											"A data definida para analisar as amostras controlo foi excedida.  Para obter mais informa��es, contacte o Servi�o de Assist�ncia T�cnica.\n",			// Portuguese
											"A data configurada para analisar as amostras de controle j� passou.  Entre em contato com o suporte t�cnico para obter mais informa��es.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_CUVETTE_MARK			,	{	"The analyzer was unable to detect the disc. If symptom continues, contact Technical Service.\n",												// English
											"L'analyseur n'est pas parvenu � d�tecter le disque. En cas de persistance des sympt�mes, contacter le service technique.\n",					// French
											"Das Analyseger�t konnte die Scheibe nicht finden. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",					// German
											"El analizador no fue capaz de detectar el disco. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"L'analizzatore non � riuscito a individuare il disco. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"O analisador n�o consegue detetar o disco. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",								// Portuguese
											"O analisador n�o conseguiu detectar o disco. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_DARK_NOISE				,	{	"There was high variability in the background signal. This degrades the accuracy of the analyzer to measure absorbance. If symptom continues, contact Technical Service.\n",														// English
											"Le signal d'arri�re-plan a pr�sent� une variabilit� �lev�e, ce qui r�duit la pr�cision de l'analyseur pour la mesure d'absorbance. En cas de persistance des sympt�mes, contacter le Service technique.\n",						// French
											"Es bestand eine hohe Hintergrundsignalvarianz. Dies beeintr�chtigt die Genauigkeit der Absorbierungsmessung des Analyseger�ts. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",							// German
											"Hab�a una gran variabilidad en la se�al de fondo. Esto rebaja la precisi�n del analizador para analizar la medida de la absorbancia. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"Grande variabilit� nel segnale di fondo. Ci� influisce negativamente la precisione dell'analizzatore nella misurazione dell'assorbenza. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"Foi observada uma grande variabilidade no sinal de fundo. Isto reduz a precis�o do analisador para realizar medi��es de absorv�ncia. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",						// Portuguese
											"Houve alta variabilidade no sinal de fundo. Isso degrada a precis�o do analisador para medir a absorb�ncia. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"														// Portuguese (Brazil)
										} 
	},
	{ EISTR_DATA_LOSS				,	{	"The software needs to change the format of the data on the analyzer.  It is not possible to convert the data and change the software.  Pressing CANCEL will allow the existing data to be archived prior to upgrading the software.  Use the Transmit All selection under the Recall screen to archive the existing data external to the analyzer.  Verify the external archive prior to upgrading the software.\n",								// English
											"Le logiciel doit modifier le format des donn�es sur l'analyseur. Il n'est pas possible de convertir les donn�es et de mettre � niveau le logiciel. Appuyer sur ANNULER pour archiver les donn�es avant la mise � niveau du logiciel. Appuyer sur Transmettre tout sous l'�cran Rappel pour archiver les donn�es de l'analyseur sur un support externe. V�rifier l'archive externe avant de mettre � niveau le logiciel.\n",						// French
											"Die Software �ndert das Datenformat auf dem Analysesystem. Es ist nicht m�glich, die Daten zu konvertieren und die Software zu aktualisieren. ABBRECHEN bet�tigen, um die bestehenden Daten vor der Softwareaktualisierung zu archivieren. Die Funktion �Alle �bertragen� im Men� �Wiederaufruf� verwenden, um die bestehenden Daten auf einem externen Ger�t zu archivieren. Das externe Archiv vor der Softwareaktualisierung �berpr�fen.\n",	// German
											"El software debe cambiar el formato de los datos del analizador. No se pueden convertir los datos y cambiar el software. Pulse CANCELAR si desea archivar los datos existentes antes de actualizar el software. Para archivar los datos existentes fuera del analizador, use la opci�n Trasmitir todo de la pantalla Recordar. Antes de actualizar el software, verifique el archivo externo.\n",													// Spanish
											"Il software deve modificare il formato dei dati sull'analizzatore. Non � possibile convertire i dati e cambiare il software. Premendo ANNULLA si consente l'archiviazione dei dati esistenti prima dell'aggiornamento del software. Usare la selezione Trasmetti tutti nella schermata Richiama per archiviare i dati esistenti all'esterno dell'analizzatore. Verificare l'archivio esterno prima di aggiornare il software.\n",					// Italian
											"O software necessita alterar o formato dos dados do analisador.  N�o � poss�vel converter os dados e alterar o software.  Ao premir CANCELAR permitir� arquivar os dados existentes antes de atualizar o software.  Para arquivar os dados existentes externos ao analisador, utilize a op��o Transmitir tudo no ecr� Recuperar.  Antes de atualizar o software, verifique o ficheiro externo.\n",													// Portuguese
											"O software precisa alterar o formato dos dados no analisador.  N�o � poss�vel converter os dados e alterar o software.  Pressione CANCELAR para que os dados existentes sejam arquivados antes de atualizar o software.  Use a sele��o Transmitir todos na tela Recuperar para arquivar os dados existentes externos ao analisador.  Verifique o arquivo externo antes de atualizar o software.\n"													// Portuguese (Brazil)
										}
	},
	{ EISTR_DILUTION				,	{	"The analyzer may not have delivered the correct amount of sample to the reagents. Confirm the reported values.\n",						// English
											"Il se peut que l'analyseur n'ait pas dispens� la quantit� d'�chantillon voulue sur les r�actifs. Confirmer les valeurs fournies.\n",	// French
											"Das Analyseger�t hat m�glicherweise die falsche Probenmenge den Reagenzien zugef�hrt. Berichtete Werte best�tigen.\n",					// German
											"Es posible que el analizador no proporcione la cantidad correcta de muestra a los reactivos. Confirme los valores dados.\n",			// Spanish
											"L'analizzatore potrebbe non aver portato la quantit� corretta di campione ai reagenti. Confermare i valori riportati.\n",				// Italian
											"� poss�vel que o analisador n�o tenha fornecido a quantidade adequada de amostra aos reagentes. Confirme os valores reportados.\n",	// Portuguese
											"O analisador pode n�o ter entregue a quantidade correta de amostra aos reagentes. Confirme os valores relatados.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_DRAWER_CLOSE			,	{	"The drawer appears to be unable to close correctly. Remove any visible obstructions. If symptom continues, contact Technical Service.\n",											// English
											"Le tiroir ne semble pas se fermer correctement. �liminer tout �l�ment visible responsable du blocage. En cas de persistance des sympt�mes, contacter le service technique.\n",		// French
											"Das Fach kann nicht ordnungsgem�� geschlossen werden. Entfernen Sie sichtbare Hindernisse. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",				// German
											"Parece que la bandeja no puede cerrarse correctamente. Retire todo obst�culo visible. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"Sembra che il cassetto non riesca a chiudersi perfettamente. Rimuovere qualsiasi ostruzione visibile. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											"Parece que a gaveta n�o fecha corretamente. Remova qualquer obst�culo vis�vel. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",								// Portuguese
											"A gaveta parece incapaz de fechar corretamente. Remova quaisquer obstru��es vis�veis. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_DRAWER_OPEN				,	{	"The drawer appears to be unable to open correctly. Remove any visible obstructions. If symptom continues, contact Technical Service.\n",											// English
											"Le tiroir ne semble pas s'ouvrir correctement. �liminer tout �l�ment visible responsable du blocage. En cas de persistance des sympt�mes, contacter le service technique.\n",		// French
											"Das Fach kann nicht ordnungsgem�� ge�ffnet werden. Entfernen Sie sichtbare Hindernisse. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",				// German
											"Parece que la bandeja no puede abrirse correctamente. Retire todo obst�culo visible. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"Sembra che il cassetto non riesca ad aprirsi perfettamente. Rimuovere qualsiasi ostruzione visibile. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											"Parece que a gaveta n�o abre corretamente. Remova qualquer obst�culo vis�vel. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",								// Portuguese
											"A gaveta parece incapaz de abrir corretamente. Remova quaisquer obstru��es vis�veis. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_DUPLICATE_ADMIN_ID		,	{	"The Administrator ID entered already exists in the system. Duplicate IDs are not allowed.\n",		// English
											"L'ID administrateur saisi existe d�j� sur le syst�me. Les ID en double ne sont pas autoris�s.\n",	// French
											"Die eingegebene Administrator-ID existiert bereits im System. Doppelte IDs sind nicht erlaubt.\n",	// German
											"Esta Id. administrador ya existe en el sistema. No se permiten Id. duplicadas.\n",					// Spanish
											"L'ID amministratore inserito � gi� presente nel sistema. Non sono consentiti ID duplicati.\n",		// Italian
											"A ID de administrador introduzida j� existe no sistema. N�o s�o permitidas IDs duplicadas.\n",		// Portuguese
											"O ID de administrador inserido j� existe no sistema. IDs duplicados n�o s�o permitidos.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_DUPLICATE_OP_ID			,	{	"The Operator ID entered already exists in the system. Duplicate IDs are not allowed.\n",		// English
											"L'ID op�rateur saisi existe d�j� sur le syst�me. Les ID en double ne sont pas autoris�s.\n",	// French
											"Die eingegebene Bediener-ID existiert bereits im System. Doppelte IDs sind nicht erlaubt.\n",	// German
											"Esta Id. operario ya existe en el sistema. No se permiten Id. duplicadas.\n",					// Spanish
											"L'ID operatore inserito � gi� presente nel sistema. Non sono consentiti ID duplicati.\n",		// Italian
											"A ID de operador introduzida j� existe no sistema. N�o s�o permitidas IDs duplicadas.\n",		// Portuguese
											"O ID de operador inserido j� existe no sistema. IDs duplicados n�o s�o permitidos.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_ENGINE_RESET			,	{	"A component of the analyzer has restarted unexpectedly. Contact Technical Service for assistance.\n",									// English
											"Un des composants de l'analyseur a red�marr� de mani�re inattendue. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Eine Komponente des Analyseger�ts wurde unerwartet neu gestartet. Wenden Sie sich an den technischen Service.\n",						// German
											"Un componente del analizador se ha reiniciado inesperadamente. P�ngase en contacto con el servicio t�cnico.\n",						// Spanish
											"Un componente dell'analizzatore si � riavviato in modo imprevisto. Per assistenza contattare il servizio tecnico\n",					// Italian
											"Um componente do analisador foi reiniciado inesperadamente.  Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",		// Portuguese
											"Um componente do analisador foi reiniciado inesperadamente.  Entre em contato com o Servi�o T�cnico para obter assist�ncia\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_EXPIRED_ROTOR			,	{	"The disc has expired and should not be used. Contact Technical Service for further details.\n",										// English
											"La date de p�remption du disque est d�pass�e. Ne pas l'utiliser. Pour obtenir plus d'informations, contacter le service technique.\n",	// French
											"Die Scheibe ist abgelaufen und sollte ersetzt werden. Wenden Sie sich an den technischen Service.\n",									// German
											"El disco ha caducado y no deber�a utilizarse. P�ngase en contacto con el servicio t�cnico.\n",											// Spanish
											"Il disco � scaduto e non dovrebbe essere usato. Per ulteriore dettagli contattare il servizio tecnico.\n",								// Italian
											"O disco expirou e n�o dever� ser utilizado. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",				// Portuguese
											"O disco expirou e n�o deve ser usado. Entre em contato com o Servi�o T�cnico para obter detalhes adicionais.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_FILTER_CURV				,	{	"The filter curve calibration factors have been corrupted. Contact Technical Service for assistance.\n",								// English
											"Les facteurs de calibrage de la courbe du filtre sont corrompus. Pour obtenir une assistance, contacter le service technique.\n",		// French
											"Die Kalibrierungsfaktoren der Filterkurve sind fehlerhaft. Wenden Sie sich an den technischen Service.\n",								// German
											"Los factores de calibraci�n de la curva del filtro est�n da�ados. P�ngase en contacto con el servicio t�cnico.\n",						// Spanish
											"I fattori di calibrazione della curva del filtro sono corrotti. Per assistenza contattare il servizio tecnico.\n",						// Italian
											"Os fatores da curva de calibra��o do filtro est�o danificados. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											"Os fatores de calibra��o da curva do filtro foram corrompidos. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_FLASH_LAMP_NOISE		,	{	"During one of the system checks, the analyzer attempted to compute an absorbance from a cluster of flashes that are too variable from each other. If symptom continues, contact Technical Service.\n",															// English
											"Lors de l'une des v�rifications du syst�me, l'analyseur a tent� de calculer l'absorbance � partir d'un ensemble de flashs qui varient trop les uns des autres. En cas de persistance des sympt�mes, contacter le service technique.\n",						// French
											"Bei einer Systempr�fung unternahm das Analyseger�t den Versuch, eine Absorbierung aus einer Gruppe von Lichtstrahlen zu berechnen, die zu sehr voneinander abweichen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",				// German
											"Durante una de las comprobaciones del sistema, el analizador intent� calcular una absorbancia de un grupo de flashes que son excesivamente distintos unos de otros. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"Durante uno dei controlli di sistema l'analizzatore ha cercato di calcolare l'assorbenza in un gruppo di flash luminosi troppo diversi tra loro. Se i sintomi continuano contattare il servizio tecnico.\n",													// Italian
											"Durante uma das verifica��es do sistema, o analisador tentou calcular uma absorv�ncia de um grupo de flashes extremamente diferentes uns dos outros. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",									// Portuguese
											"Durante uma das verifica��es do sistema, o analisador tentou calcular uma absorb�ncia de um conjunto de flashes que s�o muito vari�veis um em rela��o ao outro. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_FLASH_READ				,	{	"The analyzer is unable to read information required to operate. Contact Technical Service for assistance.\n",											// English
											"L'analyseur n'est pas en mesure de lire les informations requises pour fonctionner. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Das Analyseger�t kann f�r den Betrieb erforderliche Informationen nicht abrufen. Wenden Sie sich an den technischen Service.\n",						// German
											"El analizador es incapaz de leer informaci�n necesaria para funcionar. P�ngase en contacto con el servicio t�cnico.\n",								// Spanish
											"L'analizzatore non � in grado di leggere le informazioni necessarie per il funzionamento. Per assistenza contattare il servizio tecnico.\n",			// Italian
											"O analisador n�o consegue ler informa��o necess�ria para funcionar. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",				// Portuguese
											"O analisador n�o � capaz de ler as informa��es necess�rias para operar. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_FLASH_SAVE				,	{	"The analyzer is unable to save information required to operate. Contact Technical Service for assistance.\n",												// English
											"L'analyseur n'est pas en mesure d'enregistrer les informations requises pour fonctionner. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Das Analyseger�t kann f�r den Betrieb erforderliche Informationen nicht speichern. Wenden Sie sich an den technischen Service.\n",							// German
											"El analizador es incapaz de guardar informaci�n necesaria para funcionar. P�ngase en contacto con el servicio t�cnico.\n",									// Spanish
											"L'analizzatore non � in grado di salvare le informazioni necessarie per il funzionamento. Per assistenza contattare il servizio tecnico.\n",				// Italian
											"O analisador n�o consegue gravar informa��o necess�ria para funcionar. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",				// Portuguese
											"O analisador n�o � capaz de salvar as informa��es necess�rias para operar. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_HARDWARE				,	{	"The analyzer has detected an error in the hardware. Contact Technical Service for further details.\n",					// English
											"L'analyseur a d�tect� une erreur de mat�riel. Pour obtenir plus d'informations, contacter le service technique.\n",	// French
											"Das Analyseger�t hat einen Hardwarefehler festgestellt. Wenden Sie sich an den technischen Service.\n",				// German
											"El analizador ha detectado un error en el equipo. P�ngase en contacto con el servicio t�cnico.\n",						// Spanish
											"L'analizzatore ha individuato un errore nell'hardware. Per ulteriore dettagli contattare il servizio tecnico.\n",		// Italian
											"O analisador detetou um erro no hardware. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",	// Portuguese
											"O analisador detectou um erro no hardware. Entre em contato com o Servi�o T�cnico para obter detalhes adicionais.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_ILLEGAL_BARCODE			,	{	"The analyzer has detected an illegal barcode. Contact Technical Service for further details.\n",								// English
											"L'analyseur a d�tect� un code � barres non autoris�. Pour obtenir plus d'informations, contacter le service technique.\n",		// French
											"Das Analyseger�t hat einen falschen Barcode festgestellt. Wenden Sie sich an den technischen Service.\n",						// German
											"El analizador ha detectado un c�digo de barras ilegal. P�ngase en contacto con el servicio t�cnico.\n",						// Spanish
											"L'analizzatore ha individuato un codice a barre illegale. Per ulteriore dettagli contattare il servizio tecnico.\n",			// Italian
											"O analisador detetou um c�digo de barras inv�lido. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",	// Portuguese
											"O analisador detectou um c�digo de barras ilegal. Entre em contato com o Servi�o T�cnico para obter detalhes adicionais.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_ILLEGAL_IP_ADDRESS		,	{	"The IP Address entered cannot be used.  Please enter an IP address in the N.N.N.N format where N is a number between 0 and 255.\n",						// English
											"L'adresse IP saisie ne peut pas �tre utilis�e.  Entrer une adresse IP au format N.N.N.N, N �tant un nombre compris entre 0 et 255.\n",						// French
											"Die eingegebene IP-Adresse ist nicht nutzbar. Bitte geben Sie eine IP-Adresse im Format N.N.N.N ein, wobei N eine Zahl zwischen 0 und 255 ist.\n",			// German
											"La direcci�n IP introducida no puede utilizarse.  Por favor, introduzca una direcci�n IP con el formato N.N.N.N, donde N es un n�mero entre 0 y 255.\n",	// Spanish
											"Impossibile utilizzare l'indirizzo IP specificato.  Immettere un indirizzo IP nel formato N.N.N.N. dove N � un numero compreso fra 0 e 255.\n",			// Italian
											"O endere�o IP introduzido n�o pode ser utilizado.  Introduza um endere�o IP com o formato N.N.N.N, onde N � um n�mero entre 0 e 255.\n",					// Portuguese
											"O endere�o IP inserido n�o pode ser usado.  Insira um endere�o IP no formato N.N.N.N, em que N � um n�mero entre 0 e 255.\n"								// Portuguese (Brazil)
										}
	},

	{ EISTR_ILLEGAL_IP_MASK			,	{	"The IP Mask entered cannot be used.  Please enter an IP Mask in the N.N.N.N format where N is a number between 0 and 255.\n",								// English
											"Le masque IP saisi ne peut pas �tre utilis�.  Entrer un masque IP au format N.N.N.N, N �tant un nombre compris entre 0 et 255.\n",							// French
											"Die eingegebene IP-Maske ist nicht nutzbar. Bitte geben Sie eine IP-Maske im Format N.N.N.N ein, wobei N eine Zahl zwischen 0 und 255 ist.\n",				// German
											"La m�scara IP introducida no puede utilizarse.  Por favor, introduzca una m�scara IP con el formato N.N.N.N, donde N es un n�mero entre 0 y 255.\n",		// Spanish
											"Impossibile utilizzare la maschera IP specificata.  Immettere una maschera IP nel formato N.N.N.N. dove N � un numero compreso fra 0 e 255.\n",			// Italian
											"A m�scara de IP introduzida n�o pode ser utilizada.  Introduza uma m�scara de IP com o formato N.N.N.N, onde N � um n�mero entre 0 e 255.\n",				// Portuguese
											"A m�scara IP inserida n�o pode ser usada.  Insira uma m�scara IP no formato N.N.N.N, em que N � um n�mero entre 0 e 255.\n"								// Portuguese (Brazil)
										}
	},
	{ EISTR_ILLEGAL_SOCKET			,	{	"The socket entered cannot be used.  Please enter a socket number between 0 and 65536.\n",								// English
											"Le socket saisi ne peut pas �tre utilis�.  Entrer un num�ro de socket compris entre 0 et 65536.\n",					// French
											"Der eingegebene Anschluss (Socket) ist nicht nutzbar. Bitte eine Anschlussnummer zwischen 0 und 65536 eingeben.\n",	// German
											"El socket introducido no puede utilizarse.  Por favor, introduzca un n�mero de enchufe entre 0 y 65536.\n",			// Spanish
											"Impossibile utilizzare il socket specificato.  Immettere un numero di socket compreso fra 0 e 65536.\n",				// Italian
											"A tomada introduzida n�o pode ser utilizada.  Introduza um n�mero de tomada entre 0 e 65 536.\n",						// Portuguese
											"O soquete inserido n�o pode ser usado.  Insira um n�mero de soquete entre 0 e 65.536.\n"								// Portuguese (Brazil)
										}
	},
	{ EISTR_INSUFFICIENT_DILUENT	,	{	"Insufficient reagents exist in the disc to complete the run. Rerun sample with new disc.\n",														// English
											"La quantit� de r�actifs dans le disque est insuffisante pour ex�cuter la proc�dure. R�ex�cuter l'�chantillon � l'aide du nouveau disque.\n",		// French
											"In der Scheibe befinden sich nicht ausreichend Reagenzien, um den Vorgang abzuschlie�en. Wiederholen Sie die Probe mit einer neuen Scheibe.\n",	// German
											"No hay suficientes reactivos en el disco para completar su uso. Vuelva a utilizar la muestra con un nuevo disco.\n",								// Spanish
											"Nel disco non vi sono sufficienti reagenti per completare l'operazione. Riavviare il campione con un nuovo disco.\n",								// Italian
											"N�o existem reagentes suficientes no disco para concluir a execu��o. Executar novamente a amostra utilizando um disco novo.\n",					// Portuguese
											"Existem reagentes insuficientes no disco para concluir a execu��o. Execute novamente a amostra com o novo disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_SAMPLE		,	{	"The disc has not received enough sample to function properly. Rerun using a new disc.\n",										// English
											"Le disque n'a pas re�u suffisamment d'�chantillons pour fonctionner correctement. R�ex�cuter avec un nouveau disque.\n",		// French
											"Die Scheibe enth�lt keine ausreichende Probenmenge f�r eine ordnungsgem��e Funktion. Mit einer neuen Scheibe wiederholen.\n",	// German
											"El disco no ha recibido suficiente muestra para funcionar correctamente. �selo con un nuevo disco.\n",							// Spanish
											"Il disco non ha ricevuto campione sufficiente per funzionare nel modo corretto. Riavviare utilizzando un nuovo disco.\n",		// Italian
											"O disco n�o recebeu amostra suficiente para funcionar adequadamente. Executar novamente utilizando um disco novo.\n",			// Portuguese
											"O disco n�o recebeu amostra suficiente para funcionar corretamente. Execute novamente usando um novo disco.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_STORAGE	,	{	"There is insufficient free space on the USB storage device to complete the service archive. Please remove the USB storage device, and connect a USB storage device with at least 256 MB of available space.\n",										// English
											"Espace de stockage sur p�riph�rique USB insuffisant pour r�aliser l'archive de service. Retirer le p�riph�rique de stockage USB et brancher un p�riph�rique de stockage USB ayant au moins 25 Mo d'espace disponible.\n",								// French
											"Es ist nicht gen�gend Speicherkapazit�t auf dem USB-Speicherger�t frei, um das Service-Archiv zu vollenden. Bitte entfernen Sie das USB-Speicherger�t und schlie�en Sie ein USB-Speicherger�t mit mindestens 256 MB freiem Speicher an.\n",			// German
											"El dispositivo de almacenamiento USB no tiene espacio suficiente para finalizar el archivo de servicio. Extraiga el dispositivo y conecte otro con al menos 256 MB de espacio disponible.\n",															// Spanish
											"Spazio libero sul dispositivo di archiviazione USB insufficiente per completare l'archivio di servizio. Pregasi rimuovere il dispositivo di archiviazione USB e collegare un dispositivo di archiviazione con almeno 256 MB di spazio disponibile.\n",	// Italian
											"O dispositivo de armazenamento USB n�o tem espa�o suficiente para finalizar o ficheiro de servi�o. Remova o dispositivo de armazenamento USB e insira outro com pelo menos 256 MB de espa�o dispon�vel.\n",											// Portuguese
											"N�o h� espa�o livre suficiente no dispositivo de armazenamento USB para concluir o arquivo de servi�o. Remova o dispositivo de armazenamento USB e conecte um dispositivo de armazenamento USB com pelo menos 256 MB de espa�o dispon�vel.\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_INTERNAL_COMM			,	{	"The analyzer lost internal communication. If symptom continues, contact Technical Service.\n",												// English
											"L'analyseur a perdu sa capacit� de communication interne. En cas de persistance des sympt�mes, contacter le service technique.\n",			// French
											"Das Analyseger�t hat die interne Kommunikation verloren. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",		// German
											"El analizador perdi� la comunicaci�n interna. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"L'analizzatore ha perso la comunicazione interna. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"O analisador perdeu a comunica��o interna. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",							// Portuguese
											"O analisador perdeu a comunica��o interna. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_INTERNAL_SOFTWARE		,	{	"The analyzer detected an error with the software.  If symptom continues, contact Technical Service.\n",									// English
											"L'analyseur a d�tect� une erreur logiciel. En cas de persistance des sympt�mes, contacter le service technique.\n",						// French
											"Das Analyseger�t hat einen Softwarefehler festgestellt. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",		// German
											"El analizador detect� un error con el software. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"L'analizzatore ha individuato un errore nel software. Se i sintomi continuano contattare il servizio tecnico.\n",							// Italian
											"O analisador detetou um erro no software.  Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",	// Portuguese
											"O analisador detectou um erro no software.  Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_IQC						,	{	"The analyzer Intelligent Quality Control (iQC) reagent dye beads differ too much from the expected values. Rerun using a new disc.\n",													// English
											"Les billes de teinture du r�actif Intelligent Quality Control (iQC) divergent trop des valeurs pr�vues. R�ex�cuter avec un nouveau disque.\n",											// French
											"Die Intelligent Quality Control (iQC) Reagenz-Farbstoff-Beads des Analyseger�ts weichen zu stark von den erwarteten Werten ab. Mit einer neuen Scheibe wiederholen.\n",				// German
											"Las cuentas de tinte de reactivo Intelligent Quality Control (iQC) del analizador son muy distintas a los valores esperados. �selo con un nuevo disco.\n",								// Spanish
											"I granuli di colore reagente Intelligent Quality Control (iQC) dell'analizzatore sono troppo diversi dai valori previsti. Riavviare utilizzando un nuovo disco.\n",					// Italian
											"As esferas de reagente corante do Controlo de qualidade inteligente (iQC) do analisador s�o muito diferentes dos valores esperados. Executar novamente utilizando um disco novo.\n",	// Portuguese
											"As esferas do corante reagente de Controle de qualidade inteligente (iQC) do analisador diferem muito dos valores esperados. Execute novamente usando um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_LAMP					,	{	"The lamp is checked each time to make sure it is still flashing when the optical cuvette is sampled. Detectors did not recognize the lamp flashes. If symptom continues, contact Technical Service.\n",																					// English
											"La lampe fait � chaque fois l'objet d'une v�rification afin de s'assurer qu'elle fonctionne toujours lors du pr�l�vement sur la cuvette optique. Les d�tecteurs n'ont pas reconnu les clignotements de la lampe. En cas de persistance des sympt�mes, contacter le service technique.\n",	// French
											"Die Lampe wird jedes Mal auf Blinkzeichen �berpr�ft, wenn eine Probe der optischen Cuvette untersucht wird. Es wurden keine Blinkzeichen der Lampen festgestellt. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",												// German
											"Siempre se comprueba la l�mpara para asegurar que sigue emitiendo flashes cuando se pone la muestra en la cubeta �ptica. Los detectores no registraron la emisi�n de flashes de la l�mpara. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",		// Spanish
											"La lampada viene controllata ogni volta per assicurarsi che stia ancora lampeggiando quando viene campionata la cuvette ottica. I rilevatori non hanno riconosciuto i flash luminosi. Se i sintomi continuano contattare il servizio tecnico.\n",											// Italian
											"A l�mpada � sempre verificada para garantir que emite flashes quando a amostra � colocada na cuvete �tica. Os detetores n�o registram a emiss�o de flashes da l�mpada. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",												// Portuguese
											"A l�mpada � verificada a cada vez, para certificar-se de que ainda esteja piscando quando a cubeta �ptica for amostrada. Os detectores n�o reconheceram a luz da l�mpada. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"												// Portuguese (Brazil)
										} 
	},
	{ EISTR_LIGHT_NOISE				,	{	"There was high variability in the signal. This may impact the accuracy of the analyzer's measurement of absorbance. If symptom continues, contact Technical Service.\n",											// English
											"Le signal d'arri�re-plan a pr�sent� une variabilit� �lev�e. ce qui peut affecter la pr�cision de la mesure d'absorbance par l'analyseur. En cas de persistance des sympt�mes, contacter le service technique.\n",	// French
											"Es bestand eine hohe Signalvarianz. Dies kann die Genauigkeit der Absorbierungsmessung des Analyseger�ts beeintr�chtigen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",				// German
											"Hab�a una gran variabilidad en la se�al. Esto puede afectar a la precisi�n de la medida de absorbancia del analizador. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"Grande variabilit� nel segnale. Esso potrebbe influire sulla precisione della misurazione di assorbenza dell'analizzatore. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"Foi observada uma grande variabilidade no sinal. Isto pode afetar a precis�o da medi��o da absorv�ncia do analisador. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",						// Portuguese
											"Houve alta variabilidade no sinal. Isso pode afetar a precis�o da medi��o de absorb�ncia do analisador. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"											// Portuguese (Brazil)
										} 
	},
	{ EISTR_MISSING_FILTER_CURV		,	{	"The filter curve calibration factors have not been loaded into the analyzer. Contact Technical Service for assistance.\n",									// English
											"Les facteurs de calibrage de la courbe du filtre n'ont pas �t� charg�s sur l'analyseur. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Die Kalibrierungsfaktoren der Filterkurve wurden nicht in das Analyseger�t geladen. Wenden Sie sich an den technischen Service.\n",						// German
											"Los factores de calibraci�n de la curva del filtro no se han cargado en el analizador. P�ngase en contacto con el servicio t�cnico.\n",					// Spanish
											"I fattori di calibrazione della curva del filtro non sono stati caricati nell'analizzatore. Per assistenza contattare il servizio tecnico.\n",				// Italian
											"Os fatores da curva de calibra��o do filtro n�o foram carregados no analisador. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",		// Portuguese
											"Os fatores de calibra��o da curva do filtro n�o foram carregados no analisador. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_MOTOR_CONTROLLER		,	{	"The analyzer detected an error with the motor controller. If symptom continues, contact Technical Service.\n",												// English
											"L'analyseur a d�tect� une erreur sur le contr�leur du moteur. En cas de persistance des sympt�mes, contacter le service technique.\n",						// French
											"Das Analyseger�t hat einen Fehler an der Motorsteuerung festgestellt. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",			// German
											"El analizador detect� un error con el controlador del motor. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"L'analizzatore ha individuato un errore con il controllore a motore. Se i sintomi continuano contattare il servizio tecnico.\n",							// Italian
											"O analisador detetou um erro no controlador do motor. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",								// Portuguese
											"O analisador detectou um erro no controlador do motor. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_NEEDS_SERVICE			,	{	"The analyzer may need servicing. Contact Technical Service for further details.\n",													// English
											"Il se peut que l'analyseur n�cessite une intervention technique. Pour obtenir plus d'informations, contacter le service technique.\n",	// French
											"Das Analyseger�t mu� m�glicherweise gewartet werden. Wenden Sie sich an den technischen Service.\n",									// German
											"Puede que el analizador deba ser reparado. P�ngase en contacto con el servicio t�cnico.\n",											// Spanish
											"L'analizzatore potrebbe aver bisogno di manutenzione. Per ulteriore dettagli contattare il servizio tecnico.\n",						// Italian
											"O analisador pode necessitar de assist�ncia. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",				// Portuguese
											"O analisador pode precisar de manuten��o. Entre em contato com o Servi�o T�cnico para obter detalhes adicionais.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_NO_FILES_TO_ARCHIVE		,	{	"There are no files within the date range specified to archive.\n",					// English
											"Aucun fichier � archiver dans la plage de dates sp�cifi�e.\n",						// French
											"F�r den spezifizierten Datumsbereich sind keine Dateien zu archivieren.\n",		// German
											"No hay ning�n archivo en el intervalo de fechas especificado para archivar.\n",	// Spanish
											"Non sono presenti file da archiviare nel range di date specificato.\n",			// Italian
											"N�o existem ficheiros no intervalo de datas especificado para arquivar. \n",		// Portuguese
											"N�o h� arquivos dentro do intervalo de datas especificado para arquivar. \n"		// Portuguese (Brazil)
										}	
	},

	{ EISTR_NO_FILES_TO_TRANSMIT	,	{	"There are no files within the date range specified to transmit.\n",				// English
											"Aucun fichier � transmettre dans la plage de dates sp�cifi�e.\n",					// French
											"F�r den spezifizierten Datumsbereich sind keine Dateien zu �bertragen.\n",			// German
											"No hay ning�n archivo en el intervalo de fechas especificado para transmitir.\n",	// Spanish
											"Non sono presenti file da trasmettere nel range di date specificato.\n",			// Italian
											"N�o existem ficheiros no intervalo de datas especificado para transmitir.\n",		// Portuguese
											"N�o h� arquivos dentro do intervalo de datas especificado para transmitir.\n"		// Portuguese (Brazil)
										}	
	},

	{ EISTR_NO_RESULTS_TO_TRANSMIT	,	{	"There are no results within the date range specified to transmit.\n",				// English
											"Aucun r�sultat � transmettre dans la plage de dates sp�cifi�e.\n",					// French
											"F�r den spezifizierten Datumsbereich sind keine Ergebnisse zu �bertragen.\n",		// German
											"No hay ning�n resultado en el intervalo de fechas especificado para transmitir.\n",// Spanish
											"Non sono presenti risultati da trasmettere nel range di date specificato.\n",		// Italian
											"N�o existem resultados no intervalo de datas especificado para transmitir.\n",		// Portuguese
											"N�o h� resultados dentro do intervalo de datas especificado para transmitir.\n"	// Portuguese (Brazil)
										}	
	},

	{ EISTR_ORDER_WAS_CANCELED		,	{	"The selected order was canceled by the external order entry system.\n",						// English
											"La commande s�lectionn�e a �t� annul�e par le syst�me d'entr�e de commande externe.\n",		// French
											"Die ausgew�hlte Bestellung wurde vom externen Bestelleingabesystem storniert.\n",				// German
											"El pedido seleccionado ha sido cancelado por el sistema externo de entrada de pedidos.\n",		// Spanish
											"Ordine selezionato cancellato dal sistema di immissione ordini esterno.\n",					// Italian
											"O pedido selecionado foi cancelado pelo sistema de entrada de pedidos externo.\n",				// Portuguese
											"A solicita��o selecionada foi cancelada pelo sistema externo de entrada de solicita��es.\n"	// Portuguese (Brazil)
										}
	},

	{ EISTR_OP_ID_LIST_FULL			,	{	"All the available space for Operator ID's is used.  Please remove at least one Operator ID before adding a new Operator ID.\n",													// English
											"Tout l'espace disponible r�serv� aux ID op�rateur est utilis�. Veuillez supprimer au moins un ID op�rateur avant d'en ajouter un nouveau.\n",										// French
											"Der gesamte Speicherplatz f�r Bediener-IDs ist belegt.  Bitte entfernen Sie mindestens eine Bediener-ID, bevor Sie eine neue hinzuf�gen.\n",										// German
											"Se ha utilizado todo el espacio disponible para n�meros de identificaci�n de operarios. Elimine al menos un n�mero de identificaci�n de operario antes de a�adir otro nuevo.\n",	// Spanish
											"� stato utilizzato tutto lo spazio disponibile per l'ID operatore. Rimuovere almeno un ID operatore prima di aggiungere un nuovo ID operatore.\n",									// Italian
											"Foi utilizado todo o espa�o dispon�vel para IDs de operadores.  Elimine pelo menos uma ID de operador antes de adicionar uma nova ID de operador.\n",								// Portuguese
											"Todo o espa�o dispon�vel para os IDs de operador est� usado.  Remova pelo menos um ID de operador antes de adicionar um novo ID de operador.\n"									// Portuguese (Brazil)
										}
	},

	{ EISTR_OP_RENEWAL_REQUIRED		,	{	"The date set authorizing the operator to use the analyzer has passed.  Please contact technical support for more information.\n",										// English
											"La date pr�vue pour autorisation de l'op�rateur � utiliser l'analyseur est d�pass�e. Pour plus d'informations, contacter l'assistance technique.\n",					// French
											"Das angegebene Datum, bis zu dem der Bediener das Analyseger�t benutzen kann, ist abgelaufen. Wenden Sie sich an den technischen Service f�r weitere Information.\n",	// German
											"La fecha para autorizar al operario a usar el analizador ya ha pasado. P�ngase en contacto con el servicio t�cnico.\n",												// Spanish
											"La data impostata per l'analisi dei campioni di controllo � stata superata. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",						// Italian
											"A data definida para autorizar o operador a utilizar o analisador foi excedida.  Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",			// Portuguese
											"A data configurada, autorizando o operador a usar o analisador, j� passou.  Entre em contato com o suporte t�cnico para obter mais informa��es.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_PATIENT_IDS_DONT_MATCH	,	{	"The patient ID entered does not match the ID transmitted to the analyzer.\n",							// English
											"L'ID patient entr�e ne correspond pas � l'ID transmise � l'analyseur.\n",								// French
											"Die eingegebene Patienten-ID stimmt nicht mit der an das Analyseger�t �bermittelten ID �berein.\n",	// German
											"El Id. del paciente introducido no coincide con el Id. transmitido al analizador.\n",					// Spanish
											"L'ID paziente inserito non corrisponde a quello trasmesso all'analizzatore.\n",						// Italian
											"A ID de doente introduzida n�o coincide com a ID transmitida ao analisador.\n",						// Portuguese
											"O ID do paciente inserido n�o corresponde ao ID transmitido para o analisador.\n"						// Portuguese (Brazil)
										}
	},
	{ EISTR_POWER_SUPPLY			,	{	"The analyzer has detected power fluctuations that prevent the unit from operating correctly. If symptom continues, contact Technical Service.\n",																// English
											"L'analyseur a d�tect� des fluctuations dans l'alimentation qui emp�chent l'appareil de fonctionner correctement. En cas de persistance des sympt�mes, contacter le service technique.\n",						// French
											"Das Analyseger�t hat Energieschwankungen festgestellt, die den ordnungsgem��en Betrieb der Einheit beeintr�chtigen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",				// German
											"El analizador ha detectado fluctuaciones de la corriente que no permiten el funcionamiento correcto de la unidad. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"L'analizzatore ha individuato fluttuazioni di corrente che non permettono di funzionare correttamente. Se i sintomi continuano contattare il servizio tecnico.\n",												// Italian
											"O analisador detetou flutua��es de corrente que n�o permitem o correto funcionamento da unidade. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",										// Portuguese
											"O analisador detectou flutua��es de energia que impedem a unidade de funcionar corretamente. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"												// Portuguese (Brazil)
										} 
	},
	{ EISTR_PRINTER_NOT_FOUND		,	{	"The external printer is not available. Check that the printer is connected to the analyzer, is turned on and has paper. If symptom continues, contact Technical Service.\n",																	// English
											"L'imprimante externe n'est pas disponible. V�rifier que l'imprimante est bien raccord�e � l'analyseur, qu'elle fonctionne et qu'elle dispose d'une r�serve de papier. En cas de persistance des sympt�mes, contacter le service technique.\n",	// French
											"Der externe Drucker ist nicht verf�gbar. Pr�fen Sie die Verbindung zwischen Drucker und Analyseger�t und ob der Drucker eingeschaltet ist und �ber Papier verf�gt. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",	// German
											"La impresora externa no est� disponible. Compruebe que la impresora est� conectada al analizador, que est� encendida y que tiene papel. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.n",				// Spanish
											"La stampante esterna non � disponibile. Verificare che la stampante sia collegata all'analizzatore, che sia accesa e che non manchi carta. Se i sintomi continuano contattare il servizio tecnico.\n",											// Italian
											"A impressora externa n�o est� dispon�vel. Verifique que se a impressora est� ligada ao analisador, que est� ligada e que tem papel. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",									// Portuguese
											"A impressora externa n�o est� dispon�vel. Verifique se a impressora est� conectada ao analisador, se est� ligada e se tem papel. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"											// Portuguese (Brazil)
										} 
	},
	{ EISTR_PROCESS_INIT			,	{	"The analyzer is unable to start a process required to operate. Contact Technical Service for assistance.\n",									// English
											"L'analyseur ne parvient pas � lancer la proc�dure requise pour fonctionner. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Das Analyseger�t kann einen f�r den Betrieb erforderlichen Vorgang nicht starten. Wenden Sie sich an den technischen Service.\n",				// German
											"El analizador no puede iniciar un proceso necesario para funcionar. P�ngase en contacto con el servicio t�cnico.\n",							// Spanish
											"L'analizzatore non � in grado di iniziare una procedura necessaria al funzionamento. Per assistenza contattare il servizio tecnico.\n",		// Italian
											"O analisador n�o consegue iniciar um processo necess�rio para funcionar. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											"O analisador n�o � capaz de iniciar um processo necess�rio para operar. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_PRODUCT_CODE			,	{	"The product codes in the analyzer do not match. Contact Technical Service for more details.\n",									// English
											"Les codes produits dans l'analyseur ne correspondent pas. Pour plus d'informations, contacter le service technique.\n",			// French
											"Die Produktcodes in dem Analyseger�t passen nicht zusammen. Wenden Sie sich an den technischen Service.\n",						// German
											"Los c�digos de producto en el analizador no se corresponden. P�ngase en contacto con el servicio t�cnico para m�s informaci�n.\n",	// Spanish
											"I codici prodotto nell'analizzatore non combaciano. Per ulteriore dettagli contattare il servizio tecnico.\n",						// Italian
											"Os c�digos de produto no analisador n�o coincidem. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",		// Portuguese
											"Os c�digos de produto no analisador n�o correspondem. Entre em contato com o Servi�o T�cnico para obter mais detalhes.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_REAGENT_DISTRIBUTION	,	{	"Reagent beads have not dissolved as expected. Rerun sample with a new disc.\n",											// English
											"Les billes de r�actif ne se sont pas dissoutes comme pr�vu. R�ex�cuter l'�chantillon � l'aide d'un nouveau disque.\n",		// French
											"Reagenz-Beads wurden nicht erwartungsgem�� aufgel�st. Wiederholen Sie die Probe mit einer neuen Scheibe.\n",				// German
											"Las cuentas de reactivo no se han disuelto como se esperaba. Vuelva a utilizar la muestra con un nuevo disco.\n",			// Spanish
											"I granuli di reagente non si sono dissolti come previsto. Riavviare il campione con un nuovo disco.\n",					// Italian
											"As esferas de reagente n�o se dissolveram conforme esperado. Executar novamente a amostra utilizando um disco novo.\n",	// Portuguese
											"As esferas de reagente n�o se dissolveram conforme esperado. Execute novamente a amostra com um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_RESULT_CORRUPTION		,	{	"The saved results are corrupt.  All the previous results have been lost.  A new set of saved results has been started.  If symptom continues, contact Technical Service.\n"																// English
											"Les r�sultats enregistr�s sont corrompus.  Tous les r�sultats ant�rieurs ont �t� perdus.  Un nouvel ensemble de r�sultats enregistr�s a �t� initi�.  Si les sympt�mes persistent, contacter le service technique.\n",						// French
											"Die gespeicherten Ergebnisse sind fehlerhaft. Alle bisherigen Ergebnisse sind verloren gegangen. Es wurde ein neuer Satz gespeicherter Ergebnisse gestartet. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",	// German
											"Los resultados guardados est�n da�ados.  Se han perdido todos los resultados anteriores.  Se ha iniciado una nueva serie de resultados guardados.  Si los s�ntomas contin�an, comun�quese con el servicio t�cnico.\n",						// Spanish
											"I risultati salvati sono danneggiati.  Tutti i risultati precedenti sono andati perduti.  � stato iniziato un nuovo insieme di risultati.  Se il sintomo persiste, contattare il servizio tecnico.\n",										// Italian
											"Os resultados guardados est�o danificados.  Todos os resultados anteriores foram perdidos.  Foi iniciado um novo conjunto de resultados guardados.  Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",				// Portuguese
											"Os resultados salvos est�o corrompidos.  Todos os resultados anteriores foram perdidos.  Um novo conjunto de resultados salvos foi iniciado.  Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"							// Portuguese (Brazil)
										}
	},
	{ EISTR_ROTOR_DETECTED			,	{	"A disc may be in the analyzer and may prevent the analyzer from starting normally. Press Continue to remove the disc.\n",										// English
											"Il peut y avoir dans l'analyseur un disque susceptible de l'emp�cher de d�marrer normalement. Appuyer sur Continuer pour retirer le disque.\n",				// French
											"Eine Scheibe kann sich in dem Analyseger�t befinden und den Startvorgang des Analyseger�ts beeintr�chtigen. Zur Entfernung der Scheibe dr�cken Sie Weiter.\n",	// German
											"Puede que haya un disco en el analizador, lo que impide que el analizador se inicie correctamente. Pulse en Continuar para retirar el disco.\n",				// Spanish
											"Nell'analizzatore potrebbe essere rimasto un disco che impedisce un'attivazione normale dell'analizzatore. Premere Continua per rimuovere il disco.\n",		// Italian
											"Pode estar um disco no analisador, o que pode impedir que o analisador inicie corretamente. Prima Continuar para remover o disco.\n",							// Portuguese
											"Um disco pode estar no analisador e impedir que o analisador inicie normalmente. Pressione Continuar para remover o disco.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_RQC						,	{	"The Reagent QC beads recovered below the limit. The disc may not have been stored correctly. Rerun with a new disc.\n",												// English
											"La r�cup�ration des billes de r�actif QC est inf�rieure � la limite. Il se peut que le disque n'ait pas �t� rang� correctement. R�ex�cuter avec un nouveau disque.\n",	// French
											"Die Reagenz-QC-Beads liegen unter dem Grenzwert. Die Scheibe wurde m�glicherweise nicht ordnungsgem�� gelagert. Mit einer neuen Scheibe wiederholen.\n",				// German
											"Cuentas QC de reactivo recuperadas por debajo del l�mite. Puede que el disco no se haya guardado correctamente. Debe usarse con un nuevo disco.\n",					// Spanish
											"I granuli QC di reagente si sono ristabiliti sotto al limite. Il disco potrebbe non essere stato conservato correttamente. Riavviare con un nuovo disco.\n",			// Italian
											"As esferas de reagente de CQ foram recuperadas abaixo do limite. O disco pode n�o ter sido guardado corretamente. Executar novamente utilizando um disco novo.\n",		// Portuguese
											"As esferas de Reagente de QC se recuperaram abaixo do limite. O disco pode n�o ter sido armazenado corretamente. Execute novamente com um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_BLANK			,	{	"The sample blank bead appears to be turbid. This can be due to sample quality. Confirm sample integrity and rerun with a new disc.\n",															// English
											"La bille vierge de l'�chantillon semble trouble, ce qui peut venir de la qualit� de l'�chantillon. V�rifier l'int�grit� de l'�chantillon et r�ex�cuter avec un nouveau disque.\n",				// French
											"Die Leer-Beadprobe ist tr�be. Dies kann auf die Probenqualit�t zur�ckgef�hrt werden. Best�tigen Sie die Unversehrtheit der Probe und wiederholen Sie den Vorgang mit einer neuen Scheibe.\n",	// German
											"La cuenta virgen de la muestra parece estar turbia. Esto puede deberse a la calidad de la muestra. Confirme la integridad de la muestra y vuelva a utilizar el nuevo disco.\n",				// Spanish
											"I granuli campione vuoti sembrano torbidi. Ci� pu� essere causato dalla qualit� del campione. Confermare l'integrit� del campione e riavviare con un nuovo disco.\n",							// Italian
											"A esfera de amostra de branco parece estar turva. Isto pode ser devido � qualidade da amostra. Confirme a integridade da amostra e execute novamente utilizando um disco novo.\n",				// Portuguese
											"A esfera em branco da amostra parece estar turva. Isso pode ser decorrente da qualidade da amostra. Confirme a integridade da amostra e execute novamente com um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_DISTRIBUTION		,	{	"Sample has not reached the chemistries. Rerun with a new disc.\n",										// English
											"L'�chantillon n'est pas parvenu jusqu'aux produits chimiques. R�ex�cuter avec un nouveau disque.\n",	// French
											"Die Probe hat nicht die Chemie erreicht. Mit einer neuen Scheibe wiederholen.\n",						// German
											"La muestra no ha alcanzado los compuestos qu�micos. Debe usarse con un nuevo disco.\n",				// Spanish
											"Il campione non ha raggiunto le sostanze chimiche. Riavviare con un nuovo disco.\n",					// Italian
											"A amostra n�o atingiu as subst�ncias qu�micas. Executar novamente utilizando um disco novo.\n",		// Portuguese
											"A amostra n�o atingiu as subst�ncias qu�micas. Execute novamente com um novo disco.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_MIX				,	{	"The sample has not mixed with reagent materials sufficiently. Rerun with a new disc.\n",									// English
											"L'�chantillon ne s'est pas suffisamment m�lang� avec les mati�res de r�actifs. R�ex�cuter avec un nouveau disque.\n",		// French
											"Die Probe hat sich nicht ausreichend mit Reagenzmaterialien vermischt. Mit einer neuen Scheibe wiederholen.\n",			// German
											"La muestra no se ha mezclado suficientemente con los materiales reactivos. Debe usarse con un nuevo disco.\n",				// Spanish
											"Il campione non si � mescolato a sufficienza con le sostanze reagenti. Riavviare con un nuovo disco.\n",					// Italian
											"A amostra n�o se misturou suficientemente com os materiais dos reagentes. Executar novamente utilizando um disco novo.\n",	// Portuguese
											"A amostra n�o foi misturada com os materiais reagentes suficientemente. Execute novamente com um novo disco.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_QUALITY			,	{	"The sample may have one or more physical interferents. Confirm the sample integrity and rerun disc.\n",											// English
											"Il se peut que l'�chantillon pr�sente un ou plusieurs �l�ments interf�rents. V�rifier l'int�grit� de l'�chantillon et r�ex�cuter le disque.\n",	// French
											"Die Probe hat m�glicherweise eine oder mehrere physische st�rende Bestandteile. Unversehrtheit der Probe best�tigen und Scheibe neu starten.\n",	// German
											"Puede que la muestra tenga una o varias interferencias f�sicas. Confirme la integridad de la muestra y vuelva a usar el disco.\n",					// Spanish
											"Il campione potrebbe avere uno o pi� fattori di interferenza fisica. Confermare l'integrit� del campione e riavviare il disco.\n",					// Italian
											"A amostra pode ter uma ou v�rias interfer�ncias f�sicas. Confirme a integridade da amostra e execute novamente o disco.\n",						// Portuguese
											"A amostra pode ter um ou mais interferentes f�sicos. Confirme a integridade da amostra e execute novamente o disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_SATURATION				,	{	"The photometer in the analyzer has detected a brighter than normal flash. If symptom continues, contact Technical Service.\n",												// English
											"Le photom�tre de l'analyseur a d�tect� un clignotement plus lumineux que la normale. En cas de persistance des sympt�mes, contacter le service technique.\n",				// French
											"Das Fotometer in dem Analyseger�t hat ein helleres Blinkzeichen als normal festgestellt. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",		// German
											"El fot�metro del analizador ha detectado un flash m�s brillante de lo normal. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"Il fotometro nell'analizzatore ha individuato un flash pi� luminoso del normale. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"O fot�metro no analisador detetou um flash mais brilhante do que o normal. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",							// Portuguese
											"O fot�metro no analisador detectou um flash mais brilhante que o normal. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_SOFTWARE_UPDATE			,	{	"The disc inserted requires a software update to operate correctly. Contact Technical Service for further assistance.\n",														// English
											"Le disque ins�r� n�cessite une mise � jour de logiciel pour pouvoir fonctionner correctement. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											"Die eingesetzte Scheibe erfordert ein Software-Update f�r einen ordnungsgem��en Betrieb. Wenden Sie sich an den technischen Service.\n",										// German
											"El disco insertado necesita la actualizaci�n del software para operar correctamente. P�ngase en contacto con el servicio t�cnico.\n",											// Spanish
											"Il disco inserito richiede un aggiornamento del software per funzionare correttamente. Per ulteriore assistenza contattare il servizio tecnico.\n",							// Italian
											"O disco inserido necessita de uma atualiza��o do software para funcionar adequadamente. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",					// Portuguese
											"O disco inserido requer uma atualiza��o de software para funcionar corretamente. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SPINDLE_MOTOR_STUCK		,	{	"Spindle motor not turning properly. Contact Technical Service for further assistance.\n",											// English
											"Le moteur de l'axe ne tourne pas correctement. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",		// French
											"Der Spindelmotor dreht sich nicht ordnungsgem��. Wenden Sie sich an den technischen Service.\n",									// German
											"El motor del eje no est� girando correctamente. P�ngase en contacto con el servicio t�cnico.\n",									// Spanish
											"L'asse motore non gira correttamente. Per ulteriore assistenza contattare il servizio tecnico\n",									// Italian
											"O motor de eixo n�o est� a rodar adequadamente. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",				// Portuguese
											"O motor do fuso n�o est� girando corretamente. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_STARTING_ABSORBANCE		,	{	"Starting absorbance of the sample blank bead is outside the limit, possibly due to an exogenous substance or distribution error. Rerun with new disc.\n",																		// English
											"L'absorbance de d�part de la bille vierge de l'�chantillon n'entre pas dans les limites prescrites, �ventuellement du fait d'une substance exog�ne ou d'une erreur de distribution. R�ex�cuter � l'aide du nouveau disque.\n",	// French
											"Die Startabsorbierung der Leer-Beadprobe befindet sich m�glicherweise aufgrund einer exogenen Substanz oder eines Verteilungsfehlers au�erhalb des Grenzwerts. Mit einer neuen Scheibe wiederholen.\n",						// German
											"La absorbancia de inicio de la cuenta virgen de muestra est� fuera del l�mite, probablemente debido a una sustancia ex�gena o a un error de distribuci�n. Utilice un nuevo disco.\n",											// Spanish
											"L'assorbenza iniziale dei granuli campione vuoti non rientra nei limiti, forse a causa di una sostanza esogena o a un errore di distribuzione. Riavviare con un nuovo disco.\n",												// Italian
											"A absorv�ncia inicial da esfera de amostra de branco est� fora do limite, possivelmente devido a uma sust�ncia ex�gena ou erro de distribui��o. Executar novamente utilizando um disco novo.\n",								// Portuguese
											"A absor��o inicial da esfera em branco da amostra est� fora do limite, possivelmente devido a uma subst�ncia ex�gena ou um erro de distribui��o. Execute novamente com um novo disco.\n"										// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_SENSOR				,	{	"The disc temperature sensor is not operating. Contact Technical Service for further assistance.\n",										// English
											"Le d�tecteur de temp�rature du disque ne fonctionne pas. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											"Der Temperatursensor der Scheibe ist au�er Betrieb. Wenden Sie sich an den technischen Service.\n",										// German
											"El sensor de temperatura del disco no est� funcionando. P�ngase en contacto con el servicio t�cnico.\n",									// Spanish
											"Il sensore di temperatura del disco non � in funzione. Per ulteriore assistenza contattare il servizio tecnico.\n",						// Italian
											"O sensor de temperatura do disco n�o est� a funcionar. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",				// Portuguese
											"O sensor de temperatura do disco n�o est� funcionando. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_VARIATION			,	{	"The temperature deviated more than one degree during the run. The accuracy of certain chemistries may be affected. Rerun with new disc.\n",													// English
											"La temp�rature a pr�sent� un �cart de plus d'un degr� au cours de l'ex�cution. La pr�cision de certains produits chimiques pourra en �tre affect�e. R�ex�cuter � l'aide du nouveau disque.\n",	// French
											"Der Temperatur wich um mehr als ein Grad w�hrend des Betriebs ab. Die Genauigkeit bestimmter Chemie kann beeintr�chtigt worden sein. Mit einer neuen Scheibe wiederholen.\n",					// German
											"La temperatura cambi� en m�s de un grado durante el an�lisis. Puede que afecte a la precisi�n de an�lisis de ciertas sustancias qu�micas. Utilice un nuevo disco.\n",							// Spanish
											"Durante il funzionamento la temperatura � variata di pi� di un grado. La precisione di alcune sostanze chimiche potrebbe esserne stata influenzata. Riavviare con un nuovo disco.\n",			// Italian
											"A altera��o da temperatura foi superior a um grau durante a execu��o. A precis�o de determinadas sust�ncias qu�micas pode ser afetada. Executar novamente utilizando um disco novo.\n",		// Portuguese
											"A temperatura desviou mais de um grau durante a execu��o. A precis�o de certas subst�ncias qu�micas pode estar afetada. Execute novamente com um novo disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMPERATURE				,	{	"The temperature of the disc either varied too much or was not in the proper range during the run. If symptom continues, contact Technical Service.\n",																			// English
											"La temp�rature du disque a soit vari� dans des proportions trop importantes, soit ne s'inscrit pas dans la fourchette prescrite en cours d'ex�cution. En cas de persistance des sympt�mes, contacter le service technique.\n",	// French
											"Die Scheibentemperatur variierte zu stark oder befand sich w�hrend des Betriebs au�erhalb des optimalen Bereichs. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",									// German
											"La temperatura del disco vari� mucho o no estaba en el intervalo adecuado durante el an�lisis. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",										// Spanish
											"Durante il funzionamento la temperatura del disco ha subito variazioni eccessive oppure non era nel range adatto. Se i sintomi continuano contattare il servizio tecnico.\n",													// Italian
											"A temperatura do disco variou muito ou n�o esteve dentro do intervalo adequado durante a an�lise. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",														// Portuguese
											"A temperatura do disco variou muito ou n�o permaneceu no intervalo adequado durante a execu��o. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"																// Portuguese (Brazil)
										} 
	},
	{ EISTR_TIMEOUT					,	{	"The analyzer is not receiving required data in a timely fashion. If symptom continues, contact Technical Service.\n",										// English
											"L'analyseur ne re�oit pas les donn�es voulues dans les d�lais. En cas de persistance des sympt�mes, contacter le service technique.\n",					// French
											"Das Analyseger�t empf�ngt die erforderlichen Daten nicht rechtzeitig. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",			// German
											"El analizador no est� recibiendo a tiempo los datos necesarios. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"L'analizzatore non riceve tempestivamente i dati necessari. Se i sintomi continuano contattare il servizio tecnico.\n",									// Italian
											"O analisador n�o est� a receber atempadamente os dados necess�rios. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",				// Portuguese
											"O analisador n�o est� recebendo os dados necess�rios em tempo h�bil. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_TOO_HOT					,	{	"The analyzer is getting too hot and should be allowed to cool down for a while before further use.\n",						// English
											"L'analyseur pr�sente une surchauffe. Il convient de le laisser refroidir un moment avant de continuer � l'utiliser.\n",	// French
											"Das Analyseger�t wird zu hei� und sollte vor der weiteren Verwendung abk�hlen.\n",											// German
											"El analizador se est� sobrecalentando y se deber�a dejar enfriar antes de volver a usarlo.\n",								// Spanish
											"L'analizzatore si sta riscaldando eccessivamente e deve essere fatto raffreddare prima di essere usato nuovamente.\n",		// Italian
											"O analisador est� aquecer muito e deve-se deixar arrefecer durante algum tempo antes de o voltar a utilizar.\n",			// Portuguese
											"O analisador est� se tornando quente demais e deve ser reservado para esfriar por um tempo antes de continuar o uso.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_TOP_HEATER_HIGH			,	{	"The top heater plate draws too much current when turned on at full power. Contact Technical Service for assistance.\n",														// English
											"La plaque chauffante sup�rieure consomme trop d'�lectricit� lorsqu'elle fonctionne � fond. Pour obtenir une assistance, contacter le service technique.\n",					// French
											"Die obere Heizplatte verbraucht bei Maximalbetrieb zu viel Strom. Wenden Sie sich an den technischen Service.\n",																// German
											"La placa del calentador superior utiliza mucha corriente cuando se enciende a toda potencia. P�ngase en contacto con el servicio t�cnico.\n",									// Spanish
											"Il disco di riscaldamento superiore prende troppa corrente quando � acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",							// Italian
											"A placa do aquecedor superior utiliza elevada quantidade de corrente quando � ligada na pot�ncia m�xima. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											"A placa superior do aquecedor consome muita corrente quando ligada na pot�ncia m�xima. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_TOP_HEATER_LOW			,	{	"The top heater plate did not draw enough current when turned on at full power. Contact Technical Service for assistance.\n",															// English
											"La plaque chauffante sup�rieure n'a pas consomm� suffisamment d'�lectricit� lorsqu'elle a fonctionn� � fond. Pour obtenir une assistance, contacter le service technique.\n",			// French
											"Die obere Heizplatte verbrauchte bei Maximalbetrieb zu wenig Strom. Wenden Sie sich an den technischen Service.\n",																	// German
											"La placa del calentador superior no utiliz� suficiente corriente cuando se puso a toda potencia. P�ngase en contacto con el servicio t�cnico.\n",										// Spanish
											"Il disco di riscaldamento superiore non ha preso sufficiente corrente quando acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",							// Italian
											"A placa superior do aquecedor n�o utiliza quantidade suficiente de corrente quando ligada na pot�ncia m�xima. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											"A placa superior do aquecedor n�o atraiu corrente suficiente quando ligada na pot�ncia m�xima. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_UNAUTHORIZED_ADMIN		,	{	"The ID entered for the administrator is not in the system. Please contact technical support for more information.\n",						// English
											"L'ID saisi pour l'administrateur n'est pas sur le syst�me. Pour plus d'informations, contacter l'assistance technique.\n",					// French
											"Die eingegebene Administrator-ID existiert nicht im System. Wenden Sie sich bitte an den technischen Service f�r weitere Information.\n",	// German
											"Esta Id. administrador no est� en el sistema. P�ngase en contacto con el servicio t�cnico. \n",											// Spanish
											"L'ID immesso per l'amministratore non � presente nel sistema. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",		// Italian
											"A ID introduzida para o administrador n�o est� no sistema. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",		// Portuguese
											"O ID inserido para o administrador n�o est� no sistema. Entre em contato com o suporte t�cnico para obter mais informa��es.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_UNAUTHORIZED_OP			,	{	"The ID entered for the operator is not in the system. Please contact technical support for more information.\n",						// English
											"L'ID saisi pour l'op�rateur n'est pas sur le syst�me. Pour plus d'informations, contacter l'assistance technique.\n",					// French
											"Die eingegebene Bediener-ID existiert nicht im System. Wenden Sie sich bitte an den technischen Service f�r weitere Information.\n",	// German
											"Esta Id. operario no est� en el sistema. P�ngase en contacto con el servicio t�cnico.\n",												// Spanish
											"L'ID immesso per l'operatore non � presente nel sistema. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",		// Italian
											"A ID introduzida para o operador n�o est� no sistema. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",		// Portuguese
											"O ID inserido para o operador n�o est� no sistema. Entre em contato com o suporte t�cnico para obter mais informa��es.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_WRONG_ROTOR				,	{	"The disc inserted is not intended for this device. If symptom continues, contact Technical Service.\n",										// English
											"Le disque ins�r� n'est pas pr�vu pour cet appareil. En cas de persistance des sympt�mes, contacter le service technique.\n",					// French
											"Die eingesetzte Scheibe ist f�r dieses Ger�t nicht geeignet. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",		// German
											"El disco insertado no es apto para este aparato. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"Il disco inserito non � adatto a questo strumento. Se i sintomi continuano contattare il servizio tecnico.\n",									// Italian
											"O disco inserido n�o � adequado para este dispositivo. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",					// Portuguese
											"O disco inserido n�o � destinado a este dispositivo. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"						// Portuguese (Brazil)
										} 
	},


// Printer									ENGLISH,						FRENCH,							GERMAN,							SPANISH,							ITALIAN, 						PORTUGUESE,							PORTUGUESE (BRAZIL)
	{ PSTR_PRINTER_TEST				,	{	"Printer Test",					"Test imprimante",				"Druckertest",					"Prueba de impresora",				"Prova stampante",				"Teste da impressora",				"Teste da impressora"	}	},

// Results (display and print)				ENGLISH,						FRENCH,							GERMAN,							SPANISH,							ITALIAN, 						PORTUGUESE,							PORTUGUESE (BRAZIL)
	{ RSTR_ACCEPTABLE_MINIMUM		,	{	"Acceptable Minimum:",			"Minimum Acceptable :",			"Verwertbares min.:",			"Minimo Aceptable:",				"Minimo Accettabile:",			"M�nimo aceit�vel:",				"M�nimo aceit�vel:"	}	},
	{ RSTR_AGE						,	{	"Age:",							"�ge :",						"Alter",						"Edad:",							"Et�:",							"Idade:",							"Idade:"	}	},
	{ RSTR_CHEMISTRY_QC				,	{	"Chemistry QC:",				"Chimie CQ :",					"Chemie-QK:",					"CC Quimico:",						"QC Chimica:",					"CQ Qu�mico:",						"QC de qu�mica:"	}	},
	{ RSTR_CONFIRM_LOW_RECOVERIES	,	{	"! CONFIRM LOW RECOVERIES !",	"! CONFIRMER FAIBLE R�CUP !",	"! NDR. WIEDERHERST. BEST�T.!",	"! CONFIRME BAJA RECUPERACI�N !",	"! CONFERMA POCHI RECUPERI !",	"! CONFIRME BAIXA RECUPERA��O !",	"! CONFIRM. RECUPER. BAIXAS !"	}	},
	{ RSTR_CONTROL_ID				,	{	"Control ID:",					"ID contr.:",					"Kontr.-ID",					"Id. ctrol:",						"ID contrl:",					"ID do controlo:",					"ID de controle:"	}	},
	{ RSTR_CONTROL_RENEWAL			,	{	"Control Renewal Required",		"Renouv. contr. n�cess.",		"Kontrollerneuerung ben�tigt",	"Renovar control",					"Necess. rinnovo contr.",		"Necess�rio renovar o controlo",	"Renov. do controle necess�ria"	}	},
	{ RSTR_DEMONSTRATION_SOFTWARE	,	{	"Demonstration Software",		"Logiciel de d�monstration",	"Demonstrationssoftware",		"Demo",								"Software di dimostrazione",	"Software de demonstra��o",			"Software de demonstra��o"	}	},
	{ RSTR_ERROR					,	{	"ERROR",						"ERREUR",						"FEHLER",						"ERROR",							"ERRORE",						"ERRO",								"ERRO"	}	},
	{ RSTR_GENDER					,	{	"Gender:",						"Sexe :",						"Geschlecht:",					"Sexo:",							"Sesso:",						"Sexo:",							"G�nero:"	}	},
	{ RSTR_INSTRUMENT_QC			,	{	"Instrument QC:",				"Instrument CQ:",				"Instrument QK:",				"Control calidad instr:",			"Strum. CQ:",					"CQ do instrumento:",				"QC de instrumento:"	}	},
	{ RSTR_NC						,	{	"N/C",							"N/C",							"N/B",							"N/C",								"N/C",							"N/B",								"N/C"	}	},
	{ RSTR_NEG						,	{	"NEG",							"NEG",							"NEG",							"NEG",								"NEG",							"NEG",								"NEG"	}	},
	{ RSTR_OPERATOR_ID				,	{	"Operator ID:",					"ID op�rateur :",				"Bediener-ID:",					"Id. operario:",					"ID operatore:",				"ID do operador:",					"ID do operador:"	}	},
	{ RSTR_OPERATOR_RENEWAL			,	{	"Operator Renewal Required",	"Renouv. op�r. n�cess.",		"Bedienererneuerung ben�tigt",	"Renovar operario",					"Necess. rinnovo operat.",		"Necess�rio renovar o operador",	"Renov. de operador necess�ria"	}	},
	{ RSTR_PATIENT_ID				,	{	"Patient ID:",					"ID patient :",					"Patienten-ID:",				"Id. paciente:",					"ID paziente:",					"ID do doente:",					"ID do paciente:"	}	},
	{ RSTR_POS						,	{	"POS",							"POS",							"POS",							"POS",								"POS",							"POS",								"POS"	}	},
	{ RSTR_QC_OK					,	{	"QC     OK",					"QC     OK",					"QUALITAETSKONTROLLE    OK",	"CONTROL DE CALIDAD     OK",		"CONTROLLO DI QUALITA   OK",	"CQ     OK",						"QC     OK"	}	},
	{ RSTR_RACE						,	{	"Race:",						"Race :",						"Rasse:",						"Raza:",							"Razza:",						"Ra�a:",							"Ra�a:"	}	},
	{ RSTR_RANGE					,	{	"Range:",						"Registre :",					"Bereich:",						"Rango:",							"Banda",						"Intervalo:",						"Intervalo:"	}	},
	{ RSTR_RANGE_LABEL				,	{	"Range Label:",					"�tiq.fourch:",					"Bereichskz.:",					"Intervalo:",						"Etich.range:",					"R�t. Inter.:",						"R�t. inter.:"	}	},
	{ RSTR_RESEARCH_SOFTWARE		,	{	"Research Software",			"Logiciel recherche",			"Research Software",			"Software investig.",				"Software ricerca",				"Software de pesquisa",				"Software de pesquisa"	}	},
	{ RSTR_ROTOR_LOT_NUM			,	{	"Disc Lot Number:",				"N� de lot disque :",			"Scheiben-Losnr.:",				"No. de lote del disco:",			"Numero lotto disco:",			"N�mero lote disco:",				"N�m. de lote do disco:"	}	},
	{ RSTR_SAMPLE_TYPE				,	{	"Sample Type:",					"Type �ch. :",					"Probentyp:",					"Muestra:",							"Tipo camp.:",					"Tipo amost.:",						"Tipo amostr:"	}	},
	{ RSTR_SERIAL_NUM				,	{	"Serial Number:",				"N� de s�rie :",				"Seriennr.:",					"N�mero de serie:",					"Numero di serie:",				"N�mero de s�rie:",					"N�mero de s�rie:"	}	},
	{ RSTR_UNAUTHORIZED_OPERATOR	,	{	"Unauthorized Operator",		"Op�r. non autoris�",			"Unautorisierter Bediener",		"Operario no autorizado",			"Operat. non autorizz.",		"Operador n�o autorizado",			"Operador n�o autorizado"	}	},
	{ RSTR_UNKNOWN					,	{	"Unknown",						"Inconnu",						"Unbekannt",					"Desconocido",						"Sconosciuto",					"Desconhecido",						"Desconhecido"	}	},

// Sample ID labels for results				ENGLISH,			FRENCH,				GERMAN,				SPANISH,			ITALIAN, 			PORTUGUESE,				PORTUGUESE (BRAZIL)
	{ RSTR_ADMISSION_ID				,	{	"Admission ID:",	"ID admission :",	"Zugangs-ID:",		"Id. admisi�n:",	"ID ammissione:",	"ID de Admiss�o:",		"ID de admiss�o:"	}	},
	{ RSTR_ALTERNATE_ID				,	{	"Alternate ID:",	"Autre ID :",		"ID wechseln:",		"Id. suplente:",	"ID alternativo:",	"ID suplente:",			"ID alternativo:"	}	},
	{ RSTR_DOCTOR_ID				,	{	"Doctor ID:",		"ID m�decin :",		"Arzt-ID:",			"Id. del m�dico:",	"ID medico:",		"ID do m�dico:",		"ID do m�dico:"	}	},
	{ RSTR_DOB						,	{	"DOB:",				"DOB :",			"GD:",				"F. nac.:",			"DN:",				"DOB:",					"DN:"	}	},
	{ RSTR_LOCATION					,	{	"Location:",		"Emplcmt:",			"Ort:",				"Ubicac.:",			"Sede:",			"Localiza��o:",			"Local:"	}	},
	{ RSTR_PHONE_NUM				,	{	"Phone Number:",	"N� t�l :",			"Telefonnummer:",	"Tel�fono:",		"Num. telefono:",	"N�m. telefone:",		"N�m. telefone:"	}	},
	{ RSTR_SAMPLE_ID				,	{	"Sample ID:",		"ID �ch. :",		"Proben-ID:",		"Id. de muestra:",	"ID campione:",		"ID da amostra:",		"ID da amostra:"	}	},
	
// Age labels for results					ENGLISH,	FRENCH,		GERMAN,		SPANISH,	ITALIAN, 		PORTUGUESE,		PORTUGUESE (BRAZIL)
	{ RSTR_DAY						,	{	"Day",		"Jour",		"Tag",		"D�a",		"Giorno",		"Dia",			"Dia"	}	},
	{ RSTR_DAYS						,	{	"Days",		"Jours",	"Tage",		"D�as",		"Giorni",		"Dias",			"Dias"	}	},
	{ RSTR_WEEK						,	{	"Week",		"Semaine",	"Woche",	"Semana",	"Settimana",	"Semana",		"Semana"	}	},
	{ RSTR_WEEKS					,	{	"Weeks",	"Semaines",	"Wochen",	"Semanas",	"Settimane",	"Semanas",		"Semanas"	}	},
	{ RSTR_MONTH					,	{	"Month",	"Mois",		"Monat",	"Mes",		"Mese",			"M�s",			"M�s"	}	},
	{ RSTR_MONTHS					,	{	"Months",	"Mois",		"Monate",	"Meses",	"Mesi",			"Meses",		"Meses"	}	},
	{ RSTR_YEAR						,	{	"Year",		"Ann�e",	"Jahr",		"A�o",		"Anno",			"Ano",			"Ano"	}	},
	{ RSTR_YEARS					,	{	"Years",	"Ann�es",	"Jahre",	"A�os",		"Anni",			"Anos",			"Anos"	}	},

// Genders									ENGLISH,	FRENCH,		GERMAN,			SPANISH,		ITALIAN, 	   	PORTUGUESE,	   	PORTUGUESE (BRAZIL)
	{ GSTR_FEMALE					,	{	"Female",	"F�minin",	"Weiblich",		"Mujer",		"Uomo",			"Feminino",		"Sexo feminino"	}	},
	{ GSTR_MALE						,	{	"Male",		"Masculin",	"M�nnlich",		"Hombre",		"Donna",		"Masculino",	"Sexo masculino"	}	},
	{ GSTR_NEUTERED					,	{	"Neutered",	"Ch�tr�",	"Kastriert",	"Castrado",		"Castrato",		"Castrado",		"Castrado"	}	},
	{ GSTR_SPAYED					,	{	"Spayed",	"Ch�tr�e",	"Sterilisiert",	"Castrada",		"Sterilizzato",	"Esterilizado",	"Esterilizado"	}	},
	{ GSTR_UNKNOWN					,	{	"Unknown",	"Inconnu",	"Unbekannt",	"Desconocido",	"Sconosciuto",	"Desconhecido",	"Desconhecido"	}	},
	
// Race labels for results					ENGLISH,					FRENCH,						GERMAN,						SPANISH,						ITALIAN, 				PORTUGUESE,					PORTUGUESE (BRAZIL)
//	  RSTR_UNKNOWN - defined above
	{ RSTR_WHITE					,	{	"White",					"Blanche",					"Wei�",						"Blanco",					"Bianco",					"Branco",					"Branco"	}	},
	{ RSTR_BLACK					,	{	"Black",					"Noire",					"Schwarz",					"Negro",					"Nero",						"Negro",					"Preto"	}	},
	{ RSTR_ASIAN_PACIFIC_ISLANDER	,	{	"Asian/Pacific Islander",	"Asiat./orig. �les Pac.",	"Asiatisch/Pazifisch",		"Asi�tico/Isl. Pac�fico",	"Isolano Asia/Pacifico",	"Ilhas Asi�t./ Pac�fico",	"Asi�tico/ilhas Pac�f."	}	},
	{ RSTR_NATIVE_AMERICAN			,	{	"Native American",			"Am�rindienne",				"Indianische Abstammung",	"Americano nativo",			"Nativo americano",			"Americano nativo",			"Americano nativo"	}	},
	{ RSTR_HISPANIC					,	{	"Hispanic",					"Hispanique",				"Hispanische Abstammung",	"Hispano",					"Ispanico",					"Hisp�nico",				"Hisp�nico"	}	},

// Species									ENGLISH,			FRENCH,				GERMAN,				SPANISH,	 		ITALIAN, 			PORTUGUESE,	 		PORTUGUESE (BRAZIL)
	{ SSTR_CONTROL					,	{	"Control",			"Contr�le",			"Kontrolle",		"Control",	 		"Controllo",		"Controlo",	 		"Controle"	}	},
	{ SSTR_PATIENT					,	{	"Patient",			"Patient",			"Patient",			"Paciente",		 	"Paziente",			"Doente",	 		"Paciente"	}	},
	{ SSTR_OTHER					,	{	"Patient",			"Patient",			"Patient",			"Paciente",		 	"Paziente",			"Doente",	 		"Paciente"	}	},
	{ SSTR_SPECIES					,	{	"Special",			"Sp�cial",			"Spezial",			"Especial",		 	"Speciale",			"Especial",	 		"Especial"	}	},
	
	{ SSTR_CANINES					,	{	"Category 1", 		"Cat�gorie 1", 		"Kategorie 1", 		"Categor�a 1",		"Categoria 1",		"Categoria 1",		"Categoria 1"	}	},
	{ SSTR_FELINES					,	{	"Category 2", 		"Cat�gorie 2", 		"Kategorie 2", 		"Categor�a 2",		"Categoria 2",		"Categoria 2",		"Categoria 2"	}	},
	{ SSTR_EQUINES					,	{	"Category 3", 		"Cat�gorie 3", 		"Kategorie 3", 		"Categor�a 3",		"Categoria 3",		"Categoria 3",		"Categoria 3"	}	},
	{ SSTR_BOVINES					,	{	"Category 4", 		"Cat�gorie 4", 		"Kategorie 4", 		"Categor�a 4",		"Categoria 4",		"Categoria 4",		"Categoria 4"	}	},
	{ SSTR_AVIANS					,	{	"Category 5", 		"Cat�gorie 5", 		"Kategorie 5", 		"Categor�a 5",		"Categoria 5",		"Categoria 5",		"Categoria 5"	}	},
	{ SSTR_REPTILES_AMPHIBIANS		,	{	"Category 6", 		"Cat�gorie 6", 		"Kategorie 6", 		"Categor�a 6",		"Categoria 6",		"Categoria 6",		"Categoria 6"	}	},
	{ SSTR_SMALL_MAMMALS			,	{	"Category 7", 		"Cat�gorie 7", 		"Kategorie 7", 		"Categor�a 7",		"Categoria 7",		"Categoria 7",		"Categoria 7"	}	},
	{ SSTR_LARGE_MAMMALS			,	{	"Category 8", 		"Cat�gorie 8", 		"Kategorie 8", 		"Categor�a 8",		"Categoria 8",		"Categoria 8",		"Categoria 8"	}	},

	// Canines
	{ SSTR_DOG						,	{	"Demographic 1",	"Elmts d�mogr. 1", 	"Demographie 1", 	"Demograf�a 1",		"Demografia 1",		"Demografia 1",		"Dados demogr. 1"	}	},
	{ SSTR_COYOTE					,	{	"Demographic 2",	"Elmts d�mogr. 2", 	"Demographie 2", 	"Demograf�a 2",		"Demografia 2",		"Demografia 2",		"Dados demogr. 2"	}	},
	{ SSTR_DINGO					,	{	"Demographic 3", 	"Elmts d�mogr. 3", 	"Demographie 3", 	"Demograf�a 3",		"Demografia 3",		"Demografia 3",		"Dados demogr. 3"	}	},
	{ SSTR_FOX						,	{	"Demographic 4", 	"Elmts d�mogr. 4", 	"Demographie 4", 	"Demograf�a 4",		"Demografia 4",		"Demografia 4",		"Dados demogr. 4"	}	},
	{ SSTR_JACKAL					,	{	"Demographic 5", 	"Elmts d�mogr. 5", 	"Demographie 5", 	"Demograf�a 5",		"Demografia 5",		"Demografia 5",		"Dados demogr. 5"	}	},
	{ SSTR_WOLF						,	{	"Demographic 6", 	"Elmts d�mogr. 6", 	"Demographie 6", 	"Demograf�a 6",		"Demografia 6",		"Demografia 6",		"Dados demogr. 6"	}	},

	// Felines
	{ SSTR_CAT						,	{	"Demographic 7", 	"Elmts d�mogr. 7", 	"Demographie 7"		"Demograf�a 7", 	"Demografia 7",		"Demografia 7",		"Dados demogr. 7"	}	},
	{ SSTR_BOBCAT					,	{	"Demographic 8", 	"Elmts d�mogr. 8", 	"Demographie 8",	"Demograf�a 8", 	"Demografia 8",		"Demografia 8",		"Dados demogr. 8"	}	},
	{ SSTR_CHEETAH					,	{	"Demographic 9", 	"Elmts d�mogr. 9", 	"Demographie 9",	"Demograf�a 9", 	"Demografia 9",		"Demografia 9",		"Dados demogr. 9"	}	},
	{ SSTR_JAGUAR					,	{	"Demographic 10", 	"Elmts d�mogr. 10", "Demographie 10",	"Demograf�a 10",	"Demografia 10",	"Demografia 10",	"Dados demogr. 10"	}	},
	{ SSTR_LEOPARD					,	{	"Demographic 11", 	"Elmts d�mogr. 11", "Demographie 11",	"Demograf�a 11",	"Demografia 11",	"Demografia 11",	"Dados demogr. 11"	}	},
	{ SSTR_LION						,	{	"Demographic 12", 	"Elmts d�mogr. 12", "Demographie 12",	"Demograf�a 12",	"Demografia 12",	"Demografia 12",	"Dados demogr. 12"	}	},
	{ SSTR_LYNX						,	{	"Demographic 13", 	"Elmts d�mogr. 13", "Demographie 13",	"Demograf�a 13",	"Demografia 13",	"Demografia 13",	"Dados demogr. 13"	}	},
	{ SSTR_PANTHER					,	{	"Demographic 14", 	"Elmts d�mogr. 14", "Demographie 14",	"Demograf�a 14",	"Demografia 14",	"Demografia 14",	"Dados demogr. 14"	}	},
	{ SSTR_PUMA						,	{	"Demographic 15", 	"Elmts d�mogr. 15", "Demographie 15",	"Demograf�a 15",	"Demografia 15",	"Demografia 15",	"Dados demogr. 15"	}	},
	{ SSTR_TIGER					,	{	"Demographic 16", 	"Elmts d�mogr. 16", "Demographie 16",	"Demograf�a 16",	"Demografia 16",	"Demografia 16",	"Dados demogr. 16"	}	},
	
	// Equines
	{ SSTR_HORSE					,	{	"Demographic 17", 	"Elmts d�mogr. 17", "Demographie 17",	"Demograf�a 17",	"Demografia 17",	"Demografia 17",	"Dados demogr. 17"	}	},
	{ SSTR_DONKEY					,	{	"Demographic 18", 	"Elmts d�mogr. 18", "Demographie 18",	"Demograf�a 18",	"Demografia 18",	"Demografia 18",	"Dados demogr. 18"	}	},
	{ SSTR_PONY						,	{	"Demographic 19", 	"Elmts d�mogr. 19", "Demographie 19",	"Demograf�a 19",	"Demografia 19",	"Demografia 19",	"Dados demogr. 19"	}	},
	{ SSTR_ZEBRA					,	{	"Demographic 20", 	"Elmts d�mogr. 20", "Demographie 20",	"Demograf�a 20",	"Demografia 20",	"Demografia 20",	"Dados demogr. 20"	}	},
	
	// Bovines
	{ SSTR_COW						,	{	"Demographic 21", 	"Elmts d�mogr. 21", "Demographie 21",	"Demograf�a 21",	"Demografia 21",	"Demografia 21",	"Dados demogr. 21"	}	},
	{ SSTR_WATER_BUFFALO			,	{	"Demographic 22", 	"Elmts d�mogr. 22", "Demographie 22",	"Demograf�a 22",	"Demografia 22",	"Demografia 22",	"Dados demogr. 22"	}	},
	{ SSTR_STEER					,	{	"Demographic 23", 	"Elmts d�mogr. 23", "Demographie 23",	"Demograf�a 23",	"Demografia 23",	"Demografia 23",	"Dados demogr. 23"	}	},
	{ SSTR_OX						,	{	"Demographic 24", 	"Elmts d�mogr. 24", "Demographie 24",	"Demograf�a 24",	"Demografia 24",	"Demografia 24",	"Dados demogr. 24"	}	},
	{ SSTR_BULL						,	{	"Demographic 25", 	"Elmts d�mogr. 25", "Demographie 25",	"Demograf�a 25",	"Demografia 25",	"Demografia 25",	"Dados demogr. 25"	}	},
	{ SSTR_BUFFALO					,	{	"Demographic 26", 	"Elmts d�mogr. 26", "Demographie 26",	"Demograf�a 26",	"Demografia 26",	"Demografia 26",	"Dados demogr. 26"	}	},
	{ SSTR_BISON					,	{	"Demographic 27", 	"Elmts d�mogr. 27", "Demographie 27",	"Demograf�a 27",	"Demografia 27",	"Demografia 27",	"Dados demogr. 27"	}	},
	
	// Avians
	{ SSTR_BIRD						,	{	"Demographic 28", 	"Elmts d�mogr. 28", "Demographie 28",	"Demograf�a 28",	"Demografia 28",	"Demografia 28",	"Dados demogr. 28"	}	},
	{ SSTR_AFRICAN_GRAY				,	{	"Demographic 29", 	"Elmts d�mogr. 29", "Demographie 29",	"Demograf�a 29",	"Demografia 29",	"Demografia 29",	"Dados demogr. 29"	}	},
	{ SSTR_AMAZON					,	{	"Demographic 30", 	"Elmts d�mogr. 30", "Demographie 30",	"Demograf�a 30",	"Demografia 30",	"Demografia 30",	"Dados demogr. 30"	}	},
	{ SSTR_CHICKEN					,	{	"Demographic 31", 	"Elmts d�mogr. 31", "Demographie 31",	"Demograf�a 31",	"Demografia 31",	"Demografia 31",	"Dados demogr. 31"	}	},
	{ SSTR_COCKATIEL				,	{	"Demographic 32", 	"Elmts d�mogr. 32", "Demographie 32",	"Demograf�a 32",	"Demografia 32",	"Demografia 32",	"Dados demogr. 32"	}	},
	{ SSTR_COCKATOO					,	{	"Demographic 33", 	"Elmts d�mogr. 33", "Demographie 33",	"Demograf�a 33",	"Demografia 33",	"Demografia 33",	"Dados demogr. 33"	}	},
	{ SSTR_CONURE					,	{	"Demographic 34", 	"Elmts d�mogr. 34", "Demographie 34",	"Demograf�a 34",	"Demografia 34",	"Demografia 34",	"Dados demogr. 34"	}	},
	{ SSTR_DOVE						,	{	"Demographic 35", 	"Elmts d�mogr. 35", "Demographie 35",	"Demograf�a 35",	"Demografia 35",	"Demografia 35",	"Dados demogr. 35"	}	},
	{ SSTR_DUCK						,	{	"Demographic 36", 	"Elmts d�mogr. 36", "Demographie 36",	"Demograf�a 36",	"Demografia 36",	"Demografia 36",	"Dados demogr. 36"	}	},
	{ SSTR_FLAMINGO					,	{	"Demographic 37", 	"Elmts d�mogr. 37", "Demographie 37",	"Demograf�a 37",	"Demografia 37",	"Demografia 37",	"Dados demogr. 37"	}	},
	{ SSTR_GOOSE					,	{	"Demographic 38", 	"Elmts d�mogr. 38", "Demographie 38",	"Demograf�a 38",	"Demografia 38",	"Demografia 38",	"Dados demogr. 38"	}	},
	{ SSTR_LOVEBIRD					,	{	"Demographic 39", 	"Elmts d�mogr. 39", "Demographie 39",	"Demograf�a 39",	"Demografia 39",	"Demografia 39",	"Dados demogr. 39"	}	},
	{ SSTR_MACAW					,	{	"Demographic 40", 	"Elmts d�mogr. 40", "Demographie 40",	"Demograf�a 40",	"Demografia 40",	"Demografia 40",	"Dados demogr. 40"	}	},
	{ SSTR_MYNAH					,	{	"Demographic 41", 	"Elmts d�mogr. 41", "Demographie 41",	"Demograf�a 41",	"Demografia 41",	"Demografia 41",	"Dados demogr. 41"	}	},
	{ SSTR_OSTRICH					,	{	"Demographic 42", 	"Elmts d�mogr. 42", "Demographie 42",	"Demograf�a 42",	"Demografia 42",	"Demografia 42",	"Dados demogr. 42"	}	},
	{ SSTR_PARAKEET					,	{	"Demographic 43", 	"Elmts d�mogr. 43", "Demographie 43",	"Demograf�a 43",	"Demografia 43",	"Demografia 43",	"Dados demogr. 43"	}	},
	{ SSTR_PARROT					,	{	"Demographic 44", 	"Elmts d�mogr. 44", "Demographie 44",	"Demograf�a 44",	"Demografia 44",	"Demografia 44",	"Dados demogr. 44"	}	},
	{ SSTR_TOUCAN					,	{	"Demographic 45", 	"Elmts d�mogr. 45", "Demographie 45",	"Demograf�a 45",	"Demografia 45",	"Demografia 45",	"Dados demogr. 45"	}	},
	
	// Reptiles/amphibians
	{ SSTR_REPTILE					,	{	"Demographic 46",	"Elmts d�mogr. 46", "Demographie 46",	"Demograf�a 46",	"Demografia 46",	"Demografia 46",	"Dados demogr. 46"	}	},
	{ SSTR_ALLIGATOR				,	{	"Demographic 47",	"Elmts d�mogr. 47", "Demographie 47",	"Demograf�a 47",	"Demografia 47",	"Demografia 47",	"Dados demogr. 47"	}	},
	{ SSTR_AMPHIBIAN				,	{	"Demographic 48",	"Elmts d�mogr. 48", "Demographie 48",	"Demograf�a 48",	"Demografia 48",	"Demografia 48",	"Dados demogr. 48"	}	},
	{ SSTR_BEARDED_DRAGON			,	{	"Demographic 49",	"Elmts d�mogr. 49", "Demographie 49",	"Demograf�a 49",	"Demografia 49",	"Demografia 49",	"Dados demogr. 49"	}	},
	{ SSTR_BOA_CONSTRICTOR			,	{	"Demographic 50",	"Elmts d�mogr. 50", "Demographie 50",	"Demograf�a 50",	"Demografia 50",	"Demografia 50",	"Dados demogr. 50"	}	},
	{ SSTR_CHAMELEON				,	{	"Demographic 51",	"Elmts d�mogr. 51", "Demographie 51",	"Demograf�a 51",	"Demografia 51",	"Demografia 51",	"Dados demogr. 51"	}	},
	{ SSTR_CROCODILE				,	{	"Demographic 52",	"Elmts d�mogr. 52", "Demographie 52",	"Demograf�a 52",	"Demografia 52",	"Demografia 52",	"Dados demogr. 52"	}	},
	{ SSTR_FROG						,	{	"Demographic 53",	"Elmts d�mogr. 53", "Demographie 53",	"Demograf�a 53",	"Demografia 53",	"Demografia 53",	"Dados demogr. 53"	}	},
	{ SSTR_GECKO					,	{	"Demographic 54",	"Elmts d�mogr. 54", "Demographie 54",	"Demograf�a 54",	"Demografia 54",	"Demografia 54",	"Dados demogr. 54"	}	},
	{ SSTR_IGUANA					,	{	"Demographic 55",	"Elmts d�mogr. 55", "Demographie 55",	"Demograf�a 55",	"Demografia 55",	"Demografia 55",	"Dados demogr. 55"	}	},
	{ SSTR_LIZARD					,	{	"Demographic 56",	"Elmts d�mogr. 56", "Demographie 56",	"Demograf�a 56",	"Demografia 56",	"Demografia 56",	"Dados demogr. 56"	}	},
	{ SSTR_MONITOR					,	{	"Demographic 57",	"Elmts d�mogr. 57", "Demographie 57",	"Demograf�a 57",	"Demografia 57",	"Demografia 57",	"Dados demogr. 57"	}	},
	{ SSTR_PYTHON					,	{	"Demographic 58",	"Elmts d�mogr. 58", "Demographie 58",	"Demograf�a 58",	"Demografia 58",	"Demografia 58",	"Dados demogr. 58"	}	},
	{ SSTR_SALAMANDER				,	{	"Demographic 59",	"Elmts d�mogr. 59", "Demographie 59",	"Demograf�a 59",	"Demografia 59",	"Demografia 59",	"Dados demogr. 59"	}	},
	{ SSTR_SNAKE					,	{	"Demographic 60",	"Elmts d�mogr. 60", "Demographie 60",	"Demograf�a 60",	"Demografia 60",	"Demografia 60",	"Dados demogr. 60"	}	},
	{ SSTR_TOAD						,	{	"Demographic 61",	"Elmts d�mogr. 61", "Demographie 61",	"Demograf�a 61",	"Demografia 61",	"Demografia 61",	"Dados demogr. 61"	}	},
	{ SSTR_TORTOISE					,	{	"Demographic 62",	"Elmts d�mogr. 62", "Demographie 62",	"Demograf�a 62",	"Demografia 62",	"Demografia 62",	"Dados demogr. 62"	}	},
	{ SSTR_TURTLE					,	{	"Demographic 63",	"Elmts d�mogr. 63", "Demographie 63",	"Demograf�a 63",	"Demografia 63",	"Demografia 63",	"Dados demogr. 63"	}	},
	
	// Small mammals
	{ SSTR_SMALL_MAMMAL				,	{	"Demographic 64",	"Elmts d�mogr. 64", "Demographie 64",	"Demograf�a 64",	"Demografia 64",	"Demografia 64",	"Dados demogr. 64"	}	},
	{ SSTR_BAT						,	{	"Demographic 65",	"Elmts d�mogr. 65", "Demographie 65",	"Demograf�a 65",	"Demografia 65",	"Demografia 65",	"Dados demogr. 65"	}	},
	{ SSTR_CHINCHILLA				,	{	"Demographic 66",	"Elmts d�mogr. 66", "Demographie 66",	"Demograf�a 66",	"Demografia 66",	"Demografia 66",	"Dados demogr. 66"	}	},
	{ SSTR_FERRET					,	{	"Demographic 67",	"Elmts d�mogr. 67", "Demographie 67",	"Demograf�a 67",	"Demografia 67",	"Demografia 67",	"Dados demogr. 67"	}	},
	{ SSTR_GERBIL					,	{	"Demographic 68",	"Elmts d�mogr. 68", "Demographie 68",	"Demograf�a 68",	"Demografia 68",	"Demografia 68",	"Dados demogr. 68"	}	},
	{ SSTR_GUINEA_PIG				,	{	"Demographic 69",	"Elmts d�mogr. 69", "Demographie 69",	"Demograf�a 69",	"Demografia 69",	"Demografia 69",	"Dados demogr. 69"	}	},
	{ SSTR_HAMSTER					,	{	"Demographic 70",	"Elmts d�mogr. 70", "Demographie 70",	"Demograf�a 70",	"Demografia 70",	"Demografia 70",	"Dados demogr. 70"	}	},
	{ SSTR_HEDGEHOG					,	{	"Demographic 71",	"Elmts d�mogr. 71", "Demographie 71",	"Demograf�a 71",	"Demografia 71",	"Demografia 71",	"Dados demogr. 71"	}	},
	{ SSTR_LEMUR					,	{	"Demographic 72",	"Elmts d�mogr. 72", "Demographie 72",	"Demograf�a 72",	"Demografia 72",	"Demografia 72",	"Dados demogr. 72"	}	},
	{ SSTR_MINK						,	{	"Demographic 73",	"Elmts d�mogr. 73", "Demographie 73",	"Demograf�a 73",	"Demografia 73",	"Demografia 73",	"Dados demogr. 73"	}	},
	{ SSTR_MOUSE					,	{	"Demographic 74",	"Elmts d�mogr. 74", "Demographie 74",	"Demograf�a 74",	"Demografia 74",	"Demografia 74",	"Dados demogr. 74"	}	},
	{ SSTR_OPOSSUM					,	{	"Demographic 75",	"Elmts d�mogr. 75", "Demographie 75",	"Demograf�a 75",	"Demografia 75",	"Demografia 75",	"Dados demogr. 75"	}	},
	{ SSTR_PRAIRIE_DOG				,	{	"Demographic 76",	"Elmts d�mogr. 76", "Demographie 76",	"Demograf�a 76",	"Demografia 76",	"Demografia 76",	"Dados demogr. 76"	}	},
	{ SSTR_RABBIT					,	{	"Demographic 77",	"Elmts d�mogr. 77", "Demographie 77",	"Demograf�a 77",	"Demografia 77",	"Demografia 77",	"Dados demogr. 77"	}	},
	{ SSTR_RACCOON					,	{	"Demographic 78",	"Elmts d�mogr. 78", "Demographie 78",	"Demograf�a 78",	"Demografia 78",	"Demografia 78",	"Dados demogr. 78"	}	},
	{ SSTR_RAT						,	{	"Demographic 79",	"Elmts d�mogr. 79", "Demographie 79",	"Demograf�a 79",	"Demografia 79",	"Demografia 79",	"Dados demogr. 79"	}	},
	{ SSTR_SKUNK					,	{	"Demographic 80",	"Elmts d�mogr. 80", "Demographie 80",	"Demograf�a 80",	"Demografia 80",	"Demografia 80",	"Dados demogr. 80"	}	},
	{ SSTR_SQUIRREL					,	{	"Demographic 81",	"Elmts d�mogr. 81", "Demographie 81",	"Demograf�a 81",	"Demografia 81",	"Demografia 81",	"Dados demogr. 81"	}	},
	
	// Large mammals
	{ SSTR_LARGE_MAMMAL				,	{	"Demographic 82",	"Elmts d�mogr. 82", "Demographie 82",	"Demograf�a 82",	"Demografia 82",	"Demografia 82",	"Dados demogr. 82"	}	},
	{ SSTR_ANTELOPE					,	{	"Demographic 83",	"Elmts d�mogr. 83", "Demographie 83",	"Demograf�a 83",	"Demografia 83",	"Demografia 83",	"Dados demogr. 83"	}	},
	{ SSTR_APE						,	{	"Demographic 84",	"Elmts d�mogr. 84", "Demographie 84",	"Demograf�a 84",	"Demografia 84",	"Demografia 84",	"Dados demogr. 84"	}	},
	{ SSTR_BABOON					,	{	"Demographic 85",	"Elmts d�mogr. 85", "Demographie 85",	"Demograf�a 85",	"Demografia 85",	"Demografia 85",	"Dados demogr. 85"	}	},
	{ SSTR_BEAR						,	{	"Demographic 86",	"Elmts d�mogr. 86", "Demographie 86",	"Demograf�a 86",	"Demografia 86",	"Demografia 86",	"Dados demogr. 86"	}	},
	{ SSTR_CAMEL					,	{	"Demographic 87",	"Elmts d�mogr. 87", "Demographie 87",	"Demograf�a 87",	"Demografia 87",	"Demografia 87",	"Dados demogr. 87"	}	},
	{ SSTR_CHIMPANZEE				,	{	"Demographic 88",	"Elmts d�mogr. 88", "Demographie 88",	"Demograf�a 88",	"Demografia 88",	"Demografia 88",	"Dados demogr. 88"	}	},
	{ SSTR_DEER						,	{	"Demographic 89",	"Elmts d�mogr. 89", "Demographie 89",	"Demograf�a 89",	"Demografia 89",	"Demografia 89",	"Dados demogr. 89"	}	},
	{ SSTR_ELEPHANT					,	{	"Demographic 90",	"Elmts d�mogr. 90", "Demographie 90",	"Demograf�a 90",	"Demografia 90",	"Demografia 90",	"Dados demogr. 90"	}	},
	{ SSTR_GIRAFFE					,	{	"Demographic 91",	"Elmts d�mogr. 91", "Demographie 91",	"Demograf�a 91",	"Demografia 91",	"Demografia 91",	"Dados demogr. 91"	}	},
	{ SSTR_GOAT						,	{	"Demographic 92",	"Elmts d�mogr. 92", "Demographie 92",	"Demograf�a 92",	"Demografia 92",	"Demografia 92",	"Dados demogr. 92"	}	},
	{ SSTR_KANGAROO					,	{	"Demographic 93",	"Elmts d�mogr. 93", "Demographie 93",	"Demograf�a 93",	"Demografia 93",	"Demografia 93",	"Dados demogr. 93"	}	},
	{ SSTR_LLAMA					,	{	"Demographic 94",	"Elmts d�mogr. 94", "Demographie 94",	"Demograf�a 94",	"Demografia 94",	"Demografia 94",	"Dados demogr. 94"	}	},
	{ SSTR_MONKEY					,	{	"Demographic 95",	"Elmts d�mogr. 95", "Demographie 95",	"Demograf�a 95",	"Demografia 95",	"Demografia 95",	"Dados demogr. 95"	}	},
	{ SSTR_PIG						,	{	"Demographic 96",	"Elmts d�mogr. 96", "Demographie 96",	"Demograf�a 96",	"Demografia 96",	"Demografia 96",	"Dados demogr. 96"	}	},
	{ SSTR_SEA_LION					,	{	"Demographic 97",	"Elmts d�mogr. 97", "Demographie 97",	"Demograf�a 97",	"Demografia 97",	"Demografia 97",	"Dados demogr. 97"	}	},
	{ SSTR_SEAL						,	{	"Demographic 98",	"Elmts d�mogr. 98", "Demographie 98",	"Demograf�a 98",	"Demografia 98",	"Demografia 98",	"Dados demogr. 98"	}	},
	{ SSTR_SHEEP					,	{	"Demographic 99",	"Elmts d�mogr. 99", "Demographie 99",	"Demograf�a 99",	"Demografia 99",	"Demografia 99",	"Dados demogr. 99"	}	},
	
// Analytes									ENGLISH,  FRENCH,	  GERMAN,	  SPANISH,	  ITALIAN, 	  PORTUGUESE, PORTUGUESE (BRAZIL)
	{ ASTR_AG						,	{	"AG"	, "AG"		, "AG"		, "AG"		, "GA"		, "AG"		, "AG"	}	},
	{ ASTR_ALB						,	{	"ALB"	, "ALB"		, "ALB"		, "ALB"		, "ALB"		, "ALB"		, "ALB"	}	},
	{ ASTR_ALP						,	{	"ALP"	, "ALP"		, "ALP"		, "ALP"		, "ALP"		, "ALP"		, "ALP"	}	},
	{ ASTR_ALT						,	{	"ALT"	, "ALT"		, "ALT"		, "ALT"		, "ALT"		, "ALT"		, "ALT"	}	},
	{ ASTR_AMY						,	{	"AMY"	, "AMY"		, "AMY"		, "AMY"		, "AMY"		, "AMY"		, "AMY"	}	},
	{ ASTR_AST						,	{	"AST"	, "AST"		, "AST"		, "AST"		, "AST"		, "AST"		, "AST"	}	},
	{ ASTR_BA						,	{	"BA"	, "BA"		, "BA"		, "BA"		, "BA"		, "BA"		, "BA"	}	},
	{ ASTR_BUN						,	{	"BUN"	, "BUN"		, "BUN"		, "BUN"		, "BUN"		, "BUN"		, "BUN"	}	},
	{ ASTR_CA						,	{	"CA"	, "CA"		, "CA"		, "CA"		, "CA"		, "CA"		, "CA"	}	},
	{ ASTR_CHOL						,	{	"CHOL"	, "CHOL"	, "CHOL"	, "COL"		, "COL"		, "CHOL"	, "CHOL"	}	},
	{ ASTR_CHW						,	{	"CHW"	, "Di im"	, "HHW"		, "CHW"		, "CHW"		, "CHW"		, "CHW"	}	},
	{ ASTR_CK						,	{	"CK"	, "CK"		, "CK"		, "CK"		, "CK"		, "CK"		, "CK"	}	},
	{ ASTR_CL						,	{	"CL-"	, "CL-"		, "CL-"		, "CL-"		, "CL-"		, "CL-"		, "CL-"	}	},
	{ ASTR_CRE						,	{	"CRE"	, "CRE"		, "CRE"		, "CRE"		, "CRE"		, "CRE"		, "CRE"	}	},
	{ ASTR_CRP						,	{	"CRP"	, "CRP"		, "CRP"		, "CRP"		, "CRP"		, "CRP"		, "CRP"	}	},
	{ ASTR_DBIL						,	{	"DBIL"	, "DBIL"	, "DBIL"	, "DBIL"	, "DBIL"	, "DBIL"	, "DBIL"	}	},
	{ ASTR_EGFR						,	{	"eGFR"	, "eGFR"	, "eGFR"	, "eGFR"	, "eGFR"	, "eGFR"	, "eGFR"	}	},
	{ ASTR_GGT						,	{	"GGT"	, "GGT"		, "GGT"		, "GGT"		, "GGT"		, "GGT"		, "GGT"	}	},
	{ ASTR_GLOB						,	{	"GLOB"	, "GLOB"	, "GLOB"	, "GLOB"	, "GLOB"	, "GLOB"	, "GLOB"	}	},
	{ ASTR_GLU						,	{	"GLU"	, "GLU"		, "GLU"		, "GLU"		, "GLU"		, "GLU"		, "GLU"	}	},
	{ ASTR_HB						,	{	"HB"	, "HB"		, "HB"		, "HB"		, "HB"		, "HB"		, "HB"	}	},
	{ ASTR_HCT						,	{	"HCT"	, "HCT"		,"HCT"		,"HCT"		, "HCT"		, "HCT"		, "HCT"	}	},   
	{ ASTR_HDL						,	{	"HDL"	, "LHD"		, "HDL"		, "HDL"		, "HDL"		, "HDL"		, "HDL"	}	},
	{ ASTR_IBIL						,	{	"IBIL"	, "IBIL"	, "IBIL"	, "IBIL"	, "IBIL"	, "IBIL"	, "IBIL"	}	},
	{ ASTR_K						,	{	"K+"	, "K+"		, "K+"		, "K+"		, "K+"		, "K+"		, "K+"	}	},
	{ ASTR_LAC						,	{	"LAC"	, "LAC"		,"LAC"		,"LAC"		, "LAC"		, "LAC"		, "LAC"	}	},
	{ ASTR_LD						,	{	"LD"	, "LD"		, "LD"		, "LD"		, "LD"		, "LD"		, "LD"	}	},
	{ ASTR_LDL						,	{	"LDL"	, "LBD"		, "LDL"		, "LDL"		, "LDL"		, "LDL"		, "LDL"	}	},
	{ ASTR_MG						,	{	"MG"	, "MG"		, "MG"		, "MG"		, "MG"		, "MG"		, "MG"	}	},
	{ ASTR_NA						,	{	"NA+"	, "NA+"		, "NA+"		, "NA+"		, "NA+"		, "NA+"		, "NA+"	}	},
	{ ASTR_NHDLC					,	{	"nHDLc"	, "nHDLc"	, "nHDLc"	, "nHDLC"	, "nHDLC"	, "nHDLc"	, "nHDLc"	}	},
	{ ASTR_PHB						,	{	"PHB"	, "PHB"		, "PHB"		, "PHB"		, "PHB"		, "PHB"		, "PHB"	}	},
	{ ASTR_PHOS						,	{	"PHOS"	, "PHOS"	, "PHOS"	, "FOS"		, "FOS"		, "PHOS"	, "PHOS"	}	},
	{ ASTR_T4						,	{	"T4"	, "T4"		, "T4"		, "T4"		, "T4"		, "T4"		, "T4"	}	},
	{ ASTR_TBIL						,	{	"TBIL"	, "TBIL"	, "TBIL"	, "TBIL"	, "TBIL"	, "TBIL"	, "TBIL"	}	},
	{ ASTR_TCH						,	{	"TC/H"	, "TC/H"	, "TC/H"	, "TC/H"	, "TC/H"	, "TC/H"	, "TC/H"	}	},
	{ ASTR_TCO2						,	{	"tCO2"	, "tCO2"	, "tCO2"	, "tCO2"	, "tCO2"	, "tCO2"	, "tCO2"	}	},
	{ ASTR_TP						,	{	"TP"	, "TP"		, "TP"		, "TP"		, "TP"		, "TP"		, "TP"	}	},
	{ ASTR_TRIG						,	{	"TRIG"	, "TRIG"	, "TRIG"	, "TRIG"	, "TRIG"	, "TRIG"	, "TRIG"	}	},
	{ ASTR_UA						,	{	"UA"	, "UA"		, "UA"		, "UA"		, "UA"		, "UA"		, "UA"	}	},
	{ ASTR_VLDL						,	{	"VLDL"	, "LTBD"	, "VLDL"	, "PMBD"	, "VLDL"	, "VLDL"	, "VLDL"	}	},

// Misc.									ENGLISH,		FRENCH,		GERMAN,			SPANISH,		ITALIAN,		PORTUGUESE,		PORTUGUESE (BRAZIL)
	{ STR_JAN						,	{	"Jan",			"Jan",		"Jan",			"Ene",			"Gen",			"Jan",			"Jan"	}	},
	{ STR_FEB						,	{	"Feb",			"F�v",		"Feb",			"Feb",			"Feb",			"Fev",			"Fev"	}	},
	{ STR_MAR						,	{	"Mar",			"Mar",		"M�r",			"Mar",			"Mar",			"Mar",			"Mar"	}	},
	{ STR_APR						,	{	"Apr",			"Avr",		"Apr",			"Abr",			"Apr",			"Abr",			"Abr"	}	},
	{ STR_MAY						,	{	"May",			"Mai",		"Mai",			"May",			"Mag",			"Mai",			"Mai"	}	},
	{ STR_JUN						,	{	"Jun",			"Jun",		"Jun",			"Jun",			"Giu",			"Jun",			"Jun"	}	},
	{ STR_JUL						,	{	"Jul",			"Jul",		"Jul",			"Jul",			"Lug",			"Jul",			"Jul"	}	},
	{ STR_AUG						,	{	"Aug",			"Ao�",		"Aug",			"Ago",			"Ago",			"Ago",			"Ago"	}	},
	{ STR_SEP						,	{	"Sep",			"Sep",		"Sep",			"Sep",			"Set",			"Set",			"Set"	}	},
	{ STR_OCT						,	{	"Oct",			"Oct",		"Okt",			"Oct",			"Ott",			"Out",			"Out"	}	},
	{ STR_NOV						,	{	"Nov",			"Nov",		"Nov",			"Nov",			"Nov",			"Nov",			"Nov"	}	},
	{ STR_DEC						,	{	"Dec",			"D�c",		"Dez",			"Dic",			"Dic",			"Dez",			"Dez"	}	},

	{ STR_SUNDAY					,	{	"Sunday",		"Dimanche",	"Sonntag",		"Domingo",		"Domenica",		"Domingo",		"Domingo"	}	},
	{ STR_MONDAY					,	{	"Monday",		"Lundi",	"Montag",		"Lunes",		"Luned�",		"Segunda",		"Segunda-feira"	}	},
	{ STR_TUESDAY					,	{	"Tuesday",		"Mardi",	"Dienstag",		"Martes",		"Marted�",		"Ter�a",		"Ter�a-feira"	}	},
	{ STR_WEDNESDAY					,	{	"Wednesday",	"Mercredi",	"Mittwoch",		"Mi�rcoles",	"Mercoled�",	"Quarta",		"Quarta-feira"	}	},
	{ STR_THURSDAY					,	{	"Thursday",		"Jeudi",	"Donnerstag",	"Jueves",		"Gioved�",		"Quinta",		"Quinta-feira"	}	},
	{ STR_FRIDAY					,	{	"Friday",		"Vendredi",	"Freitag",		"Viernes",		"Venerd�",		"Sexta",		"Sexta-feira"	}	},
	{ STR_SATURDAY					,	{	"Saturday",		"Samedi",	"Samstag",		"S�bado",		"Sabato",		"S�bado",		"S�bado"	}	},

	{ STR_NONE						,	{} }
};


StringArr_t	VET_STRINGS[] = {
// 											ENGLISH,					FRENCH,								GERMAN,								SPANISH,								ITALIAN, 								PORTUGUESE,									PORTUGUESE (BRAZIL)
	{ STR_PRODUCT_NAME				,	{	"VetScan VS2",				"VetScan VS2",						"VetScan VS2",						"VetScan VS2",							"VetScan VS2",							"VetScan VS2",								"VetScan VS2"	}	},

// Top
	{ TSTR_ADD_SPECIES				,	{	"Add Species",				"Ajouter esp�ces",					"Arten hinzuf�gen",					"A�adir especie",						"Aggiungi specie",						"Adicionar esp�cies",						"Adicionar esp�cie"	}	},
	{ TSTR_FACTORY_DEFAULT_SPECIES	,	{	"Factory Default Species",	"Param�tres par d�faut esp�ces",	"Voreingestellte Standardarten",	"Especies predeterminadas",				"Specie predefinita di fabbrica",		"Esp�cies predeterminadas de f�brica",		"Esp�cie padr�o de f�brica"	}	},
	{ TSTR_INSERT_ROTOR				,	{	"Insert Rotor",				"Ins�rer rotor",					"Rotor einsetzen",					"Insertar rotor",						"Inserisci rotore",						"Inserir rotor",							"Inserir rotor"	}	},
	{ TSTR_LOAD_ROTOR				,	{	"Load Rotor",				"Charger rotor",					"Rotor laden",						"Cargar rotor",							"Carica rotore",						"Carregar rotor",							"Carregar rotor"	}	},
	{ TSTR_REMOVE_ROTOR				,	{	"Remove Rotor",				"Retirer rotor",					"Rotor entfernen",					"Retirar rotor",						"Rimuovi rotore",						"Remover rotor",							"Remover rotor"	}	},
	{ TSTR_REMOVE_SPECIES			,	{	"Remove Species",			"Retirer esp�ces",					"Arten entfernen",					"Eliminar especie",						"Rimuovi specie",						"Remover esp�cies",							"Remover esp�cies"	}	},
	{ TSTR_SELECT_SPECIES			,	{	"Select Species",			"S�lectionner esp�ces",				"Arten ausw�hlen",					"Seleccionar especie",					"Seleziona specie",						"Selecionar esp�cies",						"Selecionar esp�cies"	}	},
	{ TSTR_SPECIES					,	{	"Species",					"Esp�ces",							"Arten",							"Especie",								"Specie",								"Esp�cies",									"Esp�cies"	}	},
	{ TSTR_SPECIES_AVAILABLE		,	{	"Species Available",		"Esp�ces disponibles",				"Verf�gbare Arten",					"Especies disponibles",					"Specie disponibili",					"Esp�cies dispon�veis",						"Esp�cies dispon�veis"	}	},

// Middle									ENGLISH,																		FRENCH,																							GERMAN,																										SPANISH,																							ITALIAN,																								PORTUGUESE,																									PORTUGUESE (BRAZIL)
	{ MSTR_ADD_SPECIES				,	{	"Add Species",																	"Ajouter esp�ces",																				"Arten hinzuf�gen",																							"A�adir especie",																					"Aggiungi specie",																						"Adicionar esp�cies",																						"Adicionar esp�cie"	}	},
	{ MSTR_DETERMINING_ROTOR_TYPE	,	{	"Determining rotor type",														"D�term. type de rotor",																		"Rotortyp festlegen",																						"Determinando el tipo de rotor",																	"Determinazione tipo del rotore",																		"A determinar o tipo de rotor",																				"Determinando o tipo de rotor"	}	},
	{ MSTR_FACTORY_DEFAULT_SPECIES	,	{	"Factory Default Species",														"Param. par d�faut esp�ces",																	"Voreingestellte Arten",																					"Especie predeterminada de f�brica",																"Specie predefinita di fabbrica",																		"Esp�cies predeterminadas de f�brica",																		"Esp�cie padr�o de f�brica"	}	},
	{ MSTR_FEMALE					,	{	"Female",																		"Femelle",																						"Weiblich",																									"Hembra",																							"Femmina",																								"F�mea",																									"Sexo feminino"	}	},
	{ MSTR_LAST_ROTOR				,	{	"Last Rotor",																	"Dernier rotor",																				"Letzter Rotor",																							"�ltimo rotor",																						"Ultimo rotore",																						"�ltimo rotor",																								"�ltimo rotor"	}	},
	{ MSTR_LOAD_ROTOR_TO_ANALYZE	,	{	"To analyze additional sample, load new rotor",									"Pour analyser un �chantillon suppl�mentaire, charger un nouveau rotor",						"Zur Analyse einer weiteren Probe neuen Rotor laden",														"Para analizar otra muestra, cargue un rotor nuevo",												"Per esaminare ulteriori campioni, caricare nuovo rotore",												"Para analisar outra amostra, carregue novo rotor",															"Para analisar uma amostra adicional, carregue um novo rotor"	}	},
	{ MSTR_MALE						,	{	"Male",																			"M�le",																							"M�nnlich",																									"Macho",																							"Maschio",																								"Macho",																									"Sexo masculino"	}	},
	{ MSTR_NO_RESULTS				,	{	"No saved results to view. Please run a rotor prior to recalling results.",		"Aucun r�sultat enregistr� � afficher. Ex�cuter un rotor avant de rappeler les r�sultats.",		"Keine gespeicherten Ergebnisse zur Anzeige. Vor Wiederaufrufen der Ergebnisse einen Rotor starten.",		"No hay resultados guardados para ver. Utilice un rotor antes de buscar resultados.",				"Nessun risultato salvato da visualizzare. Avviare un rotore prima di richiamare i risultati.",			"No existem resultados guardados para visualizar. Utilize um rotor antes de recuperar resultados.",			"Nenhum resultado salvo para visualiza��o. Execute um rotor antes de recuperar os resultados."	}	},
	{ MSTR_PLEASE_REMOVE_ROTOR		,	{	"Please remove the rotor from the analyzer",									"Retirer le rotor de l'analyseur",																"Bitte den Rotor aus dem Analyseger�t entfernen",															"Retire el rotor del analizador",																	"Rimuovere il rotore dall'analizzatore",																"Retire o rotor do analisador",																				"Remova o rotor do analisador"	}	},
	{ MSTR_PRESS_YES_REF_RANGES		,	{	"Press Yes to return the active species list to the factory default",			"Appuyer sur Oui pour r�tablir la liste d'esp�ces actives par d�faut",							"Dr�cken Sie Ja, um die Werkseinstellungen f�r die Liste der aktiven Arten wieder herzustellen",			"Pulse S� para reestablecer los valores predeterminados de la lista de especies activas",			"Premere S� per mandare l'elenco specie attive nelle impostazioni di fabbrica",							"Prima Sim para reestabelecer a lista de esp�cies ativa para os valores predeterminados de f�brica",		"Pressione Sim para retornar a lista de esp�cies ativas ao padr�o de f�brica"	}	},
	{ MSTR_REMOVE_SPECIES			,	{	"Remove Species",																"Retirer esp�ces",																				"Arten entfernen",																							"Eliminar especie",																					"Rimuovi specie",																						"Remover esp�cies",																							"Remover esp�cies"	}	},
	{ MSTR_ROTORS_RUN				,	{	"Rotors Run",																	"Ex�cutions rotors",																			"Rotoren in Betrieb",																						"Usar rotores",																						"Rotori in funzione",																					"Utilizar rotores",																							"Execu��o de rotores"	}	},
	{ MSTR_SPECIES					,	{	"Species",																		"Esp�ces",																						"Arten",																									"Esp.",																								"Specie",																								"Esp�cies",																									"Esp�cies"	}	},

// Info text
	{ ISTR_LOAD_ROTOR_1				,	{	"Remove reagent rotor from its foil pouch and transfer 100ul of whole blood, plasma or serum into the rotor.\n",				// English
											"Retirer le rotor de r�actif de sa pochette alu et transf�rer 100 ul de sang entier, de plasma ou de s�rum dans le rotor.\n",	// French
											"Reagenzrotor aus dem Plastikbeutel entfernen und 100ul Vollblut, Plasma oder Serum in den Rotor einf�llen.\n",					// German
											"Retire el rotor de reactivo de su bolsa metalizada y transfiera 100 ul de sangre, plasma o suero al rotor.\n",					// Spanish
											"Rimuovere il rotore reagente dalla custodia metallizzata e trasferire 100ul di sangue intero, plasma o siero nel rotore.\n",	// Italian
											"Retire o rotor de reagente da sua bolsa metalizada e transfira 100 ul de sangue total, plasma ou soro para o rotor.\n",		// Portuguese
											"Remova o rotor de reagente da bolsa de alum�nio e transfira 100 �l de sangue total, plasma ou soro para o rotor.\n"			// Portuguese (Brazil)
										} 
	},
	{ ISTR_LOAD_ROTOR_2				,	{	"Insert the reagent rotor into the analyzer, close the drawer and follow the on-screen prompts.\n",								// English
											"Ins�rer le rotor de r�actif dans l'analyseur, fermer le tiroir et suivre les instructions qui s'affichent � l'�cran.\n",		// French
											"Reagenzrotor in das Analyseger�t einf�hren, Fach schlie�en und Bildschirmanweisungen befolgen.\n",								// German
											"Inserte el rotor de reactivo en el analizador, cierre el caj�n y siga las instrucciones de pantalla.\n",						// Spanish
											"Inserire il rotore reagente nell'analizzatore, chiudere il cassetto e seguire le istruzioni a schermo.\n",						// Italian
											"Insira o rotor de reagente no analisador, feche a gaveta e siga as instru��es no ecr�.\n",										// Portuguese
											"Insira o rotor de reagente no analisador, feche a gaveta e siga as instru��es na tela.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_SEND				,	{	"Printer: Print a copy of a species reference range.\n\n"
											"Internal Archive: Save a copy of all active reference ranges internal to the analyzer for future recall.\n\n"
											"External Archive: Transmit an ASCII dump of a species reference range to an external system.\n",																		// English
											"Imprimante : imprimer un exemplaire de fourchette de r�f�rence d'esp�ces.\n\n"
											"Archive interne : enregistrer un exemplaire de toutes les fourchettes de r�f�rence actives internes � l'analyseur afin de pouvoir les r�cup�rer ult�rieurement.\n\n"
											"Archive externe : transmettre un vidage de m�moire ASCII d'une fourchette de r�f�rence d'esp�ces � un syst�me externe.\n",												// French
											"Drucker: Kopie eines Artenreferenzbereichs drucken.\n\n"
											"Internes Archiv: Eine Kopie aller aktiven Referenzbereiche f�r sp�tere Verwendung intern im Analyseger�t speichern.\n\n"
											"Externes Archiv: ASCII-Auszug eines Artenreferenzbereichs an ein externes System �bertragen.\n",																		// German
											"Impresora: imprimir una copia de un intervalo de referencia de especies.\n\n"
											"Archivo interno: guardar una copia de todos los intervalos de referencia activos internos para el analizador para usos futuros.\n\n"
											"Archivo externo: hacer un volcado de datos ASCII de un intervalo de referencia de especies a un sistema externo.\n",													// Spanish
											"Stampante: stampa una copia di un range di riferimento per specie.\n\n"
											"Archivio interno: salva una copia di tutti i range di riferimento attivi interni all'analizzatore per utilizzo futuro.\n\n"
											"Archivio esterno: trasmette una serie ASCII di range di riferimento per specie ad un sistema esterno.\n",																// Italian
											"Impressora: Imprime uma c�pia de um intervalo de refer�ncia de esp�cies.\n\n"
											"Ficheiro interno: Guardar uma c�pia de todos os intervalos de refer�ncia ativos internos no analisador para utiliza��o futura.\n\n"
											"Ficheiro externo: Transmitir um dump de ASCII de um intervalo de refer�ncia de esp�cies para um sistema externo.\n",													// Portuguese
											"Impressora: Imprima uma c�pia de um intervalo de refer�ncia de esp�cies.\n\n"
											"Arquivo interno: Salve uma c�pia de todos os intervalos de refer�ncia ativos internos no analisador, para futura recupera��o.\n\n"
											"Arquivo externo: Transmita um despejo ASCII de um intervalo de refer�ncia de esp�cies para um sistema externo.\n"														// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_ADD			,	{	"Species n: The next available generic species label between 1 and 99.\n\n"
											"Control n: The next available control label between 1 and 99.\n\n"
											"Select: Provides a list of common species labels to select from.\n",																// English
											"Esp�ce n : la prochaine �tiquette d'esp�ce g�n�rique disponible comprise entre 1 et 99.\n\n"
											"Contr�le n : la prochaine �tiquette de contr�le disponible comprise entre 1 et 99.\n\n"
											"S�lectionner : fournit une liste d'�tiquettes d'esp�ces communes � partir de laquelle la s�lection pourra �tre effectu�e.\n",		// French
											"Arten n: Das n�chste verf�gbare generische Artenetikett zwischen 1 und 99.\n\n"
											"Steuerung n: Das n�chste verf�gbare Steuerungsetikett zwischen 1 und 99.\n\n"
											"Ausw�hlen: Stellt eine Liste �blicher Artenetiketten zur Auswahl.\n",																// German
											"Especie n: la pr�xima etiqueta de especie gen�rica disponible entre 1 y 99.\n\n"
											"Control n: la pr�xima etiqueta de control disponible entre 1 y 99.\n\n"
											"Seleccionar: proporciona una lista para selecci�n de etiquetas de especies comunes.\n",											// Spanish
											"Specie n: la successiva etichetta di specie generica disponibile tra 1 e 99.\n\n"
											"Controllo n: la successiva etichetta di controllo disponibile tra 1 e 99.\n\n"
											"Seleziona: fornisce un elenco delle etichette delle specie comuni tra cui scegliere.\n",											// Italian
											"Esp�cies n: A pr�xima etiqueta de esp�cies gen�rica dispon�vel entre 1 e 99.\n\n"
											"Controlo n: A pr�xima etiqueta de controlo dispon�vel entre 1 e 99.\n\n"
											"Selecionar: Fornece uma lista para sele��o de etiquetas de esp�cies frequentes.\n",												// Portuguese
											"N esp�cies: O pr�ximo r�tulo de esp�cie gen�rico dispon�vel entre 1 e 99.\n\n"
											"N controle: O pr�ximo r�tulo de controle dispon�vel entre 1 e 99.\n\n"
											"Selecionar: Fornece uma lista de r�tulos de esp�cies comuns para sele��o.\n"														// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_MODIFY		,	{	"Analyte: Provides a list of all active reference ranges for a particular analyte.\n\n"
											"Species: Provides a list of all analytes for all reference ranges.\n",																// English
											"Substance � analyser : fournit une liste de toutes les fourchettes de r�f�rence actives pour une substance � analyser donn�e.\n\n"
											"Esp�ce : fournit une liste de toutes les substances � analyser pour toutes les fourchettes de r�f�rence.\n",						// French
											"Analyt: Anzeige einer Liste aller aktiven Referenzbereiche f�r ein bestimmtes Analyt.\n\n"
											"Arten: Anzeige einer Liste aller Analyte f�r alle Referenzbereiche.\n",															// German
											"Analito: proporciona una lista de todos los intervalos de referencia activos para un analito particular.\n\n"
											"Especie: proporciona una lista de todos los analitos para todos los intervalos de referencia.\n",									// Spanish
											"Analita: fornisce un elenco di tutti i range di riferimento attivi per un particolare analita.\n\n"
											"Specie: fornisce un elenco di tutti gli analiti per ogni range di riferimento.\n",													// Italian
											"Analito: Fornece uma lista de todos os intervalos de referencia ativos para um analito espec�fico.\n\n"
											"Esp�cies: Fornece uma lista de todos os analitos para todos os intervalos de refer�ncia.\n",										// Portuguese
											"Analito: Fornece uma lista de todos os intervalos de refer�ncia ativos para um determinado analito.\n\n"
											"Esp�cies: Fornece uma lista de todos os analitos para todos os intervalos de refer�ncia.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_REMOVE		,	{	"Removing a species from the sample type list will remove the item from the list of species shown when running a rotor, and any values entered will be permanently lost.\n",																	// English
											"La suppression d'une esp�ce dans la liste de type d'�chantillons a pour effet de supprimer cet �l�ment dans la liste d'esp�ces affich�es lors de l'ex�cution d'un rotor, et toutes les valeurs saisies seront irr�m�diablement perdues.\n",	// French
											"Die Entfernung einer Art aus der Probentypliste entfernt das Objekt aus der Artenliste, die bei laufendem Rotor angezeigt wird, und s�mtliche eingegebene Werte gehen dauerhaft verloren.\n",													// German
											"Al eliminar una especie de la lista del tipo de muestra se eliminar� la entrada de la lista de especies mostrada al usar el rotor, y cualquier valor introducido se perder� permanentemente.\n",												// Spanish
											"Rimuovendo una specie dall'elenco dei tipi si rimuover� quella voce dall'elenco di specie proposto quando un rotore � in funzione e tutti i valori immessi saranno persi in modo permanente.\n",												// Italian
											"Ao eliminar uma esp�cie da lista de tipo de amostra ser� eliminado o item da lista de esp�cies exibida quando se utiliza um rotor, e qualquer valor introduzido ser� permanentemente perdido.\n",												// Portuguese
											"A remo��o de uma esp�cie da lista de tipos de amostra remover� o item da lista de esp�cie mostrada durante a execu��o de um rotor e quaisquer valores inseridos ser�o permanentemente perdidos.\n"												// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_ENZYME_UNITS			,	{	"Selecting a unit on this screen will change the units for ALT, ALP, AMY, AST, CK and GGT  simultaneously to the same units.\n",					// English
											"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � ALT, ALP, AMY, AST, CK et GGT.\n",												// French
											"Mit der Auswahl einer Einheit in diesem Men� werden automatisch die Einheiten f�r ALT, ALP, AMY, AST, CK und GGT auf diese Einheit umgestellt.\n",	// German
											"Al seleccionar una unidad en esta pantalla, se cambiar� ALT, ALP, AMY, AST, CK y GGT simult�neamente a la misma unidad.\n",						// Spanish
											"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per ALT, ALP, AMY, AST, CK e GGT nelle stesse unit�.\n",			// Italian
											"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades ALT, ALP, AMY, AST, CK e GGT para as mesmas unidades.\n",		// Portuguese
											"A sele��o de uma unidade nessa tela mudar� as unidades para ALT, ALP, AMY, AST, CK e GGT simultaneamente para as mesmas unidades.\n"				// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_LIPID_UNITS			,	{	"Selecting a unit on this screen will change the units for CHOL.\n",						// English
											"L'unit� s�lectionn�e dans cet �cran sera appliqu�e � CHOL.\n",								// French
											"Mit der Auswahl einer Einheit in diesem Men� wird die Einheit f�r CHOL umgestellt.\n",		// German
											"Al seleccionar una unidad en esta pantalla, se cambiar� la unidad de COL.\n",				// Spanish
											"La selezione di un'unit� in questa schermata modifica le unit� per COL.\n",				// Italian
											"Ao selecionar uma unidade neste ecr�, ser�o alteradas as unidades CHOL.\n",				// Portuguese
											"A sele��o de uma unidade nessa tela mudar� as unidades para CHOL.\n"						// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_PROTEIN_UNITS		,	{	"Selecting a unit on this screen will change the units for ALB, GLOB and TP simultaneously to the same units.\n",									// English
											"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � ALB, GLOB et TP.\n",															// French
											"Mit der Auswahl einer Einheit in diesem Men� werden automatischdie Einheiten f�r ALB, GLOB und TP auf diese Einheit umgestellt.\n",				// German
											"Al seleccionar una unidad en esta pantalla, se cambiar� ALB, GLOB y TP simult�neamente a la misma unidad.\n",										// Spanish
											"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per ALB e GLOB nelle stesse unit�.\n",								// Italian
											"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades ALB, GLOB e TP para as mesmas unidades.\n",						// Portuguese
											"A sele��o de uma unidade nessa mudar� as unidades para ALB, GLOB e TP simultaneamente para as mesmas unidades.\n"									// Portuguese (Brazil)
										} 
	},
	
// Error screens							ENGLISH,							FRENCH,									GERMAN,								SPANISH,											ITALIAN, 									PORTUGUESE,								PORTUGUESE (BRAZIL)
	{ ESTR_ILLEGAL_ROTOR_TYPE		,	{	"Illegal Rotor Type",				"Type rotor non autoris�",				"Falscher Rotortyp",				"Tipo de rotor ilegal",								"Tipo rotore illegale",						"Tipo de rotor inv�lido",				"Tipo de rotor ilegal"	}	},
	{ ESTR_NEW_ROTOR_TYPE			,	{	"New Rotor Type",					"Nouveau type de rotor",				"Neuer Rotortyp",					"Nuevo tipo de rotor",								"Nuovo tipo rotore",						"Tipo novo de rotor",					"Novo tipo de rotor"	}	},
	{ ESTR_PLEASE_REMOVE_ROTOR		,	{	"Please Remove Rotor",				"Retirer le rotor",						"Bitte Rotor entfernen",			"Retire el rotor",									"Si prega di rimuovere il rotore",			"Retirar rotor",						"Remova o rotor"	}	},
	{ ESTR_ROTOR_DATE_ILLEGAL		,	{	"Rotor Date Illegal",				"Date rotor non autoris�e",				"Falsches Rotorendatum",			"Fecha de rotor ilegal",							"Data rotore illegale",						"Data do rotor inv�lida",				"Data do rotor ilegal"	}	},
	{ ESTR_ROTOR_DATE_IMPROPER		,	{	"Rotor Date Improper",				"Date rotor incorrecte",				"Unzul�ssiges Rotorendatum",		"Fecha rotor no adecuada",							"Data rotore impropria",					"Data do rotor inadequada",				"Data do rotor impr�pria"	}	},
	{ ESTR_ROTOR_EXPIRED			,	{	"Rotor Expired",					"Expiration rotor",						"Abgelaufener Rotor",				"Rotor caducado",									"Rotore scaduto",							"Rotor expirado",						"Rotor expirado"	}	},
	{ ESTR_UPDATED_ROTOR_TYPE		,	{	"Updated Rotor Type",				"Type de rotor actualis�",				"Aktualisierter Rotortyp",			"Tipo de rotor actualizado",						"Tipo rotore aggiornato",					"Tipo de rotor atualizado",				"Tipo de rotor atualizado"	}	},

// Warning screens							ENGLISH,							FRENCH,									GERMAN,								SPANISH,											ITALIAN, 									PORTUGUESE,								PORTUGUESE (BRAZIL)
	{ ESTR_ROTOR_DETECTED			,	{	"Rotor detected during warming",	"Rotor d�tect� en cours de chauffage",	"Rotor beim Aufw�rmen vorgefunden",	"Rotor detectado durante el calentamiento inicial",	"Rotore individuato durante riscaldamento",	"Rotor detetado durante o aquecimento",	"Rotor detectado durante o aquecimento"	}	},

// Error info text
	{ EISTR_AMBIENT_TEMP			,	{	"The temperature outside the analyzer is either too hot or too cold to run a rotor. Contact Technical Service for assistance.\n",															// English
											"La temp�rature � l'ext�rieur de l'analyseur est trop �lev�e ou trop faible pour permettre le fonctionnement d'un rotor. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Die Temperatur au�erhalb des Analyseger�ts ist f�r den Rotorenbetrieb zu hei� oder zu kalt. Wenden Sie sich an den technischen Service.\n",												// German
											"La temperatura fuera del analizador es demasiado alta o demasiado baja para usar el rotor. P�ngase en contacto con el servicio t�cnico.\n",												// Spanish
											"La temperatura esterna all'analizzatore � troppo alta o troppo bassa per permettere il funzionamento di un rotore. Per assistenza contattare il servizio tecnico.\n",						// Italian
											"A temperatura fora do analisador � demasiado alta ou demasiado baixa para executar um rotor. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",							// Portuguese
											"A temperatura externa do analisador est� muito alta ou muito baixa para executar um rotor. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_ANALYZER_DATE			,	{	"Date in analyzer is earlier than that of the rotor being run. Check dates in analyzer and rerun with new rotor.\n",											// English
											"La date de l'analyseur est ant�rieure � celle du rotor en cours d'utilisation. V�rifier les dates sur l'analyseur et r�ex�cuter le nouveau rotor.\n",			// French
											"Das Datum im Analyseger�t liegt vor dem des Rotorenbetriebs. Pr�fen Sie das Datum im Analyseger�t und nehmen Sie den Betrieb mit einem neuen Rotor auf.\n",	// German
											"La fecha del analizador es anterior a la fecha del rotor que se va a usar. Compruebe las fechas del analizador y vuelva a usar el nuevo rotor.\n",				// Spanish
											"La data dell'analizzatore � precedente a quella di attivazione del rotore. Controllare le date dell'analizzatore e riavviare con un nuovo rotore.\n",			// Italian
											"A data do analisador � anterior � data do rotor que foi iniciado. Verifique as datas do analisador e reinicie com um novo rotor.\n",							// Portuguese
											"A data no analisador � anterior � do rotor em execu��o. Verifique as datas no analisador e execute novamente com um novo rotor.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE					,	{	"The analyzer was unable to read the rotor's barcode. If symptom continues, contact Technical Service.\n",														// English
											"L'analyseur n'a pas pu lire le code � barres du rotor. En cas de persistance des sympt�mes, contacter le service technique.\n",								// French
											"Das Analyseger�t konnte den Barcode des Rotors nicht lesen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",						// German
											"El analizador no pudo leer el c�digo de barras del rotor. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",			// Spanish
											"L'analizzatore non � riuscito a leggere il codice a barre del rotore. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"O analisador n�o consegue ler o c�digo de barras do rotor. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",								// Portuguese
											"O analisador n�o foi capaz de ler o c�digo de barras do rotor. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE_FORMAT			,	{	"The analyzer did not recognize the rotor. Contact Technical Service for further assistance.\n",						// English
											"L'analyseur n'a pas reconnu le rotor. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											"Das Analyseger�t konnte den Rotor nicht erkennen. Wenden Sie sich an den technischen Service.\n",						// German
											"El analizador no ha reconocido el rotor. P�ngase en contacto con el servicio t�cnico.\n",								// Spanish
											"L'analizzatore non ha riconosciuto il rotore. Per ulteriore assistenza contattare il servizio tecnico.\n",				// Italian
											"O analisador n�o consegue reconhecer o rotor. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											"O analisador n�o reconheceu o rotor. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MISSING			,	{	"The analyzer did not detect all the chemistry beads in the rotor. Rerun sample using new rotor.\n",											// English
											"L'analyseur n'a pas d�tect� toutes les billes chimiques dans le rotor. R�ex�cuter l'�chantillon � l'aide d'un nouveau rotor.\n",				// French
											"Das Analyseger�t konnte nicht alle chemischen Beads in dem Rotor erkennen. Wiederholen Sie die Probe mit einem neuen Rotor.\n",				// German
											"El analizador no detect� todas las cuentas qu�micas en el rotor. Vuelva a utilizar la muestra usando un nuevo rotor.\n",						// Spanish
											"L'analizzatore non ha individuato tutti i granuli chimici nel rotore. Riavviare il campione con un nuovo rotore.\n",							// Italian
											"O analisador n�o detetou todas as esferas de subst�ncias qu�micas no rotor. Executar novamente a amostra utilizando um rotor novo.\n",			// Portuguese
											"O analisador n�o detectou todas as esferas de subst�ncia qu�mica no rotor. Execute novamente a amostra usando o novo rotor.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MIX				,	{	"The analyzer will not be able to determine the exposure status if the QC beads have not mixed correctly. Rerun with a new rotor.\n",									// English
											"L'analyseur ne parviendra pas � d�terminer l'�tat d'exposition si les billes QC ne sont pas correctement m�lang�es. R�ex�cuter avec un nouveau rotor.\n",				// French
											"Das Analyseger�t kann den Status der Einwirkung nicht feststellen, wenn die QC-Beads nicht ordnungsgem�� vermischt wurden. Mit einem neuen Rotor wiederholen.\n",		// German
											"El analizador no ser� capaz de determinar el estado de exposici�n si las cuentas QC no se mezclan correctamente. Debe usarse con un nuevo rotor.\n",					// Spanish
											"L'analizzatore non potr� determinare lo stato di esposizione se i granuli di QC non sono stati mescolati correttamente. Riavviare con un nuovo rotore.\n",				// Italian
											"O analisador n�o conseguir� determinar o estado de exposi��o se as esferas de CQ n�o forem misturadas adequadamente. Executar novamente utilizando um rotor novo.\n",	// Portuguese
											"O analisador n�o poder� determinar o status de exposi��o se as esferas de QC n�o tiverem sido misturadas corretamente. Execute novamente com um novo rotor.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_CUVETTE_MARK			,	{	"The analyzer was unable to detect the rotor. If symptom continues, contact Technical Service.\n",												// English
											"L'analyseur n'est pas parvenu � d�tecter le rotor. En cas de persistance des sympt�mes, contacter le service technique.\n",					// French
											"Das Analyseger�t konnte den Rotor nicht finden. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",					// German
											"El analizador no fue capaz de detectar el rotor. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"L'analizzatore non � riuscito a individuare il rotore. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"O analisador n�o consegue detetar o rotor. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",								// Portuguese
											"O analisador n�o foi capaz de detectar o rotor. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_EXPIRED_ROTOR			,	{	"The rotor has expired and should not be used. Contact Technical Service for further details.\n",												// English
											"La date de p�remption du rotor est d�pass�e. Ne pas l'utiliser. Pour obtenir plus d'informations, contacter le service technique.\n",			// French
											"Der Rotor ist abgelaufen und sollte ersetzt werden. Wenden Sie sich an den technischen Service.\n",											// German
											"El rotor ha caducado y no deber�a utilizarse. P�ngase en contacto con el servicio t�cnico.\n",													// Spanish
											"Il rotore � scaduto e non dovrebbe essere usato. Per ulteriore dettagli contattare il servizio tecnico.\n",									// Italian
											"O rotor expirou e n�o deveria ser utilizado. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",						// Portuguese
											"O rotor expirou e n�o deve ser usado. Entre em contato com o Servi�o T�cnico para obter detalhes adicionais.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_DILUENT	,	{	"Insufficient reagents exist in the rotor to complete the run. Rerun sample with new rotor.\n",													// English
											"La quantit� de r�actifs dans le rotor est insuffisante pour ex�cuter la proc�dure. R�ex�cuter l'�chantillon � l'aide du nouveau rotor.\n",		// French
											"In dem Rotor befinden sich nicht ausreichend Reagenzien, um den Vorgang abzuschlie�en. Wiederholen Sie die Probe mit einem neuen Rotor.\n",	// German
											"No hay suficientes reactivos en el rotor para completar su uso. Vuelva a utilizar la muestra con un nuevo rotor.\n",							// Spanish
											"Nel rotore non vi sono sufficienti reagenti per completare l'operazione. Riavviare il campione con un nuovo rotore.\n",						// Italian
											"N�o existem reagentes suficientes no rotor para concluir a execu��o. Executar novamente a amostra utilizando um rotor novo.\n",				// Portuguese
											"Existem reagentes insuficientes no rotor para concluir a execu��o. Execute novamente a amostra com o novo rotor.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_SAMPLE		,	{	"The rotor has not received enough sample to function properly. Rerun using a new rotor.\n",													// English
											"Le rotor n'a pas re�u suffisamment d'�chantillon pour fonctionner correctement. R�ex�cuter avec un nouveau rotor.\n",							// French
											"Der Rotor enth�lt keine ausreichende Probenmenge f�r eine ordnungsgem��e Funktion. Mit einem neuen Rotor wiederholen.\n",						// German
											"El rotor no ha recibido suficiente muestra para funcionar correctamente. �selo con un nuevo rotor.\n",											// Spanish
											"Il rotore non ha ricevuto campione sufficiente per funzionare nel modo corretto. Riavviare utilizzando un nuovo rotore.\n",					// Italian
											"O rotor n�o recebeu amostra suficiente para funcionar adequadamente. Executar novamente utilizando um rotor novo.\n",							// Portuguese
											"O rotor n�o recebeu amostra suficiente para funcionar corretamente. Execute novamente usando um novo rotor.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_IQC						,	{	"The analyzer Intelligent Quality Control (iQC) reagent dye beads differ too much from the expected values. Rerun using a new rotor.\n",												// English
											"Les billes de teinture du r�actif Intelligent Quality Control (iQC) divergent trop des valeurs pr�vues. R�ex�cuter avec un nouveau rotor.\n",											// French
											"Die Intelligent Quality Control (iQC) Reagenz-Farbstoff-Beads des Analyseger�ts weichen zu stark von den erwarteten Werten ab. Mit einem neuen Rotor wiederholen.\n",					// German
											"Las cuentas de tinte de reactivo Intelligent Quality Control (iQC) del analizador son muy distintas a los valores esperados. �selo con un nuevo rotor.\n",								// Spanish
											"I granuli di colore reagente Intelligent Quality Control (iQC) dell'analizzatore sono troppo diversi dai valori previsti. Riavviare utilizzando un nuovo rotore.\n",					// Italian
											"As esferas de reagente corante do Controlo de qualidade inteligente (iQC) do analisador s�o muito diferentes dos valores esperados. Executar novamente utilizando um rotor novo.\n",	// Portuguese
											"As esferas do corante reagente de Controle de qualidade inteligente (iQC) do analisador diferem muito dos valores esperados. Execute novamente usando um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_REAGENT_DISTRIBUTION	,	{	"Reagent beads have not dissolved as expected. Rerun sample with a new rotor.\n",											// English
											"Les billes de r�actif ne se sont pas dissoutes comme pr�vu. R�ex�cuter l'�chantillon � l'aide d'un nouveau rotor.\n",		// French
											"Reagenz-Beads wurden nicht erwartungsgem�� aufgel�st. Wiederholen Sie die Probe mit einem neuen Rotor.\n",					// German
											"Las cuentas de reactivo no se han disuelto como se esperaba. Vuelva a utilizar la muestra con un nuevo rotor.\n",			// Spanish
											"I granuli di reagente non si sono dissolti come previsto. Riavviare il campione con un nuovo rotore.\n",					// Italian
											"As esferas de reagente n�o se dissolveram conforme esperado. Executar novamente a amostra utilizando um rotor novo.\n",	// Portuguese
											"As esferas de reagente n�o se dissolveram conforme esperado. Execute novamente a amostra com um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_ROTOR_DETECTED			,	{	"A rotor may be in the analyzer and may prevent the analyzer from starting normally. Press Continue to remove the rotor.\n",								// English
											"Il peut y avoir dans l'analyseur un rotor susceptible de l'emp�cher de d�marrer normalement. Appuyer sur Continuer pour retirer le rotor.\n",				// French
											"Ein Rotor kann sich in dem Analyseger�t befinden und den Startvorgang des Analyseger�ts beeintr�chtigen. Zur Entfernung des Rotors dr�cken Sie Weiter.\n",	// German
											"Puede que haya un rotor en el analizador, lo que impide que el analizador se inicie correctamente. Pulse en Continuar para retirar el rotor.\n",			// Spanish
											"Nell'analizzatore potrebbe essere rimasto un rotore che impedisce un'attivazione normale dell'analizzatore. Premere Continua per rimuovere il rotore.\n", 	// Italian
											"Pode estar um rotor no analisador, o que pode impedir que o analisador inicie corretamente. Prima Continuar para remover o rotor.\n",						// Portuguese
											"Um rotor pode estar no analisador e impedir que o analisador inicie normalmente. Pressione Continuar para remover o rotor.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_RQC						,	{	"The Reagent QC beads recovered below the limit. The rotor may not have been stored correctly. Rerun with a new rotor.\n",													// English
											"La r�cup�ration des billes de r�actif QC est inf�rieure � la limite. Il se peut que le rotor n'ait pas �t� rang� correctement. R�ex�cuter avec un nouveau rotor.\n",		// French
											"Die wiederhergestellten Reagenz-QC-Beads liegen unter dem Grenzwert. Der Rotor wurde m�glicherweise nicht ordnungsgem�� gelagert. Mit einem neuen Rotor wiederholen.\n",	// German
											"Cuentas QC de reactivo recuperadas por debajo del l�mite. Puede que el rotor no se haya guardado correctamente. Debe usarse con un nuevo rotor.\n",						// Spanish
											"I granuli QC di reagente si sono ristabiliti sotto al limite. Il rotore potrebbe non essere stato conservato correttamente. Riavviare con un nuovo rotore.\n",			   	// Italian
											"As esferas de reagente de CQ foram recuperadas abaixo do limite. O rotor pode n�o ter sido guardado corretamente. Executar novamente utilizando um rotor novo.\n",			// Portuguese
											"As esferas de Reagente de QC se recuperaram abaixo do limite. O rotor pode n�o ter sido armazenado corretamente. Execute novamente com um novo rotor.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_BLANK			,	{	"The sample blank bead appears to be turbid. This can be due to sample quality. Confirm sample integrity and rerun with a new rotor.\n",														// English
											"La bille vierge de l'�chantillon semble trouble, ce qui peut venir de la qualit� de l'�chantillon. V�rifier l'int�grit� de l'�chantillon et r�ex�cuter avec un nouveau rotor.\n",				// French
											"Die Leer-Beadprobe ist tr�be. Dies kann auf die Probenqualit�t zur�ckgef�hrt werden. Best�tigen Sie die Unversehrtheit der Probe und wiederholen Sie den Vorgang mit einem neuen Rotor.\n",	// German
											"La cuenta virgen de la muestra parece estar turbia. Esto puede deberse a la calidad de la muestra. Confirme la integridad de la muestra y vuelva a utilizar el nuevo rotor.\n",				// Spanish
											"I granuli campione vuoti sembrano torbidi. Ci� pu� essere causato dalla qualit� del campione. Confermare l'integrit� del campione e riavviare con un nuovo rotore.\n",							// Italian
											"A esfera de amostra de branco parece estar turva. Isto pode ser devido � qualidade da amostra. Confirme a integridade da amostra e execute novamente utilizando um rotor novo.\n",				// Portuguese
											"A esfera em branco da amostra parece estar turva. Isso pode ser decorrente da qualidade da amostra. Confirme a integridade da amostra e execute novamente com um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_DISTRIBUTION		,	{	"Sample has not reached the chemistries. Rerun with a new rotor.\n",									// English
											"L'�chantillon n'est pas parvenu jusqu'aux produits chimiques. R�ex�cuter avec un nouveau rotor.\n",	// French
											"Die Probe hat nicht die Chemie erreicht. Mit einem neuen Rotor wiederholen.\n",						// German
											"La muestra no ha alcanzado los compuestos qu�micos. Debe usarse con un nuevo rotor.\n",				// Spanish
											"Il campione non ha raggiunto le sostanze chimiche. Riavviare con un nuovo rotore.\n",					// Italian
											"A amostra n�o atingiu as subst�ncias qu�micas. Executar novamente utilizando um rotor novo.\n",		// Portuguese
											"A amostra n�o atingiu as subst�ncias qu�micas. Execute novamente com um novo rotor.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_MIX				,	{	"The sample has not mixed with reagent materials sufficiently. Rerun with a new rotor.\n",										// English
											"L'�chantillon ne s'est pas suffisamment m�lang� avec les mati�res de r�actifs. R�ex�cuter avec un nouveau rotor.\n",			// French
											"Die Probe hat sich nicht ausreichend mit Reagenzmaterialien vermischt. Mit einem neuen Rotor wiederholen.\n",					// German
											"La muestra no se ha mezclado suficientemente con los materiales reactivos. Debe usarse con un nuevo rotor.\n",					// Spanish
											"Il campione non si � mescolato a sufficienza con le sostanze reagenti. Riavviare con un nuovo rotore.\n",						// Italian
											"A amostra n�o se misturou suficientemente com os materiais dos reagentes. Executar novamente utilizando um rotor novo.\n",		// Portuguese
											"A amostra n�o foi misturada com os materiais reagentes suficientemente. Execute novamente com um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_QUALITY			,	{	"The sample may have one or more physical interferents. Confirm the sample integrity and rerun rotor.\n",										// English
											"Il se peut que l'�chantillon pr�sente un ou plusieurs �l�ments interf�rents. V�rifier l'int�grit� de l'�chantillon et r�ex�cuter le rotor.\n",	// French
											"Die Probe hat m�glicherweise eine oder mehrere physische st�rende Bestandteile. Unversehrtheit der Probe best�tigen und Rotor neu starten.\n",	// German
											"Puede que la muestra tenga una o varias interferencias f�sicas. Confirme la integridad de la muestra y vuelva a usar el rotor.\n",				// Spanish
											"Il campione potrebbe avere uno o pi� fattori di interferenza fisica. Confermare l'integrit� del campione e riavviare il rotore.\n",			// Italian
											"A amostra pode ter uma ou v�rias interfer�ncias f�sicas. Confirme a integridade da amostra e execute novamente o rotor.\n",					// Portuguese
											"A amostra pode ter um ou mais interferentes f�sicos. Confirme a integridade da amostra e execute novamente o rotor.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_SOFTWARE_UPDATE			,	{	"The rotor inserted requires a software update to operate correctly. Contact Technical Service for further assistance.\n",														// English
											"Le rotor ins�r� n�cessite une mise � jour de logiciel pour pouvoir fonctionner correctement. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											"Der eingesetzte Rotor erfordert ein Software-Update f�r einen ordnungsgem��en Betrieb. Wenden Sie sich an den technischen Service.\n",											// German
											"El rotor insertado necesita la actualizaci�n del software para operar correctamente. P�ngase en contacto con el servicio t�cnico.\n",											// Spanish
											"Il rotore inserito richiede un aggiornamento del software per funzionare correttamente. Per ulteriore assistenza contattare il servizio tecnico.\n",							// Italian
											"O rotor inserido necessita de uma atualiza��o do software para funcionar adequadamente. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",					// Portuguese
											"O rotor inserido requer uma atualiza��o de software para funcionar corretamente. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_STARTING_ABSORBANCE		,	{	"Starting absorbance of the sample blank bead is outside the limit, possibly due to an exogenous substance or distribution error. Rerun with new rotor.\n",																		// English
											"L'absorbance de d�part de la bille vierge de l'�chantillon n'entre pas dans les limites prescrites, �ventuellement du fait d'une substance exog�ne ou d'une erreur de distribution. R�ex�cuter � l'aide du nouveau rotor.\n",	// French
											"Die Startabsorbierung der Leer-Beadprobe befindet sich m�glicherweise aufgrund einer exogenen Substanz oder eines Verteilungsfehlers au�erhalb des Grenzwerts. Mit einem neuen Rotor wiederholen.\n",							// German
											"La absorbancia de inicio de la cuenta virgen de muestra est� fuera del l�mite, probablemente debido a una sustancia ex�gena o a un error de distribuci�n. Usar con un nuevo rotor.\n",											// Spanish
											"L'assorbenza iniziale dei granuli campione vuoti non rientra nei limiti, forse a causa di una sostanza esogena od a un errore di distribuzione. Riavviare il campione con un nuovo rotore.\n",									// Italian
											"A absorv�ncia inicial da esfera de amostra de branco est� fora do limite, possivelmente devido a uma sust�ncia ex�gena ou erro de distribui��o. Executar novamente utilizando um rotor novo.\n",								// Portuguese
											"A absor��o inicial da esfera em branco da amostra est� fora do limite, possivelmente devido a uma subst�ncia ex�gena ou um erro de distribui��o. Execute novamente com um novo rotor.\n"										// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_SENSOR				,	{	"The rotor temperature sensor is not operating. Contact Technical Service for further assistance.\n",										// English
											"Le d�tecteur de temp�rature du rotor ne fonctionne pas. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											"Der Temperatursensor des Rotors ist au�er Betrieb. Wenden Sie sich an den technischen Service.\n",											// German
											"El sensor de temperatura del rotor no est� funcionando. P�ngase en contacto con el servicio t�cnico.\n",									// Spanish
											"Il sensore di temperatura del rotore non � in funzione. Per ulteriore assistenza contattare il servizio tecnico.\n",						// Italian
											"O sensor de temperatura do rotor n�o est� a funcionar. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",				// Portuguese
											"O sensor de temperatura do rotor n�o est� funcionando. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_VARIATION			,	{	"The temperature deviated more than one degree during the run. The accuracy of certain chemistries may be affected. Rerun with new rotor.\n",															// English
											"La temp�rature a pr�sent� un �cart de plus d'un degr� au cours de l'ex�cution. La pr�cision de certains produits chimiques pourra en �tre affect�e. R�ex�cuter � l'aide du nouveau rotor.\n",			// French
											"Der Temperatur wich um mehr als ein Grad w�hrend des Betriebs ab. Die Genauigkeit bestimmter Chemie kann beeintr�chtigt worden sein. Mit einem neuen Rotor wiederholen.\n",							// German
											"La temperatura cambi� en m�s de un grado durante el an�lisis. Puede que afecte a la precisi�n del an�lisis de ciertas sustancias qu�micas. Utilice un nuevo rotor.\n",									// Spanish
											"Durante il funzionamento la temperatura � variata di pi� di un grado. La precisione di alcune sostanze chimiche potrebbe esserne stata influenzata. Riavviare il campione con un nuovo rotore.\n",		// Italian
											"A altera��o da temperatura foi superior a um grau durante a execu��o. A precis�o de determinadas sust�ncias qu�micas pode ser afetada. Executar novamente utilizando um rotor novo.\n",				// Portuguese
											"A temperatura desviou mais de um grau durante a execu��o. A precis�o de certas subst�ncias qu�micas pode estar afetada. Execute novamente com um novo rotor.\n"										// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMPERATURE				,	{	"The temperature of the rotor either varied too much or was not in the proper range during the run. If symptom continues, contact Technical Service.\n",																			// English
											"La temp�rature du rotor a soit vari� dans des proportions trop importantes, soit ne s'inscrit pas dans la fourchette prescrite en cours d'ex�cution. En cas de persistance des sympt�mes, contacter le service technique.\n",		// French
											"Die Rotortemperatur variierte zu stark oder befand sich w�hrend des Betriebs au�erhalb des optimalen Bereichs. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",											// German
											"La temperatura del rotor vari� mucho o no estaba en el intervalo adecuado durante el an�lisis. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",											// Spanish
											"Durante il funzionamento la temperatura del rotore ha subito variazioni eccessive oppure non era nel range adatto. Se i sintomi continuano contattare il servizio tecnico.\n",														// Italian
											"A temperatura do rotor variou muito ou n�o esteve dentro do intervalo adequado durante a an�lise. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",															// Portuguese
											"A temperatura do rotor variou muito ou n�o permaneceu no intervalo adequado durante a execu��o. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"																	// Portuguese (Brazil)
										} 
	},
	{ EISTR_WRONG_ROTOR				,	{	"The rotor inserted is not intended for this device. If symptom continues, contact Technical Service.\n",										// English
											"Le rotor ins�r� n'est pas pr�vu pour cet appareil. En cas de persistance des sympt�mes, contacter le service technique.\n",					// French
											"Der eingesetzte Rotor ist f�r dieses Ger�t nicht geeignet. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",			// German
											"El rotor insertado no es apto para este aparato. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											"Il rotore inserito non � adatto a questo strumento. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"O rotor inserido n�o � adequado para este dispositivo. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",					// Portuguese
											"O rotor inserido n�o � destinado a este dispositivo. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"						// Portuguese (Brazil)
										} 
	},
	
// Results (display and print)				ENGLISH,				FRENCH,					GERMAN,					SPANISH,					ITALIAN, 					PORTUGUESE,						PORTUGUESE (BRAZIL)
	{ RSTR_ROTOR_LOT_NUM			,	{	"Rotor Lot Number:",	"N� de lot rotor :",	"Rotor-Losnr.:",		"No. de lote del rotor:",	"Numero lotto rotore:",		"N�mero lote rotor:",			"N�m. de lote do rotor:"	}	},
	{ RSTR_DOV						,	{	"DOV:",					"DOV :",				"LI:",					"�l. vac.:",				"DV:",						"DOV:",							"Data vacina��o:"	}	},
	{ RSTR_KENNEL_NUM				,	{	"Kennel Number:",		"Num�ro chenil :",		"Zwingernr.:",			"No. de caseta:",			"Numero Kennel:",			"N�mero de Kennel:",			"N�mero do canil:"	}	},
	{ RSTR_OWNER_ID					,	{	"Owner ID:",			"ID prop.:",			"Besitzer-ID:",			"Id. del due�o:",			"ID propr.:",				"ID do propr.:",				"ID propriet.:"	}	},
	
// Genders									ENGLISH,	FRENCH,		GERMAN,			SPANISH,		ITALIAN, 	   	PORTUGUESE,		PORTUGUESE (BRAZIL)
	{ GSTR_FEMALE					,	{	"Female",	"Femelle",	"Weiblich",		"Hembra",		"Femmina",		"F�mea",		"Sexo feminino"	}	},
	{ GSTR_MALE						,	{	"Male",		"M�le",		"M�nnlich",		"Macho",		"Maschio",		"Macho",		"Sexo masculino"	}	},

// Species									ENGLISH,				FRENCH,					GERMAN,					SPANISH,				ITALIAN, 				PORTUGUESE,				PORTUGUESE (BRAZIL)	
	{ SSTR_OTHER					,	{	"Other",				"Autre",				"Sonstige",				"Otro",					"Altro",				"Outro",				"Outros"	}	},
	{ SSTR_SPECIES					,	{	"Species",				"Esp�ces",				"Arten",				"Especie",				"Specie",				"Esp�cies",				"Esp�cies"	}	},
	
	{ SSTR_CANINES					,	{	"Canines",				"Canins",				"Hunde",				"Caninos",				"Canini",				"Caninas",				"Caninos"	}	},
	{ SSTR_FELINES					,	{	"Felines",				"F�lins",				"Katzen",				"Felinos",				"Felini",				"Felinas",				"Felinos"	}	},
	{ SSTR_EQUINES					,	{	"Equines",				"Chevalins",			"Pferde",				"Equinos",				"Equini",				"Equinas",				"Equinos"	}	},
	{ SSTR_BOVINES					,	{	"Bovines",				"Bovins",				"Rinder",				"Bovinos",				"Bovini",				"Bovinas",				"Bovinos"	}	},
	{ SSTR_AVIANS					,	{	"Avians",				"Aviaires",				"V�gel",				"Aves",					"Aviari",				"Avi�rias",				"Aves"	}	},
	{ SSTR_REPTILES_AMPHIBIANS		,	{	"Reptiles/Amphibians",	"Reptiles/Amphibies",	"Reptilien/Amphibien",	"Reptiles/anfibios",	"Rettili/Anfibi",		"R�pteis/Anfibios",		"R�pteis/anf�bios"	}	},
	{ SSTR_SMALL_MAMMALS			,	{	"Small Mammals",		"Petits mammif�res",	"Kleins�ugetiere",		"Peque�os mam�feros",	"Piccoli mammiferi",	"Pequenos mam�feros",	"Mam�feros pequenos"	}	},
	{ SSTR_LARGE_MAMMALS			,	{	"Large Mammals",		"Grands mammif�res",	"Gro�s�ugetiere",		"Grandes mam�feros",	"Grandi mammiferi",		"Grandes mam�feros",	"Mam�feros grandes"	}	},

	// Canines
	{ SSTR_DOG						,	{	"Dog",					"Chien",				"Hund",					"Perro",				"Cane",					"C�o",					"C�o"	}	},
	{ SSTR_COYOTE					,	{	"Coyote",				"Coyote",				"Kojote",				"Coyote",				"Coyote",				"Coiote",				"Coiote"	}	},
	{ SSTR_DINGO					,	{	"Dingo",				"Dingo",				"Dingo",				"Dingo",				"Dingo",				"Dingo",				"C�o selvagem"	}	},
	{ SSTR_FOX						,	{	"Fox",					"Renard",				"Fuchs",				"Zorro",				"Volpe",				"Raposa",				"Raposa"	}	},
	{ SSTR_JACKAL					,	{	"Jackal",				"Chacal",				"Schakal",				"Chacal",				"Sciacallo",			"Chacal",				"Chacal"	}	},
	{ SSTR_WOLF						,	{	"Wolf",					"Loup",					"Wolf",					"Lobo",					"Lupo",					"Lobo",					"Lobo"	}	},

	// Felines
	{ SSTR_CAT						,	{	"Cat",					"Chat",					"Katze",				"Gato",					"Gatto",				"Gato",					"Gato"	}	},
	{ SSTR_BOBCAT					,	{	"Bobcat",				"Lynx roux",			"Rotluchs",				"Lince rojo",			"Lince rossa",			"Lince vermelho",		"Lince"	}	},
	{ SSTR_CHEETAH					,	{	"Cheetah",				"Gu�pard",				"Gepard",				"Guepardo",				"Ghepardo",				"Chita",				"Gato selvagem"	}	},
	{ SSTR_JAGUAR					,	{	"Jaguar",				"Jaguar",				"Jaguar",				"Jaguar",				"Giaguaro",				"Jaguar",				"Jaguar"	}	},
	{ SSTR_LEOPARD					,	{	"Leopard",				"L�opard",				"Leopard",				"Leopardo",				"Leopardo",				"Leopardo",				"Leopardo"	}	},
	{ SSTR_LION						,	{	"Lion",					"Lion",					"L�we",					"Le�n",					"Leone",				"Le�o",					"Le�o"	}	},
	{ SSTR_LYNX						,	{	"Lynx",					"Lynx",					"Luchs",				"Lince",				"Lince",				"Lince",				"Gato selvagem"	}	},
	{ SSTR_PANTHER					,	{	"Panther",				"Panth�re",				"Panther",				"Pantera",				"Pantera",				"Pantera",				"Pantera"	}	},
	{ SSTR_PUMA						,	{	"Puma",					"Puma",					"Puma",					"Puma",					"Puma",					"Puma",					"Puma"	}	},
	{ SSTR_TIGER					,	{	"Tiger",				"Tigre",				"Tiger",				"Tigre",				"Tigre",				"Tigre",				"Tigre"	}	},
	
	// Equines
	{ SSTR_HORSE					,	{	"Horse",				"Cheval",				"Pferd",				"Caballo",				"Cavallo",				"Cavalo",				"Cavalo"	}	},
	{ SSTR_DONKEY					,	{	"Donkey",				"Ane",					"Esel",					"Burro",				"Asino",				"Burro",				"Asno"	}	},
	{ SSTR_PONY						,	{	"Pony",					"Poney",				"Pony",					"Poni",					"Pony",					"P�nei",				"P�nei"	}	},
	{ SSTR_ZEBRA					,	{	"Zebra",				"Z�bre",				"Zebra",				"Cebra",				"Zebra",				"Zebra",				"Zebra"	}	},
	
	// Bovines
	{ SSTR_COW						,	{	"Cow",					"Vache",				"Kuh",					"Vaca",					"Mucca",				"Vaca",					"Vaca"	}	},
	{ SSTR_WATER_BUFFALO			,	{	"Water Buffalo",		"Buffle (d'Asie)",		"Wasserb�ffel",			"B�falo de agua",		"Bufalo d'acqua",		"B�falo de �gua",		"B�falo d��gua"	}	},
	{ SSTR_STEER					,	{	"Steer",				"Bouvillon",			"Stier",				"Novillo",				"Manzo",				"Boi",					"Novilho"	}	},
	{ SSTR_OX						,	{	"Ox",					"Boeuf",				"Ochse",				"Buey",					"Bue",					"Boi",					"Boi"	}	},
	{ SSTR_BULL						,	{	"Bull",					"Taureau",				"Bulle",				"Toro",					"Toro",					"Touro",				"Touro"	}	},
	{ SSTR_BUFFALO					,	{	"Buffalo",				"Buffle",				"B�ffel",				"B�falo",				"Bufalo",				"B�falo",				"B�falo"	}	},
	{ SSTR_BISON					,	{	"Bison",				"Bison",				"Bison",				"Bisonte",				"Bisonte",				"Bisonte",				"Bis�o"	}	},
	
	// Avians
	{ SSTR_BIRD						,	{	"Bird",					"Oiseau",				"Vogel",				"P�jaro",				"Uccello",				"P�ssaro",				"P�ssaro"	}	},
	{ SSTR_AFRICAN_GRAY				,	{	"African Gray",			"Perroquet gris",		"Afrik.Graupapagei",	"Rucio africano",		"Pap. grigio afr.",		"Papagaio africano",	"Papagaio africano"	}	},
	{ SSTR_AMAZON					,	{	"Amazon",				"Amazone",				"Amazone",				"Amazona",				"Amazzoni",				"Amazonas",				"Amazona"	}	},
	{ SSTR_CHICKEN					,	{	"Chicken",				"Poulet",				"Huhn",					"Pollo",				"Pollo",				"Galinha",				"Frango"	}	},
	{ SSTR_COCKATIEL				,	{	"Cockatiel",			"Calopsitte �l�gte",	"Nymphensittich",		"Carolina",				"Cockatiel",			"Caturra",				"Calopsita"	}	},
	{ SSTR_COCKATOO					,	{	"Cockatoo",				"Cacato�s",				"Kakadu",				"Cacat�a",				"Cacatua",				"Catatua",				"Cacatua"	}	},
	{ SSTR_CONURE					,	{	"Conure",				"Conure",				"Sittiche",				"Lorito americano",		"Conuro",				"Conure",				"Jandaia"	}	},
	{ SSTR_DOVE						,	{	"Dove",					"Colombe",				"Taube",				"Paloma",				"Colombo",				"Pomba",				"Pombo"	}	},
	{ SSTR_DUCK						,	{	"Duck",					"Canard",				"Ente",					"Pato",					"Anatra",				"Pato",					"Pato"	}	},
	{ SSTR_FLAMINGO					,	{	"Flamingo",				"Flamand",				"Flamingo",				"Flamenco",				"Fenicottero",			"Flamingo",				"Flamingo"	}	},
	{ SSTR_GOOSE					,	{	"Goose",				"Oie",					"Gans",					"Ganso",				"Oca",					"Ganso",				"Ganso"	}	},
	{ SSTR_LOVEBIRD					,	{	"Lovebird",				"Ins�parable",			"Erdbeerkopf",			"Inseparable",			"Inseparabile",			"Agapornis",			"P�ssaro-do-amor"	}	},
	{ SSTR_MACAW					,	{	"Macaw",				"Ara",					"Ara",					"Guacamayo",			"Ara",					"Arara",				"Arara"	}	},
	{ SSTR_MYNAH					,	{	"Mynah",				"Mainate",				"Beo",					"Gr�cula religiosa",	"Storno triste",		"Main�",				"Main�"	}	},
	{ SSTR_OSTRICH					,	{	"Ostrich",				"Autruche",				"Strau�",				"Avestruz",				"Struzzo",				"Avestruz",				"Avestruz"	}	},
	{ SSTR_PARAKEET					,	{	"Parakeet",				"Starique perroq.",		"Wellensittich",		"Periquito",			"Parrocchetto",			"Periquito",			"Periquito"	}	},
	{ SSTR_PARROT					,	{	"Parrot",				"Perroquet",			"Papagei",				"Loro",					"Pappagallo",			"Papagaio",				"Papagaio"	}	},
	{ SSTR_TOUCAN					,	{	"Toucan",				"Toucan",				"Tukan",				"Tuc�n",				"Tucano",				"Tucano",				"Tucano"	}	},
	
	// Reptiles/amphibians
	{ SSTR_REPTILE					,	{	"Reptile",				"Reptile",				"Reptil",				"Reptil",				"Rettile",				"R�ptil",				"R�ptil"	}	},
	{ SSTR_ALLIGATOR				,	{	"Alligator",			"Alligator",			"Alligator",			"Caim�n",				"Alligatore",			"Alig�tor",				"Jacar�"	}	},
	{ SSTR_AMPHIBIAN				,	{	"Amphibian",			"Amphibie",				"Amphibie",				"Anfibio",				"Anfibio",				"Anf�bio",				"Anf�bio"	}	},
	{ SSTR_BEARDED_DRAGON			,	{	"Bearded Dragon",		"L�zard � barbe",		"Bartagame",			"Drag�n barbado",		"Pogona vitticeps",		"Drag�o barbudo",		"Drag�o farpado"	}	},
	{ SSTR_BOA_CONSTRICTOR			,	{	"Boa Constrictor",		"Boa constrictor",		"Boa constrictor",		"Boa constrictor",		"Boa Costrittore",		"Jib�ia",				"Jiboia"	}	},
	{ SSTR_CHAMELEON				,	{	"Chameleon",			"Cam�l�on",				"Cham�leon",			"Camale�n",				"Camaleonte",			"Camale�o",				"Camale�o"	}	},
	{ SSTR_CROCODILE				,	{	"Crocodile",			"Crocodile",			"Krokodil",				"Cocodrilo",			"Coccodrillo",			"Crocodilo",			"Crocodilo"	}	},
	{ SSTR_FROG						,	{	"Frog",					"Grenouille",			"Frosch",				"Rana",					"Rana",					"R�",					"R�"	}	},
	{ SSTR_GECKO					,	{	"Gecko",				"Gecko",				"Gecko",				"Lagartija",			"Geco",					"Osga",					"Lagartixa"	}	},
	{ SSTR_IGUANA					,	{	"Iguana",				"Iguane",				"Leguan",				"Iguana",				"Iguana",				"Iguana",				"Iguana"	}	},
	{ SSTR_LIZARD					,	{	"Lizard",				"L�zard",				"Eidechse",				"Lagarto",				"Lucertola",			"Lagarto",				"Lagarto"	}	},
	{ SSTR_MONITOR					,	{	"Monitor",				"Varan",				"Waran",				"Varano",				"Varano",				"Monitor",				"Lagarto monitor"	}	},
	{ SSTR_PYTHON					,	{	"Python",				"Python",				"Python",				"Pit�n",				"Pitone",				"Pit�o",				"P�ton"	}	},
	{ SSTR_SALAMANDER				,	{	"Salamander",			"Salamandre",			"Salamander",			"Salamandra",			"Salamandra",			"Salamandra",			"Salamandra"	}	},
	{ SSTR_SNAKE					,	{	"Snake",				"Serpent",				"Schlange",				"Serpiente",			"Serpente",				"Cobra",				"Cobra"	}	},
	{ SSTR_TOAD						,	{	"Toad",					"Crapaud",				"Kr�te",				"Sapo",					"Rospo",				"Sapo",					"Sapo"	}	},
	{ SSTR_TORTOISE					,	{	"Tortoise",				"Tortue",				"Schildkr�te",			"Tortuga",				"Testuggine",			"Testudinidae",			"C�gado"	}	},
	{ SSTR_TURTLE					,	{	"Turtle",				"Tortue marine",		"Wasserschildkr�te",	"Tortuga marina",		"Tartaruga",			"Tartaruga",			"Tartaruga"	}	},
	
	// Small mammals
	{ SSTR_SMALL_MAMMAL				,	{	"Small Mammal",			"Petit mammif�re",		"Kleins�ugetier",		"Mam�fero peque�o",		"Piccolo mammifero",	"Pequeno mam�fero",		"Mam�fero pequeno"	}	},
	{ SSTR_BAT						,	{	"Bat",					"Chauve-souris",		"Fledermaus",			"Murci�lago",			"Pipistrello",			"Morcego",				"Morcego"	}	},
	{ SSTR_CHINCHILLA				,	{	"Chinchilla",			"Chinchilla",			"Chinchilla",			"Chinchilla",			"Cincill�",				"Chinchila",			"Chinchila"	}	},
	{ SSTR_FERRET					,	{	"Ferret",				"Furet",				"Frettchen",			"Hur�n",				"Furetto",				"Fur�o",				"Fur�o"	}	},
	{ SSTR_GERBIL					,	{	"Gerbil",				"Gerbille",				"Rennmaus",				"Jerbo",				"Gerbillo",				"Gerbo",				"Gerbo"	}	},
	{ SSTR_GUINEA_PIG				,	{	"Guinea Pig",			"Cochon d'Inde",		"Meerschweinchen",		"Cobaya",				"Cavia",				"Cobaia",				"Cobaia"	}	},
	{ SSTR_HAMSTER					,	{	"Hamster",				"Hamster",				"Hamster",				"H�mster",				"Criceto",				"Hamster",				"Hamster"	}	},
	{ SSTR_HEDGEHOG					,	{	"Hedgehog",				"H�risson",				"Igel",					"Erizo",				"Riccio",				"Ouri�o",				"Ouri�o"	}	},
	{ SSTR_LEMUR					,	{	"Lemur",				"L�murien",				"Lemur",				"L�mur",				"Lemure",				"L�mure",				"L�mure"	}	},
	{ SSTR_MINK						,	{	"Mink",					"Vison",				"Nerz",					"Vis�n",				"Visone",				"Marta",				"Marta"	}	},
	{ SSTR_MOUSE					,	{	"Mouse",				"Souris",				"Maus",					"Rat�n",				"Topo",					"Rato",					"Camundongo"	}	},
	{ SSTR_OPOSSUM					,	{	"Opossum",				"Opossum",				"Opossum",				"Zarig�eya",			"Opossum",				"Marsupial",			"Gamb�"	}	},
	{ SSTR_PRAIRIE_DOG				,	{	"Prairie Dog",			"Chien de prairie" ,	"Pr�riehund",			"Perro de Pradera",		"Cane prateria",		"C�o de pradaria",		"C�o dos prados"	}	},
	{ SSTR_RABBIT					,	{	"Rabbit",				"Lapin",				"Hase",					"Conejo",				"Coniglio",				"Coelho",				"Coelho"	}	},
	{ SSTR_RACCOON					,	{	"Raccoon",				"Raton laveur",			"Waschb�r",				"Mapache",				"Procione",				"Guaxinim",				"Guaxinim"	}	},
	{ SSTR_RAT						,	{	"Rat",					"Rat",					"Ratte",				"Rata",					"Ratto",				"Ratazana",				"Rato"	}	},
	{ SSTR_SKUNK					,	{	"Skunk",				"Moufette",				"Stinktier",			"Mofeta",				"Moffetta",				"Gamb�",				"Doninha"	}	},
	{ SSTR_SQUIRREL					,	{	"Squirrel",				"�cureuil",				"Eichh�rnchen",			"Ardilla",				"Scoiattolo",			"Esquilo",				"Esquilo"	}	},
	
	// Large mammals
	{ SSTR_LARGE_MAMMAL				,	{	"Large Mammal",			"Grand mammif�re",		"Gro�s�ugetier",		"Grandes mam�feros",	"Grande mammifero",		"Grande mam�fero",		"Mam�fero grande"	}	},
	{ SSTR_ANTELOPE					,	{	"Antelope",				"Antilope",				"Antilope",				"Ant�lope",				"Antilope",				"Ant�lope",				"Ant�lope"	}	},
	{ SSTR_APE						,	{	"Ape",					"Grand singe",			"Menschenaffe",			"Simio",				"Scimmia",				"S�mio",				"S�mio"	}	},
	{ SSTR_BABOON					,	{	"Baboon",				"Babouin",				"Pavian",				"Babuino",				"Babbuino",				"Babu�no",				"Babu�no"	}	},
	{ SSTR_BEAR						,	{	"Bear",					"Ours",					"B�r",					"Oso",					"Orso",					"Urso",					"Urso"	}	},
	{ SSTR_CAMEL					,	{	"Camel",				"Chameau",				"Kamel",				"Camello",				"Cammello",				"Camelo",				"Camelo"	}	},
	{ SSTR_CHIMPANZEE				,	{	"Chimpanzee",			"Chimpanz�",			"Schimpanse",			"Chimpanc�",			"Scimpanz�",			"Chimpanz�",			"Chimpanz�"	}	},
	{ SSTR_DEER						,	{	"Deer",					"Cerf",					"Reh",					"Venado",				"Cervide",				"Veado",				"Veado"	}	},
	{ SSTR_ELEPHANT					,	{	"Elephant",				"�l�phant",				"Elefant",				"Elefante",				"Elefante",				"Elefante",				"Elefante"	}	},
	{ SSTR_GIRAFFE					,	{	"Giraffe",				"Girafe",				"Giraffe",				"Jirafa",				"Giraffa",				"Girafa",				"Girafa"	}	},
	{ SSTR_GOAT						,	{	"Goat",					"Ch�vre",				"Ziege",				"Cabra",				"Capra",				"Cabra",				"Bode"	}	},
	{ SSTR_KANGAROO					,	{	"Kangaroo",				"Kangourou",			"K�nguru",				"Canguro",				"Canguro",				"Canguru",				"Canguru"	}	},
	{ SSTR_LLAMA					,	{	"Llama",				"Lama",					"Lama",					"Llama",				"Lama",					"Lama",					"Lhama"	}	},
	{ SSTR_MONKEY					,	{	"Monkey",				"Singe",				"Affe",					"Mono",					"Scimmia",				"Macaco",				"Macaco"	}	},
	{ SSTR_PIG						,	{	"Pig",					"Cochon",				"Schwein",				"Cerdo",				"Maiale",				"Porco",				"Porco"	}	},
	{ SSTR_SEA_LION					,	{	"Sea Lion",				"Lion de mer",			"Seel�we",				"Le�n marino",			"Leone marino",			"Le�o-marinho",			"Le�o-marinho"	}	},
	{ SSTR_SEAL						,	{	"Seal",					"Phoque",				"See-Elefant",			"Foca",					"Foca",					"Foca",					"Foca"	}	},
	{ SSTR_SHEEP					,	{	"Sheep",				"Mouton",				"Schaf",				"Oveja",				"Pecora",				"Ovelha",				"Ovelha"	}	},
	
	{ STR_NONE						, {} }
};


AStrings::
AStrings()
{
	int	i;

	memset( humanStrings, 0, sizeof( humanStrings ) );
	memset( vetStrings, 0, sizeof( vetStrings ) );

	// Initialize both the human and vet string sets to the human values.
	for ( i = 0; HUMAN_STRINGS[ i ].id != STR_NONE; i++ )
	{
//		humanStrings[ HUMAN_STRINGS[ i ].id ] = HUMAN_STRINGS[ i ].str;
//		vetStrings[ HUMAN_STRINGS[ i ].id ] = HUMAN_STRINGS[ i ].str;
		memcpy( humanStrings[ HUMAN_STRINGS[ i ].id ], HUMAN_STRINGS[ i ].str, sizeof( HUMAN_STRINGS[ i ].str ) );
		memcpy( vetStrings[ HUMAN_STRINGS[ i ].id ], HUMAN_STRINGS[ i ].str, sizeof( HUMAN_STRINGS[ i ].str ) );
	}

	// Change the differing strings in the vet string set to the correct values.
	for ( i = 0; VET_STRINGS[ i ].id != STR_NONE; i++ )
	{
//		vetStrings[ VET_STRINGS[ i ].id ] = VET_STRINGS[ i ].str;
		memcpy( vetStrings[ VET_STRINGS[ i ].id ], VET_STRINGS[ i ].str, sizeof( VET_STRINGS[ i ].str ) );
	}

	factoryData = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
	settingsData = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );
}

const char *
AStrings::
operator[]( StringId_t id )
{
	const char * str;
	
	if ( (factoryData != 0 ) && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
	{
		str = humanStrings[ id ][ settingsData->language ];
	}
	else
	{
		str = vetStrings[ id ][ settingsData->language ];
	}
	
	if ( str == 0 )
	{
		LogError( "string %d undefined in AStrings", id );

		if ( (factoryData != 0) && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
		{
			str = humanStrings[ id ][ LANG_ENGLISH ];
		}
		else
		{
			str = vetStrings[ id ][ LANG_ENGLISH ];
		}
		
		if ( str == 0 )
		{
			str = "xxx";
		}
	}
	
	return str;
}

const char *
AStrings::
English( StringId_t id )
{
	const char * str;
	
	if ( (factoryData != 0 ) && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
	{
		str = humanStrings[ id ][ LANG_ENGLISH ];
	}
	else
	{
		str = vetStrings[ id ][ LANG_ENGLISH ];
	}
	
	if ( str == 0 )
	{
		LogError( "string %d undefined in AStrings", id );
		str = "xxx";
	}
	
	return str;
}

const char *
AStrings::
DayOfWeek( unsigned index )
{
	const StringId_t	days[] = { STR_SUNDAY, STR_MONDAY, STR_TUESDAY,
								   STR_WEDNESDAY, STR_THURSDAY, STR_FRIDAY,
								   STR_SATURDAY };
								   
	if ( index < 7 )
	{
		return humanStrings[ days[ index ] ][ settingsData->language ];
	}
	else
	{
		return humanStrings[ STR_NONE ][ settingsData->language ];
	}
}

const char *
AStrings::
Month( unsigned index )
{
	const StringId_t	months[] = { STR_JAN, STR_FEB, STR_MAR, STR_APR, STR_MAY,
									 STR_JUN, STR_JUL, STR_AUG, STR_SEP, STR_OCT,
									 STR_NOV, STR_DEC };

	if ( index < 12 )
	{
		return humanStrings[ months[ index ] ][ settingsData->language ];
	}
	else
	{
		return humanStrings[ STR_NONE ][ settingsData->language ];
	}
}

const char *
AStrings::
Gender( Gender_t gender )
{
	switch ( gender )
	{
		case GENDER_MALE:		return humanStrings[ MSTR_MALE ][ settingsData->language ];	break;
		case GENDER_FEMALE:		return humanStrings[ MSTR_FEMALE ][ settingsData->language ];	break;
		case GENDER_UNKNOWN:
		default:				return humanStrings[ MSTR_UNKNOWN ][ settingsData->language ];	break;
	}
}
