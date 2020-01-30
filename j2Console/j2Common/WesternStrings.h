#ifndef _WESTERN_STRING_H_
#define _WESTERN_STRING_H_

#include "AStrings.h"
#include "CommonDefines.h"


typedef struct
{
    StringId_t       id;
    const wchar_t *  str[NUM_LANGUAGES -1];          // Excluded Chinese
} WesternStringArr_t;

WesternStringArr_t   HUMAN_WESTERN_STRINGS[] = {
//											ENGLISH,								FRENCH,									GERMAN,									SPANISH,								ITALIAN, 								PORTUGUESE,								PORTUGUESE (BRAZIL)																								PORTUGUESE (BRAZIL)
	{ STR_BLANK						,	{	L"",									L"",									    L"",									L"",									L"",									L"",										L""	}	},

	{ STR_PRODUCT_NAME				,	{	L"piccolo xpress",						L"piccolo xpress",					 	L"piccolo xpress",					 	L"piccolo xpress",					 	L"piccolo xpress",						L"piccolo xpress",						L"piccolo xpress"	}	},

// Top (Stings > 35 Characters Unlikely to Display Correctly) 
//											          11111111112222222222233333	          11111111112222222222233333	          11111111112222222222233333	          11111111112222222222233333	          11111111112222222222233333	          11111111112222222222233333	          11111111112222222222233333	
//											 12345678901234567890123456789012345 	 12345678901234567890123456789012345	 12345678901234567890123456789012345	 12345678901234567890123456789012345	 12345678901234567890123456789012345	 12345678901234567890123456789012345	 12345678901234567890123456789012345	
	{ TSTR_ADD_SPECIES				,	{	L"Add Demographic",						L"Ajouter �l�ments d�mogr.",			L"Demographie hinzuf�gen",				L"A�adir demograf�a",					L"Aggiungi demografia",					L"Adicionar demografia",				L"Adicionar dados demogr�ficos"	}	},
	{ TSTR_ADMINISTRATOR			,	{	L"Administrator",						L"Administrateur",						L"Administrator",						L"Administrador",						L"Amministratore",						L"Administrador",						L"Administrador"	}	},
	{ TSTR_ADMINISTRATOR_ID			,	{	L"Administrator ID",					L"ID administrateur",					L"Administrator-ID",					L"Id. del administrador",				L"ID amministratore",					L"ID do administrador",					L"ID do administrador"	}	},
	{ TSTR_ADVANCED_SETTINGS		,	{	L"Advanced Settings",					L"Param�tres avanc�s",					L"Erweiterte Einstellungen",			L"Configuraci�n avanzada",				L"Impostazioni avanzate",				L"Configura��es avan�adas",				L"Configura��es avan�adas"	}	},
	{ TSTR_ALERT_VOLUME				,	{	L"Alert Volume",						L"Alerte volume",						L"Mengenalarm",							L"Volumen de alerta",					L"Volume allarme",						L"Volume de alerta",					L"Volume do alerta"	}	},
	{ TSTR_ALL_RESULTS_BY_DATE		,	{	L"All Results Sorted by Date",			L"Tous r�sultats tri�s par date",		L"Alle Ergebnisse nach Datum",			L"Resultados por fecha",				L"Tutti i risultati ordinati per data",	L"Todos os resultados por data",		L"Todos resultados classif. por data"	}	},
	{ TSTR_ALL_RESULTS_BY_ID		,	{	L"All Results Sorted by ID",			L"Tous r�sultats tri�s par ID",			L"Alle Ergebnisse nach ID",				L"Resultados por Id.",					L"Tutti i risultati ordinati per ID",	L"Todos os resultados por ID",			L"Todos resultados classif. por ID"	}	},
	{ TSTR_ALL_UNITS				,	{	L"All Units",							L"Toutes unit�s",						L"Alle Einheiten",						L"Todas la unidades",					L"Tutte le unit�",						L"Todas as unidades",					L"Todas as unidades"	}	},
	{ TSTR_ALTERNATE_ID				,	{	L"Alternate ID",						L"Autre ID",							L"ID �ndern",							L"Id. suplente",						L"ID alternativo",						L"ID suplente",							L"ID alternativo"	}	},
	{ TSTR_ANALYTE_OPTIONS			,	{	L"Analyte Options", 					L"Options � analyser",					L"Analytoptionen",						L"Opciones de analito", 				L"Opzioni analita"						L"Op��es de analito",					L"Op��es de analito"	}	},
	{ TSTR_ANALYTES					,	{	L"Analytes",							L"Substances � analyser",				L"Analyte",								L"Analitos",							L"Analiti",								L"Analitos",							L"Analitos"	}	},
	{ TSTR_ANALYZER_INFORMATION		,	{	L"Analyzer Information",				L"Information analyseur",				L"Analyseger�t-Angaben",				L"Informaci�n del analizador",			L"Informazioni analizzatore",			L"Informa��o do analisador",			L"Informa��es do analisador"	}	},
	{ TSTR_ANALYZER_SETTINGS		,	{	L"Analyzer Settings",					L"Param�tres analyseur",				L"Analyseger�t-Einstellungen",			L"Configuraci�n del analizador",		L"Impostazioni analizzatore",			L"Configura��es do analisador",			L"Configura��es do analisador"	}	},
	{ TSTR_ARCHIVE_REF_RANGES		,	{	L"Archive Reference Ranges",			L"Fourchettes de r�f. archives",		L"Archiv-Referenzbereiche",				L"Guardar intervalos de referencia",	L"Range di riferimento in archivio",	L"Guardar intervalos de refer�ncia",	L"Arquivar intervalos de refer�ncia"	}	},
	{ TSTR_ASTM_ORDER_METHOD		,	{	L"ASTM Order Method",					L"M�thode d'ordre ASTM",				L"ASTM: Anforderungsmethode",			L"M�todo de pedido de ASTM",			L"Metodo ordine ASTM",					L"M�todo de pedido de ASTM",			L"M�todo de solicita��o de ASTM"	}	},
	{ TSTR_ASTM_PHYSICAL_LAYER		,	{	L"ASTM Physical Layer",					L"Couche physique ASTM",				L"ASTM: Physikalische Ebene",			L"Nivel f�sico ASTM",					L"Livello fisico ASTM",					L"N�vel f�sico de ASTM",				L"Camada f�sica de ASTM"	}	},
	{ TSTR_ASTM_TCPIP_SETTINGS		,	{	L"ASTM TCP/IP Settings", 				L"Param�tres TCP/IP ASTM",				L"ASTM: TCP/IP-Einstellungen",			L"Configuraci�n TCP/IP ASTM",			L"Impostazioni TCP/IP ASTM",			L"Configura��es de TCP/IP de ASTM",		L"Configura��es de TCP/IP do ASTM"	}	},
	{ TSTR_BACKLIGHT_BRIGHTNESS		,	{	L"Backlight Brightness",				L"Lumi�re r�tro�clairage",				L"Helligkeit Hintergrundbeleuchtung",	L"Brillo de la luz de fondo",			L"Luminosit� retroilluminazione",		L"Brilho da luz de fundo",				L"Brilho da luz de fundo"	}	},
	{ TSTR_CALIBRATE_SCREEN			,	{	L"Calibrate Screen",					L"Calibrer �cran",						L"Bildschirm kalibrieren",				L"Calibrar pantalla",					L"Calibra schermo",						L"Calibrar ecr�",						L"Calibrar tela"	}	},
	{ TSTR_CDROM_UPDATE				,	{	L"CD ROM Update",						L"Mise � jour CD ROM",					L"CD-ROM-Aktualisierung",				L"Actualizaci�n CD-ROM",				L"Aggiorna CD ROM",						L"Atualiza��o do CD-ROM",				L"Atualiza��o em CD ROM"	}	},
	{ TSTR_CHECK_TCPIP_SETTINGS		,	{	L"Check TCP/IP Settings", 				L"V�rifier les param�tres TCP/IP",		L"TCP/IP-Einstellungen �berpr�fen",		L"Comprobar configuraci�n TCP/IP",		L"Verifica impostazioni TCP/IP",		L"Verificar configura��es de TCP/IP",	L"Verificar configura��es TCP/IP"	}	},
	{ TSTR_CLICK_VOLUME				,	{	L"Click Volume",						L"Volume clic",							L"Menge anklicken",						L"Pulsar volumen",						L"Fai clic su Volume",					L"Clique em Volume",					L"Clicar em Volume"	}	},
	{ TSTR_CLINICAL_ARCHIVE			,	{	L"Clinical Archive",					L"Archive clinique",					L"Klinisches Archiv",					L"Archivo cl�nico",						L"Archivio clinico",					L"Ficheiro cl�nico",					L"Arquivo cl�nico"	}	},
	{ TSTR_CONFIGURE_PRINTER		,	{	L"Configure Printer",					L"Configurer Imprimante",				L"Drucker konfigurieren",				L"Configurar impresora",				L"Configura stampante",					L"Configurar impressora",				L"Configurar impressora"	}	},
	{ TSTR_CONFIRM					,	{	L"Confirm",								L"Confirmer",							L"Best�tigen",							L"Confirmar",							L"Conferma",							L"Confirmar",							L"Confirmar"	}	},
	{ TSTR_CONFIRM_ORDER			,	{	L"Confirm Order",						L"Confirmer ordre",						L"Anforderung best�tigen",				L"Confirmar pedido",					L"Conferma ordine",						L"Confirmar pedido",					L"Confirmar solicita��o"	}	},
	{ TSTR_CONFIRM_PASSWORD			,	{	L"Confirm Password",					L"Confirmer mot de passe",				L"Passwort best�tigen",					L"Confirmar contrase�a",				L"Conferma password",					L"Confirmar palavra-passe",				L"Confirmar senha"	}	},
	{ TSTR_CONFIRM_ROOM_TEMP		,	{	L"Confirm Room Temperature",			L"Confirmer temp�rature ambiante",		L"Raumtemperatur best�tigen",			L"Confirmar temperatura ambiente",		L"Conferma Temperatura ambiente",		L"Confirmar temperatura ambiente",		L"Confirmar temperatura ambiente" } },
	{ TSTR_CONTROL					,	{	L"Control:",							L"Contr�le :",							L"Kontrolle:",							L"Control:",							L"Controllo:",							L"Controlo:",							L"Controle:"	}	},
	{ TSTR_CONTROL_BY_DATE			,	{	L"Control Results Sorted by Date",		L"Contr�ler r�sultats tri�s par date",	L"Ergebnisse nach Dat. �berpr�fen",		L"Resultados control por fecha",		L"Controlla risult. ordinati per data",	L"Resultados de controlo por data",		L"Result. controle classif. por data"	}	},
	{ TSTR_CONTROL_EXPIRATION_DATE	,	{	L"Control Expiration Date",				L"Contr�ler date de p�remption",		L"Ablaufdatum �berpr�fen",				L"Fecha de vencimiento de control",		L"Controlla data di scadenza",			L"Prazo de validade do controlo",		L"Data de expira��o do controle"	}	},
	{ TSTR_CONTROL_UPDATE			,	{	L"Control Update",						L"Contr�ler mise � jour",				L"Update �berpr�fen",					L"Actualizaci�n de control",			L"Controlla aggiornamento",				L"Atualiza��o do controlo",				L"Atualiza��o do controle"	}	},
	{ TSTR_CRITICAL_STOP			,	{	L"CRITICAL STOP",						L"ARR�T CRITIQUE",						L"KRITISCHER ABBRUCH",					L"PARADA CR�TICA",						L"INTERRUZIONE CRITICA",				L"PARAGEM CR�TICA",						L"PARADA CR�TICA"	}	},
	{ TSTR_DATA_ENTRY_OPTIONS		,	{	L"Data Entry Options",					L"Options de saisie des donn�es",		L"Dateneingabeoptionen",				L"Opciones de entrada de datos",		L"Opzioni immissione dati",				L"Op��es de entrada de dados",			L"Op��es de inser��o de dados"	}	},		
	{ TSTR_DISPLAY_SETTINGS			,	{	L"Display Settings",					L"Affichage param�tres",				L"Anzeigeeinstellungen",				L"Configuraci�n de pantalla",			L"Impostazioni display",				L"Configura��es do visor",				L"Configura��es do display"	}	},
	{ TSTR_ELECTROLYTES_UNITS		,	{	L"Electrolytes Units",					L"Unit�s �lectrolytes",					L"Elektrolyteinheiten",					L"Unidades de electrolitos",			L"Unit� elettroliti",					L"Unidades de eletr�litos",				L"Unidades de eletr�litos"	}	},
	{ TSTR_ENTER_ADMISSION_ID		,	{	L"Enter Admission ID",					L"Saisir ID admission",					L"Zugangs-ID eingeben",					L"Introducir Id. Admisi�n",				L"Inserisci ID ammissione",				L"Introduzir ID de admiss�o",			L"Inserir ID de admiss�o"	}	},
	{ TSTR_ENTER_AGE				,	{	L"Enter Age",							L"Saisir l'�ge",						L"Alter eingeben",						L"Introducir edad",						L"Inserisci et�",						L"Introduzir idade",					L"Inserir idade"	}	},
	{ TSTR_ENTER_AGE_RANGE			,	{	L"Enter Age Range",						L"Saisir fourchette d'�ge",				L"Altersbereich eingeben",				L"Introducir intervalo de edad",		L"Inserisci fascia d'et�",				L"Introduzir intervalo de idades",		L"Inserir faixa et�ria"	}	},
	{ TSTR_ENTER_ALTERNATE_ID		,	{	L"Enter Alternate ID",					L"Saisir autre ID",						L"Alternative ID eingeben",				L"Introducir Id. suplente",				L"Inserisci ID alternativo",			L"Introduzir ID suplente",				L"Inserir ID alternativo"	}	},
	{ TSTR_ENTER_ANALYZER_IP_ADDRESS,	{	L"Enter Analyzer IP Address",			L"Saisir l'adresse IP de l'analyseur",	L"Analyseger�te-IP-Adresse eingeben",	L"Introducir direcci�n IP analizador",	L"Immettere indir. IP analizzatore",	L"Introduzir endere�o IP analisador",	L"Inserir endere�o IP do analisador"	}	},
	{ TSTR_ENTER_ANALYZER_NET_NAME	,	{	L"Enter Analyzer Network Name",			L"Entrer nom du r�seau de l�analyseur",	L"Name des Analyseger�tes im Netzwerk",	L"Introducir nombre de red analizador",	L"Immetti nome rete analizzatore",		L"Introduzir nome rede do analisador",	L"Inserir nome da rede do analisador" } },
	{ TSTR_ENTER_CLIENT_SOCKET		,	{	L"Enter Client Socket",					L"Saisir le socket du client",			L"Client-Socket eingeben",				L"Introducir socket del cliente",		L"Immetti socket client",				L"Introduzir tomada do cliente",		L"Inserir soquete do cliente"  } },
	{ TSTR_ENTER_CONTROL_ID			,	{	L"Enter Control ID",					L"Entrer ID contr�le",					L"Kontrollen-ID eingeben",				L"Introducir Id. de control",			L"Inserisci ID controllo",				L"Introduzir ID do controlo",			L"Inserir ID de controle"	}	},
	{ TSTR_ENTER_DATE				,	{	L"Enter Date",							L"Saisir date",							L"Datum eingeben",						L"Introducir fecha",					L"Inserisci data",						L"Introduzir data",						L"Inserir data"	}	},
	{ TSTR_ENTER_DATE_OF_BIRTH		,	{	L"Enter Date of Birth",					L"Saisir date de naissance",			L"Geburtsdatum eingeben",				L"Introducir fecha de nacimiento",		L"Inserisci data di nascita",			L"Introduzir data de nascimento",		L"Inserir data de nascimento"	}	},
	{ TSTR_ENTER_DOCTOR_ID			,	{	L"Enter Doctor ID",						L"Saisir ID m�decin",					L"Arzt-ID eingeben",					L"Introducir Id. del m�dico",			L"Inserisci ID medico",					L"Introduzir ID do m�dico",				L"Inserir ID do m�dico"	}	},
	{ TSTR_ENTER_END_DATE			,	{	L"Enter End Date",						L"Saisir la date de fin",				L"Enddatum eingeben",					L"Escriba fecha final",					L"Inserisci data fine",					L"Introduzir data final",				L"Inserir data final"	}	},
	{ TSTR_ENTER_GATEWAY_IP_ADDRESS	,	{	L"Enter Gateway IP Address",			L"Saisir adresse IP de la passerelle",	L"Gateway-IP-Adresse eingeben",			L"Introd. direcci�n IP puerta enlace",	L"Immettere indir. IP gateway",			L"Introduzir endere�o IP da porta",		L"Inserir endere�o IP do gateway"	}	},
	{ TSTR_ENTER_ID					,	{	L"Enter ID",							L"Saisir ID",							L"ID eingeben",							L"Introducir Id.",					 	L"Inserisci ID",						L"Introduzir ID",						L"Inserir ID"	}	},
	{ TSTR_ENTER_ID_TO_CANCEL		,	{	L"Enter ID to Cancel",					L"Saisir ID pour annuler",				L"ID zum Abbrechen eingeben",			L"Introducir ID para cancelar",		 	L"Inserisci ID per annullare",			L"Introduzir ID para cancelar",		  	L"Inserir ID para cancelar"}},
	{ TSTR_ENTER_IP_MASK			,	{	L"Enter Analyzer IP Mask", 				L"Saisir le masque IP de l'analyseur",	L"IP-Maske des Analyseger�ts eingeben",	L"Introducir m�scara IP analizador",	L"Inserisci ID per annullare",			L"Introduzir m�scara IP do analisador",	L"Inserir m�scara de IP do analisador"	}	},
	{ TSTR_ENTER_KENNEL_NUMBER		,	{	L"Enter Kennel Number",					L"Saisir n� chenil",					L"Zwingernr. eingeben",					L"Introducir n�mero de caseta",		 	L"Inserisci numero Kennel",				L"Introduzir n�mero de Kennel",		  	L"Inserir n�mero do canil"	}	},
	{ TSTR_ENTER_LAST_VACCINATION	,	{	L"Enter Last Vaccination",				L"Saisir derni�re vaccination",			L"Letzte Impfung eingeben",				L"Introducir �ltima vacuna",			L"inserisci ultima vaccinazione",		L"Introduzir �ltima vacina��o",		  	L"Inserir �ltima vacina��o"	}	},
	{ TSTR_ENTER_LOCATION			,	{	L"Enter Location",						L"Entrer emplacement",					L"Ort eingeben",						L"Introducir ubicaci�n",				L"Inserisci centro",					L"Introduzir localiza��o",			  	L"Inserir local"	}	},
	{ TSTR_ENTER_OPERATOR_ID		,	{	L"Enter Operator ID",					L"Saisir ID op�rateur",					L"Bediener-ID eingeben",				L"Introducir Id. del operario",		 	L"Inserisci ID operatore",				L"Introduzir ID do operador",		  	L"Inserir ID do operador"	}	},
	{ TSTR_ENTER_OWNER_ID			,	{	L"Enter Owner ID",						L"Saisir ID propri�taire",				L"Besitzer-ID eingeben",				L"Introducir id. del due�o",			L"Inserisci ID proprietario",			L"Introduzir ID do propriet�rio",	  	L"Inserir ID do propriet�rio"	}	},
	{ TSTR_ENTER_PASSWORD			,	{	L"Enter Password",						L"Saisir mot de passe",					L"Passwort eingeben",					L"Introducir contrase�a",			 	L"Inserisci password",					L"Introduzir palavra-passe",			L"Inserir senha"	}	},
	{ TSTR_ENTER_PATIENT_ID			,	{	L"Enter Patient ID",					L"Saisir ID patient",					L"Patienten-ID eingeben",				L"Introducir Id. Paciente",			 	L"Inserisci ID paziente",				L"Introduzir ID de doente",			  	L"Inserir ID do paciente"	}	},
	{ TSTR_ENTER_PHONE_NUMBER		,	{	L"Enter Phone Number",					L"Saisir n� de t�l",					L"Telefonnummer eingeben",				L"Introducir n�mero de tel�fono",	 	L"Inserisci numero di telefono",		L"Introduzir n�mero de telefone",	  	L"Inserir n�mero do telefone"	}	},
	{ TSTR_ENTER_SAMPLE_ID			,	{	L"Enter Sample ID",						L"Saisir ID �chantillon",				L"Proben-ID eingeben",					L"Introducir Id. de la muestra",		L"Inserisci ID campione",				L"Introduzir ID da amostra",			L"Inserir ID da amostra"	}	},
	{ TSTR_ENTER_SERVER_IP_ADDRESS	,	{	L"Enter Server IP Address",				L"Saisir l'adresse IP du serveur",		L"Server-IP-Adresse eingeben",		  	L"Introducir direcci�n IP servidor",	L"Immettere l'indirizzo IP del server",	L"Introduzir endere�o IP do servidor", 	L"Inserir endere�o IP do servidor"	}	},
	{ TSTR_ENTER_SERVER_SOCKET		,	{	L"Enter Server Socket",					L"Saisir le socket du serveur",			L"Server-Socket eingeben",			  	L"Introducir socket del servidor",	 	L"Immettere il socket del server",		L"Introduzir tomada do servidor",	  	L"Inserir soquete do servidor"	}	},
	{ TSTR_ENTER_START_DATE			,	{	L"Enter Start Date",					L"Saisir la date de d�but",				L"Anfangsdatum eingeben",			  	L"Escriba fecha inicial",			 	L"Inserisci data inizio",				L"Introduzir data inicial",			  	L"Inserir data de in�cio"	}	},
	{ TSTR_ENZYMES_UNITS			,	{	L"Enzymes Units",						L"Unit�s enzymes",						L"Enzymeinheiten",					  	L"Unidades de enzimas",				 	L"Unit� enzimi",						L"Unidades de enzimas",				  	L"Unidades de enzimas"	}	},
	{ TSTR_ERROR					,	{	L"ERROR",								L"ERREUR",								L"FEHLER",							  	L"ERROR",							 	L"ERRORE",								L"ERRO",								L"ERRO"	}	},
	{ TSTR_FACTORY_DEFAULT_SPECIES	,	{	L"Factory Default Demographics",		L"Param�tres par d�faut �lts d�mogr.",	L"Voreingestellte Demographien",		L"Demograf�a predeterminada",		 	L"Demografia predefinita di fabbrica",	L"Valores de demografia de f�brica",	L"Dados demog. padr�o de f�brica"	}	},
	{ TSTR_FACTORY_DEFAULTS			,	{	L"Factory Defaults",					L"Param�tres par d�faut en usine",		L"Werkseinstellungen",				  	L"Valores predeterminados",			 	L"Impostazioni di fabbrica",			L"Valores de f�brica",				  	L"Padr�es de f�brica"	}	},
	{ TSTR_GROUP_UNITS				,	{	L"Group Units",							L"Unit�s groupe",						L"Gruppeneinheiten",					L"Unidades de grupo",				 	L"Unit� gruppo",						L"Unidades de grupo",				  	L"Unidades do grupo"	}	},
	{ TSTR_HL7_CONFIG_SELECT		,	{	L"HL7 Configuration",					L"Configuration HL7",					L"HL7-Konfiguration",					L"Configuraci�n HL7",					L"Configurazione HL7",					L"Configura��o HL7",					L"Configura��o do HL7"  } },
	{ TSTR_HL7_PHYSICAL_LAYER		,	{	L"HL7 Settings",						L"Param�tres HL7",						L"HL7-Einstellungen",					L"Ajustes HL7",							L"Impostazioni HL7",					L"Conjuntos HL7",						L"Configura��es do HL7" } },
	{ TSTR_HL7_PROTOCOL_SET		    ,	{	L"HL7 Protocol Set",					L"Ensemble de protocol HL7",			L"HL7-Protokoll eingestellt",			L"Conjunto de protocolos HL7",			L"Set protocollo HL7",					L"Conjunto de protocolos HL7",			L"Conjunto de protocolos HL7" } },
	{ TSTR_HOME						,	{	L"Home",								L"Page d'accueil",						L"Startseite",						  	L"Inicio",							 	L"Home",								L"In�cio",							  	L"In�cio"	}	},
	{ TSTR_INCLUDE_REF_RANGES		,	{	L"Include Reference Ranges",			L"Inclure fourchettes de r�f�rence",	L"Referenzbereiche angeben",			L"Incluir intervalos de referencia",	L"Includi range di riferimento",		L"Incluir intervalos de refer�ncia",	L"Incluir intervalos de refer�ncia"	}	},
	{ TSTR_INFORMATION				,	{	L"Information",							L"Information",							L"Information",						  	L"Informaci�n",						 	L"Informazioni",						L"Informa��o",						  	L"Informa��es"	}	},
	{ TSTR_INSERT_ROTOR				,	{	L"Insert Disc",							L"Ins�rer disque",						L"Scheibe einsetzen",				  	L"Insertar disco",					 	L"Inserisci disco",						L"Inserir disco",					  	L"Inserir disco"	}	},
	{ TSTR_INSERT_SAMPLE			,	{	L"Insert Sample",						L"Ins�rer �chantillon",					L"Probe einsetzen",					  	L"Insertar muestra",					L"Inserisci campione",					L"Inserir amostra",					  	L"Inserir amostra"	}	},
	{ TSTR_INTERNAL_ARCHIVES		,	{	L"Internal Archives",					L"Archives internes",					L"Interne Archive",					  	L"Archivos internos",				 	L"Archivi interni",						L"Ficheiros internos",				  	L"Arquivos internos"	}	},
	{ TSTR_LIPIDS_UNITS				,	{	L"Lipids Units",						L"Unit�s lipides",						L"Lipideinheiten",					  	L"Unidades de l�pidos",				 	L"Unit� lipidi",						L"Unidades de l�pidos",				  	L"Unidades de lip�dios"	}	},
	{ TSTR_LOAD_CD					,	{	L"Load CD",								L"Charger CD",							L"CD laden",							L"Cargar CD",						 	L"Carica CD",							L"Carregar CD",						  	L"Carregar CD"	}	},
	{ TSTR_LOAD_CONTROLS_CD			,	{	L"Load Controls CD",					L"Charger CD commandes",				L"CD-Steuerung laden",				  	L"Cargar controles CD",				 	L"Carica CD di controllo",				L"Carregar CD de controlos",			L"Carregar CD de controles"	}	},
	{ TSTR_LOAD_ORDER				,	{	L"Load Order",							L"Charger ordre",						L"Anforderung laden",				  	L"Cargar pedido",					 	L"Carica ordine",						L"Carregar pedido",					  	L"Carregar solicita��o"	}	},
	{ TSTR_LOAD_ROTOR				,	{	L"Load Disc",							L"Charger disque",						L"Scheibe laden",					  	L"Cargar disco",						L"Carica disco",						L"Carregar disco",					  	L"Carregar disco"	}	},
	{ TSTR_LOCKOUT_SETTINGS			,	{	L"Lockout Settings",					L"Param�tres verrouillage",				L"Lookout-Eimstellungen",			  	L"Confguraci�n de bloqueo",			 	L"Impostazioni del blocco",				L"Configura��es de bloqueio",		  	L"Configura��es de bloqueio"	}	},
	{ TSTR_MANAGE_ADMINISTRATORS	,	{	L"Manage Administrators",				L"Gestion administrateurs",				L"Administratoren verwalten",		  	L"Gestionar administradores",		 	L"Gestisci amministratori",				L"Gerir administradores",			  	L"Gerenciar administradores"	}	},
	{ TSTR_MANAGE_OPERATORS			,	{	L"Manage Operators",					L"Gestion op�rateurs",					L"Bediener verwalten",				  	L"Gestionar operarios",				 	L"Gestisci operatori",					L"Gerir operadores",					L"Gerenciar operadores"	}	},
	{ TSTR_MINERALS_UNITS			,	{	L"Minerals Units",						L"Unit�s min�rales",					L"Mineralieneinheiten",				  	L"Unidades de minerales",			 	L"Unit� minerali",						L"Unidades de minerais",				L"Unidades de minerais"	}	},
	{ TSTR_MODIFY_REF_RANGES		,	{	L"Modify Reference Range",				L"Modifier fourchette de r�f�rence",	L"Referenzbereich �ndern",			  	L"Modificar intervalo de referencia", 	L"Modifica range di riferimento",		L"Alterar intervalo de refer�ncia",	  	L"Modificar intervalo de refer�ncia"	}	},
	{ TSTR_NEW_ADMINISTRATOR_ID		,	{	L"New Administrator ID",				L"Nouvel ID administrateur",			L"Neue Administratoren-ID",			  	L"Nueva Id. del administrador",		 	L"Nuova ID amministratore",				L"Nova ID do administrador",			L"Novo ID do administrador"	}	},
	{ TSTR_NEW_OPERATOR_ID			,	{	L"New Operator ID",						L"Nouvel ID op�rateur",					L"Neue Bediener-ID",					L"Nueva Id. del operario",				L"Nuova ID operatore",					L"Nova ID do operador",				  	L"ID do novo operador"	}	},
	{ TSTR_NO_RESULTS				,	{	L"No Results",							L"Aucun r�sultat",						L"Keine Ergebnisse",					L"No hay resultados",					L"Nessun risultato",					L"Sem resultados",					  	L"Nenhum resultado"	}	},
	{ TSTR_NUM_ERROR_COPIES			,	{	L"# Error Copies",						L"Nbre copies erron�es",				L"# Fehler-Kopien",					  	L"N� de copias de error",				L"N. Copie errore",						L"N.� de c�pias de erro",			  	L"No. de c�pias de erro"	}	},
	{ TSTR_NUM_IQC_COPIES			,	{	L"# iQC Copies",						L"Nbre copies iQC",						L"# iQC-Kopien",						L"N� de copias iQC",					L"N. Copie iQC",						L"N.� de c�pias do iQC",				L"No. de c�pias de iQC"	}	},
	{ TSTR_NUM_RESULTS_COPIES		,	{	L"# Results Copies",					L"Nbre copie r�sultats",				L"# Ergebniskopien",					L"N� de copias de resultados",			L"N. Copie risultati",					L"N.� de c�pias de resultados",		  	L"No. de c�pias dos resultados"	}	},
	{ TSTR_OPERATOR					,	{	L"Operator",							L"Op�rateur",							L"Bediener",							L"Operario",							L"Operatore",							L"Operador",							L"Operador"	}	},
	{ TSTR_OPERATOR_EXPIRATION		,	{	L"Operator Expiration",					L"Expiration op�rateur",				L"Ung�ltiger Bediener",				  	L"Vencimiento del operario",			L"Scadenza operatore",					L"Prazo de validade do operador",	  	L"Expira��o do operador"	}	},
	{ TSTR_OPERATOR_ID				,	{	L"Operator ID",							L"ID op�rateur",						L"Bediener-ID",						  	L"Id. del operario",					L"ID operatore",						L"ID do operador",					  	L"ID do operador"	}	},
	{ TSTR_PATIENT					,	{	L"Patient:",							L"Patient :",							L"Patient:",							L"Paciente:",							L"Paziente:",							L"Doente:",							  	L"Paciente:"	}	},
	{ TSTR_PATIENT_BY_DATE			,	{	L"Patient Results Sorted by Date",		L"R�sultats patients tri�s par date",	L"Patientenergebnisse nach Datum",	  	L"Resultados paciente por fecha",		L"Risultati paz. ordinati per data",	L"Resultados do doente por data",	  	L"Result. paciente classif. por data"	}	},
	{ TSTR_PENDING_ORDERS			,	{	L"Pending Orders",						L"Ordres en cours",					  	L"Ausstehende Anforderungen",		  	L"Pedidos pendientes",					L"Ordini in sospeso",					L"Pedidos pendentes",				  	L"Solicita��es pendentes"	}	},
	{ TSTR_POWER_SAVE_WAIT			,	{	L"Power Save Wait",						L"Attente �conomiseur alim.",		  	L"Energiesparmodus, warten",			L"Espera de ahorro de energ�a",			L"Attendere Modo risparmio",			L"Espera de poupan�a de energia",	  	L"Espera de economia de energia"	}	},
	{ TSTR_PRINTER_SETUP			,	{	L"Printer Setup",						L"Configuration imprimante",			L"Druckereinrichtung",				  	L"Configuraci�n impresora",				L"Impostazioni stampante",				L"Configura��o da impressora",		  	L"Configura��o da impressora"	}	},
	{ TSTR_PROTEINS_UNITS			,	{	L"Proteins Units",						L"Unit�s prot�ines",					L"Proteineinheiten",					L"Unidades de prote�nas",				L"Unit� proteine",						L"Unidades de prote�nas",			  	L"Unidades de prote�nas"	}	},
	{ TSTR_RANGES					,	{	L"Ranges",								L"Fourchettes",						  	L"Bereiche",							L"Intervalos",							L"Range",								L"Intervalos",						  	L"Intervalos"	}	},
	{ TSTR_READ_RESULTS				,	{	L"Read Results",						L"Lecture r�sultats",				  	L"Ergebnisse auslesen",				  	L"Leer resultados",						L"Leggi risultati",						L"Ler resultados",					  	L"Ler resultados"	}	},
	{ TSTR_RECALL					,	{	L"Recall",								L"Rappel",							  	L"Wiederaufruf",						L"Recordar",							L"Richiama",							L"Recordar",							L"Recuperar"	}	},
	{ TSTR_REMOVE_ROTOR				,	{	L"Remove Disc",							L"Retirer disque",					  	L"Scheibe entfernen",				  	L"Retirar disco",						L"Rimuovi disco",						L"Retirar disco",					  	L"Remover disco"	}	},
	{ TSTR_REMOVE_SPECIES			,	{	L"Remove Demographic",					L"Retirer �l�ments d�mographiques",	  	L"Demographien entfernen",			  	L"Eliminar demograf�a",					L"Rimuovi demografia",					L"Eliminar demografia",				  	L"Remover dados demogr�ficos"	}	},
	{ TSTR_RETRIEVE_REF_RANGES		,	{	L"Retrieve Reference Ranges",			L"R�cup�rer fourchettes de r�f�rence", 	L"Referenzbereiche empfangen",		  	L"Recuperar intervalos de referencia",	L"Recupera range di riferimento",		L"Recuperar intervalos de refer�ncia", 	L"Recuperar intervalos de refer�ncia"	}	},
//	{ TSTR_SAMPLE					,	{	L"Sample",								L"�chantillon",						  	L"Probe",							  	L"Muestra",								L"Campione",							L"Amostra",							  	L"Amostra"	}	},
	{ TSTR_SCREEN_SAVER_WAIT		,	{	L"Screen Saver Wait",					L"Attente �conomiseur d'�cran",		  	L"Bildschirmschoner, warten",		  	L"Espera del salvapantalla",			L"Attendere salvaschermo",				L"Espera de poupan�a do ecr�",		  	L"Espera de prote��o de tela"	}	},
	{ TSTR_SEARCH					,	{	L"Search",								L"Rechercher",						  	L"Suchen",							  	L"B�squeda",							L"Cerca",								L"Pesquisar",						  	L"Pesquisar"	}	},
	{ TSTR_SEARCH_ADMINISTRATORS	,	{	L"Search Administrators",				L"Rechercher administrateurs",		  	L"Administratoren suchen",			  	L"Buscar administradores",				L"Cerca amministratori",				L"Pesquisar administradores",		  	L"Pesquisar administradores"	}	},
	{ TSTR_SEARCH_OPERATORS			,	{	L"Search Operators",					L"Rechercher op�rateurs",			  	L"Bediener suchen",					  	L"Buscar operarios",					L"Cerca operatori",						L"Pesquisar operadores",				L"Pesquisar operadores"	}	},
	{ TSTR_SECURITY_SETTINGS		,	{	L"Security Settings",					L"Param�tres de s�curit�",			  	L"Sicherheitseinstellungen",			L"Configuraci�n de seguridad",			L"Impostazioni sicurezza",				L"Configura��es de seguran�a",		  	L"Configura��es de seguran�a"	}	},
	{ TSTR_SELECT_COOL_ROOM_TEMP 	,	{	L"Room Temp < 27�C Setting",			L"R�glage temp. ambiante < 27�C",		L"Raumtemp.-Einstellung < 27�C",		L"Ajuste de temp. ambiente < 27�C",		L"Impostazione temp amb < 27�C",		L"Config. da temp. ambiente < 27�C",	L"Temperatura ambiente < 27�C" } },
	{ TSTR_SELECT_DISPLAY_LANG		,	{	L"Select Display Language",				L"S�lectionner affich. langue",		  	L"Sprache ausw�hlen",				  	L"Seleccionar idioma",					L"Seleziona lingua display",			L"Selecionar idioma do visor",		  	L"Selecionar idioma de exibi��o"	}	},
	{ TSTR_SELECT_GENDER			,	{	L"Select Gender",						L"S�lectionner le sexe",				L"Geschlecht ausw�hlen",				L"Seleccionar sexo",					L"Seleziona sesso",						L"Selecionar sexo",					  	L"Selecionar sexo"	}	},
	{ TSTR_SELECT_KEYBOARD			,	{	L"Select Keyboard Type",				L"S�lectionner type de clavier",		L"Tastatur ausw�hlen",				  	L"Seleccionar tipo de teclado",			L"Seleziona tipo tastiera",				L"Selecionar tipo de teclado",		  	L"Selecionar tipo de teclado"	}	},
	{ TSTR_SELECT_PRINTER			,	{	L"Select Printer",						L"S�lectionner imprimante",			  	L"Drucker ausw�hlen",				  	L"Seleccionar impresora",				L"Seleziona stampante",					L"Selecionar impressora",			  	L"Selecionar impressora"	}	},
	{ TSTR_SELECT_PROTOCOL			,	{	L"Select Protocol",						L"S�lectionner un protocole",		  	L"Protokoll ausw�hlen",				  	L"Seleccionar protocolo",				L"Selezionare protocollo",				L"Selecionar protocolo",				L"Selecionar protocolo"	}	},
	{ TSTR_SELECT_RACE				,	{	L"Select Race",							L"Choisir la race",					  	L"Abstammung ausw�hlen",				L"Seleccione raza",						L"Seleziona razza",						L"Selecionar corrida",				  	L"Selecionar ra�a"	}	},
	{ TSTR_SELECT_REPORT			,	{	L"Select Report",						L"S�lectionner rapport",				L"Bericht ausw�hlen",				  	L"Seleccionar informe",					L"Select rapporto",						L"Selecionar relat�rio",				L"Selecionar relat�rio"	}	},
	{ TSTR_SELECT_REPORTS			,	{	L"Select Reports",						L"S�lectionner rapports",			  	L"Berichte ausw�hlen",				  	L"Seleccionar informes",				L"Seleziona rapporti",					L"Selecionar relat�rios",			  	L"Selecionar relat�rios"	}	},
	{ TSTR_SELECT_SPECIES			,	{	L"Select Demographic",					L"S�lectionner �l�ments d�mogr.",	  	L"Demographie ausw�hlen",			  	L"Seleccionar demograf�a",				L"Seleziona demografia",				L"Selecionar demografia",			  	L"Selecionar dados demogr�ficos"	}	},
	{ TSTR_SELECT_TYPE				,	{	L"Select Type",							L"S�lectionner type",				  	L"Typ ausw�hlen",					  	L"Seleccionar tipo",					L"Seleziona tipo",						L"Selecionar tipo",					  	L"Selecionar tipo"	}	},
	{ TSTR_SEND_REF_RANGES			,	{	L"Send Reference Ranges",				L"Envoyer fourchettes de r�f.",		  	L"Referenzbereiche senden",			  	L"Enviar intervalos de referencia",		L"Imposta range di riferimento",		L"Enviar intervalos de refer�ncia",	  	L"Enviar intervalos de refer�ncia"	}	},
	{ TSTR_SERVICE_ARCHIVE			,	{	L"Service Archive",						L"Archive service",					  	L"Service-Archiv",					  	L"Archivo de servicio",					L"Archivio di servizio",				L"Ficheiro do servi�o",				  	L"Arquivo de servi�o"	}	},
	{ TSTR_SET_DATE					,	{	L"Set Date",							L"D�finir date",						L"Datum einstellen",					L"Establecer fecha",					L"Imposta data",						L"Configurar data",					  	L"Configurar data"	}	},
	{ TSTR_SET_DEFAULT				,	{	L"Set Default",							L"D�finir param�tres par d�faut",	  	L"Standard einstellen",				  	L"Establecer predeterminados",			L"Imposta come predefinito",			L"Configurar valores predeterminados", 	L"Configurar padr�o"	}	},
	{ TSTR_SET_LANGUAGE				,	{	L"Set Language",						L"D�finir la langue",				  	L"Sprache einstellen",				  	L"Establecer idioma",					L"Imposta lingua",						L"Configurar idioma",				  	L"Configurar idioma"	}	},
	{ TSTR_SET_PROTOCOL				,	{	L"Set Protocol",						L"D�finir le protocole",				L"Protokoll einstellen",				L"Establecer protocolo",				L"Imposta protocollo",					L"Configurar protocolo",				L"Configurar protocolo"	}	},
	{ TSTR_SET_TIME					,	{	L"Set Time",							L"D�finir l'heure",					  	L"Zeit einstellen",					  	L"Establecer hora",						L"Imposta tempo",						L"Configurar hora",					  	L"Configurar hora"	}	},
	{ TSTR_SET_UNITS				,	{	L"Set Units",							L"D�finir les unit�s",				  	L"Einheiten einstellen",				L"Establecer unidades",					L"Imposta unit�",						L"Configurar unidades",				  	L"Configurar unidades"	}	},
	{ TSTR_SINGLE_ANALYTE			,	{	L"Single Analyte",						L"Une seule substance � analyser",	  	L"Einzelnes Analyt",					L"Analito �nico",						L"Analita singolo",						L"Analito �nico",					  	L"Analito �nico"	}	},
	{ TSTR_SOFTWARE_UPDATE			,	{	L"Software Update",						L"Mise � jour logiciel",				L"Software-Update",					  	L"Actualizaci�n del software",			L"Aggiornamento software",				L"Atualiza��o do software",			  	L"Atualiza��o de software"	}	},
	{ TSTR_SOUND_SETTINGS			,	{	L"Sound Settings",						L"Param�tres sonores",				  	L"Sound-Einstellungen",				  	L"Configuraci�n de sonido",				L"Impostazioni audio",					L"Configura��es do som",				L"Configura��es do som"	}	},
	{ TSTR_SPECIES					,	{	L"Demographic",							L"�l�ments d�mographiques",			  	L"Demographie",						  	L"Demograf�a",							L"Demografia",							L"Demografia",						  	L"Dados demogr�ficos"	}	},
	{ TSTR_SPECIES_AVAILABLE		,	{	L"Demographics Available",				L"�l�ments d�mographiques disponibles",	L"Verf�gbare Demographien",			  	L"Demograf�as disponibles",				L"Demografia disponibile",				L"Demografia dispon�vel",			  	L"Dados demogr�ficos dispon�veis"	}	},
	{ TSTR_STATUS_VOLUME			,	{	L"Status Volume",						L"�tat volume",						  	L"Mengenstatus",						L"Volumen de estado",					L"Volume di stato",						L"Volume de estado",					L"Volume do status"	}	},	
	{ TSTR_TCPIP			        ,	{	L"TCP/IP",								L"TCP/IP",								L"TCP/IP",						        L"TCP/IP",								L"TCP/IP",								L"TCP/IP",								L"TCP/IP" } },
	{ TSTR_TCPIP_ADDRESS_TYPE		,	{	L"TCP/IP Address Type",					L"Type d�adresse TCP/IP",				L"TCP/IP Adresstyp",					L"Tipo de direcci�n TCP/IP",			L"Tipo indirizzi TCP/IP",				L"Tipo de endere�o TCP/IP",				L"Tipo de endere�o TCP/IP" } },
	{ TSTR_TCPIP_SETTINGS			,	{	L"TCP/IP Settings",						L"Param�tres TCP/IP",				  	L"TCP/IP-Einstellungen",				L"Configuraci�n TCP/IP",				L"Impostazioni TCP/IP",					L"Configura��es de TCP/IP",			  	L"Configura��es de TCP/IP"	}	},
	{ TSTR_TRANSMIT_ALL				,	{	L"Transmit All",						L"Transmettre tout",					L"Alle �bertragen",					  	L"Transmitir todo",						L"Trasmetti tutti",						L"Transmitir tudo",					  	L"Transmitir todos"	}	},
	{ TSTR_TRANSMIT_DATE_RANGE		,	{	L"Transmit Date Range",					L"Transmettre la plage de dates",	  	L"Datumsbereich �bertragen",			L"Transmitir intervalo de fechas",		L"Trasmetti range di date",				L"Transmitir intervalo de data",		L"Transmitir intervalo de datas"	}	},
	{ TSTR_UNITS					,	{	L"Units",								L"Unit�s",							  	L"Einheiten",						  	L"Unidades",							L"Unit�",								L"Unidades",							L"Unidades"	}	},
	{ TSTR_VIEW_RESULT				,	{	L"View Result",							L"Afficher r�sultat",				  	L"Ergebnis ansehen",					L"Ver resultado",						L"Visualizza risultati",				L"Visualizar resultado",				L"Visualizar resultado"	}	},
	{ TSTR_WARNING					,	{	L"WARNING",								L"MISE EN GARDE",					  	L"ACHTUNG",							  	L"AVISO",								L"ATTENZIONE",							L"ADVERT�NCIA",						  	L"AVISO"	}	},

// Middle (Long Button Stings > 35 Characters Unlikely to Display Correctly)
//											ENGLISH,																			FRENCH,																											GERMAN,																												SPANISH,																								ITALIAN, 																											PORTUGUESE,																									PORTUGUESE (BRAZIL)
//											          11111111112222222222233333												          11111111112222222222233333																			          11111111112222222222233333																				          11111111112222222222233333																	          11111111112222222222233333																				          11111111112222222222233333																		          11111111112222222222233333																				          	
//											 12345678901234567890123456789012345												 12345678901234567890123456789012345																			 12345678901234567890123456789012345																				 12345678901234567890123456789012345																	 12345678901234567890123456789012345																				 12345678901234567890123456789012345																		 12345678901234567890123456789012345	
	{ MSTR_ADD_ADMINISTRATOR		,	{	L"Add Administrator",																L"Ajouter administrateur",																						L"Admin. hinzuf�gen",																								L"A�adir administrador",																				L"Agg. amministratore",																								L"Adicionar administrador",																					L"Adicionar administrador"	}	},
	{ MSTR_ADD_CONTROL_VALUES		,	{	L"Add Control Values",																L"Ajouter valeurs de contr�le",																					L"Steuerungswerte hinzuf�gen",																						L"A�adir valores de control",																			L"Aggiungi valori di controllo",																					L"Adicionar valores de controlo",																			L"Adicionar valores de controle"	}	},
	{ MSTR_ADD_OPERATOR				,	{	L"Add Operator",																	L"Ajouter op�rateur",																							L"Bediener hinzuf�gen",																								L"A�adir operario",																						L"Aggiungi operatore",																								L"Adicionar operator",																						L"Adicionar operador"	}	},
	{ MSTR_ADD_SPECIES				,	{	L"Add Demographic",																	L"Ajouter �l�ments d�mographiques",																				L"Demographie hinzuf�gen",																							L"A�adir demograf�a",																					L"Aggiungi demografia",																								L"Adicionar demografia",																					L"Adicionar dados demogr�ficos"	}	},
	{ MSTR_ADDRESS_TYPE				,	{	L"Address Type",																	L"Type d�adresse TCP/IP",																						L"TCP/IP Adresstyp",																								L"Tipo de direcci�n TCP/IP",																			L"Tipo indirizzi TCP/IP",																							L"Tipo de endere�o",																						L"Tipo de endere�o" } },
	{ MSTR_ADMISSION_ID				,	{	L"Admission ID",																	L"ID admission",																								L"Zugangs-ID",																										L"Id. admisi�n",																						L"ID ammissione",																									L"ID de Admiss�o",																							L"ID de admiss�o"	}	},
	{ MSTR_ADMINISTRATOR_ID			,	{	L"Administrator ID",																L"ID administrateur",																							L"Administrator-ID",																								L"Id. del administrador",																				L"ID amministratore",																								L"ID do administrador",																						L"ID do administrador"	}	},
	{ MSTR_ADMINISTRATORS			,	{	L"Administrators",																	L"Administrateurs",																								L"Administratoren",																									L"Administradores",																						L"Amministratori",																									L"Administradores",																							L"Administradores"	}	},
	{ MSTR_AGE						,	{	L"Age",																				L"Age",																											L"Alter",																											L"Edad",																								L"Et�",																												L"Idade",																									L"Idade"	}	},
	{ MSTR_ALERTS					,	{	L"Alerts",																			L"Avertisseurs",																								L"Warnungen",																										L"Alertas",																								L"Allarmi",																											L"Alertas",																									L"Alertas"	}	},
	{ MSTR_ALL						,	{	L"All",																				L"Tous",																										L"Alle",																											L"Todo",																								L"Tutti",																											L"Tudo",																									L"Todos"	}	},
	{ MSTR_ALL_ANALYTES				,	{	L"All Analytes",																	L"Toutes subs. � analyser",																						L"Alle Analyte",																									L"Todos los analitos",																					L"Tutti gli analiti",																								L"Todos os analitos",																						L"Todos os analitos"	}	},
	{ MSTR_ALTERNATE_ID				,	{	L"Alternate ID",																	L"Autre ID",																									L"ID �ndern",																										L"Id. suplente",																						L"ID alternativo",																									L"ID suplente",																								L"ID alternativo"	}	},
	{ MSTR_ALWAYS					,	{	L"Always",																			L"Toujours",																									L"Immer",																											L"Siempre",																								L"Sempre",																											L"Sempre",																									L"Sempre"	}	},
	{ MSTR_ANALYSIS_COMPLETE		,	{	L"Analysis Complete",																L"Analyse termin�e",																							L"Analyse abgeschlossen",																							L"An�lisis completado",																					L"Analisi completa",																								L"An�lise conclu�da",																						L"An�lise conclu�da"	}	},
	{ MSTR_ANALYTE					,	{	L"Analyte",																			L"Substance � analyser",																						L"Analyt",																											L"Analito",																								L"Analita",																											L"Analito",																									L"Analito"	}	},
	{ MSTR_ANALYTE_GROUPS			,	{	L"Analyte Groups",																	L"Gr. subst. � analyser",																						L"Analytgruppen",																									L"Grupos analito",																						L"Gruppi analita",																									L"Grupos de analito",																						L"Grupos de analitos"	}	},
	{ MSTR_ANALYTE_OPTIONS			,	{	L"Analyte Options",																	L"Options � analyser",																							L"Analytoptionen",																									L"Opciones de analito",																					L"Opzioni analita",																									L"Op��es de analito",																						L"Op��es de analito"	}	},
	{ MSTR_ANALYZE					,	{	L"Analyze",																			L"Analyser",																									L"Analysieren",																										L"Analizar",																							L"Analizzare",																										L"Analisar",																								L"Analisar"	}	},
	{ MSTR_ANALYZER_IP				,	{	L"Analyzer IP",																		L"IP de l'analyseur",																							L"Analyseger�t-IP",																									L"IP del analizador",																					L"IP analizzatore",																									L"IP do analisador",																						L"IP do analisador"	}	},
	{ MSTR_ANALYZER_NAME			,	{	L"Analyzer Name",																	L"Nom de l�analyseur",																							L"Name des Analyseger�ts",																							L"Nombre del analizador",																				L"Nome analizzatore",																								L"Nome do analisador",																						L"Nome do analisador" } },
	{ MSTR_ANALYZER_PRINTER			,	{	L"Analyzer Printer",																L"Imprimte analyseur",																							L"Drucker-Analysege.",																								L"Impres. Analiz.",																						L"Stamp. Analizzatore",																								L"Impressora do analisador",																				L"Impressora do analisador"	}	},
	{ MSTR_ANALYZING_SAMPLE			,	{	L"Analyzing Sample...",																L"Analyse �ch. en cours...",																					L"Probe wird analysiert ...",																						L"Analizando muestra...",																				L"Esame camp. in corso",																							L"A analisar amostra...",																					L"Analisando amostra..."	}	},
	{ MSTR_ASCII_TEXT				,	{	L"ASCII Text",																		L"Texte ASCII",																									L"ASCII-Text",																										L"Texto ASCII",																							L"Testo ASCII",																										L"Texto ASCII",																								L"Texto ASCII"	}	},
	{ MSTR_ASIAN_PACIFIC_ISLANDER	,	{	L"Asian/Pacific Islander",															L"Asiat./orig. �les Pac.",																						L"Asiatisch/Pazifisch",																								L"Asi�tico/Isl. Pac�fico",																				L"Isolano Asia/Pacifico",																							L"Ilhas Asi�ticas/do Pac�fico",																				L"Asi�tico/ilhas do Pac�fico"	}	},
	{ MSTR_ASTM_1394_7				,	{	L"ASTM 1394-97",																	L"ASTM 1394-97",																								L"ASTM 1394-97",																									L"ASTM 1394-97",																						L"ASTM 1394-97",																									L"ASTM 1394-97",																							L"ASTM 1394-97"	}	},
	{ MSTR_ASTM_ORDERS				,	{	L"ASTM Orders",																		L"Ordres ASTM",																									L"ASTM Anforderungen",																								L"Pedidos ASTM",																						L"Ordini ASTM",																										L"Pedidos ASTM",																							L"Solicita��es ASTM"	}	},
	{ MSTR_AUTO						,	{	L"Auto",																			L"Auto",																										L"Automat.",																										L"Auto",																								L"Auto",																											L"Auto",																									L"Auto"	}	},
	{ MSTR_BACKLIGHT				,	{	L"Backlight",																		L"R�tro�clairage", 																								L"Hintergrundbeleucht.",																							L"Luz",																									L"Retroilluminazione",																								L"Luz de fundo",																							L"Luz de fundo"	}	},
	{ MSTR_BASE_TEST				,	{	L"Base Test",																		L"Test de base",																								L"Basistest",																										L"Prueba base",																							L"Test base",																										L"Teste base",																								L"Teste de base"	}	},
	{ MSTR_BLACK					,	{	L"Black",																			L"Noire",																										L"Schwarz",																											L"Negro",																								L"Nero",																											L"Negro",																									L"Preto"	}	},
	{ MSTR_BOTH						,	{	L"Both",																			L"Les deux", 																									L"Beides",																											L"Ambos",																								L"Entrambe",																										L"Ambos",																									L"Ambos"	}	},
	{ MSTR_BROWSE					,	{	L"Browse",																			L"Naviguer", 																									L"Durchsuchen",																										L"Examinar",																							L"Sfoglia",																											L"Navegar",																									L"Procurar"	}	},
	{ MSTR_CALIBRATE				,	{	L"Calibrate",																		L"Calibrer", 																									L"Kalibrieren",																										L"Calibrar",																							L"Calibra",																											L"Calibrar",																								L"Calibrar"	}	},
	{ MSTR_CLIENT_SOCKET			,	{	L"Client Socket",																	L"Socket du client", 																							L"Client-Socket",																									L"Socket del cliente",																					L"Socket client",																									L"Tomada do cliente",																						L"Soquete do cliente" } },
	{ MSTR_CAM_CYCLES				,	{	L"Cam Cycles",																		L"Cycles cam", 																									L"Nockenzyklen",																									L"Ciclos de la leva",																					L"Cicli camma",																										L"Ciclos Cam",																								L"Ciclos da cam"	}	},
	{ MSTR_CANCEL_ANALYSIS			,	{	L"Cancel Analysis?",																L"Annuler analyse ?", 																							L"Analyse abbrechen?",																								L"�Cancelar an�lisis?",																					L"Annullare analisi?",																								L"Cancelar an�lise?",																						L"Cancelar an�lise?"	}	},
	{ MSTR_CANCEL_ARCHIVE			,	{	L"Cancel Archive?",																	L"Annuler archive ?", 																							L"Archiv stornieren?",																								L"�Cancelar archivo?",																					L"Annulla archivio?",																								L"Cancelar arquivo?",																						L"Cancelar arquivo?"	}	},
	{ MSTR_CANCEL_ORDER				,	{	L"Cancel Order",																	L"Annuler l'ordre",																								L"Anforderung abbrechen",																							L"Cancelar pedido",																						L"Annulla ordine",																									L"Cancelar pedido",																							L"Cancelar solicita��o"	}	},
	{ MSTR_CANCEL_TRANSMISSION		,	{	L"Cancel Transmission?",															L"Annuler transmission ?", 																						L"�bertragung abbrechen?",																							L"�Cancelar transmisi�n?",																				L"Annullare trasmissione?",																							L"Cancelar transmiss�o?",																					L"Cancelar transmiss�o?"	}	},
	{ MSTR_CANCELED					,	{	L"Canceled",																		L"Annul�", 																										L"Abgebrochen",																										L"Cancelado",																							L"Annullato",																										L"Cancelado",																								L"Cancelado"	}	},
	{ MSTR_CANCELING_ANALYSIS		,	{	L"Canceling analysis, please wait",													L"Annulation analyse en cours, veuillez patienter",																L"Analyse wird abgebrochen, bitte warten",																			L"Cancelando an�lisis. Espere, por favor.",																L"Annullamento analisi, attendere",																					L"A cancelar an�lise, espere por favor",																	L"Cancelando a an�lise, aguarde"	}	},
	{ MSTR_CHANGE_EXPIRATION		,	{	L"Change Expiration",																L"Modifier expiration",																							L"Ablaufdatum �ndern",																								L"Cambiar vencimiento",																					L"Modifica scadenza",																								L"Alterar prazo de validade",																				L"Alterar expira��o"	}	},
	{ MSTR_CHANGE_NET_NAME		    ,	{	L"Changing network name, please wait",							                    L"Modification du nom du r�seau, veuillez patienter",															L"Netzwerkname wird ge�ndert, bitte warten",																		L"Cambiando nombre de red; espere",																		L"Modifica nome rete, attendere",																					L"A alterar o nome da rede; espere por favor",																L"Alterando nome da rede, aguarde" } },
	{ MSTR_CHANGE_NET_TYPE		    ,	{	L"Changing network address type, please wait",							            L"Modification du type d�adresse du r�seau, veuillez patienter",												L"Netzwerkadresstyp wird ge�ndert, bitte warten",																	L"Cambiando tipo de direcci�n de red; espere",															L"Modifica tipo indirizzo di rete, attendere",																		L"A alterar o endere�o da rede; espere por favor",															L"Alterando tipo de endere�o de rede, aguarde" } },
	{ MSTR_CHANGE_PASSWORD			,	{	L"Change Password",																	L"Modifier mot de passe",																						L"Passwort �ndern",																									L"Cambiar contrase�a",																					L"Modifica password",																								L"Alterar palavra-passe",																					L"Alterar senha"	}	},
	{ MSTR_CHANGE_SETTINGS			,	{	L"Change Settings",																	L"Modifier les param�tres",																						L"Einstellungen �ndern",																							L"Cambiar configuraci�n",																				L"Modifica impostazioni",																							L"Alterar configura��es",																					L"Alterar configura��es"	}	},
	{ MSTR_CHECK_SETTINGS			,	{	L"Check Settings",																	L"V�rifier les param�tres",																						L"Einstell. �perpr�fen",																							L"Comprobar config.",																					L"Verifica impostazioni",																							L"Verificar configura��es",																					L"Verificar configura��es"	}	},
	{ MSTR_CLEAR					,	{	L"Clear",																			L"Supprimer",																									L"Freigeben",																										L"Borrar",																								L"Pulisci",																											L"Eliminar",																								L"Limpar"	}	},
	{ MSTR_CLINICAL_ARCHIVE			,	{	L"Clinical Archive",																L"Archive clinique",																							L"Klinisches Archiv",																								L"Archivo cl�nico",																						L"Archivio clinico",																								L"Ficheiro cl�nico",																						L"Arquivo cl�nico"	}	},
	{ MSTR_CLINICAL_ARCHIVE_ELLIPSIS,	{	L"Clinical Archive...",																L"Archive clinique...",																							L"Klinisches Archiv...",																							L"Archivo cl�nico..",																					L"Archivio clinico...",																								L"Ficheiro cl�nico...",																						L"Arquivo cl�nico..."	}	},
	{ MSTR_CLOSE_DRAWER_TO_ANALYZE	,	{	L"Close drawer to analyze a sample",												L"Fermer tiroir pour analyser un �chantillon",																	L"Zur Probenanalyse Fach schlie�en",																				L"Cerrar el caj�n para analizar una muestra",															L"Chiudi cassetto per analizzare campione",																			L"Fechar a gaveta para analisar uma amostra",																L"Feche a gaveta para analisar uma amostra"	}	},
	{ MSTR_CLOSING_DRAWER			,	{	L"Closing Drawer...",																L"Fermeture tiroir en cours...",																				L"Fach wird geschlossen ...",																						L"Cerrando caj�n...",																					L"Chiusura cassetto...",																							L"A fechar a gaveta...",																					L"Fechando gaveta..."	}	},
	{ MSTR_CONFIGURE				,	{	L"Configure",																		L"Configurer",																									L"Konfigurieren",																									L"Config.",																								L"Configura",																										L"Configurar",																								L"Configurar"	}	},
	{ MSTR_CONFIRM_TRANSMIT_ALL		,	{	L"Press Yes to transmit all results saved in the analyzer",							L"App. Oui pour trans. tous r�sultats enreg. ds analyseur",														L"\"Ja\", um alle im Analyseger�t gesp. Erg. zu �bertrag.",															L"Pulse S� para transmitir todos los resultados guardados en el analizador",							L"Premere S� per trasmettere tutti i risultati savalti nell'analizzatore",											L"Prima Sim para transmitir todos os resultados guardados no analisador",									L"Pressione Sim para transmitir todos os resultados salvos no analisador"	}	},
	{ MSTR_CONNECTION_FAILED		,	{	L"Connection failed",																L"�chec de la connexion",																						L"Verbindung nicht hergestellt",																					L"Fall� conexi�n",																						L"Connessione non riuscita",																						L"Falha de liga��o",																						L"Falha na conex�o"	}	},
	{ MSTR_CONNECTION_SUCCESSFUL	,	{	L"Connection successful",															L"Connexion �tablie",																							L"Verbindung hergestellt",																							L"Conexi�n establecida",																				L"Connessione riuscita",																							L"Liga��o estabelecida",																					L"Conex�o bem-sucedida"	}	},
	{ MSTR_CONTROLS					,	{	L"Controls",																		L"Contr�les",																									L"Steuerungen",																										L"Controles",																							L"Controlli",																										L"Controlos",																								L"Controles"	}	},
	{ MSTR_COOL_ROOM_TEMP			,	{	L"Fan Speed",																		L"Vitesse du ventilateur",																						L"Ventilatordrehzahl",																								L"Velocidad del ventilador",																			L"Velocit� ventola",																								L"Velocidade da ventoinha",																					L"Velocidade do ventilador" } },
    { MSTR_COOL_ROOM_TEMP_ALERT		,	{	L"This setting lowers the idle fan speed reducing background noise for climate controlled laboratories. This setting may be used in a laboratory with the room temperature below 27�C",				L"Ce r�glage r�duit la vitesse du ventilateur et le bruit de fond dans les laboratoires climatis�s. Il peut �tre utilis� dans un laboratoire dont la temp�rature ambiante est inf�rieure � 27�C",							L"Diese Einstellung reduziert die Leerlaufdrehzahl des L�fters und somit das Hintergrundrauschen in klimageregelten Laboren. Sie eignet sich f�r Raumtemp. von < 27�C",															L"Disminuye velocidad del ventilador en per�odos inactividad reduciendo el ruido de fondo de labs con ambiente controlado. Se puede usar en laboratorios con temp. < 27�C",						L"Tale impostazione riduce la velocit� al minimo della ventola e il rumore di fondo per i laboratori a clima controllato. Pu� essere utilizzata in un laboratorio con temp. ambiente < 27�C",						L"Esta configura��o diminui a velocidade da ventoinha durante per�odos de inatividade reduzindo o ru�do de fundo em laborat�rios com condi��es ambientais controladas. Esta configura��o pode ser utilizada num laborat�rio com uma temperatura ambiente inferior a 27 �C",						L"Essa configura��o reduz a velocidade do ventilador ocioso, reduzindo o ru�do de fundo para laborat�rios climatizados. Essa configura��o pode ser usada em um laborat�rio com temperatura ambiente abaixo de 27 �C"  } },
	{ MSTR_CURRENT					,	{	L"Current",																			L"Actuel",																										L"Laufend",																											L"Actual",																								L"Attuale",																											L"Atual",																									L"Atual"	}	},
	{ MSTR_DATA_ENTRY_OPTIONS		,	{	L"Data Entry Options",																L"Saisie des donn�es",																							L"Dateneingabeoptionen",																							L"Opc. entrada datos",																					L"Opzioni imm. dati",																								L"Op��es de entrada de dados",																				L"Op��es de inser��o de dados"	}	},
	{ MSTR_DATE						,	{	L"Date",																			L"Date",																										L"Datum",																											L"Fecha",																								L"Data",																											L"Data",																									L"Data"	}	},
	{ MSTR_DATE_OF_BIRTH			,	{	L"Date of Birth (DOB)",																L"Date de naissance (DOB)",																						L"Geburtsdatum (GD)",																								L"Fecha de nacimiento",																					L"Data di nascita (DN)",																							L"Data de nascimento (DDN)",																				L"Data de nascimento (DN)"	}	},
	{ MSTR_DATE_OF_VACCINATION		,	{	L"Last Vaccination (DOV)",															L"Dernier vaccin (DOV)",																						L"Letzte Impfung (LI)",																								L"�ltima vacuna",																						L"Ultima vaccinazione (DV)",																						L"�ltima vacina��o (DDV)",																					L"�ltima vacina��o (Data de vacin.)"	}	},
	{ MSTR_DAY						,	{	L"Day",																				L"Jour",																										L"Tag",																												L"D�a",																									L"Giorno",																											L"Dia",																										L"Dia"	}	},
	{ MSTR_DAYS						,	{	L"Days",																			L"Jours",																										L"Tage",																											L"D�as",																								L"Giorni",																											L"Dias",																									L"Dias"	}	},
	{ MSTR_DEFAULT					,	{	L"Default",																			L"V/d�ft",																										L"Standard",																										L"Predeterm.",																							L"Predef.",																											L"Predeterminado",																							L"Padr�o"	}	},
	{ MSTR_DELETE_ADMINISTRATOR		,	{	L"Delete Administrator",															L"Suppr. administrateur",																						L"Administrator l�schen",																							L"Eliminar administrador",																				L"Cancella amministratore",																							L"Eliminar administrador",																					L"Excluir administrador"	}	},
	{ MSTR_DELETE_OPERATOR			,	{	L"Delete Operator",																	L"Suppr. op�rateur", 																							L"Bediener l�schen",																								L"Eliminar operario",																					L"Cancella operatore",																								L"Eliminar operador",																						L"Excluir operador"	}	},
	{ MSTR_DETERMINING_ROTOR_TYPE	,	{	L"Determining disc type",															L"D�termination type de disque",																				L"Scheibentyp festlegen",																							L"Determinando el tipo de disco",																		L"Determinazione tipo del disco",																					L"A determinar o tipo de disco",																			L"Determinando o tipo de disco"	}	},
	{ MSTR_DHCP					    ,	{	L"DHCP",																			L"DHCP",																										L"DHCP",																											L"DHCP",																								L"DHCP",																											L"DHCP",																									L"DHCP" } },
	{ MSTR_DISABLED					,	{	L"Disabled",																		L"D�sactiv�",																									L"Deaktiviert",																										L"Inactivo",																							L"Disattivato",																										L"Inativo",																									L"Desativado"	}	},
	{ MSTR_DOB						,	{	L"DOB",																				L"DOB",																											L"GD",																												L"F. nac.",																								L"DN",																												L"DDN",																										L"DN"	}	},
	{ MSTR_DOCTOR_ID				,	{	L"Doctor ID",																		L"ID m�decin",																									L"Arzt-ID",																											L"Id. m�dico",																							L"ID medico",																										L"ID do m�dico",																							L"ID do m�dico"	}	},
	{ MSTR_DOV						,	{	L"DOV",																				L"DOV",																											L"LI",																												L"�l. vac.",																							L"DV",																												L"DDV",																										L"Data de vacina��o"	}	},
	{ MSTR_DRAWER_CYCLES			,	{	L"Drawer Cycles",																	L"Cycles tiroir",																								L"Fachzyklen",																										L"Ciclos del caj�n",																					L"Cicli cassetto",																									L"Ciclos da gaveta",																						L"Ciclos da gaveta"	}	},
	{ MSTR_EGFR						,	{	L"eGFR", 																			L"eGFR",																										L"eGFR",																											L"eGFR",																								L"eGFR",																											L"TFGe",																									L"TFGe"	}	},
	{ MSTR_ELECTROLYTES				,	{	L"Electrolytes",																	L"�lectrolytes",																								L"Elektrolyte",																										L"Electrolitos",																						L"Elettroliti",																										L"Eletr�litos",																								L"Eletr�litos"	}	},
	{ MSTR_ENABLED					,	{	L"Enabled",																			L"Activ�",																										L"Aktiviert",																										L"Activado",																							L"Attivato",																										L"Ativado",																									L"Ativado"	}	},
	{ MSTR_ENTER_AGE_RANGE			,	{	L"Enter Age Range",																	L"Saisir fourchette d'�ge",																						L"Altersbereich eingeben",																							L"Introducir intervalo de edad",																		L"Inserisci fascia d'et�",																							L"Introduzir intervalo de idades",																			L"Inserir faixa et�ria"	}	},
	{ MSTR_ENTER_PATIENT_ID			,	{	L"Enter Patient ID",																L"Entrer ID  patient",																							L"Patienten-ID eingeben",																							L"Introducir Id. del paciente",																			L"Inserisci ID paziente",																							L"Introduzir ID de doente",																					L"Inserir ID do paciente"	}	},
	{ MSTR_ENZYMES					,	{	L"Enzymes",																			L"Enzymes",																										L"Enzyme",																											L"Encimas",																								L"Enzimi",																											L"Enzimas",																									L"Enzimas"	}	},
	{ MSTR_ERROR_READING_CD			,	{	L"Error reading CD",																L"Erreur lecture CD",																							L"Fehler beim Lesen der CD",																						L"Error al leer el CD",																					L"Errore di lettura CD",																							L"Erro de leitura do CD",																					L"Erro ao ler o CD"	}	},
	{ MSTR_ERROR_REPORT				,	{	L"Error Report",																	L"Rapport erreur",																								L"Fehlerbericht",																									L"Informe de errores",																					L"Rapporto di errore",																								L"Relat�rio de erros",																						L"Relat�rio de erros"	}	},
	{ MSTR_ERRORS					,	{	L"Errors",																			L"Erreurs",																										L"Fehler",																											L"Errores",																								L"Errori",																											L"Erros",																									L"Erros"	}	},
	{ MSTR_EXPIRATION_DATE			,	{	L"Expiration Date",																	L"Date de p�remption",																							L"Ablaufdatum",																										L"Fecha de vencimiento",																				L"Data di scadenza",																								L"Prazo de validade",																						L"Data de expira��o"	}	},
	{ MSTR_EXTERNAL_ARCHIVE			,	{	L"External Archive",																L"Archive externe",																								L"Externes Archiv",																									L"Archivo externo",																						L"Archivio esterno",																								L"Ficheiro externo",																						L"Arquivo externo"	}	},
	{ MSTR_EXTERNAL_PRINTER			,	{	L"External Printer",																L"Imprimante externe",																							L"Externer Drucker",																								L"Impresora externa",																					L"Stampante esterna",																								L"Impressora externa",																						L"Impressora externa"	}	},
	{ MSTR_FACTORY_DEFAULT_SPECIES	,	{	L"Factory Default Demographics",													L"Param./d�faut �lmts d�mogr.",																					L"Voreingestellte Demographien",																					L"Demograf�a predeterminada",																			L"Demografia predefinita di fabbrica",																				L"Valores de demografia de f�brica",																		L"Dados demogr. padr�o de f�brica"	}	},
	{ MSTR_FEMALE					,	{	L"Female",																			L"F�minin",																										L"Weiblich",																										L"Mujer",																								L"Uomo",																											L"Feminino",																								L"Sexo feminino"	}	},
	{ MSTR_FLASHES					,	{	L"Flashes",																			L"Flashs",																										L"Blinkzeichen",																									L"Flashes",																								L"Flash luminosi",					  																				L"Flashes",																									L"Flashes"	}	},
	{ MSTR_FRACTION					,	{	L"Fraction",																		L"Fraction",																									L"Anteil",																											L"Fracci�n",																							L"Frazione",																										L"Fra��o",																									L"Fra��o"	}	},
	{ MSTR_FULL						,	{	L"Full",																			L"Complet",																										L"Voll",																											L"Lleno",																								L"Completo",																										L"Completo",																								L"Cheio"	}	},
	{ MSTR_GATEWAY_IP				,	{	L"Gateway IP",																		L"IP de passerelle",																							L"Gateway-IP",																										L"IP puerta enlace",																					L"IP gateway",																										L"IP da porta",																								L"IP do gateway"	}	},
	{ MSTR_GENDER					,	{	L"Gender",																			L"Sexe",																										L"Geschlecht",																										L"Sexo",																								L"Sesso",																											L"Sexo",																									L"G�nero"	}	},
	{ MSTR_HISPANIC					,	{	L"Hispanic",																		L"Hispanique",																									L"Hispanische Abst.",																								L"Hispano",																								L"Ispanico",																										L"Hisp�nico",																								L"Hisp�nico"	}	},
	{ MSTR_HL7						,	{	L"HL7",																				L"HL7",																											L"HL7",																												L"HL7",																									L"HL7",																												L"HL7",																										L"HL7" } },
	{ MSTR_HL7_CONFIG				,	{	L"HL7 Configuration",																L"Configuration HL7",																							L"HL7-Konfiguration",																								L"Configuraci�n HL7",																					L"Configurazione HL7",																								L"Configura��o HL7",																						L"Configura��o do HL7" } },
	{ MSTR_HOUR						,	{	L"Hour",																			L"Heure",																										L"Stunde",																											L"Hora",																								L"Ora",																												L"Hora",																									L"Hora"	}	},
	{ MSTR_HR						,	{	L"hr",																				L"h",																											L"Std.",																											L"hr",																									L"Or",																												L"h",																										L"h"	}	},
	{ MSTR_INFANT					,	{	L"Infant",																			L"Nourrisson",																									L"S�ugling",																										L"Ni�o",																								L"Cucciolo",																										L"Crian�a",																									L"Lactente"	}	},
	{ MSTR_INSERT_CD				,	{	L"Insert CD, close drawer, and press Continue",										L"Ins�rer CD, fermer tiroir et appuyer sur Continuer",															L"CD einlegen, Fach schlie�en und Weiter dr�cken",																	L"Insertar CD, cerrar bandeja y pulsar Continuar",														L"Inserisci CD, chiudi cassetto e premi Continua",																	L"Insira o CD, feche a gaveta e prima Continuar",															L"Insira o CD, feche a gaveta e pressione Continuar"	}	},
	{ MSTR_INTERNAL_ARCHIVE			,	{	L"Internal Archive",																L"Archive interne",																								L"Internes Archiv",																									L"Archivo interno",																						L"Archivio interno",																								L"Ficheiro interno",																						L"Arquivo interno"	}	},
	{ MSTR_IP_MASK					,	{	L"Subnet Mask",																		L"Masq. sous-r�s",																								L"Subnetzmaske",																									L"M�sc. de subred",																						L"Masch. sottorete",																								L"M�scara de subrede",																						L"M�scara de sub-rede"	}	},
	{ MSTR_IQC						,	{	L"iQC",																				L"iQC",																											L"iQC",																												L"iQC",																									L"iQC",																												L"iQC",																										L"iQC"	}	},
	{ MSTR_IS_NOT_VALID				,	{	L"is not valid",																	L"incorrect",																									L"ung�ltig",																										L"no es v�lido",																						L"non valido",																										L"inv�lido",																								L"n�o � v�lido"	}	},
	{ MSTR_KENNEL_NUMBER			,	{	L"Kennel Number",																	L"Num�ro chenil",																								L"Zwingernr.",																										L"N�mero de caseta",																					L"Numero Kennel",																									L"N�mero de Kennel",																						L"N�mero do canil"	}	},
	{ MSTR_KEYBOARD					,	{	L"Keyboard",																		L"Clavier",																										L"Eingabeger�t",																									L"Teclado",																								L"Tastiera",																										L"Teclado",																									L"Teclado"	}	},
	{ MSTR_LAST_ROTOR				,	{	L"Last Disc",																		L"Dernier disque",																								L"Letzte Scheibe",																									L"�ltimo disco",																						L"Ultimo disco",																									L"�ltimo disco",																							L"�ltimo disco"	}	},
	{ MSTR_LAST_MODIFIED			,	{	L"Last Modified",																	L"Derni�re modif.",																								L"Zuletzt ge�ndert",																								L"�ltimo cambio",																						L"Ultima modifica",																									L"�ltima altera��o",																						L"�ltima modifica��o"	}	},
	{ MSTR_LIPIDS					,	{	L"Lipids",																			L"Lipides",																										L"Lipide",																											L"L�pidos",																								L"Lipidi",																											L"L�pidos",																									L"Lip�dios"	}	},
	{ MSTR_LOAD_FUSE_SETTINGS		,	{	L"Load Fuse Settings", 																L"Charger les param�tres de fusible",																			L"Sicherungseinstellungen laden",																					L"Cargar configuraci�n del fusible",																	L"Carica impostazioni fusibile",																					L"Carregar configura��es de fus�veis",																		L"Carregar configura��es de fus�vel"	}	},
	{ MSTR_LOAD_ROTOR_TO_ANALYZE	,	{	L"To analyze additional sample, load new disc",										L"Pour analyser un �chantillon suppl�mentaire, charger un nouveau disque",										L"Zur Analyse einer weiteren Probe neue Scheibe laden",																L"Para analizar otra muestra, cargue un disco nuevo",													L"Per esaminare ulteriori campioni, caricare nuovo disco",															L"Para analisar outra amostra, carregue um novo disco",														L"Para analisar uma amostra adicional, carregue um novo disco"	}	},
	{ MSTR_LOCATION					,	{	L"Location",																		L"Emplacement",																									L"Ort",																												L"Ubicaci�n",																							L"Sede",																											L"Localiza��o",																								L"Local"	}	},
	{ MSTR_LOCKOUT					,	{	L"Lockout",																			L"Verrouillage",																								L"Verriegelung",																									L"Bloqueo",																								L"Serrata",																											L"Bloqueio",																								L"Bloquear"	}	},
	{ MSTR_LOWER					,	{	L"Lower",																			L"Inf�rieur",																									L"Niedriger",																										L"Inferior",																							L"Basso",																											L"Inferior",																								L"Inferior"	}	},
	{ MSTR_MAC_ADDRESS				,	{	L"MAC Address",																		L"Adresse MAC",																									L"MAC-Adresse",																										L"Direcci�n MAC",																						L"Indirizzo MAC",																									L"Endere�o MAC",																							L"Endere�o MAC" } },
	{ MSTR_MALE						,	{	L"Male",																			L"Masculin",																									L"M�nnlich",																										L"Hombre",																								L"Donna",																											L"Masculino",																								L"Sexo masculino"	}	},
	{ MSTR_MANUAL_SETTINGS			,	{	L"Manual Settings", 																L"Param�tres manuels",																							L"Manuelle Einstellungen",																							L"Configuraci�n manual",																				L"Impostazioni manuali",																							L"Configura��es manuais",																					L"Configura��es manuais"	}	},
	{ MSTR_MIN						,	{	L"min",																				L"min",																											L"min.",																											L"min",																									L"min",																												L"min",																										L"min"	}	},
	{ MSTR_MINERALS					,	{	L"Minerals",																		L"Min�raux",																									L"Mineralien",																										L"Minerales",																							L"Minerali",																										L"Minerais",																								L"Minerais"	}	},
	{ MSTR_MINUTE					,	{	L"Minute",																			L"Minute",																										L"Minute",																											L"Minuto",																								L"Minuto",																											L"Minuto",																									L"Minuto"	}	},
	{ MSTR_MONTH					,	{	L"Month",																			L"Mois",																										L"Monat",																											L"Mes",																									L"Mese",																											L"M�s",																										L"M�s"	}	},
	{ MSTR_MOS						,	{	L"Mos.",																			L"Mois",																										L"Monate",																											L"Meses",																								L"Meses",																											L"Meses",																									L"Meses"	}	},
	{ MSTR_NAME						,	{	L"Name:",																			L"Nom :",																										L"Name:",																											L"Nombre:",																								L"Nome:",																											L"Nome:",																									L"Nome:"	}	},
	{ MSTR_NATIVE_AMERICAN			,	{	L"Native American",																	L"Am�rindienne",																								L"Indianische Abst.",																								L"Americano nativo",																					L"Nativo americano",																								L"Americano nativo",																						L"Americano nativo"	}	},
	{ MSTR_NEONATE					,	{	L"Neonate",																			L"Nouveau-n�",																									L"Neugeborenes",																									L"Neonato",																								L"Neonato",																											L"Rec�m-nascido",																							L"Neonato"	}	},
	{ MSTR_NEUTERED					,	{	L"Neutered",																		L"Ch�tr�",																										L"Kastriert",																										L"Castrado",																							L"Castrato",																										L"Castrado",																								L"Castrado"	}	},
	{ MSTR_NEVER					,	{	L"Never",																			L"Jamais",																										L"Nie",																												L"Nunca",																								L"Mai",																												L"Nunca",																									L"Nunca"	}	},
	{ MSTR_NEW						,	{	L"New",																				L"Nouveau",																										L"Neu",																												L"Nuevo",																								L"Nuovo",																											L"Novo",																									L"Novo"	}	},
	{ MSTR_NO_RESULTS				,	{	L"No saved results to view. Please run a disc prior to recalling results.",			L"Aucun r�sultat enregistr� � afficher. Ex�cuter un disque avant de rappeler les r�sultats.",					L"Keine gespeicherten Ergebnisse zur Anzeige. Vor Wiederaufrufen der Ergebnisse eine Scheibe starten.",				L"No hay resultados guardados para ver. Utilice un disco antes de buscar resultados.",					L"Nessun risultato salvato da visualizzare. Avviare un disco prima di richiamare i risultati.",						L"No existem resultados guardados para visualizar. Utilize um disco antes de recuperar resultados.",		L"Nenhum resultado salvo para visualiza��o. Execute um disco antes de recuperar os resultados."	}	},
	{ MSTR_NON_SI					,	{	L"Non SI",																			L"Non SI",																										L"Kein SI",																											L"No SI",																								L"Non SI",																											L"N�o SI",																									L"N�o SI"	}	},
	{ MSTR_NONE						,	{	L"None",																			L"Aucun",																										L"Kein",																											L"Ninguno",																								L"Nessuno",																											L"Nenhum",																									L"Nenhum"	}	},
	{ MSTR_OFF						,	{	L"Off",																				L"Off",																											L"Aus",																												L"Apagado",																								L"Off",																												L"Desligado",																								L"Desligado"	}	},
	{ MSTR_OPENING_DRAWER			,	{	L"Opening Drawer...",																L"Ouverture tiroir en cours...",																				L"Fach �ffnen ...",																									L"Abriendo bandeja...",																					L"Apertura cassetto in corso...",																					L"A abrir a gaveta...",																						L"Abrindo gaveta..."	}	},
	{ MSTR_OPERATOR_ID				,	{	L"Operator ID",																		L"ID op�rateur",																								L"Bediener-ID",																										L"Id. operario:",																						L"ID operatore",																									L"ID do operador",																							L"ID do operador"	}	},
	{ MSTR_OPERATORS				,	{	L"Operators",																		L"Op�rateurs",																									L"Bediener",																										L"Operarios",																							L"Operatori",																										L"Operadores",																								L"Operadores"	}	},
	{ MSTR_OWNER_ID					,	{	L"Owner ID",																		L"ID propri�taire",																								L"Besitzer-ID",																										L"Id. due�o",																							L"ID propr.",																										L"ID do propriet�rio",																						L"ID do propriet�rio"	}	},
	{ MSTR_PATIENT_CONTROL_ID		,	{	L"Patient/Control ID",																L"ID patient/contr�le",																							L"Patienten-/Kontrollen-ID",																						L"Id. paciente/control",																				L"ID paziente/controllo",																							L"ID do doente/controlo",																					L"ID do paciente/controle"	}	},
	{ MSTR_PATIENT_ID				,	{	L"Patient ID",																		L"ID patient",																									L"Patienten-ID",																									L"Id. paciente",																						L"ID paziente",																										L"ID do doente",																							L"ID do paciente"	}	},
	{ MSTR_PATIENT_TYPE				,	{	L"Patient Type",																	L"Type de patient",																								L"Patiententyp",																									L"Tipo de paciente",																					L"Tipo di paziente",																								L"Tipo de doente",																							L"Tipo de paciente"	}	},
	{ MSTR_PATIENTS					,	{	L"Patients",																		L"Patients",																									L"Patienten",																										L"Pacientes",																							L"Pazienti",																										L"Doentes",																									L"Pacientes"	}	},
	{ MSTR_PENDING_ORDER			,	{	L"Pending Order",																	L"Ordre en cours",																								L"Ausstehende Anforderung",																							L"Pedido pendiente",																					L"Ordine in sospeso",																								L"Pedido pendente",																							L"Solicita��o pendente"	}	},
	{ MSTR_PHONE_NUMBER				,	{	L"Phone Number",																	L"N� t�l",																										L"Telefonnummer",																									L"N�mero de tel�fono",																					L"Num. telefono",																									L"N�mero de telefone",																						L"N�mero de telefone"	}	},
	{ MSTR_PLEASE_REMOVE_ROTOR		,	{	L"Please remove the disc from the analyzer",										L"Retirer le disque de l'analyseur",																			L"Bitte die Scheibe aus dem Analyseger�t entfernen",																L"Retire el disco del analizador",																		L"Rimuovere il disco dall'analizzatore",																			L"Retire o disco do analisador",																			L"Remova o disco do analisador"	}	},
	{ MSTR_PLEASE_WAIT_CDUPDATE		,	{	L"Please wait. Do not remove power. Analyzer will restart when update is complete.", L"Veuillez patienter. Ne pas couper l'alimentation. L'analyseur red�marre une fois la mise � jour termin�e.",	L"Bitte warten. Bitte Stromversorgung nicht abschalten. Analyseger�t wird nach vollst�ndigem Update neu gestartet.", L"Espere, por favor. No apague el aparato. El analizador se reiniciar� cuando acabe la actualizaci�n.", L"Si prega di attendere. Non togliere corrente. L'analizzatore si riavvier� al completamento dell'aggiornamento.",	L"Espere, por favor. N�o desligue. O analisador ser� reiniciado quando terminar a atualiza��o.",			L"Aguarde. N�o remova a energia. O analisador ser� reiniciado quando a atualiza��o concluir."	}	},
	{ MSTR_POTC_1A					,	{	L"POTC-1A",																			L"POTC-1A",																										L"POTC-1A",																											L"POTC-1A",																								L"POTC-1A",																											L"POTC-1A",																									L"POTC-1A"	}	},
	{ MSTR_POWER_SAVE				,	{	L"Power Save",																		L"�cono. alim.",																								L"Energiesparmodus",																								L"Ahorro de energ�a",																					L"Modo risparmio",																									L"Poupan�a de energia",																						L"Economia de energia"	}	},
	{ MSTR_PREMATURE				,	{	L"Premature",																		L"Pr�matur�",																									L"Vorzeitig",																										L"Prematuro",																							L"Prematuro",																										L"Prematuro",																								L"Prematuro"	}	},
	{ MSTR_PRESS_YES_DEFAULTS		,	{	L"Press Yes to return all settings to factory defaults",							L"Appuyer sur Oui pour r�tablir tous les param�tres par d�faut",												L"Dr�cken Sie Ja, um die Werkseinstellungen wieder herzustellen",													L"Pulse S� para reestablecer la configuraci�n predeterminada original",									L"Premere S� per ripristinare tutte le impostazioni di fabbrica",													L"Prima Sim para reestabelecer a configura��o para os valores predeterminados de f�brica",					L"Pressione Sim para retornar todas as configura��es aos padr�es de f�brica"	}	},
	{ MSTR_PRESS_YES_REF_RANGES		,	{	L"Press Yes to return the active demographics list to the factory default",			L"Appuyer sur Oui pour r�tablir la liste d'�l�ments d�mographiques actifs par d�faut",							L"Dr�cken Sie Ja, um die Werkseinstellungen f�r die Liste der aktiven Demographien wieder herzustellen",			L"Pulse S� para reestablecer los valores predeterminados originales de la lista de demograf�as activas", L"Premere S� per mandare l'elenco demografie attive nelle impostazioni di fabbrica",								L"Prima Sim para reestabelecer a lista de demografias ativa para os valores predeterminados de f�brica",	L"Pressione Sim para retornar a lista de dados demogr�ficos ativos ao padr�o de f�brica"	}	},
	{ MSTR_PRESS_YES_TO_START		,	{	L"Press Yes to start",																L"Appuyer sur Oui pour red�marrer",																				L"Dr�cken Sie Ja f�r Start",																						L"Pulse S� para empezar",																				L"Premere S� per avviare",																							L"Prima Sim para iniciar",																					L"Pressione Sim para come�ar"	}	},
	{ MSTR_PRINT					,	{	L"Print",																			L"Impr.",																										L"Drucken",																											L"Imprimir",																							L"Stampa",																											L"Imprimir",																								L"Imprimir"	}	},
	{ MSTR_PRINTED					,	{	L"Printed",																			L"Imprim�",																										L"Gedruckt",																										L"Impreso",																								L"Stampato",																										L"Impresso",																								L"Impresso"	}	},
	{ MSTR_PRINTER					,	{	L"Printer",																			L"Imprimante",																									L"Drucker",																											L"Impr.",																								L"Stampante",																										L"Impressora",																								L"Impressora"	}	},
	{ MSTR_PROTEINS					,	{	L"Proteins",																		L"Prot�ines",																									L"Proteine",																										L"Prote�nas",																							L"Proteine",																										L"Prote�nas",																								L"Prote�nas"	}	},
	{ MSTR_RACE						,	{	L"Race",																			L"Race",																										L"Rasse",																											L"Raza",																								L"Razza",																											L"Corrida",																									L"Ra�a"	}	},
	{ MSTR_READING_CD				,	{	L"Reading CD...",																	L"Lecture du CD...",																							L"CD wird gelesen...",																								L"Leyendo CD...",																						L"Lettura CD...",																									L"A ler o CD...",																							L"Lendo CD..."	}	},
	{ MSTR_RECALL_RESULTS			,	{	L"Recall Results", 																	L"Rappeler r�sultats",																							L"Ergebnisse neu abrufen",																							L"Buscar resultados",																					L"Recupera i risultati",																							L"Recuperar resultados",																					L"Recuperar resultados"	}	},
	{ MSTR_REMOVE_SPECIES			,	{	L"Remove Demographic",																L"Retirer �l�ments d�mographiques",																				L"Demographien entfernen",																							L"Eliminar demograf�a",																					L"Rimuovi demografia",																								L"Eliminar demografia",																						L"Remover dado demogr�fico"	}	},
	{ MSTR_RESULTS					,	{	L"Results",																			L"R�sultats",																									L"Ergebnisse",																										L"Result.",																								L"Risultati",																										L"Resultados",																								L"Resultados"	}	},
	{ MSTR_RESULTS_QUEUED			,	{	L"Results Queued",																	L"R�sultats en file d'attente",																					L"Zu �bertragende Ergebnisse",																						L"Resultados en cola",																					L"Risultati accodati",																								L"Resultados em fila",																						L"Resultados em fila"	}	},
	{ MSTR_RETRIEVE					,	{	L"Retrieve",																		L"R�cup�rer",																									L"Abrufen",																											L"Recuperar",																							L"Recupera",																										L"Recuperar",																								L"Recuperar"	}	},
	{ MSTR_ROTORS_RUN				,	{	L"Discs Run",																		L"Ex�cution disques",																							L"Scheiben in Betrieb",																								L"Usar discos",																							L"Disco in funzione",																								L"Utilizar discos",																							L"Execu��o de discos"	}	},
	{ MSTR_SAMPLE_ID				,	{	L"Sample ID",																		L"ID �chantillon",																								L"Proben-ID",																										L"Id. de muestra",																						L"ID campione",																										L"ID da amostra",																							L"ID da amostra"	}	},
	{ MSTR_SAVING_RESULTS			,	{	L"Saving Results...",																L"Enregistrement des r�sultats en cours...",																	L"Ergebnisse werden gespeichert...",																				L"Guardando resultados...",																				L"Salvataggio risultati...",																						L"A guardar os resultados...",																				L"Salvando resultados..."	}	},
	{ MSTR_SCREEN_CLICKS			,	{	L"Screen Clicks",																	L"Clics �cran",																									L"Bildschirmklicks",																								L"Clics de pantalla",																					L"Clic schermo",																									L"Cliques no ecr�",																							L"Cliques de tela"	}	},
	{ MSTR_SCREEN_SAVER				,	{	L"Screen Saver",																	L"�conomiseur d'�cran",																							L"Bildschirmschoner",																								L"Salvapantalla",																						L"Screensaver",																										L"Prote��o do ecr�",																						L"Protetor de tela"	}	},
	{ MSTR_SEARCH					,	{	L"Search",																			L"Rechercher",																									L"Suchen",																											L"B�squeda",																							L"Cerca",																											L"Pesquisar",																								L"Pesquisar"	}	},
	{ MSTR_SELECT					,	{	L"Select",																			L"S�lectionner",																								L"Ausw�hlen",																										L"Seleccionar",																							L"Seleziona",																										L"Selecionar",																								L"Selecionar"	}	},
	{ MSTR_SELECT_REPORTS			,	{	L"Select Reports",																	L"S�lectionner rapports",																						L"Berichte ausw�hlen",																								L"Seleccionar informes",																				L"Seleziona rapporti",																								L"Selecionar relat�rios",																					L"Selecionar relat�rios"	}	},
	{ MSTR_SEND						,	{	L"Send",																			L"Envoyer",																										L"Senden",																											L"Enviar",																								L"Invia",																											L"Enviar",																									L"Enviar"	}	},
	{ MSTR_SENDING					,	{	L"Sending...",																		L"Envoi en cours...",																							L"Sendet ...",																										L"Enviando...",																							L"Invio in corso...",																								L"A enviar...",																								L"Enviando..."	}	},
	{ MSTR_SENDING_REPORT			,	{	L"Sending Report...",																L"Envoi de rapport en cours...",																				L"Sende Bericht ...",																								L"Enviando informe...",																					L"Invio rapporto in corso...",																						L"A enviar relat�rio...",																					L"Enviando relat�rio..."	}	},
	{ MSTR_SERIAL_NO				,	{	L"Serial No.:",																		L"N� de s�rie :",																								L"Seriennr.:",																										L"N� de serie:",																						L"Numero di serie:",																								L"N.� de s�rie:",																							L"No. de s�rie:"	}	},
	{ MSTR_SERVER_IP				,	{	L"Server IP",																		L"IP du serveur",																								L"Server-IP",																										L"IP de servidor",																						L"IP server",																										L"IP do servidor",																							L"IP do servidor"	}	},
	{ MSTR_SERVER_SOCKET			,	{	L"Server Socket",																	L"Socket serveur",																								L"Server-Socket",																									L"Socket servidor",																						L"Socket server",																									L"Tomada do servidor",																						L"Soquete do servidor"	}	},
	{ MSTR_SERVICE_ARCHIVE			,	{	L"Service Archive",																	L"Archive service",																								L"Service-Archiv",																									L"Archivo de servicio",																					L"Archivio di servizio",																							L"Ficheiro do servi�o",																						L"Arquivo de servi�o"	}	},
	{ MSTR_SERVICE_ARCHIVE_ELLIPSIS	,	{	L"Service Archive...",																L"Archive service...",																							L"Service-Archiv ...",																								L"Archivo de servicio...",																				L"Archivio di servizio...",																							L"Ficheiro do servi�o...",																					L"Arquivo de servi�o..."	}	},
	{ MSTR_SET_DEFAULT				,	{	L"Set Default",																		L"D�f. param./d�faut",																							L"Standard einstellen",																								L"Establ. predeterm.",																					L"Imposta predef.",																									L"Configurar valores predeterminados",																		L"Configurar padr�o"	}	},
	{ MSTR_SETTINGS					,	{	L"Settings",																		L"Param�tres",																									L"Einstellungen",																									L"Configuraci�n",																						L"Impostazioni",																									L"Configura��es",																							L"Configura��es"	}	},	
	{ MSTR_SI						,	{	L"SI",																				L"SI",																											L"SI",																												L"SI",																									L"SI",																												L"SI",																										L"SI"	}	},
	{ MSTR_SINGLE_ANALYTE			,	{	L"Single Analyte",																	L"1 seule subs. � analyser",																					L"Einzelnes Analyt",																								L"Analito �nico",																						L"Analita singolo",																									L"Analito �nico",																							L"Analito �nico"	}	},
	{ MSTR_SPAYED					,	{	L"Spayed",																			L"Ch�tr�e",																										L"Sterilisiert",																									L"Castrada",																							L"Sterilizzato",																									L"Esterilizado",																							L"Esterilizado"	}	},
	{ MSTR_SPECIAL					,	{	L"Special",																			L"Sp�cial",																										L"Spezial",																											L"Especial",																							L"Speciale",																										L"Especial",																								L"Especial"	}	},
	{ MSTR_SPECIES					,	{	L"Demographic",																		L"�lmts d�mogr.",																								L"Demographien",																									L"Demograf�a",																							L"Demografia",																										L"Demografia",																								L"Dado demogr�fico"	}	},
	{ MSTR_STATIC_IP				,	{	L"Static IP",																		L"IP statique",																									L"Statische IP",																									L"IP est�tica",																							L"IP statico",																										L"IP est�tico",																								L"IP est�tico" } },
	{ MSTR_STATUS					,	{	L"Status",																			L"�tat",																										L"Status",																											L"Estado",																								L"Stato",																											L"Estado",																									L"Status"	}	},
	{ MSTR_TCPIP					,	{	L"TCP/IP",																			L"TCP/IP",																										L"TCP/IP",																											L"TCP/IP",																								L"TCP/IP",																											L"TCP/IP",																									L"TCP/IP"	}	},
	{ MSTR_TEST						,	{	L"Test",																			L"Test",																										L"Test",																											L"Prueba",																								L"Prova",																											L"Teste",																									L"Teste"	}	},
	{ MSTR_TESTING_CONNECTION		,	{	L"Testing connection...",															L"Test de la connexion en cours...",																			L"Verbindung wird gepr�ft...",																						L"Probando conexi�n...",																				L"Test della connessione...",																						L"A testar liga��o...",																						L"Testando conex�o..."	}	},
	{ MSTR_THIS_ANALYZER			,	{	L"This Analyzer",																	L"Cet analyseur",																								L"Dieses Analyseger�t",																								L"Este analizador",																						L"Questo analizzatore",																								L"Este analisador",																							L"Este analisador"	}	},
	{ MSTR_TODDLER					,	{	L"Toddler",																			L"B�b�",																										L"Kleinkind",																										L"P�rvulo",																								L"Bambino ai primi passi",																							L"Beb�",																									L"Crian�a"	}	},
	{ MSTR_TOUCH_SCREEN_TO_CALIB	,	{	L"Touch screen to calibrate or wait to cancel.",									L"Toucher l'�cran pour calibrer ou attendre pour annuler.",														L"Zur Kalibrierung Bildschirm ber�hren oder Abbruch abwarten.",														L"Toque la pantalla para calibrar o espere para cancelar.",												L"Toccare lo schermo per calibrare, oppure attendere per annullare.",												L"Toque no ecr� para calibrar ou espere para cancelar.",													L"Toque na tela para calibrar ou aguarde para cancelar."	}	},
	{ MSTR_TRANSMIT_ALL				,	{	L"Transmit All",																	L"Transmettre tout",																							L"Alle �bertragen",																									L"Transmitir todo",																						L"Trasmetti tutti",																									L"Transmitir tudo",																							L"Transmitir todos"	}	},
	{ MSTR_TRANSMIT_DATE_RANGE		,	{	L"Transmit Date Range",																L"Transm. plage de dates",																						L"Datumsbereich �bertr.",																							L"Trans. intervalo/fechas",																				L"Trasmetti range di date",																							L"Transmitir intervalo de data",																			L"Transmitir intervalo de datas"	}	},
	{ MSTR_TRANSMITTING_ELLIPSIS	,	{	L"Transmitting...",																	L"Transmission...",																								L"�bertragung...",																									L"Transmitiendo...",																					L"Trasmissione in corso...",																						L"A transmitir...",																							L"Transmitindo..."	}	},
	{ MSTR_TRANSMITTING_ALL			,	{	L"Transmitting All...",																L"Trans. en cours...",																							L"Alle werden �bertr. ...",																							L"Transmitiendo todo...",																				L"Trasm. tutti in corso...",																						L"A transmitir tudo...",																					L"Transmitindo todos..."	}	},
	{ MSTR_UNITS					,	{	L"Units",																			L"Unit�s",																										L"Einheiten",																										L"Unidades",																							L"Unit�",																											L"Unidades",																								L"Unidades"	}	},
	{ MSTR_UNKNOWN					,	{	L"Unknown",																			L"Inconnu",																										L"Unbekannt",																										L"Desconocido",																							L"Sconosciuto",																										L"Desconhecido",																							L"Desconhecido"	}	},
	{ MSTR_UPDATE_SOFTWARE			,	{	L"Update Software",																	L"Mise � jour logiciel",																						L"Software aktualisieren",																							L"Actualizar programa",																					L"Aggiorna software",																								L"Atualizar o Software",																					L"Atualizar software"	}	},
	{ MSTR_UPDATING_CONTROLS		,	{	L"Updating Controls...",															L"Actualisation des commandes en cours...",																		L"Steuerungen werden aktualisiert ...",																				L"Actualizando controles...",																			L"Aggiornamento controlli in corso...",																				L"A atualizar controlos...",																				L"Atualizando controles..."	}	},
	{ MSTR_UPDATING_SOFTWARE		,	{	L"Updating Software...",															L"Mise � jour logiciel...",																						L"Softw. wird aktual. ...",																							L"Actualizando programa",																				L"Aggior. softw...",																								L"A atualizar o Software...",																				L"Atualizando software..."	}	},
	{ MSTR_UPPER					,	{	L"Upper",																			L"Haut",																										L"Oben",																											L"Superior",																							L"Superiore",																										L"Superior",																								L"Superior"	}	},
	{ MSTR_USB						,	{	L"USB (Serial)",																	L"USB (s�rie)",																									L"USB (ser. Anschl.)",																								L"USB (serie)",																							L"USB (seriale)",																									L"USB (s�rie)",																								L"USB (serial)"	}	},
	{ MSTR_VERSION					,	{	L"Version:",																		L"Version :",																									L"Version:",																										L"Versi�n:",																							L"Versione:",																										L"Vers�o:",																									L"Vers�o:"	}	},
	{ MSTR_WARMING					,	{	L"Warming",																			L"R�chauffement",																								L"Aufw�rmen",																										L"Calentando",																							L"Riscaldamento",																									L"A aquecer",																								L"Aquecendo"	}	},
	{ MSTR_WHITE					,	{	L"White",																			L"Blanc",																										L"Weiss",																											L"Blanco",																								L"Bianco",																											L"Branco",																									L"Branco"	}	},
	{ MSTR_WKS						,	{	L"Wks.",																			L"Sem.",																										L"Wks.",																											L"Sem.",																								L"Sett.",																											L"Sem.",																									L"Semanas"	}	},
	{ MSTR_WORK_LIST				,	{	L"Work List",																		L"Liste de t�ches",																								L"Auftragsliste",																									L"Lista de trabajo",																					L"Elenco di lavoro",																								L"Lista de trabalho",																						L"Lista de trabalho"	}	},
	{ MSTR_XMIT						,	{	L"Xmit",																			L"Trans",																										L"Xmit",																											L"Transm.",																								L"Xmit",																											L"Transm.",																									L"Transmit"	}	},
	{ MSTR_XML						,	{	L"XML",																				L"XML",																											L"XML",																												L"XML",																									L"XML",																												L"XML",																										L"XML"	}	},
	{ MSTR_YEAR						,	{	L"Year",																			L"Ann�e",																										L"Jahr",																											L"A�o",																									L"Anno",																											L"Ano",																										L"Ano"	}	},
	{ MSTR_YRS						,	{	L"Yrs.",																			L"Ans.",																										L"Jahre",																											L"A�os",																								L"Anni",																											L"Anos",																									L"Anos"	}	},
	{ MSTR_ZERO_SEC					,	{	L"Zero Sec",																		L"Z�ro sec",																									L"Null Sek.",																										L"Cero seg",																							L"Zero Sec",																										L"Zero seg",																								L"Zero seg"	}	},

// Bottom									ENGLISH,				FRENCH,							GERMAN,						SPANISH,				ITALIAN, 					PORTUGUESE,				PORTUGUESE (BRAZIL)
	{ BSTR_ADD						,	{	L"Add",					L"Ajouter",						L"Hinzuf�gen",				L"A�adir",				L"Aggiungi",				L"Adicionar",			L"Adicionar"	}	},
	{ BSTR_BACK						,	{	L"Back",				L"Retour",						L"Zur�ck",					L"Volver",				L"Indietro",				L"Voltar",				L"Voltar"	}	},
	{ BSTR_CANCEL					,	{	L"Cancel",				L"Annuler",						L"Abbrechen",				L"Cancelar",			L"Annulla",					L"Cancelar",			L"Cancelar"	}	},
	{ BSTR_CLOSE 					,	{	L"Close",				L"Fermer",						L"Schlie�en",				L"Cerrar",				L"Chiudi",					L"Fechar",				L"Fechar"	}	},
	{ BSTR_CONFIRM					,	{	L"Confirm",				L"Confirmer",					L"Best�tigen",				L"Confirmar",			L"Conferma",				L"Confirmar",			L"Confirmar"	}	},
	{ BSTR_CONTINUE					,	{	L"Continue",			L"Continuer",					L"Fortsetzen",				L"Continuar",			L"Continua",				L"Continuar",			L"Continuar"	}	},
	{ BSTR_DATE						,	{	L"Date",				L"Date",						L"Datum",					L"Fecha",				L"Data",					L"Data",				L"Data"		}	},
	{ BSTR_DONE						,	{	L"Done",				L"Termin�",						L"Erledigt",				L"Terminado",			L"Fatto",					L"Terminar",			L"Conclu�do"	}	},
	{ BSTR_ENTER					,	{	L"Enter",				L"Entr�e",						L"Eingabe",					L"Entrar",				L"Invio",					L"Introduzir",			L"Entrar"	}	},
	{ BSTR_EXIT						,	{	L"Exit",				L"Quit",						L"Exit",					L"Salir",				L"Esci",					L"Sair",				L"Sair"		}	},
	{ BSTR_HOME						,	{	L"Home ",				L"Accueil",						L"Home",					L"Inicio",				L"Home",					L"In�cio",				L"In�cio"	}	},
	{ BSTR_NEXT						,	{	L"Next",				L"Suivant",						L"Weiter",					L"Sig.",				L"Successivo",				L"Seguinte",			L"Pr�ximo"	}	},
	{ BSTR_NO						,	{	L"No",					L"Non",							L"Nein",					L"No",					L"No",						L"N�o",					L"N�o"		}	},
	{ BSTR_OPEN						,	{	L"Open",				L"Ouvrir",						L"�ffnen",					L"Abrir",				L"Aperto",					L"Abrir",				L"Abrir"		}	},
	{ BSTR_PERFORMING_IQC			,	{	L"Performing iQC...",	L"Ex�cution iQC en cours...",	L"iQC wird ausgef�hrt...",	L"Realizando iQC...",	L"Esecuz. iQC in corso...",	L"A executar iQC...",	L"Executando iQC..."	}	},
	{ BSTR_PRINT					,	{	L"Print",				L"Impr.",						L"Drucken",					L"Impr.",				L"Stampa",					L"Imprimir",			L"Imprimir"	}	},
	{ BSTR_RESTART					,	{	L"Restart",				L"Red�marrer",					L"Erneut starten",			L"Reiniciar",			L"Riavvia",					L"Reiniciar",			L"Reiniciar"	}	},
	{ BSTR_SAVE						,	{	L"Save",				L"Enreg.",						L"Save",					L"Guard.",				L"Salva",					L"Guardar",				L"Salvar"	}	},
	{ BSTR_SEARCH					,	{	L"Search",				L"Rechercher",					L"Suchen",					L"B�squeda",			L"Cerca",					L"Pesquisar",			L"Pesquisar"	}	},
	{ BSTR_YES						,	{	L"Yes",					L"Oui",							L"Ja",						L"S�",					L"S�",						L"Sim",					L"Sim"		}	},

// Info text
	{ ISTR_LOAD_ROTOR_1				,	{	L"Remove reagent disc from its foil pouch and transfer 100ul of whole blood, plasma or serum into the disc.\n",					// English
											L"Retirer le disque de r�actif de sa pochette alu et transf�rer 100 ul de sang entier, de plasma ou de s�rum dans le rotor.\n",	// French
											L"Reagenzscheibe aus dem Plastikbeutel entfernen und 100ul Vollblut, Plasma oder Serum in die Scheibe einf�llen.\n",				// German
											L"Retire el disco de reactivo de su bolsa metalizada y transfiera 100 ul de sangre, plasma o suero al disco.\n",					// Spanish
											L"Rimuovere il disco reagente dalla custodia metallizzata e trasferire 100ul di sangue intero, plasma o siero nel disco.\n",		// Italian
											L"Retire o disco de reagente da sua bolsa metalizada e transfira 100 ul de sangue total, plasma ou soro para o disco.\n",		// Portuguese
											L"Remova o disco de reagente da bolsa de alum�nio e transfira 100 ul de sangue total, plasma ou soro para o disco.\n"			// Portuguese (Brazil)
										} 
	},
	{ ISTR_LOAD_ROTOR_2				,	{	L"Insert the reagent disc into the analyzer, close the drawer and follow the on-screen prompts.\n",								// English
											L"Ins�rer le disque de r�actif dans l'analyseur, fermer le tiroir et suivre les instructions qui s'affichent � l'�cran.\n",		// French
											L"Reagenzscheibe in das Analyseger�t einsetzen, Fach schlie�en und Bildschirmanweisungen befolgen.\n",							// German
											L"Inserte el disco de reactivo en el analizador, cierre el caj�n y siga las instrucciones de pantalla.\n",						// Spanish
											L"Inserire il disco reagente nell'analizzatore, chiudere il cassetto e seguire le istruzioni a schermo.\n",						// Italian
											L"Insira o disco de reagente no analisador, feche a gaveta e siga as instru��es do ecr�.\n",										// Portuguese
											L"Insira o disco de reagente no analisador, feche a gaveta e siga as instru��es na tela.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_LOAD_ROTOR_3				,	{	L"View, read or download results.\n",				// English
											L"Afficher, lire et t�l�charger les r�sultats.\n",	// French
											L"Ergebnisse ansehen, lesen oder herunterladen.\n",	// German
											L"Visualice, lea o descargue resultados.\n",			// Spanish
											L"Visualizzare, leggere o scaricare i risultati.\n",	// Italian
											L"Visualize, leia ou descarregue os resultados.\n",	// Portuguese
											L"Visualizar, ler ou baixar resultados.\n"			// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_RESTORE			,	{	L"Restores reference ranges to a particular date and time. The restored reference ranges replace the current reference ranges. All changes after the date reference ranges were archived will be lost. It is recommended that an archive of the current reference ranges be made prior to restoring an earlier set.\n",																				// English
											L"R�tablit les fourchettes de r�f�rence sur une date et une heure donn�es. Les fourchettes de r�f�rence r�tablies remplacent les fourchettes de r�f�rence actuelles. Tous les changements effectu�s apr�s l'archivage des fourchettes de date de r�f�rences seront perdus. Il est conseill� de cr�er une archive des fourchettes de r�f�rence actuelles avant de r�tablir un ensemble ant�rieur.\n",	// French
											L"Setzt die Referenzbereiche auf ein bestimmtes Datum und Zeit zur�ck. Die wiederhergestellten Referenzbereiche ersetzen die aktuellen Referenzbereiche. S�mtliche nach der Archivierung der Datumsreferenzbereiche vorgenommenen �nderungen gehen verloren. Es wird empfohlen, die aktuellen Referenzbereiche zu archivieren, vor Wiederherstellung eines fr�heren Satzes.\n",						// German
											L"Recupera los intervalos de referencia de una fecha y hora espec�ficos. Los intervalos de referencia reestablecidos reemplazan a los intervalos de referencia actuales. Se perder�n todos los cambios tras la fecha en la que se archivaron los intervalos de referencia. Se recomienda hacer un archivo de los intervalos de referencia actuales antes de reestablecer un grupo anterior.\n",		// Spanish
											L"Ripristina i range di riferimento ad una particolare data e ora. I range di riferimento ripristinati sostituiscono i range di riferimento attuali. Tutte le modifiche successive alla data di archiviazione dei range di riferimento andranno perse. Si raccomanda di creare un archivio degli attuali range di riferimento prima di ripristinare un set precedente.\n",							// Italian
											L"Restaura os intervalos de refer�ncia de una data e hora espec�ficas. Os intervalos de refer�ncia restaurados substituem os intervalos de refer�ncia atuais. Ser�o perdidas todas as altera��es ap�s a data do arquivo dos intervalos de refer�ncia. � recomendado realizar um arquivo dos intervalos de refer�ncia atuais antes de recuperar um conjunto anterior.\n",								// Portuguese
											L"Restaura os intervalos de refer�ncia para uma data e hora espec�ficas. Os intervalos de refer�ncia restaurados substituem os intervalos de refer�ncia atuais. Todas as altera��es ap�s o arquivamento dos intervalos de refer�ncia de datas ser�o perdidas. � recomend�vel fazer um arquivo dos intervalos de refer�ncia atuais antes de restaurar um conjunto anterior.\n"						// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_SELECT			,	{	L"Send: Archive reference ranges for future reference.\n\n"
											L"Retrieve: Restore an internal reference range archive.\n",											// English
											L"Envoyer : archiver les fourchettes de r�f�rence afin de pouvoir s'y reporter ult�rieurement.\n\n"
											L"R�cup�rer : restaurer une archive de fourchette de r�f�rence interne.\n",							// French
											L"Senden: Referenzbereiche f�r zuk�nftige Verwendung archivieren.\n\n"
											L"Empfangen: Internes Archiv f�r Referenzbereiche wiederherstellen.\n",								// German
											L"Enviar: archivar intervalos de referencia para usos futuros.\n\n"
											L"Recuperar: reestablecer un archivo de intervalos de referencia interno.\n",						// Spanish
											L"Invia: archivia range di riferimento per utilizzo futuro.\n\n"
											L"Recupera: ripristina un archivio di range di riferimento interno.\n",								// Italian
											L"Enviar: Guardar intervalos de refer�ncia para utiliza��o futura.\n\n"
											L"Recuperar: Restaura um ficheiro de intervalos de refer�ncia interno.\n",							// Portuguese
											L"Enviar: Arquive os intervalos de refer�ncia para futura refer�ncia.\n\n"
											L"Recuperar: Restaure um arquivo de intervalo de refer�ncia interno.\n"								// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_SEND				,	{	L"Printer: Print a copy of a demographic reference range.\n\n"
											L"Internal Archive: Save a copy of all active reference ranges internal to the analyzer for future recall.\n\n"
											L"External Archive: Transmit an ASCII dump of a demographic reference range to an external system.\n",																	// English
											L"Imprimante : imprimer un exemplaire de fourchette de r�f�rence d�mographique.\n\n"
											L"Archive interne : enregistrer un exemplaire de toutes les fourchettes de r�f�rence actives internes � l'analyseur afin de pouvoir les r�cup�rer ult�rieurement.\n\n"
											L"Archive externe : transmettre un vidage de m�moire ASCII d'une fourchette de r�f�rence d�mographique � un syst�me externe.\n",											// French
											L"Drucker: Kopie eines Demographie-Referenzbereichs drucken.\n\n"
											L"Internes Archiv: Eine Kopie aller aktiven Referenzbereiche f�r sp�tere Verwendung intern im Analyseger�t speichern.\n\n"
											L"Externes Archiv: ASCII-Auszug eines Demographie-Referenzbereichs an ein externes System �bertragen.\n",																// German
											L"Impresora: imprimir una copia de un intervalo de referencia de demograf�a.\n\n"
											L"Archivo interno: guardar una copia de todos los intervalos de referencia activos internos para el analizador para usos futuros.\n\n"
											L"Archivo externo: hacer un volcado de datos ASCII de un intervalo de referencia de demograf�a a un sistema externo.\n",													// Spanish
											L"Stampante: stampa una copia di un range di riferimento per demografia.\n\n"
											L"Archivio interno: salva una copia di tutti i range di riferimento attivi interni all'analizzatore per utilizzo futuro.\n\n"
											L"Archivio esterno: trasmette una serie ASCII di range di riferimento per demografia ad un sistema esterno.\n",															// Italian
											L"Impressora: Imprime uma c�pia de um intervalo de refer�ncia de demografia.\n\n"
											L"Ficheiro interno: Guardar uma c�pia de todos os intervalos de refer�ncia ativos internos no analisador para utiliza��o futura.\n\n"
											L"Ficheiro externo: Transmitir um dump de ASCII de um intervalo de refer�ncia de demografia para um sistema externo.\n",													// Portuguese
											L"Impressora: Imprima uma c�pia de um intervalo de refer�ncia de dados demogr�ficos.\n\n"
											L"Arquivo interno: Salve uma c�pia de todos os intervalos de refer�ncia ativos internos no analisador, para futura recupera��o.\n\n"
											L"Arquivo externo: Transmita um despejo ASCII de um intervalo de refer�ncia de dados demogr�ficos para um sistema externo.\n"											// Portuguese (Brazil)
										} 
	},
	{ ISTR_ASTM_ORDER_METHOD		,	{	L"Work List: Select the order from a list.\n\n"
											L"Enter Patient ID: Select the order from a list then confirm order based on entered patient ID.\n\n"
											L"Disabled: ASTM orders are disabled.\n",																												// English
											L"Liste de t�ches : S�lectionner l'ordre dans une liste.\n\n"
											L"Entrer ID patient : S�lectionner l'ordre dans une liste, puis confirmer l'ordre en function de l'ID patient entr�e.\n\n"
											L"D�sactiv� : Les orders ASTM sont  d�sactiv�s.",																										// French
											L"Auftragsliste: Die Anforderung aus einer Liste ausw�hlen.\n\n"
											L"Patienten-ID eingeben: Die Anforderung aus einer Liste ausw�hlen und Anforderung anhand der Patienten-ID best�tigen.\n\n"
											L"Deaktiviert: ASTM-Anforderungen sind deaktiviert.",																									// German
											L"Lista de trabajo: Seleccione el idioma en una lista.\n\n"
											L"Introducir Id. del paciente: Seleccione el pedido en una lista y, a continuaci�n, confirme el pedido en funci�n del Id. del paciente introducido.\n\n"
											L"Desactivado: Los pedidos ASTM est�n desactivados.",																									// Spanish
											L"Elenco di lavoro: Selezionare l'ordine da un elenco.\n\n"
											L"Inserisci ID paziente: Selezionare l'ordine da un elenco, quindi confermarlo in base all'ID paziente inserito.\n\n"
											L"Disattivato: Gli ordini ASTM sono disattivati.",																										// Italian
											L"Lista de trabalho: Seleciona o idioma de uma lista\n\n"
											L"Introduzir ID de doente: Seleciona o pedido de uma lista e, em seguida, confirma o pedido baseado na ID do doente introduzido.\n\n"
											L"Inativo: Os pedidos de ASTM est�o desativados.\n",																										// Portuguese
											L"Lista de trabalho: Selecione a solicita��o em uma lista\n\n"
											L"Inserir ID do paciente: Selecione a solicita��o em uma lista e confirme-a com base no ID do paciente inserido.\n\n"
											L"Desativado: As solicita��es de ASTM est�o desativadas.\n"																								// Portuguese (Brazil)
										}
	},
	{ ISTR_ASTM_PHYSICAL_LAYER		,	{	L"USB (Serial): The analyzer is connected from the USB \"B\" port to a local computer using a USB cable.\n\n"
											L"TCP/IP: The analyzer is connected using a USB-to-Ethernet adapter to a network.\n",																	// English
											L"USB (s�rie) : l'analyseur est connect� du port USB \"B\" � un ordinateur local par un c�ble USB.\n\n"
											L"TCP/IP: l'analyseur est connect� � un r�seau par le biais d'un adaptateur USB-Ethernet.\n",															// French
											L"USB (serieller Anschluss): Das Analysesystem ist �ber den USB-Anschluss \"B\" und ein USB-Kabel an einen lokalen Computer angeschlossen.\n\n"
											L"TCP/IP: Das Analyseger�t ist �ber einen USB- Ethernet-Adapter an ein Netzwerk angeschlossen.\n",														// German
											L"USB (serie): El analizador est� conectado desde un puerto USB \"B\" a un ordenador local mediante un cable USB.\n\n"
											L"TCP/IP: El analizador est� conectado a la red mediante un adaptador USB a Ethernet.\n",																// Spanish
											L"USB (seriale): L'analizzatore � connesso dalla porta USB \"B\" a un computer locale tramite un cavo USB.\n\n"
											L"TCP/IP: L'analizzatore � connesso alla rete tramite un adattatore USB-Ethernet.\n",																	// Italian
											L"USB (s�rie): O analisador est� ligado a partir da porta USB \"B\" de um computador local atrav�s de um cabo USB.\n\n"
											L"TCP/IP: O analisador est� ligado a uma rede atrav�s de um adaptador USB para Ethernet.\n",																// Portuguese
											L"USB (serial): O analisador � conectado da porta USB \"B\" a um computador local usando um cabo USB.\n\n"
											L"TCP/IP: O analisador � conectado usando um adaptador USB para Ethernet em uma rede.\n"																	// Portuguese (Brazil)
										}
	},	
	{ ISTR_CD_UPDATE				,	{	L"The CD ROM drive is located on the rear of the analyzer. Press the small rectangular button to open the drawer. Insert the CD onto the spindle and make sure it clicks into place. Close the CD ROM drive. Press Continue. After the analyzer has restarted, open the CD ROM drive and remove the CD. Store the CD in a safe place.\n",														// English
											L"Le lecteur de CD ROM se trouve � l'arri�re de l'analyseur. Appuyer sur le petit bouton rectangulaire pour ouvrir le tiroir. Ins�rer le CD sur l'axe et v�rifier qu'il est bien en place. Fermer le lecteur de CD ROM. Appuyer sur Continuer. Une fois que l'analyseur a red�marr�, ouvrir le lecteur de CD ROM et retirer le CD. Ranger le CD en lieu s�r.\n",									// French
											L"Das CD-ROM-Laufwerk befindet sich auf der R�ckseite des Analyseger�ts. Zur �ffnung des Fachs die kleine rechteckige Taste dr�cken. Die CD in die Spindel einsetzen und einrasten lassen. Schlie�en Sie das CD-ROM-Laufwerk. Dr�cken Sie auf Weiter. Nach dem Neustart des Analyseger�ts �ffnen Sie das CD-ROM-Laufwerk und entnehmen die CD. Verwahren Sie die CD an einem sicheren Ort.\n",	// German
											L"La unidad de CD-ROM est� en la parte trasera del analizador. Pulse el bot�n rectangular peque�o para abrir la bandeja. Inserte el CD en el eje y aseg�rese de que hace clic cuando se coloca en su sitio. Cierre la unidad de CD-ROM. Pulse Continuar. Despu�s de reiniciar el analizador, abra la unidad de CD-ROM y retire el CD. Guarde el CD en un lugar seguro.\n",						// Spanish
											L"Il drive del CD ROM si trova sul retro dell'analizzatore. Per aprire il cassetto premere il piccolo pulsante rettangolare. Inserire il CD sul supporto e assicurarsi che sia ben fissato. Chiudere il drive del CD ROM. Premere Continua. Una volta riavviato l'analizzatore, aprire il drive del CD ROM e rimuovere il CD. Conservare il CD in un luogo sicuro.\n",							// Italian
											L"A unidade de CD-ROM est� localizada na parte posterior do analisador. Prima o bot�o retangular pequeno para abrir a gaveta. Insira o CD no eixo e certifique-se de que ouve um clique quando � colocado no lugar. Feche a unidade de CD-ROM. Prima Continuar. Depois de o analisador ter reiniciado, abra a unidade de CD-ROM e retire o CD. Guarde o CD num local seguro.\n",					// Portuguese
											L"A unidade de CD-ROM est� localizada na parte traseira do analisador. Pressione o pequeno bot�o retangular para abrir a gaveta. Insira o CD no eixo e verifique se ele se encaixa no lugar. Feche a unidade de CD-ROM. Pressione Continuar. Depois que o analisador reiniciar, abra a unidade de CD-ROM e remova o CD. Guarde o CD em um local seguro.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_COMM_PROTOCOL			,	{	L"None: No transmission of information is possible.\n\n"
											L"ASCII Text: ASCII text dump of information unidirectional from the analyzer.\n\n"
											L"ASTM: ASTM 1394 E 97 over ASTM E 1381 02 Standard Specification for Transferring Information Between Clinical Instruments and Computer Systems.\n\n"
											L"XML: Extensible Markup Language output in proprietary Abaxis format.\n",																							// English
											L"Aucune : aucune transmission d'information n'est possible.\n\n"
											L"Texte ASCII : vidage de texte ASCII d'information unidirectionnelle � partir de l'analyseur.\n\n"
											L"ASTM : Sp�cification des normes ASTM 1394 E 97/ASTM E 1381 02 pour la transmission de donn�es entre des instruments cliniques et des syst�mes informatiques.\n\n"
											L"XML : Sortie en langage XML (Extensible Markup Language) dans le format propri�taire d'Abaxis.\n",																	// French
											L"Keine: Keine Daten�bertragung m�glich.\n\n"
											L"ASCII-Text: ASCII-Textauszug einseitig vom Analyseger�t.\n\n"
											L"ASTM: ASTM 1394 E 97 �ber ASTM E 1381 02 Standardspezifikation f�r die �bertragung von Informationen zwischen klinischen Ger�ten und Computersystemen.\n\n"
											L"XML: Ausgabe als Extensible Language in propriet�rem Abaxis-Format.\n",																							// German
											L"Ninguna: no es posible realizar la transmisi�n de la informaci�n.\n\n"
											L"Texto ASCII: volcado de datos en texto ASCII unidireccional desde el analizador.\n\n"
											L"ASTM 1394 E 97 sobre ASTM E 1381 02 Especificaci�n est�ndar sobre transferencia de informaci�n entre instrumentos cl�nicos y sistemas inform�ticos.\n\n"
											L"XML: Salida en lenguaje de marcado extensible en formato propiedad de Abaxis.\n",																					// Spanish
											L"Nessuno: non � possibile alcuna trasmissione di informazioni.\n\n"
											L"Testo ASCII: serie unidirezionale di informazioni in testo ASCII dall'analizzatore.\n\n"
											L"ASTM: ASTM 1394 E 97 sulla specifica standard ASTM E 1381 02 per il trasferimento di informazioni tra strumenti clinici e computer.\n\n"
											L"XML: Output di tipo Extensible Markup Language in formato proprietario Abaxis.\n",																					// Italian
											L"Nenhum: N�o � poss�vel realizar a transmiss�o de informa��o.\n\n"
											L"Texto ASCII: Dump de informa��es de texto ASCII, unidirecional do analisador.\n\n"
											L"ASTM: ASTM 1394 E 97 sobre ASTM E 1381 02 Especifica��o padr�o sobre transfer�ncia de informa��o entre instrumentos cl�nicos e sistemas inform�ticos.\n\n"
											L"XML: Sa�da em linguagem de marca��o extens�vel em formato propriedade da Abaxis.\n",																				// Portuguese
											L"Nenhum: Nenhuma transmiss�o de informa��es � poss�vel.\n\n"
											L"Texto ASCII: Despejo em texto ASCII de informa��es unidirecionais do analisador.\n\n"
											L"ASTM: ASTM 1394 E 97 via ASTM E 1381 02 Especifica��o padr�o para transfer�ncia de informa��es entre instrumentos cl�nicos e sistemas de computadores.\n\n"
											L"XML: Sa�da da linguagem de marca��o extens�vel no formato patenteado da Abaxis.\n"																					// Portuguese (Brazil)
										} 
	},
	{ ISTR_DATE_INVALID				,	{	L"Only valid calendar days may be entered. Please set the date to today's date.\n",														// English
											L"Saisir uniquement des jours civils valides. D�finir la date d'aujourd'hui comme date du jour.\n",										// French
											L"Nur g�ltige Kalendertage eingeben. Datumsanzeige auf das heutige Datum einstellen.\n",													// German
											L"Solo se pueden introducir d�as naturales correctos. Cambie la fecha al d�a de hoy.\n",													// Spanish
											L"Possono essere inseriti solo giorni validi. Si prega di impostare la data odierna.\n",													// Italian
											L"Apenas podem ser introduzidos dias de calend�rio v�lidos. Defina a data para a data do dia de hoje.\n",								// Portuguese
											L"Apenas dias corridos v�lidos podem ser inseridos. Configure a data como a data de hoje.\n"												// Portuguese (Brazil)
										} 
	},
	{ ISTR_DELETE_ADMINISTRATOR		,	{	L"Removing an administrator from the administrator list will permanently remove the administrator, and all information for that administrator will be lost.\n",					// English
											L"La suppression d'un administrateur dans la liste d'administrateurs a pour effet de le supprimer et toutes les informations associ�es seront perdues.\n",						// French
											L"Die Entfernung eines Administrators aus der Administratorliste entfernt den Administrator dauerhaft, und s�mtliche Informationen zu diesem Administrator gehen verloren.\n",	// German
											L"Al eliminar un administrador de la lista de administradores se le elimina permanentemente. Se perder� toda la informaci�n de ese administrador.\n",							// Spanish
											L"La rimozione di un amministratore dall'elenco amministratori sar� permanente e tutte le informazioni per tale amministratore saranno perse.\n",								// Italian
											L"Ao eliminar um administrador da lista de administradores ir� eliminar permanentemente o administrador, e toda a informa��o desse administrador ser� perdida.\n",				// Portuguese
											L"A remo��o de um administrador da lista de administradores o remover� permanentemente e todas as informa��es desse administrador ser�o perdidas.\n"								// Portuguese (Brazil)
										} 
	},
	{ ISTR_DELETE_OPERATOR			,	{	L"Removing an operator from the operator list will permanently remove the operator, and all information for that operator will be lost.\n",										// English
											L"La suppression d'un op�rateur dans la liste d'op�rateurs a pour effet de le supprimer et toutes les informations associ�es seront perdues.\n",									// French
											L"Die Entfernung eines Bedieners aus der Bedienerliste entfernt den Bediener dauerhaft, und s�mtliche Informationen zu diesem Bediener gehen verloren.\n",						// German
											L"Al eliminar un operario de la lista de operarios se le elimina permanentemente. Se perder� toda la informaci�n de ese operario.\n",											// Spanish
											L"La rimozione di un operatore dall'elenco operatori sar� permanente e tutte le informazioni per tale operatore saranno perse.\n",												// Italian
											L"Ao eliminar um operador da lista de operadores ir� eliminar permanentemente o operador, e toda a informa��o desse operador ser� perdida.\n",									// Portuguese
											L"A remo��o de um operador da lista de operadores o remover� permanentemente e todas as informa��es desse operador ser�o perdidas.\n"											// Portuguese (Brazil)
										} 
	},
	{ ISTR_DISPLAY_SETTINGS			,	{	L"Backlight: Controls the backlight level for the display.\n\n"
											L"Screen Saver: Sets the time from the last screen press until the screen saver starts.\n\n"
											L"Power Save: Sets the time from the last screen press until the display is turned off.\n\n"
											L"Calibrate: Aligns the touch screen to the LCD display.\n",																				// English
											L"R�tro�clairage : commande le niveau de r�tro�clairage de l'�cran.\n\n"
											L"�conomiseur d'�cran : d�finit la dur�e entre la derni�re activation de l'�cran et la mise en marche de l'�conomiseur d'�cran.\n\n"
											L"�conomiseur d'alimentation : d�finit la dur�e entre la derni�re activation de l'�cran et le moment o� l'�cran est arr�t�.\n\n"
											L"Calibrage : alignement de l'�cran tactile avec l'�cran � cristaux liquides.\n",														// French
											L"Hintergrundbeleuchtung: Steuert die Hintergrundbeleuchtung des Bildschirms.\n\n"
											L"Bildschirmschoner: Zur Einstellung des Zeitintervalls von der letzten Bildschirmber�hrung bis zum Start des Bildschirmschoners.\n\n"
											L"Energiesparmodus: Zur Einstellung der Zeit von der letzten Bildschirmber�hrung bis zur Abschaltung des Bildschirms.\n\n"
											L"Kalibrieren: Richtet den Touch-Screen am LCD-Bildschirm aus.\n",																		// German
											L"Luz de fondo: controla el nivel de la luz de fondo de la pantalla.\n\n"
											L"Salvapantalla: marca el tiempo desde la �ltima presi�n sobre la pantalla hasta la activaci�n del salvapantalla.\n\n"
											L"Ahorro de energ�a: marca el tiempo desde la �ltima presi�n de pantalla hasta que la pantalla se apaga.\n\n"
											L"Calibrar: alinea la pantalla t�ctil a la pantalla de cristal l�quido.\n",																// Spanish
											L"Retroilluminazione: controlla il livello di retroilluminazione per il display.\n\n"
											L"Screensaver: imposta il tempo tra l'ultima pressione sullo schermo e l'avvio dello screensaver.\n\n"
											L"Modo risparmio: imposta il tempo tra l'ultima pressione sullo schermo allo spegnimento del display.\n\n"
											L"Calibra: allinea il touchscreen al display LCD.\n",																					// Italian
											L"Luz de fundo: Controla o n�vel da luz de fundo do visor.\n\n"
											L"Prote��o do ecr�: Define o tempo desde a �ltima vez que o ecr� � premido at� ao in�cio da prote��o do ecr�.\n\n"
											L"Poupan�a de energia: Define o tempo desde a �ltima vez que o ecr� � premido at� que o visor � desligado.\n\n"
											L"Calibrar: Alinha o ecr� t�til com o visor LCD.\n",																						// Portuguese
											L"Luz de fundo: Controla o n�vel de luz de fundo do visor.\n\n"
											L"Protetor de tela: Configura o tempo desde a �ltima press�o de tela at� a prote��o de tela iniciar.\n\n"
											L"Economia de energia: Configura o tempo desde a �ltima press�o de tela at� o display desligar.\n\n"
											L"Calibrar: Alinha a tela sens�vel ao toque com o display LCD.\n"																		// Portuguese (Brazil)
										} 
	},
	{ ISTR_FACTORY_DEFAULTS			,	{	L"Restoring the analyzer to factory defaults resets the analyzer to the same conditions as when the analyzer left the factory. It is recommended that the reference ranges be archived prior to resetting the analyzer to factory defaults. Saved results and internal reference range archives will still be available after the settings are reset to factory defaults.\n",																						// English
											L"Le fait de r�tablir les valeurs par d�faut de l'analyseur remet l'analyseur en �tat sortie d'usine. Il est conseill� d'archiver les fourchettes de r�f�rence avant de r�tablir les valeurs par d�faut de l'analyseur. Les r�sultats enregistr�s et les archives des fourchettes de r�f�rence internes seront toujours disponibles une fois les param�tres par d�faut r�tablis.\n",																					// French
											L"Die Wiederherstellung der Werkseinstellungen des Analyseger�ts stellt die Einstellungen des Analyseger�ts auf den Auslieferungszustand zur�ck. Es wird empfohlen, die Referenzbereiche vor dem Zur�cksetzen der Referenzbereiche auf die Werkseinstellungen zu archivieren. Gespeicherte Ergebnisse und interne Referenzbereichsarchive sind auch nach Wiederherstellung der Werkseinstellungen verf�gbar.\n",														// German
											L"La restauraci�n del analizador a los valores predeterminados originales reestablece las mismas condiciones de f�brica del analizador. Se recomienda archivar los intervalos de referencia antes de reestablecer los valores predeterminados de f�brica del analizador. Los archivos de resultados y los intervalos de referencia internos estar�n disponibles despu�s de reestablecer los valores predeterminados originales del analizador.\n",					// Spanish
											L"Se si ripristinano le impostazioni predefinite, l'analizzatore ritorna alle condizioni in cui ha lasciato la fabbrica. Si raccomanda di archiviare i range di riferimento prima di resettare l'analizzatore sulle impostazioni di fabbrica. I risultati salvati e gli archivi dei range di riferimento interni saranno disponibili anche dopo aver ripristinato le impostazioni di fabbrica.\n",																	// Italian
											L"O restauro do analisador para os valores predeterminados de f�brica restaura o analisador para as mesmas condi��es do analisador quando saiu da f�brica. � recomendado arquivar os intervalos de refer�ncia antes de restaurar o analisador para os valores predeterminados de f�brica. Os ficheiros de resultados e dos intervalos de refer�ncia internos continuar�o dispon�veis ap�s o restauro do analisador para os valores predeterminados de f�brica.\n",	// Portuguese
											L"A restaura��o do analisador para os padr�es de f�brica o redefine para as mesmas condi��es de quando ele saiu da f�brica. � recomend�vel que os intervalos de refer�ncia sejam arquivados antes de redefinir o analisador para os padr�es de f�brica. Os resultados salvos e os arquivos do intervalo de refer�ncia interno ainda estar�o dispon�veis depois que as configura��es forem redefinidas para os padr�es de f�brica.\n"									// Portuguese (Brazil)
										} 
	},
	{ ISTR_HL7_SETTINGS				,	{	L"HL7 Format: Provides a list of EMR/EHR systems that have specific HL7 configuration as part of the analyzer settings. If your EMR/EHR system is not listed, try the Generic HL7 setting. If that does not work correctly, contact your EMR/EHR provider and Abaxis Technical Support to get them added to the list.\n",																					// English
											L"Format HL7 : R�pertorie les syst�mes EMR/EHR ayant une configuration HL7 sp�cifique au niveau des param�tres de l�analyseur. Si votre syst�me EMR/EHR n�est pas r�pertori�, essayer le param�tre HL7 g�n�rique. Si cela ne fonctionne pas correctement, contacter votre fournisseur EMR/EHR et le support technique d�Abaxis pour les ajouter � la liste.\n",												// French
											L"HL7-Format: Enth�lt eine Liste von EMR/EHR-Ger�ten mit spezifischer HL7-Konfiguration als Teil der Ger�teeinstellungen. Ist Ihr EMR/EHR-Ger�t nicht darin enthalten, so w�hlen Sie die allgemeine HL7-Einstellung. Ist auch dies nicht erfolgreich, wenden Sie sich an den Anbieter Ihres EMR/EHR-Ger�tes sowie den technischen Support von Abaxis, um das Ger�t der Liste hinzuf�gen zu lassen.\n",		// German 
											L"Formato HL7: Proporciona una lista de sistemas EMR/EHR que tienen una configuraci�n espec�fica de HL7 como parte de los ajustes del analizador. Si no aparece su sistema EMR/EHR en la lista, pruebe con el ajuste Gen�rico HL7. Si no funciona correctamente, p�ngase en contacto con su proveedor de EMR/EHR y con el servicio t�cnico de Abaxis para que lo a�adan a la lista.\n",						// Spanish 
											L"Formato HL7: Fornisce un elenco dei sistemi EMR/EHR che presentano configurazioni HL7 specifiche come parte delle impostazioni dell'analizzatore. Se il proprio sistema EMR/EHR non � nell'elenco, provare con le impostazioni HL7 generiche. Nel caso di funzionamento errato, contattare il proprio fornitore EMR/EHR e l'assistenza tecnica Abaxis per ottenere l'aggiunta all'elenco.\n", 				// Italian 
											L"Formato HL7: Fornece uma lista de sistemas EMR/EHR que t�m uma configura��o HL7 espec�fica como parte das configura��es do analisador.  Se o seu sistema EMR/EHR n�o aparecer na lista, tente a configura��o HL7 gen�rica.  Se n�o funcionar adequadamente, contacte o seu fornecedor de EMR/EHR e o servi�o de assist�ncia t�cnica da Abaxis para o adicionarem � lista.\n",								// Portuguese
											L"Formato HL7: Fornece uma lista de sistemas de EMR/EHR que possuem configura��o espec�fica do HL7 como parte das configura��es do analisador.  Se o sistema EMR/EHR n�o estiver listado, tente a configura��o HL7 gen�rica.  Se isso n�o funcionar corretamente, entre em contato com o provedor de EMR/EHR e o Suporte T�cnico da Abaxis para adicion�-lo � lista. \n"										// Portuguese (Brazil)
									  } 	
	},
	{ ISTR_LANGUAGE_SET				,	{	L"Both: Both the display and the external keyboard will be set to the same language at the same time.\n\n"
											L"Display: Only the language used on the display will be changed.\n\n"
											L"Keyboard: Only the keyboard type will be changed. The keyboard selected must match the keyboard used.\n",										// English
											L"Les deux : l'�cran et le clavier externe seront dans la m�me langue en m�me temps.\n\n"
											L"�cran : seule la langue utilis�e � l'�cran sera modifi�e.\n\n"
											L"Clavier : seule le type de clavier sera modifi�. Le clavier s�lectionn� doit correspondre au clavier utilis�.\n",								// French
											L"Beide: Anzeige und externes Eingabeger�t werden gleichzeitig auf dieselbe Sprache eingestellt.\n\n"
											L"Anzeige: Nur die Sprache der Anzeige wird ver�ndert.\n\n"
											L"Eingabeger�t: Nur der Eingabeger�tetyp wird ver�ndert. Das ausgew�hlte Eingabeger�t muss mit dem verwendeten Eingabeger�t �bereinstimmen.\n",	// German
											L"Ambos: se establecer�n simult�neamente tanto la pantalla como el teclado externo con el mismo idioma.\n\n"
											L"Pantalla: solo se cambiar� el idioma de la pantalla.\n\n"
											L"Teclado: solo cambiar� el tipo de teclado. El teclado seleccionado deber� coincidir con el teclado usado.\n",									// Spanish
											L"Entrambi: sia il display che la tastiera esterna saranno impostati sulla stessa lingua nello stesso momento.\n\n"
											L"Display: viene modificata solo la lingua utilizzata dal display.\n\n"
											L"Tastiera: viene modificato solo il tipo di tastiera. La tastiera selezionata deve coincidere con la tastiera utilizzata.\n",					// Italian
											L"Ambos: O visor e o teclado externo ser�o definidos simultaneamente com o mesmo idioma.\n\n"
											L"Visor: Apenas ser� alterado o idioma utilizado no visor.\n\n"
											L"Teclado: Apenas ser� alterado o tipo de teclado. O teclado selecionado dever� corresponder ao teclado utilizado.\n",							// Portuguese
											L"Ambos: O display e o teclado externo ser�o configurados para o mesmo idioma ao mesmo tempo.\n\n"
											L"Display: Apenas o idioma usado no display ser� alterado.\n\n"
											L"Teclado: Apenas o tipo de teclado ser� alterado. O teclado selecionado deve corresponder ao teclado usado.\n"									// Portuguese (Brazil)
										} 
	},
	{ ISTR_NETWORK_TYPE				,	{	L"Static IP: The network settings are set manually and will not change when the analyzer is powered on.  Please contact your IT group for the correct settings for your organization.\n\n"
											L"DHCP: The network settings are obtained automatically by the analyzer from a DHPC server on the network.\n\n"
											L"Both: The ASTM and HL7 settings need to be set manually.  Please contact your IT group for the correct settings for your organization.\n" ,		// English
											L"IP statique : Les param�tres du r�seau sont d�finis manuellement et ne seront pas modifi�s � la mise en marche de l�analyseur. Veuillez contacter votre groupe informatique pour obtenir les bons param�tres pour votre organisation.\n\n"
											L"DHCP : L�analyseur obtient automatiquement les param�tres du r�seau depuis un serveur DHCP du r�seau.\n\n"
											L"Les deux : Les param�tres ASTM et HL7 doivent �tre d�finis manuellement. Veuillez contacter votre groupe informatique pour obtenir les bons param�tres pour votre organisation.\n",     //French
											L"Statische IP: Die Netzwerkeinstellungen werden manuell vorgenommen und bleiben bei Einschalten des Analyseger�tes unver�ndert. Wenden Sie sich an Ihre IT-Abteilung, um die korrekten Einstellungen f�r Ihr Unternehmen zu erfahren.\n\n"
											L"DHCP: Die Netzwerkeinstellungen werden automatisch vom Analyseger�t �ber einen DHCP-Netzwerkserver abgerufen.\n\n"
											L"Beide: Die Einstellungen f�r ASTM und HL7 sind manuell vorzunehmen. Wenden Sie sich an Ihre IT-Abteilung, um die korrekten Einstellungen f�r Ihr Unternehmen zu erfahren.\n",			// German		
											L"IP est�tica: Los ajustes de red se definen manualmente y no cambiar�n cuando se encienda el analizador. P�ngase en contacto con su departamento de inform�tica para elegir los ajustes adecuados para su organizaci�n.\n\n"
											L"DHCP: Los ajustes de red se obtienen autom�ticamente mediante el analizador de un servidor DHCP de la red.\n\n"
											L"Ambos: Los ajustes de ASTM y HL7 se tienen que definir manualmente. P�ngase en contacto con su departamento de inform�tica para elegir los ajustes adecuados para su organizaci�n.\n",	// Spanish	
											L"IP statico: Le impostazioni di rete sono impostate manualmente e non verranno modificate all'accensione dell'analizzatore. Contattare il gruppo IT per le impostazioni corrette per la propria azienda.\n\n"
											L"DHCP: Le impostazioni di rete vengono ottenute automaticamente mediante l'analizzatore da un server DHCP sulla rete.\n\n"
											L"Entrambi: Sia l'ASTM sia l'HL7 devono essere impostati manualmente. Contattare il gruppo IT per le impostazioni corrette per la propria azienda.\n",									// Italian
											L"IP est�tico: As configura��es de rede s�o configuradas manualmente e n�o se alterar�o quando o analisador for ligado.  Contacte o seu Departamento de TI para selecionar as configura��es adequadas para a sua organiza��o.\n\n"
											L"DHCP: As configura��es de rede s�o obtidas automaticamente pelo analisador atrav�s de um servidor DHCP da rede. \n\n"
											L"Ambos: � necess�rio definir manualmente as configura��es ASTM e HL7.  Contacte o seu Departamento de TI para selecionar as configura��es adequadas para a sua organiza��o.\n",		// Portuguese
											L"IP est�tico: As configura��es de rede s�o configuradas manualmente e n�o ser�o alteradas quando o analisador estiver ligado.  Entre em contato com o grupo de TI para obter as configura��es corretas para sua organiza��o.\n\n"
											L"DHCP: As configura��es de rede s�o obtidas automaticamente pelo analisador de um servidor DHPC na rede.\n\n"
											L"Ambos: As configura��es ASTM e HL7 precisam ser configuradas manualmente.  Entre em contato com o grupo de TI para obter as configura��es corretas para sua organiza��o.\n"		// Portuguese (Brazil)	
									    }
 
	},
	{ ISTR_PRINTER_CONFIGURE		,	{	L"Set Default: Select the printer to use for printing.\n\n"
											L"Select Reports: Select the type and number of reports.\n",									// English
											L"Imprimante par d�faut : s�lectionner l'imprimante � utiliser pour l'impression.\n\n"
											L"S�lectionner des rapports : s�lectionner le type et le nombre de rapports.\n",				// French
											L"Standard einstellen: Drucker ausw�hlen.\n\n"
											L"Berichte ausw�hlen: Typ und Zahl der Berichte ausw�hlen.\n",								// German
											L"Establecer valores predeterminados: seleccione la impresora que se va a utilizar.\n\n"
											L"Seleccionar informes: seleccione el tipo y n�mero de informes.\n",							// Spanish
											L"Imposta come predefinito: seleziona la stampante da utilizzare per la stampa.\n\n"
											L"Seleziona rapporti: seleziona il tipo e il numero di rapporti.\n",							// Italian
											L"Configurar valores predeterminados: Selecione a impressora a utilizar para imprimir.\n\n"
											L"Selecionar relat�rios: Selecione o tipo e o n�mero de relat�rios.\n",						// Portuguese
											L"Configurar padr�o: Selecione a impressora a ser usada para a impress�o.\n\n"
											L"Selecionar relat�rios: Selecione o tipo e o n�mero de relat�rios.\n"						// Portuguese (Brazil)
										} 
	},
	{ ISTR_PRINTER_SELECT_REPORTS	,	{	L"Results: Select the number of results reports to print at the end of each analysis.\n\n"
											L"iQC: Select the number of iQC reports to print at the end of each analysis.\n\n"
											L"Error: Select the number of error reports to print at the end of each analysis. The reports can be printed automatically when there is an error, or may be printed for every analysis.\n",								// English
											L"R�sultats : s�lectionner le nombre de rapports de r�sultats � imprimer � l'issue de chaque analyse.\n\n"
											L"iQC : s�lectionner le nombre de rapports iQC � imprimer � l'issue de chaque analyse.\n\n"
											L"Erreur : s�lectionner le nombre de rapports d'erreurs � imprimer � l'issue de chaque analyse. Des rapports peuvent s'imprimer automatiquement en cas d'erreur ou bien pour chaque analyse.\n",							// French
											L"Ergebnisse: Anzahl der nach Abschluss jeder Analyse zu druckenden Ergebnisberichte ausw�hlen.\n\n"
											L"iQC: Anzahl der nach Abschluss jeder Analyse zu druckenden iQC-Berichte ausw�hlen.\n\n"
											L"Fehler: Anzahl der nach Abschluss jeder Analyse zu druckenden Fehlerberichte ausw�hlen. Bei Vorliegen eines Fehlers k�nnen die Berichte automatisch oder f�r jede Analyse gedruckt werden.\n",							// German
											L"Resultados: seleccione el n�mero de informes de resultados para imprimir al final de cada an�lisis.\n\n"
											L"iQC: seleccione el n�mero de informes de iQC para imprimir al final de cada an�lisis.\n\n"
											L"Error: seleccione el n�mero de informes de error para imprimir al final de cada an�lisis. Los informes se pueden imprimir autom�ticamente cuando hay un error, o se pueden imprimir con cada an�lisis.\n",				// Spanish
											L"Risultati: seleziona il numero di rapporti di risultati da stampare al termine di ogni analisi.\n\n"
											L"iQC: seleziona il numero di rapporti di iQC da stampare al termine di ogni analisi.\n\n"
											L"Errore: seleziona il numero di rapporti di errore da stampare al termine di ogni analisi. I rapporti possono essere stampati automaticamente in caso di errore, oppure essere stampati per ogni analisi.\n",			// Italian
											L"Resultados: Seleccone o n�mero de relat�rios de resultados a imprimir no final de cada an�lise.\n\n"
											L"iQC: Selecione o n�mero de relat�rios de iQC a imprimir no final de cada an�lise.\n\n"
											L"Erro: Selecione o n�mero de relat�rios de erros a imprimir no final de cada an�lise. Os relat�rios podem ser impressos automaticamente quando existir um erro, ou podem ser impressos para cada an�lise.\n",			// Portuguese
											L"Resultados: Selecione o n�mero de relat�rios de resultados a serem impressos no final de cada an�lise.\n\n"
											L"iQC: Selecione o n�mero de relat�rios de iQC a serem impressos no final de cada an�lise.\n\n"
											L"Erro: Selecione o n�mero de relat�rios de erro a serem impressos no final de cada an�lise. Os relat�rios podem ser impressos automaticamente quando houver um erro ou podem ser impressos para todas as an�lises.\n"	// Portuguese (Brazil)
										} 
	},
	{ ISTR_PRINTER_SETUP			,	{	L"Configure: Select the printer to use and the number and type of reports to print.\n\n"
											L"Test: Print a test page on the printer.\n",																			// English
											L"Configuration : s�lectionner l'imprimante � utiliser, le nombre et le type de rapports � imprimer.\n\n"
											L"Test : imprimer une page de test sur l'imprimante.\n",																	// French
											L"Konfigurieren: Verwendeten Drucker sowie Zahl und Typ der zu druckenden Berichte ausw�hlen.\n\n"
											L"Test: Testseite drucken.\n",																							// German
											L"Configurar: seleccione la impresora que se va a utilizar y el n�mero y tipo de informes que se van a imprimir.\n\n"
											L"Test: imprima una p�gina de prueba en la impresora.\n",																// Spanish
											L"Configura: seleziona la stampante da usare, nonch� il numero e il tipo di rapporti da stampare.\n\n"
											L"Prova: stampa una pagina di prova.\n",																					// Italian
											L"Configurar: Selecione a impressora a utilizar e o n�mero e tipo de relat�rios a imprimir.\n\n"
											L"Teste: Imprima uma p�gina de teste na impressora.\n",																	// Portuguese
											L"Configurar: Selecione a impressora a ser usada e o n�mero e tipo de relat�rios a serem impressos.\n\n"
											L"Teste: Imprima uma p�gina de teste na impressora.\n"																	// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_ADD			,	{	L"Special n: The next available generic demographic label between 1 and 99.\n\n"
											L"Control n: The next available control label between 1 and 99.\n\n",													// English
											L"Sp�cial n : la prochaine �tiquette d�mographique g�n�rique disponible comprise entre 1 et 99.\n\n"
											L"Contr�le n : la prochaine �tiquette de contr�le disponible comprise entre 1 et 99.\n",									// French
											L"Spezielles n: Das n�chste verf�gbare Demographie-Etikett zwischen 1 und 99.\n\n"
											L"Steuerung n: Das n�chste verf�gbare Steuerungsetikett zwischen 1 und 99.\n",											// German
											L"Especial n: la pr�xima etiqueta de demograf�a gen�rica disponible entre 1 y 99.\n\n"
											L"Control n: la pr�xima etiqueta de control disponible entre 1 y 99.\n",													// Spanish
											L"Speciale n: la successiva etichetta di demografia generica disponibile tra 1 e 99.\n\n"
											L"Controllo n: la successiva etichetta di controllo disponibile tra 1 e 99.\n",											// Italian
											L"Especial n: A pr�xima etiqueta de demografia gen�rica dispon�vel entre 1 e 99.\n\n"
											L"Controlo n: A pr�xima etiqueta de controlo dispon�vel entre 1 e 99.\n",												// Portuguese
											L"N especial: O pr�ximo r�tulo de dado demogr�fico gen�rico dispon�vel entre 1 e 99.\n\n"
											L"N controle: O pr�ximo r�tulo de controle dispon�vel entre 1 e 99.\n"													// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_INCLUDE		,	{	L"Last Modified XXX: The last reference range modified.\n\n"
											L"All: Allows the user to select from a list of all reference ranges known to the analyzer.\n",														// English
											L"Derni�re modification XXX : la derni�re fourchette de r�f�rence modifi�e.\n\n"
											L"Tous : permet � l'utilisateur d'effectuer une s�lection � partir de la liste de toutes les fourchettes de r�f�rences connues de l'analyseur.\n",	// French
											L"Letzte �nderung XXX: Letzter ge�nderter Referenzbereich.\n\n"
											L"Alle: Erm�glicht dem Benutzer die Auswahl aus einer Liste aller im Analyseger�t verf�gbaren Referenzbereiche.\n",									// German
											L"�ltima modificaci�n XXX: el �ltimo intervalo de referencia modificado.\n\n"
											L"Todo: permite al usuario elegir de una lista de todos los intervalos de referencia conocidos por el analizador.\n",								// Spanish
											L"Ultima modifica XXX: l'ultimo range di riferimento modificato.\n\n"
											L"Tutto: consente all'utente di selezionare da un elenco tutti i range di riferimento noti all'analizzatore.\n",										// Italian
											L"�ltima modifica��o XXX: O �ltimo intervalo de refer�ncia modificado.\n\n"
											L"Tudo: Permite ao utilizador selecionar a partir de uma lista com todos os intervalos de refer�ncia conhecidos para o analisador.\n",				// Portuguese
											L"�ltima modifica��o de XXX: O �ltimo intervalo de refer�ncia modificado.\n\n"
											L"Todos: Permite ao usu�rio selecionar em uma lista de todos os intervalos de refer�ncia conhecidos pelo analisador.\n"								// Portuguese (Brazil)
										}
	},
	{ ISTR_REF_RANGES_MODIFY		,	{	L"Analyte: Provides a list of all active reference ranges for a particular analyte.\n\n"
											L"Demographic: Provides a list of all analytes for all reference ranges.\n",																			// English
											L"Substance � analyser : fournit une liste de toutes les fourchettes de r�f�rence actives pour une substance � analyser donn�e.\n\n"
											L"�l�ment d�mographique : fournit une liste de toutes les substances � analyser pour toutes les fourchettes de r�f�rence.\n",						// French
											L"Analyt: Anzeige einer Liste aller aktiven Referenzbereiche f�r ein bestimmtes Analyt.\n\n"
											L"Demographie: Anzeige einer Liste aller Analyte f�r alle Referenzbereiche.\n",																		// German
											L"Analito: proporciona una lista de todos los intervalos de referencia activos para un analito particular.\n\n"
											L"Demograf�a: proporciona una lista de todos los analitos para todos los intervalos de referencia.\n",												// Spanish
											L"Analita: fornisce un elenco di tutti i range di riferimento attivi per un particolare analita.\n\n"
											L"Demografia: fornisce un elenco di tutti gli analiti per ogni range di riferimento.\n",																// Italian
											L"Analito: Fornece uma lista de todos os intervalos de referencia ativos para um analito espec�fico.\n\n"
											L"Demografia: Fornece uma lista de todos os analitos para todos os intervalos de refer�ncia.\n",														// Portuguese
											L"Analito: Fornece uma lista de todos os intervalos de refer�ncia ativos para um determinado analito.\n\n"
											L"Dados demogr�ficos: Fornece uma lista de todos os analitos para todos os intervalos de refer�ncia.\n"												// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_REMOVE		,	{	L"Removing a demographic from the sample type list will remove the item from the list of demographics shown when running a disc, and any values entered will be permanently lost.\n",																						// English
											L"La suppression d'un �l�ment d�mographique dans la liste de type d'�chantillons a pour effet de supprimer cet �l�ment dans la liste d'�l�ments d�mographiques affich�s lors de l'ex�cution d'un disque, et toutes les valeurs saisies seront irr�m�diablement perdues.\n",	// French
											L"Die Entfernung einer Demographie aus der Probentypliste entfernt das Objekt aus der Demographieliste, die bei laufendem Rotor angezeigt wird, und s�mtliche eingegebene Werte gehen dauerhaft verloren.\n",																// German
											L"Al eliminar una demograf�a de la lista de tipo de muestra se eliminar� la entrada de la lista de demograf�as mostrada al usar el disco y cualquier valor introducido se perder� permanentemente.\n",																		// Spanish
											L"Rimuovendo una demografia dall'elenco dei tipi si rimuover� quella voce dall'elenco di demografie proposto quando un disco � in funzione e tutti i valori immessi saranno persi in modo permanente.\n",																	// Italian
											L"Ao eliminar uma demografia da lista de tipo de amostra, ser� eliminado o item da lista de demografias exibida quando se utiliza um disco, e qualquer valor introduzido ser� permanentemente perdido.\n",																	// Portuguese
											L"A remo��o de um dado demogr�fico da lista de tipos de amostra remover� o item da lista de dados demogr�ficos mostrada durante a execu��o de um disco e quaisquer valores inseridos ser�o permanentemente perdidos.\n"														// Portuguese (Brazil)
										} 
	},
	{ ISTR_SECURITY_LOCKOUT			,	{	L"Analyze: When checked, only a authorized operator can open the drawer at the start or the end of an analysis.\n\n"
											L"Recall Results: When checked, only a authorized operator can recall results.\n\n"
											L"Settings: When checked, only a authorized operator can change settings.\n",																// English
											L"Analyse : lors de la v�rification, seul un op�rateur autoris� peut ouvrir le tiroir au d�but ou � la fin d'une analyse.\n\n"
											L"Rappeler r�sultats : lors de la v�rification, seul un op�rateur autoris� peut rappeler les r�sultats.\n\n"
											L"Param�tres : lors de la v�rification, seul un op�rateur autoris� peut modifier les param�tres.\n",											// French
											L"Analysieren: Wenn aktiviert, kann nur ein autorisierter Bediener das Fach bei Start oder Ende einer Analyse �ffnen. \n\n"
											L"Ergebnisse erneut abrufen: Wenn aktiviert, kann nur ein autorisierter Bediener Ergebnisse erneut abrufen.\n\n"
											L"Einstellungen: Wenn aktiviert, kann nur ein autorisierter Bediener Einstellungen �ndern.\n",												// German
											L"Analizar: cuando se activa, solo un operario autorizado puede abrir el caj�n al comienzo o al final de un an�lisis.\n\n"				
											L"Buscar resultados: cuando se activa, solo un operario autorizado puede buscar los resultados.\n\n"
											L"Configuraci�n: cuando se activa, solo un operario autorizado puede cambiar la configuraci�n.\n",											// Spanish
											L"Analizzare: quando spuntato, esclusivamente un operatore autorizzato pu� aprire il cassetto all'inizio o alla fine di un'analisi.\n\n"
											L"Recupera i risultati: quando spuntato, esclusivamente un operatore autorizzato pu� recuperare i risultati.\n\n"
											L"Impostazioni; quando spuntato, esclusivamente un operatore autorizzato pu� modificare le impostazioni.\n",									// Italian
											L"Analisar: Quando selecionado, apenas um operador autorizado pode abrir a gaveta no in�cio ou no final de uma an�lise.\n\n"
											L"Recuperar resultados: Quando selecionado, apenas um operador autorizado pode recuperar os resultados.\n\n"
											L"Configura��es: Quando selecionado, apenas um operador autorizado pode alterar as configura��es.\n",										// Portuguese
											L"Analisar: Quando marcado, apenas um operador autorizado pode abrir a gaveta no in�cio ou no final de uma an�lise.\n\n"
											L"Recuperar resultados: Quando marcado, somente um operador autorizado pode recuperar os resultados.\n\n"
											L"Configura��es: Quando marcado, apenas um operador autorizado pode alterar as configura��es.\n"												// Portuguese (Brazil)
										}	
	},
	{ ISTR_SECURITY_SETTINGS		,	{	L"Administrators: Can access the settings in the second settings screen. Security features take effect when the first administrator is added.  Security features are disabled when all administrators are deleted.\n\n"
											L"Operators: Authorized operators for the analyzer. Adding an operator to the list will turn on the Operator ID prompt when analyzing a sample.\n\n"
											L"Controls: The date that a new set of control samples must be run by. Analyzing a sample after this date will result in a warning when the sample is analyzed.\n",																																	// English
											L"Administrateurs : ils peuvent acc�der aux param�tres sur le second �cran de param�tres. Les caract�ristiques de s�curit� entrent en vigueur lors de l'ajout du premier administrateur. Les caract�ristiques de s�curit� sont d�sactiv�es lorsque tous les administrateurs sont supprim�s.\n\n"
											L"Op�rateurs : op�rateurs autoris�s pour l'analyseur. L'ajout d'un op�rateur � la liste a pour effet d'activer un message d'invitation � saisir l'ID op�rateur lors de l'analyse d'un �chantillon.\n\n"
											L"Contr�les : date butoir � laquelle un nouvel ensemble d'�chantillons de contr�le doit avoir �t� ex�cut�. Le fait d'analyser un �chantillon apr�s cette date entra�ne un message d'avertissement lors de l'analyse de l'�chantillon.\n",															// French
											L"Administrator: Haben Zugang zu den Einstellungen in dem Zweiteinstellungsbildschirm. Sicherheitsmerkmale werden bei Hinzuf�gung des ersten Administrators wirksam. Sicherheitsmerkmale werden bei L�schung aller Administratoren deaktiviert.\n\n"
											L"Bediener: Befugte Bediener des Analyseger�ts. Die Hinzuf�gung eines Bedieners zur Liste aktiviert bei Analyse einer Probe die Aufforderung zur Eingabe der Bediener-ID.\n\n"
											L"Steuerungen: Das Datum, zu dem ein neuer Satz von Kontrollproben vorgenommen werden muss. Die Analyse einer Probe nach diesem Datum f�hrt zu einem Warnhinweis bei Analyse der Probe.\n",																											// German
											L"Administradores: pueden acceder a la configuraci�n en la segunda pantalla de configuraciones. Las medidas de seguridad empiezan a adoptarse despu�s de a�adir el primer administrador. Las medidas de seguridad est�n inactivas al eliminar a todos los administradores.\n\n"
											L"Operarios: operarios autorizados para uso del analizador. Al a�adir un operario a la lista se activar� un mensaje de identificaci�n de operario al analizar la muestra.\n\n"
											L"Controles: la fecha en la que se debe usar un nuevo grupo de muestras de control. El an�lisis de una muestra despu�s de esta fecha motivar� un mensaje de aviso durante el an�lisis.\n",																											// Spanish
											L"Amministratori: possono accedere alle impostazioni nel secondo schermo di impostazioni. Le impostazioni di sicurezza entrano in vigore quando si aggiunge il primo amministratore. Le impostazioni di sicurezza sono disattivate quando tutti gli amministratori sono cancellati.\n\n"
											L"Operatori: operatori autorizzati all'uso dell'analizzatore. Se si aggiunge un operatore all'elenco, durante l'analisi di un campione viene attivato il prompt \"ID operatore\".\n\n"
											L"Controlli: la data in cui deve essere effettuata una nuova serie di controlli campione. Se l'analisi di un campione viene eseguita dopo tale data, verr� visualizzato un messaggio di attenzione.\n",																								// Italian
											L"Administradores: Podem aceder �s configura��es no segundo ecr� de configura��es. Os recursos de seguran�a tornam-se efetivos quando o primeiro administrador � adicionado.  Os recursos de seguran�a ficam inativos quando todos os administradores s�o eliminados.\n\n"
											L"Operadores: Operadores autorizados para utiliza��o do analisador. Adicionar um operador � lista ir� ativar um pedido de ID do operador ao analisar a amostra.\n\n"
											L"Controlos: A data na qual se deve utilizar um grupo novo de amostras controlo. A an�lise de uma amostra ap�s esta data resultar� numa mensagem de aviso durante a an�lise.\n",																														// Portuguese
											L"Administradores: Podem acessar as configura��es na segunda tela de configura��es. Os recursos de seguran�a entram em vigor quando o primeiro administrador � adicionado.  Os recursos de seguran�a s�o desativados quando todos os administradores s�o exclu�dos.\n\n"
											L"Operadores: Operadores autorizados para o analisador. Adicionar um operador � lista ativar� o prompt do ID do operador ao analisar uma amostra.\n\n"
											L"Controles: A data at� a qual um novo conjunto de amostras de controle deve ser executado. A an�lise de uma amostra ap�s essa data resultar� em um aviso quando a amostra for analisada.\n"																											// Portuguese (Brazil)
										} 
	},
	{ ISTR_SECURITY_SETTINGS_LOCKOUT,	{	L"Administrators: Can access the settings in the second settings screen. Security features take effect when the first administrator is added.  Security features are disabled when all administrators are deleted.\n\n"
											L"Operators: Authorized operators for the analyzer. Adding an operator to the list will turn on the Operator ID prompt when analyzing a sample.\n\n"
											L"Controls: The date that a new set of control samples must be run by. Analyzing a sample after this date will result in a warning when the sample is analyzed.\n\n"
											L"Lockout: Limits use of the analyzer functions to authorized operators.  Create authorized operators prior to enabling lockout.\n",																																									// English
											L"Administrateurs : ils peuvent acc�der aux param�tres sur le second �cran de param�tres. Les caract�ristiques de s�curit� entrent en vigueur lors de l'ajout du premier administrateur. Les caract�ristiques de s�curit� sont d�sactiv�es lorsque tous les administrateurs sont supprim�s.\n\n"
											L"Op�rateurs : op�rateurs autoris�s pour l'analyseur. L'ajout d'un op�rateur � la liste a pour effet d'activer un message d'invitation � saisir l'ID op�rateur lors de l'analyse d'un �chantillon.\n\n"
											L"Contr�les : date butoir � laquelle un nouvel ensemble d'�chantillons de contr�le doit avoir �t� ex�cut�. Le fait d'analyser un �chantillon apr�s cette date entra�ne un message d'avertissement lors de l'analyse de l'�chantillon.\n\n"
											L"Verrouillage : limite l'utilisation des fonctions de l'analyseur aux op�rateurs autoris�s. Les op�rateurs autoris�s doivent �tre cr��s avant d'autoriser le verrouillage.\n",																														// French
											L"Administrator: Haben Zugang zu den Einstellungen in dem Zweiteinstellungsbildschirm. Sicherheitsmerkmale werden bei Hinzuf�gung des ersten Administrators wirksam. Sicherheitsmerkmale werden bei L�schung aller Administratoren deaktiviert.\n\n"
											L"Bediener: Befugte Bediener des Analyseger�ts. Die Hinzuf�gung eines Bedieners zur Liste aktiviert bei Analyse einer Probe die Aufforderung zur Eingabe der Bediener-ID.\n\n"
											L"Steuerungen: Das Datum, zu dem ein neuer Satz von Kontrollproben vorgenommen werden muss. Die Analyse einer Probe nach diesem Datum f�hrt zu einem Warnhinweis bei Analyse der Probe.\n\n"
											L"Verriegelung: Beschr�nkt die Verwendung von Analyseger�tfunktionen auf autorisierte Bediener. Erstellen Sie autorisierte Bediener bevor Sie die Verriegelung aktivieren.\n",																														// German
											L"Administradores: pueden acceder a la configuraci�n en la segunda pantalla de configuraci�n. Las caracter�sticas de seguridad se activan cuando se agrega el primer administrador y se desactivan si se eliminan todos los administradores.\n\n"
											L"Operarios: operarios autorizados a utilizar el analizador. Al a�adir un operario a la lista, se preguntar� el ID del operario al analizar una muestra.\n\n"
											L"Controles: fecha en la que se debe ejecutar un nuevo conjunto de muestras de control. Si se analiza una muestra despu�s de esta fecha, aparecer� una advertencia cuando se analice la muestra.\n\n"
											L"Bloqueo: limita el uso de las funciones del analizador a los operarios autorizados. Debe crear operarios autorizados antes de activar el bloqueo.\n",																																				// Spanish
											L"Amministratori: possono accedere alle impostazioni nel secondo schermo di impostazioni. Le impostazioni di sicurezza entrano in vigore quando si aggiunge il primo amministratore. Le impostazioni di sicurezza sono disattivate quando tutti gli amministratori sono cancellati.\n\n"
											L"Operatori: operatori autorizzati all'uso dell'analizzatore. Se si aggiunge un operatore all'elenco, durante l'analisi di un campione viene attivato il prompt \"ID operatore\".\n\n"
											L"Controlli: la data in cui deve essere effettuata una nuova serie di controlli campione. Se l'analisi di un campione viene eseguita dopo tale data, verr� visualizzato un messaggio di attenzione.\n\n",																							
											L"Serrata: limita l'uso delle funzioni dell'analizzatore agli operatori autorizzati.  Creare operatori autorizzati prima di abilitare la serrata.\n"																																					// Italian
											L"Administradores: Podem aceder �s configura��es no segundo ecr� de configura��es. Os recursos de seguran�a tornam-se efetivos quando o primeiro administrador � adicionado.  Os recursos de seguran�a ficam inativos quando todos os administradores s�o eliminados.\n\n"
											L"Operadores: Operadores autorizados para utiliza��o do analisador. Adicionar um operador � lista ir� ativar um pedido de ID do operador ao analisar a amostra.\n\n"
											L"Controlos: A data na qual se deve utilizar um grupo novo de amostras controlo. A an�lise de uma amostra ap�s esta data resultar� numa mensagem de aviso durante a an�lise.\n\n"
											L"Bloqueio: Limita a utiliza��o das fun��es do analisador aos operadores autorizados.  Deve criar operadores autorizados antes de ativar o bloqueio.\n",																																				// Portuguese
											L"Administradores: Podem acessar as configura��es na segunda tela de configura��es. Os recursos de seguran�a entram em vigor quando o primeiro administrador � adicionado.  Os recursos de seguran�a s�o desativados quando todos os administradores s�o exclu�dos.\n\n"
											L"Operadores: Operadores autorizados para o analisador. Adicionar um operador � lista ativar� o prompt do ID do operador ao analisar uma amostra.\n\n"
											L"Controles: A data at� a qual um novo conjunto de amostras de controle deve ser executado. A an�lise de uma amostra ap�s essa data resultar� em um aviso quando a amostra for analisada.\n\n"
											L"Bloquear: Limita o uso das fun��es do analisador a operadores autorizados.  Crie operadores autorizados antes de ativar o bloqueio.\n"																																								// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_ELECTROLYTE_UNITS	,	{	L"Selecting a unit on this screen will change the units for CL, K, NA and tCO2 simultaneously to the same units.\n",										// English
											L"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � CL, K, NA et tCO2.\n",																// French
											L"Mit der Auswahl einer Einheit in diesem Men� werden automatisch die Einheiten f�r CL, K, NA und tCO2 auf diese Einheit umgestellt.\n",					// German
											L"Al seleccionar una unidad en esta pantalla, se cambiar� CL, K, NA y tCO2 simult�neamente a la misma unidad.\n",										// Spanish
											L"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per CL, K, NA e tCO2 nelle stesse unit�.\n",							// Italian
											L"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades CL, K, NA e tCO2 para as mesmas unidades.\n",						// Portuguese
											L"A sele��o de uma unidade nessa tela mudar� as unidades para CL, K, NA e tCO2 simultaneamente para as mesmas unidades.\n"								// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_ENZYME_UNITS			,	{	L"Selecting a unit on this screen will change the units for ALT, ALP, AMY, AST, CK, GGT and LD simultaneously to the same units.\n",						// English
											L"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � ALT, ALP, AMY, AST, CK, GGT et LD.\n",												// French
											L"Mit der Auswahl einer Einheit in diesem Men� werden automatisch die Einheiten f�r ALT, ALP, AMY, AST, CK, GGT und LD auf diese Einheit umgestellt.\n",	// German
											L"Al seleccionar una unidad en esta pantalla, se cambiar� ALT, ALP, AMY, AST, CK, GGT y LD simult�neamente a la misma unidad.\n",						// Spanish
											L"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per ALT, ALP, AMY, AST, CK, GGT ed LD nelle stesse unit�.\n",			// Italian
											L"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades ALT, ALP, AMY, AST, CK, GGT e LD para as mesmas unidades.\n",		// Portuguese
											L"A sele��o de uma unidade nessa tela mudar� as unidades para ALT, ALP, AMY, AST, CK, GGT e LD simultaneamente para as mesmas unidades.\n"				// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_LIPID_UNITS			,	{	L"Selecting a unit on this screen will change the units for CHOL, HDL, LDL, TRIG and VLDL simultaneously to the same units.\n",							// English
											L"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � CHOL, LHD, LBD, TRIG et LTBD.\n",													// French
											L"Mit der Auswahl einer Einheit in diesem Men� werden automatisch die Einheiten f�r CHOL, HDL, LDL, TRIG und VLDL auf diese Einheit umgestellt.\n",		// German
											L"Al seleccionar una unidad en esta pantalla, se cambiar� COL, HDL, LDL, TRIG y PMBD simult�neamente a la misma unidad.\n",								// Spanish
											L"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per COL, HDL, LDL, TRIG e VLDL nelle stesse unit�.\n",					// Italian
											L"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades CHOL, HDL, LDL, TRIG e VLDL para as mesmas unidades.\n",				// Portuguese
											L"A sele��o de uma unidade nessa tela mudar� as unidades para CHOL, HDL, LDL, TRIG e VLDL simultaneamente para as mesmas unidades.\n"					// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_MINERAL_UNITS		,	{	L"Selecting a unit on this screen will change the units for CA and MG simultaneously to the same units.\n",												// English
											L"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � CA et MG.\n",																		// French
											L"Mit der Auswahl einer Einheit in diesem Men� werden automatisch die Einheiten f�r CA und MG auf diese Einheit umgestellt.\n",							// German
											L"Al seleccionar una unidad en esta pantalla, se cambiar� CA y MG simult�neamente a la misma unidad.\n",													// Spanish
											L"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per CA ed MG nelle stesse unit�.\n",									// Italian
											L"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades CA e MG para as mesmas unidades.\n",									// Portuguese
											L"A sele��o de uma unidade nessa tela mudar� as unidades para CA e MG simultaneamente para as mesmas unidades.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_PROTEIN_UNITS		,	{	L"Selecting a unit on this screen will change the units for ALB and TP simultaneously to the same units.\n",												// English
											L"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � ALB et TP.\n",																		// French
											L"Mit der Auswahl einer Einheit in diesem Men� werden automatisch die Einheiten f�r ALB und TP auf diese Einheit umgestellt.\n",							// German
											L"Al seleccionar una unidad en esta pantalla, se cambiar� ALB y TP simult�neamente a la misma unidad.\n",												// Spanish
											L"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per ALB e TP nelle stesse unit�.\n",									// Italian
											L"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades ALB e TP para as mesmas unidades.\n",								// Portuguese
											L"A sele��o de uma unidade nessa mudar� as unidades para ALB e TP simultaneamente para as mesmas unidades.\n"											// Portuguese (Brazil)
										} 
	},
	{ ISTR_SOUND_SETTINGS			,	{	L"Screen Click: Sets the volume level of the sound made when touching a button on the display.\n\n"
											L"Alerts: Sets the volume level when a warning or error screen is displayed.\n\n"
											L"Status: Sets the volume level for status sounds.\n",																									// English
											L"Clic �cran : cette fonction d�finit le volume sonore lors de l'activation d'un bouton � l'�cran.\n\n"
											L"Avertisseurs sonores : cette fonction d�finit le volume lors de l'affichage d'un avertissement ou d'un �cran d'erreur.\n\n"
											L"�tat : d�finit le volume sonore pour les indicateurs d'�tat.\n",																						// French
											L"Bildschirmklick: Einstellung der Tonlautst�rke bei Ber�hrung einer Taste auf der Anzeige.\n\n"
											L"Alarme: Einstellung der Tonlautst�rke bei Erscheinen der Warn- oder Fehleranzeige.\n\n"
											L"Status: Einstellung der Lautst�rke f�r Statust�ne.\n",																									// German
											L"Clic de pantalla: establece el volumen del sonido que se hace cuando se toca un bot�n en la pantalla.\n\n"
											L"Alertas: establece el volumen del sonido que se hace cuando aparece una ventana de aviso o error.\n\n"
											L"Estado: establece el volumen del sonido de estado.\n",																									// Spanish
											L"Clic schermo: imposta il livello del volume del suono che si sente quando si tocca un pulsante sullo schermo.\n\n"
											L"Allarmi: imposta il livello del volume dei messaggi di attenzione o errore visualizzati a schermo.\n\n"
											L"Stato: imposta il livello del volume per i suoni di stato.\n",																							// Italian
											L"Clique no ecr�: Define o n�vel de volume do som que se ouve quando se toca uma tecla no visor.\n\n"
											L"Alertas: Define o n�vel de volume do som que se ouve quando � visualizado um ecr� de aviso ou erro.\n\n"
											L"Estado: Define o n�vel de volume para os sons de estado.\n",																							// Portuguese
											L"Clique na tela: Configura o n�vel de volume do som produzido ao tocar em um bot�o no visor.\n\n"
											L"Alertas: Configura o n�vel do volume quando uma tela de aviso ou erro � exibida.\n\n"
											L"Status: Configura o n�vel de volume para os sons de status.\n"																							// Portuguese (Brazil)
										} 
	},
	{ ISTR_TCPIP_SETTINGS			,	{	L"Change Settings: Change the TCP/IP Settings where Analyzer IP is the Static IP Address for the analyzer, the Subnet Mask is the IP Address mask for the analyzer, the Gateway IP is the IP Address of the router for the analyzer to reach other networks, the Server IP is the IP address of the server that will host the ASTM connection, and the Server Socket is the Socket (Port) on the Server that will be used for the ASTM TCP/IP connection.\n\n"
											L"Check Settings: The analyzer will attempt to make an ASTM connection using the settings under the Change Settings button.  The analyzer will display the result of the connection attempt.  Ensure the ASTM server is running and configured to accept a connection before checking the settings.\n",																																																																												// English																																																																																																																																																																																																																												// English                 					
											L"Modifier les param�tres : modifier les param�tres TCP/IP si le champ Adresse IP de l'analyseur est renseign� par l'adresse IP statique de l'appareil, le champ Masque de sous-r�seau contient le masque d'adresse IP de l'analyseur, le champ Adresse IP de la passerelle d�signe l'adresse IP du routeur qui permet � l'analyseur de communiquer avec d'autres r�seaux. Le champ Adresse IP du serveur est renseign� par l'adresse IP du serveur qui h�bergera la connexion ASTM et enfin le champ Socket du serveur d�signe le Socket (Port) du serveur qui sera utilis� pour la connexion TCP/IP ASTM.\n\n"
											L"V�rifier les param�tres : l'analyseur tentera d'�tablir une connexion ASTM sur la base des param�tres d�finis sous le bouton Modifier les param�tres.  L'analyseur affichera le r�sultat de la tentative de connexion.  V�rifier que le serveur ASTM est en marche et qu'il est configur� pour accepter une connexion avant de v�rifier les param�tres.\n",																																																														// French
											L"Einstellungen �ndern: Die TCP/IP-Einstellungen so �ndern, dass die IP-Adresse des Analyseger�ts die statische IP-Adresse f�r das Analyseger�t ist, die Subnetzmaske die IP-Adressmaske f�r das Analyseger�t ist, die Gateway-IP-Adresse die IP-Adresse des Routers f�r das Analyseger�t ist, �ber den dieses andere Netzwerke erreichen kann, die Server-IP-Adresse die IP-Adresse des Servers ist, der als Host f�r den ASTM-Anschluss dient, und der Server-Socket der Socket (Anschluss ) des Servers ist, der f�r die ASTM-TCP/IP-Verbindung genutzt werden soll.\n\n"
											L"Einstellungen �berpr�fen: Das Analyseger�t wird versuchen, eine ASTM-Verbindung herzustellen, und bedient sich dabei der �ber die Taste \"Einstellungen �ndern\" eingegebenen Einstellungen. Das Analyseger�t zeigt das Resultat des Verbindungsversuchs an. Vor dem �berpr�fen der Einstellungen sicherstellen, dass der ASTM-Server in Betrieb und f�r die Annahme einer Verbindung konfiguriert ist.\n",																																																		// German
											L"Cambiar configuraci�n: Cambie la configuraci�n de TCP/IP, donde la direcci�n IP del analizador es la direcci�n IP est�tica del analizador, la m�scara de la subred es la m�scara de la direcci�n IP del analizador, la direcci�n IP de la puerta de enlace es la direcci�n IP del enrutador para conectividad entre el analizador y otras redes, la direcci�n IP del servidor es la direcci�n IP del servidor que alojar� la conexi�n ASTM y el socket del servidor es el socket (puerto) del servidor que se utilizar� para la conexi�n TCP/IP ASTM.\n\n"
											L"Comprobar configuraci�n: El analizador intentar� realizar una conexi�n ASTM usando los par�metros que aparecen tras pulsar el bot�n \"Cambiar configuraci�n\".  El analizador mostrar� el resultado del intento de conexi�n.  Aseg�rese de que el servidor ASTM est� funcionando y configurado para aceptar una conexi�n antes de comprobar los par�metros de configuraci�n.\n",																																																									// Spanish
											L"Modifica impostazioni: Modificare le impostazioni TCP/IP dove IP analizzatore � l'indirizzo IP statico per l'analizzatore, Maschera di sottorete � la maschera dell'indirizzo IP per l'analizzatore, IP gateway � l'indirizzo IP del router per consentire all'analizzatore di raggiungere altre reti, IP server � l'indirizzo IP del server per l'hosting della connessione ASTM, e Socket server � il Socket (porta) sul server che verr� utilizzato per la connessione TCP/IP ASTM.\n\n"
											L"Verifica impostazioni: L'analizzatore tenter� di effettuare una connessione ASTM utilizzando le impostazioni sotto il pulsante Modifica impostazioni.  L'analizzatore visualizzer� il risultato del tentativo di connessione.  Prima di eseguire la verifica delle impostazioni, accertarsi che il server ASTM sia attivo e configurato per accettare una connessione.\n",																																																											// Italian
											L"Alterar configura��es: Altera as configura��es de TCP/IP, nas quais o IP do analisador � o endere�o IP est�tico do analisador, a m�scara de subrede � a m�scara do endere�o IP do analisador, o IP do Portal � o endere�o IP do rotor para que o analisador se ligue a outras redes, o IP do servidor � o endere�o IP do servidor que alojar� a liga��o ASTM e a tomada do servidor � a tomada (Porta) no Servidor que ser� utilizada para a liga��o TCP/IP ASTM.\n\n"
											L"Verificar configura��es: O analisador tentar� realizar uma liga��o ASTM utilizando as defini��es que surgem sob o bot�o Alterar configura��es.  O analisador apresentar� o resultado da tentativa de liga��o.  Certifique-se que o servidor ASTM est� a funcionar e que est� configurado para aceitar uma liga��o antes de verificar as configura��es.\n",																																																															// Portuguese
											L"Alterar configura��es: Altere as configura��es de TCP/IP em que o IP do analisador � o endere�o IP est�tico do analisador, a m�scara de sub-rede � a m�scara do endere�o IP do analisador, o IP do gateway � o endere�o IP do roteador para que o analisador alcance outras redes, IP do servidor � o endere�o IP do servidor que hospedar� a conex�o ASTM e Soquete do servidor � o soquete (porta) do servidor que ser� usado para a conex�o TCP/IP de ASTM.\n\n"
											L"Verificar configura��es: O analisador tentar� fazer uma conex�o ASTM usando as configura��es no bot�o Alterar configura��es.  O analisador exibir� o resultado da tentativa de conex�o.  Certifique-se de que o servidor ASTM esteja em execu��o e configurado para aceitar uma conex�o antes de verificar as configura��es.\n"																																																																					// Portuguese (Brazil)
										}
	},		
	{ ISTR_TOUCH_CALIBRATE			,	{	L"Pressing Yes will align the touch screen to the LCD display. It is necessary to use a soft-pointed stylus to accurately align the touch screen. Using a hard-pointed object will damage the touch screen. Pressing the screen other than at the center of the target will misalign the touch screen. If you touch the screen in the wrong location, complete the alignment by touching the remaining targets and do not touch the final alignment button. If you have questions contact technical support before performing the alignment.\n",																											// English
											L"Appuyer sur Oui pour aligner l'�cran tactile et l'�cran � cristaux liquides. Pour aligner correctement l'�cran tactile, utiliser un stylet � pointe souple. Toute utilisation d'un objet pointu risquerait d'endommager l'�cran tactile. Le fait d'appuyer sur l'�cran ailleurs qu'au centre de la cible entra�ne un d�faut d'alignement de l'�cran tactile. Si vous touchez l'�cran au mauvais endroit, terminer l'alignement en touchant les cibles restantes et ne pas toucher le bouton d'alignement final. Pour toute question, contacter l'assistance technique avant de proc�der � l'alignement.\n",											// French
											L"Dr�cken auf Ja richtet den Touch-Screen auf das LCD-Display aus. Zur genauen Ausrichtung des Touch-Screens muss ein Eingabestift mit weicher Spitze verwendet werden. Die Verwendung eines Objekts mit harter Spitze besch�digt den Touch-Screen. Dr�cken der Anzeige au�erhalb der Zielmitte f�hrt zu einer falschen Ausrichtung des Touch-Screens. Wenn Sie den Screen an der falschen Stelle ber�hren, schlie�en Sie die Ausrichtung durch Ber�hrung der verbleibenden Ziele ab. Die endg�ltige Ausrichtungstaste nicht ber�hren. Bei Fragen vor der Ausrichtung den technischen Support kontaktieren.\n",											// German
											L"Al pulsar S� se alinear� la pantalla t�ctil con la pantalla de cristal l�quido. Si fuese necesario, utilice un l�piz con punta blanda para alinear con precisi�n la pantalla de contacto. Si se utiliza un objeto con punta dura, la pantalla t�ctil resultar� da�ada. Si se presiona la pantalla en otro lugar distinto al centro del objetivo, la pantalla t�ctil resultar� desalineada. Si toca la pantalla en el lugar equivocado, complete la alineaci�n tocando los objetivos restantes y no toque el bot�n de alineaci�n final. Si tiene alguna duda o pregunta, llame al servicio de asistencia t�cnica antes de realizar la alineaci�n.\n",	// Spanish
											L"Premendo S� il touchscreen verr� allineato al display LCD. Per allineare accuratamente il touchscreen � necessario utilizzare una penna stilo a punta morbida. Oggetti a punta rigida possono danneggiare il touchscreen. Se si preme lo schermo non al centro dell'obiettivo, il touchscreen verr� allineato male. Se si tocca lo schermo nel punto sbagliato, completare l'allineamento toccando gli altri obiettivi e non toccare il pulsante di allineamento finale. Per eventuali dubbi, prima di effettuare l'allineamento contattare l'assistenza tecnica.\n",																					// Italian
											L"Ao premir Sim ir� alinhar o ecr� t�til com o visor LCD. � necess�rio utilizar um ponteiro de ponta suave para alinhar o ecr� t�til com precis�o. Se utilizar um objeto de ponta dura ir� provocar danos no ecr� t�til. Ao pressionar o ecr� noutro local que n�o seja o centro do alvo, ir� desalinhar o ecr� t�til. Se tocar no ecr� no local errado, conclua o alinhamento tocando nos alvos restantes e n�o toque na tecla final de alinhamento. Se tiver alguma d�vida, contacte a servi�o de assist�ncia t�cnica antes de efetuar o alinhamento.\n",																								// Portuguese
											L"Pressionar Sim alinhar� a tela sens�vel ao toque com o display LCD. � necess�rio usar uma caneta de ponta macia para alinhar precisamente a tela sens�vel ao toque. O uso de um objeto pontiagudo danificar� a tela sens�vel ao toque. Pressionar a tela fora do centro do alvo desalinhar� a tela sens�vel ao toque. Se voc� tocar na tela no local errado, conclua o alinhamento tocando nos alvos restantes e n�o toque no bot�o de alinhamento final. Se tiver d�vidas, entre em contato com o suporte t�cnico antes de realizar o alinhamento.\n"																									// Portuguese (Brazil)
										} 
	},
	{ ISTR_UNITS_GLOBAL				,	{	L"Non SI: Also known as Common Units. The units in which all results are calculated and the units commonly used in the United States.\n\n"
											L"SI: Units identified by Abaxis as the commonly used SI units.",																																// English
											L"Non SI : �galement d�sign�es \"Unit�s ordinaires\". Il s'agit des unit�s dans lesquelles tous les r�sultats sont calcul�s, ainsi que des unit�s ordinairement utilis�es aux �tats-Unis.\n\n"
											L"SI : unit�s identifi�es par Abaxis comme �tant les unit�s SI ordinairement utilis�es.\n",																										// French
											L"Kein SI: Auch als H�ufige Einheiten bekannt. Die Einheiten zur Ergebnisermittlung sind die in den USA gebr�uchlichen Einheiten.\n\n"
											L"SI: Als Abaxis identifizierte Einheiten als h�ufig verwendete SI-Einheiten.\n",																												// German
											L"No SI: tambi�n se le conoce como Unidades comunes. La unidades en las que se calculan todos los resultados y las unidades com�nmente usadas en los Estados Unidos.\n\n"
											L"SI: unidades identificadas por Abaxis como las unidades del sistema internacional usadas normalmente.\n",																						// Spanish
											L"Non SI: conosciuto anche come Unit� comuni. Le unit� in cui tutti i risultati sono calcolati e le unit� comunemente usate negli Stati Uniti.\n\n"
											L"SI: unit� identificate come Abaxis come le unit� SI comunemente usate.\n",																														// Italian
											L"N�o SI: Tamb�m conhecidas como Unidades comuns. As unidades nas quais todos os resultados s�o calculados e as unidades frequentemente utilizadas nos Estados Unidos.\n\n"
											L"SI: Unidades identificadas pela Abaxis como as unidades do sistema internacional utilizadas frequentemente.\n",																				// Portuguese
											L"N�o SI: Tamb�m conhecido como Unidades comuns. As unidades nas quais todos os resultados s�o calculados e as unidades comumente usadas nos Estados Unidos.\n\n"
											L"SI: Unidades identificadas pela Abaxis como as unidades SI comumente usadas.\n"																												// Portuguese (Brazil)
										} 
	},
	

// Error screens							ENGLISH,							FRENCH,										GERMAN,									SPANISH,												ITALIAN, 								PORTUGUESE,											PORTUGUESE (BRAZIL)
	{ ESTR_ADMIN_ID_LIST_FULL		,	{	L"Administrator ID List Full",		L"Liste ID administrateur pleine",			L"Vollst�ndige Administrator-ID-Liste",	L"Lista de id. de administradores completa",			L"Elenco ID Amministratore completo",	L"Lista de ID de administradores completa",			L"Lista de IDs de administrador completa"	}	},
	{ ESTR_AMBIENT_TEMP				,	{	L"Ambient Temp. Error",				L"Temp. ambiante Erreur",					L"Umgebungstemp. Fehler",				L"Temp. ambiente Error",								L"Temp. ambiente Errore",				L"Temperatura ambiente Erro",						L"Temp. ambiente Erro"	}	},
	{ ESTR_ANALYSIS_CANCELED		,	{	L"Analysis canceled by operator",	L"Anal. annul�e par l'op�rateur",			L"Anal. vom Bedien. abgebrochen",		L"An�l. cancelado por oper.",							L"Anal. annullata dall'operatore",		L"An�lise cancelada pelo operador",					L"An�lise cancelada pelo operador"	}	},
	{ ESTR_ANALYSIS_TIMEOUT			,	{	L"Analysis Time-out",				L"Temps d'analyse d�pass�",					L"Analysezeit abgelaufen",				L"Tiempo an�lisis agotado",								L"Time-out analisi",					L"Tempo de an�lise esgotado",						L"Tempo limite da an�lise"	}	},
	{ ESTR_ANALYZER_TIMEOUT			,	{	L"Analyzer Time-out",				L"D�l. ex�c. analyseur exp.",				L"Analyseger�t-Unterbr.",				L"Tiempo inactiv. analizad.",							L"Time-out analizzatore",				L"Tempo inat. analisador",							L"Tempo limite do analis."	}	},
	{ ESTR_ANALYZER_TOO_HOT			,	{	L"Analyzer Too Hot",				L"Analyseur trop chaud",					L"Analyseger�t zu hei�",				L"Analizador muy caliente",								L"Analizzatore troppo caldo",			L"Analisador muito quente",							L"Analisador muito quente"	}	},
	{ ESTR_BARCODE_FORMAT			,	{	L"Barcode Format Error",			L"Erreur format code barres",				L"Barcode Formatfehler",				L"Error formato c�d. barras",							L"Err. formato cod. a barre",			L"Erro formato c�d. barras",						L"Erro formato c�d. barras"	}	},
	{ ESTR_BARCODE_SENSOR			,	{	L"Barcode Sensor Error",			L"Erreur d�tect. c. barres",				L"Barcode Sensorfehler",				L"Error sensor c�d. barras",							L"Err. sens. codice a barre",			L"Erro sensor c�d. barras",							L"Erro sensor c�d. barras"	}	},
	{ ESTR_BARCODE_TIMEOUT			,	{	L"Barcode Time-out Error",			L"D�l. ex�c. c. barres exp.",				L"Barcode Unterbr.-Fehler",				L"Err tiem inact c�d barr",								L"Err. time-out cod. barre",			L"Erro tempo inat. c�d bar",						L"Erro temp lim. c�d. barra"	}	},
	{ ESTR_BEADS_MISSING			,	{	L"Beads Missing Error",				L"Erreur billes manquantes",				L"Beads fehlen, Fehler",				L"Err. p�rdida cuentas",								L"Errore granuli mancanti",				L"Erro de perda de esferas",						L"Erro de esferas ausentes"	}	},
	{ ESTR_CAM_DOWN_JAM				,	{	L"CAM Down Jam",					L"Blocage CAM haut",						L"CAM oben Stau",						L"LEVA bloqueada abajo",								L"Blocco CAMMA gi�",					L"CAM bloqueada abaixo",							L"Emperramento CAM p/ baixo"	}	},
	{ ESTR_CAM_UP_JAM				,	{	L"CAM Up Jam",						L"Blocage CAM bas",							L"CAM unten Stau",						L"LEVA bloqueada arriba",								L"Blocco CAMMA su",						L"CAM bloqueada acima",								L"Emperramento CAM p/ cima"	}	},
	{ ESTR_CD_READ					,	{	L"Error Reading CD ROM",			L"Erreur lecture CD ROM",					L"Fehler beim Lesen der CD-ROM",		L"Error al leer el CD-ROM",								L"Errore di lettura CD ROM",			L"Erro de leitura do CD-ROM",						L"Erro ao ler o CD ROM"	}	},
	{ ESTR_CORRUPTED_CALIBRATION	,	{	L"Corrupted Calibration",			L"Calibrage corrompu",						L"Fehlerhafte Kalibrierung",			L"Calibraci�n err�nea",									L"Calibrazione corrotta",				L"Calibra��o corrompida",							L"Calibra��o corrompida"	}	},
	{ ESTR_CUVETTE_SENSOR			,	{	L"Cuvette Sensor Error",			L"Erreur d�tecteur cuvette",				L"Cuvette Sensorfehler",				L"Error sensor cubeta",									L"Errore sensore cuvette",				L"Erro do sensor de cuvetes",						L"Erro do sensor da cuveta"	}	},
	{ ESTR_CUVETTE_TIMEOUT			,	{	L"Cuvette Time-out Error",			L"Err. d�lai ex�c. cuvette",				L"Cuvette Unterbr.-Fehler",				L"Err tiem inact cubeta",								L"Errore time-out cuvette",				L"Erro tempo inat. cuvete",							L"Erro tempo limite cuveta"	}	},
	{ ESTR_DILUENT					,	{	L"Diluent Error",					L"Erreur diluant",							L"Fehler des Verd�nners",				L"Error del diluyente",									L"Errore diluente",						L"Erro de solvente",								L"Erro de diluente"	}	},
	{ ESTR_DILUENT_ABSORBANCE		,	{	L"Diluent Absorbance Error",		L"Erreur absorbance diluant",				L"Verd�nner Absorb.-Fehler",			L"Err absorbancia diluyente",							L"Err. assorb. diluente",				L"Erro absorv�ncia solvente",						L"Erro absor��o do diluente"	}	},
	{ ESTR_DILUTION					,	{	L"Dilution Error",					L"Erreur dilution",							L"Verd�nnungsfehler",					L"Error de diluci�n",									L"Errore diluizione",					L"Erro de dilui��o",								L"Erro de dilui��o"	}	},
	{ ESTR_DISTRIBUTION				,	{	L"Distribution Error",				L"Erreur distribution",						L"Verteilungsfehler",					L"Error de distribuci�n",								L"Errore distribuzione",				L"Erro de distribui��o",							L"Erro de distribui��o"	}	},
	{ ESTR_DRAWER_CLOSED_JAM		,	{	L"Drawer Closed Jam",				L"Blocage fermeture tiroir",				L"Fach geschlossen Stau",				L"Bloqueo de la bandeja cerrada",						L"Blocco cassetto chiuso",				L"Bloqueio da gaveta fechada",						L"Emperramento de gaveta fechada"	}	},
	{ ESTR_DRAWER_OPEN_JAM			,	{	L"Drawer Open Jam",					L"Blocage tiroir ouvert",					L"Fach offen Stau",						L"Bloqueo de la bandeja abierta",						L"Blocco cassetto aperto",				L"Bloqueio da gaveta aberta",						L"Emperramento de gaveta aberta"	}	},
	{ ESTR_ENGINE_RESET				,	{	L"Asynchronous Reset",				L"R�initialisation asynchr.",				L"Asynchroner Resetvorgang",			L"Restablecim. as�ncrono",								L"Ripristino asincrono",				L"Rein�cio ass�ncrono",								L"Redefini��o ass�ncrona"	}	},
	{ ESTR_ILLEGAL_IP_ADDRESS		,	{	L"Illegal IP Address",				L"Adresse IP non valide",					L"Falsche IP-Adresse",					L"Direcci�n IP no v�lida",	   							L"Indirizzo IP non valido",				L"Endere�o IP inv�lido",							L"Endere�o IP ilegal"	}	},
	{ ESTR_ILLEGAL_IP_MASK			,	{	L"Illegal IP Mask",					L"Masque IP non valide",					L"Falsche IP-Maske",					L"M�scara IP no v�lida",								L"Maschera IP non valida",				L"M�scara de IP inv�lida",							L"M�scara IP ilegal"	}	},
	{ ESTR_ILLEGAL_ROTOR_TYPE		,	{	L"Illegal Disc Type",				L"Type disque non autoris�",				L"Falscher Scheibentyp",				L"Tipo de disco ilegal",								L"Tipo disco illegale",					L"Tipo de disco inv�lido",							L"Tipo de disco ilegal"	}	},
	{ ESTR_ILLEGAL_SOCKET			,	{	L"Illegal Socket Value",			L"Valeur de socket non valide",				L"Falscher Socket-Wert",				L"Valor de socket no v�lido",							L"Valore socket non valido",			L"Valor da tomada inv�lido",						L"Valor de soquete ilegal"	}	},
	{ ESTR_INITIALIZATION			,	{	L"Initialization Error",			L"Erreur d'initialisation",					L"Initialisierungsfehler",				L"Error de inicio",										L"Errore inizializzazione",				L"Erro de inicializa��o",							L"Erro de inicializa��o"	}	},
	{ ESTR_INSUFF_SAMPLE			,	{	L"Insufficient Sample Error",		L"Erreur �ch. insuffisant",					L"Unzureich. Probe, Fehler",			L"Err. muestra insuficien",								L"Err. campione insuff.",				L"Erro amostra insuficiente",						L"Erro amostra insuficiente"	}	},
	{ ESTR_INSUFF_STORAGE			,	{	L"Insufficient Storage Space",		L"Espace de stockage insuffisant",			L"Ungen�gend Speicherplatz",			L"Espacio insuficiente",								L"Spazio archiviazione insufficiente",	L"Espa�o de armazenamento insuficiente",			L"Espa�o de armazenamento insuficiente"	}	},
	{ ESTR_INTERNAL_COMM			,	{	L"Internal Comm. Error",			L"Comm. interne Erreur",					L"Interne Komm. Fehler",				L"Com. interna Error",									L"Comm. interno Errore",				L"Com. interna Erro",								L"Com. interna Erro"	}	},
	{ ESTR_INTERNAL_HEATER			,	{	L"Internal Heater Error",			L"Err. disp. chauf. interne",				L"Interner Fehler Heizger�t",			L"Err. calentador interno",								L"Errore riscald. interno",				L"Erro de aquecedor interno",						L"Erro do aquecedor interno"	}	},
	{ ESTR_INTERNAL_LOGIC			,	{	L"Internal Logic Error",			L"Erreur logique interne",					L"Interner Logic-Fehler",				L"Error l�gico interno",								L"Errore logica interna",				L"Erro l�gico interno",								L"Erro da l�gica interna"	}	},
	{ ESTR_INTERNAL_PS				,	{	L"Internal Power Error",			L"Erreur d'alim. interne",					L"Interner Energiefehler",				L"Error de energ�a interno",							L"Errore corrente interna",				L"Erro de energia interno",							L"Erro de energia interna"	}	},
	{ ESTR_INTERNAL_SOFTWARE		,	{	L"Internal Software Error",			L"Erreur logiciel interne",					L"Interner Softwarefehler",				L"Error de software interno",							L"Errore software interno",				L"Erro de software interno",						L"Erro de software interno"	}	},
	{ ESTR_IQC_BEAD					,	{	L"iQC Bead Error",					L"Erreur bille iQC",						L"iQC-Bead-Fehler",						L"Error de cuenta iQC",									L"Errore granuli iQC",					L"Erro de esfera iQC",								L"Erro de esfera iQC"	}	},
	{ ESTR_IQC_BEAD_MIX				,	{	L"iQC Bead Mix Error",				L"Erreur mix billes iQC",					L"iQC-Bead-Mischfehler",				L"Error mezcla cuenta iQC",								L"Err. miscela granuli iQC",			L"Erro de mist. esfera iQC",						L"Erro mistura esferas iQC"	}	},
	{ ESTR_IQC_PRECISION			,	{	L"iQC Precision Error",				L"Erreur pr�cision iQC",					L"iQC-Pr�zisionsfehler",				L"Error de precisi�n iQC",								L"Errore precisione iQC",				L"Erro de precis�o iQC",							L"Erro de precis�o de iQC"	}	},
	{ ESTR_IQC_RATIO				,	{	L"iQC Ratio Error",					L"Erreur ratio iQC",						L"iQC-Verh�ltnisfehler",				L"Error de proporci�n iQC",								L"Errore rapporto iQC",					L"Erro de propor��o iQC",							L"Erro de rela��o de iQC"	}	},
	{ ESTR_LAMP_LEVEL				,	{	L"Flash Lamp Level Error",			L"Erreur niveau lampe flash",				L"Blinkleuchte-Stufenfehler",			L"Error nivel l�mpara flash",							L"Err. liv. lampada flash",				L"Erro n�vel flash l�mpada",						L"Erro n�vel l�mpada flash"	}	},
	{ ESTR_LAMP_NOISE				,	{	L"Flash Lamp Noise Error",			L"Erreur bruit lampe flash",				L"Blinkleuchte-Ger�uschfeh.",			L"Error ruido l�mpara flash",							L"Err. rumore lamp. flash",				L"Erro ru�do flash l�mpada",						L"Erro ru�do l�mpada flash"	}	},
	{ ESTR_MEMORY_READ				,	{	L"Memory Read Error",				L"Erreur de lecture m�moire",				L"Fehler bei Speicheraufruf",			L"Error lectura memoria",								L"Errore lettura memoria",				L"Erro leitura mem�ria",							L"Erro leitura mem�ria"	}	},
	{ ESTR_MEMORY_SAVE				,	{	L"Memory Save Error",				L"Erreur enregistr. m�moire",				L"Fehler bei Speicherung",				L"Err. guardar memoria",								L"Errore salv. Memoria",				L"Erro guardar na mem�ria",							L"Erro salvamento mem�ria"	}	},
	{ ESTR_NEW_ROTOR_TYPE			,	{	L"New Disc Type",					L"Nouveau type de disque",					L"Neuer Scheibentyp",					L"Nuevo tipo de disco",									L"Nuovo tipo disco",					L"Tipo novo de disco",								L"Novo tipo de disco"	}	},
	{ ESTR_NO_FILES_TO_ARCHIVE		,	{	L"No Files to Archive",				L"Aucun fichier � archiver",				L"Keine zu archiv. Dateien",			L"No archi para archivar",								L"Nessun file da archiviare",			L"Nenhum ficheiro arquivar",						L"Sem arquivo para arquivar"	}	},
	{ ESTR_NO_FILES_TO_TRANSMIT		,	{	L"No Files to Transmit",			L"Aucun fich. � transmettre",				L"Keine zu �bertr. Dateien",			L"No archi para transmit",								L"Nessun file da trasm.",				L"Nenhum ficheiro transmit.",						L"Sem arquivo p/ transmitir"	}	},
	{ ESTR_NO_RESULTS_TO_TRANSMIT	,	{	L"No Results to Transmit",			L"Aucun r�sulat � trans.",					L"Keine Erg. zu �bertragen",			L"No result para transmit",								L"Nessun risult. da trasm.",			L"Nenhum result. transmitir",						L"Sem result. p/ transmitir"	}	},
	{ ESTR_OP_ID_LIST_FULL			,	{	L"Operator ID List Full",			L"Liste ID op�rateur pleine", 				L"Vollst. Bediener-ID-Liste",			L"List id. operari complet",							L"Elenco ID Oper. completo",			L"Lista ID operadores comp.",						L"Lista IDs oper. completa"	}	},
	{ ESTR_ORDER_WAS_CANCELED		,	{	L"Order Was Canceled",				L"Commande a �t� annul�e",					L"Bestellung storniert",				L"El pedido se ha cancelado",							L"Ordine cancellato", 					L"O pedido foi cancelado",							L"Solicita��o cancelada"	}	},
	{ ESTR_PATIENT_IDS_DONT_MATCH	,	{	L"Patient IDs Do Not Match",		L"ID patient ne corr. pas",					L"Patienten-IDs unstimmig",				L"No coinciden Id.  pacien",							L"ID paziente non corrisp.",			L"IDs doente n�o coincidem",						L"IDs paciente n�o corresp."	}	},
	{ ESTR_PHOTOMETER				,	{	L"Photometer Error",				L"Erreur photom�tre",						L"Fotometer-Fehler",					L"Error del fot�metro",									L"Errore fotometro",					L"Erro do fot�metro",								L"Erro de fot�metro"	}	},
	{ ESTR_PLEASE_REMOVE_ROTOR		,	{	L"Please Remove Disc",				L"Retirer le disque",						L"Bitte Scheibe entfernen",				L"Retire el disco",										L"Rimuovere il disco",					L"Retirar disco",									L"Remova o disco"	}	},
	{ ESTR_PRINTER_NOT_FOUND		,	{	L"Printer Not Found",				L"Imprimante introuvable",					L"Kein Drucker gefunden",				L"No se encuentra impresora",							L"Stampante non trovata",				L"Impressora n�o localizada",						L"Impressora n�o encontrada"	}	},
	{ ESTR_PRODUCT_CODE				,	{	L"Illegal Product Code",			L"Code produit non autoris�",				L"Falscher Produktcode",				L"C�digo de producto ilegal",							L"Codice prodotto illegale",			L"C�d. produto inv�lido",							L"C�digo de produto ilegal"	}	},
	{ ESTR_RESULT_CORRUPTION		,	{	L"Saved Results Error",				L"Erreur dans les r�sultats enregistr�s",	L"Fehler bei gespeicherten Ergebnissen", L"Error de resultados guardados",						L"Errore risultati salvati",			L"Erro de resultados guardados",					L"Erro de resultados salvos"	}	},
	{ ESTR_ROTOR_DATE_ILLEGAL		,	{	L"Disc Date Illegal",				L"Date disque non autoris�e",				L"Falsches Scheibendatum",				L"Fecha de disco ilegal",								L"Data disco illegale",					L"Data do disco inv�lida",							L"Data do disco ilegal"	}	},
	{ ESTR_ROTOR_DATE_IMPROPER		,	{	L"Disc Date Improper",				L"Date disque incorrecte",					L"Unzul. Scheibendatum",				L"Fecha disco no adecuada",								L"Data disco impropria",				L"Data do disco inadequada",						L"Data do disco impr�pria"	}	},
	{ ESTR_ROTOR_EXPIRED			,	{	L"Disc Expired",					L"Expiration disque",						L"Abgelaufene Scheibe",					L"Disco caducado",										L"Disco scaduto",						L"Disco expirado",									L"Disco expirado"	}	},
	{ ESTR_RQC						,	{	L"RQC Error",						L"Erreur RQC",								L"RQC-Fehler",							L"Error RQC",											L"Errore RQC",							L"Erro RQC",										L"Erro de RQC"	}	},
	{ ESTR_SAMPLE_BLANK_BEAD		,	{	L"Sample Blank Bead Error",			L"Erreur bille vierge �ch.",				L"Probe leer, Bead-Fehler",				L"Err.  cuent. virg.  muest",							L"Err. granuli camp. vuoti",			L"Erro esf. branco amostra",						L"Erro esf. vazia amostra"	}	},
	{ ESTR_SAMPLE_DISTRIBUTION		,	{	L"Sample Distribution Error",		L"Erreur distribution �ch.",				L"Probenverteilungsfehler",				L"Err. distrib. muestra",								L"Err. distrib. campione",				L"Erro de distrib. amostra",						L"Erro distr. de amostra"	}	},
	{ ESTR_SAMPLE_MIX				,	{	L"Sample Mix Error",				L"Erreur mix �chantillon",					L"Probenmischfehler",					L"Error mezcla muestra",								L"Errore miscela campione",				L"Erro de mist. da amostra",						L"Erro mistura de amostra"	}	},
	{ ESTR_SAMPLE_QUALITY			,	{	L"Sample Quality Error",			L"Erreur qualit� �chantil.",				L"Fehler, Probenqualit�t",				L"Error calidad muestra",								L"Errore qualit� campione",				L"Erro de qual. da amostra",						L"Erro qualidade da amostra"	}	},
	{ ESTR_SOFTWARE_EXPIRED         ,	{	L"Analyzer Needs Renewal",			L"Renouvell. analyseur obligatoire",		L"Erneuerung von Analyseger�t erforderlich", L"Es necesario renovar el analizador",				L"Occorre aggiornare l�analizzatore",	L"� necess�rio renovar o analisador",				L"Analisador precisa de renova��o" } },        
	{ ESTR_SPINDLE_MOTOR			,	{	L"Spindle Motor Error",				L"Erreur moteur de l'axe",					L"Fehler, Spindelmotor",				L"Error del motor del eje",								L"Errore asse motore",					L"Erro do eixo do motor",							L"Erro de motor do fuso"	}	},
	{ ESTR_SPINDLE_MOTOR_STUCK		,	{	L"Spindle Motor Error",				L"Erreur moteur de l'axe",					L"Fehler, Spindelmotor",				L"Error del motor del eje",								L"Errore asse motore",					L"Erro do eixo do motor",							L"Erro de motor do fuso"	}	},
	{ ESTR_TEMPERATURE				,	{	L"Temperature Error",				L"Erreur temp�rature",						L"Temperaturfehler",					L"Error de temperatura",								L"Errore temperatura",					L"Erro de temperatura",								L"Erro de temperatura"	}	},
	{ ESTR_UPDATED_ROTOR_TYPE		,	{	L"Updated Disc Type",				L"Type de disque actualis�",				L"Aktual. Scheibentyp",					L"Tipo de disco actualizado",							L"Tipo disco aggiornato",				L"Tipo de disco atualizado",						L"Tipo de disco atualizado"	}	},

// Warning screens							ENGLISH,																													FRENCH,																																				GERMAN,																																		SPANISH,																													ITALIAN, 																														PORTUGUESE,																															PORTUGUESE (BRAZIL)
	{ ESTR_ADMIN_NOT_FOUND			,	{	L"Administrator ID not found",																								L"ID admin. introuvable",																														L"Administrator-ID nicht gefunden",																											L"No se encontr� Id. administrador",																						L"ID amministratore non trovato",																									L"ID de administrador n�o encontrada",																								L"ID de administrador n�o encontrado"	}	},
	{ ESTR_CANNOT_REMOVE_OP_ID		,	{	L"All operators must be deleted before the Operator ID screen can be removed",												L"Tous les op�rateurs doivent �tre supprim�s avant retrait de l'�cran ID op�rat.",																L"Alle Bediener m�ssen gel�scht werden, bevor der Bediener-ID-Bildschirm entfernt werden kann",												L"Debe eliminar todos los operarios antes de eliminar la pantalla Id. operario",											L"Prima di rimuovere lo schermo ID operatore, annullare tutti gli operatori",														L"Devem ser eliminados todos os operadores antes do ecr� de ID de operadores poder ser eliminado",									L"Todos os operadores devem ser exclu�dos antes que a tela de ID de operador possa ser removida"	}	},
	{ ESTR_CONTROL_EXPIRED			,	{	L"Control renewal required",																								L"Renouvell. contr�le obligatoire",																												L"Kontrollerneuerung erforderlich",																											L"Debe renovar control",																									L"Necessario rinnovo controllo",																									L"� necess�rio renovar o controlo",																									L"Renova��o do controle necess�ria"	}	},
	{ ESTR_DATA_LOSS				,	{	L"Changing software will result in data loss.  Press CANCEL to archive data prior to upgrading software.",					L"La mise � niveau va occasionner la perte de donn�es. Appuyer sur ANNULER pour archiver les donn�es avant de mettre � niveau le logiciel.",	L"Bei einer Softwareaktualisierung werden Daten gel�scht. ABBRECHEN bet�tigen, um Daten vor der Softwareaktualisierung zu archivieren.",	L"Al cambiar el software perder� sus datos. Pulse CANCELAR si desea archivar los datos antes de actualizar el software.",	L"La modifica di software determina la perdita di dati. Premere ANNULLA per archiviare i dati prima di aggiornare il software.",	L"Ao alterar o software ir� perder os seus dados. Prima CANCELAR se desejar arquivar os dados antes da atualiza��o do software.",	L"A altera��o do software resultar� em perda de dados. Pressione CANCELAR para arquivar os dados antes de atualizar o software."	}	},
	{ ESTR_DATABASE_FILE_TOO_LARGE	,	{	L"Analyzer memory needs to be optimized.  Press CONTINUE to restart analyzer.",												L"La m�moire de l�analyseur doit �tre optimis�e.  Appuyez sur CONTINUER pour red�marrer l�analyseur.",											L"Speicher des Analyseger�ts muss optimiert werden.  FORTSETZEN dr�cken, um das Analyseger�t neu zu starten.",								L"Es necesario optimizar la memoria del analizador.  Pulse CONTINUAR para reiniciar el analizador.",						L"� necessario ottimizzare la memoria dell�analizzatore.  Premere CONTINUA per riavviare l�analizzatore.",							L"A mem�ria do analisador tem que ser otimizada.  Prima CONTINUAR para reiniciar o analisador.",									L"A mem�ria do analisador precisa ser otimizada.  Pressione CONTINUAR para reiniciar o analisador.",	}	},
	{ ESTR_DUPLICATE_ADMIN_ID		,	{	L"Duplicate Administrator ID entered",																						L"ID admin. saisi en double",																													L"Doppelte Administrator-ID eingegeben",																									L"Esta Id. administrador ya existe",																						L"Inserito duplicato ID amministratore",																							L"A ID do administrador j� existe",																									L"ID de administrador duplicado inserido"	}	},
	{ ESTR_DUPLICATE_OP_ID			,	{	L"Duplicate Operator ID entered",																							L"ID op�rateur saisi en double",																												L"Doppelte Bediener-ID eingegeben",																											L"Esta Id. operario ya existe",																								L"Inserito duplicato ID operatore",																									L"A ID do operador j� existe",																										L"C�digo de operador duplicado digitado"	}	},
	{ ESTR_OP_RENEWAL_REQUIRED		,	{	L"Operator renewal required",																								L"Renouvell. op�rateur obligatoire",																											L"Bediener-Erneuerung erforderlich",																										L"Debe renovar operario",																									L"Necessario rinnovo operatore",																									L"� necess�rio renovar o operador",																									L"Renova��o de operador necess�ria"	}	},
	{ ESTR_REMOVE_FROM_ADMIN_LIST	,	{	L"will be removed from administrators list",																				L"sera retir� de la liste d'administrateurs",																									L"wird aus der Administratorenliste entfernt",																								L"se eliminar� de la lista de administradores", 																			L"sar� rimosso dall'elenco amministratori",																							L"ser� eliminado da lista de administradores",																						L"ser� removido da lista de administradores"	}	},
	{ ESTR_REMOVE_FROM_OP_LIST		,	{	L"will be removed from operators list",																						L"sera retir� de la liste d'op�rateurs",																										L"wird aus der Bedienerliste entfernt",																										L"se eliminar� de la lista de operarios",																					L"sar� rimosso dall'elenco operatori",																								L"ser� eliminado da lista de operadores",																							L"ser� removido da lista de operadores"	}	},
	{ ESTR_REMOVE_FROM_TYPE_LIST	,	{	L"will be removed from Type list",																							L"sera retir� du type de liste",																												L"wird aus der Typenliste entfernt",																										L"se eliminar� de la lista de tipos",																						L"sar� rimosso dall'elenco Tipi",																									L"ser� eliminado da lista de tipos",																								L"ser� removido da lista de Tipo"	}	},
	{ ESTR_RESTORING_REF_RANGES		,	{	L"Restoring Reference Ranges to",																							L"R�tablissement des fourchettes de r�f�rence sur",																								L"Wiederherstellung der Referenzbereiche auf",																								L"Reestablecer los intervalos de referencia a",																				L"Riportare i range di riferimento a",																								L"Restaurar os intervalos de refer�ncia para",																						L"Restaurando intervalos de refer�ncia para"	}	},
	{ ESTR_ROTOR_DETECTED			,	{	L"Disc detected during warming",																							L"Disque d�tect� en cours de chauffage",																										L"Scheibe beim Aufw�rmen vorgefunden",																										L"Disco detectado durante el calentamiento inicial",																		L"Disco individuato durante riscaldamento",																							L"Disco detetado durante o aquecimento",																							L"Disco detectado durante o aquecimento"	}	},
	{ ESTR_UNAUTHORIZED_OP			,	{	L"Unauthorized operator",																									L"Op�rateur non-autoris�",																														L"Unautorisierter Bediener",																												L"Operario no autorizado",																									L"Operatore non autorizzato",																										L"Operador n�o autorizado",																											L"Operador n�o autorizado"	}	},

// Error info text
	{ EISTR_ADMIN_ID_LIST_FULL		,	{	L"All the available space for Administrator ID's is used.  Please remove at least one Administrator ID before adding a new Administrator ID.\n",													// English
											L"Tout l'espace disponible r�serv� aux ID administrateur est utilis�. Veuillez supprimer au moins un ID administrateur avant d'en ajouter un nouveau.\n",										// French
											L"Der gesamte Speicherplatz f�r Administrator-IDs ist belegt. Bitte entfernen Sie mindestens eine Administrator-ID, bevor Sie eine neue hinzuf�gen.\n",											// German
											L"Se ha utilizado todo el espacio disponible para n�meros de identificaci�n de administradores. Elimine al menos un n�mero de identificaci�n de administrador antes de a�adir otro nuevo.\n",	// Spanish
											L"� stato utilizzato tutto lo spazio disponibile per l'ID amministratore. Rimuovere almeno un ID amministratore prima di aggiungere un nuovo ID amministratore.\n",								// Italian
											L"Foi utilizado todo o espa�o dispon�vel para IDs de administradores.  Elimine pelo menos uma ID de administrador antes de adicionar uma ID de administrador nova.\n",							// Portuguese
											L"Todo o espa�o dispon�vel para os IDs de administrador est� usado.  Remova pelo menos um ID de administrador antes de adicionar um novo ID de administrador.\n"									// Portuguese (Brazil)
										}
	},

	{ EISTR_ADMIN_NOT_FOUND			,	{	L"The administrator ID entered is not in the system. Please re-enter the ID.\n",						// English
											L"L'ID admin. n'est pas saisi sur le syst�me. Saisir � nouveau l'ID\n",								// French
											L"Die eingegebene Administrator-ID existiert nicht im System. Bitte geben Sie die ID erneut ein.\n",	// German
											L"Esta Id. administrador no est� en el sistema. Por favor, vuelva a escribirla.\n",					// Spanish
											L"L'ID amministratore inserito non presente nel sistema. Pregasi inserire ID nuovamente.\n",			// Italian
											L"A ID de administrador introduzida n�o est� no sistema. Introduza novamente a ID.\n",				// Portuguese
											L"O ID de administrador inserido n�o est� no sistema. Insira novamente o ID.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_AMBIENT_TEMP			,	{	L"The temperature outside the analyzer is either too hot or too cold to run a disc. Contact Technical Service for assistance.\n",															// English
											L"La temp�rature � l'ext�rieur de l'analyseur est trop �lev�e ou trop faible pour permettre le fonctionnement d'un disque. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Die Temperatur au�erhalb des Analyseger�ts ist f�r den Scheibenbetrieb zu hei� oder zu kalt. Wenden Sie sich an den technischen Service.\n",												// German
											L"La temperatura fuera del analizador es demasiado alta o demasiado baja para usar el disco. P�ngase en contacto con el servicio t�cnico.\n",												// Spanish
											L"La temperatura esterna all'analizzatore � troppo alta o troppo bassa per permettere il funzionamento di un disco. Per assistenza contattare il servizio tecnico.\n",						// Italian
											L"A temperatura fora do analisador � demasiado alta ou demasiado baixa para executar um disco. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",							// Portuguese
											L"A temperatura externa do analisador est� muito alta ou muito baixa para executar um disco. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_ANALYSIS_CANCELED		,	{	L"The analysis was cancelled by pressing the Cancel on the screen.\n",					// English
											L"Annulation de l'analyse par pression sur la touche Annuler sur l'�cran\n",				// French
											L"Die Analyse wurde mit der Schaltfl�che Abbrechen auf dem Bildschirm abgebrochen.\n",	// German
											L"El an�lisis fue cancelado al pulsar Cancelar en la pantalla.\n",						// Spanish
											L"L'analisi � stata annullata premendo il tasto Annulla sullo schermo.\n",				// Italian
											L"A an�lise foi cancelada ao premir Cancelar no ecr�.\n",								// Portuguese
											L"A an�lise foi cancelada pela press�o do bot�o Cancelar na tela.\n"						// Portuguese (Brazil)
										}
	},
	{ EISTR_ANALYSIS_TIMEOUT		,	{	L"The analyzer is not completing the analysis within the time allotted. Contact Technical Service for assistance.\n",								// English
											L"L'Analyseur n'a pas termin� l'analyse dans le temps imparti. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											L"Das Analyseger�t f�hrt die Analyse nicht im vorgesehenen Zeitraum durch. Wenden Sie sich an den technischen Service.\n",						// German
											L"El analizador no est� completando el an�lisis en el tiempo asignado. P�ngase en contacto con el servicio t�cnico.\n",							// Spanish
											L"L'analizzatore non sta completando l'analisi entro il tempo assegnato. Per assistenza contattare il servizio tecnico\n",						// Italian
											L"O analisador n�o est� a concluir a an�lise no tempo atribu�do.  Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia\n",			// Portuguese
											L"O analisador n�o est� concluindo a an�lise dentro do tempo alocado.  Entre em contato com o Servi�o T�cnico para obter assist�ncia\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_ANALYZER_DATE			,	{	L"Date in analyzer is earlier than that of the disc being run. Check dates in analyzer and rerun with new disc.\n",												// English
											L"La date de l'analyseur est ant�rieure � celle du disque en cours d'utilisation. V�rifier les dates sur l'analyseur et r�ex�cuter le nouveau disque.\n",		// French
											L"Das Datum im Analyseger�t liegt vor dem des Scheibenbetriebs. Pr�fen Sie das Datum im Analyseger�t und nehmen Sie den Betrieb mit einer neuen Scheibe auf.\n",	// German
											L"La fecha del analizador es anterior a la fecha del disco que se va a usar. Compruebe las fechas del analizador y vuelva a usar el nuevo disco.\n",				// Spanish
											L"La data dell'analizzatore � precedente a quella di attivazione del disco Controllare le date dell'analizzatore e riavviare con un nuovo disco.\n",				// Italian
											L"A data do analisador � anterior � data do disco que foi iniciado. Verifique as datas do analisador e reinicie com um disco novo.\n",							// Portuguese
											L"A data no analisador � anterior � do disco em execu��o. Verifique as datas no analisador e execute novamente com um novo disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_BACKGROUND_ABSORBANCE	,	{	L"Background absorbance is too high. Contact Technical Service for assistance.\n",								// English
											L"L'absorbance d'arri�re-plan est trop �lev�e. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Die Hintergrundabsorbierung ist zu hoch. Wenden Sie sich an den technischen Service.\n",						// German
											L"La absorbancia de fondo es muy alta. P�ngase en contacto con el servicio t�cnico.\n",							// Spanish
											L"L'assorbimento di fondo � troppo alto. Per assistenza contattare il servizio tecnico.\n",						// Italian
											L"A absorv�ncia de fundo � muito elevada. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											L"A absorb�ncia do fundo est� muito alta. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_BACKGROUND_NOISE		,	{	L"The analyzer detected a high background signal. This may impact the accuracy of the analyzer's absorbance measurement. If symptom continues, contact Technical Service.\n",														// English
											L"L'analyseur a d�tect� un signal d'arri�re-plan �lev�. Ceci peut affecter la pr�cision de la mesure d'absorbance par l'analyseur. En cas de persistance des sympt�mes, contacter le service technique.\n",							// French
											L"Das Analyseger�t hat ein hohes Hintergrundsignal festgestellt. Dies kann die Genauigkeit der Absorbierungsmessung des Analyseger�ts beeintr�chtigen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",	// German
											L"El analizador detecta una se�al de fondo alta. Esto puede afectar a la precisi�n de la medida de absorbancia del analizador. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",			// Spanish
											L"L'analizzatore ha individuato un forte segnale di fondo. Esso potrebbe influire sulla precisione della misurazione di assorbenza dell'analizzatore. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											L"O analisador detetou um sinal de fundo elevado. Isto pode afetar a precis�o da medi��o da absorv�ncia do analisador. Se o problema persistir, contacte o Servi�o de Assist�ncia T�cnica.\n",										// Portuguese
											L"O analisador detectou um sinal de fundo alto. Isso pode afetar a precis�o da medi��o de absorb�ncia do analisador. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"												// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE					,	{	L"The analyzer was unable to read the disc's barcode. If symptom continues, contact Technical Service.\n",												// English
											L"L'analyseur n'a pas pu lire le code � barres du disque. En cas de persistance des sympt�mes, contacter le service technique.\n",						// French
											L"Das Analyseger�t konnte den Barcode der Scheibe nicht lesen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",				// German
											L"El analizador no pudo leer el c�digo de barras del disco. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"L'analizzatore non � riuscito a leggere il codice a barre del disco. Se i sintomi continuano contattare il servizio tecnico.\n",						// Italian
											L"O analisador n�o consegue ler o c�digo de barras do disco. Se o problema persistir, contacte o Servi�o de Assist�ncia T�cnica.\n",						// Portuguese
											L"O analisador n�o foi capaz de ler o c�digo de barras do disco. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE_FORMAT			,	{	L"The analyzer did not recognize the disc. Contact Technical Service for further assistance.\n",							// English
											L"L'analyseur n'a pas reconnu le disque. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											L"Das Analyseger�t konnte die Scheibe nicht erkennen. Wenden Sie sich an den technischen Service.\n",					// German
											L"El analizador no ha reconocido el disco. P�ngase en contacto con el servicio t�cnico.\n",								// Spanish
											L"L'analizzatore non ha riconosciuto il disco. Per ulteriore assistenza contattare il servizio tecnico.\n",				// Italian
											L"O analisador n�o consegue reconhecer o disco. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											L"O analisador n�o reconheceu o disco. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MISSING			,	{	L"The analyzer did not detect all the chemistry beads in the disc. Rerun sample using new disc.\n",										// English
											L"L'analyseur n'a pas d�tect� toutes les billes chimiques dans le disque. R�ex�cuter l'�chantillon � l'aide d'un nouveau disque.\n",		// French
											L"Das Analyseger�t konnte nicht alle chemischen Beads in der Scheibe erkennen. Wiederholen Sie die Probe mit einer neuen Scheibe.\n",	// German
											L"El analizador no detect� todas las cuentas qu�micas en el disco. Vuelva a utilizar la muestra usando un nuevo disco.\n",				// Spanish
											L"L'analizzatore non ha individuato tutti i granuli chimici nel disco. Riavviare il campione con un nuovo disco.\n",						// Italian
											L"O analisador n�o detetou todas as esferas qu�micas no disco. Executar novamente a amostra utilizando um disco novo.\n",				// Portuguese
											L"O analisador n�o detectou todas as esferas de qu�micas no disco. Execute novamente a amostra usando um novo disco.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MIX				,	{	L"The analyzer will not be able to determine the exposure status if the QC beads have not mixed correctly. Rerun with a new disc.\n",									// English
											L"L'analyseur ne parviendra pas � d�terminer l'�tat d'exposition si les billes QC ne sont pas correctement m�lang�es. R�ex�cuter avec un nouveau disque.\n",				// French
											L"Das Analyseger�t kann den Status der Einwirkung nicht feststellen, wenn die QC-Beads nicht ordnungsgem�� vermischt wurden. Mit einem neuen Rotor wiederholen.\n",		// German
											L"El analizador no ser� capaz de determinar el estado de exposici�n si las cuentas QC no se mezclan correctamente. Debe usarse con un nuevo disco.\n",					// Spanish
											L"L'analizzatore non potr� determinare lo stato di esposizione se i granuli di QC non sono stati mescolati correttamente. Riavviare con un nuovo disco.\n",				// Italian
											L"O analisador n�o conseguir� determinar o estado de exposi��o se as esferas de CQ n�o forem misturadas adequadamente. Executar novamente utilizando um disco novo.\n",	// Portuguese
											L"O analisador n�o poder� determinar o status de exposi��o se as esferas de QC n�o tiverem sido misturadas corretamente. Execute novamente com um novo disco.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_BIAS					,	{	L"The analyzer detected a high bias signal. This may impact the accuracy of the analyzer's measurement of absorbance. If symptom continues, contact Technical Service.\n",														// English
											L"L'analyseur a d�tect� un signal de polarisation �lev�, ce qui peut affecter la pr�cision de la mesure d'absorbance par l'analyseur. En cas de persistance des sympt�mes, contacter le service technique.\n",					// French
											L"Das Analyseger�t hat ein hohes St�rsignal festgestellt. Dies kann die Genauigkeit der Absorbierungsmessung des Analyseger�ts beeintr�chtigen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",		// German
											L"El analizador detecta una se�al de polarizaci�n alta. Esto puede afectar a la precisi�n de la medida de absorbancia del analizador. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"L'analizzatore ha individuato un forte segnale di bias. Esso potrebbe influire sulla precisione della misurazione di assorbenza dell'analizzatore. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											L"O analisador detetou um sinal de desvio elevado. Isto pode afetar a precis�o da medi��o da absorv�ncia do analisador. Se o problema persistir, contacte o Servi�o de Assist�ncia T�cnica.\n",									// Portuguese
											L"O analisador detectou um sinal de polariza��o alto. Isso pode afetar a precis�o da medi��o de absorb�ncia do analisador. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_BOTTOM_HEATER_HIGH		,	{	L"The bottom heater plate draws too much current when turned on at full power. Contact Technical Service for assistance.\n",												// English
											L"La plaque chauffante inf�rieure consomme trop d'�lectricit� lorsqu'elle fonctionne � fond. Pour obtenir une assistance, contacter le service technique.\n",			// French
											L"Die untere Heizplatte verbraucht bei Maximalbetrieb zu viel Strom. Wenden Sie sich an den technischen Service.\n",														// German
											L"La placa del calentador inferior utiliza mucha corriente cuando se enciende a toda potencia. P�ngase en contacto con el servicio t�cnico.\n",							// Spanish
											L"Il disco di riscaldamento inferiore prende troppa corrente quando � acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",					// Italian
											L"A placa inferior do aquecedor utiliza demasiada de corrente quando ligada na pot�ncia m�xima. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											L"A placa inferior do aquecedor consome muita corrente quando ligada na pot�ncia m�xima. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_BOTTOM_HEATER_LOW		,	{	L"The bottom heater plate did not draw enough current when turned on at full power. Contact Technical Service for assistance.\n",										// English
											L"La plaque chauffante inf�rieure n'a pas consomm� trop d'�lectricit� lorsqu'elle a fonctionn� � fond. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Die untere Heizplatte verbrauchte bei Maximalbetrieb zu wenig Strom. Wenden Sie sich an den technischen Service.\n",													// German
											L"La placa del calentador inferior no utiliz� suficiente corriente cuando se puso a toda potencia. P�ngase en contacto con el servicio t�cnico.\n",						// Spanish
											L"Il disco di riscaldamento inferiore non ha preso sufficiente corrente quando � acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",			// Italian
											L"A placa inferior do aquecedor n�o utiliza corrente suficiente quando ligada na pot�ncia m�xima. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											L"A placa inferior do aquecedor n�o atraiu corrente suficiente quando ligada na pot�ncia m�xima. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_CANNOT_REMOVE_OP_ID		,	{	L"To remove the operator ID prompt it is necessary to remove all operator IDs first.  Please use the Operator Administration menu to remove all operators, and then return here to remove the operator prompt.\n",																// English
											L"Pour supprimer l'invite ID op�rateur, il est n�cessaire de commencer par supprimer tous les ID op�rateur.  Supprimer tous les op�rateurs � partir du menu Administration op�rateur, puis revenir ici pour supprimer l'invite op�rateur.\n",									// French
											L"Um die Bediener-ID-Eingabeaufforderung zu entfernen, m�ssen Sie zuerst alle Bediener-IDs l�schen. Bitte machen Sie dies �ber das Bediener-Administrationsmen� und gehen zur�ck, um die Bediener-Eingabeaufforderung zu entfernen.\n",											// German
											L"Para eliminar el aviso de Id. operario debe eliminar todas las Id. operador. Para esto, vaya al men� Administraci�n de operarios para eliminar todos los operarios y despu�s vuelva aqu� para eliminar el aviso.\n",															// Spanish
											L"Per rimuovere il prompt ID operatore si devono prima rimuovere tutti gli ID operatore. Pregasi utilizzare il menu Amministrazione operatore per rimuovere tutti gli operatori, quindi tornare a questo punto e rimuovere il prompt operatore.\n",								// Italian
											L"Para eliminar o aviso de ID de operador � necess�rio eliminar primeiro todas as ID de operador.  Para remover todos os operadores utilize o men� Administra��o de operadores para eliminar todos os operadores e, em seguida, volte novamente aqui para eliminar o aviso.\n",	// Portuguese
											L"Para remover o prompt do ID de operador, � necess�rio remover todos os IDs de operador antes.  Use o menu Administra��o do operador para remover todos os operadores e, em seguida, retorne aqui para remover o prompt do operador.\n"											// Portuguese (Brazil)
										} 
	},
	{ EISTR_CDROM					,	{	L"The analyzer is unable to read information from the CD ROM. Contact Technical Service for assistance.\n",									// English
											L"L'analyseur n'est pas en mesure de lire des informations sur le CD ROM. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Das Analyseger�t kann die CD-ROM nicht lesen. Wenden Sie sich an den technischen Service.\n",												// German
											L"El analizador no puede leer la informaci�n del CD-ROM. P�ngase en contacto con el servicio t�cnico.\n",									// Spanish
											L"L'analizzatore non � in grado di leggere le informazioni dal CD ROM. Per assistenza contattare il servizio tecnico.\n",					// Italian
											L"O analisador n�o consegue ler a informa��o do CD-ROM. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",				// Portuguese
											L"O analisador n�o � capaz de ler as informa��es do CD-ROM. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_CONTROL_EXPIRED			,	{	L"The date set for analyzing control samples has passed.  Please contact technical support for more information.\n",										// English
											L"La date pr�vue pour l'analyse des �chantillons de contr�le est d�pass�e. Pour plus d'informations, contacter l'assistance technique.\n",				// French
											L"Das angegebene Datum, um die Kontrollproben zu analysieren, ist abgelaufen. Wenden Sie sich an den technischen Service f�r weitere Information.\n",	// German
											L"La fecha para analizar las muestras de control ya ha pasado. P�ngase en contacto con el servicio t�cnico.\n",											// Spanish
											L"La data impostata per l'analisi dei campioni di controllo � stata superata. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",		// Italian
											L"A data definida para analisar as amostras controlo foi excedida.  Para obter mais informa��es, contacte o Servi�o de Assist�ncia T�cnica.\n",			// Portuguese
											L"A data configurada para analisar as amostras de controle j� passou.  Entre em contato com o suporte t�cnico para obter mais informa��es.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_CUVETTE_MARK			,	{	L"The analyzer was unable to detect the disc. If symptom continues, contact Technical Service.\n",												// English
											L"L'analyseur n'est pas parvenu � d�tecter le disque. En cas de persistance des sympt�mes, contacter le service technique.\n",					// French
											L"Das Analyseger�t konnte die Scheibe nicht finden. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",					// German
											L"El analizador no fue capaz de detectar el disco. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"L'analizzatore non � riuscito a individuare il disco. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"O analisador n�o consegue detetar o disco. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",								// Portuguese
											L"O analisador n�o conseguiu detectar o disco. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_DARK_NOISE				,	{	L"There was high variability in the background signal. This degrades the accuracy of the analyzer to measure absorbance. If symptom continues, contact Technical Service.\n",														// English
											L"Le signal d'arri�re-plan a pr�sent� une variabilit� �lev�e, ce qui r�duit la pr�cision de l'analyseur pour la mesure d'absorbance. En cas de persistance des sympt�mes, contacter le Service technique.\n",						// French
											L"Es bestand eine hohe Hintergrundsignalvarianz. Dies beeintr�chtigt die Genauigkeit der Absorbierungsmessung des Analyseger�ts. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",							// German
											L"Hab�a una gran variabilidad en la se�al de fondo. Esto rebaja la precisi�n del analizador para analizar la medida de la absorbancia. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"Grande variabilit� nel segnale di fondo. Ci� influisce negativamente la precisione dell'analizzatore nella misurazione dell'assorbenza. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"Foi observada uma grande variabilidade no sinal de fundo. Isto reduz a precis�o do analisador para realizar medi��es de absorv�ncia. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",						// Portuguese
											L"Houve alta variabilidade no sinal de fundo. Isso degrada a precis�o do analisador para medir a absorb�ncia. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"														// Portuguese (Brazil)
										} 
	},
	{ EISTR_DATA_LOSS				,	{	L"The software needs to change the format of the data on the analyzer.  It is not possible to convert the data and change the software.  Pressing CANCEL will allow the existing data to be archived prior to upgrading the software.  Use the Transmit All selection under the Recall screen to archive the existing data external to the analyzer.  Verify the external archive prior to upgrading the software.\n",								// English
											L"Le logiciel doit modifier le format des donn�es sur l'analyseur. Il n'est pas possible de convertir les donn�es et de mettre � niveau le logiciel. Appuyer sur ANNULER pour archiver les donn�es avant la mise � niveau du logiciel. Appuyer sur Transmettre tout sous l'�cran Rappel pour archiver les donn�es de l'analyseur sur un support externe. V�rifier l'archive externe avant de mettre � niveau le logiciel.\n",						// French
											L"Die Software �ndert das Datenformat auf dem Analysesystem. Es ist nicht m�glich, die Daten zu konvertieren und die Software zu aktualisieren. ABBRECHEN bet�tigen, um die bestehenden Daten vor der Softwareaktualisierung zu archivieren. Die Funktion �Alle �bertragen� im Men� �Wiederaufruf� verwenden, um die bestehenden Daten auf einem externen Ger�t zu archivieren. Das externe Archiv vor der Softwareaktualisierung �berpr�fen.\n",	// German
											L"El software debe cambiar el formato de los datos del analizador. No se pueden convertir los datos y cambiar el software. Pulse CANCELAR si desea archivar los datos existentes antes de actualizar el software. Para archivar los datos existentes fuera del analizador, use la opci�n Trasmitir todo de la pantalla Recordar. Antes de actualizar el software, verifique el archivo externo.\n",													// Spanish
											L"Il software deve modificare il formato dei dati sull'analizzatore. Non � possibile convertire i dati e cambiare il software. Premendo ANNULLA si consente l'archiviazione dei dati esistenti prima dell'aggiornamento del software. Usare la selezione Trasmetti tutti nella schermata Richiama per archiviare i dati esistenti all'esterno dell'analizzatore. Verificare l'archivio esterno prima di aggiornare il software.\n",					// Italian
											L"O software necessita alterar o formato dos dados do analisador.  N�o � poss�vel converter os dados e alterar o software.  Ao premir CANCELAR permitir� arquivar os dados existentes antes de atualizar o software.  Para arquivar os dados existentes externos ao analisador, utilize a op��o Transmitir tudo no ecr� Recuperar.  Antes de atualizar o software, verifique o ficheiro externo.\n",													// Portuguese
											L"O software precisa alterar o formato dos dados no analisador.  N�o � poss�vel converter os dados e alterar o software.  Pressione CANCELAR para que os dados existentes sejam arquivados antes de atualizar o software.  Use a sele��o Transmitir todos na tela Recuperar para arquivar os dados existentes externos ao analisador.  Verifique o arquivo externo antes de atualizar o software.\n"													// Portuguese (Brazil)
										}
	},
    {EISTR_DATABASE_FILE_TOO_LARGE ,    {	L"The analyzer memory needs to be optimized. The memory will be optimized and the analzyer will restart. \n",								// English
											L"La m�moire de l�analyseur doit �tre optimis�e. La m�moire va �tre optimis�e et l�analyseur va red�marrer.\n",								// French 
											L"Der Speicher des Analyseger�ts muss optimiert werden. Der Speicher wird optimiert und das Analyseger�t wird neu gestartet.\n",			// German
											L"Es necesario optimizar la memoria del analizador. La memoria se optimizar� y el analizador se reiniciar�.\n",								// Spanish
											L"� necessario ottimizzare la memoria dell�analizzatore. La memoria verr� ottimizzata e l�analizzatore riavviato.\n",						// Italian
											L"A mem�ria do analisador tem que ser otimizada. A mem�ria ser� otimizada e o analisador ser� reiniciado.\n",								// Portuguese
											L"A mem�ria do analisador precisa ser otimizada. A mem�ria ser� otimizada e o analisador ser� reiniciado.\n"								// Portuguese (Brazil)
                                        } 
    },
	{ EISTR_DILUTION				,	{	L"The analyzer may not have delivered the correct amount of sample to the reagents. Confirm the reported values.\n",						// English
											L"Il se peut que l'analyseur n'ait pas dispens� la quantit� d'�chantillon voulue sur les r�actifs. Confirmer les valeurs fournies.\n",	// French
											L"Das Analyseger�t hat m�glicherweise die falsche Probenmenge den Reagenzien zugef�hrt. Berichtete Werte best�tigen.\n",					// German
											L"Es posible que el analizador no proporcione la cantidad correcta de muestra a los reactivos. Confirme los valores dados.\n",			// Spanish
											L"L'analizzatore potrebbe non aver portato la quantit� corretta di campione ai reagenti. Confermare i valori riportati.\n",				// Italian
											L"� poss�vel que o analisador n�o tenha fornecido a quantidade adequada de amostra aos reagentes. Confirme os valores reportados.\n",	// Portuguese
											L"O analisador pode n�o ter entregue a quantidade correta de amostra aos reagentes. Confirme os valores relatados.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_DRAWER_CLOSE			,	{	L"The drawer appears to be unable to close correctly. Remove any visible obstructions. If symptom continues, contact Technical Service.\n",											// English
											L"Le tiroir ne semble pas se fermer correctement. �liminer tout �l�ment visible responsable du blocage. En cas de persistance des sympt�mes, contacter le service technique.\n",		// French
											L"Das Fach kann nicht ordnungsgem�� geschlossen werden. Entfernen Sie sichtbare Hindernisse. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",				// German
											L"Parece que la bandeja no puede cerrarse correctamente. Retire todo obst�culo visible. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"Sembra che il cassetto non riesca a chiudersi perfettamente. Rimuovere qualsiasi ostruzione visibile. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											L"Parece que a gaveta n�o fecha corretamente. Remova qualquer obst�culo vis�vel. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",								// Portuguese
											L"A gaveta parece incapaz de fechar corretamente. Remova quaisquer obstru��es vis�veis. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_DRAWER_OPEN				,	{	L"The drawer appears to be unable to open correctly. Remove any visible obstructions. If symptom continues, contact Technical Service.\n",											// English
											L"Le tiroir ne semble pas s'ouvrir correctement. �liminer tout �l�ment visible responsable du blocage. En cas de persistance des sympt�mes, contacter le service technique.\n",		// French
											L"Das Fach kann nicht ordnungsgem�� ge�ffnet werden. Entfernen Sie sichtbare Hindernisse. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",				// German
											L"Parece que la bandeja no puede abrirse correctamente. Retire todo obst�culo visible. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"Sembra che il cassetto non riesca ad aprirsi perfettamente. Rimuovere qualsiasi ostruzione visibile. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											L"Parece que a gaveta n�o abre corretamente. Remova qualquer obst�culo vis�vel. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",								// Portuguese
											L"A gaveta parece incapaz de abrir corretamente. Remova quaisquer obstru��es vis�veis. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_DUPLICATE_ADMIN_ID		,	{	L"The Administrator ID entered already exists in the system. Duplicate IDs are not allowed.\n",		// English
											L"L'ID administrateur saisi existe d�j� sur le syst�me. Les ID en double ne sont pas autoris�s.\n",	// French
											L"Die eingegebene Administrator-ID existiert bereits im System. Doppelte IDs sind nicht erlaubt.\n",	// German
											L"Esta Id. administrador ya existe en el sistema. No se permiten Id. duplicadas.\n",					// Spanish
											L"L'ID amministratore inserito � gi� presente nel sistema. Non sono consentiti ID duplicati.\n",		// Italian
											L"A ID de administrador introduzida j� existe no sistema. N�o s�o permitidas IDs duplicadas.\n",		// Portuguese
											L"O ID de administrador inserido j� existe no sistema. IDs duplicados n�o s�o permitidos.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_DUPLICATE_OP_ID			,	{	L"The Operator ID entered already exists in the system. Duplicate IDs are not allowed.\n",		// English
											L"L'ID op�rateur saisi existe d�j� sur le syst�me. Les ID en double ne sont pas autoris�s.\n",	// French
											L"Die eingegebene Bediener-ID existiert bereits im System. Doppelte IDs sind nicht erlaubt.\n",	// German
											L"Esta Id. operario ya existe en el sistema. No se permiten Id. duplicadas.\n",					// Spanish
											L"L'ID operatore inserito � gi� presente nel sistema. Non sono consentiti ID duplicati.\n",		// Italian
											L"A ID de operador introduzida j� existe no sistema. N�o s�o permitidas IDs duplicadas.\n",		// Portuguese
											L"O ID de operador inserido j� existe no sistema. IDs duplicados n�o s�o permitidos.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_ENGINE_CODE_DOWNLOAD	, {		L"The analyzer detected an error when updating the software. Contact Technical Service for assistance.\n",    // English
											L"L'analyseur a d�tect� une erreur lors de la mise � jour du logiciel. Pour obtenir une assistance, contacter le service technique.\n",    // French
											L"Das Analyseger�t hat einen Fehler beim Aktualisieren der Software festgestellt. Wenden Sie sich an den technischen Service.\n",    // German
											L"El analizador ha detectado un error al actualizar el software. P�ngase en contacto con el servicio t�cnico.\n",    // Spanish
											L"L'analizzatore ha rilevato un errore durante l'aggiornamento del software. Per assistenza contattare il servizio tecnico.\n",    // Italian
											L"Ao atualizar o software o analisador detetou um erro. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese	
											L"O analisador detectou um erro ao atualizar o software. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"		// Portuguese (Brazil)
                                      }
    },	
	{ EISTR_ENGINE_CODE_OUT_SYNC	, {		L"The analyzer detected an error with the software. Please install the software from CD, If symptom continues, contact Technical Service.\n",									// English	
											L"L'analyseur a d�tect� une erreur dans le logiciel. Installer le logiciel � partir du CD. En cas de persistance des sympt�mes, contacter le service technique.\n",				// French
											L"Das Analyseger�t hat einen Softwarefehler festgestellt. Installieren Sie die Software von der CD. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service\n",	// German
											L"El analizador ha detectado un error con el software. Instale el software desde el CD. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",//Spanish
											L"L'analizzatore ha rilevato un errore relativo al software. Installare il software dal CD. Se i sintomi continuano contattare il servizio tecnico.\n",							 //Italian 
											L"O analisador detetou um erro no software. Instale o software a partir do CD. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",							// Portuguese
											L"O analisador detectou um erro no software. Instale o software do CD. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"											// Portuguese (Brazil)
                                      }
    },
	{ EISTR_ENGINE_RESET			,	{	L"A component of the analyzer has restarted unexpectedly. Contact Technical Service for assistance.\n",									// English
											L"Un des composants de l'analyseur a red�marr� de mani�re inattendue. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Eine Komponente des Analyseger�ts wurde unerwartet neu gestartet. Wenden Sie sich an den technischen Service.\n",						// German
											L"Un componente del analizador se ha reiniciado inesperadamente. P�ngase en contacto con el servicio t�cnico.\n",						// Spanish
											L"Un componente dell'analizzatore si � riavviato in modo imprevisto. Per assistenza contattare il servizio tecnico\n",					// Italian
											L"Um componente do analisador foi reiniciado inesperadamente.  Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",		// Portuguese
											L"Um componente do analisador foi reiniciado inesperadamente.  Entre em contato com o Servi�o T�cnico para obter assist�ncia\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_EXPIRED_ROTOR			,	{	L"The disc has expired and should not be used. Contact Technical Service for further details.\n",										// English
											L"La date de p�remption du disque est d�pass�e. Ne pas l'utiliser. Pour obtenir plus d'informations, contacter le service technique.\n",	// French
											L"Die Scheibe ist abgelaufen und sollte ersetzt werden. Wenden Sie sich an den technischen Service.\n",									// German
											L"El disco ha caducado y no deber�a utilizarse. P�ngase en contacto con el servicio t�cnico.\n",											// Spanish
											L"Il disco � scaduto e non dovrebbe essere usato. Per ulteriore dettagli contattare il servizio tecnico.\n",								// Italian
											L"O disco expirou e n�o dever� ser utilizado. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",				// Portuguese
											L"O disco expirou e n�o deve ser usado. Entre em contato com o Servi�o T�cnico para obter detalhes adicionais.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_FILTER_CURV				,	{	L"The filter curve calibration factors have been corrupted. Contact Technical Service for assistance.\n",								// English
											L"Les facteurs de calibrage de la courbe du filtre sont corrompus. Pour obtenir une assistance, contacter le service technique.\n",		// French
											L"Die Kalibrierungsfaktoren der Filterkurve sind fehlerhaft. Wenden Sie sich an den technischen Service.\n",								// German
											L"Los factores de calibraci�n de la curva del filtro est�n da�ados. P�ngase en contacto con el servicio t�cnico.\n",						// Spanish
											L"I fattori di calibrazione della curva del filtro sono corrotti. Per assistenza contattare il servizio tecnico.\n",						// Italian
											L"Os fatores da curva de calibra��o do filtro est�o danificados. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											L"Os fatores de calibra��o da curva do filtro foram corrompidos. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_FLASH_LAMP_NOISE		,	{	L"During one of the system checks, the analyzer attempted to compute an absorbance from a cluster of flashes that are too variable from each other. If symptom continues, contact Technical Service.\n",															// English
											L"Lors de l'une des v�rifications du syst�me, l'analyseur a tent� de calculer l'absorbance � partir d'un ensemble de flashs qui varient trop les uns des autres. En cas de persistance des sympt�mes, contacter le service technique.\n",						// French
											L"Bei einer Systempr�fung unternahm das Analyseger�t den Versuch, eine Absorbierung aus einer Gruppe von Lichtstrahlen zu berechnen, die zu sehr voneinander abweichen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",				// German
											L"Durante una de las comprobaciones del sistema, el analizador intent� calcular una absorbancia de un grupo de flashes que son excesivamente distintos unos de otros. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"Durante uno dei controlli di sistema l'analizzatore ha cercato di calcolare l'assorbenza in un gruppo di flash luminosi troppo diversi tra loro. Se i sintomi continuano contattare il servizio tecnico.\n",													// Italian
											L"Durante uma das verifica��es do sistema, o analisador tentou calcular uma absorv�ncia de um grupo de flashes extremamente diferentes uns dos outros. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",									// Portuguese
											L"Durante uma das verifica��es do sistema, o analisador tentou calcular uma absorb�ncia de um conjunto de flashes que s�o muito vari�veis um em rela��o ao outro. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_FLASH_READ				,	{	L"The analyzer is unable to read information required to operate. Contact Technical Service for assistance.\n",											// English
											L"L'analyseur n'est pas en mesure de lire les informations requises pour fonctionner. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Das Analyseger�t kann f�r den Betrieb erforderliche Informationen nicht abrufen. Wenden Sie sich an den technischen Service.\n",						// German
											L"El analizador es incapaz de leer informaci�n necesaria para funcionar. P�ngase en contacto con el servicio t�cnico.\n",								// Spanish
											L"L'analizzatore non � in grado di leggere le informazioni necessarie per il funzionamento. Per assistenza contattare il servizio tecnico.\n",			// Italian
											L"O analisador n�o consegue ler informa��o necess�ria para funcionar. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",				// Portuguese
											L"O analisador n�o � capaz de ler as informa��es necess�rias para operar. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_FLASH_SAVE				,	{	L"The analyzer is unable to save information required to operate. Contact Technical Service for assistance.\n",												// English
											L"L'analyseur n'est pas en mesure d'enregistrer les informations requises pour fonctionner. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Das Analyseger�t kann f�r den Betrieb erforderliche Informationen nicht speichern. Wenden Sie sich an den technischen Service.\n",							// German
											L"El analizador es incapaz de guardar informaci�n necesaria para funcionar. P�ngase en contacto con el servicio t�cnico.\n",									// Spanish
											L"L'analizzatore non � in grado di salvare le informazioni necessarie per il funzionamento. Per assistenza contattare il servizio tecnico.\n",				// Italian
											L"O analisador n�o consegue gravar informa��o necess�ria para funcionar. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",				// Portuguese
											L"O analisador n�o � capaz de salvar as informa��es necess�rias para operar. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_HARDWARE				,	{	L"The analyzer has detected an error in the hardware. Contact Technical Service for further details.\n",					// English
											L"L'analyseur a d�tect� une erreur de mat�riel. Pour obtenir plus d'informations, contacter le service technique.\n",	// French
											L"Das Analyseger�t hat einen Hardwarefehler festgestellt. Wenden Sie sich an den technischen Service.\n",				// German
											L"El analizador ha detectado un error en el equipo. P�ngase en contacto con el servicio t�cnico.\n",						// Spanish
											L"L'analizzatore ha individuato un errore nell'hardware. Per ulteriore dettagli contattare il servizio tecnico.\n",		// Italian
											L"O analisador detetou um erro no hardware. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",	// Portuguese
											L"O analisador detectou um erro no hardware. Entre em contato com o Servi�o T�cnico para obter detalhes adicionais.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_ILLEGAL_BARCODE			,	{	L"The analyzer has detected an illegal barcode. Contact Technical Service for further details.\n",								// English
											L"L'analyseur a d�tect� un code � barres non autoris�. Pour obtenir plus d'informations, contacter le service technique.\n",		// French
											L"Das Analyseger�t hat einen falschen Barcode festgestellt. Wenden Sie sich an den technischen Service.\n",						// German
											L"El analizador ha detectado un c�digo de barras ilegal. P�ngase en contacto con el servicio t�cnico.\n",						// Spanish
											L"L'analizzatore ha individuato un codice a barre illegale. Per ulteriore dettagli contattare il servizio tecnico.\n",			// Italian
											L"O analisador detetou um c�digo de barras inv�lido. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",	// Portuguese
											L"O analisador detectou um c�digo de barras ilegal. Entre em contato com o Servi�o T�cnico para obter detalhes adicionais.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_ILLEGAL_IP_ADDRESS		,	{	L"The IP Address entered cannot be used.  Please enter an IP address in the N.N.N.N format where N is a number between 0 and 255.\n",						// English
											L"L'adresse IP saisie ne peut pas �tre utilis�e.  Entrer une adresse IP au format N.N.N.N, N �tant un nombre compris entre 0 et 255.\n",						// French
											L"Die eingegebene IP-Adresse ist nicht nutzbar. Bitte geben Sie eine IP-Adresse im Format N.N.N.N ein, wobei N eine Zahl zwischen 0 und 255 ist.\n",			// German
											L"La direcci�n IP introducida no puede utilizarse.  Por favor, introduzca una direcci�n IP con el formato N.N.N.N, donde N es un n�mero entre 0 y 255.\n",	// Spanish
											L"Impossibile utilizzare l'indirizzo IP specificato.  Immettere un indirizzo IP nel formato N.N.N.N. dove N � un numero compreso fra 0 e 255.\n",			// Italian
											L"O endere�o IP introduzido n�o pode ser utilizado.  Introduza um endere�o IP com o formato N.N.N.N, onde N � um n�mero entre 0 e 255.\n",					// Portuguese
											L"O endere�o IP inserido n�o pode ser usado.  Insira um endere�o IP no formato N.N.N.N, em que N � um n�mero entre 0 e 255.\n"								// Portuguese (Brazil)
										}
	},

	{ EISTR_ILLEGAL_IP_MASK			,	{	L"The IP Mask entered cannot be used.  Please enter an IP Mask in the N.N.N.N format where N is a number between 0 and 255.\n",								// English
											L"Le masque IP saisi ne peut pas �tre utilis�.  Entrer un masque IP au format N.N.N.N, N �tant un nombre compris entre 0 et 255.\n",							// French
											L"Die eingegebene IP-Maske ist nicht nutzbar. Bitte geben Sie eine IP-Maske im Format N.N.N.N ein, wobei N eine Zahl zwischen 0 und 255 ist.\n",				// German
											L"La m�scara IP introducida no puede utilizarse.  Por favor, introduzca una m�scara IP con el formato N.N.N.N, donde N es un n�mero entre 0 y 255.\n",		// Spanish
											L"Impossibile utilizzare la maschera IP specificata.  Immettere una maschera IP nel formato N.N.N.N. dove N � un numero compreso fra 0 e 255.\n",			// Italian
											L"A m�scara de IP introduzida n�o pode ser utilizada.  Introduza uma m�scara de IP com o formato N.N.N.N, onde N � um n�mero entre 0 e 255.\n",				// Portuguese
											L"A m�scara IP inserida n�o pode ser usada.  Insira uma m�scara IP no formato N.N.N.N, em que N � um n�mero entre 0 e 255.\n"								// Portuguese (Brazil)
										}
	},
	{ EISTR_ILLEGAL_SOCKET			,	{	L"The socket entered cannot be used.  Please enter a socket number between 0 and 65536.\n",								// English
											L"Le socket saisi ne peut pas �tre utilis�.  Entrer un num�ro de socket compris entre 0 et 65536.\n",					// French
											L"Der eingegebene Anschluss (Socket) ist nicht nutzbar. Bitte eine Anschlussnummer zwischen 0 und 65536 eingeben.\n",	// German
											L"El socket introducido no puede utilizarse.  Por favor, introduzca un n�mero de enchufe entre 0 y 65536.\n",			// Spanish
											L"Impossibile utilizzare il socket specificato.  Immettere un numero di socket compreso fra 0 e 65536.\n",				// Italian
											L"A tomada introduzida n�o pode ser utilizada.  Introduza um n�mero de tomada entre 0 e 65 536.\n",						// Portuguese
											L"O soquete inserido n�o pode ser usado.  Insira um n�mero de soquete entre 0 e 65.536.\n"								// Portuguese (Brazil)
										}
	},
	{ EISTR_INSUFFICIENT_DILUENT	,	{	L"Insufficient reagents exist in the disc to complete the run. Rerun sample with new disc.\n",														// English
											L"La quantit� de r�actifs dans le disque est insuffisante pour ex�cuter la proc�dure. R�ex�cuter l'�chantillon � l'aide du nouveau disque.\n",		// French
											L"In der Scheibe befinden sich nicht ausreichend Reagenzien, um den Vorgang abzuschlie�en. Wiederholen Sie die Probe mit einer neuen Scheibe.\n",	// German
											L"No hay suficientes reactivos en el disco para completar su uso. Vuelva a utilizar la muestra con un nuevo disco.\n",								// Spanish
											L"Nel disco non vi sono sufficienti reagenti per completare l'operazione. Riavviare il campione con un nuovo disco.\n",								// Italian
											L"N�o existem reagentes suficientes no disco para concluir a execu��o. Executar novamente a amostra utilizando um disco novo.\n",					// Portuguese
											L"Existem reagentes insuficientes no disco para concluir a execu��o. Execute novamente a amostra com o novo disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_SAMPLE		,	{	L"The disc has not received enough sample to function properly. Rerun using a new disc.\n",										// English
											L"Le disque n'a pas re�u suffisamment d'�chantillons pour fonctionner correctement. R�ex�cuter avec un nouveau disque.\n",		// French
											L"Die Scheibe enth�lt keine ausreichende Probenmenge f�r eine ordnungsgem��e Funktion. Mit einer neuen Scheibe wiederholen.\n",	// German
											L"El disco no ha recibido suficiente muestra para funcionar correctamente. �selo con un nuevo disco.\n",							// Spanish
											L"Il disco non ha ricevuto campione sufficiente per funzionare nel modo corretto. Riavviare utilizzando un nuovo disco.\n",		// Italian
											L"O disco n�o recebeu amostra suficiente para funcionar adequadamente. Executar novamente utilizando um disco novo.\n",			// Portuguese
											L"O disco n�o recebeu amostra suficiente para funcionar corretamente. Execute novamente usando um novo disco.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_STORAGE	,	{	L"There is insufficient free space on the USB storage device to complete the service archive. Please remove the USB storage device, and connect a USB storage device with at least 256 MB of available space.\n",										// English
											L"Espace de stockage sur p�riph�rique USB insuffisant pour r�aliser l'archive de service. Retirer le p�riph�rique de stockage USB et brancher un p�riph�rique de stockage USB ayant au moins 25 Mo d'espace disponible.\n",								// French
											L"Es ist nicht gen�gend Speicherkapazit�t auf dem USB-Speicherger�t frei, um das Service-Archiv zu vollenden. Bitte entfernen Sie das USB-Speicherger�t und schlie�en Sie ein USB-Speicherger�t mit mindestens 256 MB freiem Speicher an.\n",			// German
											L"El dispositivo de almacenamiento USB no tiene espacio suficiente para finalizar el archivo de servicio. Extraiga el dispositivo y conecte otro con al menos 256 MB de espacio disponible.\n",															// Spanish
											L"Spazio libero sul dispositivo di archiviazione USB insufficiente per completare l'archivio di servizio. Pregasi rimuovere il dispositivo di archiviazione USB e collegare un dispositivo di archiviazione con almeno 256 MB di spazio disponibile.\n",	// Italian
											L"O dispositivo de armazenamento USB n�o tem espa�o suficiente para finalizar o ficheiro de servi�o. Remova o dispositivo de armazenamento USB e insira outro com pelo menos 256 MB de espa�o dispon�vel.\n",											// Portuguese
											L"N�o h� espa�o livre suficiente no dispositivo de armazenamento USB para concluir o arquivo de servi�o. Remova o dispositivo de armazenamento USB e conecte um dispositivo de armazenamento USB com pelo menos 256 MB de espa�o dispon�vel.\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_INTERNAL_COMM			,	{	L"The analyzer lost internal communication. If symptom continues, contact Technical Service.\n",												// English
											L"L'analyseur a perdu sa capacit� de communication interne. En cas de persistance des sympt�mes, contacter le service technique.\n",			// French
											L"Das Analyseger�t hat die interne Kommunikation verloren. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",		// German
											L"El analizador perdi� la comunicaci�n interna. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"L'analizzatore ha perso la comunicazione interna. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"O analisador perdeu a comunica��o interna. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",							// Portuguese
											L"O analisador perdeu a comunica��o interna. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_INTERNAL_SOFTWARE		,	{	L"The analyzer detected an error with the software.  If symptom continues, contact Technical Service.\n",									// English
											L"L'analyseur a d�tect� une erreur logiciel. En cas de persistance des sympt�mes, contacter le service technique.\n",						// French
											L"Das Analyseger�t hat einen Softwarefehler festgestellt. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",		// German
											L"El analizador detect� un error con el software. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"L'analizzatore ha individuato un errore nel software. Se i sintomi continuano contattare il servizio tecnico.\n",							// Italian
											L"O analisador detetou um erro no software.  Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",							// Portuguese
											L"O analisador detectou um erro no software.  Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_IQC						,	{	L"The analyzer Intelligent Quality Control (iQC) reagent dye beads differ too much from the expected values. Rerun using a new disc.\n",													// English
											L"Les billes de teinture du r�actif Intelligent Quality Control (iQC) divergent trop des valeurs pr�vues. R�ex�cuter avec un nouveau disque.\n",											// French
											L"Die Intelligent Quality Control (iQC) Reagenz-Farbstoff-Beads des Analyseger�ts weichen zu stark von den erwarteten Werten ab. Mit einer neuen Scheibe wiederholen.\n",				// German
											L"Las cuentas de tinte de reactivo Intelligent Quality Control (iQC) del analizador son muy distintas a los valores esperados. �selo con un nuevo disco.\n",								// Spanish
											L"I granuli di colore reagente Intelligent Quality Control (iQC) dell'analizzatore sono troppo diversi dai valori previsti. Riavviare utilizzando un nuovo disco.\n",					// Italian
											L"As esferas de reagente corante do Controlo de qualidade inteligente (iQC) do analisador s�o muito diferentes dos valores esperados. Executar novamente utilizando um disco novo.\n",	// Portuguese
											L"As esferas do corante reagente de Controle de qualidade inteligente (iQC) do analisador diferem muito dos valores esperados. Execute novamente usando um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_LAMP					,	{	L"The lamp is checked each time to make sure it is still flashing when the optical cuvette is sampled. Detectors did not recognize the lamp flashes. If symptom continues, contact Technical Service.\n",																					// English
											L"La lampe fait � chaque fois l'objet d'une v�rification afin de s'assurer qu'elle fonctionne toujours lors du pr�l�vement sur la cuvette optique. Les d�tecteurs n'ont pas reconnu les clignotements de la lampe. En cas de persistance des sympt�mes, contacter le service technique.\n",	// French
											L"Die Lampe wird jedes Mal auf Blinkzeichen �berpr�ft, wenn eine Probe der optischen Cuvette untersucht wird. Es wurden keine Blinkzeichen der Lampen festgestellt. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",												// German
											L"Siempre se comprueba la l�mpara para asegurar que sigue emitiendo flashes cuando se pone la muestra en la cubeta �ptica. Los detectores no registraron la emisi�n de flashes de la l�mpara. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",		// Spanish
											L"La lampada viene controllata ogni volta per assicurarsi che stia ancora lampeggiando quando viene campionata la cuvette ottica. I rilevatori non hanno riconosciuto i flash luminosi. Se i sintomi continuano contattare il servizio tecnico.\n",											// Italian
											L"A l�mpada � sempre verificada para garantir que emite flashes quando a amostra � colocada na cuvete �tica. Os detetores n�o registram a emiss�o de flashes da l�mpada. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",												// Portuguese
											L"A l�mpada � verificada a cada vez, para certificar-se de que ainda esteja piscando quando a cubeta �ptica for amostrada. Os detectores n�o reconheceram a luz da l�mpada. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"												// Portuguese (Brazil)
										} 
	},
	{ EISTR_LIGHT_NOISE				,	{	L"There was high variability in the signal. This may impact the accuracy of the analyzer's measurement of absorbance. If symptom continues, contact Technical Service.\n",											// English
											L"Le signal d'arri�re-plan a pr�sent� une variabilit� �lev�e. ce qui peut affecter la pr�cision de la mesure d'absorbance par l'analyseur. En cas de persistance des sympt�mes, contacter le service technique.\n",	// French
											L"Es bestand eine hohe Signalvarianz. Dies kann die Genauigkeit der Absorbierungsmessung des Analyseger�ts beeintr�chtigen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",				// German
											L"Hab�a una gran variabilidad en la se�al. Esto puede afectar a la precisi�n de la medida de absorbancia del analizador. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"Grande variabilit� nel segnale. Esso potrebbe influire sulla precisione della misurazione di assorbenza dell'analizzatore. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"Foi observada uma grande variabilidade no sinal. Isto pode afetar a precis�o da medi��o da absorv�ncia do analisador. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",						// Portuguese
											L"Houve alta variabilidade no sinal. Isso pode afetar a precis�o da medi��o de absorb�ncia do analisador. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"											// Portuguese (Brazil)
										} 
	},
	{ EISTR_MISSING_FILTER_CURV		,	{	L"The filter curve calibration factors have not been loaded into the analyzer. Contact Technical Service for assistance.\n",									// English
											L"Les facteurs de calibrage de la courbe du filtre n'ont pas �t� charg�s sur l'analyseur. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Die Kalibrierungsfaktoren der Filterkurve wurden nicht in das Analyseger�t geladen. Wenden Sie sich an den technischen Service.\n",						// German
											L"Los factores de calibraci�n de la curva del filtro no se han cargado en el analizador. P�ngase en contacto con el servicio t�cnico.\n",					// Spanish
											L"I fattori di calibrazione della curva del filtro non sono stati caricati nell'analizzatore. Per assistenza contattare il servizio tecnico.\n",				// Italian
											L"Os fatores da curva de calibra��o do filtro n�o foram carregados no analisador. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",		// Portuguese
											L"Os fatores de calibra��o da curva do filtro n�o foram carregados no analisador. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_MOTOR_CONTROLLER		,	{	L"The analyzer detected an error with the motor controller. If symptom continues, contact Technical Service.\n",												// English
											L"L'analyseur a d�tect� une erreur sur le contr�leur du moteur. En cas de persistance des sympt�mes, contacter le service technique.\n",						// French
											L"Das Analyseger�t hat einen Fehler an der Motorsteuerung festgestellt. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",			// German
											L"El analizador detect� un error con el controlador del motor. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"L'analizzatore ha individuato un errore con il controllore a motore. Se i sintomi continuano contattare il servizio tecnico.\n",							// Italian
											L"O analisador detetou um erro no controlador do motor. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",								// Portuguese
											L"O analisador detectou um erro no controlador do motor. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_NEEDS_SERVICE			,	{	L"The analyzer may need servicing. Contact Technical Service for further details.\n",													// English
											L"Il se peut que l'analyseur n�cessite une intervention technique. Pour obtenir plus d'informations, contacter le service technique.\n",	// French
											L"Das Analyseger�t mu� m�glicherweise gewartet werden. Wenden Sie sich an den technischen Service.\n",									// German
											L"Puede que el analizador deba ser reparado. P�ngase en contacto con el servicio t�cnico.\n",											// Spanish
											L"L'analizzatore potrebbe aver bisogno di manutenzione. Per ulteriore dettagli contattare il servizio tecnico.\n",						// Italian
											L"O analisador pode necessitar de assist�ncia. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",				// Portuguese
											L"O analisador pode precisar de manuten��o. Entre em contato com o Servi�o T�cnico para obter detalhes adicionais.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_NO_FILES_TO_ARCHIVE		,	{	L"There are no files within the date range specified to archive.\n",					// English
											L"Aucun fichier � archiver dans la plage de dates sp�cifi�e.\n",						// French
											L"F�r den spezifizierten Datumsbereich sind keine Dateien zu archivieren.\n",		// German
											L"No hay ning�n archivo en el intervalo de fechas especificado para archivar.\n",	// Spanish
											L"Non sono presenti file da archiviare nel range di date specificato.\n",			// Italian
											L"N�o existem ficheiros no intervalo de datas especificado para arquivar. \n",		// Portuguese
											L"N�o h� arquivos dentro do intervalo de datas especificado para arquivar. \n"		// Portuguese (Brazil)
										}	
	},

	{ EISTR_NO_FILES_TO_TRANSMIT	,	{	L"There are no files within the date range specified to transmit.\n",				// English
											L"Aucun fichier � transmettre dans la plage de dates sp�cifi�e.\n",					// French
											L"F�r den spezifizierten Datumsbereich sind keine Dateien zu �bertragen.\n",			// German
											L"No hay ning�n archivo en el intervalo de fechas especificado para transmitir.\n",	// Spanish
											L"Non sono presenti file da trasmettere nel range di date specificato.\n",			// Italian
											L"N�o existem ficheiros no intervalo de datas especificado para transmitir.\n",		// Portuguese
											L"N�o h� arquivos dentro do intervalo de datas especificado para transmitir.\n"		// Portuguese (Brazil)
										}	
	},

	{ EISTR_NO_RESULTS_TO_TRANSMIT	,	{	L"There are no results within the date range specified to transmit.\n",				// English
											L"Aucun r�sultat � transmettre dans la plage de dates sp�cifi�e.\n",					// French
											L"F�r den spezifizierten Datumsbereich sind keine Ergebnisse zu �bertragen.\n",		// German
											L"No hay ning�n resultado en el intervalo de fechas especificado para transmitir.\n",// Spanish
											L"Non sono presenti risultati da trasmettere nel range di date specificato.\n",		// Italian
											L"N�o existem resultados no intervalo de datas especificado para transmitir.\n",		// Portuguese
											L"N�o h� resultados dentro do intervalo de datas especificado para transmitir.\n"	// Portuguese (Brazil)
										}	
	},

	{ EISTR_ORDER_WAS_CANCELED		,	{	L"The selected order was canceled by the external order entry system.\n",						// English
											L"La commande s�lectionn�e a �t� annul�e par le syst�me d'entr�e de commande externe.\n",		// French
											L"Die ausgew�hlte Bestellung wurde vom externen Bestelleingabesystem storniert.\n",				// German
											L"El pedido seleccionado ha sido cancelado por el sistema externo de entrada de pedidos.\n",		// Spanish
											L"Ordine selezionato cancellato dal sistema di immissione ordini esterno.\n",					// Italian
											L"O pedido selecionado foi cancelado pelo sistema de entrada de pedidos externo.\n",				// Portuguese
											L"A solicita��o selecionada foi cancelada pelo sistema externo de entrada de solicita��es.\n"	// Portuguese (Brazil)
										}
	},

	{ EISTR_OP_ID_LIST_FULL			,	{	L"All the available space for Operator ID's is used.  Please remove at least one Operator ID before adding a new Operator ID.\n",													// English
											L"Tout l'espace disponible r�serv� aux ID op�rateur est utilis�. Veuillez supprimer au moins un ID op�rateur avant d'en ajouter un nouveau.\n",										// French
											L"Der gesamte Speicherplatz f�r Bediener-IDs ist belegt.  Bitte entfernen Sie mindestens eine Bediener-ID, bevor Sie eine neue hinzuf�gen.\n",										// German
											L"Se ha utilizado todo el espacio disponible para n�meros de identificaci�n de operarios. Elimine al menos un n�mero de identificaci�n de operario antes de a�adir otro nuevo.\n",	// Spanish
											L"� stato utilizzato tutto lo spazio disponibile per l'ID operatore. Rimuovere almeno un ID operatore prima di aggiungere un nuovo ID operatore.\n",									// Italian
											L"Foi utilizado todo o espa�o dispon�vel para IDs de operadores.  Elimine pelo menos uma ID de operador antes de adicionar uma nova ID de operador.\n",								// Portuguese
											L"Todo o espa�o dispon�vel para os IDs de operador est� usado.  Remova pelo menos um ID de operador antes de adicionar um novo ID de operador.\n"									// Portuguese (Brazil)
										}
	},

	{ EISTR_OP_RENEWAL_REQUIRED		,	{	L"The date set authorizing the operator to use the analyzer has passed.  Please contact technical support for more information.\n",										// English
											L"La date pr�vue pour autorisation de l'op�rateur � utiliser l'analyseur est d�pass�e. Pour plus d'informations, contacter l'assistance technique.\n",					// French
											L"Das angegebene Datum, bis zu dem der Bediener das Analyseger�t benutzen kann, ist abgelaufen. Wenden Sie sich an den technischen Service f�r weitere Information.\n",	// German
											L"La fecha para autorizar al operario a usar el analizador ya ha pasado. P�ngase en contacto con el servicio t�cnico.\n",												// Spanish
											L"La data impostata per l'analisi dei campioni di controllo � stata superata. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",						// Italian
											L"A data definida para autorizar o operador a utilizar o analisador foi excedida.  Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",			// Portuguese
											L"A data configurada, autorizando o operador a usar o analisador, j� passou.  Entre em contato com o suporte t�cnico para obter mais informa��es.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_PATIENT_IDS_DONT_MATCH	,	{	L"The patient ID entered does not match the ID transmitted to the analyzer.\n",							// English
											L"L'ID patient entr�e ne correspond pas � l'ID transmise � l'analyseur.\n",								// French
											L"Die eingegebene Patienten-ID stimmt nicht mit der an das Analyseger�t �bermittelten ID �berein.\n",	// German
											L"El Id. del paciente introducido no coincide con el Id. transmitido al analizador.\n",					// Spanish
											L"L'ID paziente inserito non corrisponde a quello trasmesso all'analizzatore.\n",						// Italian
											L"A ID de doente introduzida n�o coincide com a ID transmitida ao analisador.\n",						// Portuguese
											L"O ID do paciente inserido n�o corresponde ao ID transmitido para o analisador.\n"						// Portuguese (Brazil)
										}
	},
	{ EISTR_POWER_SUPPLY			,	{	L"The analyzer has detected power fluctuations that prevent the unit from operating correctly. If symptom continues, contact Technical Service.\n",																// English
											L"L'analyseur a d�tect� des fluctuations dans l'alimentation qui emp�chent l'appareil de fonctionner correctement. En cas de persistance des sympt�mes, contacter le service technique.\n",						// French
											L"Das Analyseger�t hat Energieschwankungen festgestellt, die den ordnungsgem��en Betrieb der Einheit beeintr�chtigen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",				// German
											L"El analizador ha detectado fluctuaciones de la corriente que no permiten el funcionamiento correcto de la unidad. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"L'analizzatore ha individuato fluttuazioni di corrente che non permettono di funzionare correttamente. Se i sintomi continuano contattare il servizio tecnico.\n",												// Italian
											L"O analisador detetou flutua��es de corrente que n�o permitem o correto funcionamento da unidade. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",										// Portuguese
											L"O analisador detectou flutua��es de energia que impedem a unidade de funcionar corretamente. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"												// Portuguese (Brazil)
										} 
	},
	{ EISTR_PRINTER_NOT_FOUND		,	{	L"The external printer is not available. Check that the printer is connected to the analyzer, is turned on and has paper. If symptom continues, contact Technical Service.\n",																	// English
											L"L'imprimante externe n'est pas disponible. V�rifier que l'imprimante est bien raccord�e � l'analyseur, qu'elle fonctionne et qu'elle dispose d'une r�serve de papier. En cas de persistance des sympt�mes, contacter le service technique.\n",	// French
											L"Der externe Drucker ist nicht verf�gbar. Pr�fen Sie die Verbindung zwischen Drucker und Analyseger�t und ob der Drucker eingeschaltet ist und �ber Papier verf�gt. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",	// German
											L"La impresora externa no est� disponible. Compruebe que la impresora est� conectada al analizador, que est� encendida y que tiene papel. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.n",				// Spanish
											L"La stampante esterna non � disponibile. Verificare che la stampante sia collegata all'analizzatore, che sia accesa e che non manchi carta. Se i sintomi continuano contattare il servizio tecnico.\n",											// Italian
											L"A impressora externa n�o est� dispon�vel. Verifique que se a impressora est� ligada ao analisador, que est� ligada e que tem papel. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",									// Portuguese
											L"A impressora externa n�o est� dispon�vel. Verifique se a impressora est� conectada ao analisador, se est� ligada e se tem papel. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"											// Portuguese (Brazil)
										} 
	},
	{ EISTR_PROCESS_INIT			,	{	L"The analyzer is unable to start a process required to operate. Contact Technical Service for assistance.\n",									// English
											L"L'analyseur ne parvient pas � lancer la proc�dure requise pour fonctionner. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Das Analyseger�t kann einen f�r den Betrieb erforderlichen Vorgang nicht starten. Wenden Sie sich an den technischen Service.\n",				// German
											L"El analizador no puede iniciar un proceso necesario para funcionar. P�ngase en contacto con el servicio t�cnico.\n",							// Spanish
											L"L'analizzatore non � in grado di iniziare una procedura necessaria al funzionamento. Per assistenza contattare il servizio tecnico.\n",		// Italian
											L"O analisador n�o consegue iniciar um processo necess�rio para funcionar. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											L"O analisador n�o � capaz de iniciar um processo necess�rio para operar. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_PRODUCT_CODE			,	{	L"The product codes in the analyzer do not match. Contact Technical Service for more details.\n",									// English
											L"Les codes produits dans l'analyseur ne correspondent pas. Pour plus d'informations, contacter le service technique.\n",			// French
											L"Die Produktcodes in dem Analyseger�t passen nicht zusammen. Wenden Sie sich an den technischen Service.\n",						// German
											L"Los c�digos de producto en el analizador no se corresponden. P�ngase en contacto con el servicio t�cnico para m�s informaci�n.\n",	// Spanish
											L"I codici prodotto nell'analizzatore non combaciano. Per ulteriore dettagli contattare il servizio tecnico.\n",						// Italian
											L"Os c�digos de produto no analisador n�o coincidem. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",		// Portuguese
											L"Os c�digos de produto no analisador n�o correspondem. Entre em contato com o Servi�o T�cnico para obter mais detalhes.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_REAGENT_DISTRIBUTION	,	{	L"Reagent beads have not dissolved as expected. Rerun sample with a new disc.\n",											// English
											L"Les billes de r�actif ne se sont pas dissoutes comme pr�vu. R�ex�cuter l'�chantillon � l'aide d'un nouveau disque.\n",		// French
											L"Reagenz-Beads wurden nicht erwartungsgem�� aufgel�st. Wiederholen Sie die Probe mit einer neuen Scheibe.\n",				// German
											L"Las cuentas de reactivo no se han disuelto como se esperaba. Vuelva a utilizar la muestra con un nuevo disco.\n",			// Spanish
											L"I granuli di reagente non si sono dissolti come previsto. Riavviare il campione con un nuovo disco.\n",					// Italian
											L"As esferas de reagente n�o se dissolveram conforme esperado. Executar novamente a amostra utilizando um disco novo.\n",	// Portuguese
											L"As esferas de reagente n�o se dissolveram conforme esperado. Execute novamente a amostra com um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_RESULT_CORRUPTION		,	{	L"The saved results are corrupt.  All the previous results have been lost.  A new set of saved results has been started.  If symptom continues, contact Technical Service.\n"																// English
											L"Les r�sultats enregistr�s sont corrompus.  Tous les r�sultats ant�rieurs ont �t� perdus.  Un nouvel ensemble de r�sultats enregistr�s a �t� initi�.  Si les sympt�mes persistent, contacter le service technique.\n",						// French
											L"Die gespeicherten Ergebnisse sind fehlerhaft. Alle bisherigen Ergebnisse sind verloren gegangen. Es wurde ein neuer Satz gespeicherter Ergebnisse gestartet. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",	// German
											L"Los resultados guardados est�n da�ados.  Se han perdido todos los resultados anteriores.  Se ha iniciado una nueva serie de resultados guardados.  Si los s�ntomas contin�an, comun�quese con el servicio t�cnico.\n",						// Spanish
											L"I risultati salvati sono danneggiati.  Tutti i risultati precedenti sono andati perduti.  � stato iniziato un nuovo insieme di risultati.  Se il sintomo persiste, contattare il servizio tecnico.\n",										// Italian
											L"Os resultados guardados est�o danificados.  Todos os resultados anteriores foram perdidos.  Foi iniciado um novo conjunto de resultados guardados.  Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",				// Portuguese
											L"Os resultados salvos est�o corrompidos.  Todos os resultados anteriores foram perdidos.  Um novo conjunto de resultados salvos foi iniciado.  Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"							// Portuguese (Brazil)
										}
	},
	{ EISTR_ROTOR_DETECTED			,	{	L"A disc may be in the analyzer and may prevent the analyzer from starting normally. Press Continue to remove the disc.\n",										// English
											L"Il peut y avoir dans l'analyseur un disque susceptible de l'emp�cher de d�marrer normalement. Appuyer sur Continuer pour retirer le disque.\n",				// French
											L"Eine Scheibe kann sich in dem Analyseger�t befinden und den Startvorgang des Analyseger�ts beeintr�chtigen. Zur Entfernung der Scheibe dr�cken Sie Weiter.\n",	// German
											L"Puede que haya un disco en el analizador, lo que impide que el analizador se inicie correctamente. Pulse en Continuar para retirar el disco.\n",				// Spanish
											L"Nell'analizzatore potrebbe essere rimasto un disco che impedisce un'attivazione normale dell'analizzatore. Premere Continua per rimuovere il disco.\n",		// Italian
											L"Pode estar um disco no analisador, o que pode impedir que o analisador inicie corretamente. Prima Continuar para remover o disco.\n",							// Portuguese
											L"Um disco pode estar no analisador e impedir que o analisador inicie normalmente. Pressione Continuar para remover o disco.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_RQC						,	{	L"The Reagent QC beads recovered below the limit. The disc may not have been stored correctly. Rerun with a new disc.\n",												// English
											L"La r�cup�ration des billes de r�actif QC est inf�rieure � la limite. Il se peut que le disque n'ait pas �t� rang� correctement. R�ex�cuter avec un nouveau disque.\n",	// French
											L"Die Reagenz-QC-Beads liegen unter dem Grenzwert. Die Scheibe wurde m�glicherweise nicht ordnungsgem�� gelagert. Mit einer neuen Scheibe wiederholen.\n",				// German
											L"Cuentas QC de reactivo recuperadas por debajo del l�mite. Puede que el disco no se haya guardado correctamente. Debe usarse con un nuevo disco.\n",					// Spanish
											L"I granuli QC di reagente si sono ristabiliti sotto al limite. Il disco potrebbe non essere stato conservato correttamente. Riavviare con un nuovo disco.\n",			// Italian
											L"As esferas de reagente de CQ foram recuperadas abaixo do limite. O disco pode n�o ter sido guardado corretamente. Executar novamente utilizando um disco novo.\n",		// Portuguese
											L"As esferas de Reagente de QC se recuperaram abaixo do limite. O disco pode n�o ter sido armazenado corretamente. Execute novamente com um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_BLANK			,	{	L"The sample blank bead appears to be turbid. This can be due to sample quality. Confirm sample integrity and rerun with a new disc.\n",															// English
											L"La bille vierge de l'�chantillon semble trouble, ce qui peut venir de la qualit� de l'�chantillon. V�rifier l'int�grit� de l'�chantillon et r�ex�cuter avec un nouveau disque.\n",				// French
											L"Die Leer-Beadprobe ist tr�be. Dies kann auf die Probenqualit�t zur�ckgef�hrt werden. Best�tigen Sie die Unversehrtheit der Probe und wiederholen Sie den Vorgang mit einer neuen Scheibe.\n",	// German
											L"La cuenta virgen de la muestra parece estar turbia. Esto puede deberse a la calidad de la muestra. Confirme la integridad de la muestra y vuelva a utilizar el nuevo disco.\n",				// Spanish
											L"I granuli campione vuoti sembrano torbidi. Ci� pu� essere causato dalla qualit� del campione. Confermare l'integrit� del campione e riavviare con un nuovo disco.\n",							// Italian
											L"A esfera de amostra de branco parece estar turva. Isto pode ser devido � qualidade da amostra. Confirme a integridade da amostra e execute novamente utilizando um disco novo.\n",				// Portuguese
											L"A esfera em branco da amostra parece estar turva. Isso pode ser decorrente da qualidade da amostra. Confirme a integridade da amostra e execute novamente com um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_DISTRIBUTION		,	{	L"Sample has not reached the chemistries. Rerun with a new disc.\n",										// English
											L"L'�chantillon n'est pas parvenu jusqu'aux produits chimiques. R�ex�cuter avec un nouveau disque.\n",	// French
											L"Die Probe hat nicht die Chemie erreicht. Mit einer neuen Scheibe wiederholen.\n",						// German
											L"La muestra no ha alcanzado los compuestos qu�micos. Debe usarse con un nuevo disco.\n",				// Spanish
											L"Il campione non ha raggiunto le sostanze chimiche. Riavviare con un nuovo disco.\n",					// Italian
											L"A amostra n�o atingiu as subst�ncias qu�micas. Executar novamente utilizando um disco novo.\n",		// Portuguese
											L"A amostra n�o atingiu as subst�ncias qu�micas. Execute novamente com um novo disco.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_MIX				,	{	L"The sample has not mixed with reagent materials sufficiently. Rerun with a new disc.\n",									// English
											L"L'�chantillon ne s'est pas suffisamment m�lang� avec les mati�res de r�actifs. R�ex�cuter avec un nouveau disque.\n",		// French
											L"Die Probe hat sich nicht ausreichend mit Reagenzmaterialien vermischt. Mit einer neuen Scheibe wiederholen.\n",			// German
											L"La muestra no se ha mezclado suficientemente con los materiales reactivos. Debe usarse con un nuevo disco.\n",				// Spanish
											L"Il campione non si � mescolato a sufficienza con le sostanze reagenti. Riavviare con un nuovo disco.\n",					// Italian
											L"A amostra n�o se misturou suficientemente com os materiais dos reagentes. Executar novamente utilizando um disco novo.\n",	// Portuguese
											L"A amostra n�o foi misturada com os materiais reagentes suficientemente. Execute novamente com um novo disco.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_QUALITY			,	{	L"The sample may have one or more physical interferents. Confirm the sample integrity and rerun disc.\n",											// English
											L"Il se peut que l'�chantillon pr�sente un ou plusieurs �l�ments interf�rents. V�rifier l'int�grit� de l'�chantillon et r�ex�cuter le disque.\n",	// French
											L"Die Probe hat m�glicherweise eine oder mehrere physische st�rende Bestandteile. Unversehrtheit der Probe best�tigen und Scheibe neu starten.\n",	// German
											L"Puede que la muestra tenga una o varias interferencias f�sicas. Confirme la integridad de la muestra y vuelva a usar el disco.\n",					// Spanish
											L"Il campione potrebbe avere uno o pi� fattori di interferenza fisica. Confermare l'integrit� del campione e riavviare il disco.\n",					// Italian
											L"A amostra pode ter uma ou v�rias interfer�ncias f�sicas. Confirme a integridade da amostra e execute novamente o disco.\n",						// Portuguese
											L"A amostra pode ter um ou mais interferentes f�sicos. Confirme a integridade da amostra e execute novamente o disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_SATURATION				,	{	L"The photometer in the analyzer has detected a brighter than normal flash. If symptom continues, contact Technical Service.\n",												// English
											L"Le photom�tre de l'analyseur a d�tect� un clignotement plus lumineux que la normale. En cas de persistance des sympt�mes, contacter le service technique.\n",				// French
											L"Das Fotometer in dem Analyseger�t hat ein helleres Blinkzeichen als normal festgestellt. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",		// German
											L"El fot�metro del analizador ha detectado un flash m�s brillante de lo normal. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"Il fotometro nell'analizzatore ha individuato un flash pi� luminoso del normale. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"O fot�metro no analisador detetou um flash mais brilhante do que o normal. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",							// Portuguese
											L"O fot�metro no analisador detectou um flash mais brilhante que o normal. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_SOFTWARE_EXPIRED		, {		L"This Analyzer is for Investigational Use Only.  Please return the Analyzer to Abaxis Research and Development for renewal.\n",															// English
											L"Cet analyseur est utilis� � des fins d'investigation uniquement. Retourner l'analyseur � Abaxis Research and Development pour le faire remplacer.\n",									// French
											L"Dieses Analyseger�t ist ausschlie�lich f�r die Verwendung als Untersuchungsger�t bestimmt. Geben Sie das Analyseger�t zur Erneuerung an Abaxis Research and Development zur�ck.\n",	// German									// German
											L"Este analizador es solo para su uso en investigaci�n. Devuelva el analizador al departamento de investigaci�n y desarrollo de Abaxis para su renovaci�n.\n",							// Spanish
											L"Questo Analizzatore � destinato esclusivamente a scopo di ricerca.  Restituire l'Analizzatore a Abaxis Ricerca e Sviluppo per il rinnovo.\n",											// Italian
											L"Este analisador destina-se apenas a ser utilizado para investiga��o.  Devolva o analisador ao departamento de investiga��o e desenvolvimento da Abaxis para renova��o.\n",				// Portuguese
											L"Este analisador destina-se apenas ao uso em investiga��o.  Devolva o analisador para a Abaxis Research and Development, para renova��o.\n"												// Portuguese (Brazil)
                                      }
    },
	{ EISTR_SOFTWARE_UPDATE			,	{	L"The disc inserted requires a software update to operate correctly. Contact Technical Service for further assistance.\n",														// English
											L"Le disque ins�r� n�cessite une mise � jour de logiciel pour pouvoir fonctionner correctement. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											L"Die eingesetzte Scheibe erfordert ein Software-Update f�r einen ordnungsgem��en Betrieb. Wenden Sie sich an den technischen Service.\n",										// German
											L"El disco insertado necesita la actualizaci�n del software para operar correctamente. P�ngase en contacto con el servicio t�cnico.\n",											// Spanish
											L"Il disco inserito richiede un aggiornamento del software per funzionare correttamente. Per ulteriore assistenza contattare il servizio tecnico.\n",							// Italian
											L"O disco inserido necessita de uma atualiza��o do software para funcionar adequadamente. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",					// Portuguese
											L"O disco inserido requer uma atualiza��o de software para funcionar corretamente. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SPINDLE_MOTOR_STUCK		,	{	L"Spindle motor not turning properly. Contact Technical Service for further assistance.\n",											// English
											L"Le moteur de l'axe ne tourne pas correctement. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",		// French
											L"Der Spindelmotor dreht sich nicht ordnungsgem��. Wenden Sie sich an den technischen Service.\n",									// German
											L"El motor del eje no est� girando correctamente. P�ngase en contacto con el servicio t�cnico.\n",									// Spanish
											L"L'asse motore non gira correttamente. Per ulteriore assistenza contattare il servizio tecnico\n",									// Italian
											L"O motor de eixo n�o est� a rodar adequadamente. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",				// Portuguese
											L"O motor do fuso n�o est� girando corretamente. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_STARTING_ABSORBANCE		,	{	L"Starting absorbance of the sample blank bead is outside the limit, possibly due to an exogenous substance or distribution error. Rerun with new disc.\n",																		// English
											L"L'absorbance de d�part de la bille vierge de l'�chantillon n'entre pas dans les limites prescrites, �ventuellement du fait d'une substance exog�ne ou d'une erreur de distribution. R�ex�cuter � l'aide du nouveau disque.\n",	// French
											L"Die Startabsorbierung der Leer-Beadprobe befindet sich m�glicherweise aufgrund einer exogenen Substanz oder eines Verteilungsfehlers au�erhalb des Grenzwerts. Mit einer neuen Scheibe wiederholen.\n",						// German
											L"La absorbancia de inicio de la cuenta virgen de muestra est� fuera del l�mite, probablemente debido a una sustancia ex�gena o a un error de distribuci�n. Utilice un nuevo disco.\n",											// Spanish
											L"L'assorbenza iniziale dei granuli campione vuoti non rientra nei limiti, forse a causa di una sostanza esogena o a un errore di distribuzione. Riavviare con un nuovo disco.\n",												// Italian
											L"A absorv�ncia inicial da esfera de amostra de branco est� fora do limite, possivelmente devido a uma sust�ncia ex�gena ou erro de distribui��o. Executar novamente utilizando um disco novo.\n",								// Portuguese
											L"A absor��o inicial da esfera em branco da amostra est� fora do limite, possivelmente devido a uma subst�ncia ex�gena ou um erro de distribui��o. Execute novamente com um novo disco.\n"										// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_SENSOR				,	{	L"The disc temperature sensor is not operating. Contact Technical Service for further assistance.\n",										// English
											L"Le d�tecteur de temp�rature du disque ne fonctionne pas. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											L"Der Temperatursensor der Scheibe ist au�er Betrieb. Wenden Sie sich an den technischen Service.\n",										// German
											L"El sensor de temperatura del disco no est� funcionando. P�ngase en contacto con el servicio t�cnico.\n",									// Spanish
											L"Il sensore di temperatura del disco non � in funzione. Per ulteriore assistenza contattare il servizio tecnico.\n",						// Italian
											L"O sensor de temperatura do disco n�o est� a funcionar. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",				// Portuguese
											L"O sensor de temperatura do disco n�o est� funcionando. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_VARIATION			,	{	L"The temperature deviated more than one degree during the run. The accuracy of certain chemistries may be affected. Rerun with new disc.\n",													// English
											L"La temp�rature a pr�sent� un �cart de plus d'un degr� au cours de l'ex�cution. La pr�cision de certains produits chimiques pourra en �tre affect�e. R�ex�cuter � l'aide du nouveau disque.\n",	// French
											L"Der Temperatur wich um mehr als ein Grad w�hrend des Betriebs ab. Die Genauigkeit bestimmter Chemie kann beeintr�chtigt worden sein. Mit einer neuen Scheibe wiederholen.\n",					// German
											L"La temperatura cambi� en m�s de un grado durante el an�lisis. Puede que afecte a la precisi�n de an�lisis de ciertas sustancias qu�micas. Utilice un nuevo disco.\n",							// Spanish
											L"Durante il funzionamento la temperatura � variata di pi� di un grado. La precisione di alcune sostanze chimiche potrebbe esserne stata influenzata. Riavviare con un nuovo disco.\n",			// Italian
											L"A altera��o da temperatura foi superior a um grau durante a execu��o. A precis�o de determinadas sust�ncias qu�micas pode ser afetada. Executar novamente utilizando um disco novo.\n",		// Portuguese
											L"A temperatura desviou mais de um grau durante a execu��o. A precis�o de certas subst�ncias qu�micas pode estar afetada. Execute novamente com um novo disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMPERATURE				,	{	L"The temperature of the disc either varied too much or was not in the proper range during the run. If symptom continues, contact Technical Service.\n",																			// English
											L"La temp�rature du disque a soit vari� dans des proportions trop importantes, soit ne s'inscrit pas dans la fourchette prescrite en cours d'ex�cution. En cas de persistance des sympt�mes, contacter le service technique.\n",	// French
											L"Die Scheibentemperatur variierte zu stark oder befand sich w�hrend des Betriebs au�erhalb des optimalen Bereichs. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",									// German
											L"La temperatura del disco vari� mucho o no estaba en el intervalo adecuado durante el an�lisis. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",										// Spanish
											L"Durante il funzionamento la temperatura del disco ha subito variazioni eccessive oppure non era nel range adatto. Se i sintomi continuano contattare il servizio tecnico.\n",													// Italian
											L"A temperatura do disco variou muito ou n�o esteve dentro do intervalo adequado durante a an�lise. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",														// Portuguese
											L"A temperatura do disco variou muito ou n�o permaneceu no intervalo adequado durante a execu��o. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"																// Portuguese (Brazil)
										} 
	},
	{ EISTR_TIMEOUT					,	{	L"The analyzer is not receiving required data in a timely fashion. If symptom continues, contact Technical Service.\n",										// English
											L"L'analyseur ne re�oit pas les donn�es voulues dans les d�lais. En cas de persistance des sympt�mes, contacter le service technique.\n",					// French
											L"Das Analyseger�t empf�ngt die erforderlichen Daten nicht rechtzeitig. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",			// German
											L"El analizador no est� recibiendo a tiempo los datos necesarios. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"L'analizzatore non riceve tempestivamente i dati necessari. Se i sintomi continuano contattare il servizio tecnico.\n",									// Italian
											L"O analisador n�o est� a receber atempadamente os dados necess�rios. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",				// Portuguese
											L"O analisador n�o est� recebendo os dados necess�rios em tempo h�bil. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_TOO_HOT					,	{	L"The analyzer is getting too hot and should be allowed to cool down for a while before further use.\n",						// English
											L"L'analyseur pr�sente une surchauffe. Il convient de le laisser refroidir un moment avant de continuer � l'utiliser.\n",	// French
											L"Das Analyseger�t wird zu hei� und sollte vor der weiteren Verwendung abk�hlen.\n",											// German
											L"El analizador se est� sobrecalentando y se deber�a dejar enfriar antes de volver a usarlo.\n",								// Spanish
											L"L'analizzatore si sta riscaldando eccessivamente e deve essere fatto raffreddare prima di essere usato nuovamente.\n",		// Italian
											L"O analisador est� aquecer muito e deve-se deixar arrefecer durante algum tempo antes de o voltar a utilizar.\n",			// Portuguese
											L"O analisador est� se tornando quente demais e deve ser reservado para esfriar por um tempo antes de continuar o uso.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_TOP_HEATER_HIGH			,	{	L"The top heater plate draws too much current when turned on at full power. Contact Technical Service for assistance.\n",														// English
											L"La plaque chauffante sup�rieure consomme trop d'�lectricit� lorsqu'elle fonctionne � fond. Pour obtenir une assistance, contacter le service technique.\n",					// French
											L"Die obere Heizplatte verbraucht bei Maximalbetrieb zu viel Strom. Wenden Sie sich an den technischen Service.\n",																// German
											L"La placa del calentador superior utiliza mucha corriente cuando se enciende a toda potencia. P�ngase en contacto con el servicio t�cnico.\n",									// Spanish
											L"Il disco di riscaldamento superiore prende troppa corrente quando � acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",							// Italian
											L"A placa do aquecedor superior utiliza elevada quantidade de corrente quando � ligada na pot�ncia m�xima. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											L"A placa superior do aquecedor consome muita corrente quando ligada na pot�ncia m�xima. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_TOP_HEATER_LOW			,	{	L"The top heater plate did not draw enough current when turned on at full power. Contact Technical Service for assistance.\n",															// English
											L"La plaque chauffante sup�rieure n'a pas consomm� suffisamment d'�lectricit� lorsqu'elle a fonctionn� � fond. Pour obtenir une assistance, contacter le service technique.\n",			// French
											L"Die obere Heizplatte verbrauchte bei Maximalbetrieb zu wenig Strom. Wenden Sie sich an den technischen Service.\n",																	// German
											L"La placa del calentador superior no utiliz� suficiente corriente cuando se puso a toda potencia. P�ngase en contacto con el servicio t�cnico.\n",										// Spanish
											L"Il disco di riscaldamento superiore non ha preso sufficiente corrente quando acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",							// Italian
											L"A placa superior do aquecedor n�o utiliza quantidade suficiente de corrente quando ligada na pot�ncia m�xima. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											L"A placa superior do aquecedor n�o atraiu corrente suficiente quando ligada na pot�ncia m�xima. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_UNAUTHORIZED_ADMIN		,	{	L"The ID entered for the administrator is not in the system. Please contact technical support for more information.\n",						// English
											L"L'ID saisi pour l'administrateur n'est pas sur le syst�me. Pour plus d'informations, contacter l'assistance technique.\n",					// French
											L"Die eingegebene Administrator-ID existiert nicht im System. Wenden Sie sich bitte an den technischen Service f�r weitere Information.\n",	// German
											L"Esta Id. administrador no est� en el sistema. P�ngase en contacto con el servicio t�cnico. \n",											// Spanish
											L"L'ID immesso per l'amministratore non � presente nel sistema. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",		// Italian
											L"A ID introduzida para o administrador n�o est� no sistema. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",		// Portuguese
											L"O ID inserido para o administrador n�o est� no sistema. Entre em contato com o suporte t�cnico para obter mais informa��es.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_UNAUTHORIZED_OP			,	{	L"The ID entered for the operator is not in the system. Please contact technical support for more information.\n",						// English
											L"L'ID saisi pour l'op�rateur n'est pas sur le syst�me. Pour plus d'informations, contacter l'assistance technique.\n",					// French
											L"Die eingegebene Bediener-ID existiert nicht im System. Wenden Sie sich bitte an den technischen Service f�r weitere Information.\n",	// German
											L"Esta Id. operario no est� en el sistema. P�ngase en contacto con el servicio t�cnico.\n",												// Spanish
											L"L'ID immesso per l'operatore non � presente nel sistema. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",		// Italian
											L"A ID introduzida para o operador n�o est� no sistema. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",		// Portuguese
											L"O ID inserido para o operador n�o est� no sistema. Entre em contato com o suporte t�cnico para obter mais informa��es.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_WRONG_ROTOR				,	{	L"The disc inserted is not intended for this device. If symptom continues, contact Technical Service.\n",										// English
											L"Le disque ins�r� n'est pas pr�vu pour cet appareil. En cas de persistance des sympt�mes, contacter le service technique.\n",					// French
											L"Die eingesetzte Scheibe ist f�r dieses Ger�t nicht geeignet. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",		// German
											L"El disco insertado no es apto para este aparato. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"Il disco inserito non � adatto a questo strumento. Se i sintomi continuano contattare il servizio tecnico.\n",									// Italian
											L"O disco inserido n�o � adequado para este dispositivo. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",					// Portuguese
											L"O disco inserido n�o � destinado a este dispositivo. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"						// Portuguese (Brazil)
										} 
	},


// Printer									ENGLISH,						FRENCH,							GERMAN,							SPANISH,							ITALIAN, 						PORTUGUESE,							PORTUGUESE (BRAZIL)
	{ PSTR_PRINTER_TEST				,	{	L"Printer Test",				L"Test imprimante",				L"Druckertest",					L"Prueba de impresora",				L"Prova stampante",				L"Teste da impressora",				L"Teste da impressora"	}	},

// Results (display and print)				ENGLISH,						FRENCH,							GERMAN,							SPANISH,							ITALIAN, 						PORTUGUESE,							PORTUGUESE (BRAZIL)
	{ RSTR_ACCEPTABLE_MINIMUM		,	{	L"Acceptable Minimum:",			L"Minimum Acceptable :",		L"Verwertbares min.:",			L"Minimo Aceptable:",				L"Minimo Accettabile:",			L"M�nimo aceit�vel:",				L"M�nimo aceit�vel:"	}	},
	{ RSTR_AGE						,	{	L"Age:",						L"�ge :",						L"Alter",						L"Edad:",							L"Et�:",						L"Idade:",							L"Idade:"	}	},
	{ RSTR_CHEMISTRY_QC				,	{	L"Chemistry QC:",				L"Chimie CQ :",					L"Chemie-QK:",					L"CC Quimico:",						L"QC Chimica:",					L"CQ Qu�mico:",						L"QC de qu�mica:"	}	},
	{ RSTR_CONFIRM_LOW_RECOVERIES	,	{	L"! CONFIRM LOW RECOVERIES !",	L"! CONFIRMER FAIBLE R�CUP !",	L"! NDR. WIEDERHERST. BEST�T.!", L"! CONFIRME BAJA RECUPERACI�N !",	L"! CONFERMA POCHI RECUPERI !",	L"! CONFIRME BAIXA RECUPERA��O !",	L"! CONFIRM. RECUPER. BAIXAS !"	}	},
	{ RSTR_CONTROL_ID				,	{	L"Control ID:",					L"ID contr.:",					L"Kontr.-ID",					L"Id. ctrol:",						L"ID contrl:",					L"ID do controlo:",					L"ID de controle:"	}	},
	{ RSTR_CONTROL_RENEWAL			,	{	L"Control Renewal Required",	L"Renouv. contr. n�cess.",		L"Kontrollerneuerung ben�tigt",	L"Renovar control",					L"Necess. rinnovo contr.",		L"Necess�rio renovar o controlo",	L"Renov. do controle necess�ria"	}	},
	{ RSTR_DEMONSTRATION_SOFTWARE	,	{	L"Demonstration Software",		L"Logiciel de d�monstration",	L"Demonstrationssoftware",		L"Demo",							L"Software di dimostrazione",	L"Software de demonstra��o",		L"Software de demonstra��o"	}	},
	{ RSTR_ERROR					,	{	L"ERROR",						L"ERREUR",						L"FEHLER",						L"ERROR",							L"ERRORE",						L"ERRO",							L"ERRO"	}	},
	{ RSTR_GENDER					,	{	L"Gender:",						L"Sexe :",						L"Geschlecht:",					L"Sexo:",							L"Sesso:",						L"Sexo:",							L"G�nero:"	}	},
	{ RSTR_INSTRUMENT_QC			,	{	L"Instrument QC:",				L"Instrument CQ:",				L"Instrument QK:",				L"Control calidad instr:",			L"Strum. CQ:",					L"CQ do instrumento:",				L"QC de instrumento:"	}	},
	{ RSTR_NC						,	{	L"N/C",							L"N/C",							L"N/B",							L"N/C",								L"N/C",							L"N/B",								L"N/C"	}	},
	{ RSTR_NEG						,	{	L"NEG",							L"NEG",							L"NEG",							L"NEG",								L"NEG",							L"NEG",								L"NEG"	}	},
	{ RSTR_OPERATOR_ID				,	{	L"Operator ID:",				L"ID op�rateur :",				L"Bediener-ID:",				L"Id. operario:",					L"ID operatore:",				L"ID do operador:",					L"ID do operador:"	}	},
	{ RSTR_OPERATOR_RENEWAL			,	{	L"Operator Renewal Required",	L"Renouv. op�r. n�cess.",		L"Bedienererneuerung ben�tigt",	L"Renovar operario",				L"Necess. rinnovo operat.",		L"Necess�rio renovar o operador",	L"Renov. de operador necess�ria"	}	},
	{ RSTR_PATIENT_ID				,	{	L"Patient ID:",					L"ID patient :",				L"Patienten-ID:",				L"Id. paciente:",					L"ID paziente:",				L"ID do doente:",					L"ID do paciente:"	}	},
	{ RSTR_POS						,	{	L"POS",							L"POS",							L"POS",							L"POS",								L"POS",							L"POS",								L"POS"	}	},
	{ RSTR_QC_OK					,	{	L"QC     OK",					L"QC     OK",					L"QUALITAETSKONTROLLE    OK",	L"CONTROL DE CALIDAD     OK",		L"CONTROLLO DI QUALITA   OK",	L"CQ     OK",						L"QC     OK"	}	},
	{ RSTR_RACE						,	{	L"Race:",						L"Race :",						L"Rasse:",						L"Raza:",							L"Razza:",						L"Ra�a:",							L"Ra�a:"	}	},
	{ RSTR_RANGE					,	{	L"Range:",						L"Registre :",					L"Bereich:",					L"Rango:",							L"Banda",						L"Intervalo:",						L"Intervalo:"	}	},
	{ RSTR_RANGE_LABEL				,	{	L"Range Label:",				L"�tiq.fourch:",				L"Bereichskz.:",				L"Intervalo:",						L"Etich.range:",				L"R�t. Inter.:",					L"R�t. inter.:"	}	},
	{ RSTR_RESEARCH_SOFTWARE		,	{	L"Research Software",			L"Logiciel recherche",			L"Research Software",			L"Software investig.",				L"Software ricerca",			L"Software de pesquisa",			L"Software de pesquisa"	}	},
	{ RSTR_ROTOR_LOT_NUM			,	{	L"Disc Lot Number:",			L"N� de lot disque :",			L"Scheiben-Losnr.:",			L"No. de lote del disco:",			L"Numero lotto disco:",			L"N�mero lote disco:",				L"N�m. de lote do disco:"	}	},
	{ RSTR_SAMPLE_TYPE				,	{	L"Sample Type:",				L"Type �ch. :",					L"Probentyp:",					L"Muestra:",						L"Tipo camp.:",					L"Tipo amost.:",					L"Tipo amostr:"	}	},
	{ RSTR_SERIAL_NUM				,	{	L"Serial Number:",				L"N� de s�rie :",				L"Seriennr.:",					L"N�mero de serie:",				L"Numero di serie:",			L"N�mero de s�rie:",				L"N�mero de s�rie:"	}	},
	{ RSTR_UNAUTHORIZED_OPERATOR	,	{	L"Unauthorized Operator",		L"Op�r. non autoris�",			L"Unautorisierter Bediener",	L"Operario no autorizado",			L"Operat. non autorizz.",		L"Operador n�o autorizado",			L"Operador n�o autorizado"	}	},
	{ RSTR_UNKNOWN					,	{	L"Unknown",						L"Inconnu",						L"Unbekannt",					L"Desconocido",						L"Sconosciuto",					L"Desconhecido",					L"Desconhecido"	}	},

// Sample ID labels for results				ENGLISH,			FRENCH,				GERMAN,				SPANISH,			ITALIAN, 			PORTUGUESE,				PORTUGUESE (BRAZIL)
	{ RSTR_ADMISSION_ID				,	{	L"Admission ID:",	L"ID admission :",	L"Zugangs-ID:",		L"Id. admisi�n:",	L"ID ammissione:",	L"ID de Admiss�o:",		L"ID de admiss�o:"	}	},
	{ RSTR_ALTERNATE_ID				,	{	L"Alternate ID:",	L"Autre ID :",		L"ID wechseln:",	L"Id. suplente:",	L"ID alternativo:",	L"ID suplente:",		L"ID alternativo:"	}	},
	{ RSTR_DOCTOR_ID				,	{	L"Doctor ID:",		L"ID m�decin :",	L"Arzt-ID:",		L"Id. del m�dico:",	L"ID medico:",		L"ID do m�dico:",		L"ID do m�dico:"	}	},
	{ RSTR_DOB						,	{	L"DOB:",			L"DOB :",			L"GD:",				L"F. nac.:",		L"DN:",				L"DOB:",				L"DN:"	}	},
	{ RSTR_LOCATION					,	{	L"Location:",		L"Emplcmt:",		L"Ort:",			L"Ubicac.:",		L"Sede:",			L"Localiza��o:",		L"Local:"	}	},
	{ RSTR_PHONE_NUM				,	{	L"Phone Number:",	L"N� t�l :",		L"Telefonnummer:",	L"Tel�fono:",		L"Num. telefono:",	L"N�m. telefone:",		L"N�m. telefone:"	}	},
	{ RSTR_SAMPLE_ID				,	{	L"Sample ID:",		L"ID �ch. :",		L"Proben-ID:",		L"Id. de muestra:",	L"ID campione:",	L"ID da amostra:",		L"ID da amostra:"	}	},
	
// Age labels for results					ENGLISH,	FRENCH,			GERMAN,		SPANISH,	ITALIAN, 		PORTUGUESE,		PORTUGUESE (BRAZIL)
	{ RSTR_DAY						,	{	L"Day",		L"Jour",		L"Tag",		L"D�a",		L"Giorno",		L"Dia",			L"Dia"	}	},
	{ RSTR_DAYS						,	{	L"Days",	L"Jours",		L"Tage",	L"D�as",	L"Giorni",		L"Dias",		L"Dias"	}	},
	{ RSTR_WEEK						,	{	L"Week",	L"Semaine",		L"Woche",	L"Semana",	L"Settimana",	L"Semana",		L"Semana"	}	},
	{ RSTR_WEEKS					,	{	L"Weeks",	L"Semaines",	L"Wochen",	L"Semanas",	L"Settimane",	L"Semanas",		L"Semanas"	}	},
	{ RSTR_MONTH					,	{	L"Month",	L"Mois",		L"Monat",	L"Mes",		L"Mese",		L"M�s",			L"M�s"	}	},
	{ RSTR_MONTHS					,	{	L"Months",	L"Mois",		L"Monate",	L"Meses",	L"Mesi",		L"Meses",		L"Meses"	}	},
	{ RSTR_YEAR						,	{	L"Year",	L"Ann�e",		L"Jahr",	L"A�o",		L"Anno",		L"Ano",			L"Ano"	}	},
	{ RSTR_YEARS					,	{	L"Years",	L"Ann�es",		L"Jahre",	L"A�os",	L"Anni",		L"Anos",		L"Anos"	}	},

// Genders									ENGLISH,		FRENCH,			GERMAN,				SPANISH,		ITALIAN, 	   		PORTUGUESE,	   		PORTUGUESE (BRAZIL)
	{ GSTR_FEMALE					,	{	L"Female",		L"F�minin",		L"Weiblich",		L"Mujer",		L"Uomo",			L"Feminino",		L"Sexo feminino"	}	},
	{ GSTR_MALE						,	{	L"Male",		L"Masculin",	L"M�nnlich",		L"Hombre",		L"Donna",			L"Masculino",		L"Sexo masculino"	}	},
	{ GSTR_NEUTERED					,	{	L"Neutered",	L"Ch�tr�",		L"Kastriert",		L"Castrado",	L"Castrato",		L"Castrado",		L"Castrado"	}	},
	{ GSTR_SPAYED					,	{	L"Spayed",		L"Ch�tr�e",		L"Sterilisiert",	L"Castrada",	L"Sterilizzato",	L"Esterilizado",	L"Esterilizado"	}	},
	{ GSTR_UNKNOWN					,	{	L"Unknown",		L"Inconnu",		L"Unbekannt",		L"Desconocido",	L"Sconosciuto",		L"Desconhecido",	L"Desconhecido"	}	},
	
// Race labels for results					ENGLISH,					FRENCH,						GERMAN,						SPANISH,						ITALIAN, 				PORTUGUESE,					PORTUGUESE (BRAZIL)
//	  RSTR_UNKNOWN - defined above
	{ RSTR_WHITE					,	{	L"White",					L"Blanche",					L"Wei�",					L"Blanco",					L"Bianco",					L"Branco",					L"Branco"	}	},
	{ RSTR_BLACK					,	{	L"Black",					L"Noire",					L"Schwarz",					L"Negro",					L"Nero",					L"Negro",					L"Preto"	}	},
	{ RSTR_ASIAN_PACIFIC_ISLANDER	,	{	L"Asian/Pacific Islander",	L"Asiat./orig. �les Pac.",	L"Asiatisch/Pazifisch",		L"Asi�tico/Isl. Pac�fico",	L"Isolano Asia/Pacifico",	L"Ilhas Asi�t./ Pac�fico",	L"Asi�tico/ilhas Pac�f."	}	},
	{ RSTR_NATIVE_AMERICAN			,	{	L"Native American",			L"Am�rindienne",			L"Indianische Abstammung",	L"Americano nativo",		L"Nativo americano",		L"Americano nativo",		L"Americano nativo"	}	},
	{ RSTR_HISPANIC					,	{	L"Hispanic",				L"Hispanique",				L"Hispanische Abstammung",	L"Hispano",					L"Ispanico",				L"Hisp�nico",				L"Hisp�nico"	}	},

// Species									ENGLISH,			FRENCH,				GERMAN,				SPANISH,	 		ITALIAN, 			PORTUGUESE,	 		PORTUGUESE (BRAZIL)
	{ SSTR_CONTROL					,	{	L"Control",			L"Contr�le",		L"Kontrolle",		L"Control",	 		L"Controllo",		L"Controlo",	 	L"Controle"	}	},
	{ SSTR_PATIENT					,	{	L"Patient",			L"Patient",			L"Patient",			L"Paciente",		L"Paziente",		L"Doente",	 		L"Paciente"	}	},
	{ SSTR_OTHER					,	{	L"Patient",			L"Patient",			L"Patient",			L"Paciente",		L"Paziente",		L"Doente",	 		L"Paciente"	}	},
	{ SSTR_SPECIES					,	{	L"Special",			L"Sp�cial",			L"Spezial",			L"Especial",		L"Speciale",		L"Especial",	 	L"Especial"	}	},
	
	{ SSTR_CANINES					,	{	L"Category 1", 		L"Cat�gorie 1", 	L"Kategorie 1", 	L"Categor�a 1",		L"Categoria 1",		L"Categoria 1",		L"Categoria 1"	}	},
	{ SSTR_FELINES					,	{	L"Category 2", 		L"Cat�gorie 2", 	L"Kategorie 2", 	L"Categor�a 2",		L"Categoria 2",		L"Categoria 2",		L"Categoria 2"	}	},
	{ SSTR_EQUINES					,	{	L"Category 3", 		L"Cat�gorie 3", 	L"Kategorie 3", 	L"Categor�a 3",		L"Categoria 3",		L"Categoria 3",		L"Categoria 3"	}	},
	{ SSTR_BOVINES					,	{	L"Category 4", 		L"Cat�gorie 4", 	L"Kategorie 4", 	L"Categor�a 4",		L"Categoria 4",		L"Categoria 4",		L"Categoria 4"	}	},
	{ SSTR_AVIANS					,	{	L"Category 5", 		L"Cat�gorie 5", 	L"Kategorie 5", 	L"Categor�a 5",		L"Categoria 5",		L"Categoria 5",		L"Categoria 5"	}	},
	{ SSTR_REPTILES_AMPHIBIANS		,	{	L"Category 6", 		L"Cat�gorie 6", 	L"Kategorie 6", 	L"Categor�a 6",		L"Categoria 6",		L"Categoria 6",		L"Categoria 6"	}	},
	{ SSTR_SMALL_MAMMALS			,	{	L"Category 7", 		L"Cat�gorie 7", 	L"Kategorie 7", 	L"Categor�a 7",		L"Categoria 7",		L"Categoria 7",		L"Categoria 7"	}	},
	{ SSTR_LARGE_MAMMALS			,	{	L"Category 8", 		L"Cat�gorie 8", 	L"Kategorie 8", 	L"Categor�a 8",		L"Categoria 8",		L"Categoria 8",		L"Categoria 8"	}	},

	// Canines
	{ SSTR_DOG						,	{	L"Demographic 1",	L"Elmts d�mogr. 1", L"Demographie 1", 	L"Demograf�a 1",	L"Demografia 1",	L"Demografia 1",	L"Dados demogr. 1"	}	},
	{ SSTR_COYOTE					,	{	L"Demographic 2",	L"Elmts d�mogr. 2", L"Demographie 2", 	L"Demograf�a 2",	L"Demografia 2",	L"Demografia 2",	L"Dados demogr. 2"	}	},
	{ SSTR_DINGO					,	{	L"Demographic 3", 	L"Elmts d�mogr. 3", L"Demographie 3", 	L"Demograf�a 3",	L"Demografia 3",	L"Demografia 3",	L"Dados demogr. 3"	}	},
	{ SSTR_FOX						,	{	L"Demographic 4", 	L"Elmts d�mogr. 4", L"Demographie 4", 	L"Demograf�a 4",	L"Demografia 4",	L"Demografia 4",	L"Dados demogr. 4"	}	},
	{ SSTR_JACKAL					,	{	L"Demographic 5", 	L"Elmts d�mogr. 5", L"Demographie 5", 	L"Demograf�a 5",	L"Demografia 5",	L"Demografia 5",	L"Dados demogr. 5"	}	},
	{ SSTR_WOLF						,	{	L"Demographic 6", 	L"Elmts d�mogr. 6", L"Demographie 6", 	L"Demograf�a 6",	L"Demografia 6",	L"Demografia 6",	L"Dados demogr. 6"	}	},

	// Felines
	{ SSTR_CAT						,	{	L"Demographic 7", 	L"Elmts d�mogr. 7", L"Demographie 7"	L"Demograf�a 7", 	L"Demografia 7",	L"Demografia 7",	L"Dados demogr. 7"	}	},
	{ SSTR_BOBCAT					,	{	L"Demographic 8", 	L"Elmts d�mogr. 8", L"Demographie 8",	L"Demograf�a 8", 	L"Demografia 8",	L"Demografia 8",	L"Dados demogr. 8"	}	},
	{ SSTR_CHEETAH					,	{	L"Demographic 9", 	L"Elmts d�mogr. 9", L"Demographie 9",	L"Demograf�a 9", 	L"Demografia 9",	L"Demografia 9",	L"Dados demogr. 9"	}	},
	{ SSTR_JAGUAR					,	{	L"Demographic 10", 	L"Elmts d�mogr. 10", L"Demographie 10",	L"Demograf�a 10",	L"Demografia 10",	L"Demografia 10",	L"Dados demogr. 10"	}	},
	{ SSTR_LEOPARD					,	{	L"Demographic 11", 	L"Elmts d�mogr. 11", L"Demographie 11",	L"Demograf�a 11",	L"Demografia 11",	L"Demografia 11",	L"Dados demogr. 11"	}	},
	{ SSTR_LION						,	{	L"Demographic 12", 	L"Elmts d�mogr. 12", L"Demographie 12",	L"Demograf�a 12",	L"Demografia 12",	L"Demografia 12",	L"Dados demogr. 12"	}	},
	{ SSTR_LYNX						,	{	L"Demographic 13", 	L"Elmts d�mogr. 13", L"Demographie 13",	L"Demograf�a 13",	L"Demografia 13",	L"Demografia 13",	L"Dados demogr. 13"	}	},
	{ SSTR_PANTHER					,	{	L"Demographic 14", 	L"Elmts d�mogr. 14", L"Demographie 14",	L"Demograf�a 14",	L"Demografia 14",	L"Demografia 14",	L"Dados demogr. 14"	}	},
	{ SSTR_PUMA						,	{	L"Demographic 15", 	L"Elmts d�mogr. 15", L"Demographie 15",	L"Demograf�a 15",	L"Demografia 15",	L"Demografia 15",	L"Dados demogr. 15"	}	},
	{ SSTR_TIGER					,	{	L"Demographic 16", 	L"Elmts d�mogr. 16", L"Demographie 16",	L"Demograf�a 16",	L"Demografia 16",	L"Demografia 16",	L"Dados demogr. 16"	}	},
	
	// Equines
	{ SSTR_HORSE					,	{	L"Demographic 17", 	L"Elmts d�mogr. 17", L"Demographie 17",	L"Demograf�a 17",	L"Demografia 17",	L"Demografia 17",	L"Dados demogr. 17"	}	},
	{ SSTR_DONKEY					,	{	L"Demographic 18", 	L"Elmts d�mogr. 18", L"Demographie 18",	L"Demograf�a 18",	L"Demografia 18",	L"Demografia 18",	L"Dados demogr. 18"	}	},
	{ SSTR_PONY						,	{	L"Demographic 19", 	L"Elmts d�mogr. 19", L"Demographie 19",	L"Demograf�a 19",	L"Demografia 19",	L"Demografia 19",	L"Dados demogr. 19"	}	},
	{ SSTR_ZEBRA					,	{	L"Demographic 20", 	L"Elmts d�mogr. 20", L"Demographie 20",	L"Demograf�a 20",	L"Demografia 20",	L"Demografia 20",	L"Dados demogr. 20"	}	},
	
	// Bovines
	{ SSTR_COW						,	{	L"Demographic 21", 	L"Elmts d�mogr. 21", L"Demographie 21",	L"Demograf�a 21",	L"Demografia 21",	L"Demografia 21",	L"Dados demogr. 21"	}	},
	{ SSTR_WATER_BUFFALO			,	{	L"Demographic 22", 	L"Elmts d�mogr. 22", L"Demographie 22",	L"Demograf�a 22",	L"Demografia 22",	L"Demografia 22",	L"Dados demogr. 22"	}	},
	{ SSTR_STEER					,	{	L"Demographic 23", 	L"Elmts d�mogr. 23", L"Demographie 23",	L"Demograf�a 23",	L"Demografia 23",	L"Demografia 23",	L"Dados demogr. 23"	}	},
	{ SSTR_OX						,	{	L"Demographic 24", 	L"Elmts d�mogr. 24", L"Demographie 24",	L"Demograf�a 24",	L"Demografia 24",	L"Demografia 24",	L"Dados demogr. 24"	}	},
	{ SSTR_BULL						,	{	L"Demographic 25", 	L"Elmts d�mogr. 25", L"Demographie 25",	L"Demograf�a 25",	L"Demografia 25",	L"Demografia 25",	L"Dados demogr. 25"	}	},
	{ SSTR_BUFFALO					,	{	L"Demographic 26", 	L"Elmts d�mogr. 26", L"Demographie 26",	L"Demograf�a 26",	L"Demografia 26",	L"Demografia 26",	L"Dados demogr. 26"	}	},
	{ SSTR_BISON					,	{	L"Demographic 27", 	L"Elmts d�mogr. 27", L"Demographie 27",	L"Demograf�a 27",	L"Demografia 27",	L"Demografia 27",	L"Dados demogr. 27"	}	},
	
	// Avians
	{ SSTR_BIRD						,	{	L"Demographic 28", 	L"Elmts d�mogr. 28", L"Demographie 28",	L"Demograf�a 28",	L"Demografia 28",	L"Demografia 28",	L"Dados demogr. 28"	}	},
	{ SSTR_AFRICAN_GRAY				,	{	L"Demographic 29", 	L"Elmts d�mogr. 29", L"Demographie 29",	L"Demograf�a 29",	L"Demografia 29",	L"Demografia 29",	L"Dados demogr. 29"	}	},
	{ SSTR_AMAZON					,	{	L"Demographic 30", 	L"Elmts d�mogr. 30", L"Demographie 30",	L"Demograf�a 30",	L"Demografia 30",	L"Demografia 30",	L"Dados demogr. 30"	}	},
	{ SSTR_CHICKEN					,	{	L"Demographic 31", 	L"Elmts d�mogr. 31", L"Demographie 31",	L"Demograf�a 31",	L"Demografia 31",	L"Demografia 31",	L"Dados demogr. 31"	}	},
	{ SSTR_COCKATIEL				,	{	L"Demographic 32", 	L"Elmts d�mogr. 32", L"Demographie 32",	L"Demograf�a 32",	L"Demografia 32",	L"Demografia 32",	L"Dados demogr. 32"	}	},
	{ SSTR_COCKATOO					,	{	L"Demographic 33", 	L"Elmts d�mogr. 33", L"Demographie 33",	L"Demograf�a 33",	L"Demografia 33",	L"Demografia 33",	L"Dados demogr. 33"	}	},
	{ SSTR_CONURE					,	{	L"Demographic 34", 	L"Elmts d�mogr. 34", L"Demographie 34",	L"Demograf�a 34",	L"Demografia 34",	L"Demografia 34",	L"Dados demogr. 34"	}	},
	{ SSTR_DOVE						,	{	L"Demographic 35", 	L"Elmts d�mogr. 35", L"Demographie 35",	L"Demograf�a 35",	L"Demografia 35",	L"Demografia 35",	L"Dados demogr. 35"	}	},
	{ SSTR_DUCK						,	{	L"Demographic 36", 	L"Elmts d�mogr. 36", L"Demographie 36",	L"Demograf�a 36",	L"Demografia 36",	L"Demografia 36",	L"Dados demogr. 36"	}	},
	{ SSTR_FLAMINGO					,	{	L"Demographic 37", 	L"Elmts d�mogr. 37", L"Demographie 37",	L"Demograf�a 37",	L"Demografia 37",	L"Demografia 37",	L"Dados demogr. 37"	}	},
	{ SSTR_GOOSE					,	{	L"Demographic 38", 	L"Elmts d�mogr. 38", L"Demographie 38",	L"Demograf�a 38",	L"Demografia 38",	L"Demografia 38",	L"Dados demogr. 38"	}	},
	{ SSTR_LOVEBIRD					,	{	L"Demographic 39", 	L"Elmts d�mogr. 39", L"Demographie 39",	L"Demograf�a 39",	L"Demografia 39",	L"Demografia 39",	L"Dados demogr. 39"	}	},
	{ SSTR_MACAW					,	{	L"Demographic 40", 	L"Elmts d�mogr. 40", L"Demographie 40",	L"Demograf�a 40",	L"Demografia 40",	L"Demografia 40",	L"Dados demogr. 40"	}	},
	{ SSTR_MYNAH					,	{	L"Demographic 41", 	L"Elmts d�mogr. 41", L"Demographie 41",	L"Demograf�a 41",	L"Demografia 41",	L"Demografia 41",	L"Dados demogr. 41"	}	},
	{ SSTR_OSTRICH					,	{	L"Demographic 42", 	L"Elmts d�mogr. 42", L"Demographie 42",	L"Demograf�a 42",	L"Demografia 42",	L"Demografia 42",	L"Dados demogr. 42"	}	},
	{ SSTR_PARAKEET					,	{	L"Demographic 43", 	L"Elmts d�mogr. 43", L"Demographie 43",	L"Demograf�a 43",	L"Demografia 43",	L"Demografia 43",	L"Dados demogr. 43"	}	},
	{ SSTR_PARROT					,	{	L"Demographic 44", 	L"Elmts d�mogr. 44", L"Demographie 44",	L"Demograf�a 44",	L"Demografia 44",	L"Demografia 44",	L"Dados demogr. 44"	}	},
	{ SSTR_TOUCAN					,	{	L"Demographic 45", 	L"Elmts d�mogr. 45", L"Demographie 45",	L"Demograf�a 45",	L"Demografia 45",	L"Demografia 45",	L"Dados demogr. 45"	}	},
	
	// Reptiles/amphibians
	{ SSTR_REPTILE					,	{	L"Demographic 46",	L"Elmts d�mogr. 46", L"Demographie 46",	L"Demograf�a 46",	L"Demografia 46",	L"Demografia 46",	L"Dados demogr. 46"	}	},
	{ SSTR_ALLIGATOR				,	{	L"Demographic 47",	L"Elmts d�mogr. 47", L"Demographie 47",	L"Demograf�a 47",	L"Demografia 47",	L"Demografia 47",	L"Dados demogr. 47"	}	},
	{ SSTR_AMPHIBIAN				,	{	L"Demographic 48",	L"Elmts d�mogr. 48", L"Demographie 48",	L"Demograf�a 48",	L"Demografia 48",	L"Demografia 48",	L"Dados demogr. 48"	}	},
	{ SSTR_BEARDED_DRAGON			,	{	L"Demographic 49",	L"Elmts d�mogr. 49", L"Demographie 49",	L"Demograf�a 49",	L"Demografia 49",	L"Demografia 49",	L"Dados demogr. 49"	}	},
	{ SSTR_BOA_CONSTRICTOR			,	{	L"Demographic 50",	L"Elmts d�mogr. 50", L"Demographie 50",	L"Demograf�a 50",	L"Demografia 50",	L"Demografia 50",	L"Dados demogr. 50"	}	},
	{ SSTR_CHAMELEON				,	{	L"Demographic 51",	L"Elmts d�mogr. 51", L"Demographie 51",	L"Demograf�a 51",	L"Demografia 51",	L"Demografia 51",	L"Dados demogr. 51"	}	},
	{ SSTR_CROCODILE				,	{	L"Demographic 52",	L"Elmts d�mogr. 52", L"Demographie 52",	L"Demograf�a 52",	L"Demografia 52",	L"Demografia 52",	L"Dados demogr. 52"	}	},
	{ SSTR_FROG						,	{	L"Demographic 53",	L"Elmts d�mogr. 53", L"Demographie 53",	L"Demograf�a 53",	L"Demografia 53",	L"Demografia 53",	L"Dados demogr. 53"	}	},
	{ SSTR_GECKO					,	{	L"Demographic 54",	L"Elmts d�mogr. 54", L"Demographie 54",	L"Demograf�a 54",	L"Demografia 54",	L"Demografia 54",	L"Dados demogr. 54"	}	},
	{ SSTR_IGUANA					,	{	L"Demographic 55",	L"Elmts d�mogr. 55", L"Demographie 55",	L"Demograf�a 55",	L"Demografia 55",	L"Demografia 55",	L"Dados demogr. 55"	}	},
	{ SSTR_LIZARD					,	{	L"Demographic 56",	L"Elmts d�mogr. 56", L"Demographie 56",	L"Demograf�a 56",	L"Demografia 56",	L"Demografia 56",	L"Dados demogr. 56"	}	},
	{ SSTR_MONITOR					,	{	L"Demographic 57",	L"Elmts d�mogr. 57", L"Demographie 57",	L"Demograf�a 57",	L"Demografia 57",	L"Demografia 57",	L"Dados demogr. 57"	}	},
	{ SSTR_PYTHON					,	{	L"Demographic 58",	L"Elmts d�mogr. 58", L"Demographie 58",	L"Demograf�a 58",	L"Demografia 58",	L"Demografia 58",	L"Dados demogr. 58"	}	},
	{ SSTR_SALAMANDER				,	{	L"Demographic 59",	L"Elmts d�mogr. 59", L"Demographie 59",	L"Demograf�a 59",	L"Demografia 59",	L"Demografia 59",	L"Dados demogr. 59"	}	},
	{ SSTR_SNAKE					,	{	L"Demographic 60",	L"Elmts d�mogr. 60", L"Demographie 60",	L"Demograf�a 60",	L"Demografia 60",	L"Demografia 60",	L"Dados demogr. 60"	}	},
	{ SSTR_TOAD						,	{	L"Demographic 61",	L"Elmts d�mogr. 61", L"Demographie 61",	L"Demograf�a 61",	L"Demografia 61",	L"Demografia 61",	L"Dados demogr. 61"	}	},
	{ SSTR_TORTOISE					,	{	L"Demographic 62",	L"Elmts d�mogr. 62", L"Demographie 62",	L"Demograf�a 62",	L"Demografia 62",	L"Demografia 62",	L"Dados demogr. 62"	}	},
	{ SSTR_TURTLE					,	{	L"Demographic 63",	L"Elmts d�mogr. 63", L"Demographie 63",	L"Demograf�a 63",	L"Demografia 63",	L"Demografia 63",	L"Dados demogr. 63"	}	},
	
	// Small mammals
	{ SSTR_SMALL_MAMMAL				,	{	L"Demographic 64",	L"Elmts d�mogr. 64", L"Demographie 64",	L"Demograf�a 64",	L"Demografia 64",	L"Demografia 64",	L"Dados demogr. 64"	}	},
	{ SSTR_BAT						,	{	L"Demographic 65",	L"Elmts d�mogr. 65", L"Demographie 65",	L"Demograf�a 65",	L"Demografia 65",	L"Demografia 65",	L"Dados demogr. 65"	}	},
	{ SSTR_CHINCHILLA				,	{	L"Demographic 66",	L"Elmts d�mogr. 66", L"Demographie 66",	L"Demograf�a 66",	L"Demografia 66",	L"Demografia 66",	L"Dados demogr. 66"	}	},
	{ SSTR_FERRET					,	{	L"Demographic 67",	L"Elmts d�mogr. 67", L"Demographie 67",	L"Demograf�a 67",	L"Demografia 67",	L"Demografia 67",	L"Dados demogr. 67"	}	},
	{ SSTR_GERBIL					,	{	L"Demographic 68",	L"Elmts d�mogr. 68", L"Demographie 68",	L"Demograf�a 68",	L"Demografia 68",	L"Demografia 68",	L"Dados demogr. 68"	}	},
	{ SSTR_GUINEA_PIG				,	{	L"Demographic 69",	L"Elmts d�mogr. 69", L"Demographie 69",	L"Demograf�a 69",	L"Demografia 69",	L"Demografia 69",	L"Dados demogr. 69"	}	},
	{ SSTR_HAMSTER					,	{	L"Demographic 70",	L"Elmts d�mogr. 70", L"Demographie 70",	L"Demograf�a 70",	L"Demografia 70",	L"Demografia 70",	L"Dados demogr. 70"	}	},
	{ SSTR_HEDGEHOG					,	{	L"Demographic 71",	L"Elmts d�mogr. 71", L"Demographie 71",	L"Demograf�a 71",	L"Demografia 71",	L"Demografia 71",	L"Dados demogr. 71"	}	},
	{ SSTR_LEMUR					,	{	L"Demographic 72",	L"Elmts d�mogr. 72", L"Demographie 72",	L"Demograf�a 72",	L"Demografia 72",	L"Demografia 72",	L"Dados demogr. 72"	}	},
	{ SSTR_MINK						,	{	L"Demographic 73",	L"Elmts d�mogr. 73", L"Demographie 73",	L"Demograf�a 73",	L"Demografia 73",	L"Demografia 73",	L"Dados demogr. 73"	}	},
	{ SSTR_MOUSE					,	{	L"Demographic 74",	L"Elmts d�mogr. 74", L"Demographie 74",	L"Demograf�a 74",	L"Demografia 74",	L"Demografia 74",	L"Dados demogr. 74"	}	},
	{ SSTR_OPOSSUM					,	{	L"Demographic 75",	L"Elmts d�mogr. 75", L"Demographie 75",	L"Demograf�a 75",	L"Demografia 75",	L"Demografia 75",	L"Dados demogr. 75"	}	},
	{ SSTR_PRAIRIE_DOG				,	{	L"Demographic 76",	L"Elmts d�mogr. 76", L"Demographie 76",	L"Demograf�a 76",	L"Demografia 76",	L"Demografia 76",	L"Dados demogr. 76"	}	},
	{ SSTR_RABBIT					,	{	L"Demographic 77",	L"Elmts d�mogr. 77", L"Demographie 77",	L"Demograf�a 77",	L"Demografia 77",	L"Demografia 77",	L"Dados demogr. 77"	}	},
	{ SSTR_RACCOON					,	{	L"Demographic 78",	L"Elmts d�mogr. 78", L"Demographie 78",	L"Demograf�a 78",	L"Demografia 78",	L"Demografia 78",	L"Dados demogr. 78"	}	},
	{ SSTR_RAT						,	{	L"Demographic 79",	L"Elmts d�mogr. 79", L"Demographie 79",	L"Demograf�a 79",	L"Demografia 79",	L"Demografia 79",	L"Dados demogr. 79"	}	},
	{ SSTR_SKUNK					,	{	L"Demographic 80",	L"Elmts d�mogr. 80", L"Demographie 80",	L"Demograf�a 80",	L"Demografia 80",	L"Demografia 80",	L"Dados demogr. 80"	}	},
	{ SSTR_SQUIRREL					,	{	L"Demographic 81",	L"Elmts d�mogr. 81", L"Demographie 81",	L"Demograf�a 81",	L"Demografia 81",	L"Demografia 81",	L"Dados demogr. 81"	}	},
	
	// Large mammals
	{ SSTR_LARGE_MAMMAL				,	{	L"Demographic 82",	L"Elmts d�mogr. 82", L"Demographie 82",	L"Demograf�a 82",	L"Demografia 82",	L"Demografia 82",	L"Dados demogr. 82"	}	},
	{ SSTR_ANTELOPE					,	{	L"Demographic 83",	L"Elmts d�mogr. 83", L"Demographie 83",	L"Demograf�a 83",	L"Demografia 83",	L"Demografia 83",	L"Dados demogr. 83"	}	},
	{ SSTR_APE						,	{	L"Demographic 84",	L"Elmts d�mogr. 84", L"Demographie 84",	L"Demograf�a 84",	L"Demografia 84",	L"Demografia 84",	L"Dados demogr. 84"	}	},
	{ SSTR_BABOON					,	{	L"Demographic 85",	L"Elmts d�mogr. 85", L"Demographie 85",	L"Demograf�a 85",	L"Demografia 85",	L"Demografia 85",	L"Dados demogr. 85"	}	},
	{ SSTR_BEAR						,	{	L"Demographic 86",	L"Elmts d�mogr. 86", L"Demographie 86",	L"Demograf�a 86",	L"Demografia 86",	L"Demografia 86",	L"Dados demogr. 86"	}	},
	{ SSTR_CAMEL					,	{	L"Demographic 87",	L"Elmts d�mogr. 87", L"Demographie 87",	L"Demograf�a 87",	L"Demografia 87",	L"Demografia 87",	L"Dados demogr. 87"	}	},
	{ SSTR_CHIMPANZEE				,	{	L"Demographic 88",	L"Elmts d�mogr. 88", L"Demographie 88",	L"Demograf�a 88",	L"Demografia 88",	L"Demografia 88",	L"Dados demogr. 88"	}	},
	{ SSTR_DEER						,	{	L"Demographic 89",	L"Elmts d�mogr. 89", L"Demographie 89",	L"Demograf�a 89",	L"Demografia 89",	L"Demografia 89",	L"Dados demogr. 89"	}	},
	{ SSTR_ELEPHANT					,	{	L"Demographic 90",	L"Elmts d�mogr. 90", L"Demographie 90",	L"Demograf�a 90",	L"Demografia 90",	L"Demografia 90",	L"Dados demogr. 90"	}	},
	{ SSTR_GIRAFFE					,	{	L"Demographic 91",	L"Elmts d�mogr. 91", L"Demographie 91",	L"Demograf�a 91",	L"Demografia 91",	L"Demografia 91",	L"Dados demogr. 91"	}	},
	{ SSTR_GOAT						,	{	L"Demographic 92",	L"Elmts d�mogr. 92", L"Demographie 92",	L"Demograf�a 92",	L"Demografia 92",	L"Demografia 92",	L"Dados demogr. 92"	}	},
	{ SSTR_KANGAROO					,	{	L"Demographic 93",	L"Elmts d�mogr. 93", L"Demographie 93",	L"Demograf�a 93",	L"Demografia 93",	L"Demografia 93",	L"Dados demogr. 93"	}	},
	{ SSTR_LLAMA					,	{	L"Demographic 94",	L"Elmts d�mogr. 94", L"Demographie 94",	L"Demograf�a 94",	L"Demografia 94",	L"Demografia 94",	L"Dados demogr. 94"	}	},
	{ SSTR_MONKEY					,	{	L"Demographic 95",	L"Elmts d�mogr. 95", L"Demographie 95",	L"Demograf�a 95",	L"Demografia 95",	L"Demografia 95",	L"Dados demogr. 95"	}	},
	{ SSTR_PIG						,	{	L"Demographic 96",	L"Elmts d�mogr. 96", L"Demographie 96",	L"Demograf�a 96",	L"Demografia 96",	L"Demografia 96",	L"Dados demogr. 96"	}	},
	{ SSTR_SEA_LION					,	{	L"Demographic 97",	L"Elmts d�mogr. 97", L"Demographie 97",	L"Demograf�a 97",	L"Demografia 97",	L"Demografia 97",	L"Dados demogr. 97"	}	},
	{ SSTR_SEAL						,	{	L"Demographic 98",	L"Elmts d�mogr. 98", L"Demographie 98",	L"Demograf�a 98",	L"Demografia 98",	L"Demografia 98",	L"Dados demogr. 98"	}	},
	{ SSTR_SHEEP					,	{	L"Demographic 99",	L"Elmts d�mogr. 99", L"Demographie 99",	L"Demograf�a 99",	L"Demografia 99",	L"Demografia 99",	L"Dados demogr. 99"	}	},
	
// Analytes									ENGLISH,  FRENCH,	  GERMAN,	  SPANISH,	  ITALIAN, 	  PORTUGUESE, PORTUGUESE (BRAZIL)
	{ ASTR_AG						,	{	L"AG"	, L"AG"		, L"AG"		, L"AG"		, L"GA"		, L"AG"		, L"AG"		}	},
	{ ASTR_ALB						,	{	L"ALB"	, L"ALB"	, L"ALB"	, L"ALB"	, L"ALB"	, L"ALB"	, L"ALB"	}	},
	{ ASTR_ALP						,	{	L"ALP"	, L"ALP"	, L"ALP"	, L"ALP"	, L"ALP"	, L"ALP"	, L"ALP"	}	},
	{ ASTR_ALT						,	{	L"ALT"	, L"ALT"	, L"ALT"	, L"ALT"	, L"ALT"	, L"ALT"	, L"ALT"	}	},
	{ ASTR_AMY						,	{	L"AMY"	, L"AMY"	, L"AMY"	, L"AMY"	, L"AMY"	, L"AMY"	, L"AMY"	}	},
	{ ASTR_AST						,	{	L"AST"	, L"AST"	, L"AST"	, L"AST"	, L"AST"	, L"AST"	, L"AST"	}	},
	{ ASTR_BA						,	{	L"BA"	, L"BA"		, L"BA"		, L"BA"		, L"BA"		, L"BA"		, L"BA"		}	},
	{ ASTR_BUN						,	{	L"BUN"	, L"BUN"	, L"BUN"	, L"BUN"	, L"BUN"	, L"BUN"	, L"BUN"	}	},
	{ ASTR_CA						,	{	L"CA"	, L"CA"		, L"CA"		, L"CA"		, L"CA"		, L"CA"		, L"CA"		}	},
	{ ASTR_CHOL						,	{	L"CHOL"	, L"CHOL"	, L"CHOL"	, L"COL"	, L"COL"	, L"CHOL"	, L"CHOL"	}	},
	{ ASTR_CHW						,	{	L"CHW"	, L"Di im"	, L"HHW"	, L"CHW"	, L"CHW"	, L"CHW"	, L"CHW"	}	},
	{ ASTR_CK						,	{	L"CK"	, L"CK"		, L"CK"		, L"CK"		, L"CK"		, L"CK"		, L"CK"		}	},
	{ ASTR_CL						,	{	L"CL-"	, L"CL-"	, L"CL-"	, L"CL-"	, L"CL-"	, L"CL-"	, L"CL-"	}	},
	{ ASTR_CRE						,	{	L"CRE"	, L"CRE"	, L"CRE"	, L"CRE"	, L"CRE"	, L"CRE"	, L"CRE"	}	},
	{ ASTR_CRP						,	{	L"CRP"	, L"CRP"	, L"CRP"	, L"CRP"	, L"CRP"	, L"CRP"	, L"CRP"	}	},
	{ ASTR_DBIL						,	{	L"DBIL"	, L"DBIL"	, L"DBIL"	, L"DBIL"	, L"DBIL"	, L"DBIL"	, L"DBIL"	}	},
	{ ASTR_EGFR						,	{	L"eGFR"	, L"eGFR"	, L"eGFR"	, L"eGFR"	, L"eGFR"	, L"eGFR"	, L"eGFR"	}	},
	{ ASTR_GGT						,	{	L"GGT"	, L"GGT"	, L"GGT"	, L"GGT"	, L"GGT"	, L"GGT"	, L"GGT"	}	},
	{ ASTR_GLOB						,	{	L"GLOB"	, L"GLOB"	, L"GLOB"	, L"GLOB"	, L"GLOB"	, L"GLOB"	, L"GLOB"	}	},
	{ ASTR_GLU						,	{	L"GLU"	, L"GLU"	, L"GLU"	, L"GLU"	, L"GLU"	, L"GLU"	, L"GLU"	}	},
	{ ASTR_HB						,	{	L"HB"	, L"HB"		, L"HB"		, L"HB"		, L"HB"		, L"HB"		, L"HB"		}	},
	{ ASTR_HCT						,	{	L"HCT"	, L"HCT"	, L"HCT"	, L"HCT"	, L"HCT"	, L"HCT"	, L"HCT"	}	},   
	{ ASTR_HDL						,	{	L"HDL"	, L"LHD"	, L"HDL"	, L"HDL"	, L"HDL"	, L"HDL"	, L"HDL"	}	},
	{ ASTR_IBIL						,	{	L"IBIL"	, L"IBIL"	, L"IBIL"	, L"IBIL"	, L"IBIL"	, L"IBIL"	, L"IBIL"	}	},
	{ ASTR_K						,	{	L"K+"	, L"K+"		, L"K+"		, L"K+"		, L"K+"		, L"K+"		, L"K+"		}	},
	{ ASTR_LAC						,	{	L"LAC"	, L"LAC"	, L"LAC"	, L"LAC"	, L"LAC"	, L"LAC"	, L"LAC"	}	},
	{ ASTR_LD						,	{	L"LD"	, L"LD"		, L"LD"		, L"LD"		, L"LD"		, L"LD"		, L"LD"		}	},
	{ ASTR_LDL						,	{	L"LDL"	, L"LBD"	, L"LDL"	, L"LDL"	, L"LDL"	, L"LDL"	, L"LDL"	}	},
	{ ASTR_MG						,	{	L"MG"	, L"MG"		, L"MG"		, L"MG"		, L"MG"		, L"MG"		, L"MG"		}	},
	{ ASTR_NA						,	{	L"NA+"	, L"NA+"	, L"NA+"	, L"NA+"	, L"NA+"	, L"NA+"	, L"NA+"	}	},
	{ ASTR_NHDLC					,	{	L"nHDLc", L"nHDLc"	, L"nHDLc"	, L"nHDLC"	, L"nHDLC"	, L"nHDLc"	, L"nHDLc"	}	},
	{ ASTR_PHB						,	{	L"PHB"	, L"PHB"	, L"PHB"	, L"PHB"	, L"PHB"	, L"PHB"	, L"PHB"	}	},
	{ ASTR_PHOS						,	{	L"PHOS"	, L"PHOS"	, L"PHOS"	, L"FOS"	, L"FOS"	, L"PHOS"	, L"PHOS"	}	},
	{ ASTR_T4						,	{	L"T4"	, L"T4"		, L"T4"		, L"T4"		, L"T4"		, L"T4"		, L"T4"		}	},
	{ ASTR_TBIL						,	{	L"TBIL"	, L"TBIL"	, L"TBIL"	, L"TBIL"	, L"TBIL"	, L"TBIL"	, L"TBIL"	}	},
	{ ASTR_TCH						,	{	L"TC/H"	, L"TC/H"	, L"TC/H"	, L"TC/H"	, L"TC/H"	, L"TC/H"	, L"TC/H"	}	},
	{ ASTR_TCO2						,	{	L"tCO2"	, L"tCO2"	, L"tCO2"	, L"tCO2"	, L"tCO2"	, L"tCO2"	, L"tCO2"	}	},
	{ ASTR_TP						,	{	L"TP"	, L"TP"		, L"TP"		, L"TP"		, L"TP"		, L"TP"		, L"TP"		}	},
	{ ASTR_TRIG						,	{	L"TRIG"	, L"TRIG"	, L"TRIG"	, L"TRIG"	, L"TRIG"	, L"TRIG"	, L"TRIG"	}	},
	{ ASTR_UA						,	{	L"UA"	, L"UA"		, L"UA"		, L"UA"		, L"UA"		, L"UA"		, L"UA"		}	},
	{ ASTR_VLDL						,	{	L"VLDL"	, L"LTBD"	, L"VLDL"	, L"PMBD"	, L"VLDL"	, L"VLDL"	, L"VLDL"	}	},

// Misc.									ENGLISH,		FRENCH,		GERMAN,			SPANISH,		ITALIAN,		PORTUGUESE,		PORTUGUESE (BRAZIL)
	{ STR_JAN						,	{	L"Jan",			L"Jan",		L"Jan",			L"Ene",			L"Gen",			L"Jan",			L"Jan"	}	},
	{ STR_FEB						,	{	L"Feb",			L"F�v",		L"Feb",			L"Feb",			L"Feb",			L"Fev",			L"Fev"	}	},
	{ STR_MAR						,	{	L"Mar",			L"Mar",		L"M�r",			L"Mar",			L"Mar",			L"Mar",			L"Mar"	}	},
	{ STR_APR						,	{	L"Apr",			L"Avr",		L"Apr",			L"Abr",			L"Apr",			L"Abr",			L"Abr"	}	},
	{ STR_MAY						,	{	L"May",			L"Mai",		L"Mai",			L"May",			L"Mag",			L"Mai",			L"Mai"	}	},
	{ STR_JUN						,	{	L"Jun",			L"Jun",		L"Jun",			L"Jun",			L"Giu",			L"Jun",			L"Jun"	}	},
	{ STR_JUL						,	{	L"Jul",			L"Jul",		L"Jul",			L"Jul",			L"Lug",			L"Jul",			L"Jul"	}	},
	{ STR_AUG						,	{	L"Aug",			L"Ao�",		L"Aug",			L"Ago",			L"Ago",			L"Ago",			L"Ago"	}	},
	{ STR_SEP						,	{	L"Sep",			L"Sep",		L"Sep",			L"Sep",			L"Set",			L"Set",			L"Set"	}	},
	{ STR_OCT						,	{	L"Oct",			L"Oct",		L"Okt",			L"Oct",			L"Ott",			L"Out",			L"Out"	}	},
	{ STR_NOV						,	{	L"Nov",			L"Nov",		L"Nov",			L"Nov",			L"Nov",			L"Nov",			L"Nov"	}	},
	{ STR_DEC						,	{	L"Dec",			L"D�c",		L"Dez",			L"Dic",			L"Dic",			L"Dez",			L"Dez"	}	},

	{ STR_SUNDAY					,	{	L"Sunday",		L"Dimanche",	L"Sonntag",		L"Domingo",		L"Domenica",	L"Domingo",		L"Domingo"	}	},
	{ STR_MONDAY					,	{	L"Monday",		L"Lundi",		L"Montag",		L"Lunes",		L"Luned�",		L"Segunda",		L"Segunda-feira"	}	},
	{ STR_TUESDAY					,	{	L"Tuesday",		L"Mardi",		L"Dienstag",	L"Martes",		L"Marted�",		L"Ter�a",		L"Ter�a-feira"	}	},
	{ STR_WEDNESDAY					,	{	L"Wednesday",	L"Mercredi",	L"Mittwoch",	L"Mi�rcoles",	L"Mercoled�",	L"Quarta",		L"Quarta-feira"	}	},
	{ STR_THURSDAY					,	{	L"Thursday",	L"Jeudi",		L"Donnerstag",	L"Jueves",		L"Gioved�",		L"Quinta",		L"Quinta-feira"	}	},
	{ STR_FRIDAY					,	{	L"Friday",		L"Vendredi",	L"Freitag",		L"Viernes",		L"Venerd�",		L"Sexta",		L"Sexta-feira"	}	},
	{ STR_SATURDAY					,	{	L"Saturday",	L"Samedi",		L"Samstag",		L"S�bado",		L"Sabato",		L"S�bado",		L"S�bado"	}	},
	{ STR_LANGUAGE                  ,	{	L"English",		L"Fran�ais",	L"Deutsch",		L"Espa�ol",     L"Italiano",	L"Portugu�s",	L"Portugu�s Brasil"}	},
	{ STR_NONE						,	{} }
};


WesternStringArr_t  VET_WESTERN_STRINGS[]  = {
// 											ENGLISH,					FRENCH,								GERMAN,								SPANISH,								ITALIAN, 								PORTUGUESE,									PORTUGUESE (BRAZIL)
	{ STR_PRODUCT_NAME				,	{	L"VetScan VS2",				L"VetScan VS2",						L"VetScan VS2",						L"VetScan VS2",							L"VetScan VS2",							L"VetScan VS2",								L"VetScan VS2"	}	},

// Top
	{ TSTR_ADD_SPECIES				,	{	L"Add Species",				L"Ajouter esp�ces",					L"Arten hinzuf�gen",				L"A�adir especie",						L"Aggiungi specie",						L"Adicionar esp�cies",						L"Adicionar esp�cie"	}	},
	{ TSTR_FACTORY_DEFAULT_SPECIES	,	{	L"Factory Default Species",	L"Param�tres par d�faut esp�ces",	L"Voreingestellte Standardarten",	L"Especies predeterminadas",			L"Specie predefinita di fabbrica",		L"Esp�cies predeterminadas de f�brica",		L"Esp�cie padr�o de f�brica"	}	},
	{ TSTR_INSERT_ROTOR				,	{	L"Insert Rotor",			L"Ins�rer rotor",					L"Rotor einsetzen",					L"Insertar rotor",						L"Inserisci rotore",					L"Inserir rotor",							L"Inserir rotor"	}	},
	{ TSTR_LOAD_ROTOR				,	{	L"Load Rotor",				L"Charger rotor",					L"Rotor laden",						L"Cargar rotor",						L"Carica rotore",						L"Carregar rotor",							L"Carregar rotor"	}	},
	{ TSTR_REMOVE_ROTOR				,	{	L"Remove Rotor",			L"Retirer rotor",					L"Rotor entfernen",					L"Retirar rotor",						L"Rimuovi rotore",						L"Remover rotor",							L"Remover rotor"	}	},
	{ TSTR_REMOVE_SPECIES			,	{	L"Remove Species",			L"Retirer esp�ces",					L"Arten entfernen",					L"Eliminar especie",					L"Rimuovi specie",						L"Remover esp�cies",						L"Remover esp�cies"	}	},
	{ TSTR_SELECT_SPECIES			,	{	L"Select Species",			L"S�lectionner esp�ces",			L"Arten ausw�hlen",					L"Seleccionar especie",					L"Seleziona specie",					L"Selecionar esp�cies",						L"Selecionar esp�cies"	}	},
	{ TSTR_SPECIES					,	{	L"Species",					L"Esp�ces",							L"Arten",							L"Especie",								L"Specie",								L"Esp�cies",								L"Esp�cies"	}	},
	{ TSTR_SPECIES_AVAILABLE		,	{	L"Species Available",		L"Esp�ces disponibles",				L"Verf�gbare Arten",				L"Especies disponibles",				L"Specie disponibili",					L"Esp�cies dispon�veis",					L"Esp�cies dispon�veis"	}	},

// Middle									ENGLISH,																		FRENCH,																							GERMAN,																										SPANISH,																							ITALIAN,																								PORTUGUESE,																									PORTUGUESE (BRAZIL)
	{ MSTR_ADD_SPECIES				,	{	L"Add Species",																	L"Ajouter esp�ces",																				L"Arten hinzuf�gen",																						L"A�adir especie",																					L"Aggiungi specie",																						L"Adicionar esp�cies",																						L"Adicionar esp�cie"	}	},
	{ MSTR_DETERMINING_ROTOR_TYPE	,	{	L"Determining rotor type",														L"D�term. type de rotor",																		L"Rotortyp festlegen",																						L"Determinando el tipo de rotor",																	L"Determinazione tipo del rotore",																		L"A determinar o tipo de rotor",																			L"Determinando o tipo de rotor"	}	},
	{ MSTR_FACTORY_DEFAULT_SPECIES	,	{	L"Factory Default Species",														L"Param. par d�faut esp�ces",																	L"Voreingestellte Arten",																					L"Especie predeterminada de f�brica",																L"Specie predefinita di fabbrica",																		L"Esp�cies predeterminadas de f�brica",																		L"Esp�cie padr�o de f�brica"	}	},
	{ MSTR_FEMALE					,	{	L"Female",																		L"Femelle",																						L"Weiblich",																								L"Hembra",																							L"Femmina",																								L"F�mea",																									L"Sexo feminino"	}	},
	{ MSTR_LAST_ROTOR				,	{	L"Last Rotor",																	L"Dernier rotor",																				L"Letzter Rotor",																							L"�ltimo rotor",																					L"Ultimo rotore",																						L"�ltimo rotor",																							L"�ltimo rotor"	}	},
	{ MSTR_LOAD_ROTOR_TO_ANALYZE	,	{	L"To analyze additional sample, load new rotor",								L"Pour analyser un �chantillon suppl�mentaire, charger un nouveau rotor",						L"Zur Analyse einer weiteren Probe neuen Rotor laden",														L"Para analizar otra muestra, cargue un rotor nuevo",												L"Per esaminare ulteriori campioni, caricare nuovo rotore",												L"Para analisar outra amostra, carregue novo rotor",														L"Para analisar uma amostra adicional, carregue um novo rotor"	}	},
	{ MSTR_MALE						,	{	L"Male",																		L"M�le",																						L"M�nnlich",																								L"Macho",																							L"Maschio",																								L"Macho",																									L"Sexo masculino"	}	},
	{ MSTR_NO_RESULTS				,	{	L"No saved results to view. Please run a rotor prior to recalling results.",	L"Aucun r�sultat enregistr� � afficher. Ex�cuter un rotor avant de rappeler les r�sultats.",	L"Keine gespeicherten Ergebnisse zur Anzeige. Vor Wiederaufrufen der Ergebnisse einen Rotor starten.",		L"No hay resultados guardados para ver. Utilice un rotor antes de buscar resultados.",				L"Nessun risultato salvato da visualizzare. Avviare un rotore prima di richiamare i risultati.",		L"No existem resultados guardados para visualizar. Utilize um rotor antes de recuperar resultados.",		L"Nenhum resultado salvo para visualiza��o. Execute um rotor antes de recuperar os resultados."	}	},
	{ MSTR_PLEASE_REMOVE_ROTOR		,	{	L"Please remove the rotor from the analyzer",									L"Retirer le rotor de l'analyseur",																L"Bitte den Rotor aus dem Analyseger�t entfernen",															L"Retire el rotor del analizador",																	L"Rimuovere il rotore dall'analizzatore",																L"Retire o rotor do analisador",																			L"Remova o rotor do analisador"	}	},
	{ MSTR_PRESS_YES_REF_RANGES		,	{	L"Press Yes to return the active species list to the factory default",			L"Appuyer sur Oui pour r�tablir la liste d'esp�ces actives par d�faut",							L"Dr�cken Sie Ja, um die Werkseinstellungen f�r die Liste der aktiven Arten wieder herzustellen",			L"Pulse S� para reestablecer los valores predeterminados de la lista de especies activas",			L"Premere S� per mandare l'elenco specie attive nelle impostazioni di fabbrica",						L"Prima Sim para reestabelecer a lista de esp�cies ativa para os valores predeterminados de f�brica",		L"Pressione Sim para retornar a lista de esp�cies ativas ao padr�o de f�brica"	}	},
	{ MSTR_REMOVE_SPECIES			,	{	L"Remove Species",																L"Retirer esp�ces",																				L"Arten entfernen",																							L"Eliminar especie",																				L"Rimuovi specie",																						L"Remover esp�cies",																						L"Remover esp�cies"	}	},
	{ MSTR_ROTORS_RUN				,	{	L"Rotors Run",																	L"Ex�cutions rotors",																			L"Rotoren in Betrieb",																						L"Usar rotores",																					L"Rotori in funzione",																					L"Utilizar rotores",																						L"Execu��o de rotores"	}	},
	{ MSTR_SPECIES					,	{	L"Species",																		L"Esp�ces",																						L"Arten",																									L"Esp.",																							L"Specie",																								L"Esp�cies",																								L"Esp�cies"	}	},

// Info text
	{ ISTR_LOAD_ROTOR_1				,	{	L"Remove reagent rotor from its foil pouch and transfer 100ul of whole blood, plasma or serum into the rotor.\n",				// English
											L"Retirer le rotor de r�actif de sa pochette alu et transf�rer 100 ul de sang entier, de plasma ou de s�rum dans le rotor.\n",	// French
											L"Reagenzrotor aus dem Plastikbeutel entfernen und 100ul Vollblut, Plasma oder Serum in den Rotor einf�llen.\n",					// German
											L"Retire el rotor de reactivo de su bolsa metalizada y transfiera 100 ul de sangre, plasma o suero al rotor.\n",					// Spanish
											L"Rimuovere il rotore reagente dalla custodia metallizzata e trasferire 100ul di sangue intero, plasma o siero nel rotore.\n",	// Italian
											L"Retire o rotor de reagente da sua bolsa metalizada e transfira 100 ul de sangue total, plasma ou soro para o rotor.\n",		// Portuguese
											L"Remova o rotor de reagente da bolsa de alum�nio e transfira 100 �l de sangue total, plasma ou soro para o rotor.\n"			// Portuguese (Brazil)
										} 
	},
	{ ISTR_LOAD_ROTOR_2				,	{	L"Insert the reagent rotor into the analyzer, close the drawer and follow the on-screen prompts.\n",								// English
											L"Ins�rer le rotor de r�actif dans l'analyseur, fermer le tiroir et suivre les instructions qui s'affichent � l'�cran.\n",		// French
											L"Reagenzrotor in das Analyseger�t einf�hren, Fach schlie�en und Bildschirmanweisungen befolgen.\n",								// German
											L"Inserte el rotor de reactivo en el analizador, cierre el caj�n y siga las instrucciones de pantalla.\n",						// Spanish
											L"Inserire il rotore reagente nell'analizzatore, chiudere il cassetto e seguire le istruzioni a schermo.\n",						// Italian
											L"Insira o rotor de reagente no analisador, feche a gaveta e siga as instru��es no ecr�.\n",										// Portuguese
											L"Insira o rotor de reagente no analisador, feche a gaveta e siga as instru��es na tela.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_SEND				,	{	L"Printer: Print a copy of a species reference range.\n\n"
											L"Internal Archive: Save a copy of all active reference ranges internal to the analyzer for future recall.\n\n"
											L"External Archive: Transmit an ASCII dump of a species reference range to an external system.\n",																		// English
											L"Imprimante : imprimer un exemplaire de fourchette de r�f�rence d'esp�ces.\n\n"
											L"Archive interne : enregistrer un exemplaire de toutes les fourchettes de r�f�rence actives internes � l'analyseur afin de pouvoir les r�cup�rer ult�rieurement.\n\n"
											L"Archive externe : transmettre un vidage de m�moire ASCII d'une fourchette de r�f�rence d'esp�ces � un syst�me externe.\n",												// French
											L"Drucker: Kopie eines Artenreferenzbereichs drucken.\n\n"
											L"Internes Archiv: Eine Kopie aller aktiven Referenzbereiche f�r sp�tere Verwendung intern im Analyseger�t speichern.\n\n"
											L"Externes Archiv: ASCII-Auszug eines Artenreferenzbereichs an ein externes System �bertragen.\n",																		// German
											L"Impresora: imprimir una copia de un intervalo de referencia de especies.\n\n"
											L"Archivo interno: guardar una copia de todos los intervalos de referencia activos internos para el analizador para usos futuros.\n\n"
											L"Archivo externo: hacer un volcado de datos ASCII de un intervalo de referencia de especies a un sistema externo.\n",													// Spanish
											L"Stampante: stampa una copia di un range di riferimento per specie.\n\n"
											L"Archivio interno: salva una copia di tutti i range di riferimento attivi interni all'analizzatore per utilizzo futuro.\n\n"
											L"Archivio esterno: trasmette una serie ASCII di range di riferimento per specie ad un sistema esterno.\n",																// Italian
											L"Impressora: Imprime uma c�pia de um intervalo de refer�ncia de esp�cies.\n\n"
											L"Ficheiro interno: Guardar uma c�pia de todos os intervalos de refer�ncia ativos internos no analisador para utiliza��o futura.\n\n"
											L"Ficheiro externo: Transmitir um dump de ASCII de um intervalo de refer�ncia de esp�cies para um sistema externo.\n",													// Portuguese
											L"Impressora: Imprima uma c�pia de um intervalo de refer�ncia de esp�cies.\n\n"
											L"Arquivo interno: Salve uma c�pia de todos os intervalos de refer�ncia ativos internos no analisador, para futura recupera��o.\n\n"
											L"Arquivo externo: Transmita um despejo ASCII de um intervalo de refer�ncia de esp�cies para um sistema externo.\n"														// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_ADD			,	{	L"Species n: The next available generic species label between 1 and 99.\n\n"
											L"Control n: The next available control label between 1 and 99.\n\n"
											L"Select: Provides a list of common species labels to select from.\n",																// English
											L"Esp�ce n : la prochaine �tiquette d'esp�ce g�n�rique disponible comprise entre 1 et 99.\n\n"
											L"Contr�le n : la prochaine �tiquette de contr�le disponible comprise entre 1 et 99.\n\n"
											L"S�lectionner : fournit une liste d'�tiquettes d'esp�ces communes � partir de laquelle la s�lection pourra �tre effectu�e.\n",		// French
											L"Arten n: Das n�chste verf�gbare generische Artenetikett zwischen 1 und 99.\n\n"
											L"Steuerung n: Das n�chste verf�gbare Steuerungsetikett zwischen 1 und 99.\n\n"
											L"Ausw�hlen: Stellt eine Liste �blicher Artenetiketten zur Auswahl.\n",																// German
											L"Especie n: la pr�xima etiqueta de especie gen�rica disponible entre 1 y 99.\n\n"
											L"Control n: la pr�xima etiqueta de control disponible entre 1 y 99.\n\n"
											L"Seleccionar: proporciona una lista para selecci�n de etiquetas de especies comunes.\n",											// Spanish
											L"Specie n: la successiva etichetta di specie generica disponibile tra 1 e 99.\n\n"
											L"Controllo n: la successiva etichetta di controllo disponibile tra 1 e 99.\n\n"
											L"Seleziona: fornisce un elenco delle etichette delle specie comuni tra cui scegliere.\n",											// Italian
											L"Esp�cies n: A pr�xima etiqueta de esp�cies gen�rica dispon�vel entre 1 e 99.\n\n"
											L"Controlo n: A pr�xima etiqueta de controlo dispon�vel entre 1 e 99.\n\n"
											L"Selecionar: Fornece uma lista para sele��o de etiquetas de esp�cies frequentes.\n",												// Portuguese
											L"N esp�cies: O pr�ximo r�tulo de esp�cie gen�rico dispon�vel entre 1 e 99.\n\n"
											L"N controle: O pr�ximo r�tulo de controle dispon�vel entre 1 e 99.\n\n"
											L"Selecionar: Fornece uma lista de r�tulos de esp�cies comuns para sele��o.\n"														// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_MODIFY		,	{	L"Analyte: Provides a list of all active reference ranges for a particular analyte.\n\n"
											L"Species: Provides a list of all analytes for all reference ranges.\n",																// English
											L"Substance � analyser : fournit une liste de toutes les fourchettes de r�f�rence actives pour une substance � analyser donn�e.\n\n"
											L"Esp�ce : fournit une liste de toutes les substances � analyser pour toutes les fourchettes de r�f�rence.\n",						// French
											L"Analyt: Anzeige einer Liste aller aktiven Referenzbereiche f�r ein bestimmtes Analyt.\n\n"
											L"Arten: Anzeige einer Liste aller Analyte f�r alle Referenzbereiche.\n",															// German
											L"Analito: proporciona una lista de todos los intervalos de referencia activos para un analito particular.\n\n"
											L"Especie: proporciona una lista de todos los analitos para todos los intervalos de referencia.\n",									// Spanish
											L"Analita: fornisce un elenco di tutti i range di riferimento attivi per un particolare analita.\n\n"
											L"Specie: fornisce un elenco di tutti gli analiti per ogni range di riferimento.\n",													// Italian
											L"Analito: Fornece uma lista de todos os intervalos de referencia ativos para um analito espec�fico.\n\n"
											L"Esp�cies: Fornece uma lista de todos os analitos para todos os intervalos de refer�ncia.\n",										// Portuguese
											L"Analito: Fornece uma lista de todos os intervalos de refer�ncia ativos para um determinado analito.\n\n"
											L"Esp�cies: Fornece uma lista de todos os analitos para todos os intervalos de refer�ncia.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_REMOVE		,	{	L"Removing a species from the sample type list will remove the item from the list of species shown when running a rotor, and any values entered will be permanently lost.\n",																	// English
											L"La suppression d'une esp�ce dans la liste de type d'�chantillons a pour effet de supprimer cet �l�ment dans la liste d'esp�ces affich�es lors de l'ex�cution d'un rotor, et toutes les valeurs saisies seront irr�m�diablement perdues.\n",	// French
											L"Die Entfernung einer Art aus der Probentypliste entfernt das Objekt aus der Artenliste, die bei laufendem Rotor angezeigt wird, und s�mtliche eingegebene Werte gehen dauerhaft verloren.\n",													// German
											L"Al eliminar una especie de la lista del tipo de muestra se eliminar� la entrada de la lista de especies mostrada al usar el rotor, y cualquier valor introducido se perder� permanentemente.\n",												// Spanish
											L"Rimuovendo una specie dall'elenco dei tipi si rimuover� quella voce dall'elenco di specie proposto quando un rotore � in funzione e tutti i valori immessi saranno persi in modo permanente.\n",												// Italian
											L"Ao eliminar uma esp�cie da lista de tipo de amostra ser� eliminado o item da lista de esp�cies exibida quando se utiliza um rotor, e qualquer valor introduzido ser� permanentemente perdido.\n",												// Portuguese
											L"A remo��o de uma esp�cie da lista de tipos de amostra remover� o item da lista de esp�cie mostrada durante a execu��o de um rotor e quaisquer valores inseridos ser�o permanentemente perdidos.\n"												// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_ENZYME_UNITS			,	{	L"Selecting a unit on this screen will change the units for ALT, ALP, AMY, AST, CK and GGT  simultaneously to the same units.\n",					// English
											L"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � ALT, ALP, AMY, AST, CK et GGT.\n",												// French
											L"Mit der Auswahl einer Einheit in diesem Men� werden automatisch die Einheiten f�r ALT, ALP, AMY, AST, CK und GGT auf diese Einheit umgestellt.\n",	// German
											L"Al seleccionar una unidad en esta pantalla, se cambiar� ALT, ALP, AMY, AST, CK y GGT simult�neamente a la misma unidad.\n",						// Spanish
											L"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per ALT, ALP, AMY, AST, CK e GGT nelle stesse unit�.\n",			// Italian
											L"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades ALT, ALP, AMY, AST, CK e GGT para as mesmas unidades.\n",		// Portuguese
											L"A sele��o de uma unidade nessa tela mudar� as unidades para ALT, ALP, AMY, AST, CK e GGT simultaneamente para as mesmas unidades.\n"				// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_LIPID_UNITS			,	{	L"Selecting a unit on this screen will change the units for CHOL.\n",						// English
											L"L'unit� s�lectionn�e dans cet �cran sera appliqu�e � CHOL.\n",								// French
											L"Mit der Auswahl einer Einheit in diesem Men� wird die Einheit f�r CHOL umgestellt.\n",		// German
											L"Al seleccionar una unidad en esta pantalla, se cambiar� la unidad de COL.\n",				// Spanish
											L"La selezione di un'unit� in questa schermata modifica le unit� per COL.\n",				// Italian
											L"Ao selecionar uma unidade neste ecr�, ser�o alteradas as unidades CHOL.\n",				// Portuguese
											L"A sele��o de uma unidade nessa tela mudar� as unidades para CHOL.\n"						// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_PROTEIN_UNITS		,	{	L"Selecting a unit on this screen will change the units for ALB, GLOB and TP simultaneously to the same units.\n",									// English
											L"L'unit� s�lectionn�e dans cet �cran sera appliqu�e simultan�ment � ALB, GLOB et TP.\n",															// French
											L"Mit der Auswahl einer Einheit in diesem Men� werden automatischdie Einheiten f�r ALB, GLOB und TP auf diese Einheit umgestellt.\n",				// German
											L"Al seleccionar una unidad en esta pantalla, se cambiar� ALB, GLOB y TP simult�neamente a la misma unidad.\n",										// Spanish
											L"La selezione di un'unit� in questa schermata modifica simultaneamente le unit� per ALB e GLOB nelle stesse unit�.\n",								// Italian
											L"Ao selecionar uma unidade neste ecr�, ser�o simultaneamente alteradas as unidades ALB, GLOB e TP para as mesmas unidades.\n",						// Portuguese
											L"A sele��o de uma unidade nessa mudar� as unidades para ALB, GLOB e TP simultaneamente para as mesmas unidades.\n"									// Portuguese (Brazil)
										} 
	},
	
// Error screens							ENGLISH,							FRENCH,									GERMAN,								SPANISH,											ITALIAN, 									PORTUGUESE,								PORTUGUESE (BRAZIL)
	{ ESTR_ILLEGAL_ROTOR_TYPE		,	{	L"Illegal Rotor Type",				L"Type rotor non autoris�",				L"Falscher Rotortyp",				L"Tipo de rotor ilegal",							L"Tipo rotore illegale",					L"Tipo de rotor inv�lido",				L"Tipo de rotor ilegal"	}	},
	{ ESTR_NEW_ROTOR_TYPE			,	{	L"New Rotor Type",					L"Nouveau type de rotor",				L"Neuer Rotortyp",					L"Nuevo tipo de rotor",								L"Nuovo tipo rotore",						L"Tipo novo de rotor",					L"Novo tipo de rotor"	}	},
	{ ESTR_PLEASE_REMOVE_ROTOR		,	{	L"Please Remove Rotor",				L"Retirer le rotor",					L"Bitte Rotor entfernen",			L"Retire el rotor",									L"Si prega di rimuovere il rotore",			L"Retirar rotor",						L"Remova o rotor"	}	},
	{ ESTR_ROTOR_DATE_ILLEGAL		,	{	L"Rotor Date Illegal",				L"Date rotor non autoris�e",			L"Falsches Rotorendatum",			L"Fecha de rotor ilegal",							L"Data rotore illegale",					L"Data do rotor inv�lida",				L"Data do rotor ilegal"	}	},
	{ ESTR_ROTOR_DATE_IMPROPER		,	{	L"Rotor Date Improper",				L"Date rotor incorrecte",				L"Unzul�ssiges Rotorendatum",		L"Fecha rotor no adecuada",							L"Data rotore impropria",					L"Data do rotor inadequada",			L"Data do rotor impr�pria"	}	},
	{ ESTR_ROTOR_EXPIRED			,	{	L"Rotor Expired",					L"Expiration rotor",					L"Abgelaufener Rotor",				L"Rotor caducado",									L"Rotore scaduto",							L"Rotor expirado",						L"Rotor expirado"	}	},
	{ ESTR_UPDATED_ROTOR_TYPE		,	{	L"Updated Rotor Type",				L"Type de rotor actualis�",				L"Aktualisierter Rotortyp",			L"Tipo de rotor actualizado",						L"Tipo rotore aggiornato",					L"Tipo de rotor atualizado",			L"Tipo de rotor atualizado"	}	},

// Warning screens							ENGLISH,							FRENCH,									GERMAN,								SPANISH,											ITALIAN, 									PORTUGUESE,								PORTUGUESE (BRAZIL)
	{ ESTR_ROTOR_DETECTED			,	{	L"Rotor detected during warming",	L"Rotor d�tect� en cours de chauffage",	L"Rotor beim Aufw�rmen vorgefunden",	L"Rotor detectado durante el calentamiento inicial",	L"Rotore individuato durante riscaldamento",	L"Rotor detetado durante o aquecimento",	L"Rotor detectado durante o aquecimento"	}	},

// Error info text
	{ EISTR_AMBIENT_TEMP			,	{	L"The temperature outside the analyzer is either too hot or too cold to run a rotor. Contact Technical Service for assistance.\n",															// English
											L"La temp�rature � l'ext�rieur de l'analyseur est trop �lev�e ou trop faible pour permettre le fonctionnement d'un rotor. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Die Temperatur au�erhalb des Analyseger�ts ist f�r den Rotorenbetrieb zu hei� oder zu kalt. Wenden Sie sich an den technischen Service.\n",												// German
											L"La temperatura fuera del analizador es demasiado alta o demasiado baja para usar el rotor. P�ngase en contacto con el servicio t�cnico.\n",												// Spanish
											L"La temperatura esterna all'analizzatore � troppo alta o troppo bassa per permettere il funzionamento di un rotore. Per assistenza contattare il servizio tecnico.\n",						// Italian
											L"A temperatura fora do analisador � demasiado alta ou demasiado baixa para executar um rotor. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",							// Portuguese
											L"A temperatura externa do analisador est� muito alta ou muito baixa para executar um rotor. Entre em contato com o Servi�o T�cnico para obter assist�ncia.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_ANALYZER_DATE			,	{	L"Date in analyzer is earlier than that of the rotor being run. Check dates in analyzer and rerun with new rotor.\n",											// English
											L"La date de l'analyseur est ant�rieure � celle du rotor en cours d'utilisation. V�rifier les dates sur l'analyseur et r�ex�cuter le nouveau rotor.\n",			// French
											L"Das Datum im Analyseger�t liegt vor dem des Rotorenbetriebs. Pr�fen Sie das Datum im Analyseger�t und nehmen Sie den Betrieb mit einem neuen Rotor auf.\n",	// German
											L"La fecha del analizador es anterior a la fecha del rotor que se va a usar. Compruebe las fechas del analizador y vuelva a usar el nuevo rotor.\n",				// Spanish
											L"La data dell'analizzatore � precedente a quella di attivazione del rotore. Controllare le date dell'analizzatore e riavviare con un nuovo rotore.\n",			// Italian
											L"A data do analisador � anterior � data do rotor que foi iniciado. Verifique as datas do analisador e reinicie com um novo rotor.\n",							// Portuguese
											L"A data no analisador � anterior � do rotor em execu��o. Verifique as datas no analisador e execute novamente com um novo rotor.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE					,	{	L"The analyzer was unable to read the rotor's barcode. If symptom continues, contact Technical Service.\n",														// English
											L"L'analyseur n'a pas pu lire le code � barres du rotor. En cas de persistance des sympt�mes, contacter le service technique.\n",								// French
											L"Das Analyseger�t konnte den Barcode des Rotors nicht lesen. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",						// German
											L"El analizador no pudo leer el c�digo de barras del rotor. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",			// Spanish
											L"L'analizzatore non � riuscito a leggere il codice a barre del rotore. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"O analisador n�o consegue ler o c�digo de barras do rotor. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",								// Portuguese
											L"O analisador n�o foi capaz de ler o c�digo de barras do rotor. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE_FORMAT			,	{	L"The analyzer did not recognize the rotor. Contact Technical Service for further assistance.\n",						// English
											L"L'analyseur n'a pas reconnu le rotor. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											L"Das Analyseger�t konnte den Rotor nicht erkennen. Wenden Sie sich an den technischen Service.\n",						// German
											L"El analizador no ha reconocido el rotor. P�ngase en contacto con el servicio t�cnico.\n",								// Spanish
											L"L'analizzatore non ha riconosciuto il rotore. Per ulteriore assistenza contattare il servizio tecnico.\n",				// Italian
											L"O analisador n�o consegue reconhecer o rotor. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",	// Portuguese
											L"O analisador n�o reconheceu o rotor. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MISSING			,	{	L"The analyzer did not detect all the chemistry beads in the rotor. Rerun sample using new rotor.\n",											// English
											L"L'analyseur n'a pas d�tect� toutes les billes chimiques dans le rotor. R�ex�cuter l'�chantillon � l'aide d'un nouveau rotor.\n",				// French
											L"Das Analyseger�t konnte nicht alle chemischen Beads in dem Rotor erkennen. Wiederholen Sie die Probe mit einem neuen Rotor.\n",				// German
											L"El analizador no detect� todas las cuentas qu�micas en el rotor. Vuelva a utilizar la muestra usando un nuevo rotor.\n",						// Spanish
											L"L'analizzatore non ha individuato tutti i granuli chimici nel rotore. Riavviare il campione con un nuovo rotore.\n",							// Italian
											L"O analisador n�o detetou todas as esferas de subst�ncias qu�micas no rotor. Executar novamente a amostra utilizando um rotor novo.\n",			// Portuguese
											L"O analisador n�o detectou todas as esferas de subst�ncia qu�mica no rotor. Execute novamente a amostra usando o novo rotor.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MIX				,	{	L"The analyzer will not be able to determine the exposure status if the QC beads have not mixed correctly. Rerun with a new rotor.\n",									// English
											L"L'analyseur ne parviendra pas � d�terminer l'�tat d'exposition si les billes QC ne sont pas correctement m�lang�es. R�ex�cuter avec un nouveau rotor.\n",				// French
											L"Das Analyseger�t kann den Status der Einwirkung nicht feststellen, wenn die QC-Beads nicht ordnungsgem�� vermischt wurden. Mit einem neuen Rotor wiederholen.\n",		// German
											L"El analizador no ser� capaz de determinar el estado de exposici�n si las cuentas QC no se mezclan correctamente. Debe usarse con un nuevo rotor.\n",					// Spanish
											L"L'analizzatore non potr� determinare lo stato di esposizione se i granuli di QC non sono stati mescolati correttamente. Riavviare con un nuovo rotore.\n",				// Italian
											L"O analisador n�o conseguir� determinar o estado de exposi��o se as esferas de CQ n�o forem misturadas adequadamente. Executar novamente utilizando um rotor novo.\n",	// Portuguese
											L"O analisador n�o poder� determinar o status de exposi��o se as esferas de QC n�o tiverem sido misturadas corretamente. Execute novamente com um novo rotor.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_CUVETTE_MARK			,	{	L"The analyzer was unable to detect the rotor. If symptom continues, contact Technical Service.\n",												// English
											L"L'analyseur n'est pas parvenu � d�tecter le rotor. En cas de persistance des sympt�mes, contacter le service technique.\n",					// French
											L"Das Analyseger�t konnte den Rotor nicht finden. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",					// German
											L"El analizador no fue capaz de detectar el rotor. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"L'analizzatore non � riuscito a individuare il rotore. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"O analisador n�o consegue detetar o rotor. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",								// Portuguese
											L"O analisador n�o foi capaz de detectar o rotor. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_EXPIRED_ROTOR			,	{	L"The rotor has expired and should not be used. Contact Technical Service for further details.\n",												// English
											L"La date de p�remption du rotor est d�pass�e. Ne pas l'utiliser. Pour obtenir plus d'informations, contacter le service technique.\n",			// French
											L"Der Rotor ist abgelaufen und sollte ersetzt werden. Wenden Sie sich an den technischen Service.\n",											// German
											L"El rotor ha caducado y no deber�a utilizarse. P�ngase en contacto con el servicio t�cnico.\n",													// Spanish
											L"Il rotore � scaduto e non dovrebbe essere usato. Per ulteriore dettagli contattare il servizio tecnico.\n",									// Italian
											L"O rotor expirou e n�o deveria ser utilizado. Para obter mais informa��es, contacte o servi�o de Assist�ncia T�cnica.\n",						// Portuguese
											L"O rotor expirou e n�o deve ser usado. Entre em contato com o Servi�o T�cnico para obter detalhes adicionais.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_DILUENT	,	{	L"Insufficient reagents exist in the rotor to complete the run. Rerun sample with new rotor.\n",													// English
											L"La quantit� de r�actifs dans le rotor est insuffisante pour ex�cuter la proc�dure. R�ex�cuter l'�chantillon � l'aide du nouveau rotor.\n",		// French
											L"In dem Rotor befinden sich nicht ausreichend Reagenzien, um den Vorgang abzuschlie�en. Wiederholen Sie die Probe mit einem neuen Rotor.\n",	// German
											L"No hay suficientes reactivos en el rotor para completar su uso. Vuelva a utilizar la muestra con un nuevo rotor.\n",							// Spanish
											L"Nel rotore non vi sono sufficienti reagenti per completare l'operazione. Riavviare il campione con un nuovo rotore.\n",						// Italian
											L"N�o existem reagentes suficientes no rotor para concluir a execu��o. Executar novamente a amostra utilizando um rotor novo.\n",				// Portuguese
											L"Existem reagentes insuficientes no rotor para concluir a execu��o. Execute novamente a amostra com o novo rotor.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_SAMPLE		,	{	L"The rotor has not received enough sample to function properly. Rerun using a new rotor.\n",													// English
											L"Le rotor n'a pas re�u suffisamment d'�chantillon pour fonctionner correctement. R�ex�cuter avec un nouveau rotor.\n",							// French
											L"Der Rotor enth�lt keine ausreichende Probenmenge f�r eine ordnungsgem��e Funktion. Mit einem neuen Rotor wiederholen.\n",						// German
											L"El rotor no ha recibido suficiente muestra para funcionar correctamente. �selo con un nuevo rotor.\n",											// Spanish
											L"Il rotore non ha ricevuto campione sufficiente per funzionare nel modo corretto. Riavviare utilizzando un nuovo rotore.\n",					// Italian
											L"O rotor n�o recebeu amostra suficiente para funcionar adequadamente. Executar novamente utilizando um rotor novo.\n",							// Portuguese
											L"O rotor n�o recebeu amostra suficiente para funcionar corretamente. Execute novamente usando um novo rotor.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_IQC						,	{	L"The analyzer Intelligent Quality Control (iQC) reagent dye beads differ too much from the expected values. Rerun using a new rotor.\n",												// English
											L"Les billes de teinture du r�actif Intelligent Quality Control (iQC) divergent trop des valeurs pr�vues. R�ex�cuter avec un nouveau rotor.\n",											// French
											L"Die Intelligent Quality Control (iQC) Reagenz-Farbstoff-Beads des Analyseger�ts weichen zu stark von den erwarteten Werten ab. Mit einem neuen Rotor wiederholen.\n",					// German
											L"Las cuentas de tinte de reactivo Intelligent Quality Control (iQC) del analizador son muy distintas a los valores esperados. �selo con un nuevo rotor.\n",								// Spanish
											L"I granuli di colore reagente Intelligent Quality Control (iQC) dell'analizzatore sono troppo diversi dai valori previsti. Riavviare utilizzando un nuovo rotore.\n",					// Italian
											L"As esferas de reagente corante do Controlo de qualidade inteligente (iQC) do analisador s�o muito diferentes dos valores esperados. Executar novamente utilizando um rotor novo.\n",	// Portuguese
											L"As esferas do corante reagente de Controle de qualidade inteligente (iQC) do analisador diferem muito dos valores esperados. Execute novamente usando um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_REAGENT_DISTRIBUTION	,	{	L"Reagent beads have not dissolved as expected. Rerun sample with a new rotor.\n",											// English
											L"Les billes de r�actif ne se sont pas dissoutes comme pr�vu. R�ex�cuter l'�chantillon � l'aide d'un nouveau rotor.\n",		// French
											L"Reagenz-Beads wurden nicht erwartungsgem�� aufgel�st. Wiederholen Sie die Probe mit einem neuen Rotor.\n",					// German
											L"Las cuentas de reactivo no se han disuelto como se esperaba. Vuelva a utilizar la muestra con un nuevo rotor.\n",			// Spanish
											L"I granuli di reagente non si sono dissolti come previsto. Riavviare il campione con un nuovo rotore.\n",					// Italian
											L"As esferas de reagente n�o se dissolveram conforme esperado. Executar novamente a amostra utilizando um rotor novo.\n",	// Portuguese
											L"As esferas de reagente n�o se dissolveram conforme esperado. Execute novamente a amostra com um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_ROTOR_DETECTED			,	{	L"A rotor may be in the analyzer and may prevent the analyzer from starting normally. Press Continue to remove the rotor.\n",								// English
											L"Il peut y avoir dans l'analyseur un rotor susceptible de l'emp�cher de d�marrer normalement. Appuyer sur Continuer pour retirer le rotor.\n",				// French
											L"Ein Rotor kann sich in dem Analyseger�t befinden und den Startvorgang des Analyseger�ts beeintr�chtigen. Zur Entfernung des Rotors dr�cken Sie Weiter.\n",	// German
											L"Puede que haya un rotor en el analizador, lo que impide que el analizador se inicie correctamente. Pulse en Continuar para retirar el rotor.\n",			// Spanish
											L"Nell'analizzatore potrebbe essere rimasto un rotore che impedisce un'attivazione normale dell'analizzatore. Premere Continua per rimuovere il rotore.\n", 	// Italian
											L"Pode estar um rotor no analisador, o que pode impedir que o analisador inicie corretamente. Prima Continuar para remover o rotor.\n",						// Portuguese
											L"Um rotor pode estar no analisador e impedir que o analisador inicie normalmente. Pressione Continuar para remover o rotor.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_RQC						,	{	L"The Reagent QC beads recovered below the limit. The rotor may not have been stored correctly. Rerun with a new rotor.\n",													// English
											L"La r�cup�ration des billes de r�actif QC est inf�rieure � la limite. Il se peut que le rotor n'ait pas �t� rang� correctement. R�ex�cuter avec un nouveau rotor.\n",		// French
											L"Die wiederhergestellten Reagenz-QC-Beads liegen unter dem Grenzwert. Der Rotor wurde m�glicherweise nicht ordnungsgem�� gelagert. Mit einem neuen Rotor wiederholen.\n",	// German
											L"Cuentas QC de reactivo recuperadas por debajo del l�mite. Puede que el rotor no se haya guardado correctamente. Debe usarse con un nuevo rotor.\n",						// Spanish
											L"I granuli QC di reagente si sono ristabiliti sotto al limite. Il rotore potrebbe non essere stato conservato correttamente. Riavviare con un nuovo rotore.\n",			   	// Italian
											L"As esferas de reagente de CQ foram recuperadas abaixo do limite. O rotor pode n�o ter sido guardado corretamente. Executar novamente utilizando um rotor novo.\n",			// Portuguese
											L"As esferas de Reagente de QC se recuperaram abaixo do limite. O rotor pode n�o ter sido armazenado corretamente. Execute novamente com um novo rotor.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_BLANK			,	{	L"The sample blank bead appears to be turbid. This can be due to sample quality. Confirm sample integrity and rerun with a new rotor.\n",														// English
											L"La bille vierge de l'�chantillon semble trouble, ce qui peut venir de la qualit� de l'�chantillon. V�rifier l'int�grit� de l'�chantillon et r�ex�cuter avec un nouveau rotor.\n",				// French
											L"Die Leer-Beadprobe ist tr�be. Dies kann auf die Probenqualit�t zur�ckgef�hrt werden. Best�tigen Sie die Unversehrtheit der Probe und wiederholen Sie den Vorgang mit einem neuen Rotor.\n",	// German
											L"La cuenta virgen de la muestra parece estar turbia. Esto puede deberse a la calidad de la muestra. Confirme la integridad de la muestra y vuelva a utilizar el nuevo rotor.\n",				// Spanish
											L"I granuli campione vuoti sembrano torbidi. Ci� pu� essere causato dalla qualit� del campione. Confermare l'integrit� del campione e riavviare con un nuovo rotore.\n",							// Italian
											L"A esfera de amostra de branco parece estar turva. Isto pode ser devido � qualidade da amostra. Confirme a integridade da amostra e execute novamente utilizando um rotor novo.\n",				// Portuguese
											L"A esfera em branco da amostra parece estar turva. Isso pode ser decorrente da qualidade da amostra. Confirme a integridade da amostra e execute novamente com um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_DISTRIBUTION		,	{	L"Sample has not reached the chemistries. Rerun with a new rotor.\n",									// English
											L"L'�chantillon n'est pas parvenu jusqu'aux produits chimiques. R�ex�cuter avec un nouveau rotor.\n",	// French
											L"Die Probe hat nicht die Chemie erreicht. Mit einem neuen Rotor wiederholen.\n",						// German
											L"La muestra no ha alcanzado los compuestos qu�micos. Debe usarse con un nuevo rotor.\n",				// Spanish
											L"Il campione non ha raggiunto le sostanze chimiche. Riavviare con un nuovo rotore.\n",					// Italian
											L"A amostra n�o atingiu as subst�ncias qu�micas. Executar novamente utilizando um rotor novo.\n",		// Portuguese
											L"A amostra n�o atingiu as subst�ncias qu�micas. Execute novamente com um novo rotor.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_MIX				,	{	L"The sample has not mixed with reagent materials sufficiently. Rerun with a new rotor.\n",										// English
											L"L'�chantillon ne s'est pas suffisamment m�lang� avec les mati�res de r�actifs. R�ex�cuter avec un nouveau rotor.\n",			// French
											L"Die Probe hat sich nicht ausreichend mit Reagenzmaterialien vermischt. Mit einem neuen Rotor wiederholen.\n",					// German
											L"La muestra no se ha mezclado suficientemente con los materiales reactivos. Debe usarse con un nuevo rotor.\n",					// Spanish
											L"Il campione non si � mescolato a sufficienza con le sostanze reagenti. Riavviare con un nuovo rotore.\n",						// Italian
											L"A amostra n�o se misturou suficientemente com os materiais dos reagentes. Executar novamente utilizando um rotor novo.\n",		// Portuguese
											L"A amostra n�o foi misturada com os materiais reagentes suficientemente. Execute novamente com um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_QUALITY			,	{	L"The sample may have one or more physical interferents. Confirm the sample integrity and rerun rotor.\n",										// English
											L"Il se peut que l'�chantillon pr�sente un ou plusieurs �l�ments interf�rents. V�rifier l'int�grit� de l'�chantillon et r�ex�cuter le rotor.\n",	// French
											L"Die Probe hat m�glicherweise eine oder mehrere physische st�rende Bestandteile. Unversehrtheit der Probe best�tigen und Rotor neu starten.\n",	// German
											L"Puede que la muestra tenga una o varias interferencias f�sicas. Confirme la integridad de la muestra y vuelva a usar el rotor.\n",				// Spanish
											L"Il campione potrebbe avere uno o pi� fattori di interferenza fisica. Confermare l'integrit� del campione e riavviare il rotore.\n",			// Italian
											L"A amostra pode ter uma ou v�rias interfer�ncias f�sicas. Confirme a integridade da amostra e execute novamente o rotor.\n",					// Portuguese
											L"A amostra pode ter um ou mais interferentes f�sicos. Confirme a integridade da amostra e execute novamente o rotor.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_SOFTWARE_UPDATE			,	{	L"The rotor inserted requires a software update to operate correctly. Contact Technical Service for further assistance.\n",														// English
											L"Le rotor ins�r� n�cessite une mise � jour de logiciel pour pouvoir fonctionner correctement. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											L"Der eingesetzte Rotor erfordert ein Software-Update f�r einen ordnungsgem��en Betrieb. Wenden Sie sich an den technischen Service.\n",											// German
											L"El rotor insertado necesita la actualizaci�n del software para operar correctamente. P�ngase en contacto con el servicio t�cnico.\n",											// Spanish
											L"Il rotore inserito richiede un aggiornamento del software per funzionare correttamente. Per ulteriore assistenza contattare il servizio tecnico.\n",							// Italian
											L"O rotor inserido necessita de uma atualiza��o do software para funcionar adequadamente. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",					// Portuguese
											L"O rotor inserido requer uma atualiza��o de software para funcionar corretamente. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_STARTING_ABSORBANCE		,	{	L"Starting absorbance of the sample blank bead is outside the limit, possibly due to an exogenous substance or distribution error. Rerun with new rotor.\n",																		// English
											L"L'absorbance de d�part de la bille vierge de l'�chantillon n'entre pas dans les limites prescrites, �ventuellement du fait d'une substance exog�ne ou d'une erreur de distribution. R�ex�cuter � l'aide du nouveau rotor.\n",	// French
											L"Die Startabsorbierung der Leer-Beadprobe befindet sich m�glicherweise aufgrund einer exogenen Substanz oder eines Verteilungsfehlers au�erhalb des Grenzwerts. Mit einem neuen Rotor wiederholen.\n",							// German
											L"La absorbancia de inicio de la cuenta virgen de muestra est� fuera del l�mite, probablemente debido a una sustancia ex�gena o a un error de distribuci�n. Usar con un nuevo rotor.\n",											// Spanish
											L"L'assorbenza iniziale dei granuli campione vuoti non rientra nei limiti, forse a causa di una sostanza esogena od a un errore di distribuzione. Riavviare il campione con un nuovo rotore.\n",									// Italian
											L"A absorv�ncia inicial da esfera de amostra de branco est� fora do limite, possivelmente devido a uma sust�ncia ex�gena ou erro de distribui��o. Executar novamente utilizando um rotor novo.\n",								// Portuguese
											L"A absor��o inicial da esfera em branco da amostra est� fora do limite, possivelmente devido a uma subst�ncia ex�gena ou um erro de distribui��o. Execute novamente com um novo rotor.\n"										// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_SENSOR				,	{	L"The rotor temperature sensor is not operating. Contact Technical Service for further assistance.\n",										// English
											L"Le d�tecteur de temp�rature du rotor ne fonctionne pas. Pour obtenir une assistance compl�mentaire, contacter le service technique.\n",	// French
											L"Der Temperatursensor des Rotors ist au�er Betrieb. Wenden Sie sich an den technischen Service.\n",											// German
											L"El sensor de temperatura del rotor no est� funcionando. P�ngase en contacto con el servicio t�cnico.\n",									// Spanish
											L"Il sensore di temperatura del rotore non � in funzione. Per ulteriore assistenza contattare il servizio tecnico.\n",						// Italian
											L"O sensor de temperatura do rotor n�o est� a funcionar. Contacte o Servi�o de Assist�ncia T�cnica para obter assist�ncia.\n",				// Portuguese
											L"O sensor de temperatura do rotor n�o est� funcionando. Entre em contato com o Servi�o T�cnico para obter assist�ncia adicional.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_VARIATION			,	{	L"The temperature deviated more than one degree during the run. The accuracy of certain chemistries may be affected. Rerun with new rotor.\n",															// English
											L"La temp�rature a pr�sent� un �cart de plus d'un degr� au cours de l'ex�cution. La pr�cision de certains produits chimiques pourra en �tre affect�e. R�ex�cuter � l'aide du nouveau rotor.\n",			// French
											L"Der Temperatur wich um mehr als ein Grad w�hrend des Betriebs ab. Die Genauigkeit bestimmter Chemie kann beeintr�chtigt worden sein. Mit einem neuen Rotor wiederholen.\n",							// German
											L"La temperatura cambi� en m�s de un grado durante el an�lisis. Puede que afecte a la precisi�n del an�lisis de ciertas sustancias qu�micas. Utilice un nuevo rotor.\n",									// Spanish
											L"Durante il funzionamento la temperatura � variata di pi� di un grado. La precisione di alcune sostanze chimiche potrebbe esserne stata influenzata. Riavviare il campione con un nuovo rotore.\n",		// Italian
											L"A altera��o da temperatura foi superior a um grau durante a execu��o. A precis�o de determinadas sust�ncias qu�micas pode ser afetada. Executar novamente utilizando um rotor novo.\n",				// Portuguese
											L"A temperatura desviou mais de um grau durante a execu��o. A precis�o de certas subst�ncias qu�micas pode estar afetada. Execute novamente com um novo rotor.\n"										// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMPERATURE				,	{	L"The temperature of the rotor either varied too much or was not in the proper range during the run. If symptom continues, contact Technical Service.\n",																			// English
											L"La temp�rature du rotor a soit vari� dans des proportions trop importantes, soit ne s'inscrit pas dans la fourchette prescrite en cours d'ex�cution. En cas de persistance des sympt�mes, contacter le service technique.\n",		// French
											L"Die Rotortemperatur variierte zu stark oder befand sich w�hrend des Betriebs au�erhalb des optimalen Bereichs. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",											// German
											L"La temperatura del rotor vari� mucho o no estaba en el intervalo adecuado durante el an�lisis. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",											// Spanish
											L"Durante il funzionamento la temperatura del rotore ha subito variazioni eccessive oppure non era nel range adatto. Se i sintomi continuano contattare il servizio tecnico.\n",														// Italian
											L"A temperatura do rotor variou muito ou n�o esteve dentro do intervalo adequado durante a an�lise. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",															// Portuguese
											L"A temperatura do rotor variou muito ou n�o permaneceu no intervalo adequado durante a execu��o. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"																	// Portuguese (Brazil)
										} 
	},
	{ EISTR_WRONG_ROTOR				,	{	L"The rotor inserted is not intended for this device. If symptom continues, contact Technical Service.\n",										// English
											L"Le rotor ins�r� n'est pas pr�vu pour cet appareil. En cas de persistance des sympt�mes, contacter le service technique.\n",					// French
											L"Der eingesetzte Rotor ist f�r dieses Ger�t nicht geeignet. Bei Fortsetzung der St�rung wenden Sie sich an den technischen Service.\n",			// German
											L"El rotor insertado no es apto para este aparato. Si el mismo problema siguiera ocurriendo, p�ngase en contacto con el servicio t�cnico.\n",	// Spanish
											L"Il rotore inserito non � adatto a questo strumento. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"O rotor inserido n�o � adequado para este dispositivo. Se o problema persistir, contacte o servi�o de Assist�ncia T�cnica.\n",					// Portuguese
											L"O rotor inserido n�o � destinado a este dispositivo. Se o sintoma persistir, entre em contato com o Servi�o T�cnico.\n"						// Portuguese (Brazil)
										} 
	},
	
// Results (display and print)				ENGLISH,				FRENCH,					GERMAN,					SPANISH,					ITALIAN, 					PORTUGUESE,						PORTUGUESE (BRAZIL)
	{ RSTR_ROTOR_LOT_NUM			,	{	L"Rotor Lot Number:",	L"N� de lot rotor :",	L"Rotor-Losnr.:",		L"No. de lote del rotor:",	L"Numero lotto rotore:",	L"N�mero lote rotor:",			L"N�m. de lote do rotor:"	}	},
	{ RSTR_DOV						,	{	L"DOV:",				L"DOV :",				L"LI:",					L"�l. vac.:",				L"DV:",						L"DOV:",						L"Data vacina��o:"	}	},
	{ RSTR_KENNEL_NUM				,	{	L"Kennel Number:",		L"Num�ro chenil :",		L"Zwingernr.:",			L"No. de caseta:",			L"Numero Kennel:",			L"N�mero de Kennel:",			L"N�mero do canil:"	}	},
	{ RSTR_OWNER_ID					,	{	L"Owner ID:",			L"ID prop.:",			L"Besitzer-ID:",		L"Id. del due�o:",			L"ID propr.:",				L"ID do propr.:",				L"ID propriet.:"	}	},
	
// Genders									ENGLISH,	FRENCH,		GERMAN,			SPANISH,		ITALIAN, 	   	PORTUGUESE,		PORTUGUESE (BRAZIL)
	{ GSTR_FEMALE					,	{	L"Female",	L"Femelle",	L"Weiblich",	L"Hembra",		L"Femmina",		L"F�mea",		L"Sexo feminino"	}	},
	{ GSTR_MALE						,	{	L"Male",	L"M�le",	L"M�nnlich",	L"Macho",		L"Maschio",		L"Macho",		L"Sexo masculino"	}	},

// Species									ENGLISH,				FRENCH,					GERMAN,					SPANISH,				ITALIAN, 				PORTUGUESE,				PORTUGUESE (BRAZIL)	
	{ SSTR_OTHER					,	{	L"Other",				L"Autre",				L"Sonstige",			L"Otro",				L"Altro",				L"Outro",				L"Outros"	}	},
	{ SSTR_SPECIES					,	{	L"Species",				L"Esp�ces",				L"Arten",				L"Especie",				L"Specie",				L"Esp�cies",			L"Esp�cies"	}	},	
	{ SSTR_CANINES					,	{	L"Canines",				L"Canins",				L"Hunde",				L"Caninos",				L"Canini",				L"Caninas",				L"Caninos"	}	},
	{ SSTR_FELINES					,	{	L"Felines",				L"F�lins",				L"Katzen",				L"Felinos",				L"Felini",				L"Felinas",				L"Felinos"	}	},
	{ SSTR_EQUINES					,	{	L"Equines",				L"Chevalins",			L"Pferde",				L"Equinos",				L"Equini",				L"Equinas",				L"Equinos"	}	},
	{ SSTR_BOVINES					,	{	L"Bovines",				L"Bovins",				L"Rinder",				L"Bovinos",				L"Bovini",				L"Bovinas",				L"Bovinos"	}	},
	{ SSTR_AVIANS					,	{	L"Avians",				L"Aviaires",			L"V�gel",				L"Aves",				L"Aviari",				L"Avi�rias",			L"Aves"	}	},
	{ SSTR_REPTILES_AMPHIBIANS		,	{	L"Reptiles/Amphibians",	L"Reptiles/Amphibies",	L"Reptilien/Amphibien",	L"Reptiles/anfibios",	L"Rettili/Anfibi",		L"R�pteis/Anfibios",	L"R�pteis/anf�bios"	}	},
	{ SSTR_SMALL_MAMMALS			,	{	L"Small Mammals",		L"Petits mammif�res",	L"Kleins�ugetiere",		L"Peque�os mam�feros",	L"Piccoli mammiferi",	L"Pequenos mam�feros",	L"Mam�feros pequenos"	}	},
	{ SSTR_LARGE_MAMMALS			,	{	L"Large Mammals",		L"Grands mammif�res",	L"Gro�s�ugetiere",		L"Grandes mam�feros",	L"Grandi mammiferi",	L"Grandes mam�feros",	L"Mam�feros grandes"	}	},

	// Canines
	{ SSTR_DOG						,	{	L"Dog",					L"Chien",				L"Hund",				L"Perro",				L"Cane",				L"C�o",					L"C�o"	}	},
	{ SSTR_COYOTE					,	{	L"Coyote",				L"Coyote",				L"Kojote",				L"Coyote",				L"Coyote",				L"Coiote",				L"Coiote"	}	},
	{ SSTR_DINGO					,	{	L"Dingo",				L"Dingo",				L"Dingo",				L"Dingo",				L"Dingo",				L"Dingo",				L"C�o selvagem"	}	},
	{ SSTR_FOX						,	{	L"Fox",					L"Renard",				L"Fuchs",				L"Zorro",				L"Volpe",				L"Raposa",				L"Raposa"	}	},
	{ SSTR_JACKAL					,	{	L"Jackal",				L"Chacal",				L"Schakal",				L"Chacal",				L"Sciacallo",			L"Chacal",				L"Chacal"	}	},
	{ SSTR_WOLF						,	{	L"Wolf",				L"Loup",				L"Wolf",				L"Lobo",				L"Lupo",				L"Lobo",				L"Lobo"	}	},

	// Felines
	{ SSTR_CAT						,	{	L"Cat",					L"Chat",				L"Katze",				L"Gato",				L"Gatto",				L"Gato",				L"Gato"	}	},
	{ SSTR_BOBCAT					,	{	L"Bobcat",				L"Lynx roux",			L"Rotluchs",			L"Lince rojo",			L"Lince rossa",			L"Lince vermelho",		L"Lince"	}	},
	{ SSTR_CHEETAH					,	{	L"Cheetah",				L"Gu�pard",				L"Gepard",				L"Guepardo",			L"Ghepardo",			L"Chita",				L"Gato selvagem"	}	},
	{ SSTR_JAGUAR					,	{	L"Jaguar",				L"Jaguar",				L"Jaguar",				L"Jaguar",				L"Giaguaro",			L"Jaguar",				L"Jaguar"	}	},
	{ SSTR_LEOPARD					,	{	L"Leopard",				L"L�opard",				L"Leopard",				L"Leopardo",			L"Leopardo",			L"Leopardo",			L"Leopardo"	}	},
	{ SSTR_LION						,	{	L"Lion",				L"Lion",				L"L�we",				L"Le�n",				L"Leone",				L"Le�o",				L"Le�o"	}	},
	{ SSTR_LYNX						,	{	L"Lynx",				L"Lynx",				L"Luchs",				L"Lince",				L"Lince",				L"Lince",				L"Gato selvagem"	}	},
	{ SSTR_PANTHER					,	{	L"Panther",				L"Panth�re",			L"Panther",				L"Pantera",				L"Pantera",				L"Pantera",				L"Pantera"	}	},
	{ SSTR_PUMA						,	{	L"Puma",				L"Puma",				L"Puma",				L"Puma",				L"Puma",				L"Puma",				L"Puma"	}	},
	{ SSTR_TIGER					,	{	L"Tiger",				L"Tigre",				L"Tiger",				L"Tigre",				L"Tigre",				L"Tigre",				L"Tigre"	}	},
	
	// Equines
	{ SSTR_HORSE					,	{	L"Horse",				L"Cheval",				L"Pferd",				L"Caballo",				L"Cavallo",				L"Cavalo",				L"Cavalo"	}	},
	{ SSTR_DONKEY					,	{	L"Donkey",				L"Ane",					L"Esel",				L"Burro",				L"Asino",				L"Burro",				L"Asno"	}	},
	{ SSTR_PONY						,	{	L"Pony",				L"Poney",				L"Pony",				L"Poni",				L"Pony",				L"P�nei",				L"P�nei"	}	},
	{ SSTR_ZEBRA					,	{	L"Zebra",				L"Z�bre",				L"Zebra",				L"Cebra",				L"Zebra",				L"Zebra",				L"Zebra"	}	},
	
	// Bovines
	{ SSTR_COW						,	{	L"Cow",					L"Vache",				L"Kuh",					L"Vaca",				L"Mucca",				L"Vaca",					L"Vaca"	}	},
	{ SSTR_WATER_BUFFALO			,	{	L"Water Buffalo",		L"Buffle (d'Asie)",		L"Wasserb�ffel",		L"B�falo de agua",		L"Bufalo d'acqua",		L"B�falo de �gua",		L"B�falo d��gua"	}	},
	{ SSTR_STEER					,	{	L"Steer",				L"Bouvillon",			L"Stier",				L"Novillo",				L"Manzo",				L"Boi",					L"Novilho"	}	},
	{ SSTR_OX						,	{	L"Ox",					L"Boeuf",				L"Ochse",				L"Buey",				L"Bue",					L"Boi",					L"Boi"	}	},
	{ SSTR_BULL						,	{	L"Bull",				L"Taureau",				L"Bulle",				L"Toro",				L"Toro",				L"Touro",				L"Touro"	}	},
	{ SSTR_BUFFALO					,	{	L"Buffalo",				L"Buffle",				L"B�ffel",				L"B�falo",				L"Bufalo",				L"B�falo",				L"B�falo"	}	},
	{ SSTR_BISON					,	{	L"Bison",				L"Bison",				L"Bison",				L"Bisonte",				L"Bisonte",				L"Bisonte",				L"Bis�o"	}	},
	
	// Avians
	{ SSTR_BIRD						,	{	L"Bird",				L"Oiseau",				L"Vogel",				L"P�jaro",				L"Uccello",				L"P�ssaro",				L"P�ssaro"	}	},
	{ SSTR_AFRICAN_GRAY				,	{	L"African Gray",		L"Perroquet gris",		L"Afrik.Graupapagei",	L"Rucio africano",		L"Pap. grigio afr.",	L"Papagaio africano",	L"Papagaio africano"	}	},
	{ SSTR_AMAZON					,	{	L"Amazon",				L"Amazone",				L"Amazone",				L"Amazona",				L"Amazzoni",			L"Amazonas",			L"Amazona"	}	},
	{ SSTR_CHICKEN					,	{	L"Chicken",				L"Poulet",				L"Huhn",				L"Pollo",				L"Pollo",				L"Galinha",				L"Frango"	}	},
	{ SSTR_COCKATIEL				,	{	L"Cockatiel",			L"Calopsitte �l�gte",	L"Nymphensittich",		L"Carolina",			L"Cockatiel",			L"Caturra",				L"Calopsita"	}	},
	{ SSTR_COCKATOO					,	{	L"Cockatoo",			L"Cacato�s",			L"Kakadu",				L"Cacat�a",				L"Cacatua",				L"Catatua",				L"Cacatua"	}	},
	{ SSTR_CONURE					,	{	L"Conure",				L"Conure",				L"Sittiche",			L"Lorito americano",	L"Conuro",				L"Conure",				L"Jandaia"	}	},
	{ SSTR_DOVE						,	{	L"Dove",				L"Colombe",				L"Taube",				L"Paloma",				L"Colombo",				L"Pomba",				L"Pombo"	}	},
	{ SSTR_DUCK						,	{	L"Duck",				L"Canard",				L"Ente",				L"Pato",				L"Anatra",				L"Pato",				L"Pato"	}	},
	{ SSTR_FLAMINGO					,	{	L"Flamingo",			L"Flamand",				L"Flamingo",			L"Flamenco",			L"Fenicottero",			L"Flamingo",			L"Flamingo"	}	},
	{ SSTR_GOOSE					,	{	L"Goose",				L"Oie",					L"Gans",				L"Ganso",				L"Oca",					L"Ganso",				L"Ganso"	}	},
	{ SSTR_LOVEBIRD					,	{	L"Lovebird",			L"Ins�parable",			L"Erdbeerkopf",			L"Inseparable",			L"Inseparabile",		L"Agapornis",			L"P�ssaro-do-amor"	}	},
	{ SSTR_MACAW					,	{	L"Macaw",				L"Ara",					L"Ara",					L"Guacamayo",			L"Ara",					L"Arara",				L"Arara"	}	},
	{ SSTR_MYNAH					,	{	L"Mynah",				L"Mainate",				L"Beo",					L"Gr�cula religiosa",	L"Storno triste",		L"Main�",				L"Main�"	}	},
	{ SSTR_OSTRICH					,	{	L"Ostrich",				L"Autruche",			L"Strau�",				L"Avestruz",			L"Struzzo",				L"Avestruz",			L"Avestruz"	}	},
	{ SSTR_PARAKEET					,	{	L"Parakeet",			L"Starique perroq.",	L"Wellensittich",		L"Periquito",			L"Parrocchetto",		L"Periquito",			L"Periquito"	}	},
	{ SSTR_PARROT					,	{	L"Parrot",				L"Perroquet",			L"Papagei",				L"Loro",				L"Pappagallo",			L"Papagaio",			L"Papagaio"	}	},
	{ SSTR_TOUCAN					,	{	L"Toucan",				L"Toucan",				L"Tukan",				L"Tuc�n",				L"Tucano",				L"Tucano",				L"Tucano"	}	},
	
	// Reptiles/amphibians
	{ SSTR_REPTILE					,	{	L"Reptile",				L"Reptile",				L"Reptil",				L"Reptil",				L"Rettile",				L"R�ptil",				L"R�ptil"	}	},
	{ SSTR_ALLIGATOR				,	{	L"Alligator",			L"Alligator",			L"Alligator",			L"Caim�n",				L"Alligatore",			L"Alig�tor",			L"Jacar�"	}	},
	{ SSTR_AMPHIBIAN				,	{	L"Amphibian",			L"Amphibie",			L"Amphibie",			L"Anfibio",				L"Anfibio",				L"Anf�bio",				L"Anf�bio"	}	},
	{ SSTR_BEARDED_DRAGON			,	{	L"Bearded Dragon",		L"L�zard � barbe",		L"Bartagame",			L"Drag�n barbado",		L"Pogona vitticeps",	L"Drag�o barbudo",		L"Drag�o farpado"	}	},
	{ SSTR_BOA_CONSTRICTOR			,	{	L"Boa Constrictor",		L"Boa constrictor",		L"Boa constrictor",		L"Boa constrictor",		L"Boa Costrittore",		L"Jib�ia",				L"Jiboia"	}	},
	{ SSTR_CHAMELEON				,	{	L"Chameleon",			L"Cam�l�on",			L"Cham�leon",			L"Camale�n",			L"Camaleonte",			L"Camale�o",			L"Camale�o"	}	},
	{ SSTR_CROCODILE				,	{	L"Crocodile",			L"Crocodile",			L"Krokodil",			L"Cocodrilo",			L"Coccodrillo",			L"Crocodilo",			L"Crocodilo"	}	},
	{ SSTR_FROG						,	{	L"Frog",				L"Grenouille",			L"Frosch",				L"Rana",				L"Rana",				L"R�",					L"R�"	}	},
	{ SSTR_GECKO					,	{	L"Gecko",				L"Gecko",				L"Gecko",				L"Lagartija",			L"Geco",				L"Osga",				L"Lagartixa"	}	},
	{ SSTR_IGUANA					,	{	L"Iguana",				L"Iguane",				L"Leguan",				L"Iguana",				L"Iguana",				L"Iguana",				L"Iguana"	}	},
	{ SSTR_LIZARD					,	{	L"Lizard",				L"L�zard",				L"Eidechse",			L"Lagarto",				L"Lucertola",			L"Lagarto",				L"Lagarto"	}	},
	{ SSTR_MONITOR					,	{	L"Monitor",				L"Varan",				L"Waran",				L"Varano",				L"Varano",				L"Monitor",				L"Lagarto monitor"	}	},
	{ SSTR_PYTHON					,	{	L"Python",				L"Python",				L"Python",				L"Pit�n",				L"Pitone",				L"Pit�o",				L"P�ton"	}	},
	{ SSTR_SALAMANDER				,	{	L"Salamander",			L"Salamandre",			L"Salamander",			L"Salamandra",			L"Salamandra",			L"Salamandra",			L"Salamandra"	}	},
	{ SSTR_SNAKE					,	{	L"Snake",				L"Serpent",				L"Schlange",			L"Serpiente",			L"Serpente",			L"Cobra",				L"Cobra"	}	},
	{ SSTR_TOAD						,	{	L"Toad",				L"Crapaud",				L"Kr�te",				L"Sapo",				L"Rospo",				L"Sapo",				L"Sapo"	}	},
	{ SSTR_TORTOISE					,	{	L"Tortoise",			L"Tortue",				L"Schildkr�te",			L"Tortuga",				L"Testuggine",			L"Testudinidae",		L"C�gado"	}	},
	{ SSTR_TURTLE					,	{	L"Turtle",				L"Tortue marine",		L"Wasserschildkr�te",	L"Tortuga marina",		L"Tartaruga",			L"Tartaruga",			L"Tartaruga"	}	},
	
	// Small mammals
	{ SSTR_SMALL_MAMMAL				,	{	L"Small Mammal",		L"Petit mammif�re",		L"Kleins�ugetier",		L"Mam�fero peque�o",	L"Piccolo mammifero",	L"Pequeno mam�fero",	L"Mam�fero pequeno"	}	},
	{ SSTR_BAT						,	{	L"Bat",					L"Chauve-souris",		L"Fledermaus",			L"Murci�lago",			L"Pipistrello",			L"Morcego",				L"Morcego"	}	},
	{ SSTR_CHINCHILLA				,	{	L"Chinchilla",			L"Chinchilla",			L"Chinchilla",			L"Chinchilla",			L"Cincill�",			L"Chinchila",			L"Chinchila"	}	},
	{ SSTR_FERRET					,	{	L"Ferret",				L"Furet",				L"Frettchen",			L"Hur�n",				L"Furetto",				L"Fur�o",				L"Fur�o"	}	},
	{ SSTR_GERBIL					,	{	L"Gerbil",				L"Gerbille",			L"Rennmaus",			L"Jerbo",				L"Gerbillo",			L"Gerbo",				L"Gerbo"	}	},
	{ SSTR_GUINEA_PIG				,	{	L"Guinea Pig",			L"Cochon d'Inde",		L"Meerschweinchen",		L"Cobaya",				L"Cavia",				L"Cobaia",				L"Cobaia"	}	},
	{ SSTR_HAMSTER					,	{	L"Hamster",				L"Hamster",				L"Hamster",				L"H�mster",				L"Criceto",				L"Hamster",				L"Hamster"	}	},
	{ SSTR_HEDGEHOG					,	{	L"Hedgehog",			L"H�risson",			L"Igel",				L"Erizo",				L"Riccio",				L"Ouri�o",				L"Ouri�o"	}	},
	{ SSTR_LEMUR					,	{	L"Lemur",				L"L�murien",			L"Lemur",				L"L�mur",				L"Lemure",				L"L�mure",				L"L�mure"	}	},
	{ SSTR_MINK						,	{	L"Mink",				L"Vison",				L"Nerz",				L"Vis�n",				L"Visone",				L"Marta",				L"Marta"	}	},
	{ SSTR_MOUSE					,	{	L"Mouse",				L"Souris",				L"Maus",				L"Rat�n",				L"Topo",				L"Rato",				L"Camundongo"	}	},
	{ SSTR_OPOSSUM					,	{	L"Opossum",				L"Opossum",				L"Opossum",				L"Zarig�eya",			L"Opossum",				L"Marsupial",			L"Gamb�"	}	},
	{ SSTR_PRAIRIE_DOG				,	{	L"Prairie Dog",			L"Chien de prairie" ,	L"Pr�riehund",			L"Perro de Pradera",	L"Cane prateria",		L"C�o de pradaria",		L"C�o dos prados"	}	},
	{ SSTR_RABBIT					,	{	L"Rabbit",				L"Lapin",				L"Hase",				L"Conejo",				L"Coniglio",			L"Coelho",				L"Coelho"	}	},
	{ SSTR_RACCOON					,	{	L"Raccoon",				L"Raton laveur",		L"Waschb�r",			L"Mapache",				L"Procione",			L"Guaxinim",			L"Guaxinim"	}	},
	{ SSTR_RAT						,	{	L"Rat",					L"Rat",					L"Ratte",				L"Rata",				L"Ratto",				L"Ratazana",			L"Rato"	}	},
	{ SSTR_SKUNK					,	{	L"Skunk",				L"Moufette",			L"Stinktier",			L"Mofeta",				L"Moffetta",			L"Gamb�",				L"Doninha"	}	},
	{ SSTR_SQUIRREL					,	{	L"Squirrel",			L"�cureuil",			L"Eichh�rnchen",		L"Ardilla",				L"Scoiattolo",			L"Esquilo",				L"Esquilo"	}	},
	
	// Large mammals
	{ SSTR_LARGE_MAMMAL				,	{	L"Large Mammal",		L"Grand mammif�re",		L"Gro�s�ugetier",		L"Grandes mam�feros",	L"Grande mammifero",	L"Grande mam�fero",		L"Mam�fero grande"	}	},
	{ SSTR_ANTELOPE					,	{	L"Antelope",			L"Antilope",			L"Antilope",			L"Ant�lope",			L"Antilope",			L"Ant�lope",			L"Ant�lope"	}	},
	{ SSTR_APE						,	{	L"Ape",					L"Grand singe",			L"Menschenaffe",		L"Simio",				L"Scimmia",				L"S�mio",				L"S�mio"	}	},
	{ SSTR_BABOON					,	{	L"Baboon",				L"Babouin",				L"Pavian",				L"Babuino",				L"Babbuino",			L"Babu�no",				L"Babu�no"	}	},
	{ SSTR_BEAR						,	{	L"Bear",				L"Ours",				L"B�r",					L"Oso",					L"Orso",				L"Urso",				L"Urso"	}	},
	{ SSTR_CAMEL					,	{	L"Camel",				L"Chameau",				L"Kamel",				L"Camello",				L"Cammello",			L"Camelo",				L"Camelo"	}	},
	{ SSTR_CHIMPANZEE				,	{	L"Chimpanzee",			L"Chimpanz�",			L"Schimpanse",			L"Chimpanc�",			L"Scimpanz�",			L"Chimpanz�",			L"Chimpanz�"	}	},
	{ SSTR_DEER						,	{	L"Deer",				L"Cerf",				L"Reh",					L"Venado",				L"Cervide",				L"Veado",				L"Veado"	}	},
	{ SSTR_ELEPHANT					,	{	L"Elephant",			L"�l�phant",			L"Elefant",				L"Elefante",			L"Elefante",			L"Elefante",			L"Elefante"	}	},
	{ SSTR_GIRAFFE					,	{	L"Giraffe",				L"Girafe",				L"Giraffe",				L"Jirafa",				L"Giraffa",				L"Girafa",				L"Girafa"	}	},
	{ SSTR_GOAT						,	{	L"Goat",				L"Ch�vre",				L"Ziege",				L"Cabra",				L"Capra",				L"Cabra",				L"Bode"	}	},
	{ SSTR_KANGAROO					,	{	L"Kangaroo",			L"Kangourou",			L"K�nguru",				L"Canguro",				L"Canguro",				L"Canguru",				L"Canguru"	}	},
	{ SSTR_LLAMA					,	{	L"Llama",				L"Lama",				L"Lama",				L"Llama",				L"Lama",				L"Lama",				L"Lhama"	}	},
	{ SSTR_MONKEY					,	{	L"Monkey",				L"Singe",				L"Affe",				L"Mono",				L"Scimmia",				L"Macaco",				L"Macaco"	}	},
	{ SSTR_PIG						,	{	L"Pig",					L"Cochon",				L"Schwein",				L"Cerdo",				L"Maiale",				L"Porco",				L"Porco"	}	},
	{ SSTR_SEA_LION					,	{	L"Sea Lion",			L"Lion de mer",			L"Seel�we",				L"Le�n marino",			L"Leone marino",		L"Le�o-marinho",		L"Le�o-marinho"	}	},
	{ SSTR_SEAL						,	{	L"Seal",				L"Phoque",				L"See-Elefant",			L"Foca",				L"Foca",				L"Foca",				L"Foca"	}	},
	{ SSTR_SHEEP					,	{	L"Sheep",				L"Mouton",				L"Schaf",				L"Oveja",				L"Pecora",				L"Ovelha",				L"Ovelha"	}	},
	
	{ STR_NONE						, {} }
};

#endif
