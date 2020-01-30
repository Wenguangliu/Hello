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
	{ TSTR_ADD_SPECIES				,	{	L"Add Demographic",						L"Ajouter éléments démogr.",			L"Demographie hinzufügen",				L"Añadir demografía",					L"Aggiungi demografia",					L"Adicionar demografia",				L"Adicionar dados demográficos"	}	},
	{ TSTR_ADMINISTRATOR			,	{	L"Administrator",						L"Administrateur",						L"Administrator",						L"Administrador",						L"Amministratore",						L"Administrador",						L"Administrador"	}	},
	{ TSTR_ADMINISTRATOR_ID			,	{	L"Administrator ID",					L"ID administrateur",					L"Administrator-ID",					L"Id. del administrador",				L"ID amministratore",					L"ID do administrador",					L"ID do administrador"	}	},
	{ TSTR_ADVANCED_SETTINGS		,	{	L"Advanced Settings",					L"Paramètres avancés",					L"Erweiterte Einstellungen",			L"Configuración avanzada",				L"Impostazioni avanzate",				L"Configurações avançadas",				L"Configurações avançadas"	}	},
	{ TSTR_ALERT_VOLUME				,	{	L"Alert Volume",						L"Alerte volume",						L"Mengenalarm",							L"Volumen de alerta",					L"Volume allarme",						L"Volume de alerta",					L"Volume do alerta"	}	},
	{ TSTR_ALL_RESULTS_BY_DATE		,	{	L"All Results Sorted by Date",			L"Tous résultats triés par date",		L"Alle Ergebnisse nach Datum",			L"Resultados por fecha",				L"Tutti i risultati ordinati per data",	L"Todos os resultados por data",		L"Todos resultados classif. por data"	}	},
	{ TSTR_ALL_RESULTS_BY_ID		,	{	L"All Results Sorted by ID",			L"Tous résultats triés par ID",			L"Alle Ergebnisse nach ID",				L"Resultados por Id.",					L"Tutti i risultati ordinati per ID",	L"Todos os resultados por ID",			L"Todos resultados classif. por ID"	}	},
	{ TSTR_ALL_UNITS				,	{	L"All Units",							L"Toutes unités",						L"Alle Einheiten",						L"Todas la unidades",					L"Tutte le unità",						L"Todas as unidades",					L"Todas as unidades"	}	},
	{ TSTR_ALTERNATE_ID				,	{	L"Alternate ID",						L"Autre ID",							L"ID ändern",							L"Id. suplente",						L"ID alternativo",						L"ID suplente",							L"ID alternativo"	}	},
	{ TSTR_ANALYTE_OPTIONS			,	{	L"Analyte Options", 					L"Options à analyser",					L"Analytoptionen",						L"Opciones de analito", 				L"Opzioni analita"						L"Opções de analito",					L"Opções de analito"	}	},
	{ TSTR_ANALYTES					,	{	L"Analytes",							L"Substances à analyser",				L"Analyte",								L"Analitos",							L"Analiti",								L"Analitos",							L"Analitos"	}	},
	{ TSTR_ANALYZER_INFORMATION		,	{	L"Analyzer Information",				L"Information analyseur",				L"Analysegerät-Angaben",				L"Información del analizador",			L"Informazioni analizzatore",			L"Informação do analisador",			L"Informações do analisador"	}	},
	{ TSTR_ANALYZER_SETTINGS		,	{	L"Analyzer Settings",					L"Paramètres analyseur",				L"Analysegerät-Einstellungen",			L"Configuración del analizador",		L"Impostazioni analizzatore",			L"Configurações do analisador",			L"Configurações do analisador"	}	},
	{ TSTR_ARCHIVE_REF_RANGES		,	{	L"Archive Reference Ranges",			L"Fourchettes de réf. archives",		L"Archiv-Referenzbereiche",				L"Guardar intervalos de referencia",	L"Range di riferimento in archivio",	L"Guardar intervalos de referência",	L"Arquivar intervalos de referência"	}	},
	{ TSTR_ASTM_ORDER_METHOD		,	{	L"ASTM Order Method",					L"Méthode d'ordre ASTM",				L"ASTM: Anforderungsmethode",			L"Método de pedido de ASTM",			L"Metodo ordine ASTM",					L"Método de pedido de ASTM",			L"Método de solicitação de ASTM"	}	},
	{ TSTR_ASTM_PHYSICAL_LAYER		,	{	L"ASTM Physical Layer",					L"Couche physique ASTM",				L"ASTM: Physikalische Ebene",			L"Nivel físico ASTM",					L"Livello fisico ASTM",					L"Nível físico de ASTM",				L"Camada física de ASTM"	}	},
	{ TSTR_ASTM_TCPIP_SETTINGS		,	{	L"ASTM TCP/IP Settings", 				L"Paramètres TCP/IP ASTM",				L"ASTM: TCP/IP-Einstellungen",			L"Configuración TCP/IP ASTM",			L"Impostazioni TCP/IP ASTM",			L"Configurações de TCP/IP de ASTM",		L"Configurações de TCP/IP do ASTM"	}	},
	{ TSTR_BACKLIGHT_BRIGHTNESS		,	{	L"Backlight Brightness",				L"Lumière rétroéclairage",				L"Helligkeit Hintergrundbeleuchtung",	L"Brillo de la luz de fondo",			L"Luminosità retroilluminazione",		L"Brilho da luz de fundo",				L"Brilho da luz de fundo"	}	},
	{ TSTR_CALIBRATE_SCREEN			,	{	L"Calibrate Screen",					L"Calibrer écran",						L"Bildschirm kalibrieren",				L"Calibrar pantalla",					L"Calibra schermo",						L"Calibrar ecrã",						L"Calibrar tela"	}	},
	{ TSTR_CDROM_UPDATE				,	{	L"CD ROM Update",						L"Mise à jour CD ROM",					L"CD-ROM-Aktualisierung",				L"Actualización CD-ROM",				L"Aggiorna CD ROM",						L"Atualização do CD-ROM",				L"Atualização em CD ROM"	}	},
	{ TSTR_CHECK_TCPIP_SETTINGS		,	{	L"Check TCP/IP Settings", 				L"Vérifier les paramètres TCP/IP",		L"TCP/IP-Einstellungen überprüfen",		L"Comprobar configuración TCP/IP",		L"Verifica impostazioni TCP/IP",		L"Verificar configurações de TCP/IP",	L"Verificar configurações TCP/IP"	}	},
	{ TSTR_CLICK_VOLUME				,	{	L"Click Volume",						L"Volume clic",							L"Menge anklicken",						L"Pulsar volumen",						L"Fai clic su Volume",					L"Clique em Volume",					L"Clicar em Volume"	}	},
	{ TSTR_CLINICAL_ARCHIVE			,	{	L"Clinical Archive",					L"Archive clinique",					L"Klinisches Archiv",					L"Archivo clínico",						L"Archivio clinico",					L"Ficheiro clínico",					L"Arquivo clínico"	}	},
	{ TSTR_CONFIGURE_PRINTER		,	{	L"Configure Printer",					L"Configurer Imprimante",				L"Drucker konfigurieren",				L"Configurar impresora",				L"Configura stampante",					L"Configurar impressora",				L"Configurar impressora"	}	},
	{ TSTR_CONFIRM					,	{	L"Confirm",								L"Confirmer",							L"Bestätigen",							L"Confirmar",							L"Conferma",							L"Confirmar",							L"Confirmar"	}	},
	{ TSTR_CONFIRM_ORDER			,	{	L"Confirm Order",						L"Confirmer ordre",						L"Anforderung bestätigen",				L"Confirmar pedido",					L"Conferma ordine",						L"Confirmar pedido",					L"Confirmar solicitação"	}	},
	{ TSTR_CONFIRM_PASSWORD			,	{	L"Confirm Password",					L"Confirmer mot de passe",				L"Passwort bestätigen",					L"Confirmar contraseña",				L"Conferma password",					L"Confirmar palavra-passe",				L"Confirmar senha"	}	},
	{ TSTR_CONFIRM_ROOM_TEMP		,	{	L"Confirm Room Temperature",			L"Confirmer température ambiante",		L"Raumtemperatur bestätigen",			L"Confirmar temperatura ambiente",		L"Conferma Temperatura ambiente",		L"Confirmar temperatura ambiente",		L"Confirmar temperatura ambiente" } },
	{ TSTR_CONTROL					,	{	L"Control:",							L"Contrôle :",							L"Kontrolle:",							L"Control:",							L"Controllo:",							L"Controlo:",							L"Controle:"	}	},
	{ TSTR_CONTROL_BY_DATE			,	{	L"Control Results Sorted by Date",		L"Contrôler résultats triés par date",	L"Ergebnisse nach Dat. überprüfen",		L"Resultados control por fecha",		L"Controlla risult. ordinati per data",	L"Resultados de controlo por data",		L"Result. controle classif. por data"	}	},
	{ TSTR_CONTROL_EXPIRATION_DATE	,	{	L"Control Expiration Date",				L"Contrôler date de péremption",		L"Ablaufdatum überprüfen",				L"Fecha de vencimiento de control",		L"Controlla data di scadenza",			L"Prazo de validade do controlo",		L"Data de expiração do controle"	}	},
	{ TSTR_CONTROL_UPDATE			,	{	L"Control Update",						L"Contrôler mise à jour",				L"Update überprüfen",					L"Actualización de control",			L"Controlla aggiornamento",				L"Atualização do controlo",				L"Atualização do controle"	}	},
	{ TSTR_CRITICAL_STOP			,	{	L"CRITICAL STOP",						L"ARRÊT CRITIQUE",						L"KRITISCHER ABBRUCH",					L"PARADA CRÍTICA",						L"INTERRUZIONE CRITICA",				L"PARAGEM CRÍTICA",						L"PARADA CRÍTICA"	}	},
	{ TSTR_DATA_ENTRY_OPTIONS		,	{	L"Data Entry Options",					L"Options de saisie des données",		L"Dateneingabeoptionen",				L"Opciones de entrada de datos",		L"Opzioni immissione dati",				L"Opções de entrada de dados",			L"Opções de inserção de dados"	}	},		
	{ TSTR_DISPLAY_SETTINGS			,	{	L"Display Settings",					L"Affichage paramètres",				L"Anzeigeeinstellungen",				L"Configuración de pantalla",			L"Impostazioni display",				L"Configurações do visor",				L"Configurações do display"	}	},
	{ TSTR_ELECTROLYTES_UNITS		,	{	L"Electrolytes Units",					L"Unités électrolytes",					L"Elektrolyteinheiten",					L"Unidades de electrolitos",			L"Unità elettroliti",					L"Unidades de eletrólitos",				L"Unidades de eletrólitos"	}	},
	{ TSTR_ENTER_ADMISSION_ID		,	{	L"Enter Admission ID",					L"Saisir ID admission",					L"Zugangs-ID eingeben",					L"Introducir Id. Admisión",				L"Inserisci ID ammissione",				L"Introduzir ID de admissão",			L"Inserir ID de admissão"	}	},
	{ TSTR_ENTER_AGE				,	{	L"Enter Age",							L"Saisir l'âge",						L"Alter eingeben",						L"Introducir edad",						L"Inserisci età",						L"Introduzir idade",					L"Inserir idade"	}	},
	{ TSTR_ENTER_AGE_RANGE			,	{	L"Enter Age Range",						L"Saisir fourchette d'âge",				L"Altersbereich eingeben",				L"Introducir intervalo de edad",		L"Inserisci fascia d'età",				L"Introduzir intervalo de idades",		L"Inserir faixa etária"	}	},
	{ TSTR_ENTER_ALTERNATE_ID		,	{	L"Enter Alternate ID",					L"Saisir autre ID",						L"Alternative ID eingeben",				L"Introducir Id. suplente",				L"Inserisci ID alternativo",			L"Introduzir ID suplente",				L"Inserir ID alternativo"	}	},
	{ TSTR_ENTER_ANALYZER_IP_ADDRESS,	{	L"Enter Analyzer IP Address",			L"Saisir l'adresse IP de l'analyseur",	L"Analysegeräte-IP-Adresse eingeben",	L"Introducir dirección IP analizador",	L"Immettere indir. IP analizzatore",	L"Introduzir endereço IP analisador",	L"Inserir endereço IP do analisador"	}	},
	{ TSTR_ENTER_ANALYZER_NET_NAME	,	{	L"Enter Analyzer Network Name",			L"Entrer nom du réseau de l’analyseur",	L"Name des Analysegerätes im Netzwerk",	L"Introducir nombre de red analizador",	L"Immetti nome rete analizzatore",		L"Introduzir nome rede do analisador",	L"Inserir nome da rede do analisador" } },
	{ TSTR_ENTER_CLIENT_SOCKET		,	{	L"Enter Client Socket",					L"Saisir le socket du client",			L"Client-Socket eingeben",				L"Introducir socket del cliente",		L"Immetti socket client",				L"Introduzir tomada do cliente",		L"Inserir soquete do cliente"  } },
	{ TSTR_ENTER_CONTROL_ID			,	{	L"Enter Control ID",					L"Entrer ID contrôle",					L"Kontrollen-ID eingeben",				L"Introducir Id. de control",			L"Inserisci ID controllo",				L"Introduzir ID do controlo",			L"Inserir ID de controle"	}	},
	{ TSTR_ENTER_DATE				,	{	L"Enter Date",							L"Saisir date",							L"Datum eingeben",						L"Introducir fecha",					L"Inserisci data",						L"Introduzir data",						L"Inserir data"	}	},
	{ TSTR_ENTER_DATE_OF_BIRTH		,	{	L"Enter Date of Birth",					L"Saisir date de naissance",			L"Geburtsdatum eingeben",				L"Introducir fecha de nacimiento",		L"Inserisci data di nascita",			L"Introduzir data de nascimento",		L"Inserir data de nascimento"	}	},
	{ TSTR_ENTER_DOCTOR_ID			,	{	L"Enter Doctor ID",						L"Saisir ID médecin",					L"Arzt-ID eingeben",					L"Introducir Id. del médico",			L"Inserisci ID medico",					L"Introduzir ID do médico",				L"Inserir ID do médico"	}	},
	{ TSTR_ENTER_END_DATE			,	{	L"Enter End Date",						L"Saisir la date de fin",				L"Enddatum eingeben",					L"Escriba fecha final",					L"Inserisci data fine",					L"Introduzir data final",				L"Inserir data final"	}	},
	{ TSTR_ENTER_GATEWAY_IP_ADDRESS	,	{	L"Enter Gateway IP Address",			L"Saisir adresse IP de la passerelle",	L"Gateway-IP-Adresse eingeben",			L"Introd. dirección IP puerta enlace",	L"Immettere indir. IP gateway",			L"Introduzir endereço IP da porta",		L"Inserir endereço IP do gateway"	}	},
	{ TSTR_ENTER_ID					,	{	L"Enter ID",							L"Saisir ID",							L"ID eingeben",							L"Introducir Id.",					 	L"Inserisci ID",						L"Introduzir ID",						L"Inserir ID"	}	},
	{ TSTR_ENTER_ID_TO_CANCEL		,	{	L"Enter ID to Cancel",					L"Saisir ID pour annuler",				L"ID zum Abbrechen eingeben",			L"Introducir ID para cancelar",		 	L"Inserisci ID per annullare",			L"Introduzir ID para cancelar",		  	L"Inserir ID para cancelar"}},
	{ TSTR_ENTER_IP_MASK			,	{	L"Enter Analyzer IP Mask", 				L"Saisir le masque IP de l'analyseur",	L"IP-Maske des Analysegeräts eingeben",	L"Introducir máscara IP analizador",	L"Inserisci ID per annullare",			L"Introduzir máscara IP do analisador",	L"Inserir máscara de IP do analisador"	}	},
	{ TSTR_ENTER_KENNEL_NUMBER		,	{	L"Enter Kennel Number",					L"Saisir n° chenil",					L"Zwingernr. eingeben",					L"Introducir número de caseta",		 	L"Inserisci numero Kennel",				L"Introduzir número de Kennel",		  	L"Inserir número do canil"	}	},
	{ TSTR_ENTER_LAST_VACCINATION	,	{	L"Enter Last Vaccination",				L"Saisir dernière vaccination",			L"Letzte Impfung eingeben",				L"Introducir última vacuna",			L"inserisci ultima vaccinazione",		L"Introduzir última vacinação",		  	L"Inserir última vacinação"	}	},
	{ TSTR_ENTER_LOCATION			,	{	L"Enter Location",						L"Entrer emplacement",					L"Ort eingeben",						L"Introducir ubicación",				L"Inserisci centro",					L"Introduzir localização",			  	L"Inserir local"	}	},
	{ TSTR_ENTER_OPERATOR_ID		,	{	L"Enter Operator ID",					L"Saisir ID opérateur",					L"Bediener-ID eingeben",				L"Introducir Id. del operario",		 	L"Inserisci ID operatore",				L"Introduzir ID do operador",		  	L"Inserir ID do operador"	}	},
	{ TSTR_ENTER_OWNER_ID			,	{	L"Enter Owner ID",						L"Saisir ID propriétaire",				L"Besitzer-ID eingeben",				L"Introducir id. del dueño",			L"Inserisci ID proprietario",			L"Introduzir ID do proprietário",	  	L"Inserir ID do proprietário"	}	},
	{ TSTR_ENTER_PASSWORD			,	{	L"Enter Password",						L"Saisir mot de passe",					L"Passwort eingeben",					L"Introducir contraseña",			 	L"Inserisci password",					L"Introduzir palavra-passe",			L"Inserir senha"	}	},
	{ TSTR_ENTER_PATIENT_ID			,	{	L"Enter Patient ID",					L"Saisir ID patient",					L"Patienten-ID eingeben",				L"Introducir Id. Paciente",			 	L"Inserisci ID paziente",				L"Introduzir ID de doente",			  	L"Inserir ID do paciente"	}	},
	{ TSTR_ENTER_PHONE_NUMBER		,	{	L"Enter Phone Number",					L"Saisir n° de tél",					L"Telefonnummer eingeben",				L"Introducir número de teléfono",	 	L"Inserisci numero di telefono",		L"Introduzir número de telefone",	  	L"Inserir número do telefone"	}	},
	{ TSTR_ENTER_SAMPLE_ID			,	{	L"Enter Sample ID",						L"Saisir ID échantillon",				L"Proben-ID eingeben",					L"Introducir Id. de la muestra",		L"Inserisci ID campione",				L"Introduzir ID da amostra",			L"Inserir ID da amostra"	}	},
	{ TSTR_ENTER_SERVER_IP_ADDRESS	,	{	L"Enter Server IP Address",				L"Saisir l'adresse IP du serveur",		L"Server-IP-Adresse eingeben",		  	L"Introducir dirección IP servidor",	L"Immettere l'indirizzo IP del server",	L"Introduzir endereço IP do servidor", 	L"Inserir endereço IP do servidor"	}	},
	{ TSTR_ENTER_SERVER_SOCKET		,	{	L"Enter Server Socket",					L"Saisir le socket du serveur",			L"Server-Socket eingeben",			  	L"Introducir socket del servidor",	 	L"Immettere il socket del server",		L"Introduzir tomada do servidor",	  	L"Inserir soquete do servidor"	}	},
	{ TSTR_ENTER_START_DATE			,	{	L"Enter Start Date",					L"Saisir la date de début",				L"Anfangsdatum eingeben",			  	L"Escriba fecha inicial",			 	L"Inserisci data inizio",				L"Introduzir data inicial",			  	L"Inserir data de início"	}	},
	{ TSTR_ENZYMES_UNITS			,	{	L"Enzymes Units",						L"Unités enzymes",						L"Enzymeinheiten",					  	L"Unidades de enzimas",				 	L"Unità enzimi",						L"Unidades de enzimas",				  	L"Unidades de enzimas"	}	},
	{ TSTR_ERROR					,	{	L"ERROR",								L"ERREUR",								L"FEHLER",							  	L"ERROR",							 	L"ERRORE",								L"ERRO",								L"ERRO"	}	},
	{ TSTR_FACTORY_DEFAULT_SPECIES	,	{	L"Factory Default Demographics",		L"Paramètres par défaut élts démogr.",	L"Voreingestellte Demographien",		L"Demografía predeterminada",		 	L"Demografia predefinita di fabbrica",	L"Valores de demografia de fábrica",	L"Dados demog. padrão de fábrica"	}	},
	{ TSTR_FACTORY_DEFAULTS			,	{	L"Factory Defaults",					L"Paramètres par défaut en usine",		L"Werkseinstellungen",				  	L"Valores predeterminados",			 	L"Impostazioni di fabbrica",			L"Valores de fábrica",				  	L"Padrões de fábrica"	}	},
	{ TSTR_GROUP_UNITS				,	{	L"Group Units",							L"Unités groupe",						L"Gruppeneinheiten",					L"Unidades de grupo",				 	L"Unità gruppo",						L"Unidades de grupo",				  	L"Unidades do grupo"	}	},
	{ TSTR_HL7_CONFIG_SELECT		,	{	L"HL7 Configuration",					L"Configuration HL7",					L"HL7-Konfiguration",					L"Configuración HL7",					L"Configurazione HL7",					L"Configuração HL7",					L"Configuração do HL7"  } },
	{ TSTR_HL7_PHYSICAL_LAYER		,	{	L"HL7 Settings",						L"Paramètres HL7",						L"HL7-Einstellungen",					L"Ajustes HL7",							L"Impostazioni HL7",					L"Conjuntos HL7",						L"Configurações do HL7" } },
	{ TSTR_HL7_PROTOCOL_SET		    ,	{	L"HL7 Protocol Set",					L"Ensemble de protocol HL7",			L"HL7-Protokoll eingestellt",			L"Conjunto de protocolos HL7",			L"Set protocollo HL7",					L"Conjunto de protocolos HL7",			L"Conjunto de protocolos HL7" } },
	{ TSTR_HOME						,	{	L"Home",								L"Page d'accueil",						L"Startseite",						  	L"Inicio",							 	L"Home",								L"Início",							  	L"Início"	}	},
	{ TSTR_INCLUDE_REF_RANGES		,	{	L"Include Reference Ranges",			L"Inclure fourchettes de référence",	L"Referenzbereiche angeben",			L"Incluir intervalos de referencia",	L"Includi range di riferimento",		L"Incluir intervalos de referência",	L"Incluir intervalos de referência"	}	},
	{ TSTR_INFORMATION				,	{	L"Information",							L"Information",							L"Information",						  	L"Información",						 	L"Informazioni",						L"Informação",						  	L"Informações"	}	},
	{ TSTR_INSERT_ROTOR				,	{	L"Insert Disc",							L"Insérer disque",						L"Scheibe einsetzen",				  	L"Insertar disco",					 	L"Inserisci disco",						L"Inserir disco",					  	L"Inserir disco"	}	},
	{ TSTR_INSERT_SAMPLE			,	{	L"Insert Sample",						L"Insérer échantillon",					L"Probe einsetzen",					  	L"Insertar muestra",					L"Inserisci campione",					L"Inserir amostra",					  	L"Inserir amostra"	}	},
	{ TSTR_INTERNAL_ARCHIVES		,	{	L"Internal Archives",					L"Archives internes",					L"Interne Archive",					  	L"Archivos internos",				 	L"Archivi interni",						L"Ficheiros internos",				  	L"Arquivos internos"	}	},
	{ TSTR_LIPIDS_UNITS				,	{	L"Lipids Units",						L"Unités lipides",						L"Lipideinheiten",					  	L"Unidades de lípidos",				 	L"Unità lipidi",						L"Unidades de lípidos",				  	L"Unidades de lipídios"	}	},
	{ TSTR_LOAD_CD					,	{	L"Load CD",								L"Charger CD",							L"CD laden",							L"Cargar CD",						 	L"Carica CD",							L"Carregar CD",						  	L"Carregar CD"	}	},
	{ TSTR_LOAD_CONTROLS_CD			,	{	L"Load Controls CD",					L"Charger CD commandes",				L"CD-Steuerung laden",				  	L"Cargar controles CD",				 	L"Carica CD di controllo",				L"Carregar CD de controlos",			L"Carregar CD de controles"	}	},
	{ TSTR_LOAD_ORDER				,	{	L"Load Order",							L"Charger ordre",						L"Anforderung laden",				  	L"Cargar pedido",					 	L"Carica ordine",						L"Carregar pedido",					  	L"Carregar solicitação"	}	},
	{ TSTR_LOAD_ROTOR				,	{	L"Load Disc",							L"Charger disque",						L"Scheibe laden",					  	L"Cargar disco",						L"Carica disco",						L"Carregar disco",					  	L"Carregar disco"	}	},
	{ TSTR_LOCKOUT_SETTINGS			,	{	L"Lockout Settings",					L"Paramètres verrouillage",				L"Lookout-Eimstellungen",			  	L"Confguración de bloqueo",			 	L"Impostazioni del blocco",				L"Configurações de bloqueio",		  	L"Configurações de bloqueio"	}	},
	{ TSTR_MANAGE_ADMINISTRATORS	,	{	L"Manage Administrators",				L"Gestion administrateurs",				L"Administratoren verwalten",		  	L"Gestionar administradores",		 	L"Gestisci amministratori",				L"Gerir administradores",			  	L"Gerenciar administradores"	}	},
	{ TSTR_MANAGE_OPERATORS			,	{	L"Manage Operators",					L"Gestion opérateurs",					L"Bediener verwalten",				  	L"Gestionar operarios",				 	L"Gestisci operatori",					L"Gerir operadores",					L"Gerenciar operadores"	}	},
	{ TSTR_MINERALS_UNITS			,	{	L"Minerals Units",						L"Unités minérales",					L"Mineralieneinheiten",				  	L"Unidades de minerales",			 	L"Unità minerali",						L"Unidades de minerais",				L"Unidades de minerais"	}	},
	{ TSTR_MODIFY_REF_RANGES		,	{	L"Modify Reference Range",				L"Modifier fourchette de référence",	L"Referenzbereich ändern",			  	L"Modificar intervalo de referencia", 	L"Modifica range di riferimento",		L"Alterar intervalo de referência",	  	L"Modificar intervalo de referência"	}	},
	{ TSTR_NEW_ADMINISTRATOR_ID		,	{	L"New Administrator ID",				L"Nouvel ID administrateur",			L"Neue Administratoren-ID",			  	L"Nueva Id. del administrador",		 	L"Nuova ID amministratore",				L"Nova ID do administrador",			L"Novo ID do administrador"	}	},
	{ TSTR_NEW_OPERATOR_ID			,	{	L"New Operator ID",						L"Nouvel ID opérateur",					L"Neue Bediener-ID",					L"Nueva Id. del operario",				L"Nuova ID operatore",					L"Nova ID do operador",				  	L"ID do novo operador"	}	},
	{ TSTR_NO_RESULTS				,	{	L"No Results",							L"Aucun résultat",						L"Keine Ergebnisse",					L"No hay resultados",					L"Nessun risultato",					L"Sem resultados",					  	L"Nenhum resultado"	}	},
	{ TSTR_NUM_ERROR_COPIES			,	{	L"# Error Copies",						L"Nbre copies erronées",				L"# Fehler-Kopien",					  	L"N° de copias de error",				L"N. Copie errore",						L"N.° de cópias de erro",			  	L"No. de cópias de erro"	}	},
	{ TSTR_NUM_IQC_COPIES			,	{	L"# iQC Copies",						L"Nbre copies iQC",						L"# iQC-Kopien",						L"N° de copias iQC",					L"N. Copie iQC",						L"N.° de cópias do iQC",				L"No. de cópias de iQC"	}	},
	{ TSTR_NUM_RESULTS_COPIES		,	{	L"# Results Copies",					L"Nbre copie résultats",				L"# Ergebniskopien",					L"N° de copias de resultados",			L"N. Copie risultati",					L"N.° de cópias de resultados",		  	L"No. de cópias dos resultados"	}	},
	{ TSTR_OPERATOR					,	{	L"Operator",							L"Opérateur",							L"Bediener",							L"Operario",							L"Operatore",							L"Operador",							L"Operador"	}	},
	{ TSTR_OPERATOR_EXPIRATION		,	{	L"Operator Expiration",					L"Expiration opérateur",				L"Ungültiger Bediener",				  	L"Vencimiento del operario",			L"Scadenza operatore",					L"Prazo de validade do operador",	  	L"Expiração do operador"	}	},
	{ TSTR_OPERATOR_ID				,	{	L"Operator ID",							L"ID opérateur",						L"Bediener-ID",						  	L"Id. del operario",					L"ID operatore",						L"ID do operador",					  	L"ID do operador"	}	},
	{ TSTR_PATIENT					,	{	L"Patient:",							L"Patient :",							L"Patient:",							L"Paciente:",							L"Paziente:",							L"Doente:",							  	L"Paciente:"	}	},
	{ TSTR_PATIENT_BY_DATE			,	{	L"Patient Results Sorted by Date",		L"Résultats patients triés par date",	L"Patientenergebnisse nach Datum",	  	L"Resultados paciente por fecha",		L"Risultati paz. ordinati per data",	L"Resultados do doente por data",	  	L"Result. paciente classif. por data"	}	},
	{ TSTR_PENDING_ORDERS			,	{	L"Pending Orders",						L"Ordres en cours",					  	L"Ausstehende Anforderungen",		  	L"Pedidos pendientes",					L"Ordini in sospeso",					L"Pedidos pendentes",				  	L"Solicitações pendentes"	}	},
	{ TSTR_POWER_SAVE_WAIT			,	{	L"Power Save Wait",						L"Attente économiseur alim.",		  	L"Energiesparmodus, warten",			L"Espera de ahorro de energía",			L"Attendere Modo risparmio",			L"Espera de poupança de energia",	  	L"Espera de economia de energia"	}	},
	{ TSTR_PRINTER_SETUP			,	{	L"Printer Setup",						L"Configuration imprimante",			L"Druckereinrichtung",				  	L"Configuración impresora",				L"Impostazioni stampante",				L"Configuração da impressora",		  	L"Configuração da impressora"	}	},
	{ TSTR_PROTEINS_UNITS			,	{	L"Proteins Units",						L"Unités protéines",					L"Proteineinheiten",					L"Unidades de proteínas",				L"Unità proteine",						L"Unidades de proteínas",			  	L"Unidades de proteínas"	}	},
	{ TSTR_RANGES					,	{	L"Ranges",								L"Fourchettes",						  	L"Bereiche",							L"Intervalos",							L"Range",								L"Intervalos",						  	L"Intervalos"	}	},
	{ TSTR_READ_RESULTS				,	{	L"Read Results",						L"Lecture résultats",				  	L"Ergebnisse auslesen",				  	L"Leer resultados",						L"Leggi risultati",						L"Ler resultados",					  	L"Ler resultados"	}	},
	{ TSTR_RECALL					,	{	L"Recall",								L"Rappel",							  	L"Wiederaufruf",						L"Recordar",							L"Richiama",							L"Recordar",							L"Recuperar"	}	},
	{ TSTR_REMOVE_ROTOR				,	{	L"Remove Disc",							L"Retirer disque",					  	L"Scheibe entfernen",				  	L"Retirar disco",						L"Rimuovi disco",						L"Retirar disco",					  	L"Remover disco"	}	},
	{ TSTR_REMOVE_SPECIES			,	{	L"Remove Demographic",					L"Retirer éléments démographiques",	  	L"Demographien entfernen",			  	L"Eliminar demografía",					L"Rimuovi demografia",					L"Eliminar demografia",				  	L"Remover dados demográficos"	}	},
	{ TSTR_RETRIEVE_REF_RANGES		,	{	L"Retrieve Reference Ranges",			L"Récupérer fourchettes de référence", 	L"Referenzbereiche empfangen",		  	L"Recuperar intervalos de referencia",	L"Recupera range di riferimento",		L"Recuperar intervalos de referência", 	L"Recuperar intervalos de referência"	}	},
//	{ TSTR_SAMPLE					,	{	L"Sample",								L"Échantillon",						  	L"Probe",							  	L"Muestra",								L"Campione",							L"Amostra",							  	L"Amostra"	}	},
	{ TSTR_SCREEN_SAVER_WAIT		,	{	L"Screen Saver Wait",					L"Attente économiseur d'écran",		  	L"Bildschirmschoner, warten",		  	L"Espera del salvapantalla",			L"Attendere salvaschermo",				L"Espera de poupança do ecrã",		  	L"Espera de proteção de tela"	}	},
	{ TSTR_SEARCH					,	{	L"Search",								L"Rechercher",						  	L"Suchen",							  	L"Búsqueda",							L"Cerca",								L"Pesquisar",						  	L"Pesquisar"	}	},
	{ TSTR_SEARCH_ADMINISTRATORS	,	{	L"Search Administrators",				L"Rechercher administrateurs",		  	L"Administratoren suchen",			  	L"Buscar administradores",				L"Cerca amministratori",				L"Pesquisar administradores",		  	L"Pesquisar administradores"	}	},
	{ TSTR_SEARCH_OPERATORS			,	{	L"Search Operators",					L"Rechercher opérateurs",			  	L"Bediener suchen",					  	L"Buscar operarios",					L"Cerca operatori",						L"Pesquisar operadores",				L"Pesquisar operadores"	}	},
	{ TSTR_SECURITY_SETTINGS		,	{	L"Security Settings",					L"Paramètres de sécurité",			  	L"Sicherheitseinstellungen",			L"Configuración de seguridad",			L"Impostazioni sicurezza",				L"Configurações de segurança",		  	L"Configurações de segurança"	}	},
	{ TSTR_SELECT_COOL_ROOM_TEMP 	,	{	L"Room Temp < 27°C Setting",			L"Réglage temp. ambiante < 27°C",		L"Raumtemp.-Einstellung < 27°C",		L"Ajuste de temp. ambiente < 27°C",		L"Impostazione temp amb < 27°C",		L"Config. da temp. ambiente < 27°C",	L"Temperatura ambiente < 27°C" } },
	{ TSTR_SELECT_DISPLAY_LANG		,	{	L"Select Display Language",				L"Sélectionner affich. langue",		  	L"Sprache auswählen",				  	L"Seleccionar idioma",					L"Seleziona lingua display",			L"Selecionar idioma do visor",		  	L"Selecionar idioma de exibição"	}	},
	{ TSTR_SELECT_GENDER			,	{	L"Select Gender",						L"Sélectionner le sexe",				L"Geschlecht auswählen",				L"Seleccionar sexo",					L"Seleziona sesso",						L"Selecionar sexo",					  	L"Selecionar sexo"	}	},
	{ TSTR_SELECT_KEYBOARD			,	{	L"Select Keyboard Type",				L"Sélectionner type de clavier",		L"Tastatur auswählen",				  	L"Seleccionar tipo de teclado",			L"Seleziona tipo tastiera",				L"Selecionar tipo de teclado",		  	L"Selecionar tipo de teclado"	}	},
	{ TSTR_SELECT_PRINTER			,	{	L"Select Printer",						L"Sélectionner imprimante",			  	L"Drucker auswählen",				  	L"Seleccionar impresora",				L"Seleziona stampante",					L"Selecionar impressora",			  	L"Selecionar impressora"	}	},
	{ TSTR_SELECT_PROTOCOL			,	{	L"Select Protocol",						L"Sélectionner un protocole",		  	L"Protokoll auswählen",				  	L"Seleccionar protocolo",				L"Selezionare protocollo",				L"Selecionar protocolo",				L"Selecionar protocolo"	}	},
	{ TSTR_SELECT_RACE				,	{	L"Select Race",							L"Choisir la race",					  	L"Abstammung auswählen",				L"Seleccione raza",						L"Seleziona razza",						L"Selecionar corrida",				  	L"Selecionar raça"	}	},
	{ TSTR_SELECT_REPORT			,	{	L"Select Report",						L"Sélectionner rapport",				L"Bericht auswählen",				  	L"Seleccionar informe",					L"Select rapporto",						L"Selecionar relatório",				L"Selecionar relatório"	}	},
	{ TSTR_SELECT_REPORTS			,	{	L"Select Reports",						L"Sélectionner rapports",			  	L"Berichte auswählen",				  	L"Seleccionar informes",				L"Seleziona rapporti",					L"Selecionar relatórios",			  	L"Selecionar relatórios"	}	},
	{ TSTR_SELECT_SPECIES			,	{	L"Select Demographic",					L"Sélectionner éléments démogr.",	  	L"Demographie auswählen",			  	L"Seleccionar demografía",				L"Seleziona demografia",				L"Selecionar demografia",			  	L"Selecionar dados demográficos"	}	},
	{ TSTR_SELECT_TYPE				,	{	L"Select Type",							L"Sélectionner type",				  	L"Typ auswählen",					  	L"Seleccionar tipo",					L"Seleziona tipo",						L"Selecionar tipo",					  	L"Selecionar tipo"	}	},
	{ TSTR_SEND_REF_RANGES			,	{	L"Send Reference Ranges",				L"Envoyer fourchettes de réf.",		  	L"Referenzbereiche senden",			  	L"Enviar intervalos de referencia",		L"Imposta range di riferimento",		L"Enviar intervalos de referência",	  	L"Enviar intervalos de referência"	}	},
	{ TSTR_SERVICE_ARCHIVE			,	{	L"Service Archive",						L"Archive service",					  	L"Service-Archiv",					  	L"Archivo de servicio",					L"Archivio di servizio",				L"Ficheiro do serviço",				  	L"Arquivo de serviço"	}	},
	{ TSTR_SET_DATE					,	{	L"Set Date",							L"Définir date",						L"Datum einstellen",					L"Establecer fecha",					L"Imposta data",						L"Configurar data",					  	L"Configurar data"	}	},
	{ TSTR_SET_DEFAULT				,	{	L"Set Default",							L"Définir paramètres par défaut",	  	L"Standard einstellen",				  	L"Establecer predeterminados",			L"Imposta come predefinito",			L"Configurar valores predeterminados", 	L"Configurar padrão"	}	},
	{ TSTR_SET_LANGUAGE				,	{	L"Set Language",						L"Définir la langue",				  	L"Sprache einstellen",				  	L"Establecer idioma",					L"Imposta lingua",						L"Configurar idioma",				  	L"Configurar idioma"	}	},
	{ TSTR_SET_PROTOCOL				,	{	L"Set Protocol",						L"Définir le protocole",				L"Protokoll einstellen",				L"Establecer protocolo",				L"Imposta protocollo",					L"Configurar protocolo",				L"Configurar protocolo"	}	},
	{ TSTR_SET_TIME					,	{	L"Set Time",							L"Définir l'heure",					  	L"Zeit einstellen",					  	L"Establecer hora",						L"Imposta tempo",						L"Configurar hora",					  	L"Configurar hora"	}	},
	{ TSTR_SET_UNITS				,	{	L"Set Units",							L"Définir les unités",				  	L"Einheiten einstellen",				L"Establecer unidades",					L"Imposta unità",						L"Configurar unidades",				  	L"Configurar unidades"	}	},
	{ TSTR_SINGLE_ANALYTE			,	{	L"Single Analyte",						L"Une seule substance à analyser",	  	L"Einzelnes Analyt",					L"Analito único",						L"Analita singolo",						L"Analito único",					  	L"Analito único"	}	},
	{ TSTR_SOFTWARE_UPDATE			,	{	L"Software Update",						L"Mise à jour logiciel",				L"Software-Update",					  	L"Actualización del software",			L"Aggiornamento software",				L"Atualização do software",			  	L"Atualização de software"	}	},
	{ TSTR_SOUND_SETTINGS			,	{	L"Sound Settings",						L"Paramètres sonores",				  	L"Sound-Einstellungen",				  	L"Configuración de sonido",				L"Impostazioni audio",					L"Configurações do som",				L"Configurações do som"	}	},
	{ TSTR_SPECIES					,	{	L"Demographic",							L"Éléments démographiques",			  	L"Demographie",						  	L"Demografía",							L"Demografia",							L"Demografia",						  	L"Dados demográficos"	}	},
	{ TSTR_SPECIES_AVAILABLE		,	{	L"Demographics Available",				L"Éléments démographiques disponibles",	L"Verfügbare Demographien",			  	L"Demografías disponibles",				L"Demografia disponibile",				L"Demografia disponível",			  	L"Dados demográficos disponíveis"	}	},
	{ TSTR_STATUS_VOLUME			,	{	L"Status Volume",						L"État volume",						  	L"Mengenstatus",						L"Volumen de estado",					L"Volume di stato",						L"Volume de estado",					L"Volume do status"	}	},	
	{ TSTR_TCPIP			        ,	{	L"TCP/IP",								L"TCP/IP",								L"TCP/IP",						        L"TCP/IP",								L"TCP/IP",								L"TCP/IP",								L"TCP/IP" } },
	{ TSTR_TCPIP_ADDRESS_TYPE		,	{	L"TCP/IP Address Type",					L"Type d’adresse TCP/IP",				L"TCP/IP Adresstyp",					L"Tipo de dirección TCP/IP",			L"Tipo indirizzi TCP/IP",				L"Tipo de endereço TCP/IP",				L"Tipo de endereço TCP/IP" } },
	{ TSTR_TCPIP_SETTINGS			,	{	L"TCP/IP Settings",						L"Paramètres TCP/IP",				  	L"TCP/IP-Einstellungen",				L"Configuración TCP/IP",				L"Impostazioni TCP/IP",					L"Configurações de TCP/IP",			  	L"Configurações de TCP/IP"	}	},
	{ TSTR_TRANSMIT_ALL				,	{	L"Transmit All",						L"Transmettre tout",					L"Alle übertragen",					  	L"Transmitir todo",						L"Trasmetti tutti",						L"Transmitir tudo",					  	L"Transmitir todos"	}	},
	{ TSTR_TRANSMIT_DATE_RANGE		,	{	L"Transmit Date Range",					L"Transmettre la plage de dates",	  	L"Datumsbereich übertragen",			L"Transmitir intervalo de fechas",		L"Trasmetti range di date",				L"Transmitir intervalo de data",		L"Transmitir intervalo de datas"	}	},
	{ TSTR_UNITS					,	{	L"Units",								L"Unités",							  	L"Einheiten",						  	L"Unidades",							L"Unità",								L"Unidades",							L"Unidades"	}	},
	{ TSTR_VIEW_RESULT				,	{	L"View Result",							L"Afficher résultat",				  	L"Ergebnis ansehen",					L"Ver resultado",						L"Visualizza risultati",				L"Visualizar resultado",				L"Visualizar resultado"	}	},
	{ TSTR_WARNING					,	{	L"WARNING",								L"MISE EN GARDE",					  	L"ACHTUNG",							  	L"AVISO",								L"ATTENZIONE",							L"ADVERTÊNCIA",						  	L"AVISO"	}	},

// Middle (Long Button Stings > 35 Characters Unlikely to Display Correctly)
//											ENGLISH,																			FRENCH,																											GERMAN,																												SPANISH,																								ITALIAN, 																											PORTUGUESE,																									PORTUGUESE (BRAZIL)
//											          11111111112222222222233333												          11111111112222222222233333																			          11111111112222222222233333																				          11111111112222222222233333																	          11111111112222222222233333																				          11111111112222222222233333																		          11111111112222222222233333																				          	
//											 12345678901234567890123456789012345												 12345678901234567890123456789012345																			 12345678901234567890123456789012345																				 12345678901234567890123456789012345																	 12345678901234567890123456789012345																				 12345678901234567890123456789012345																		 12345678901234567890123456789012345	
	{ MSTR_ADD_ADMINISTRATOR		,	{	L"Add Administrator",																L"Ajouter administrateur",																						L"Admin. hinzufügen",																								L"Añadir administrador",																				L"Agg. amministratore",																								L"Adicionar administrador",																					L"Adicionar administrador"	}	},
	{ MSTR_ADD_CONTROL_VALUES		,	{	L"Add Control Values",																L"Ajouter valeurs de contrôle",																					L"Steuerungswerte hinzufügen",																						L"Añadir valores de control",																			L"Aggiungi valori di controllo",																					L"Adicionar valores de controlo",																			L"Adicionar valores de controle"	}	},
	{ MSTR_ADD_OPERATOR				,	{	L"Add Operator",																	L"Ajouter opérateur",																							L"Bediener hinzufügen",																								L"Añadir operario",																						L"Aggiungi operatore",																								L"Adicionar operator",																						L"Adicionar operador"	}	},
	{ MSTR_ADD_SPECIES				,	{	L"Add Demographic",																	L"Ajouter éléments démographiques",																				L"Demographie hinzufügen",																							L"Añadir demografía",																					L"Aggiungi demografia",																								L"Adicionar demografia",																					L"Adicionar dados demográficos"	}	},
	{ MSTR_ADDRESS_TYPE				,	{	L"Address Type",																	L"Type d’adresse TCP/IP",																						L"TCP/IP Adresstyp",																								L"Tipo de dirección TCP/IP",																			L"Tipo indirizzi TCP/IP",																							L"Tipo de endereço",																						L"Tipo de endereço" } },
	{ MSTR_ADMISSION_ID				,	{	L"Admission ID",																	L"ID admission",																								L"Zugangs-ID",																										L"Id. admisión",																						L"ID ammissione",																									L"ID de Admissão",																							L"ID de admissão"	}	},
	{ MSTR_ADMINISTRATOR_ID			,	{	L"Administrator ID",																L"ID administrateur",																							L"Administrator-ID",																								L"Id. del administrador",																				L"ID amministratore",																								L"ID do administrador",																						L"ID do administrador"	}	},
	{ MSTR_ADMINISTRATORS			,	{	L"Administrators",																	L"Administrateurs",																								L"Administratoren",																									L"Administradores",																						L"Amministratori",																									L"Administradores",																							L"Administradores"	}	},
	{ MSTR_AGE						,	{	L"Age",																				L"Age",																											L"Alter",																											L"Edad",																								L"Età",																												L"Idade",																									L"Idade"	}	},
	{ MSTR_ALERTS					,	{	L"Alerts",																			L"Avertisseurs",																								L"Warnungen",																										L"Alertas",																								L"Allarmi",																											L"Alertas",																									L"Alertas"	}	},
	{ MSTR_ALL						,	{	L"All",																				L"Tous",																										L"Alle",																											L"Todo",																								L"Tutti",																											L"Tudo",																									L"Todos"	}	},
	{ MSTR_ALL_ANALYTES				,	{	L"All Analytes",																	L"Toutes subs. à analyser",																						L"Alle Analyte",																									L"Todos los analitos",																					L"Tutti gli analiti",																								L"Todos os analitos",																						L"Todos os analitos"	}	},
	{ MSTR_ALTERNATE_ID				,	{	L"Alternate ID",																	L"Autre ID",																									L"ID ändern",																										L"Id. suplente",																						L"ID alternativo",																									L"ID suplente",																								L"ID alternativo"	}	},
	{ MSTR_ALWAYS					,	{	L"Always",																			L"Toujours",																									L"Immer",																											L"Siempre",																								L"Sempre",																											L"Sempre",																									L"Sempre"	}	},
	{ MSTR_ANALYSIS_COMPLETE		,	{	L"Analysis Complete",																L"Analyse terminée",																							L"Analyse abgeschlossen",																							L"Análisis completado",																					L"Analisi completa",																								L"Análise concluída",																						L"Análise concluída"	}	},
	{ MSTR_ANALYTE					,	{	L"Analyte",																			L"Substance à analyser",																						L"Analyt",																											L"Analito",																								L"Analita",																											L"Analito",																									L"Analito"	}	},
	{ MSTR_ANALYTE_GROUPS			,	{	L"Analyte Groups",																	L"Gr. subst. à analyser",																						L"Analytgruppen",																									L"Grupos analito",																						L"Gruppi analita",																									L"Grupos de analito",																						L"Grupos de analitos"	}	},
	{ MSTR_ANALYTE_OPTIONS			,	{	L"Analyte Options",																	L"Options à analyser",																							L"Analytoptionen",																									L"Opciones de analito",																					L"Opzioni analita",																									L"Opções de analito",																						L"Opções de analito"	}	},
	{ MSTR_ANALYZE					,	{	L"Analyze",																			L"Analyser",																									L"Analysieren",																										L"Analizar",																							L"Analizzare",																										L"Analisar",																								L"Analisar"	}	},
	{ MSTR_ANALYZER_IP				,	{	L"Analyzer IP",																		L"IP de l'analyseur",																							L"Analysegerät-IP",																									L"IP del analizador",																					L"IP analizzatore",																									L"IP do analisador",																						L"IP do analisador"	}	},
	{ MSTR_ANALYZER_NAME			,	{	L"Analyzer Name",																	L"Nom de l’analyseur",																							L"Name des Analysegeräts",																							L"Nombre del analizador",																				L"Nome analizzatore",																								L"Nome do analisador",																						L"Nome do analisador" } },
	{ MSTR_ANALYZER_PRINTER			,	{	L"Analyzer Printer",																L"Imprimte analyseur",																							L"Drucker-Analysege.",																								L"Impres. Analiz.",																						L"Stamp. Analizzatore",																								L"Impressora do analisador",																				L"Impressora do analisador"	}	},
	{ MSTR_ANALYZING_SAMPLE			,	{	L"Analyzing Sample...",																L"Analyse éch. en cours...",																					L"Probe wird analysiert ...",																						L"Analizando muestra...",																				L"Esame camp. in corso",																							L"A analisar amostra...",																					L"Analisando amostra..."	}	},
	{ MSTR_ASCII_TEXT				,	{	L"ASCII Text",																		L"Texte ASCII",																									L"ASCII-Text",																										L"Texto ASCII",																							L"Testo ASCII",																										L"Texto ASCII",																								L"Texto ASCII"	}	},
	{ MSTR_ASIAN_PACIFIC_ISLANDER	,	{	L"Asian/Pacific Islander",															L"Asiat./orig. îles Pac.",																						L"Asiatisch/Pazifisch",																								L"Asiático/Isl. Pacífico",																				L"Isolano Asia/Pacifico",																							L"Ilhas Asiáticas/do Pacífico",																				L"Asiático/ilhas do Pacífico"	}	},
	{ MSTR_ASTM_1394_7				,	{	L"ASTM 1394-97",																	L"ASTM 1394-97",																								L"ASTM 1394-97",																									L"ASTM 1394-97",																						L"ASTM 1394-97",																									L"ASTM 1394-97",																							L"ASTM 1394-97"	}	},
	{ MSTR_ASTM_ORDERS				,	{	L"ASTM Orders",																		L"Ordres ASTM",																									L"ASTM Anforderungen",																								L"Pedidos ASTM",																						L"Ordini ASTM",																										L"Pedidos ASTM",																							L"Solicitações ASTM"	}	},
	{ MSTR_AUTO						,	{	L"Auto",																			L"Auto",																										L"Automat.",																										L"Auto",																								L"Auto",																											L"Auto",																									L"Auto"	}	},
	{ MSTR_BACKLIGHT				,	{	L"Backlight",																		L"Rétroéclairage", 																								L"Hintergrundbeleucht.",																							L"Luz",																									L"Retroilluminazione",																								L"Luz de fundo",																							L"Luz de fundo"	}	},
	{ MSTR_BASE_TEST				,	{	L"Base Test",																		L"Test de base",																								L"Basistest",																										L"Prueba base",																							L"Test base",																										L"Teste base",																								L"Teste de base"	}	},
	{ MSTR_BLACK					,	{	L"Black",																			L"Noire",																										L"Schwarz",																											L"Negro",																								L"Nero",																											L"Negro",																									L"Preto"	}	},
	{ MSTR_BOTH						,	{	L"Both",																			L"Les deux", 																									L"Beides",																											L"Ambos",																								L"Entrambe",																										L"Ambos",																									L"Ambos"	}	},
	{ MSTR_BROWSE					,	{	L"Browse",																			L"Naviguer", 																									L"Durchsuchen",																										L"Examinar",																							L"Sfoglia",																											L"Navegar",																									L"Procurar"	}	},
	{ MSTR_CALIBRATE				,	{	L"Calibrate",																		L"Calibrer", 																									L"Kalibrieren",																										L"Calibrar",																							L"Calibra",																											L"Calibrar",																								L"Calibrar"	}	},
	{ MSTR_CLIENT_SOCKET			,	{	L"Client Socket",																	L"Socket du client", 																							L"Client-Socket",																									L"Socket del cliente",																					L"Socket client",																									L"Tomada do cliente",																						L"Soquete do cliente" } },
	{ MSTR_CAM_CYCLES				,	{	L"Cam Cycles",																		L"Cycles cam", 																									L"Nockenzyklen",																									L"Ciclos de la leva",																					L"Cicli camma",																										L"Ciclos Cam",																								L"Ciclos da cam"	}	},
	{ MSTR_CANCEL_ANALYSIS			,	{	L"Cancel Analysis?",																L"Annuler analyse ?", 																							L"Analyse abbrechen?",																								L"¿Cancelar análisis?",																					L"Annullare analisi?",																								L"Cancelar análise?",																						L"Cancelar análise?"	}	},
	{ MSTR_CANCEL_ARCHIVE			,	{	L"Cancel Archive?",																	L"Annuler archive ?", 																							L"Archiv stornieren?",																								L"¿Cancelar archivo?",																					L"Annulla archivio?",																								L"Cancelar arquivo?",																						L"Cancelar arquivo?"	}	},
	{ MSTR_CANCEL_ORDER				,	{	L"Cancel Order",																	L"Annuler l'ordre",																								L"Anforderung abbrechen",																							L"Cancelar pedido",																						L"Annulla ordine",																									L"Cancelar pedido",																							L"Cancelar solicitação"	}	},
	{ MSTR_CANCEL_TRANSMISSION		,	{	L"Cancel Transmission?",															L"Annuler transmission ?", 																						L"Übertragung abbrechen?",																							L"¿Cancelar transmisión?",																				L"Annullare trasmissione?",																							L"Cancelar transmissão?",																					L"Cancelar transmissão?"	}	},
	{ MSTR_CANCELED					,	{	L"Canceled",																		L"Annulé", 																										L"Abgebrochen",																										L"Cancelado",																							L"Annullato",																										L"Cancelado",																								L"Cancelado"	}	},
	{ MSTR_CANCELING_ANALYSIS		,	{	L"Canceling analysis, please wait",													L"Annulation analyse en cours, veuillez patienter",																L"Analyse wird abgebrochen, bitte warten",																			L"Cancelando análisis. Espere, por favor.",																L"Annullamento analisi, attendere",																					L"A cancelar análise, espere por favor",																	L"Cancelando a análise, aguarde"	}	},
	{ MSTR_CHANGE_EXPIRATION		,	{	L"Change Expiration",																L"Modifier expiration",																							L"Ablaufdatum ändern",																								L"Cambiar vencimiento",																					L"Modifica scadenza",																								L"Alterar prazo de validade",																				L"Alterar expiração"	}	},
	{ MSTR_CHANGE_NET_NAME		    ,	{	L"Changing network name, please wait",							                    L"Modification du nom du réseau, veuillez patienter",															L"Netzwerkname wird geändert, bitte warten",																		L"Cambiando nombre de red; espere",																		L"Modifica nome rete, attendere",																					L"A alterar o nome da rede; espere por favor",																L"Alterando nome da rede, aguarde" } },
	{ MSTR_CHANGE_NET_TYPE		    ,	{	L"Changing network address type, please wait",							            L"Modification du type d’adresse du réseau, veuillez patienter",												L"Netzwerkadresstyp wird geändert, bitte warten",																	L"Cambiando tipo de dirección de red; espere",															L"Modifica tipo indirizzo di rete, attendere",																		L"A alterar o endereço da rede; espere por favor",															L"Alterando tipo de endereço de rede, aguarde" } },
	{ MSTR_CHANGE_PASSWORD			,	{	L"Change Password",																	L"Modifier mot de passe",																						L"Passwort ändern",																									L"Cambiar contraseña",																					L"Modifica password",																								L"Alterar palavra-passe",																					L"Alterar senha"	}	},
	{ MSTR_CHANGE_SETTINGS			,	{	L"Change Settings",																	L"Modifier les paramètres",																						L"Einstellungen ändern",																							L"Cambiar configuración",																				L"Modifica impostazioni",																							L"Alterar configurações",																					L"Alterar configurações"	}	},
	{ MSTR_CHECK_SETTINGS			,	{	L"Check Settings",																	L"Vérifier les paramètres",																						L"Einstell. Üperprüfen",																							L"Comprobar config.",																					L"Verifica impostazioni",																							L"Verificar configurações",																					L"Verificar configurações"	}	},
	{ MSTR_CLEAR					,	{	L"Clear",																			L"Supprimer",																									L"Freigeben",																										L"Borrar",																								L"Pulisci",																											L"Eliminar",																								L"Limpar"	}	},
	{ MSTR_CLINICAL_ARCHIVE			,	{	L"Clinical Archive",																L"Archive clinique",																							L"Klinisches Archiv",																								L"Archivo clínico",																						L"Archivio clinico",																								L"Ficheiro clínico",																						L"Arquivo clínico"	}	},
	{ MSTR_CLINICAL_ARCHIVE_ELLIPSIS,	{	L"Clinical Archive...",																L"Archive clinique...",																							L"Klinisches Archiv...",																							L"Archivo clínico..",																					L"Archivio clinico...",																								L"Ficheiro clínico...",																						L"Arquivo clínico..."	}	},
	{ MSTR_CLOSE_DRAWER_TO_ANALYZE	,	{	L"Close drawer to analyze a sample",												L"Fermer tiroir pour analyser un échantillon",																	L"Zur Probenanalyse Fach schließen",																				L"Cerrar el cajón para analizar una muestra",															L"Chiudi cassetto per analizzare campione",																			L"Fechar a gaveta para analisar uma amostra",																L"Feche a gaveta para analisar uma amostra"	}	},
	{ MSTR_CLOSING_DRAWER			,	{	L"Closing Drawer...",																L"Fermeture tiroir en cours...",																				L"Fach wird geschlossen ...",																						L"Cerrando cajón...",																					L"Chiusura cassetto...",																							L"A fechar a gaveta...",																					L"Fechando gaveta..."	}	},
	{ MSTR_CONFIGURE				,	{	L"Configure",																		L"Configurer",																									L"Konfigurieren",																									L"Config.",																								L"Configura",																										L"Configurar",																								L"Configurar"	}	},
	{ MSTR_CONFIRM_TRANSMIT_ALL		,	{	L"Press Yes to transmit all results saved in the analyzer",							L"App. Oui pour trans. tous résultats enreg. ds analyseur",														L"\"Ja\", um alle im Analysegerät gesp. Erg. zu übertrag.",															L"Pulse Sí para transmitir todos los resultados guardados en el analizador",							L"Premere Sì per trasmettere tutti i risultati savalti nell'analizzatore",											L"Prima Sim para transmitir todos os resultados guardados no analisador",									L"Pressione Sim para transmitir todos os resultados salvos no analisador"	}	},
	{ MSTR_CONNECTION_FAILED		,	{	L"Connection failed",																L"Échec de la connexion",																						L"Verbindung nicht hergestellt",																					L"Falló conexión",																						L"Connessione non riuscita",																						L"Falha de ligação",																						L"Falha na conexão"	}	},
	{ MSTR_CONNECTION_SUCCESSFUL	,	{	L"Connection successful",															L"Connexion établie",																							L"Verbindung hergestellt",																							L"Conexión establecida",																				L"Connessione riuscita",																							L"Ligação estabelecida",																					L"Conexão bem-sucedida"	}	},
	{ MSTR_CONTROLS					,	{	L"Controls",																		L"Contrôles",																									L"Steuerungen",																										L"Controles",																							L"Controlli",																										L"Controlos",																								L"Controles"	}	},
	{ MSTR_COOL_ROOM_TEMP			,	{	L"Fan Speed",																		L"Vitesse du ventilateur",																						L"Ventilatordrehzahl",																								L"Velocidad del ventilador",																			L"Velocità ventola",																								L"Velocidade da ventoinha",																					L"Velocidade do ventilador" } },
    { MSTR_COOL_ROOM_TEMP_ALERT		,	{	L"This setting lowers the idle fan speed reducing background noise for climate controlled laboratories. This setting may be used in a laboratory with the room temperature below 27°C",				L"Ce réglage réduit la vitesse du ventilateur et le bruit de fond dans les laboratoires climatisés. Il peut être utilisé dans un laboratoire dont la température ambiante est inférieure à 27°C",							L"Diese Einstellung reduziert die Leerlaufdrehzahl des Lüfters und somit das Hintergrundrauschen in klimageregelten Laboren. Sie eignet sich für Raumtemp. von < 27°C",															L"Disminuye velocidad del ventilador en períodos inactividad reduciendo el ruido de fondo de labs con ambiente controlado. Se puede usar en laboratorios con temp. < 27°C",						L"Tale impostazione riduce la velocità al minimo della ventola e il rumore di fondo per i laboratori a clima controllato. Può essere utilizzata in un laboratorio con temp. ambiente < 27°C",						L"Esta configuração diminui a velocidade da ventoinha durante períodos de inatividade reduzindo o ruído de fundo em laboratórios com condições ambientais controladas. Esta configuração pode ser utilizada num laboratório com uma temperatura ambiente inferior a 27 °C",						L"Essa configuração reduz a velocidade do ventilador ocioso, reduzindo o ruído de fundo para laboratórios climatizados. Essa configuração pode ser usada em um laboratório com temperatura ambiente abaixo de 27 °C"  } },
	{ MSTR_CURRENT					,	{	L"Current",																			L"Actuel",																										L"Laufend",																											L"Actual",																								L"Attuale",																											L"Atual",																									L"Atual"	}	},
	{ MSTR_DATA_ENTRY_OPTIONS		,	{	L"Data Entry Options",																L"Saisie des données",																							L"Dateneingabeoptionen",																							L"Opc. entrada datos",																					L"Opzioni imm. dati",																								L"Opções de entrada de dados",																				L"Opções de inserção de dados"	}	},
	{ MSTR_DATE						,	{	L"Date",																			L"Date",																										L"Datum",																											L"Fecha",																								L"Data",																											L"Data",																									L"Data"	}	},
	{ MSTR_DATE_OF_BIRTH			,	{	L"Date of Birth (DOB)",																L"Date de naissance (DOB)",																						L"Geburtsdatum (GD)",																								L"Fecha de nacimiento",																					L"Data di nascita (DN)",																							L"Data de nascimento (DDN)",																				L"Data de nascimento (DN)"	}	},
	{ MSTR_DATE_OF_VACCINATION		,	{	L"Last Vaccination (DOV)",															L"Dernier vaccin (DOV)",																						L"Letzte Impfung (LI)",																								L"Última vacuna",																						L"Ultima vaccinazione (DV)",																						L"Última vacinação (DDV)",																					L"Última vacinação (Data de vacin.)"	}	},
	{ MSTR_DAY						,	{	L"Day",																				L"Jour",																										L"Tag",																												L"Día",																									L"Giorno",																											L"Dia",																										L"Dia"	}	},
	{ MSTR_DAYS						,	{	L"Days",																			L"Jours",																										L"Tage",																											L"Días",																								L"Giorni",																											L"Dias",																									L"Dias"	}	},
	{ MSTR_DEFAULT					,	{	L"Default",																			L"V/déft",																										L"Standard",																										L"Predeterm.",																							L"Predef.",																											L"Predeterminado",																							L"Padrão"	}	},
	{ MSTR_DELETE_ADMINISTRATOR		,	{	L"Delete Administrator",															L"Suppr. administrateur",																						L"Administrator löschen",																							L"Eliminar administrador",																				L"Cancella amministratore",																							L"Eliminar administrador",																					L"Excluir administrador"	}	},
	{ MSTR_DELETE_OPERATOR			,	{	L"Delete Operator",																	L"Suppr. opérateur", 																							L"Bediener löschen",																								L"Eliminar operario",																					L"Cancella operatore",																								L"Eliminar operador",																						L"Excluir operador"	}	},
	{ MSTR_DETERMINING_ROTOR_TYPE	,	{	L"Determining disc type",															L"Détermination type de disque",																				L"Scheibentyp festlegen",																							L"Determinando el tipo de disco",																		L"Determinazione tipo del disco",																					L"A determinar o tipo de disco",																			L"Determinando o tipo de disco"	}	},
	{ MSTR_DHCP					    ,	{	L"DHCP",																			L"DHCP",																										L"DHCP",																											L"DHCP",																								L"DHCP",																											L"DHCP",																									L"DHCP" } },
	{ MSTR_DISABLED					,	{	L"Disabled",																		L"Désactivé",																									L"Deaktiviert",																										L"Inactivo",																							L"Disattivato",																										L"Inativo",																									L"Desativado"	}	},
	{ MSTR_DOB						,	{	L"DOB",																				L"DOB",																											L"GD",																												L"F. nac.",																								L"DN",																												L"DDN",																										L"DN"	}	},
	{ MSTR_DOCTOR_ID				,	{	L"Doctor ID",																		L"ID médecin",																									L"Arzt-ID",																											L"Id. médico",																							L"ID medico",																										L"ID do médico",																							L"ID do médico"	}	},
	{ MSTR_DOV						,	{	L"DOV",																				L"DOV",																											L"LI",																												L"Úl. vac.",																							L"DV",																												L"DDV",																										L"Data de vacinação"	}	},
	{ MSTR_DRAWER_CYCLES			,	{	L"Drawer Cycles",																	L"Cycles tiroir",																								L"Fachzyklen",																										L"Ciclos del cajón",																					L"Cicli cassetto",																									L"Ciclos da gaveta",																						L"Ciclos da gaveta"	}	},
	{ MSTR_EGFR						,	{	L"eGFR", 																			L"eGFR",																										L"eGFR",																											L"eGFR",																								L"eGFR",																											L"TFGe",																									L"TFGe"	}	},
	{ MSTR_ELECTROLYTES				,	{	L"Electrolytes",																	L"Électrolytes",																								L"Elektrolyte",																										L"Electrolitos",																						L"Elettroliti",																										L"Eletrólitos",																								L"Eletrólitos"	}	},
	{ MSTR_ENABLED					,	{	L"Enabled",																			L"Activé",																										L"Aktiviert",																										L"Activado",																							L"Attivato",																										L"Ativado",																									L"Ativado"	}	},
	{ MSTR_ENTER_AGE_RANGE			,	{	L"Enter Age Range",																	L"Saisir fourchette d'âge",																						L"Altersbereich eingeben",																							L"Introducir intervalo de edad",																		L"Inserisci fascia d'età",																							L"Introduzir intervalo de idades",																			L"Inserir faixa etária"	}	},
	{ MSTR_ENTER_PATIENT_ID			,	{	L"Enter Patient ID",																L"Entrer ID  patient",																							L"Patienten-ID eingeben",																							L"Introducir Id. del paciente",																			L"Inserisci ID paziente",																							L"Introduzir ID de doente",																					L"Inserir ID do paciente"	}	},
	{ MSTR_ENZYMES					,	{	L"Enzymes",																			L"Enzymes",																										L"Enzyme",																											L"Encimas",																								L"Enzimi",																											L"Enzimas",																									L"Enzimas"	}	},
	{ MSTR_ERROR_READING_CD			,	{	L"Error reading CD",																L"Erreur lecture CD",																							L"Fehler beim Lesen der CD",																						L"Error al leer el CD",																					L"Errore di lettura CD",																							L"Erro de leitura do CD",																					L"Erro ao ler o CD"	}	},
	{ MSTR_ERROR_REPORT				,	{	L"Error Report",																	L"Rapport erreur",																								L"Fehlerbericht",																									L"Informe de errores",																					L"Rapporto di errore",																								L"Relatório de erros",																						L"Relatório de erros"	}	},
	{ MSTR_ERRORS					,	{	L"Errors",																			L"Erreurs",																										L"Fehler",																											L"Errores",																								L"Errori",																											L"Erros",																									L"Erros"	}	},
	{ MSTR_EXPIRATION_DATE			,	{	L"Expiration Date",																	L"Date de péremption",																							L"Ablaufdatum",																										L"Fecha de vencimiento",																				L"Data di scadenza",																								L"Prazo de validade",																						L"Data de expiração"	}	},
	{ MSTR_EXTERNAL_ARCHIVE			,	{	L"External Archive",																L"Archive externe",																								L"Externes Archiv",																									L"Archivo externo",																						L"Archivio esterno",																								L"Ficheiro externo",																						L"Arquivo externo"	}	},
	{ MSTR_EXTERNAL_PRINTER			,	{	L"External Printer",																L"Imprimante externe",																							L"Externer Drucker",																								L"Impresora externa",																					L"Stampante esterna",																								L"Impressora externa",																						L"Impressora externa"	}	},
	{ MSTR_FACTORY_DEFAULT_SPECIES	,	{	L"Factory Default Demographics",													L"Param./défaut élmts démogr.",																					L"Voreingestellte Demographien",																					L"Demografía predeterminada",																			L"Demografia predefinita di fabbrica",																				L"Valores de demografia de fábrica",																		L"Dados demogr. padrão de fábrica"	}	},
	{ MSTR_FEMALE					,	{	L"Female",																			L"Féminin",																										L"Weiblich",																										L"Mujer",																								L"Uomo",																											L"Feminino",																								L"Sexo feminino"	}	},
	{ MSTR_FLASHES					,	{	L"Flashes",																			L"Flashs",																										L"Blinkzeichen",																									L"Flashes",																								L"Flash luminosi",					  																				L"Flashes",																									L"Flashes"	}	},
	{ MSTR_FRACTION					,	{	L"Fraction",																		L"Fraction",																									L"Anteil",																											L"Fracción",																							L"Frazione",																										L"Fração",																									L"Fração"	}	},
	{ MSTR_FULL						,	{	L"Full",																			L"Complet",																										L"Voll",																											L"Lleno",																								L"Completo",																										L"Completo",																								L"Cheio"	}	},
	{ MSTR_GATEWAY_IP				,	{	L"Gateway IP",																		L"IP de passerelle",																							L"Gateway-IP",																										L"IP puerta enlace",																					L"IP gateway",																										L"IP da porta",																								L"IP do gateway"	}	},
	{ MSTR_GENDER					,	{	L"Gender",																			L"Sexe",																										L"Geschlecht",																										L"Sexo",																								L"Sesso",																											L"Sexo",																									L"Gênero"	}	},
	{ MSTR_HISPANIC					,	{	L"Hispanic",																		L"Hispanique",																									L"Hispanische Abst.",																								L"Hispano",																								L"Ispanico",																										L"Hispânico",																								L"Hispânico"	}	},
	{ MSTR_HL7						,	{	L"HL7",																				L"HL7",																											L"HL7",																												L"HL7",																									L"HL7",																												L"HL7",																										L"HL7" } },
	{ MSTR_HL7_CONFIG				,	{	L"HL7 Configuration",																L"Configuration HL7",																							L"HL7-Konfiguration",																								L"Configuración HL7",																					L"Configurazione HL7",																								L"Configuração HL7",																						L"Configuração do HL7" } },
	{ MSTR_HOUR						,	{	L"Hour",																			L"Heure",																										L"Stunde",																											L"Hora",																								L"Ora",																												L"Hora",																									L"Hora"	}	},
	{ MSTR_HR						,	{	L"hr",																				L"h",																											L"Std.",																											L"hr",																									L"Or",																												L"h",																										L"h"	}	},
	{ MSTR_INFANT					,	{	L"Infant",																			L"Nourrisson",																									L"Säugling",																										L"Niño",																								L"Cucciolo",																										L"Criança",																									L"Lactente"	}	},
	{ MSTR_INSERT_CD				,	{	L"Insert CD, close drawer, and press Continue",										L"Insérer CD, fermer tiroir et appuyer sur Continuer",															L"CD einlegen, Fach schließen und Weiter drücken",																	L"Insertar CD, cerrar bandeja y pulsar Continuar",														L"Inserisci CD, chiudi cassetto e premi Continua",																	L"Insira o CD, feche a gaveta e prima Continuar",															L"Insira o CD, feche a gaveta e pressione Continuar"	}	},
	{ MSTR_INTERNAL_ARCHIVE			,	{	L"Internal Archive",																L"Archive interne",																								L"Internes Archiv",																									L"Archivo interno",																						L"Archivio interno",																								L"Ficheiro interno",																						L"Arquivo interno"	}	},
	{ MSTR_IP_MASK					,	{	L"Subnet Mask",																		L"Masq. sous-rés",																								L"Subnetzmaske",																									L"Másc. de subred",																						L"Masch. sottorete",																								L"Máscara de subrede",																						L"Máscara de sub-rede"	}	},
	{ MSTR_IQC						,	{	L"iQC",																				L"iQC",																											L"iQC",																												L"iQC",																									L"iQC",																												L"iQC",																										L"iQC"	}	},
	{ MSTR_IS_NOT_VALID				,	{	L"is not valid",																	L"incorrect",																									L"ungültig",																										L"no es válido",																						L"non valido",																										L"inválido",																								L"não é válido"	}	},
	{ MSTR_KENNEL_NUMBER			,	{	L"Kennel Number",																	L"Numéro chenil",																								L"Zwingernr.",																										L"Número de caseta",																					L"Numero Kennel",																									L"Número de Kennel",																						L"Número do canil"	}	},
	{ MSTR_KEYBOARD					,	{	L"Keyboard",																		L"Clavier",																										L"Eingabegerät",																									L"Teclado",																								L"Tastiera",																										L"Teclado",																									L"Teclado"	}	},
	{ MSTR_LAST_ROTOR				,	{	L"Last Disc",																		L"Dernier disque",																								L"Letzte Scheibe",																									L"Último disco",																						L"Ultimo disco",																									L"Último disco",																							L"Último disco"	}	},
	{ MSTR_LAST_MODIFIED			,	{	L"Last Modified",																	L"Dernière modif.",																								L"Zuletzt geändert",																								L"Último cambio",																						L"Ultima modifica",																									L"Última alteração",																						L"Última modificação"	}	},
	{ MSTR_LIPIDS					,	{	L"Lipids",																			L"Lipides",																										L"Lipide",																											L"Lípidos",																								L"Lipidi",																											L"Lípidos",																									L"Lipídios"	}	},
	{ MSTR_LOAD_FUSE_SETTINGS		,	{	L"Load Fuse Settings", 																L"Charger les paramètres de fusible",																			L"Sicherungseinstellungen laden",																					L"Cargar configuración del fusible",																	L"Carica impostazioni fusibile",																					L"Carregar configurações de fusíveis",																		L"Carregar configurações de fusível"	}	},
	{ MSTR_LOAD_ROTOR_TO_ANALYZE	,	{	L"To analyze additional sample, load new disc",										L"Pour analyser un échantillon supplémentaire, charger un nouveau disque",										L"Zur Analyse einer weiteren Probe neue Scheibe laden",																L"Para analizar otra muestra, cargue un disco nuevo",													L"Per esaminare ulteriori campioni, caricare nuovo disco",															L"Para analisar outra amostra, carregue um novo disco",														L"Para analisar uma amostra adicional, carregue um novo disco"	}	},
	{ MSTR_LOCATION					,	{	L"Location",																		L"Emplacement",																									L"Ort",																												L"Ubicación",																							L"Sede",																											L"Localização",																								L"Local"	}	},
	{ MSTR_LOCKOUT					,	{	L"Lockout",																			L"Verrouillage",																								L"Verriegelung",																									L"Bloqueo",																								L"Serrata",																											L"Bloqueio",																								L"Bloquear"	}	},
	{ MSTR_LOWER					,	{	L"Lower",																			L"Inférieur",																									L"Niedriger",																										L"Inferior",																							L"Basso",																											L"Inferior",																								L"Inferior"	}	},
	{ MSTR_MAC_ADDRESS				,	{	L"MAC Address",																		L"Adresse MAC",																									L"MAC-Adresse",																										L"Dirección MAC",																						L"Indirizzo MAC",																									L"Endereço MAC",																							L"Endereço MAC" } },
	{ MSTR_MALE						,	{	L"Male",																			L"Masculin",																									L"Männlich",																										L"Hombre",																								L"Donna",																											L"Masculino",																								L"Sexo masculino"	}	},
	{ MSTR_MANUAL_SETTINGS			,	{	L"Manual Settings", 																L"Paramètres manuels",																							L"Manuelle Einstellungen",																							L"Configuración manual",																				L"Impostazioni manuali",																							L"Configurações manuais",																					L"Configurações manuais"	}	},
	{ MSTR_MIN						,	{	L"min",																				L"min",																											L"min.",																											L"min",																									L"min",																												L"min",																										L"min"	}	},
	{ MSTR_MINERALS					,	{	L"Minerals",																		L"Minéraux",																									L"Mineralien",																										L"Minerales",																							L"Minerali",																										L"Minerais",																								L"Minerais"	}	},
	{ MSTR_MINUTE					,	{	L"Minute",																			L"Minute",																										L"Minute",																											L"Minuto",																								L"Minuto",																											L"Minuto",																									L"Minuto"	}	},
	{ MSTR_MONTH					,	{	L"Month",																			L"Mois",																										L"Monat",																											L"Mes",																									L"Mese",																											L"Mês",																										L"Mês"	}	},
	{ MSTR_MOS						,	{	L"Mos.",																			L"Mois",																										L"Monate",																											L"Meses",																								L"Meses",																											L"Meses",																									L"Meses"	}	},
	{ MSTR_NAME						,	{	L"Name:",																			L"Nom :",																										L"Name:",																											L"Nombre:",																								L"Nome:",																											L"Nome:",																									L"Nome:"	}	},
	{ MSTR_NATIVE_AMERICAN			,	{	L"Native American",																	L"Amérindienne",																								L"Indianische Abst.",																								L"Americano nativo",																					L"Nativo americano",																								L"Americano nativo",																						L"Americano nativo"	}	},
	{ MSTR_NEONATE					,	{	L"Neonate",																			L"Nouveau-né",																									L"Neugeborenes",																									L"Neonato",																								L"Neonato",																											L"Recém-nascido",																							L"Neonato"	}	},
	{ MSTR_NEUTERED					,	{	L"Neutered",																		L"Châtré",																										L"Kastriert",																										L"Castrado",																							L"Castrato",																										L"Castrado",																								L"Castrado"	}	},
	{ MSTR_NEVER					,	{	L"Never",																			L"Jamais",																										L"Nie",																												L"Nunca",																								L"Mai",																												L"Nunca",																									L"Nunca"	}	},
	{ MSTR_NEW						,	{	L"New",																				L"Nouveau",																										L"Neu",																												L"Nuevo",																								L"Nuovo",																											L"Novo",																									L"Novo"	}	},
	{ MSTR_NO_RESULTS				,	{	L"No saved results to view. Please run a disc prior to recalling results.",			L"Aucun résultat enregistré à afficher. Exécuter un disque avant de rappeler les résultats.",					L"Keine gespeicherten Ergebnisse zur Anzeige. Vor Wiederaufrufen der Ergebnisse eine Scheibe starten.",				L"No hay resultados guardados para ver. Utilice un disco antes de buscar resultados.",					L"Nessun risultato salvato da visualizzare. Avviare un disco prima di richiamare i risultati.",						L"No existem resultados guardados para visualizar. Utilize um disco antes de recuperar resultados.",		L"Nenhum resultado salvo para visualização. Execute um disco antes de recuperar os resultados."	}	},
	{ MSTR_NON_SI					,	{	L"Non SI",																			L"Non SI",																										L"Kein SI",																											L"No SI",																								L"Non SI",																											L"Não SI",																									L"Não SI"	}	},
	{ MSTR_NONE						,	{	L"None",																			L"Aucun",																										L"Kein",																											L"Ninguno",																								L"Nessuno",																											L"Nenhum",																									L"Nenhum"	}	},
	{ MSTR_OFF						,	{	L"Off",																				L"Off",																											L"Aus",																												L"Apagado",																								L"Off",																												L"Desligado",																								L"Desligado"	}	},
	{ MSTR_OPENING_DRAWER			,	{	L"Opening Drawer...",																L"Ouverture tiroir en cours...",																				L"Fach öffnen ...",																									L"Abriendo bandeja...",																					L"Apertura cassetto in corso...",																					L"A abrir a gaveta...",																						L"Abrindo gaveta..."	}	},
	{ MSTR_OPERATOR_ID				,	{	L"Operator ID",																		L"ID opérateur",																								L"Bediener-ID",																										L"Id. operario:",																						L"ID operatore",																									L"ID do operador",																							L"ID do operador"	}	},
	{ MSTR_OPERATORS				,	{	L"Operators",																		L"Opérateurs",																									L"Bediener",																										L"Operarios",																							L"Operatori",																										L"Operadores",																								L"Operadores"	}	},
	{ MSTR_OWNER_ID					,	{	L"Owner ID",																		L"ID propriétaire",																								L"Besitzer-ID",																										L"Id. dueño",																							L"ID propr.",																										L"ID do proprietário",																						L"ID do proprietário"	}	},
	{ MSTR_PATIENT_CONTROL_ID		,	{	L"Patient/Control ID",																L"ID patient/contrôle",																							L"Patienten-/Kontrollen-ID",																						L"Id. paciente/control",																				L"ID paziente/controllo",																							L"ID do doente/controlo",																					L"ID do paciente/controle"	}	},
	{ MSTR_PATIENT_ID				,	{	L"Patient ID",																		L"ID patient",																									L"Patienten-ID",																									L"Id. paciente",																						L"ID paziente",																										L"ID do doente",																							L"ID do paciente"	}	},
	{ MSTR_PATIENT_TYPE				,	{	L"Patient Type",																	L"Type de patient",																								L"Patiententyp",																									L"Tipo de paciente",																					L"Tipo di paziente",																								L"Tipo de doente",																							L"Tipo de paciente"	}	},
	{ MSTR_PATIENTS					,	{	L"Patients",																		L"Patients",																									L"Patienten",																										L"Pacientes",																							L"Pazienti",																										L"Doentes",																									L"Pacientes"	}	},
	{ MSTR_PENDING_ORDER			,	{	L"Pending Order",																	L"Ordre en cours",																								L"Ausstehende Anforderung",																							L"Pedido pendiente",																					L"Ordine in sospeso",																								L"Pedido pendente",																							L"Solicitação pendente"	}	},
	{ MSTR_PHONE_NUMBER				,	{	L"Phone Number",																	L"N° tél",																										L"Telefonnummer",																									L"Número de teléfono",																					L"Num. telefono",																									L"Número de telefone",																						L"Número de telefone"	}	},
	{ MSTR_PLEASE_REMOVE_ROTOR		,	{	L"Please remove the disc from the analyzer",										L"Retirer le disque de l'analyseur",																			L"Bitte die Scheibe aus dem Analysegerät entfernen",																L"Retire el disco del analizador",																		L"Rimuovere il disco dall'analizzatore",																			L"Retire o disco do analisador",																			L"Remova o disco do analisador"	}	},
	{ MSTR_PLEASE_WAIT_CDUPDATE		,	{	L"Please wait. Do not remove power. Analyzer will restart when update is complete.", L"Veuillez patienter. Ne pas couper l'alimentation. L'analyseur redémarre une fois la mise à jour terminée.",	L"Bitte warten. Bitte Stromversorgung nicht abschalten. Analysegerät wird nach vollständigem Update neu gestartet.", L"Espere, por favor. No apague el aparato. El analizador se reiniciará cuando acabe la actualización.", L"Si prega di attendere. Non togliere corrente. L'analizzatore si riavvierà al completamento dell'aggiornamento.",	L"Espere, por favor. Não desligue. O analisador será reiniciado quando terminar a atualização.",			L"Aguarde. Não remova a energia. O analisador será reiniciado quando a atualização concluir."	}	},
	{ MSTR_POTC_1A					,	{	L"POTC-1A",																			L"POTC-1A",																										L"POTC-1A",																											L"POTC-1A",																								L"POTC-1A",																											L"POTC-1A",																									L"POTC-1A"	}	},
	{ MSTR_POWER_SAVE				,	{	L"Power Save",																		L"Écono. alim.",																								L"Energiesparmodus",																								L"Ahorro de energía",																					L"Modo risparmio",																									L"Poupança de energia",																						L"Economia de energia"	}	},
	{ MSTR_PREMATURE				,	{	L"Premature",																		L"Prématuré",																									L"Vorzeitig",																										L"Prematuro",																							L"Prematuro",																										L"Prematuro",																								L"Prematuro"	}	},
	{ MSTR_PRESS_YES_DEFAULTS		,	{	L"Press Yes to return all settings to factory defaults",							L"Appuyer sur Oui pour rétablir tous les paramètres par défaut",												L"Drücken Sie Ja, um die Werkseinstellungen wieder herzustellen",													L"Pulse Sí para reestablecer la configuración predeterminada original",									L"Premere Sì per ripristinare tutte le impostazioni di fabbrica",													L"Prima Sim para reestabelecer a configuração para os valores predeterminados de fábrica",					L"Pressione Sim para retornar todas as configurações aos padrões de fábrica"	}	},
	{ MSTR_PRESS_YES_REF_RANGES		,	{	L"Press Yes to return the active demographics list to the factory default",			L"Appuyer sur Oui pour rétablir la liste d'éléments démographiques actifs par défaut",							L"Drücken Sie Ja, um die Werkseinstellungen für die Liste der aktiven Demographien wieder herzustellen",			L"Pulse Sí para reestablecer los valores predeterminados originales de la lista de demografías activas", L"Premere Sì per mandare l'elenco demografie attive nelle impostazioni di fabbrica",								L"Prima Sim para reestabelecer a lista de demografias ativa para os valores predeterminados de fábrica",	L"Pressione Sim para retornar a lista de dados demográficos ativos ao padrão de fábrica"	}	},
	{ MSTR_PRESS_YES_TO_START		,	{	L"Press Yes to start",																L"Appuyer sur Oui pour redémarrer",																				L"Drücken Sie Ja für Start",																						L"Pulse Sí para empezar",																				L"Premere Sì per avviare",																							L"Prima Sim para iniciar",																					L"Pressione Sim para começar"	}	},
	{ MSTR_PRINT					,	{	L"Print",																			L"Impr.",																										L"Drucken",																											L"Imprimir",																							L"Stampa",																											L"Imprimir",																								L"Imprimir"	}	},
	{ MSTR_PRINTED					,	{	L"Printed",																			L"Imprimé",																										L"Gedruckt",																										L"Impreso",																								L"Stampato",																										L"Impresso",																								L"Impresso"	}	},
	{ MSTR_PRINTER					,	{	L"Printer",																			L"Imprimante",																									L"Drucker",																											L"Impr.",																								L"Stampante",																										L"Impressora",																								L"Impressora"	}	},
	{ MSTR_PROTEINS					,	{	L"Proteins",																		L"Protéines",																									L"Proteine",																										L"Proteínas",																							L"Proteine",																										L"Proteínas",																								L"Proteínas"	}	},
	{ MSTR_RACE						,	{	L"Race",																			L"Race",																										L"Rasse",																											L"Raza",																								L"Razza",																											L"Corrida",																									L"Raça"	}	},
	{ MSTR_READING_CD				,	{	L"Reading CD...",																	L"Lecture du CD...",																							L"CD wird gelesen...",																								L"Leyendo CD...",																						L"Lettura CD...",																									L"A ler o CD...",																							L"Lendo CD..."	}	},
	{ MSTR_RECALL_RESULTS			,	{	L"Recall Results", 																	L"Rappeler résultats",																							L"Ergebnisse neu abrufen",																							L"Buscar resultados",																					L"Recupera i risultati",																							L"Recuperar resultados",																					L"Recuperar resultados"	}	},
	{ MSTR_REMOVE_SPECIES			,	{	L"Remove Demographic",																L"Retirer éléments démographiques",																				L"Demographien entfernen",																							L"Eliminar demografía",																					L"Rimuovi demografia",																								L"Eliminar demografia",																						L"Remover dado demográfico"	}	},
	{ MSTR_RESULTS					,	{	L"Results",																			L"Résultats",																									L"Ergebnisse",																										L"Result.",																								L"Risultati",																										L"Resultados",																								L"Resultados"	}	},
	{ MSTR_RESULTS_QUEUED			,	{	L"Results Queued",																	L"Résultats en file d'attente",																					L"Zu übertragende Ergebnisse",																						L"Resultados en cola",																					L"Risultati accodati",																								L"Resultados em fila",																						L"Resultados em fila"	}	},
	{ MSTR_RETRIEVE					,	{	L"Retrieve",																		L"Récupérer",																									L"Abrufen",																											L"Recuperar",																							L"Recupera",																										L"Recuperar",																								L"Recuperar"	}	},
	{ MSTR_ROTORS_RUN				,	{	L"Discs Run",																		L"Exécution disques",																							L"Scheiben in Betrieb",																								L"Usar discos",																							L"Disco in funzione",																								L"Utilizar discos",																							L"Execução de discos"	}	},
	{ MSTR_SAMPLE_ID				,	{	L"Sample ID",																		L"ID échantillon",																								L"Proben-ID",																										L"Id. de muestra",																						L"ID campione",																										L"ID da amostra",																							L"ID da amostra"	}	},
	{ MSTR_SAVING_RESULTS			,	{	L"Saving Results...",																L"Enregistrement des résultats en cours...",																	L"Ergebnisse werden gespeichert...",																				L"Guardando resultados...",																				L"Salvataggio risultati...",																						L"A guardar os resultados...",																				L"Salvando resultados..."	}	},
	{ MSTR_SCREEN_CLICKS			,	{	L"Screen Clicks",																	L"Clics écran",																									L"Bildschirmklicks",																								L"Clics de pantalla",																					L"Clic schermo",																									L"Cliques no ecrã",																							L"Cliques de tela"	}	},
	{ MSTR_SCREEN_SAVER				,	{	L"Screen Saver",																	L"Économiseur d'écran",																							L"Bildschirmschoner",																								L"Salvapantalla",																						L"Screensaver",																										L"Proteção do ecrã",																						L"Protetor de tela"	}	},
	{ MSTR_SEARCH					,	{	L"Search",																			L"Rechercher",																									L"Suchen",																											L"Búsqueda",																							L"Cerca",																											L"Pesquisar",																								L"Pesquisar"	}	},
	{ MSTR_SELECT					,	{	L"Select",																			L"Sélectionner",																								L"Auswählen",																										L"Seleccionar",																							L"Seleziona",																										L"Selecionar",																								L"Selecionar"	}	},
	{ MSTR_SELECT_REPORTS			,	{	L"Select Reports",																	L"Sélectionner rapports",																						L"Berichte auswählen",																								L"Seleccionar informes",																				L"Seleziona rapporti",																								L"Selecionar relatórios",																					L"Selecionar relatórios"	}	},
	{ MSTR_SEND						,	{	L"Send",																			L"Envoyer",																										L"Senden",																											L"Enviar",																								L"Invia",																											L"Enviar",																									L"Enviar"	}	},
	{ MSTR_SENDING					,	{	L"Sending...",																		L"Envoi en cours...",																							L"Sendet ...",																										L"Enviando...",																							L"Invio in corso...",																								L"A enviar...",																								L"Enviando..."	}	},
	{ MSTR_SENDING_REPORT			,	{	L"Sending Report...",																L"Envoi de rapport en cours...",																				L"Sende Bericht ...",																								L"Enviando informe...",																					L"Invio rapporto in corso...",																						L"A enviar relatório...",																					L"Enviando relatório..."	}	},
	{ MSTR_SERIAL_NO				,	{	L"Serial No.:",																		L"N° de série :",																								L"Seriennr.:",																										L"N° de serie:",																						L"Numero di serie:",																								L"N.° de série:",																							L"No. de série:"	}	},
	{ MSTR_SERVER_IP				,	{	L"Server IP",																		L"IP du serveur",																								L"Server-IP",																										L"IP de servidor",																						L"IP server",																										L"IP do servidor",																							L"IP do servidor"	}	},
	{ MSTR_SERVER_SOCKET			,	{	L"Server Socket",																	L"Socket serveur",																								L"Server-Socket",																									L"Socket servidor",																						L"Socket server",																									L"Tomada do servidor",																						L"Soquete do servidor"	}	},
	{ MSTR_SERVICE_ARCHIVE			,	{	L"Service Archive",																	L"Archive service",																								L"Service-Archiv",																									L"Archivo de servicio",																					L"Archivio di servizio",																							L"Ficheiro do serviço",																						L"Arquivo de serviço"	}	},
	{ MSTR_SERVICE_ARCHIVE_ELLIPSIS	,	{	L"Service Archive...",																L"Archive service...",																							L"Service-Archiv ...",																								L"Archivo de servicio...",																				L"Archivio di servizio...",																							L"Ficheiro do serviço...",																					L"Arquivo de serviço..."	}	},
	{ MSTR_SET_DEFAULT				,	{	L"Set Default",																		L"Déf. param./défaut",																							L"Standard einstellen",																								L"Establ. predeterm.",																					L"Imposta predef.",																									L"Configurar valores predeterminados",																		L"Configurar padrão"	}	},
	{ MSTR_SETTINGS					,	{	L"Settings",																		L"Paramètres",																									L"Einstellungen",																									L"Configuración",																						L"Impostazioni",																									L"Configurações",																							L"Configurações"	}	},	
	{ MSTR_SI						,	{	L"SI",																				L"SI",																											L"SI",																												L"SI",																									L"SI",																												L"SI",																										L"SI"	}	},
	{ MSTR_SINGLE_ANALYTE			,	{	L"Single Analyte",																	L"1 seule subs. à analyser",																					L"Einzelnes Analyt",																								L"Analito único",																						L"Analita singolo",																									L"Analito único",																							L"Analito único"	}	},
	{ MSTR_SPAYED					,	{	L"Spayed",																			L"Châtrée",																										L"Sterilisiert",																									L"Castrada",																							L"Sterilizzato",																									L"Esterilizado",																							L"Esterilizado"	}	},
	{ MSTR_SPECIAL					,	{	L"Special",																			L"Spécial",																										L"Spezial",																											L"Especial",																							L"Speciale",																										L"Especial",																								L"Especial"	}	},
	{ MSTR_SPECIES					,	{	L"Demographic",																		L"Élmts démogr.",																								L"Demographien",																									L"Demografía",																							L"Demografia",																										L"Demografia",																								L"Dado demográfico"	}	},
	{ MSTR_STATIC_IP				,	{	L"Static IP",																		L"IP statique",																									L"Statische IP",																									L"IP estática",																							L"IP statico",																										L"IP estático",																								L"IP estático" } },
	{ MSTR_STATUS					,	{	L"Status",																			L"État",																										L"Status",																											L"Estado",																								L"Stato",																											L"Estado",																									L"Status"	}	},
	{ MSTR_TCPIP					,	{	L"TCP/IP",																			L"TCP/IP",																										L"TCP/IP",																											L"TCP/IP",																								L"TCP/IP",																											L"TCP/IP",																									L"TCP/IP"	}	},
	{ MSTR_TEST						,	{	L"Test",																			L"Test",																										L"Test",																											L"Prueba",																								L"Prova",																											L"Teste",																									L"Teste"	}	},
	{ MSTR_TESTING_CONNECTION		,	{	L"Testing connection...",															L"Test de la connexion en cours...",																			L"Verbindung wird geprüft...",																						L"Probando conexión...",																				L"Test della connessione...",																						L"A testar ligação...",																						L"Testando conexão..."	}	},
	{ MSTR_THIS_ANALYZER			,	{	L"This Analyzer",																	L"Cet analyseur",																								L"Dieses Analysegerät",																								L"Este analizador",																						L"Questo analizzatore",																								L"Este analisador",																							L"Este analisador"	}	},
	{ MSTR_TODDLER					,	{	L"Toddler",																			L"Bébé",																										L"Kleinkind",																										L"Párvulo",																								L"Bambino ai primi passi",																							L"Bebé",																									L"Criança"	}	},
	{ MSTR_TOUCH_SCREEN_TO_CALIB	,	{	L"Touch screen to calibrate or wait to cancel.",									L"Toucher l'écran pour calibrer ou attendre pour annuler.",														L"Zur Kalibrierung Bildschirm berühren oder Abbruch abwarten.",														L"Toque la pantalla para calibrar o espere para cancelar.",												L"Toccare lo schermo per calibrare, oppure attendere per annullare.",												L"Toque no ecrã para calibrar ou espere para cancelar.",													L"Toque na tela para calibrar ou aguarde para cancelar."	}	},
	{ MSTR_TRANSMIT_ALL				,	{	L"Transmit All",																	L"Transmettre tout",																							L"Alle übertragen",																									L"Transmitir todo",																						L"Trasmetti tutti",																									L"Transmitir tudo",																							L"Transmitir todos"	}	},
	{ MSTR_TRANSMIT_DATE_RANGE		,	{	L"Transmit Date Range",																L"Transm. plage de dates",																						L"Datumsbereich übertr.",																							L"Trans. intervalo/fechas",																				L"Trasmetti range di date",																							L"Transmitir intervalo de data",																			L"Transmitir intervalo de datas"	}	},
	{ MSTR_TRANSMITTING_ELLIPSIS	,	{	L"Transmitting...",																	L"Transmission...",																								L"Übertragung...",																									L"Transmitiendo...",																					L"Trasmissione in corso...",																						L"A transmitir...",																							L"Transmitindo..."	}	},
	{ MSTR_TRANSMITTING_ALL			,	{	L"Transmitting All...",																L"Trans. en cours...",																							L"Alle werden übertr. ...",																							L"Transmitiendo todo...",																				L"Trasm. tutti in corso...",																						L"A transmitir tudo...",																					L"Transmitindo todos..."	}	},
	{ MSTR_UNITS					,	{	L"Units",																			L"Unités",																										L"Einheiten",																										L"Unidades",																							L"Unità",																											L"Unidades",																								L"Unidades"	}	},
	{ MSTR_UNKNOWN					,	{	L"Unknown",																			L"Inconnu",																										L"Unbekannt",																										L"Desconocido",																							L"Sconosciuto",																										L"Desconhecido",																							L"Desconhecido"	}	},
	{ MSTR_UPDATE_SOFTWARE			,	{	L"Update Software",																	L"Mise à jour logiciel",																						L"Software aktualisieren",																							L"Actualizar programa",																					L"Aggiorna software",																								L"Atualizar o Software",																					L"Atualizar software"	}	},
	{ MSTR_UPDATING_CONTROLS		,	{	L"Updating Controls...",															L"Actualisation des commandes en cours...",																		L"Steuerungen werden aktualisiert ...",																				L"Actualizando controles...",																			L"Aggiornamento controlli in corso...",																				L"A atualizar controlos...",																				L"Atualizando controles..."	}	},
	{ MSTR_UPDATING_SOFTWARE		,	{	L"Updating Software...",															L"Mise à jour logiciel...",																						L"Softw. wird aktual. ...",																							L"Actualizando programa",																				L"Aggior. softw...",																								L"A atualizar o Software...",																				L"Atualizando software..."	}	},
	{ MSTR_UPPER					,	{	L"Upper",																			L"Haut",																										L"Oben",																											L"Superior",																							L"Superiore",																										L"Superior",																								L"Superior"	}	},
	{ MSTR_USB						,	{	L"USB (Serial)",																	L"USB (série)",																									L"USB (ser. Anschl.)",																								L"USB (serie)",																							L"USB (seriale)",																									L"USB (série)",																								L"USB (serial)"	}	},
	{ MSTR_VERSION					,	{	L"Version:",																		L"Version :",																									L"Version:",																										L"Versión:",																							L"Versione:",																										L"Versão:",																									L"Versão:"	}	},
	{ MSTR_WARMING					,	{	L"Warming",																			L"Réchauffement",																								L"Aufwärmen",																										L"Calentando",																							L"Riscaldamento",																									L"A aquecer",																								L"Aquecendo"	}	},
	{ MSTR_WHITE					,	{	L"White",																			L"Blanc",																										L"Weiss",																											L"Blanco",																								L"Bianco",																											L"Branco",																									L"Branco"	}	},
	{ MSTR_WKS						,	{	L"Wks.",																			L"Sem.",																										L"Wks.",																											L"Sem.",																								L"Sett.",																											L"Sem.",																									L"Semanas"	}	},
	{ MSTR_WORK_LIST				,	{	L"Work List",																		L"Liste de tâches",																								L"Auftragsliste",																									L"Lista de trabajo",																					L"Elenco di lavoro",																								L"Lista de trabalho",																						L"Lista de trabalho"	}	},
	{ MSTR_XMIT						,	{	L"Xmit",																			L"Trans",																										L"Xmit",																											L"Transm.",																								L"Xmit",																											L"Transm.",																									L"Transmit"	}	},
	{ MSTR_XML						,	{	L"XML",																				L"XML",																											L"XML",																												L"XML",																									L"XML",																												L"XML",																										L"XML"	}	},
	{ MSTR_YEAR						,	{	L"Year",																			L"Année",																										L"Jahr",																											L"Año",																									L"Anno",																											L"Ano",																										L"Ano"	}	},
	{ MSTR_YRS						,	{	L"Yrs.",																			L"Ans.",																										L"Jahre",																											L"Años",																								L"Anni",																											L"Anos",																									L"Anos"	}	},
	{ MSTR_ZERO_SEC					,	{	L"Zero Sec",																		L"Zéro sec",																									L"Null Sek.",																										L"Cero seg",																							L"Zero Sec",																										L"Zero seg",																								L"Zero seg"	}	},

// Bottom									ENGLISH,				FRENCH,							GERMAN,						SPANISH,				ITALIAN, 					PORTUGUESE,				PORTUGUESE (BRAZIL)
	{ BSTR_ADD						,	{	L"Add",					L"Ajouter",						L"Hinzufügen",				L"Añadir",				L"Aggiungi",				L"Adicionar",			L"Adicionar"	}	},
	{ BSTR_BACK						,	{	L"Back",				L"Retour",						L"Zurück",					L"Volver",				L"Indietro",				L"Voltar",				L"Voltar"	}	},
	{ BSTR_CANCEL					,	{	L"Cancel",				L"Annuler",						L"Abbrechen",				L"Cancelar",			L"Annulla",					L"Cancelar",			L"Cancelar"	}	},
	{ BSTR_CLOSE 					,	{	L"Close",				L"Fermer",						L"Schließen",				L"Cerrar",				L"Chiudi",					L"Fechar",				L"Fechar"	}	},
	{ BSTR_CONFIRM					,	{	L"Confirm",				L"Confirmer",					L"Bestätigen",				L"Confirmar",			L"Conferma",				L"Confirmar",			L"Confirmar"	}	},
	{ BSTR_CONTINUE					,	{	L"Continue",			L"Continuer",					L"Fortsetzen",				L"Continuar",			L"Continua",				L"Continuar",			L"Continuar"	}	},
	{ BSTR_DATE						,	{	L"Date",				L"Date",						L"Datum",					L"Fecha",				L"Data",					L"Data",				L"Data"		}	},
	{ BSTR_DONE						,	{	L"Done",				L"Terminé",						L"Erledigt",				L"Terminado",			L"Fatto",					L"Terminar",			L"Concluído"	}	},
	{ BSTR_ENTER					,	{	L"Enter",				L"Entrée",						L"Eingabe",					L"Entrar",				L"Invio",					L"Introduzir",			L"Entrar"	}	},
	{ BSTR_EXIT						,	{	L"Exit",				L"Quit",						L"Exit",					L"Salir",				L"Esci",					L"Sair",				L"Sair"		}	},
	{ BSTR_HOME						,	{	L"Home ",				L"Accueil",						L"Home",					L"Inicio",				L"Home",					L"Início",				L"Início"	}	},
	{ BSTR_NEXT						,	{	L"Next",				L"Suivant",						L"Weiter",					L"Sig.",				L"Successivo",				L"Seguinte",			L"Próximo"	}	},
	{ BSTR_NO						,	{	L"No",					L"Non",							L"Nein",					L"No",					L"No",						L"Não",					L"Não"		}	},
	{ BSTR_OPEN						,	{	L"Open",				L"Ouvrir",						L"Öffnen",					L"Abrir",				L"Aperto",					L"Abrir",				L"Abrir"		}	},
	{ BSTR_PERFORMING_IQC			,	{	L"Performing iQC...",	L"Exécution iQC en cours...",	L"iQC wird ausgeführt...",	L"Realizando iQC...",	L"Esecuz. iQC in corso...",	L"A executar iQC...",	L"Executando iQC..."	}	},
	{ BSTR_PRINT					,	{	L"Print",				L"Impr.",						L"Drucken",					L"Impr.",				L"Stampa",					L"Imprimir",			L"Imprimir"	}	},
	{ BSTR_RESTART					,	{	L"Restart",				L"Redémarrer",					L"Erneut starten",			L"Reiniciar",			L"Riavvia",					L"Reiniciar",			L"Reiniciar"	}	},
	{ BSTR_SAVE						,	{	L"Save",				L"Enreg.",						L"Save",					L"Guard.",				L"Salva",					L"Guardar",				L"Salvar"	}	},
	{ BSTR_SEARCH					,	{	L"Search",				L"Rechercher",					L"Suchen",					L"Búsqueda",			L"Cerca",					L"Pesquisar",			L"Pesquisar"	}	},
	{ BSTR_YES						,	{	L"Yes",					L"Oui",							L"Ja",						L"Sí",					L"Sì",						L"Sim",					L"Sim"		}	},

// Info text
	{ ISTR_LOAD_ROTOR_1				,	{	L"Remove reagent disc from its foil pouch and transfer 100ul of whole blood, plasma or serum into the disc.\n",					// English
											L"Retirer le disque de réactif de sa pochette alu et transférer 100 ul de sang entier, de plasma ou de sérum dans le rotor.\n",	// French
											L"Reagenzscheibe aus dem Plastikbeutel entfernen und 100ul Vollblut, Plasma oder Serum in die Scheibe einfüllen.\n",				// German
											L"Retire el disco de reactivo de su bolsa metalizada y transfiera 100 ul de sangre, plasma o suero al disco.\n",					// Spanish
											L"Rimuovere il disco reagente dalla custodia metallizzata e trasferire 100ul di sangue intero, plasma o siero nel disco.\n",		// Italian
											L"Retire o disco de reagente da sua bolsa metalizada e transfira 100 ul de sangue total, plasma ou soro para o disco.\n",		// Portuguese
											L"Remova o disco de reagente da bolsa de alumínio e transfira 100 ul de sangue total, plasma ou soro para o disco.\n"			// Portuguese (Brazil)
										} 
	},
	{ ISTR_LOAD_ROTOR_2				,	{	L"Insert the reagent disc into the analyzer, close the drawer and follow the on-screen prompts.\n",								// English
											L"Insérer le disque de réactif dans l'analyseur, fermer le tiroir et suivre les instructions qui s'affichent à l'écran.\n",		// French
											L"Reagenzscheibe in das Analysegerät einsetzen, Fach schließen und Bildschirmanweisungen befolgen.\n",							// German
											L"Inserte el disco de reactivo en el analizador, cierre el cajón y siga las instrucciones de pantalla.\n",						// Spanish
											L"Inserire il disco reagente nell'analizzatore, chiudere il cassetto e seguire le istruzioni a schermo.\n",						// Italian
											L"Insira o disco de reagente no analisador, feche a gaveta e siga as instruções do ecrã.\n",										// Portuguese
											L"Insira o disco de reagente no analisador, feche a gaveta e siga as instruções na tela.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_LOAD_ROTOR_3				,	{	L"View, read or download results.\n",				// English
											L"Afficher, lire et télécharger les résultats.\n",	// French
											L"Ergebnisse ansehen, lesen oder herunterladen.\n",	// German
											L"Visualice, lea o descargue resultados.\n",			// Spanish
											L"Visualizzare, leggere o scaricare i risultati.\n",	// Italian
											L"Visualize, leia ou descarregue os resultados.\n",	// Portuguese
											L"Visualizar, ler ou baixar resultados.\n"			// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_RESTORE			,	{	L"Restores reference ranges to a particular date and time. The restored reference ranges replace the current reference ranges. All changes after the date reference ranges were archived will be lost. It is recommended that an archive of the current reference ranges be made prior to restoring an earlier set.\n",																				// English
											L"Rétablit les fourchettes de référence sur une date et une heure données. Les fourchettes de référence rétablies remplacent les fourchettes de référence actuelles. Tous les changements effectués après l'archivage des fourchettes de date de références seront perdus. Il est conseillé de créer une archive des fourchettes de référence actuelles avant de rétablir un ensemble antérieur.\n",	// French
											L"Setzt die Referenzbereiche auf ein bestimmtes Datum und Zeit zurück. Die wiederhergestellten Referenzbereiche ersetzen die aktuellen Referenzbereiche. Sämtliche nach der Archivierung der Datumsreferenzbereiche vorgenommenen Änderungen gehen verloren. Es wird empfohlen, die aktuellen Referenzbereiche zu archivieren, vor Wiederherstellung eines früheren Satzes.\n",						// German
											L"Recupera los intervalos de referencia de una fecha y hora específicos. Los intervalos de referencia reestablecidos reemplazan a los intervalos de referencia actuales. Se perderán todos los cambios tras la fecha en la que se archivaron los intervalos de referencia. Se recomienda hacer un archivo de los intervalos de referencia actuales antes de reestablecer un grupo anterior.\n",		// Spanish
											L"Ripristina i range di riferimento ad una particolare data e ora. I range di riferimento ripristinati sostituiscono i range di riferimento attuali. Tutte le modifiche successive alla data di archiviazione dei range di riferimento andranno perse. Si raccomanda di creare un archivio degli attuali range di riferimento prima di ripristinare un set precedente.\n",							// Italian
											L"Restaura os intervalos de referência de una data e hora específicas. Os intervalos de referência restaurados substituem os intervalos de referência atuais. Serão perdidas todas as alterações após a data do arquivo dos intervalos de referência. É recomendado realizar um arquivo dos intervalos de referência atuais antes de recuperar um conjunto anterior.\n",								// Portuguese
											L"Restaura os intervalos de referência para uma data e hora específicas. Os intervalos de referência restaurados substituem os intervalos de referência atuais. Todas as alterações após o arquivamento dos intervalos de referência de datas serão perdidas. É recomendável fazer um arquivo dos intervalos de referência atuais antes de restaurar um conjunto anterior.\n"						// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_SELECT			,	{	L"Send: Archive reference ranges for future reference.\n\n"
											L"Retrieve: Restore an internal reference range archive.\n",											// English
											L"Envoyer : archiver les fourchettes de référence afin de pouvoir s'y reporter ultérieurement.\n\n"
											L"Récupérer : restaurer une archive de fourchette de référence interne.\n",							// French
											L"Senden: Referenzbereiche für zukünftige Verwendung archivieren.\n\n"
											L"Empfangen: Internes Archiv für Referenzbereiche wiederherstellen.\n",								// German
											L"Enviar: archivar intervalos de referencia para usos futuros.\n\n"
											L"Recuperar: reestablecer un archivo de intervalos de referencia interno.\n",						// Spanish
											L"Invia: archivia range di riferimento per utilizzo futuro.\n\n"
											L"Recupera: ripristina un archivio di range di riferimento interno.\n",								// Italian
											L"Enviar: Guardar intervalos de referência para utilização futura.\n\n"
											L"Recuperar: Restaura um ficheiro de intervalos de referência interno.\n",							// Portuguese
											L"Enviar: Arquive os intervalos de referência para futura referência.\n\n"
											L"Recuperar: Restaure um arquivo de intervalo de referência interno.\n"								// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_SEND				,	{	L"Printer: Print a copy of a demographic reference range.\n\n"
											L"Internal Archive: Save a copy of all active reference ranges internal to the analyzer for future recall.\n\n"
											L"External Archive: Transmit an ASCII dump of a demographic reference range to an external system.\n",																	// English
											L"Imprimante : imprimer un exemplaire de fourchette de référence démographique.\n\n"
											L"Archive interne : enregistrer un exemplaire de toutes les fourchettes de référence actives internes à l'analyseur afin de pouvoir les récupérer ultérieurement.\n\n"
											L"Archive externe : transmettre un vidage de mémoire ASCII d'une fourchette de référence démographique à un système externe.\n",											// French
											L"Drucker: Kopie eines Demographie-Referenzbereichs drucken.\n\n"
											L"Internes Archiv: Eine Kopie aller aktiven Referenzbereiche für spätere Verwendung intern im Analysegerät speichern.\n\n"
											L"Externes Archiv: ASCII-Auszug eines Demographie-Referenzbereichs an ein externes System übertragen.\n",																// German
											L"Impresora: imprimir una copia de un intervalo de referencia de demografía.\n\n"
											L"Archivo interno: guardar una copia de todos los intervalos de referencia activos internos para el analizador para usos futuros.\n\n"
											L"Archivo externo: hacer un volcado de datos ASCII de un intervalo de referencia de demografía a un sistema externo.\n",													// Spanish
											L"Stampante: stampa una copia di un range di riferimento per demografia.\n\n"
											L"Archivio interno: salva una copia di tutti i range di riferimento attivi interni all'analizzatore per utilizzo futuro.\n\n"
											L"Archivio esterno: trasmette una serie ASCII di range di riferimento per demografia ad un sistema esterno.\n",															// Italian
											L"Impressora: Imprime uma cópia de um intervalo de referência de demografia.\n\n"
											L"Ficheiro interno: Guardar uma cópia de todos os intervalos de referência ativos internos no analisador para utilização futura.\n\n"
											L"Ficheiro externo: Transmitir um dump de ASCII de um intervalo de referência de demografia para um sistema externo.\n",													// Portuguese
											L"Impressora: Imprima uma cópia de um intervalo de referência de dados demográficos.\n\n"
											L"Arquivo interno: Salve uma cópia de todos os intervalos de referência ativos internos no analisador, para futura recuperação.\n\n"
											L"Arquivo externo: Transmita um despejo ASCII de um intervalo de referência de dados demográficos para um sistema externo.\n"											// Portuguese (Brazil)
										} 
	},
	{ ISTR_ASTM_ORDER_METHOD		,	{	L"Work List: Select the order from a list.\n\n"
											L"Enter Patient ID: Select the order from a list then confirm order based on entered patient ID.\n\n"
											L"Disabled: ASTM orders are disabled.\n",																												// English
											L"Liste de tâches : Sélectionner l'ordre dans une liste.\n\n"
											L"Entrer ID patient : Sélectionner l'ordre dans une liste, puis confirmer l'ordre en function de l'ID patient entrée.\n\n"
											L"Désactivé : Les orders ASTM sont  désactivés.",																										// French
											L"Auftragsliste: Die Anforderung aus einer Liste auswählen.\n\n"
											L"Patienten-ID eingeben: Die Anforderung aus einer Liste auswählen und Anforderung anhand der Patienten-ID bestätigen.\n\n"
											L"Deaktiviert: ASTM-Anforderungen sind deaktiviert.",																									// German
											L"Lista de trabajo: Seleccione el idioma en una lista.\n\n"
											L"Introducir Id. del paciente: Seleccione el pedido en una lista y, a continuación, confirme el pedido en función del Id. del paciente introducido.\n\n"
											L"Desactivado: Los pedidos ASTM están desactivados.",																									// Spanish
											L"Elenco di lavoro: Selezionare l'ordine da un elenco.\n\n"
											L"Inserisci ID paziente: Selezionare l'ordine da un elenco, quindi confermarlo in base all'ID paziente inserito.\n\n"
											L"Disattivato: Gli ordini ASTM sono disattivati.",																										// Italian
											L"Lista de trabalho: Seleciona o idioma de uma lista\n\n"
											L"Introduzir ID de doente: Seleciona o pedido de uma lista e, em seguida, confirma o pedido baseado na ID do doente introduzido.\n\n"
											L"Inativo: Os pedidos de ASTM estão desativados.\n",																										// Portuguese
											L"Lista de trabalho: Selecione a solicitação em uma lista\n\n"
											L"Inserir ID do paciente: Selecione a solicitação em uma lista e confirme-a com base no ID do paciente inserido.\n\n"
											L"Desativado: As solicitações de ASTM estão desativadas.\n"																								// Portuguese (Brazil)
										}
	},
	{ ISTR_ASTM_PHYSICAL_LAYER		,	{	L"USB (Serial): The analyzer is connected from the USB \"B\" port to a local computer using a USB cable.\n\n"
											L"TCP/IP: The analyzer is connected using a USB-to-Ethernet adapter to a network.\n",																	// English
											L"USB (série) : l'analyseur est connecté du port USB \"B\" à un ordinateur local par un câble USB.\n\n"
											L"TCP/IP: l'analyseur est connecté à un réseau par le biais d'un adaptateur USB-Ethernet.\n",															// French
											L"USB (serieller Anschluss): Das Analysesystem ist über den USB-Anschluss \"B\" und ein USB-Kabel an einen lokalen Computer angeschlossen.\n\n"
											L"TCP/IP: Das Analysegerät ist über einen USB- Ethernet-Adapter an ein Netzwerk angeschlossen.\n",														// German
											L"USB (serie): El analizador está conectado desde un puerto USB \"B\" a un ordenador local mediante un cable USB.\n\n"
											L"TCP/IP: El analizador está conectado a la red mediante un adaptador USB a Ethernet.\n",																// Spanish
											L"USB (seriale): L'analizzatore è connesso dalla porta USB \"B\" a un computer locale tramite un cavo USB.\n\n"
											L"TCP/IP: L'analizzatore è connesso alla rete tramite un adattatore USB-Ethernet.\n",																	// Italian
											L"USB (série): O analisador está ligado a partir da porta USB \"B\" de um computador local através de um cabo USB.\n\n"
											L"TCP/IP: O analisador está ligado a uma rede através de um adaptador USB para Ethernet.\n",																// Portuguese
											L"USB (serial): O analisador é conectado da porta USB \"B\" a um computador local usando um cabo USB.\n\n"
											L"TCP/IP: O analisador é conectado usando um adaptador USB para Ethernet em uma rede.\n"																	// Portuguese (Brazil)
										}
	},	
	{ ISTR_CD_UPDATE				,	{	L"The CD ROM drive is located on the rear of the analyzer. Press the small rectangular button to open the drawer. Insert the CD onto the spindle and make sure it clicks into place. Close the CD ROM drive. Press Continue. After the analyzer has restarted, open the CD ROM drive and remove the CD. Store the CD in a safe place.\n",														// English
											L"Le lecteur de CD ROM se trouve à l'arrière de l'analyseur. Appuyer sur le petit bouton rectangulaire pour ouvrir le tiroir. Insérer le CD sur l'axe et vérifier qu'il est bien en place. Fermer le lecteur de CD ROM. Appuyer sur Continuer. Une fois que l'analyseur a redémarré, ouvrir le lecteur de CD ROM et retirer le CD. Ranger le CD en lieu sûr.\n",									// French
											L"Das CD-ROM-Laufwerk befindet sich auf der Rückseite des Analysegeräts. Zur Öffnung des Fachs die kleine rechteckige Taste drücken. Die CD in die Spindel einsetzen und einrasten lassen. Schließen Sie das CD-ROM-Laufwerk. Drücken Sie auf Weiter. Nach dem Neustart des Analysegeräts öffnen Sie das CD-ROM-Laufwerk und entnehmen die CD. Verwahren Sie die CD an einem sicheren Ort.\n",	// German
											L"La unidad de CD-ROM está en la parte trasera del analizador. Pulse el botón rectangular pequeño para abrir la bandeja. Inserte el CD en el eje y asegúrese de que hace clic cuando se coloca en su sitio. Cierre la unidad de CD-ROM. Pulse Continuar. Después de reiniciar el analizador, abra la unidad de CD-ROM y retire el CD. Guarde el CD en un lugar seguro.\n",						// Spanish
											L"Il drive del CD ROM si trova sul retro dell'analizzatore. Per aprire il cassetto premere il piccolo pulsante rettangolare. Inserire il CD sul supporto e assicurarsi che sia ben fissato. Chiudere il drive del CD ROM. Premere Continua. Una volta riavviato l'analizzatore, aprire il drive del CD ROM e rimuovere il CD. Conservare il CD in un luogo sicuro.\n",							// Italian
											L"A unidade de CD-ROM está localizada na parte posterior do analisador. Prima o botão retangular pequeno para abrir a gaveta. Insira o CD no eixo e certifique-se de que ouve um clique quando é colocado no lugar. Feche a unidade de CD-ROM. Prima Continuar. Depois de o analisador ter reiniciado, abra a unidade de CD-ROM e retire o CD. Guarde o CD num local seguro.\n",					// Portuguese
											L"A unidade de CD-ROM está localizada na parte traseira do analisador. Pressione o pequeno botão retangular para abrir a gaveta. Insira o CD no eixo e verifique se ele se encaixa no lugar. Feche a unidade de CD-ROM. Pressione Continuar. Depois que o analisador reiniciar, abra a unidade de CD-ROM e remova o CD. Guarde o CD em um local seguro.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_COMM_PROTOCOL			,	{	L"None: No transmission of information is possible.\n\n"
											L"ASCII Text: ASCII text dump of information unidirectional from the analyzer.\n\n"
											L"ASTM: ASTM 1394 E 97 over ASTM E 1381 02 Standard Specification for Transferring Information Between Clinical Instruments and Computer Systems.\n\n"
											L"XML: Extensible Markup Language output in proprietary Abaxis format.\n",																							// English
											L"Aucune : aucune transmission d'information n'est possible.\n\n"
											L"Texte ASCII : vidage de texte ASCII d'information unidirectionnelle à partir de l'analyseur.\n\n"
											L"ASTM : Spécification des normes ASTM 1394 E 97/ASTM E 1381 02 pour la transmission de données entre des instruments cliniques et des systèmes informatiques.\n\n"
											L"XML : Sortie en langage XML (Extensible Markup Language) dans le format propriétaire d'Abaxis.\n",																	// French
											L"Keine: Keine Datenübertragung möglich.\n\n"
											L"ASCII-Text: ASCII-Textauszug einseitig vom Analysegerät.\n\n"
											L"ASTM: ASTM 1394 E 97 über ASTM E 1381 02 Standardspezifikation für die Übertragung von Informationen zwischen klinischen Geräten und Computersystemen.\n\n"
											L"XML: Ausgabe als Extensible Language in proprietärem Abaxis-Format.\n",																							// German
											L"Ninguna: no es posible realizar la transmisión de la información.\n\n"
											L"Texto ASCII: volcado de datos en texto ASCII unidireccional desde el analizador.\n\n"
											L"ASTM 1394 E 97 sobre ASTM E 1381 02 Especificación estándar sobre transferencia de información entre instrumentos clínicos y sistemas informáticos.\n\n"
											L"XML: Salida en lenguaje de marcado extensible en formato propiedad de Abaxis.\n",																					// Spanish
											L"Nessuno: non è possibile alcuna trasmissione di informazioni.\n\n"
											L"Testo ASCII: serie unidirezionale di informazioni in testo ASCII dall'analizzatore.\n\n"
											L"ASTM: ASTM 1394 E 97 sulla specifica standard ASTM E 1381 02 per il trasferimento di informazioni tra strumenti clinici e computer.\n\n"
											L"XML: Output di tipo Extensible Markup Language in formato proprietario Abaxis.\n",																					// Italian
											L"Nenhum: Não é possível realizar a transmissão de informação.\n\n"
											L"Texto ASCII: Dump de informações de texto ASCII, unidirecional do analisador.\n\n"
											L"ASTM: ASTM 1394 E 97 sobre ASTM E 1381 02 Especificação padrão sobre transferência de informação entre instrumentos clínicos e sistemas informáticos.\n\n"
											L"XML: Saída em linguagem de marcação extensível em formato propriedade da Abaxis.\n",																				// Portuguese
											L"Nenhum: Nenhuma transmissão de informações é possível.\n\n"
											L"Texto ASCII: Despejo em texto ASCII de informações unidirecionais do analisador.\n\n"
											L"ASTM: ASTM 1394 E 97 via ASTM E 1381 02 Especificação padrão para transferência de informações entre instrumentos clínicos e sistemas de computadores.\n\n"
											L"XML: Saída da linguagem de marcação extensível no formato patenteado da Abaxis.\n"																					// Portuguese (Brazil)
										} 
	},
	{ ISTR_DATE_INVALID				,	{	L"Only valid calendar days may be entered. Please set the date to today's date.\n",														// English
											L"Saisir uniquement des jours civils valides. Définir la date d'aujourd'hui comme date du jour.\n",										// French
											L"Nur gültige Kalendertage eingeben. Datumsanzeige auf das heutige Datum einstellen.\n",													// German
											L"Solo se pueden introducir días naturales correctos. Cambie la fecha al día de hoy.\n",													// Spanish
											L"Possono essere inseriti solo giorni validi. Si prega di impostare la data odierna.\n",													// Italian
											L"Apenas podem ser introduzidos dias de calendário válidos. Defina a data para a data do dia de hoje.\n",								// Portuguese
											L"Apenas dias corridos válidos podem ser inseridos. Configure a data como a data de hoje.\n"												// Portuguese (Brazil)
										} 
	},
	{ ISTR_DELETE_ADMINISTRATOR		,	{	L"Removing an administrator from the administrator list will permanently remove the administrator, and all information for that administrator will be lost.\n",					// English
											L"La suppression d'un administrateur dans la liste d'administrateurs a pour effet de le supprimer et toutes les informations associées seront perdues.\n",						// French
											L"Die Entfernung eines Administrators aus der Administratorliste entfernt den Administrator dauerhaft, und sämtliche Informationen zu diesem Administrator gehen verloren.\n",	// German
											L"Al eliminar un administrador de la lista de administradores se le elimina permanentemente. Se perderá toda la información de ese administrador.\n",							// Spanish
											L"La rimozione di un amministratore dall'elenco amministratori sarà permanente e tutte le informazioni per tale amministratore saranno perse.\n",								// Italian
											L"Ao eliminar um administrador da lista de administradores irá eliminar permanentemente o administrador, e toda a informação desse administrador será perdida.\n",				// Portuguese
											L"A remoção de um administrador da lista de administradores o removerá permanentemente e todas as informações desse administrador serão perdidas.\n"								// Portuguese (Brazil)
										} 
	},
	{ ISTR_DELETE_OPERATOR			,	{	L"Removing an operator from the operator list will permanently remove the operator, and all information for that operator will be lost.\n",										// English
											L"La suppression d'un opérateur dans la liste d'opérateurs a pour effet de le supprimer et toutes les informations associées seront perdues.\n",									// French
											L"Die Entfernung eines Bedieners aus der Bedienerliste entfernt den Bediener dauerhaft, und sämtliche Informationen zu diesem Bediener gehen verloren.\n",						// German
											L"Al eliminar un operario de la lista de operarios se le elimina permanentemente. Se perderá toda la información de ese operario.\n",											// Spanish
											L"La rimozione di un operatore dall'elenco operatori sarà permanente e tutte le informazioni per tale operatore saranno perse.\n",												// Italian
											L"Ao eliminar um operador da lista de operadores irá eliminar permanentemente o operador, e toda a informação desse operador será perdida.\n",									// Portuguese
											L"A remoção de um operador da lista de operadores o removerá permanentemente e todas as informações desse operador serão perdidas.\n"											// Portuguese (Brazil)
										} 
	},
	{ ISTR_DISPLAY_SETTINGS			,	{	L"Backlight: Controls the backlight level for the display.\n\n"
											L"Screen Saver: Sets the time from the last screen press until the screen saver starts.\n\n"
											L"Power Save: Sets the time from the last screen press until the display is turned off.\n\n"
											L"Calibrate: Aligns the touch screen to the LCD display.\n",																				// English
											L"Rétroéclairage : commande le niveau de rétroéclairage de l'écran.\n\n"
											L"Économiseur d'écran : définit la durée entre la dernière activation de l'écran et la mise en marche de l'économiseur d'écran.\n\n"
											L"Économiseur d'alimentation : définit la durée entre la dernière activation de l'écran et le moment où l'écran est arrêté.\n\n"
											L"Calibrage : alignement de l'écran tactile avec l'écran à cristaux liquides.\n",														// French
											L"Hintergrundbeleuchtung: Steuert die Hintergrundbeleuchtung des Bildschirms.\n\n"
											L"Bildschirmschoner: Zur Einstellung des Zeitintervalls von der letzten Bildschirmberührung bis zum Start des Bildschirmschoners.\n\n"
											L"Energiesparmodus: Zur Einstellung der Zeit von der letzten Bildschirmberührung bis zur Abschaltung des Bildschirms.\n\n"
											L"Kalibrieren: Richtet den Touch-Screen am LCD-Bildschirm aus.\n",																		// German
											L"Luz de fondo: controla el nivel de la luz de fondo de la pantalla.\n\n"
											L"Salvapantalla: marca el tiempo desde la última presión sobre la pantalla hasta la activación del salvapantalla.\n\n"
											L"Ahorro de energía: marca el tiempo desde la última presión de pantalla hasta que la pantalla se apaga.\n\n"
											L"Calibrar: alinea la pantalla táctil a la pantalla de cristal líquido.\n",																// Spanish
											L"Retroilluminazione: controlla il livello di retroilluminazione per il display.\n\n"
											L"Screensaver: imposta il tempo tra l'ultima pressione sullo schermo e l'avvio dello screensaver.\n\n"
											L"Modo risparmio: imposta il tempo tra l'ultima pressione sullo schermo allo spegnimento del display.\n\n"
											L"Calibra: allinea il touchscreen al display LCD.\n",																					// Italian
											L"Luz de fundo: Controla o nível da luz de fundo do visor.\n\n"
											L"Proteção do ecrã: Define o tempo desde a última vez que o ecrã é premido até ao início da proteção do ecrã.\n\n"
											L"Poupança de energia: Define o tempo desde a última vez que o ecrã é premido até que o visor é desligado.\n\n"
											L"Calibrar: Alinha o ecrã tátil com o visor LCD.\n",																						// Portuguese
											L"Luz de fundo: Controla o nível de luz de fundo do visor.\n\n"
											L"Protetor de tela: Configura o tempo desde a última pressão de tela até a proteção de tela iniciar.\n\n"
											L"Economia de energia: Configura o tempo desde a última pressão de tela até o display desligar.\n\n"
											L"Calibrar: Alinha a tela sensível ao toque com o display LCD.\n"																		// Portuguese (Brazil)
										} 
	},
	{ ISTR_FACTORY_DEFAULTS			,	{	L"Restoring the analyzer to factory defaults resets the analyzer to the same conditions as when the analyzer left the factory. It is recommended that the reference ranges be archived prior to resetting the analyzer to factory defaults. Saved results and internal reference range archives will still be available after the settings are reset to factory defaults.\n",																						// English
											L"Le fait de rétablir les valeurs par défaut de l'analyseur remet l'analyseur en état sortie d'usine. Il est conseillé d'archiver les fourchettes de référence avant de rétablir les valeurs par défaut de l'analyseur. Les résultats enregistrés et les archives des fourchettes de référence internes seront toujours disponibles une fois les paramètres par défaut rétablis.\n",																					// French
											L"Die Wiederherstellung der Werkseinstellungen des Analysegeräts stellt die Einstellungen des Analysegeräts auf den Auslieferungszustand zurück. Es wird empfohlen, die Referenzbereiche vor dem Zurücksetzen der Referenzbereiche auf die Werkseinstellungen zu archivieren. Gespeicherte Ergebnisse und interne Referenzbereichsarchive sind auch nach Wiederherstellung der Werkseinstellungen verfügbar.\n",														// German
											L"La restauración del analizador a los valores predeterminados originales reestablece las mismas condiciones de fábrica del analizador. Se recomienda archivar los intervalos de referencia antes de reestablecer los valores predeterminados de fábrica del analizador. Los archivos de resultados y los intervalos de referencia internos estarán disponibles después de reestablecer los valores predeterminados originales del analizador.\n",					// Spanish
											L"Se si ripristinano le impostazioni predefinite, l'analizzatore ritorna alle condizioni in cui ha lasciato la fabbrica. Si raccomanda di archiviare i range di riferimento prima di resettare l'analizzatore sulle impostazioni di fabbrica. I risultati salvati e gli archivi dei range di riferimento interni saranno disponibili anche dopo aver ripristinato le impostazioni di fabbrica.\n",																	// Italian
											L"O restauro do analisador para os valores predeterminados de fábrica restaura o analisador para as mesmas condições do analisador quando saiu da fábrica. É recomendado arquivar os intervalos de referência antes de restaurar o analisador para os valores predeterminados de fábrica. Os ficheiros de resultados e dos intervalos de referência internos continuarão disponíveis após o restauro do analisador para os valores predeterminados de fábrica.\n",	// Portuguese
											L"A restauração do analisador para os padrões de fábrica o redefine para as mesmas condições de quando ele saiu da fábrica. É recomendável que os intervalos de referência sejam arquivados antes de redefinir o analisador para os padrões de fábrica. Os resultados salvos e os arquivos do intervalo de referência interno ainda estarão disponíveis depois que as configurações forem redefinidas para os padrões de fábrica.\n"									// Portuguese (Brazil)
										} 
	},
	{ ISTR_HL7_SETTINGS				,	{	L"HL7 Format: Provides a list of EMR/EHR systems that have specific HL7 configuration as part of the analyzer settings. If your EMR/EHR system is not listed, try the Generic HL7 setting. If that does not work correctly, contact your EMR/EHR provider and Abaxis Technical Support to get them added to the list.\n",																					// English
											L"Format HL7 : Répertorie les systèmes EMR/EHR ayant une configuration HL7 spécifique au niveau des paramètres de l’analyseur. Si votre système EMR/EHR n’est pas répertorié, essayer le paramètre HL7 générique. Si cela ne fonctionne pas correctement, contacter votre fournisseur EMR/EHR et le support technique d’Abaxis pour les ajouter à la liste.\n",												// French
											L"HL7-Format: Enthält eine Liste von EMR/EHR-Geräten mit spezifischer HL7-Konfiguration als Teil der Geräteeinstellungen. Ist Ihr EMR/EHR-Gerät nicht darin enthalten, so wählen Sie die allgemeine HL7-Einstellung. Ist auch dies nicht erfolgreich, wenden Sie sich an den Anbieter Ihres EMR/EHR-Gerätes sowie den technischen Support von Abaxis, um das Gerät der Liste hinzufügen zu lassen.\n",		// German 
											L"Formato HL7: Proporciona una lista de sistemas EMR/EHR que tienen una configuración específica de HL7 como parte de los ajustes del analizador. Si no aparece su sistema EMR/EHR en la lista, pruebe con el ajuste Genérico HL7. Si no funciona correctamente, póngase en contacto con su proveedor de EMR/EHR y con el servicio técnico de Abaxis para que lo añadan a la lista.\n",						// Spanish 
											L"Formato HL7: Fornisce un elenco dei sistemi EMR/EHR che presentano configurazioni HL7 specifiche come parte delle impostazioni dell'analizzatore. Se il proprio sistema EMR/EHR non è nell'elenco, provare con le impostazioni HL7 generiche. Nel caso di funzionamento errato, contattare il proprio fornitore EMR/EHR e l'assistenza tecnica Abaxis per ottenere l'aggiunta all'elenco.\n", 				// Italian 
											L"Formato HL7: Fornece uma lista de sistemas EMR/EHR que têm uma configuração HL7 específica como parte das configurações do analisador.  Se o seu sistema EMR/EHR não aparecer na lista, tente a configuração HL7 genérica.  Se não funcionar adequadamente, contacte o seu fornecedor de EMR/EHR e o serviço de assistência técnica da Abaxis para o adicionarem à lista.\n",								// Portuguese
											L"Formato HL7: Fornece uma lista de sistemas de EMR/EHR que possuem configuração específica do HL7 como parte das configurações do analisador.  Se o sistema EMR/EHR não estiver listado, tente a configuração HL7 genérica.  Se isso não funcionar corretamente, entre em contato com o provedor de EMR/EHR e o Suporte Técnico da Abaxis para adicioná-lo à lista. \n"										// Portuguese (Brazil)
									  } 	
	},
	{ ISTR_LANGUAGE_SET				,	{	L"Both: Both the display and the external keyboard will be set to the same language at the same time.\n\n"
											L"Display: Only the language used on the display will be changed.\n\n"
											L"Keyboard: Only the keyboard type will be changed. The keyboard selected must match the keyboard used.\n",										// English
											L"Les deux : l'écran et le clavier externe seront dans la même langue en même temps.\n\n"
											L"Écran : seule la langue utilisée à l'écran sera modifiée.\n\n"
											L"Clavier : seule le type de clavier sera modifié. Le clavier sélectionné doit correspondre au clavier utilisé.\n",								// French
											L"Beide: Anzeige und externes Eingabegerät werden gleichzeitig auf dieselbe Sprache eingestellt.\n\n"
											L"Anzeige: Nur die Sprache der Anzeige wird verändert.\n\n"
											L"Eingabegerät: Nur der Eingabegerätetyp wird verändert. Das ausgewählte Eingabegerät muss mit dem verwendeten Eingabegerät übereinstimmen.\n",	// German
											L"Ambos: se establecerán simultáneamente tanto la pantalla como el teclado externo con el mismo idioma.\n\n"
											L"Pantalla: solo se cambiará el idioma de la pantalla.\n\n"
											L"Teclado: solo cambiará el tipo de teclado. El teclado seleccionado deberá coincidir con el teclado usado.\n",									// Spanish
											L"Entrambi: sia il display che la tastiera esterna saranno impostati sulla stessa lingua nello stesso momento.\n\n"
											L"Display: viene modificata solo la lingua utilizzata dal display.\n\n"
											L"Tastiera: viene modificato solo il tipo di tastiera. La tastiera selezionata deve coincidere con la tastiera utilizzata.\n",					// Italian
											L"Ambos: O visor e o teclado externo serão definidos simultaneamente com o mesmo idioma.\n\n"
											L"Visor: Apenas será alterado o idioma utilizado no visor.\n\n"
											L"Teclado: Apenas será alterado o tipo de teclado. O teclado selecionado deverá corresponder ao teclado utilizado.\n",							// Portuguese
											L"Ambos: O display e o teclado externo serão configurados para o mesmo idioma ao mesmo tempo.\n\n"
											L"Display: Apenas o idioma usado no display será alterado.\n\n"
											L"Teclado: Apenas o tipo de teclado será alterado. O teclado selecionado deve corresponder ao teclado usado.\n"									// Portuguese (Brazil)
										} 
	},
	{ ISTR_NETWORK_TYPE				,	{	L"Static IP: The network settings are set manually and will not change when the analyzer is powered on.  Please contact your IT group for the correct settings for your organization.\n\n"
											L"DHCP: The network settings are obtained automatically by the analyzer from a DHPC server on the network.\n\n"
											L"Both: The ASTM and HL7 settings need to be set manually.  Please contact your IT group for the correct settings for your organization.\n" ,		// English
											L"IP statique : Les paramètres du réseau sont définis manuellement et ne seront pas modifiés à la mise en marche de l’analyseur. Veuillez contacter votre groupe informatique pour obtenir les bons paramètres pour votre organisation.\n\n"
											L"DHCP : L’analyseur obtient automatiquement les paramètres du réseau depuis un serveur DHCP du réseau.\n\n"
											L"Les deux : Les paramètres ASTM et HL7 doivent être définis manuellement. Veuillez contacter votre groupe informatique pour obtenir les bons paramètres pour votre organisation.\n",     //French
											L"Statische IP: Die Netzwerkeinstellungen werden manuell vorgenommen und bleiben bei Einschalten des Analysegerätes unverändert. Wenden Sie sich an Ihre IT-Abteilung, um die korrekten Einstellungen für Ihr Unternehmen zu erfahren.\n\n"
											L"DHCP: Die Netzwerkeinstellungen werden automatisch vom Analysegerät über einen DHCP-Netzwerkserver abgerufen.\n\n"
											L"Beide: Die Einstellungen für ASTM und HL7 sind manuell vorzunehmen. Wenden Sie sich an Ihre IT-Abteilung, um die korrekten Einstellungen für Ihr Unternehmen zu erfahren.\n",			// German		
											L"IP estática: Los ajustes de red se definen manualmente y no cambiarán cuando se encienda el analizador. Póngase en contacto con su departamento de informática para elegir los ajustes adecuados para su organización.\n\n"
											L"DHCP: Los ajustes de red se obtienen automáticamente mediante el analizador de un servidor DHCP de la red.\n\n"
											L"Ambos: Los ajustes de ASTM y HL7 se tienen que definir manualmente. Póngase en contacto con su departamento de informática para elegir los ajustes adecuados para su organización.\n",	// Spanish	
											L"IP statico: Le impostazioni di rete sono impostate manualmente e non verranno modificate all'accensione dell'analizzatore. Contattare il gruppo IT per le impostazioni corrette per la propria azienda.\n\n"
											L"DHCP: Le impostazioni di rete vengono ottenute automaticamente mediante l'analizzatore da un server DHCP sulla rete.\n\n"
											L"Entrambi: Sia l'ASTM sia l'HL7 devono essere impostati manualmente. Contattare il gruppo IT per le impostazioni corrette per la propria azienda.\n",									// Italian
											L"IP estático: As configurações de rede são configuradas manualmente e não se alterarão quando o analisador for ligado.  Contacte o seu Departamento de TI para selecionar as configurações adequadas para a sua organização.\n\n"
											L"DHCP: As configurações de rede são obtidas automaticamente pelo analisador através de um servidor DHCP da rede. \n\n"
											L"Ambos: É necessário definir manualmente as configurações ASTM e HL7.  Contacte o seu Departamento de TI para selecionar as configurações adequadas para a sua organização.\n",		// Portuguese
											L"IP estático: As configurações de rede são configuradas manualmente e não serão alteradas quando o analisador estiver ligado.  Entre em contato com o grupo de TI para obter as configurações corretas para sua organização.\n\n"
											L"DHCP: As configurações de rede são obtidas automaticamente pelo analisador de um servidor DHPC na rede.\n\n"
											L"Ambos: As configurações ASTM e HL7 precisam ser configuradas manualmente.  Entre em contato com o grupo de TI para obter as configurações corretas para sua organização.\n"		// Portuguese (Brazil)	
									    }
 
	},
	{ ISTR_PRINTER_CONFIGURE		,	{	L"Set Default: Select the printer to use for printing.\n\n"
											L"Select Reports: Select the type and number of reports.\n",									// English
											L"Imprimante par défaut : sélectionner l'imprimante à utiliser pour l'impression.\n\n"
											L"Sélectionner des rapports : sélectionner le type et le nombre de rapports.\n",				// French
											L"Standard einstellen: Drucker auswählen.\n\n"
											L"Berichte auswählen: Typ und Zahl der Berichte auswählen.\n",								// German
											L"Establecer valores predeterminados: seleccione la impresora que se va a utilizar.\n\n"
											L"Seleccionar informes: seleccione el tipo y número de informes.\n",							// Spanish
											L"Imposta come predefinito: seleziona la stampante da utilizzare per la stampa.\n\n"
											L"Seleziona rapporti: seleziona il tipo e il numero di rapporti.\n",							// Italian
											L"Configurar valores predeterminados: Selecione a impressora a utilizar para imprimir.\n\n"
											L"Selecionar relatórios: Selecione o tipo e o número de relatórios.\n",						// Portuguese
											L"Configurar padrão: Selecione a impressora a ser usada para a impressão.\n\n"
											L"Selecionar relatórios: Selecione o tipo e o número de relatórios.\n"						// Portuguese (Brazil)
										} 
	},
	{ ISTR_PRINTER_SELECT_REPORTS	,	{	L"Results: Select the number of results reports to print at the end of each analysis.\n\n"
											L"iQC: Select the number of iQC reports to print at the end of each analysis.\n\n"
											L"Error: Select the number of error reports to print at the end of each analysis. The reports can be printed automatically when there is an error, or may be printed for every analysis.\n",								// English
											L"Résultats : sélectionner le nombre de rapports de résultats à imprimer à l'issue de chaque analyse.\n\n"
											L"iQC : sélectionner le nombre de rapports iQC à imprimer à l'issue de chaque analyse.\n\n"
											L"Erreur : sélectionner le nombre de rapports d'erreurs à imprimer à l'issue de chaque analyse. Des rapports peuvent s'imprimer automatiquement en cas d'erreur ou bien pour chaque analyse.\n",							// French
											L"Ergebnisse: Anzahl der nach Abschluss jeder Analyse zu druckenden Ergebnisberichte auswählen.\n\n"
											L"iQC: Anzahl der nach Abschluss jeder Analyse zu druckenden iQC-Berichte auswählen.\n\n"
											L"Fehler: Anzahl der nach Abschluss jeder Analyse zu druckenden Fehlerberichte auswählen. Bei Vorliegen eines Fehlers können die Berichte automatisch oder für jede Analyse gedruckt werden.\n",							// German
											L"Resultados: seleccione el número de informes de resultados para imprimir al final de cada análisis.\n\n"
											L"iQC: seleccione el número de informes de iQC para imprimir al final de cada análisis.\n\n"
											L"Error: seleccione el número de informes de error para imprimir al final de cada análisis. Los informes se pueden imprimir automáticamente cuando hay un error, o se pueden imprimir con cada análisis.\n",				// Spanish
											L"Risultati: seleziona il numero di rapporti di risultati da stampare al termine di ogni analisi.\n\n"
											L"iQC: seleziona il numero di rapporti di iQC da stampare al termine di ogni analisi.\n\n"
											L"Errore: seleziona il numero di rapporti di errore da stampare al termine di ogni analisi. I rapporti possono essere stampati automaticamente in caso di errore, oppure essere stampati per ogni analisi.\n",			// Italian
											L"Resultados: Seleccone o número de relatórios de resultados a imprimir no final de cada análise.\n\n"
											L"iQC: Selecione o número de relatórios de iQC a imprimir no final de cada análise.\n\n"
											L"Erro: Selecione o número de relatórios de erros a imprimir no final de cada análise. Os relatórios podem ser impressos automaticamente quando existir um erro, ou podem ser impressos para cada análise.\n",			// Portuguese
											L"Resultados: Selecione o número de relatórios de resultados a serem impressos no final de cada análise.\n\n"
											L"iQC: Selecione o número de relatórios de iQC a serem impressos no final de cada análise.\n\n"
											L"Erro: Selecione o número de relatórios de erro a serem impressos no final de cada análise. Os relatórios podem ser impressos automaticamente quando houver um erro ou podem ser impressos para todas as análises.\n"	// Portuguese (Brazil)
										} 
	},
	{ ISTR_PRINTER_SETUP			,	{	L"Configure: Select the printer to use and the number and type of reports to print.\n\n"
											L"Test: Print a test page on the printer.\n",																			// English
											L"Configuration : sélectionner l'imprimante à utiliser, le nombre et le type de rapports à imprimer.\n\n"
											L"Test : imprimer une page de test sur l'imprimante.\n",																	// French
											L"Konfigurieren: Verwendeten Drucker sowie Zahl und Typ der zu druckenden Berichte auswählen.\n\n"
											L"Test: Testseite drucken.\n",																							// German
											L"Configurar: seleccione la impresora que se va a utilizar y el número y tipo de informes que se van a imprimir.\n\n"
											L"Test: imprima una página de prueba en la impresora.\n",																// Spanish
											L"Configura: seleziona la stampante da usare, nonché il numero e il tipo di rapporti da stampare.\n\n"
											L"Prova: stampa una pagina di prova.\n",																					// Italian
											L"Configurar: Selecione a impressora a utilizar e o número e tipo de relatórios a imprimir.\n\n"
											L"Teste: Imprima uma página de teste na impressora.\n",																	// Portuguese
											L"Configurar: Selecione a impressora a ser usada e o número e tipo de relatórios a serem impressos.\n\n"
											L"Teste: Imprima uma página de teste na impressora.\n"																	// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_ADD			,	{	L"Special n: The next available generic demographic label between 1 and 99.\n\n"
											L"Control n: The next available control label between 1 and 99.\n\n",													// English
											L"Spécial n : la prochaine étiquette démographique générique disponible comprise entre 1 et 99.\n\n"
											L"Contrôle n : la prochaine étiquette de contrôle disponible comprise entre 1 et 99.\n",									// French
											L"Spezielles n: Das nächste verfügbare Demographie-Etikett zwischen 1 und 99.\n\n"
											L"Steuerung n: Das nächste verfügbare Steuerungsetikett zwischen 1 und 99.\n",											// German
											L"Especial n: la próxima etiqueta de demografía genérica disponible entre 1 y 99.\n\n"
											L"Control n: la próxima etiqueta de control disponible entre 1 y 99.\n",													// Spanish
											L"Speciale n: la successiva etichetta di demografia generica disponibile tra 1 e 99.\n\n"
											L"Controllo n: la successiva etichetta di controllo disponibile tra 1 e 99.\n",											// Italian
											L"Especial n: A próxima etiqueta de demografia genérica disponível entre 1 e 99.\n\n"
											L"Controlo n: A próxima etiqueta de controlo disponível entre 1 e 99.\n",												// Portuguese
											L"N especial: O próximo rótulo de dado demográfico genérico disponível entre 1 e 99.\n\n"
											L"N controle: O próximo rótulo de controle disponível entre 1 e 99.\n"													// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_INCLUDE		,	{	L"Last Modified XXX: The last reference range modified.\n\n"
											L"All: Allows the user to select from a list of all reference ranges known to the analyzer.\n",														// English
											L"Dernière modification XXX : la dernière fourchette de référence modifiée.\n\n"
											L"Tous : permet à l'utilisateur d'effectuer une sélection à partir de la liste de toutes les fourchettes de références connues de l'analyseur.\n",	// French
											L"Letzte Änderung XXX: Letzter geänderter Referenzbereich.\n\n"
											L"Alle: Ermöglicht dem Benutzer die Auswahl aus einer Liste aller im Analysegerät verfügbaren Referenzbereiche.\n",									// German
											L"Última modificación XXX: el último intervalo de referencia modificado.\n\n"
											L"Todo: permite al usuario elegir de una lista de todos los intervalos de referencia conocidos por el analizador.\n",								// Spanish
											L"Ultima modifica XXX: l'ultimo range di riferimento modificato.\n\n"
											L"Tutto: consente all'utente di selezionare da un elenco tutti i range di riferimento noti all'analizzatore.\n",										// Italian
											L"Última modificação XXX: O último intervalo de referência modificado.\n\n"
											L"Tudo: Permite ao utilizador selecionar a partir de uma lista com todos os intervalos de referência conhecidos para o analisador.\n",				// Portuguese
											L"Última modificação de XXX: O último intervalo de referência modificado.\n\n"
											L"Todos: Permite ao usuário selecionar em uma lista de todos os intervalos de referência conhecidos pelo analisador.\n"								// Portuguese (Brazil)
										}
	},
	{ ISTR_REF_RANGES_MODIFY		,	{	L"Analyte: Provides a list of all active reference ranges for a particular analyte.\n\n"
											L"Demographic: Provides a list of all analytes for all reference ranges.\n",																			// English
											L"Substance à analyser : fournit une liste de toutes les fourchettes de référence actives pour une substance à analyser donnée.\n\n"
											L"Élément démographique : fournit une liste de toutes les substances à analyser pour toutes les fourchettes de référence.\n",						// French
											L"Analyt: Anzeige einer Liste aller aktiven Referenzbereiche für ein bestimmtes Analyt.\n\n"
											L"Demographie: Anzeige einer Liste aller Analyte für alle Referenzbereiche.\n",																		// German
											L"Analito: proporciona una lista de todos los intervalos de referencia activos para un analito particular.\n\n"
											L"Demografía: proporciona una lista de todos los analitos para todos los intervalos de referencia.\n",												// Spanish
											L"Analita: fornisce un elenco di tutti i range di riferimento attivi per un particolare analita.\n\n"
											L"Demografia: fornisce un elenco di tutti gli analiti per ogni range di riferimento.\n",																// Italian
											L"Analito: Fornece uma lista de todos os intervalos de referencia ativos para um analito específico.\n\n"
											L"Demografia: Fornece uma lista de todos os analitos para todos os intervalos de referência.\n",														// Portuguese
											L"Analito: Fornece uma lista de todos os intervalos de referência ativos para um determinado analito.\n\n"
											L"Dados demográficos: Fornece uma lista de todos os analitos para todos os intervalos de referência.\n"												// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_REMOVE		,	{	L"Removing a demographic from the sample type list will remove the item from the list of demographics shown when running a disc, and any values entered will be permanently lost.\n",																						// English
											L"La suppression d'un élément démographique dans la liste de type d'échantillons a pour effet de supprimer cet élément dans la liste d'éléments démographiques affichés lors de l'exécution d'un disque, et toutes les valeurs saisies seront irrémédiablement perdues.\n",	// French
											L"Die Entfernung einer Demographie aus der Probentypliste entfernt das Objekt aus der Demographieliste, die bei laufendem Rotor angezeigt wird, und sämtliche eingegebene Werte gehen dauerhaft verloren.\n",																// German
											L"Al eliminar una demografía de la lista de tipo de muestra se eliminará la entrada de la lista de demografías mostrada al usar el disco y cualquier valor introducido se perderá permanentemente.\n",																		// Spanish
											L"Rimuovendo una demografia dall'elenco dei tipi si rimuoverà quella voce dall'elenco di demografie proposto quando un disco è in funzione e tutti i valori immessi saranno persi in modo permanente.\n",																	// Italian
											L"Ao eliminar uma demografia da lista de tipo de amostra, será eliminado o item da lista de demografias exibida quando se utiliza um disco, e qualquer valor introduzido será permanentemente perdido.\n",																	// Portuguese
											L"A remoção de um dado demográfico da lista de tipos de amostra removerá o item da lista de dados demográficos mostrada durante a execução de um disco e quaisquer valores inseridos serão permanentemente perdidos.\n"														// Portuguese (Brazil)
										} 
	},
	{ ISTR_SECURITY_LOCKOUT			,	{	L"Analyze: When checked, only a authorized operator can open the drawer at the start or the end of an analysis.\n\n"
											L"Recall Results: When checked, only a authorized operator can recall results.\n\n"
											L"Settings: When checked, only a authorized operator can change settings.\n",																// English
											L"Analyse : lors de la vérification, seul un opérateur autorisé peut ouvrir le tiroir au début ou à la fin d'une analyse.\n\n"
											L"Rappeler résultats : lors de la vérification, seul un opérateur autorisé peut rappeler les résultats.\n\n"
											L"Paramètres : lors de la vérification, seul un opérateur autorisé peut modifier les paramètres.\n",											// French
											L"Analysieren: Wenn aktiviert, kann nur ein autorisierter Bediener das Fach bei Start oder Ende einer Analyse öffnen. \n\n"
											L"Ergebnisse erneut abrufen: Wenn aktiviert, kann nur ein autorisierter Bediener Ergebnisse erneut abrufen.\n\n"
											L"Einstellungen: Wenn aktiviert, kann nur ein autorisierter Bediener Einstellungen ändern.\n",												// German
											L"Analizar: cuando se activa, solo un operario autorizado puede abrir el cajón al comienzo o al final de un análisis.\n\n"				
											L"Buscar resultados: cuando se activa, solo un operario autorizado puede buscar los resultados.\n\n"
											L"Configuración: cuando se activa, solo un operario autorizado puede cambiar la configuración.\n",											// Spanish
											L"Analizzare: quando spuntato, esclusivamente un operatore autorizzato può aprire il cassetto all'inizio o alla fine di un'analisi.\n\n"
											L"Recupera i risultati: quando spuntato, esclusivamente un operatore autorizzato può recuperare i risultati.\n\n"
											L"Impostazioni; quando spuntato, esclusivamente un operatore autorizzato può modificare le impostazioni.\n",									// Italian
											L"Analisar: Quando selecionado, apenas um operador autorizado pode abrir a gaveta no início ou no final de uma análise.\n\n"
											L"Recuperar resultados: Quando selecionado, apenas um operador autorizado pode recuperar os resultados.\n\n"
											L"Configurações: Quando selecionado, apenas um operador autorizado pode alterar as configurações.\n",										// Portuguese
											L"Analisar: Quando marcado, apenas um operador autorizado pode abrir a gaveta no início ou no final de uma análise.\n\n"
											L"Recuperar resultados: Quando marcado, somente um operador autorizado pode recuperar os resultados.\n\n"
											L"Configurações: Quando marcado, apenas um operador autorizado pode alterar as configurações.\n"												// Portuguese (Brazil)
										}	
	},
	{ ISTR_SECURITY_SETTINGS		,	{	L"Administrators: Can access the settings in the second settings screen. Security features take effect when the first administrator is added.  Security features are disabled when all administrators are deleted.\n\n"
											L"Operators: Authorized operators for the analyzer. Adding an operator to the list will turn on the Operator ID prompt when analyzing a sample.\n\n"
											L"Controls: The date that a new set of control samples must be run by. Analyzing a sample after this date will result in a warning when the sample is analyzed.\n",																																	// English
											L"Administrateurs : ils peuvent accéder aux paramètres sur le second écran de paramètres. Les caractéristiques de sécurité entrent en vigueur lors de l'ajout du premier administrateur. Les caractéristiques de sécurité sont désactivées lorsque tous les administrateurs sont supprimés.\n\n"
											L"Opérateurs : opérateurs autorisés pour l'analyseur. L'ajout d'un opérateur à la liste a pour effet d'activer un message d'invitation à saisir l'ID opérateur lors de l'analyse d'un échantillon.\n\n"
											L"Contrôles : date butoir à laquelle un nouvel ensemble d'échantillons de contrôle doit avoir été exécuté. Le fait d'analyser un échantillon après cette date entraîne un message d'avertissement lors de l'analyse de l'échantillon.\n",															// French
											L"Administrator: Haben Zugang zu den Einstellungen in dem Zweiteinstellungsbildschirm. Sicherheitsmerkmale werden bei Hinzufügung des ersten Administrators wirksam. Sicherheitsmerkmale werden bei Löschung aller Administratoren deaktiviert.\n\n"
											L"Bediener: Befugte Bediener des Analysegeräts. Die Hinzufügung eines Bedieners zur Liste aktiviert bei Analyse einer Probe die Aufforderung zur Eingabe der Bediener-ID.\n\n"
											L"Steuerungen: Das Datum, zu dem ein neuer Satz von Kontrollproben vorgenommen werden muss. Die Analyse einer Probe nach diesem Datum führt zu einem Warnhinweis bei Analyse der Probe.\n",																											// German
											L"Administradores: pueden acceder a la configuración en la segunda pantalla de configuraciones. Las medidas de seguridad empiezan a adoptarse después de añadir el primer administrador. Las medidas de seguridad están inactivas al eliminar a todos los administradores.\n\n"
											L"Operarios: operarios autorizados para uso del analizador. Al añadir un operario a la lista se activará un mensaje de identificación de operario al analizar la muestra.\n\n"
											L"Controles: la fecha en la que se debe usar un nuevo grupo de muestras de control. El análisis de una muestra después de esta fecha motivará un mensaje de aviso durante el análisis.\n",																											// Spanish
											L"Amministratori: possono accedere alle impostazioni nel secondo schermo di impostazioni. Le impostazioni di sicurezza entrano in vigore quando si aggiunge il primo amministratore. Le impostazioni di sicurezza sono disattivate quando tutti gli amministratori sono cancellati.\n\n"
											L"Operatori: operatori autorizzati all'uso dell'analizzatore. Se si aggiunge un operatore all'elenco, durante l'analisi di un campione viene attivato il prompt \"ID operatore\".\n\n"
											L"Controlli: la data in cui deve essere effettuata una nuova serie di controlli campione. Se l'analisi di un campione viene eseguita dopo tale data, verrà visualizzato un messaggio di attenzione.\n",																								// Italian
											L"Administradores: Podem aceder às configurações no segundo ecrã de configurações. Os recursos de segurança tornam-se efetivos quando o primeiro administrador é adicionado.  Os recursos de segurança ficam inativos quando todos os administradores são eliminados.\n\n"
											L"Operadores: Operadores autorizados para utilização do analisador. Adicionar um operador à lista irá ativar um pedido de ID do operador ao analisar a amostra.\n\n"
											L"Controlos: A data na qual se deve utilizar um grupo novo de amostras controlo. A análise de uma amostra após esta data resultará numa mensagem de aviso durante a análise.\n",																														// Portuguese
											L"Administradores: Podem acessar as configurações na segunda tela de configurações. Os recursos de segurança entram em vigor quando o primeiro administrador é adicionado.  Os recursos de segurança são desativados quando todos os administradores são excluídos.\n\n"
											L"Operadores: Operadores autorizados para o analisador. Adicionar um operador à lista ativará o prompt do ID do operador ao analisar uma amostra.\n\n"
											L"Controles: A data até a qual um novo conjunto de amostras de controle deve ser executado. A análise de uma amostra após essa data resultará em um aviso quando a amostra for analisada.\n"																											// Portuguese (Brazil)
										} 
	},
	{ ISTR_SECURITY_SETTINGS_LOCKOUT,	{	L"Administrators: Can access the settings in the second settings screen. Security features take effect when the first administrator is added.  Security features are disabled when all administrators are deleted.\n\n"
											L"Operators: Authorized operators for the analyzer. Adding an operator to the list will turn on the Operator ID prompt when analyzing a sample.\n\n"
											L"Controls: The date that a new set of control samples must be run by. Analyzing a sample after this date will result in a warning when the sample is analyzed.\n\n"
											L"Lockout: Limits use of the analyzer functions to authorized operators.  Create authorized operators prior to enabling lockout.\n",																																									// English
											L"Administrateurs : ils peuvent accéder aux paramètres sur le second écran de paramètres. Les caractéristiques de sécurité entrent en vigueur lors de l'ajout du premier administrateur. Les caractéristiques de sécurité sont désactivées lorsque tous les administrateurs sont supprimés.\n\n"
											L"Opérateurs : opérateurs autorisés pour l'analyseur. L'ajout d'un opérateur à la liste a pour effet d'activer un message d'invitation à saisir l'ID opérateur lors de l'analyse d'un échantillon.\n\n"
											L"Contrôles : date butoir à laquelle un nouvel ensemble d'échantillons de contrôle doit avoir été exécuté. Le fait d'analyser un échantillon après cette date entraîne un message d'avertissement lors de l'analyse de l'échantillon.\n\n"
											L"Verrouillage : limite l'utilisation des fonctions de l'analyseur aux opérateurs autorisés. Les opérateurs autorisés doivent être créés avant d'autoriser le verrouillage.\n",																														// French
											L"Administrator: Haben Zugang zu den Einstellungen in dem Zweiteinstellungsbildschirm. Sicherheitsmerkmale werden bei Hinzufügung des ersten Administrators wirksam. Sicherheitsmerkmale werden bei Löschung aller Administratoren deaktiviert.\n\n"
											L"Bediener: Befugte Bediener des Analysegeräts. Die Hinzufügung eines Bedieners zur Liste aktiviert bei Analyse einer Probe die Aufforderung zur Eingabe der Bediener-ID.\n\n"
											L"Steuerungen: Das Datum, zu dem ein neuer Satz von Kontrollproben vorgenommen werden muss. Die Analyse einer Probe nach diesem Datum führt zu einem Warnhinweis bei Analyse der Probe.\n\n"
											L"Verriegelung: Beschränkt die Verwendung von Analysegerätfunktionen auf autorisierte Bediener. Erstellen Sie autorisierte Bediener bevor Sie die Verriegelung aktivieren.\n",																														// German
											L"Administradores: pueden acceder a la configuración en la segunda pantalla de configuración. Las características de seguridad se activan cuando se agrega el primer administrador y se desactivan si se eliminan todos los administradores.\n\n"
											L"Operarios: operarios autorizados a utilizar el analizador. Al añadir un operario a la lista, se preguntará el ID del operario al analizar una muestra.\n\n"
											L"Controles: fecha en la que se debe ejecutar un nuevo conjunto de muestras de control. Si se analiza una muestra después de esta fecha, aparecerá una advertencia cuando se analice la muestra.\n\n"
											L"Bloqueo: limita el uso de las funciones del analizador a los operarios autorizados. Debe crear operarios autorizados antes de activar el bloqueo.\n",																																				// Spanish
											L"Amministratori: possono accedere alle impostazioni nel secondo schermo di impostazioni. Le impostazioni di sicurezza entrano in vigore quando si aggiunge il primo amministratore. Le impostazioni di sicurezza sono disattivate quando tutti gli amministratori sono cancellati.\n\n"
											L"Operatori: operatori autorizzati all'uso dell'analizzatore. Se si aggiunge un operatore all'elenco, durante l'analisi di un campione viene attivato il prompt \"ID operatore\".\n\n"
											L"Controlli: la data in cui deve essere effettuata una nuova serie di controlli campione. Se l'analisi di un campione viene eseguita dopo tale data, verrà visualizzato un messaggio di attenzione.\n\n",																							
											L"Serrata: limita l'uso delle funzioni dell'analizzatore agli operatori autorizzati.  Creare operatori autorizzati prima di abilitare la serrata.\n"																																					// Italian
											L"Administradores: Podem aceder às configurações no segundo ecrã de configurações. Os recursos de segurança tornam-se efetivos quando o primeiro administrador é adicionado.  Os recursos de segurança ficam inativos quando todos os administradores são eliminados.\n\n"
											L"Operadores: Operadores autorizados para utilização do analisador. Adicionar um operador à lista irá ativar um pedido de ID do operador ao analisar a amostra.\n\n"
											L"Controlos: A data na qual se deve utilizar um grupo novo de amostras controlo. A análise de uma amostra após esta data resultará numa mensagem de aviso durante a análise.\n\n"
											L"Bloqueio: Limita a utilização das funções do analisador aos operadores autorizados.  Deve criar operadores autorizados antes de ativar o bloqueio.\n",																																				// Portuguese
											L"Administradores: Podem acessar as configurações na segunda tela de configurações. Os recursos de segurança entram em vigor quando o primeiro administrador é adicionado.  Os recursos de segurança são desativados quando todos os administradores são excluídos.\n\n"
											L"Operadores: Operadores autorizados para o analisador. Adicionar um operador à lista ativará o prompt do ID do operador ao analisar uma amostra.\n\n"
											L"Controles: A data até a qual um novo conjunto de amostras de controle deve ser executado. A análise de uma amostra após essa data resultará em um aviso quando a amostra for analisada.\n\n"
											L"Bloquear: Limita o uso das funções do analisador a operadores autorizados.  Crie operadores autorizados antes de ativar o bloqueio.\n"																																								// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_ELECTROLYTE_UNITS	,	{	L"Selecting a unit on this screen will change the units for CL, K, NA and tCO2 simultaneously to the same units.\n",										// English
											L"L'unité sélectionnée dans cet écran sera appliquée simultanément à CL, K, NA et tCO2.\n",																// French
											L"Mit der Auswahl einer Einheit in diesem Menü werden automatisch die Einheiten für CL, K, NA und tCO2 auf diese Einheit umgestellt.\n",					// German
											L"Al seleccionar una unidad en esta pantalla, se cambiará CL, K, NA y tCO2 simultáneamente a la misma unidad.\n",										// Spanish
											L"La selezione di un'unità in questa schermata modifica simultaneamente le unità per CL, K, NA e tCO2 nelle stesse unità.\n",							// Italian
											L"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades CL, K, NA e tCO2 para as mesmas unidades.\n",						// Portuguese
											L"A seleção de uma unidade nessa tela mudará as unidades para CL, K, NA e tCO2 simultaneamente para as mesmas unidades.\n"								// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_ENZYME_UNITS			,	{	L"Selecting a unit on this screen will change the units for ALT, ALP, AMY, AST, CK, GGT and LD simultaneously to the same units.\n",						// English
											L"L'unité sélectionnée dans cet écran sera appliquée simultanément à ALT, ALP, AMY, AST, CK, GGT et LD.\n",												// French
											L"Mit der Auswahl einer Einheit in diesem Menü werden automatisch die Einheiten für ALT, ALP, AMY, AST, CK, GGT und LD auf diese Einheit umgestellt.\n",	// German
											L"Al seleccionar una unidad en esta pantalla, se cambiará ALT, ALP, AMY, AST, CK, GGT y LD simultáneamente a la misma unidad.\n",						// Spanish
											L"La selezione di un'unità in questa schermata modifica simultaneamente le unità per ALT, ALP, AMY, AST, CK, GGT ed LD nelle stesse unità.\n",			// Italian
											L"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades ALT, ALP, AMY, AST, CK, GGT e LD para as mesmas unidades.\n",		// Portuguese
											L"A seleção de uma unidade nessa tela mudará as unidades para ALT, ALP, AMY, AST, CK, GGT e LD simultaneamente para as mesmas unidades.\n"				// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_LIPID_UNITS			,	{	L"Selecting a unit on this screen will change the units for CHOL, HDL, LDL, TRIG and VLDL simultaneously to the same units.\n",							// English
											L"L'unité sélectionnée dans cet écran sera appliquée simultanément à CHOL, LHD, LBD, TRIG et LTBD.\n",													// French
											L"Mit der Auswahl einer Einheit in diesem Menü werden automatisch die Einheiten für CHOL, HDL, LDL, TRIG und VLDL auf diese Einheit umgestellt.\n",		// German
											L"Al seleccionar una unidad en esta pantalla, se cambiará COL, HDL, LDL, TRIG y PMBD simultáneamente a la misma unidad.\n",								// Spanish
											L"La selezione di un'unità in questa schermata modifica simultaneamente le unità per COL, HDL, LDL, TRIG e VLDL nelle stesse unità.\n",					// Italian
											L"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades CHOL, HDL, LDL, TRIG e VLDL para as mesmas unidades.\n",				// Portuguese
											L"A seleção de uma unidade nessa tela mudará as unidades para CHOL, HDL, LDL, TRIG e VLDL simultaneamente para as mesmas unidades.\n"					// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_MINERAL_UNITS		,	{	L"Selecting a unit on this screen will change the units for CA and MG simultaneously to the same units.\n",												// English
											L"L'unité sélectionnée dans cet écran sera appliquée simultanément à CA et MG.\n",																		// French
											L"Mit der Auswahl einer Einheit in diesem Menü werden automatisch die Einheiten für CA und MG auf diese Einheit umgestellt.\n",							// German
											L"Al seleccionar una unidad en esta pantalla, se cambiará CA y MG simultáneamente a la misma unidad.\n",													// Spanish
											L"La selezione di un'unità in questa schermata modifica simultaneamente le unità per CA ed MG nelle stesse unità.\n",									// Italian
											L"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades CA e MG para as mesmas unidades.\n",									// Portuguese
											L"A seleção de uma unidade nessa tela mudará as unidades para CA e MG simultaneamente para as mesmas unidades.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_PROTEIN_UNITS		,	{	L"Selecting a unit on this screen will change the units for ALB and TP simultaneously to the same units.\n",												// English
											L"L'unité sélectionnée dans cet écran sera appliquée simultanément à ALB et TP.\n",																		// French
											L"Mit der Auswahl einer Einheit in diesem Menü werden automatisch die Einheiten für ALB und TP auf diese Einheit umgestellt.\n",							// German
											L"Al seleccionar una unidad en esta pantalla, se cambiará ALB y TP simultáneamente a la misma unidad.\n",												// Spanish
											L"La selezione di un'unità in questa schermata modifica simultaneamente le unità per ALB e TP nelle stesse unità.\n",									// Italian
											L"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades ALB e TP para as mesmas unidades.\n",								// Portuguese
											L"A seleção de uma unidade nessa mudará as unidades para ALB e TP simultaneamente para as mesmas unidades.\n"											// Portuguese (Brazil)
										} 
	},
	{ ISTR_SOUND_SETTINGS			,	{	L"Screen Click: Sets the volume level of the sound made when touching a button on the display.\n\n"
											L"Alerts: Sets the volume level when a warning or error screen is displayed.\n\n"
											L"Status: Sets the volume level for status sounds.\n",																									// English
											L"Clic écran : cette fonction définit le volume sonore lors de l'activation d'un bouton à l'écran.\n\n"
											L"Avertisseurs sonores : cette fonction définit le volume lors de l'affichage d'un avertissement ou d'un écran d'erreur.\n\n"
											L"État : définit le volume sonore pour les indicateurs d'état.\n",																						// French
											L"Bildschirmklick: Einstellung der Tonlautstärke bei Berührung einer Taste auf der Anzeige.\n\n"
											L"Alarme: Einstellung der Tonlautstärke bei Erscheinen der Warn- oder Fehleranzeige.\n\n"
											L"Status: Einstellung der Lautstärke für Statustöne.\n",																									// German
											L"Clic de pantalla: establece el volumen del sonido que se hace cuando se toca un botón en la pantalla.\n\n"
											L"Alertas: establece el volumen del sonido que se hace cuando aparece una ventana de aviso o error.\n\n"
											L"Estado: establece el volumen del sonido de estado.\n",																									// Spanish
											L"Clic schermo: imposta il livello del volume del suono che si sente quando si tocca un pulsante sullo schermo.\n\n"
											L"Allarmi: imposta il livello del volume dei messaggi di attenzione o errore visualizzati a schermo.\n\n"
											L"Stato: imposta il livello del volume per i suoni di stato.\n",																							// Italian
											L"Clique no ecrã: Define o nível de volume do som que se ouve quando se toca uma tecla no visor.\n\n"
											L"Alertas: Define o nível de volume do som que se ouve quando é visualizado um ecrã de aviso ou erro.\n\n"
											L"Estado: Define o nível de volume para os sons de estado.\n",																							// Portuguese
											L"Clique na tela: Configura o nível de volume do som produzido ao tocar em um botão no visor.\n\n"
											L"Alertas: Configura o nível do volume quando uma tela de aviso ou erro é exibida.\n\n"
											L"Status: Configura o nível de volume para os sons de status.\n"																							// Portuguese (Brazil)
										} 
	},
	{ ISTR_TCPIP_SETTINGS			,	{	L"Change Settings: Change the TCP/IP Settings where Analyzer IP is the Static IP Address for the analyzer, the Subnet Mask is the IP Address mask for the analyzer, the Gateway IP is the IP Address of the router for the analyzer to reach other networks, the Server IP is the IP address of the server that will host the ASTM connection, and the Server Socket is the Socket (Port) on the Server that will be used for the ASTM TCP/IP connection.\n\n"
											L"Check Settings: The analyzer will attempt to make an ASTM connection using the settings under the Change Settings button.  The analyzer will display the result of the connection attempt.  Ensure the ASTM server is running and configured to accept a connection before checking the settings.\n",																																																																												// English																																																																																																																																																																																																																												// English                 					
											L"Modifier les paramètres : modifier les paramètres TCP/IP si le champ Adresse IP de l'analyseur est renseigné par l'adresse IP statique de l'appareil, le champ Masque de sous-réseau contient le masque d'adresse IP de l'analyseur, le champ Adresse IP de la passerelle désigne l'adresse IP du routeur qui permet à l'analyseur de communiquer avec d'autres réseaux. Le champ Adresse IP du serveur est renseigné par l'adresse IP du serveur qui hébergera la connexion ASTM et enfin le champ Socket du serveur désigne le Socket (Port) du serveur qui sera utilisé pour la connexion TCP/IP ASTM.\n\n"
											L"Vérifier les paramètres : l'analyseur tentera d'établir une connexion ASTM sur la base des paramètres définis sous le bouton Modifier les paramètres.  L'analyseur affichera le résultat de la tentative de connexion.  Vérifier que le serveur ASTM est en marche et qu'il est configuré pour accepter une connexion avant de vérifier les paramètres.\n",																																																														// French
											L"Einstellungen ändern: Die TCP/IP-Einstellungen so ändern, dass die IP-Adresse des Analysegeräts die statische IP-Adresse für das Analysegerät ist, die Subnetzmaske die IP-Adressmaske für das Analysegerät ist, die Gateway-IP-Adresse die IP-Adresse des Routers für das Analysegerät ist, über den dieses andere Netzwerke erreichen kann, die Server-IP-Adresse die IP-Adresse des Servers ist, der als Host für den ASTM-Anschluss dient, und der Server-Socket der Socket (Anschluss ) des Servers ist, der für die ASTM-TCP/IP-Verbindung genutzt werden soll.\n\n"
											L"Einstellungen überprüfen: Das Analysegerät wird versuchen, eine ASTM-Verbindung herzustellen, und bedient sich dabei der über die Taste \"Einstellungen ändern\" eingegebenen Einstellungen. Das Analysegerät zeigt das Resultat des Verbindungsversuchs an. Vor dem Überprüfen der Einstellungen sicherstellen, dass der ASTM-Server in Betrieb und für die Annahme einer Verbindung konfiguriert ist.\n",																																																		// German
											L"Cambiar configuración: Cambie la configuración de TCP/IP, donde la dirección IP del analizador es la dirección IP estática del analizador, la máscara de la subred es la máscara de la dirección IP del analizador, la dirección IP de la puerta de enlace es la dirección IP del enrutador para conectividad entre el analizador y otras redes, la dirección IP del servidor es la dirección IP del servidor que alojará la conexión ASTM y el socket del servidor es el socket (puerto) del servidor que se utilizará para la conexión TCP/IP ASTM.\n\n"
											L"Comprobar configuración: El analizador intentará realizar una conexión ASTM usando los parámetros que aparecen tras pulsar el botón \"Cambiar configuración\".  El analizador mostrará el resultado del intento de conexión.  Asegúrese de que el servidor ASTM está funcionando y configurado para aceptar una conexión antes de comprobar los parámetros de configuración.\n",																																																									// Spanish
											L"Modifica impostazioni: Modificare le impostazioni TCP/IP dove IP analizzatore è l'indirizzo IP statico per l'analizzatore, Maschera di sottorete è la maschera dell'indirizzo IP per l'analizzatore, IP gateway è l'indirizzo IP del router per consentire all'analizzatore di raggiungere altre reti, IP server è l'indirizzo IP del server per l'hosting della connessione ASTM, e Socket server è il Socket (porta) sul server che verrà utilizzato per la connessione TCP/IP ASTM.\n\n"
											L"Verifica impostazioni: L'analizzatore tenterà di effettuare una connessione ASTM utilizzando le impostazioni sotto il pulsante Modifica impostazioni.  L'analizzatore visualizzerà il risultato del tentativo di connessione.  Prima di eseguire la verifica delle impostazioni, accertarsi che il server ASTM sia attivo e configurato per accettare una connessione.\n",																																																											// Italian
											L"Alterar configurações: Altera as configurações de TCP/IP, nas quais o IP do analisador é o endereço IP estático do analisador, a máscara de subrede é a máscara do endereço IP do analisador, o IP do Portal é o endereço IP do rotor para que o analisador se ligue a outras redes, o IP do servidor é o endereço IP do servidor que alojará a ligação ASTM e a tomada do servidor é a tomada (Porta) no Servidor que será utilizada para a ligação TCP/IP ASTM.\n\n"
											L"Verificar configurações: O analisador tentará realizar uma ligação ASTM utilizando as definições que surgem sob o botão Alterar configurações.  O analisador apresentará o resultado da tentativa de ligação.  Certifique-se que o servidor ASTM está a funcionar e que está configurado para aceitar uma ligação antes de verificar as configurações.\n",																																																															// Portuguese
											L"Alterar configurações: Altere as configurações de TCP/IP em que o IP do analisador é o endereço IP estático do analisador, a máscara de sub-rede é a máscara do endereço IP do analisador, o IP do gateway é o endereço IP do roteador para que o analisador alcance outras redes, IP do servidor é o endereço IP do servidor que hospedará a conexão ASTM e Soquete do servidor é o soquete (porta) do servidor que será usado para a conexão TCP/IP de ASTM.\n\n"
											L"Verificar configurações: O analisador tentará fazer uma conexão ASTM usando as configurações no botão Alterar configurações.  O analisador exibirá o resultado da tentativa de conexão.  Certifique-se de que o servidor ASTM esteja em execução e configurado para aceitar uma conexão antes de verificar as configurações.\n"																																																																					// Portuguese (Brazil)
										}
	},		
	{ ISTR_TOUCH_CALIBRATE			,	{	L"Pressing Yes will align the touch screen to the LCD display. It is necessary to use a soft-pointed stylus to accurately align the touch screen. Using a hard-pointed object will damage the touch screen. Pressing the screen other than at the center of the target will misalign the touch screen. If you touch the screen in the wrong location, complete the alignment by touching the remaining targets and do not touch the final alignment button. If you have questions contact technical support before performing the alignment.\n",																											// English
											L"Appuyer sur Oui pour aligner l'écran tactile et l'écran à cristaux liquides. Pour aligner correctement l'écran tactile, utiliser un stylet à pointe souple. Toute utilisation d'un objet pointu risquerait d'endommager l'écran tactile. Le fait d'appuyer sur l'écran ailleurs qu'au centre de la cible entraîne un défaut d'alignement de l'écran tactile. Si vous touchez l'écran au mauvais endroit, terminer l'alignement en touchant les cibles restantes et ne pas toucher le bouton d'alignement final. Pour toute question, contacter l'assistance technique avant de procéder à l'alignement.\n",											// French
											L"Drücken auf Ja richtet den Touch-Screen auf das LCD-Display aus. Zur genauen Ausrichtung des Touch-Screens muss ein Eingabestift mit weicher Spitze verwendet werden. Die Verwendung eines Objekts mit harter Spitze beschädigt den Touch-Screen. Drücken der Anzeige außerhalb der Zielmitte führt zu einer falschen Ausrichtung des Touch-Screens. Wenn Sie den Screen an der falschen Stelle berühren, schließen Sie die Ausrichtung durch Berührung der verbleibenden Ziele ab. Die endgültige Ausrichtungstaste nicht berühren. Bei Fragen vor der Ausrichtung den technischen Support kontaktieren.\n",											// German
											L"Al pulsar Sí se alineará la pantalla táctil con la pantalla de cristal líquido. Si fuese necesario, utilice un lápiz con punta blanda para alinear con precisión la pantalla de contacto. Si se utiliza un objeto con punta dura, la pantalla táctil resultará dañada. Si se presiona la pantalla en otro lugar distinto al centro del objetivo, la pantalla táctil resultará desalineada. Si toca la pantalla en el lugar equivocado, complete la alineación tocando los objetivos restantes y no toque el botón de alineación final. Si tiene alguna duda o pregunta, llame al servicio de asistencia técnica antes de realizar la alineación.\n",	// Spanish
											L"Premendo Sì il touchscreen verrà allineato al display LCD. Per allineare accuratamente il touchscreen è necessario utilizzare una penna stilo a punta morbida. Oggetti a punta rigida possono danneggiare il touchscreen. Se si preme lo schermo non al centro dell'obiettivo, il touchscreen verrà allineato male. Se si tocca lo schermo nel punto sbagliato, completare l'allineamento toccando gli altri obiettivi e non toccare il pulsante di allineamento finale. Per eventuali dubbi, prima di effettuare l'allineamento contattare l'assistenza tecnica.\n",																					// Italian
											L"Ao premir Sim irá alinhar o ecrã tátil com o visor LCD. É necessário utilizar um ponteiro de ponta suave para alinhar o ecrã tátil com precisão. Se utilizar um objeto de ponta dura irá provocar danos no ecrã tátil. Ao pressionar o ecrã noutro local que não seja o centro do alvo, irá desalinhar o ecrã tátil. Se tocar no ecrã no local errado, conclua o alinhamento tocando nos alvos restantes e não toque na tecla final de alinhamento. Se tiver alguma dúvida, contacte a serviço de assistência técnica antes de efetuar o alinhamento.\n",																								// Portuguese
											L"Pressionar Sim alinhará a tela sensível ao toque com o display LCD. É necessário usar uma caneta de ponta macia para alinhar precisamente a tela sensível ao toque. O uso de um objeto pontiagudo danificará a tela sensível ao toque. Pressionar a tela fora do centro do alvo desalinhará a tela sensível ao toque. Se você tocar na tela no local errado, conclua o alinhamento tocando nos alvos restantes e não toque no botão de alinhamento final. Se tiver dúvidas, entre em contato com o suporte técnico antes de realizar o alinhamento.\n"																									// Portuguese (Brazil)
										} 
	},
	{ ISTR_UNITS_GLOBAL				,	{	L"Non SI: Also known as Common Units. The units in which all results are calculated and the units commonly used in the United States.\n\n"
											L"SI: Units identified by Abaxis as the commonly used SI units.",																																// English
											L"Non SI : également désignées \"Unités ordinaires\". Il s'agit des unités dans lesquelles tous les résultats sont calculés, ainsi que des unités ordinairement utilisées aux États-Unis.\n\n"
											L"SI : unités identifiées par Abaxis comme étant les unités SI ordinairement utilisées.\n",																										// French
											L"Kein SI: Auch als Häufige Einheiten bekannt. Die Einheiten zur Ergebnisermittlung sind die in den USA gebräuchlichen Einheiten.\n\n"
											L"SI: Als Abaxis identifizierte Einheiten als häufig verwendete SI-Einheiten.\n",																												// German
											L"No SI: también se le conoce como Unidades comunes. La unidades en las que se calculan todos los resultados y las unidades comúnmente usadas en los Estados Unidos.\n\n"
											L"SI: unidades identificadas por Abaxis como las unidades del sistema internacional usadas normalmente.\n",																						// Spanish
											L"Non SI: conosciuto anche come Unità comuni. Le unità in cui tutti i risultati sono calcolati e le unità comunemente usate negli Stati Uniti.\n\n"
											L"SI: unità identificate come Abaxis come le unità SI comunemente usate.\n",																														// Italian
											L"Não SI: Também conhecidas como Unidades comuns. As unidades nas quais todos os resultados são calculados e as unidades frequentemente utilizadas nos Estados Unidos.\n\n"
											L"SI: Unidades identificadas pela Abaxis como as unidades do sistema internacional utilizadas frequentemente.\n",																				// Portuguese
											L"Não SI: Também conhecido como Unidades comuns. As unidades nas quais todos os resultados são calculados e as unidades comumente usadas nos Estados Unidos.\n\n"
											L"SI: Unidades identificadas pela Abaxis como as unidades SI comumente usadas.\n"																												// Portuguese (Brazil)
										} 
	},
	

// Error screens							ENGLISH,							FRENCH,										GERMAN,									SPANISH,												ITALIAN, 								PORTUGUESE,											PORTUGUESE (BRAZIL)
	{ ESTR_ADMIN_ID_LIST_FULL		,	{	L"Administrator ID List Full",		L"Liste ID administrateur pleine",			L"Vollständige Administrator-ID-Liste",	L"Lista de id. de administradores completa",			L"Elenco ID Amministratore completo",	L"Lista de ID de administradores completa",			L"Lista de IDs de administrador completa"	}	},
	{ ESTR_AMBIENT_TEMP				,	{	L"Ambient Temp. Error",				L"Temp. ambiante Erreur",					L"Umgebungstemp. Fehler",				L"Temp. ambiente Error",								L"Temp. ambiente Errore",				L"Temperatura ambiente Erro",						L"Temp. ambiente Erro"	}	},
	{ ESTR_ANALYSIS_CANCELED		,	{	L"Analysis canceled by operator",	L"Anal. annulée par l'opérateur",			L"Anal. vom Bedien. abgebrochen",		L"Anál. cancelado por oper.",							L"Anal. annullata dall'operatore",		L"Análise cancelada pelo operador",					L"Análise cancelada pelo operador"	}	},
	{ ESTR_ANALYSIS_TIMEOUT			,	{	L"Analysis Time-out",				L"Temps d'analyse dépassé",					L"Analysezeit abgelaufen",				L"Tiempo análisis agotado",								L"Time-out analisi",					L"Tempo de análise esgotado",						L"Tempo limite da análise"	}	},
	{ ESTR_ANALYZER_TIMEOUT			,	{	L"Analyzer Time-out",				L"Dél. exéc. analyseur exp.",				L"Analysegerät-Unterbr.",				L"Tiempo inactiv. analizad.",							L"Time-out analizzatore",				L"Tempo inat. analisador",							L"Tempo limite do analis."	}	},
	{ ESTR_ANALYZER_TOO_HOT			,	{	L"Analyzer Too Hot",				L"Analyseur trop chaud",					L"Analysegerät zu heiß",				L"Analizador muy caliente",								L"Analizzatore troppo caldo",			L"Analisador muito quente",							L"Analisador muito quente"	}	},
	{ ESTR_BARCODE_FORMAT			,	{	L"Barcode Format Error",			L"Erreur format code barres",				L"Barcode Formatfehler",				L"Error formato cód. barras",							L"Err. formato cod. a barre",			L"Erro formato cód. barras",						L"Erro formato cód. barras"	}	},
	{ ESTR_BARCODE_SENSOR			,	{	L"Barcode Sensor Error",			L"Erreur détect. c. barres",				L"Barcode Sensorfehler",				L"Error sensor cód. barras",							L"Err. sens. codice a barre",			L"Erro sensor cód. barras",							L"Erro sensor cód. barras"	}	},
	{ ESTR_BARCODE_TIMEOUT			,	{	L"Barcode Time-out Error",			L"Dél. exéc. c. barres exp.",				L"Barcode Unterbr.-Fehler",				L"Err tiem inact cód barr",								L"Err. time-out cod. barre",			L"Erro tempo inat. cód bar",						L"Erro temp lim. cód. barra"	}	},
	{ ESTR_BEADS_MISSING			,	{	L"Beads Missing Error",				L"Erreur billes manquantes",				L"Beads fehlen, Fehler",				L"Err. pérdida cuentas",								L"Errore granuli mancanti",				L"Erro de perda de esferas",						L"Erro de esferas ausentes"	}	},
	{ ESTR_CAM_DOWN_JAM				,	{	L"CAM Down Jam",					L"Blocage CAM haut",						L"CAM oben Stau",						L"LEVA bloqueada abajo",								L"Blocco CAMMA giù",					L"CAM bloqueada abaixo",							L"Emperramento CAM p/ baixo"	}	},
	{ ESTR_CAM_UP_JAM				,	{	L"CAM Up Jam",						L"Blocage CAM bas",							L"CAM unten Stau",						L"LEVA bloqueada arriba",								L"Blocco CAMMA su",						L"CAM bloqueada acima",								L"Emperramento CAM p/ cima"	}	},
	{ ESTR_CD_READ					,	{	L"Error Reading CD ROM",			L"Erreur lecture CD ROM",					L"Fehler beim Lesen der CD-ROM",		L"Error al leer el CD-ROM",								L"Errore di lettura CD ROM",			L"Erro de leitura do CD-ROM",						L"Erro ao ler o CD ROM"	}	},
	{ ESTR_CORRUPTED_CALIBRATION	,	{	L"Corrupted Calibration",			L"Calibrage corrompu",						L"Fehlerhafte Kalibrierung",			L"Calibración errónea",									L"Calibrazione corrotta",				L"Calibração corrompida",							L"Calibração corrompida"	}	},
	{ ESTR_CUVETTE_SENSOR			,	{	L"Cuvette Sensor Error",			L"Erreur détecteur cuvette",				L"Cuvette Sensorfehler",				L"Error sensor cubeta",									L"Errore sensore cuvette",				L"Erro do sensor de cuvetes",						L"Erro do sensor da cuveta"	}	},
	{ ESTR_CUVETTE_TIMEOUT			,	{	L"Cuvette Time-out Error",			L"Err. délai exéc. cuvette",				L"Cuvette Unterbr.-Fehler",				L"Err tiem inact cubeta",								L"Errore time-out cuvette",				L"Erro tempo inat. cuvete",							L"Erro tempo limite cuveta"	}	},
	{ ESTR_DILUENT					,	{	L"Diluent Error",					L"Erreur diluant",							L"Fehler des Verdünners",				L"Error del diluyente",									L"Errore diluente",						L"Erro de solvente",								L"Erro de diluente"	}	},
	{ ESTR_DILUENT_ABSORBANCE		,	{	L"Diluent Absorbance Error",		L"Erreur absorbance diluant",				L"Verdünner Absorb.-Fehler",			L"Err absorbancia diluyente",							L"Err. assorb. diluente",				L"Erro absorvância solvente",						L"Erro absorção do diluente"	}	},
	{ ESTR_DILUTION					,	{	L"Dilution Error",					L"Erreur dilution",							L"Verdünnungsfehler",					L"Error de dilución",									L"Errore diluizione",					L"Erro de diluição",								L"Erro de diluição"	}	},
	{ ESTR_DISTRIBUTION				,	{	L"Distribution Error",				L"Erreur distribution",						L"Verteilungsfehler",					L"Error de distribución",								L"Errore distribuzione",				L"Erro de distribuição",							L"Erro de distribuição"	}	},
	{ ESTR_DRAWER_CLOSED_JAM		,	{	L"Drawer Closed Jam",				L"Blocage fermeture tiroir",				L"Fach geschlossen Stau",				L"Bloqueo de la bandeja cerrada",						L"Blocco cassetto chiuso",				L"Bloqueio da gaveta fechada",						L"Emperramento de gaveta fechada"	}	},
	{ ESTR_DRAWER_OPEN_JAM			,	{	L"Drawer Open Jam",					L"Blocage tiroir ouvert",					L"Fach offen Stau",						L"Bloqueo de la bandeja abierta",						L"Blocco cassetto aperto",				L"Bloqueio da gaveta aberta",						L"Emperramento de gaveta aberta"	}	},
	{ ESTR_ENGINE_RESET				,	{	L"Asynchronous Reset",				L"Réinitialisation asynchr.",				L"Asynchroner Resetvorgang",			L"Restablecim. asíncrono",								L"Ripristino asincrono",				L"Reinício assíncrono",								L"Redefinição assíncrona"	}	},
	{ ESTR_ILLEGAL_IP_ADDRESS		,	{	L"Illegal IP Address",				L"Adresse IP non valide",					L"Falsche IP-Adresse",					L"Dirección IP no válida",	   							L"Indirizzo IP non valido",				L"Endereço IP inválido",							L"Endereço IP ilegal"	}	},
	{ ESTR_ILLEGAL_IP_MASK			,	{	L"Illegal IP Mask",					L"Masque IP non valide",					L"Falsche IP-Maske",					L"Máscara IP no válida",								L"Maschera IP non valida",				L"Máscara de IP inválida",							L"Máscara IP ilegal"	}	},
	{ ESTR_ILLEGAL_ROTOR_TYPE		,	{	L"Illegal Disc Type",				L"Type disque non autorisé",				L"Falscher Scheibentyp",				L"Tipo de disco ilegal",								L"Tipo disco illegale",					L"Tipo de disco inválido",							L"Tipo de disco ilegal"	}	},
	{ ESTR_ILLEGAL_SOCKET			,	{	L"Illegal Socket Value",			L"Valeur de socket non valide",				L"Falscher Socket-Wert",				L"Valor de socket no válido",							L"Valore socket non valido",			L"Valor da tomada inválido",						L"Valor de soquete ilegal"	}	},
	{ ESTR_INITIALIZATION			,	{	L"Initialization Error",			L"Erreur d'initialisation",					L"Initialisierungsfehler",				L"Error de inicio",										L"Errore inizializzazione",				L"Erro de inicialização",							L"Erro de inicialização"	}	},
	{ ESTR_INSUFF_SAMPLE			,	{	L"Insufficient Sample Error",		L"Erreur éch. insuffisant",					L"Unzureich. Probe, Fehler",			L"Err. muestra insuficien",								L"Err. campione insuff.",				L"Erro amostra insuficiente",						L"Erro amostra insuficiente"	}	},
	{ ESTR_INSUFF_STORAGE			,	{	L"Insufficient Storage Space",		L"Espace de stockage insuffisant",			L"Ungenügend Speicherplatz",			L"Espacio insuficiente",								L"Spazio archiviazione insufficiente",	L"Espaço de armazenamento insuficiente",			L"Espaço de armazenamento insuficiente"	}	},
	{ ESTR_INTERNAL_COMM			,	{	L"Internal Comm. Error",			L"Comm. interne Erreur",					L"Interne Komm. Fehler",				L"Com. interna Error",									L"Comm. interno Errore",				L"Com. interna Erro",								L"Com. interna Erro"	}	},
	{ ESTR_INTERNAL_HEATER			,	{	L"Internal Heater Error",			L"Err. disp. chauf. interne",				L"Interner Fehler Heizgerät",			L"Err. calentador interno",								L"Errore riscald. interno",				L"Erro de aquecedor interno",						L"Erro do aquecedor interno"	}	},
	{ ESTR_INTERNAL_LOGIC			,	{	L"Internal Logic Error",			L"Erreur logique interne",					L"Interner Logic-Fehler",				L"Error lógico interno",								L"Errore logica interna",				L"Erro lógico interno",								L"Erro da lógica interna"	}	},
	{ ESTR_INTERNAL_PS				,	{	L"Internal Power Error",			L"Erreur d'alim. interne",					L"Interner Energiefehler",				L"Error de energía interno",							L"Errore corrente interna",				L"Erro de energia interno",							L"Erro de energia interna"	}	},
	{ ESTR_INTERNAL_SOFTWARE		,	{	L"Internal Software Error",			L"Erreur logiciel interne",					L"Interner Softwarefehler",				L"Error de software interno",							L"Errore software interno",				L"Erro de software interno",						L"Erro de software interno"	}	},
	{ ESTR_IQC_BEAD					,	{	L"iQC Bead Error",					L"Erreur bille iQC",						L"iQC-Bead-Fehler",						L"Error de cuenta iQC",									L"Errore granuli iQC",					L"Erro de esfera iQC",								L"Erro de esfera iQC"	}	},
	{ ESTR_IQC_BEAD_MIX				,	{	L"iQC Bead Mix Error",				L"Erreur mix billes iQC",					L"iQC-Bead-Mischfehler",				L"Error mezcla cuenta iQC",								L"Err. miscela granuli iQC",			L"Erro de mist. esfera iQC",						L"Erro mistura esferas iQC"	}	},
	{ ESTR_IQC_PRECISION			,	{	L"iQC Precision Error",				L"Erreur précision iQC",					L"iQC-Präzisionsfehler",				L"Error de precisión iQC",								L"Errore precisione iQC",				L"Erro de precisão iQC",							L"Erro de precisão de iQC"	}	},
	{ ESTR_IQC_RATIO				,	{	L"iQC Ratio Error",					L"Erreur ratio iQC",						L"iQC-Verhältnisfehler",				L"Error de proporción iQC",								L"Errore rapporto iQC",					L"Erro de proporção iQC",							L"Erro de relação de iQC"	}	},
	{ ESTR_LAMP_LEVEL				,	{	L"Flash Lamp Level Error",			L"Erreur niveau lampe flash",				L"Blinkleuchte-Stufenfehler",			L"Error nivel lámpara flash",							L"Err. liv. lampada flash",				L"Erro nível flash lâmpada",						L"Erro nível lâmpada flash"	}	},
	{ ESTR_LAMP_NOISE				,	{	L"Flash Lamp Noise Error",			L"Erreur bruit lampe flash",				L"Blinkleuchte-Geräuschfeh.",			L"Error ruido lámpara flash",							L"Err. rumore lamp. flash",				L"Erro ruído flash lâmpada",						L"Erro ruído lâmpada flash"	}	},
	{ ESTR_MEMORY_READ				,	{	L"Memory Read Error",				L"Erreur de lecture mémoire",				L"Fehler bei Speicheraufruf",			L"Error lectura memoria",								L"Errore lettura memoria",				L"Erro leitura memória",							L"Erro leitura memória"	}	},
	{ ESTR_MEMORY_SAVE				,	{	L"Memory Save Error",				L"Erreur enregistr. mémoire",				L"Fehler bei Speicherung",				L"Err. guardar memoria",								L"Errore salv. Memoria",				L"Erro guardar na memória",							L"Erro salvamento memória"	}	},
	{ ESTR_NEW_ROTOR_TYPE			,	{	L"New Disc Type",					L"Nouveau type de disque",					L"Neuer Scheibentyp",					L"Nuevo tipo de disco",									L"Nuovo tipo disco",					L"Tipo novo de disco",								L"Novo tipo de disco"	}	},
	{ ESTR_NO_FILES_TO_ARCHIVE		,	{	L"No Files to Archive",				L"Aucun fichier à archiver",				L"Keine zu archiv. Dateien",			L"No archi para archivar",								L"Nessun file da archiviare",			L"Nenhum ficheiro arquivar",						L"Sem arquivo para arquivar"	}	},
	{ ESTR_NO_FILES_TO_TRANSMIT		,	{	L"No Files to Transmit",			L"Aucun fich. à transmettre",				L"Keine zu übertr. Dateien",			L"No archi para transmit",								L"Nessun file da trasm.",				L"Nenhum ficheiro transmit.",						L"Sem arquivo p/ transmitir"	}	},
	{ ESTR_NO_RESULTS_TO_TRANSMIT	,	{	L"No Results to Transmit",			L"Aucun résulat à trans.",					L"Keine Erg. zu übertragen",			L"No result para transmit",								L"Nessun risult. da trasm.",			L"Nenhum result. transmitir",						L"Sem result. p/ transmitir"	}	},
	{ ESTR_OP_ID_LIST_FULL			,	{	L"Operator ID List Full",			L"Liste ID opérateur pleine", 				L"Vollst. Bediener-ID-Liste",			L"List id. operari complet",							L"Elenco ID Oper. completo",			L"Lista ID operadores comp.",						L"Lista IDs oper. completa"	}	},
	{ ESTR_ORDER_WAS_CANCELED		,	{	L"Order Was Canceled",				L"Commande a été annulée",					L"Bestellung storniert",				L"El pedido se ha cancelado",							L"Ordine cancellato", 					L"O pedido foi cancelado",							L"Solicitação cancelada"	}	},
	{ ESTR_PATIENT_IDS_DONT_MATCH	,	{	L"Patient IDs Do Not Match",		L"ID patient ne corr. pas",					L"Patienten-IDs unstimmig",				L"No coinciden Id.  pacien",							L"ID paziente non corrisp.",			L"IDs doente não coincidem",						L"IDs paciente não corresp."	}	},
	{ ESTR_PHOTOMETER				,	{	L"Photometer Error",				L"Erreur photomètre",						L"Fotometer-Fehler",					L"Error del fotómetro",									L"Errore fotometro",					L"Erro do fotómetro",								L"Erro de fotômetro"	}	},
	{ ESTR_PLEASE_REMOVE_ROTOR		,	{	L"Please Remove Disc",				L"Retirer le disque",						L"Bitte Scheibe entfernen",				L"Retire el disco",										L"Rimuovere il disco",					L"Retirar disco",									L"Remova o disco"	}	},
	{ ESTR_PRINTER_NOT_FOUND		,	{	L"Printer Not Found",				L"Imprimante introuvable",					L"Kein Drucker gefunden",				L"No se encuentra impresora",							L"Stampante non trovata",				L"Impressora não localizada",						L"Impressora não encontrada"	}	},
	{ ESTR_PRODUCT_CODE				,	{	L"Illegal Product Code",			L"Code produit non autorisé",				L"Falscher Produktcode",				L"Código de producto ilegal",							L"Codice prodotto illegale",			L"Cód. produto inválido",							L"Código de produto ilegal"	}	},
	{ ESTR_RESULT_CORRUPTION		,	{	L"Saved Results Error",				L"Erreur dans les résultats enregistrés",	L"Fehler bei gespeicherten Ergebnissen", L"Error de resultados guardados",						L"Errore risultati salvati",			L"Erro de resultados guardados",					L"Erro de resultados salvos"	}	},
	{ ESTR_ROTOR_DATE_ILLEGAL		,	{	L"Disc Date Illegal",				L"Date disque non autorisée",				L"Falsches Scheibendatum",				L"Fecha de disco ilegal",								L"Data disco illegale",					L"Data do disco inválida",							L"Data do disco ilegal"	}	},
	{ ESTR_ROTOR_DATE_IMPROPER		,	{	L"Disc Date Improper",				L"Date disque incorrecte",					L"Unzul. Scheibendatum",				L"Fecha disco no adecuada",								L"Data disco impropria",				L"Data do disco inadequada",						L"Data do disco imprópria"	}	},
	{ ESTR_ROTOR_EXPIRED			,	{	L"Disc Expired",					L"Expiration disque",						L"Abgelaufene Scheibe",					L"Disco caducado",										L"Disco scaduto",						L"Disco expirado",									L"Disco expirado"	}	},
	{ ESTR_RQC						,	{	L"RQC Error",						L"Erreur RQC",								L"RQC-Fehler",							L"Error RQC",											L"Errore RQC",							L"Erro RQC",										L"Erro de RQC"	}	},
	{ ESTR_SAMPLE_BLANK_BEAD		,	{	L"Sample Blank Bead Error",			L"Erreur bille vierge éch.",				L"Probe leer, Bead-Fehler",				L"Err.  cuent. virg.  muest",							L"Err. granuli camp. vuoti",			L"Erro esf. branco amostra",						L"Erro esf. vazia amostra"	}	},
	{ ESTR_SAMPLE_DISTRIBUTION		,	{	L"Sample Distribution Error",		L"Erreur distribution éch.",				L"Probenverteilungsfehler",				L"Err. distrib. muestra",								L"Err. distrib. campione",				L"Erro de distrib. amostra",						L"Erro distr. de amostra"	}	},
	{ ESTR_SAMPLE_MIX				,	{	L"Sample Mix Error",				L"Erreur mix échantillon",					L"Probenmischfehler",					L"Error mezcla muestra",								L"Errore miscela campione",				L"Erro de mist. da amostra",						L"Erro mistura de amostra"	}	},
	{ ESTR_SAMPLE_QUALITY			,	{	L"Sample Quality Error",			L"Erreur qualité échantil.",				L"Fehler, Probenqualität",				L"Error calidad muestra",								L"Errore qualità campione",				L"Erro de qual. da amostra",						L"Erro qualidade da amostra"	}	},
	{ ESTR_SOFTWARE_EXPIRED         ,	{	L"Analyzer Needs Renewal",			L"Renouvell. analyseur obligatoire",		L"Erneuerung von Analysegerät erforderlich", L"Es necesario renovar el analizador",				L"Occorre aggiornare l’analizzatore",	L"É necessário renovar o analisador",				L"Analisador precisa de renovação" } },        
	{ ESTR_SPINDLE_MOTOR			,	{	L"Spindle Motor Error",				L"Erreur moteur de l'axe",					L"Fehler, Spindelmotor",				L"Error del motor del eje",								L"Errore asse motore",					L"Erro do eixo do motor",							L"Erro de motor do fuso"	}	},
	{ ESTR_SPINDLE_MOTOR_STUCK		,	{	L"Spindle Motor Error",				L"Erreur moteur de l'axe",					L"Fehler, Spindelmotor",				L"Error del motor del eje",								L"Errore asse motore",					L"Erro do eixo do motor",							L"Erro de motor do fuso"	}	},
	{ ESTR_TEMPERATURE				,	{	L"Temperature Error",				L"Erreur température",						L"Temperaturfehler",					L"Error de temperatura",								L"Errore temperatura",					L"Erro de temperatura",								L"Erro de temperatura"	}	},
	{ ESTR_UPDATED_ROTOR_TYPE		,	{	L"Updated Disc Type",				L"Type de disque actualisé",				L"Aktual. Scheibentyp",					L"Tipo de disco actualizado",							L"Tipo disco aggiornato",				L"Tipo de disco atualizado",						L"Tipo de disco atualizado"	}	},

// Warning screens							ENGLISH,																													FRENCH,																																				GERMAN,																																		SPANISH,																													ITALIAN, 																														PORTUGUESE,																															PORTUGUESE (BRAZIL)
	{ ESTR_ADMIN_NOT_FOUND			,	{	L"Administrator ID not found",																								L"ID admin. introuvable",																														L"Administrator-ID nicht gefunden",																											L"No se encontró Id. administrador",																						L"ID amministratore non trovato",																									L"ID de administrador não encontrada",																								L"ID de administrador não encontrado"	}	},
	{ ESTR_CANNOT_REMOVE_OP_ID		,	{	L"All operators must be deleted before the Operator ID screen can be removed",												L"Tous les opérateurs doivent être supprimés avant retrait de l'écran ID opérat.",																L"Alle Bediener müssen gelöscht werden, bevor der Bediener-ID-Bildschirm entfernt werden kann",												L"Debe eliminar todos los operarios antes de eliminar la pantalla Id. operario",											L"Prima di rimuovere lo schermo ID operatore, annullare tutti gli operatori",														L"Devem ser eliminados todos os operadores antes do ecrã de ID de operadores poder ser eliminado",									L"Todos os operadores devem ser excluídos antes que a tela de ID de operador possa ser removida"	}	},
	{ ESTR_CONTROL_EXPIRED			,	{	L"Control renewal required",																								L"Renouvell. contrôle obligatoire",																												L"Kontrollerneuerung erforderlich",																											L"Debe renovar control",																									L"Necessario rinnovo controllo",																									L"É necessário renovar o controlo",																									L"Renovação do controle necessária"	}	},
	{ ESTR_DATA_LOSS				,	{	L"Changing software will result in data loss.  Press CANCEL to archive data prior to upgrading software.",					L"La mise à niveau va occasionner la perte de données. Appuyer sur ANNULER pour archiver les données avant de mettre à niveau le logiciel.",	L"Bei einer Softwareaktualisierung werden Daten gelöscht. ABBRECHEN betätigen, um Daten vor der Softwareaktualisierung zu archivieren.",	L"Al cambiar el software perderá sus datos. Pulse CANCELAR si desea archivar los datos antes de actualizar el software.",	L"La modifica di software determina la perdita di dati. Premere ANNULLA per archiviare i dati prima di aggiornare il software.",	L"Ao alterar o software irá perder os seus dados. Prima CANCELAR se desejar arquivar os dados antes da atualização do software.",	L"A alteração do software resultará em perda de dados. Pressione CANCELAR para arquivar os dados antes de atualizar o software."	}	},
	{ ESTR_DATABASE_FILE_TOO_LARGE	,	{	L"Analyzer memory needs to be optimized.  Press CONTINUE to restart analyzer.",												L"La mémoire de l’analyseur doit être optimisée.  Appuyez sur CONTINUER pour redémarrer l’analyseur.",											L"Speicher des Analysegeräts muss optimiert werden.  FORTSETZEN drücken, um das Analysegerät neu zu starten.",								L"Es necesario optimizar la memoria del analizador.  Pulse CONTINUAR para reiniciar el analizador.",						L"È necessario ottimizzare la memoria dell’analizzatore.  Premere CONTINUA per riavviare l’analizzatore.",							L"A memória do analisador tem que ser otimizada.  Prima CONTINUAR para reiniciar o analisador.",									L"A memória do analisador precisa ser otimizada.  Pressione CONTINUAR para reiniciar o analisador.",	}	},
	{ ESTR_DUPLICATE_ADMIN_ID		,	{	L"Duplicate Administrator ID entered",																						L"ID admin. saisi en double",																													L"Doppelte Administrator-ID eingegeben",																									L"Esta Id. administrador ya existe",																						L"Inserito duplicato ID amministratore",																							L"A ID do administrador já existe",																									L"ID de administrador duplicado inserido"	}	},
	{ ESTR_DUPLICATE_OP_ID			,	{	L"Duplicate Operator ID entered",																							L"ID opérateur saisi en double",																												L"Doppelte Bediener-ID eingegeben",																											L"Esta Id. operario ya existe",																								L"Inserito duplicato ID operatore",																									L"A ID do operador já existe",																										L"Código de operador duplicado digitado"	}	},
	{ ESTR_OP_RENEWAL_REQUIRED		,	{	L"Operator renewal required",																								L"Renouvell. opérateur obligatoire",																											L"Bediener-Erneuerung erforderlich",																										L"Debe renovar operario",																									L"Necessario rinnovo operatore",																									L"É necessário renovar o operador",																									L"Renovação de operador necessária"	}	},
	{ ESTR_REMOVE_FROM_ADMIN_LIST	,	{	L"will be removed from administrators list",																				L"sera retiré de la liste d'administrateurs",																									L"wird aus der Administratorenliste entfernt",																								L"se eliminará de la lista de administradores", 																			L"sarà rimosso dall'elenco amministratori",																							L"será eliminado da lista de administradores",																						L"será removido da lista de administradores"	}	},
	{ ESTR_REMOVE_FROM_OP_LIST		,	{	L"will be removed from operators list",																						L"sera retiré de la liste d'opérateurs",																										L"wird aus der Bedienerliste entfernt",																										L"se eliminará de la lista de operarios",																					L"sarà rimosso dall'elenco operatori",																								L"será eliminado da lista de operadores",																							L"será removido da lista de operadores"	}	},
	{ ESTR_REMOVE_FROM_TYPE_LIST	,	{	L"will be removed from Type list",																							L"sera retiré du type de liste",																												L"wird aus der Typenliste entfernt",																										L"se eliminará de la lista de tipos",																						L"sarà rimosso dall'elenco Tipi",																									L"será eliminado da lista de tipos",																								L"será removido da lista de Tipo"	}	},
	{ ESTR_RESTORING_REF_RANGES		,	{	L"Restoring Reference Ranges to",																							L"Rétablissement des fourchettes de référence sur",																								L"Wiederherstellung der Referenzbereiche auf",																								L"Reestablecer los intervalos de referencia a",																				L"Riportare i range di riferimento a",																								L"Restaurar os intervalos de referência para",																						L"Restaurando intervalos de referência para"	}	},
	{ ESTR_ROTOR_DETECTED			,	{	L"Disc detected during warming",																							L"Disque détecté en cours de chauffage",																										L"Scheibe beim Aufwärmen vorgefunden",																										L"Disco detectado durante el calentamiento inicial",																		L"Disco individuato durante riscaldamento",																							L"Disco detetado durante o aquecimento",																							L"Disco detectado durante o aquecimento"	}	},
	{ ESTR_UNAUTHORIZED_OP			,	{	L"Unauthorized operator",																									L"Opérateur non-autorisé",																														L"Unautorisierter Bediener",																												L"Operario no autorizado",																									L"Operatore non autorizzato",																										L"Operador não autorizado",																											L"Operador não autorizado"	}	},

// Error info text
	{ EISTR_ADMIN_ID_LIST_FULL		,	{	L"All the available space for Administrator ID's is used.  Please remove at least one Administrator ID before adding a new Administrator ID.\n",													// English
											L"Tout l'espace disponible réservé aux ID administrateur est utilisé. Veuillez supprimer au moins un ID administrateur avant d'en ajouter un nouveau.\n",										// French
											L"Der gesamte Speicherplatz für Administrator-IDs ist belegt. Bitte entfernen Sie mindestens eine Administrator-ID, bevor Sie eine neue hinzufügen.\n",											// German
											L"Se ha utilizado todo el espacio disponible para números de identificación de administradores. Elimine al menos un número de identificación de administrador antes de añadir otro nuevo.\n",	// Spanish
											L"È stato utilizzato tutto lo spazio disponibile per l'ID amministratore. Rimuovere almeno un ID amministratore prima di aggiungere un nuovo ID amministratore.\n",								// Italian
											L"Foi utilizado todo o espaço disponível para IDs de administradores.  Elimine pelo menos uma ID de administrador antes de adicionar uma ID de administrador nova.\n",							// Portuguese
											L"Todo o espaço disponível para os IDs de administrador está usado.  Remova pelo menos um ID de administrador antes de adicionar um novo ID de administrador.\n"									// Portuguese (Brazil)
										}
	},

	{ EISTR_ADMIN_NOT_FOUND			,	{	L"The administrator ID entered is not in the system. Please re-enter the ID.\n",						// English
											L"L'ID admin. n'est pas saisi sur le système. Saisir à nouveau l'ID\n",								// French
											L"Die eingegebene Administrator-ID existiert nicht im System. Bitte geben Sie die ID erneut ein.\n",	// German
											L"Esta Id. administrador no está en el sistema. Por favor, vuelva a escribirla.\n",					// Spanish
											L"L'ID amministratore inserito non presente nel sistema. Pregasi inserire ID nuovamente.\n",			// Italian
											L"A ID de administrador introduzida não está no sistema. Introduza novamente a ID.\n",				// Portuguese
											L"O ID de administrador inserido não está no sistema. Insira novamente o ID.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_AMBIENT_TEMP			,	{	L"The temperature outside the analyzer is either too hot or too cold to run a disc. Contact Technical Service for assistance.\n",															// English
											L"La température à l'extérieur de l'analyseur est trop élevée ou trop faible pour permettre le fonctionnement d'un disque. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Die Temperatur außerhalb des Analysegeräts ist für den Scheibenbetrieb zu heiß oder zu kalt. Wenden Sie sich an den technischen Service.\n",												// German
											L"La temperatura fuera del analizador es demasiado alta o demasiado baja para usar el disco. Póngase en contacto con el servicio técnico.\n",												// Spanish
											L"La temperatura esterna all'analizzatore è troppo alta o troppo bassa per permettere il funzionamento di un disco. Per assistenza contattare il servizio tecnico.\n",						// Italian
											L"A temperatura fora do analisador é demasiado alta ou demasiado baixa para executar um disco. Contacte o Serviço de Assistência Técnica para obter assistência.\n",							// Portuguese
											L"A temperatura externa do analisador está muito alta ou muito baixa para executar um disco. Entre em contato com o Serviço Técnico para obter assistência.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_ANALYSIS_CANCELED		,	{	L"The analysis was cancelled by pressing the Cancel on the screen.\n",					// English
											L"Annulation de l'analyse par pression sur la touche Annuler sur l'écran\n",				// French
											L"Die Analyse wurde mit der Schaltfläche Abbrechen auf dem Bildschirm abgebrochen.\n",	// German
											L"El análisis fue cancelado al pulsar Cancelar en la pantalla.\n",						// Spanish
											L"L'analisi è stata annullata premendo il tasto Annulla sullo schermo.\n",				// Italian
											L"A análise foi cancelada ao premir Cancelar no ecrã.\n",								// Portuguese
											L"A análise foi cancelada pela pressão do botão Cancelar na tela.\n"						// Portuguese (Brazil)
										}
	},
	{ EISTR_ANALYSIS_TIMEOUT		,	{	L"The analyzer is not completing the analysis within the time allotted. Contact Technical Service for assistance.\n",								// English
											L"L'Analyseur n'a pas terminé l'analyse dans le temps imparti. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											L"Das Analysegerät führt die Analyse nicht im vorgesehenen Zeitraum durch. Wenden Sie sich an den technischen Service.\n",						// German
											L"El analizador no está completando el análisis en el tiempo asignado. Póngase en contacto con el servicio técnico.\n",							// Spanish
											L"L'analizzatore non sta completando l'analisi entro il tempo assegnato. Per assistenza contattare il servizio tecnico\n",						// Italian
											L"O analisador não está a concluir a análise no tempo atribuído.  Contacte o Serviço de Assistência Técnica para obter assistência\n",			// Portuguese
											L"O analisador não está concluindo a análise dentro do tempo alocado.  Entre em contato com o Serviço Técnico para obter assistência\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_ANALYZER_DATE			,	{	L"Date in analyzer is earlier than that of the disc being run. Check dates in analyzer and rerun with new disc.\n",												// English
											L"La date de l'analyseur est antérieure à celle du disque en cours d'utilisation. Vérifier les dates sur l'analyseur et réexécuter le nouveau disque.\n",		// French
											L"Das Datum im Analysegerät liegt vor dem des Scheibenbetriebs. Prüfen Sie das Datum im Analysegerät und nehmen Sie den Betrieb mit einer neuen Scheibe auf.\n",	// German
											L"La fecha del analizador es anterior a la fecha del disco que se va a usar. Compruebe las fechas del analizador y vuelva a usar el nuevo disco.\n",				// Spanish
											L"La data dell'analizzatore è precedente a quella di attivazione del disco Controllare le date dell'analizzatore e riavviare con un nuovo disco.\n",				// Italian
											L"A data do analisador é anterior à data do disco que foi iniciado. Verifique as datas do analisador e reinicie com um disco novo.\n",							// Portuguese
											L"A data no analisador é anterior à do disco em execução. Verifique as datas no analisador e execute novamente com um novo disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_BACKGROUND_ABSORBANCE	,	{	L"Background absorbance is too high. Contact Technical Service for assistance.\n",								// English
											L"L'absorbance d'arrière-plan est trop élevée. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Die Hintergrundabsorbierung ist zu hoch. Wenden Sie sich an den technischen Service.\n",						// German
											L"La absorbancia de fondo es muy alta. Póngase en contacto con el servicio técnico.\n",							// Spanish
											L"L'assorbimento di fondo è troppo alto. Per assistenza contattare il servizio tecnico.\n",						// Italian
											L"A absorvância de fundo é muito elevada. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											L"A absorbância do fundo está muito alta. Entre em contato com o Serviço Técnico para obter assistência.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_BACKGROUND_NOISE		,	{	L"The analyzer detected a high background signal. This may impact the accuracy of the analyzer's absorbance measurement. If symptom continues, contact Technical Service.\n",														// English
											L"L'analyseur a détecté un signal d'arrière-plan élevé. Ceci peut affecter la précision de la mesure d'absorbance par l'analyseur. En cas de persistance des symptômes, contacter le service technique.\n",							// French
											L"Das Analysegerät hat ein hohes Hintergrundsignal festgestellt. Dies kann die Genauigkeit der Absorbierungsmessung des Analysegeräts beeinträchtigen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",	// German
											L"El analizador detecta una señal de fondo alta. Esto puede afectar a la precisión de la medida de absorbancia del analizador. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",			// Spanish
											L"L'analizzatore ha individuato un forte segnale di fondo. Esso potrebbe influire sulla precisione della misurazione di assorbenza dell'analizzatore. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											L"O analisador detetou um sinal de fundo elevado. Isto pode afetar a precisão da medição da absorvância do analisador. Se o problema persistir, contacte o Serviço de Assistência Técnica.\n",										// Portuguese
											L"O analisador detectou um sinal de fundo alto. Isso pode afetar a precisão da medição de absorbância do analisador. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"												// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE					,	{	L"The analyzer was unable to read the disc's barcode. If symptom continues, contact Technical Service.\n",												// English
											L"L'analyseur n'a pas pu lire le code à barres du disque. En cas de persistance des symptômes, contacter le service technique.\n",						// French
											L"Das Analysegerät konnte den Barcode der Scheibe nicht lesen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",				// German
											L"El analizador no pudo leer el código de barras del disco. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"L'analizzatore non è riuscito a leggere il codice a barre del disco. Se i sintomi continuano contattare il servizio tecnico.\n",						// Italian
											L"O analisador não consegue ler o código de barras do disco. Se o problema persistir, contacte o Serviço de Assistência Técnica.\n",						// Portuguese
											L"O analisador não foi capaz de ler o código de barras do disco. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE_FORMAT			,	{	L"The analyzer did not recognize the disc. Contact Technical Service for further assistance.\n",							// English
											L"L'analyseur n'a pas reconnu le disque. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											L"Das Analysegerät konnte die Scheibe nicht erkennen. Wenden Sie sich an den technischen Service.\n",					// German
											L"El analizador no ha reconocido el disco. Póngase en contacto con el servicio técnico.\n",								// Spanish
											L"L'analizzatore non ha riconosciuto il disco. Per ulteriore assistenza contattare il servizio tecnico.\n",				// Italian
											L"O analisador não consegue reconhecer o disco. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											L"O analisador não reconheceu o disco. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MISSING			,	{	L"The analyzer did not detect all the chemistry beads in the disc. Rerun sample using new disc.\n",										// English
											L"L'analyseur n'a pas détecté toutes les billes chimiques dans le disque. Réexécuter l'échantillon à l'aide d'un nouveau disque.\n",		// French
											L"Das Analysegerät konnte nicht alle chemischen Beads in der Scheibe erkennen. Wiederholen Sie die Probe mit einer neuen Scheibe.\n",	// German
											L"El analizador no detectó todas las cuentas químicas en el disco. Vuelva a utilizar la muestra usando un nuevo disco.\n",				// Spanish
											L"L'analizzatore non ha individuato tutti i granuli chimici nel disco. Riavviare il campione con un nuovo disco.\n",						// Italian
											L"O analisador não detetou todas as esferas químicas no disco. Executar novamente a amostra utilizando um disco novo.\n",				// Portuguese
											L"O analisador não detectou todas as esferas de químicas no disco. Execute novamente a amostra usando um novo disco.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MIX				,	{	L"The analyzer will not be able to determine the exposure status if the QC beads have not mixed correctly. Rerun with a new disc.\n",									// English
											L"L'analyseur ne parviendra pas à déterminer l'état d'exposition si les billes QC ne sont pas correctement mélangées. Réexécuter avec un nouveau disque.\n",				// French
											L"Das Analysegerät kann den Status der Einwirkung nicht feststellen, wenn die QC-Beads nicht ordnungsgemäß vermischt wurden. Mit einem neuen Rotor wiederholen.\n",		// German
											L"El analizador no será capaz de determinar el estado de exposición si las cuentas QC no se mezclan correctamente. Debe usarse con un nuevo disco.\n",					// Spanish
											L"L'analizzatore non potrà determinare lo stato di esposizione se i granuli di QC non sono stati mescolati correttamente. Riavviare con un nuovo disco.\n",				// Italian
											L"O analisador não conseguirá determinar o estado de exposição se as esferas de CQ não forem misturadas adequadamente. Executar novamente utilizando um disco novo.\n",	// Portuguese
											L"O analisador não poderá determinar o status de exposição se as esferas de QC não tiverem sido misturadas corretamente. Execute novamente com um novo disco.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_BIAS					,	{	L"The analyzer detected a high bias signal. This may impact the accuracy of the analyzer's measurement of absorbance. If symptom continues, contact Technical Service.\n",														// English
											L"L'analyseur a détecté un signal de polarisation élevé, ce qui peut affecter la précision de la mesure d'absorbance par l'analyseur. En cas de persistance des symptômes, contacter le service technique.\n",					// French
											L"Das Analysegerät hat ein hohes Störsignal festgestellt. Dies kann die Genauigkeit der Absorbierungsmessung des Analysegeräts beeinträchtigen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",		// German
											L"El analizador detecta una señal de polarización alta. Esto puede afectar a la precisión de la medida de absorbancia del analizador. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"L'analizzatore ha individuato un forte segnale di bias. Esso potrebbe influire sulla precisione della misurazione di assorbenza dell'analizzatore. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											L"O analisador detetou um sinal de desvio elevado. Isto pode afetar a precisão da medição da absorvância do analisador. Se o problema persistir, contacte o Serviço de Assistência Técnica.\n",									// Portuguese
											L"O analisador detectou um sinal de polarização alto. Isso pode afetar a precisão da medição de absorbância do analisador. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_BOTTOM_HEATER_HIGH		,	{	L"The bottom heater plate draws too much current when turned on at full power. Contact Technical Service for assistance.\n",												// English
											L"La plaque chauffante inférieure consomme trop d'électricité lorsqu'elle fonctionne à fond. Pour obtenir une assistance, contacter le service technique.\n",			// French
											L"Die untere Heizplatte verbraucht bei Maximalbetrieb zu viel Strom. Wenden Sie sich an den technischen Service.\n",														// German
											L"La placa del calentador inferior utiliza mucha corriente cuando se enciende a toda potencia. Póngase en contacto con el servicio técnico.\n",							// Spanish
											L"Il disco di riscaldamento inferiore prende troppa corrente quando è acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",					// Italian
											L"A placa inferior do aquecedor utiliza demasiada de corrente quando ligada na potência máxima. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											L"A placa inferior do aquecedor consome muita corrente quando ligada na potência máxima. Entre em contato com o Serviço Técnico para obter assistência.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_BOTTOM_HEATER_LOW		,	{	L"The bottom heater plate did not draw enough current when turned on at full power. Contact Technical Service for assistance.\n",										// English
											L"La plaque chauffante inférieure n'a pas consommé trop d'électricité lorsqu'elle a fonctionné à fond. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Die untere Heizplatte verbrauchte bei Maximalbetrieb zu wenig Strom. Wenden Sie sich an den technischen Service.\n",													// German
											L"La placa del calentador inferior no utilizó suficiente corriente cuando se puso a toda potencia. Póngase en contacto con el servicio técnico.\n",						// Spanish
											L"Il disco di riscaldamento inferiore non ha preso sufficiente corrente quando è acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",			// Italian
											L"A placa inferior do aquecedor não utiliza corrente suficiente quando ligada na potência máxima. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											L"A placa inferior do aquecedor não atraiu corrente suficiente quando ligada na potência máxima. Entre em contato com o Serviço Técnico para obter assistência.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_CANNOT_REMOVE_OP_ID		,	{	L"To remove the operator ID prompt it is necessary to remove all operator IDs first.  Please use the Operator Administration menu to remove all operators, and then return here to remove the operator prompt.\n",																// English
											L"Pour supprimer l'invite ID opérateur, il est nécessaire de commencer par supprimer tous les ID opérateur.  Supprimer tous les opérateurs à partir du menu Administration opérateur, puis revenir ici pour supprimer l'invite opérateur.\n",									// French
											L"Um die Bediener-ID-Eingabeaufforderung zu entfernen, müssen Sie zuerst alle Bediener-IDs löschen. Bitte machen Sie dies über das Bediener-Administrationsmenü und gehen zurück, um die Bediener-Eingabeaufforderung zu entfernen.\n",											// German
											L"Para eliminar el aviso de Id. operario debe eliminar todas las Id. operador. Para esto, vaya al menú Administración de operarios para eliminar todos los operarios y después vuelva aquí para eliminar el aviso.\n",															// Spanish
											L"Per rimuovere il prompt ID operatore si devono prima rimuovere tutti gli ID operatore. Pregasi utilizzare il menu Amministrazione operatore per rimuovere tutti gli operatori, quindi tornare a questo punto e rimuovere il prompt operatore.\n",								// Italian
											L"Para eliminar o aviso de ID de operador é necessário eliminar primeiro todas as ID de operador.  Para remover todos os operadores utilize o menú Administração de operadores para eliminar todos os operadores e, em seguida, volte novamente aqui para eliminar o aviso.\n",	// Portuguese
											L"Para remover o prompt do ID de operador, é necessário remover todos os IDs de operador antes.  Use o menu Administração do operador para remover todos os operadores e, em seguida, retorne aqui para remover o prompt do operador.\n"											// Portuguese (Brazil)
										} 
	},
	{ EISTR_CDROM					,	{	L"The analyzer is unable to read information from the CD ROM. Contact Technical Service for assistance.\n",									// English
											L"L'analyseur n'est pas en mesure de lire des informations sur le CD ROM. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Das Analysegerät kann die CD-ROM nicht lesen. Wenden Sie sich an den technischen Service.\n",												// German
											L"El analizador no puede leer la información del CD-ROM. Póngase en contacto con el servicio técnico.\n",									// Spanish
											L"L'analizzatore non è in grado di leggere le informazioni dal CD ROM. Per assistenza contattare il servizio tecnico.\n",					// Italian
											L"O analisador não consegue ler a informação do CD-ROM. Contacte o Serviço de Assistência Técnica para obter assistência.\n",				// Portuguese
											L"O analisador não é capaz de ler as informações do CD-ROM. Entre em contato com o Serviço Técnico para obter assistência.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_CONTROL_EXPIRED			,	{	L"The date set for analyzing control samples has passed.  Please contact technical support for more information.\n",										// English
											L"La date prévue pour l'analyse des échantillons de contrôle est dépassée. Pour plus d'informations, contacter l'assistance technique.\n",				// French
											L"Das angegebene Datum, um die Kontrollproben zu analysieren, ist abgelaufen. Wenden Sie sich an den technischen Service für weitere Information.\n",	// German
											L"La fecha para analizar las muestras de control ya ha pasado. Póngase en contacto con el servicio técnico.\n",											// Spanish
											L"La data impostata per l'analisi dei campioni di controllo è stata superata. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",		// Italian
											L"A data definida para analisar as amostras controlo foi excedida.  Para obter mais informações, contacte o Serviço de Assistência Técnica.\n",			// Portuguese
											L"A data configurada para analisar as amostras de controle já passou.  Entre em contato com o suporte técnico para obter mais informações.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_CUVETTE_MARK			,	{	L"The analyzer was unable to detect the disc. If symptom continues, contact Technical Service.\n",												// English
											L"L'analyseur n'est pas parvenu à détecter le disque. En cas de persistance des symptômes, contacter le service technique.\n",					// French
											L"Das Analysegerät konnte die Scheibe nicht finden. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",					// German
											L"El analizador no fue capaz de detectar el disco. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"L'analizzatore non è riuscito a individuare il disco. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"O analisador não consegue detetar o disco. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",								// Portuguese
											L"O analisador não conseguiu detectar o disco. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_DARK_NOISE				,	{	L"There was high variability in the background signal. This degrades the accuracy of the analyzer to measure absorbance. If symptom continues, contact Technical Service.\n",														// English
											L"Le signal d'arrière-plan a présenté une variabilité élevée, ce qui réduit la précision de l'analyseur pour la mesure d'absorbance. En cas de persistance des symptômes, contacter le Service technique.\n",						// French
											L"Es bestand eine hohe Hintergrundsignalvarianz. Dies beeinträchtigt die Genauigkeit der Absorbierungsmessung des Analysegeräts. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",							// German
											L"Había una gran variabilidad en la señal de fondo. Esto rebaja la precisión del analizador para analizar la medida de la absorbancia. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"Grande variabilità nel segnale di fondo. Ciò influisce negativamente la precisione dell'analizzatore nella misurazione dell'assorbenza. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"Foi observada uma grande variabilidade no sinal de fundo. Isto reduz a precisão do analisador para realizar medições de absorvância. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",						// Portuguese
											L"Houve alta variabilidade no sinal de fundo. Isso degrada a precisão do analisador para medir a absorbância. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"														// Portuguese (Brazil)
										} 
	},
	{ EISTR_DATA_LOSS				,	{	L"The software needs to change the format of the data on the analyzer.  It is not possible to convert the data and change the software.  Pressing CANCEL will allow the existing data to be archived prior to upgrading the software.  Use the Transmit All selection under the Recall screen to archive the existing data external to the analyzer.  Verify the external archive prior to upgrading the software.\n",								// English
											L"Le logiciel doit modifier le format des données sur l'analyseur. Il n'est pas possible de convertir les données et de mettre à niveau le logiciel. Appuyer sur ANNULER pour archiver les données avant la mise à niveau du logiciel. Appuyer sur Transmettre tout sous l'écran Rappel pour archiver les données de l'analyseur sur un support externe. Vérifier l'archive externe avant de mettre à niveau le logiciel.\n",						// French
											L"Die Software ändert das Datenformat auf dem Analysesystem. Es ist nicht möglich, die Daten zu konvertieren und die Software zu aktualisieren. ABBRECHEN betätigen, um die bestehenden Daten vor der Softwareaktualisierung zu archivieren. Die Funktion „Alle übertragen“ im Menü „Wiederaufruf“ verwenden, um die bestehenden Daten auf einem externen Gerät zu archivieren. Das externe Archiv vor der Softwareaktualisierung überprüfen.\n",	// German
											L"El software debe cambiar el formato de los datos del analizador. No se pueden convertir los datos y cambiar el software. Pulse CANCELAR si desea archivar los datos existentes antes de actualizar el software. Para archivar los datos existentes fuera del analizador, use la opción Trasmitir todo de la pantalla Recordar. Antes de actualizar el software, verifique el archivo externo.\n",													// Spanish
											L"Il software deve modificare il formato dei dati sull'analizzatore. Non è possibile convertire i dati e cambiare il software. Premendo ANNULLA si consente l'archiviazione dei dati esistenti prima dell'aggiornamento del software. Usare la selezione Trasmetti tutti nella schermata Richiama per archiviare i dati esistenti all'esterno dell'analizzatore. Verificare l'archivio esterno prima di aggiornare il software.\n",					// Italian
											L"O software necessita alterar o formato dos dados do analisador.  Não é possível converter os dados e alterar o software.  Ao premir CANCELAR permitirá arquivar os dados existentes antes de atualizar o software.  Para arquivar os dados existentes externos ao analisador, utilize a opção Transmitir tudo no ecrã Recuperar.  Antes de atualizar o software, verifique o ficheiro externo.\n",													// Portuguese
											L"O software precisa alterar o formato dos dados no analisador.  Não é possível converter os dados e alterar o software.  Pressione CANCELAR para que os dados existentes sejam arquivados antes de atualizar o software.  Use a seleção Transmitir todos na tela Recuperar para arquivar os dados existentes externos ao analisador.  Verifique o arquivo externo antes de atualizar o software.\n"													// Portuguese (Brazil)
										}
	},
    {EISTR_DATABASE_FILE_TOO_LARGE ,    {	L"The analyzer memory needs to be optimized. The memory will be optimized and the analzyer will restart. \n",								// English
											L"La mémoire de l’analyseur doit être optimisée. La mémoire va être optimisée et l’analyseur va redémarrer.\n",								// French 
											L"Der Speicher des Analysegeräts muss optimiert werden. Der Speicher wird optimiert und das Analysegerät wird neu gestartet.\n",			// German
											L"Es necesario optimizar la memoria del analizador. La memoria se optimizará y el analizador se reiniciará.\n",								// Spanish
											L"È necessario ottimizzare la memoria dell’analizzatore. La memoria verrà ottimizzata e l’analizzatore riavviato.\n",						// Italian
											L"A memória do analisador tem que ser otimizada. A memória será otimizada e o analisador será reiniciado.\n",								// Portuguese
											L"A memória do analisador precisa ser otimizada. A memória será otimizada e o analisador será reiniciado.\n"								// Portuguese (Brazil)
                                        } 
    },
	{ EISTR_DILUTION				,	{	L"The analyzer may not have delivered the correct amount of sample to the reagents. Confirm the reported values.\n",						// English
											L"Il se peut que l'analyseur n'ait pas dispensé la quantité d'échantillon voulue sur les réactifs. Confirmer les valeurs fournies.\n",	// French
											L"Das Analysegerät hat möglicherweise die falsche Probenmenge den Reagenzien zugeführt. Berichtete Werte bestätigen.\n",					// German
											L"Es posible que el analizador no proporcione la cantidad correcta de muestra a los reactivos. Confirme los valores dados.\n",			// Spanish
											L"L'analizzatore potrebbe non aver portato la quantità corretta di campione ai reagenti. Confermare i valori riportati.\n",				// Italian
											L"É possível que o analisador não tenha fornecido a quantidade adequada de amostra aos reagentes. Confirme os valores reportados.\n",	// Portuguese
											L"O analisador pode não ter entregue a quantidade correta de amostra aos reagentes. Confirme os valores relatados.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_DRAWER_CLOSE			,	{	L"The drawer appears to be unable to close correctly. Remove any visible obstructions. If symptom continues, contact Technical Service.\n",											// English
											L"Le tiroir ne semble pas se fermer correctement. Éliminer tout élément visible responsable du blocage. En cas de persistance des symptômes, contacter le service technique.\n",		// French
											L"Das Fach kann nicht ordnungsgemäß geschlossen werden. Entfernen Sie sichtbare Hindernisse. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",				// German
											L"Parece que la bandeja no puede cerrarse correctamente. Retire todo obstáculo visible. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"Sembra che il cassetto non riesca a chiudersi perfettamente. Rimuovere qualsiasi ostruzione visibile. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											L"Parece que a gaveta não fecha corretamente. Remova qualquer obstáculo visível. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",								// Portuguese
											L"A gaveta parece incapaz de fechar corretamente. Remova quaisquer obstruções visíveis. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_DRAWER_OPEN				,	{	L"The drawer appears to be unable to open correctly. Remove any visible obstructions. If symptom continues, contact Technical Service.\n",											// English
											L"Le tiroir ne semble pas s'ouvrir correctement. Éliminer tout élément visible responsable du blocage. En cas de persistance des symptômes, contacter le service technique.\n",		// French
											L"Das Fach kann nicht ordnungsgemäß geöffnet werden. Entfernen Sie sichtbare Hindernisse. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",				// German
											L"Parece que la bandeja no puede abrirse correctamente. Retire todo obstáculo visible. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"Sembra che il cassetto non riesca ad aprirsi perfettamente. Rimuovere qualsiasi ostruzione visibile. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											L"Parece que a gaveta não abre corretamente. Remova qualquer obstáculo visível. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",								// Portuguese
											L"A gaveta parece incapaz de abrir corretamente. Remova quaisquer obstruções visíveis. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_DUPLICATE_ADMIN_ID		,	{	L"The Administrator ID entered already exists in the system. Duplicate IDs are not allowed.\n",		// English
											L"L'ID administrateur saisi existe déjà sur le système. Les ID en double ne sont pas autorisés.\n",	// French
											L"Die eingegebene Administrator-ID existiert bereits im System. Doppelte IDs sind nicht erlaubt.\n",	// German
											L"Esta Id. administrador ya existe en el sistema. No se permiten Id. duplicadas.\n",					// Spanish
											L"L'ID amministratore inserito è già presente nel sistema. Non sono consentiti ID duplicati.\n",		// Italian
											L"A ID de administrador introduzida já existe no sistema. Não são permitidas IDs duplicadas.\n",		// Portuguese
											L"O ID de administrador inserido já existe no sistema. IDs duplicados não são permitidos.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_DUPLICATE_OP_ID			,	{	L"The Operator ID entered already exists in the system. Duplicate IDs are not allowed.\n",		// English
											L"L'ID opérateur saisi existe déjà sur le système. Les ID en double ne sont pas autorisés.\n",	// French
											L"Die eingegebene Bediener-ID existiert bereits im System. Doppelte IDs sind nicht erlaubt.\n",	// German
											L"Esta Id. operario ya existe en el sistema. No se permiten Id. duplicadas.\n",					// Spanish
											L"L'ID operatore inserito è già presente nel sistema. Non sono consentiti ID duplicati.\n",		// Italian
											L"A ID de operador introduzida já existe no sistema. Não são permitidas IDs duplicadas.\n",		// Portuguese
											L"O ID de operador inserido já existe no sistema. IDs duplicados não são permitidos.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_ENGINE_CODE_DOWNLOAD	, {		L"The analyzer detected an error when updating the software. Contact Technical Service for assistance.\n",    // English
											L"L'analyseur a détecté une erreur lors de la mise à jour du logiciel. Pour obtenir une assistance, contacter le service technique.\n",    // French
											L"Das Analysegerät hat einen Fehler beim Aktualisieren der Software festgestellt. Wenden Sie sich an den technischen Service.\n",    // German
											L"El analizador ha detectado un error al actualizar el software. Póngase en contacto con el servicio técnico.\n",    // Spanish
											L"L'analizzatore ha rilevato un errore durante l'aggiornamento del software. Per assistenza contattare il servizio tecnico.\n",    // Italian
											L"Ao atualizar o software o analisador detetou um erro. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese	
											L"O analisador detectou um erro ao atualizar o software. Entre em contato com o Serviço Técnico para obter assistência.\n"		// Portuguese (Brazil)
                                      }
    },	
	{ EISTR_ENGINE_CODE_OUT_SYNC	, {		L"The analyzer detected an error with the software. Please install the software from CD, If symptom continues, contact Technical Service.\n",									// English	
											L"L'analyseur a détecté une erreur dans le logiciel. Installer le logiciel à partir du CD. En cas de persistance des symptômes, contacter le service technique.\n",				// French
											L"Das Analysegerät hat einen Softwarefehler festgestellt. Installieren Sie die Software von der CD. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service\n",	// German
											L"El analizador ha detectado un error con el software. Instale el software desde el CD. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",//Spanish
											L"L'analizzatore ha rilevato un errore relativo al software. Installare il software dal CD. Se i sintomi continuano contattare il servizio tecnico.\n",							 //Italian 
											L"O analisador detetou um erro no software. Instale o software a partir do CD. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",							// Portuguese
											L"O analisador detectou um erro no software. Instale o software do CD. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"											// Portuguese (Brazil)
                                      }
    },
	{ EISTR_ENGINE_RESET			,	{	L"A component of the analyzer has restarted unexpectedly. Contact Technical Service for assistance.\n",									// English
											L"Un des composants de l'analyseur a redémarré de manière inattendue. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Eine Komponente des Analysegeräts wurde unerwartet neu gestartet. Wenden Sie sich an den technischen Service.\n",						// German
											L"Un componente del analizador se ha reiniciado inesperadamente. Póngase en contacto con el servicio técnico.\n",						// Spanish
											L"Un componente dell'analizzatore si è riavviato in modo imprevisto. Per assistenza contattare il servizio tecnico\n",					// Italian
											L"Um componente do analisador foi reiniciado inesperadamente.  Contacte o Serviço de Assistência Técnica para obter assistência.\n",		// Portuguese
											L"Um componente do analisador foi reiniciado inesperadamente.  Entre em contato com o Serviço Técnico para obter assistência\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_EXPIRED_ROTOR			,	{	L"The disc has expired and should not be used. Contact Technical Service for further details.\n",										// English
											L"La date de péremption du disque est dépassée. Ne pas l'utiliser. Pour obtenir plus d'informations, contacter le service technique.\n",	// French
											L"Die Scheibe ist abgelaufen und sollte ersetzt werden. Wenden Sie sich an den technischen Service.\n",									// German
											L"El disco ha caducado y no debería utilizarse. Póngase en contacto con el servicio técnico.\n",											// Spanish
											L"Il disco è scaduto e non dovrebbe essere usato. Per ulteriore dettagli contattare il servizio tecnico.\n",								// Italian
											L"O disco expirou e não deverá ser utilizado. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",				// Portuguese
											L"O disco expirou e não deve ser usado. Entre em contato com o Serviço Técnico para obter detalhes adicionais.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_FILTER_CURV				,	{	L"The filter curve calibration factors have been corrupted. Contact Technical Service for assistance.\n",								// English
											L"Les facteurs de calibrage de la courbe du filtre sont corrompus. Pour obtenir une assistance, contacter le service technique.\n",		// French
											L"Die Kalibrierungsfaktoren der Filterkurve sind fehlerhaft. Wenden Sie sich an den technischen Service.\n",								// German
											L"Los factores de calibración de la curva del filtro están dañados. Póngase en contacto con el servicio técnico.\n",						// Spanish
											L"I fattori di calibrazione della curva del filtro sono corrotti. Per assistenza contattare il servizio tecnico.\n",						// Italian
											L"Os fatores da curva de calibração do filtro estão danificados. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											L"Os fatores de calibração da curva do filtro foram corrompidos. Entre em contato com o Serviço Técnico para obter assistência.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_FLASH_LAMP_NOISE		,	{	L"During one of the system checks, the analyzer attempted to compute an absorbance from a cluster of flashes that are too variable from each other. If symptom continues, contact Technical Service.\n",															// English
											L"Lors de l'une des vérifications du système, l'analyseur a tenté de calculer l'absorbance à partir d'un ensemble de flashs qui varient trop les uns des autres. En cas de persistance des symptômes, contacter le service technique.\n",						// French
											L"Bei einer Systemprüfung unternahm das Analysegerät den Versuch, eine Absorbierung aus einer Gruppe von Lichtstrahlen zu berechnen, die zu sehr voneinander abweichen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",				// German
											L"Durante una de las comprobaciones del sistema, el analizador intentó calcular una absorbancia de un grupo de flashes que son excesivamente distintos unos de otros. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"Durante uno dei controlli di sistema l'analizzatore ha cercato di calcolare l'assorbenza in un gruppo di flash luminosi troppo diversi tra loro. Se i sintomi continuano contattare il servizio tecnico.\n",													// Italian
											L"Durante uma das verificações do sistema, o analisador tentou calcular uma absorvância de um grupo de flashes extremamente diferentes uns dos outros. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",									// Portuguese
											L"Durante uma das verificações do sistema, o analisador tentou calcular uma absorbância de um conjunto de flashes que são muito variáveis um em relação ao outro. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_FLASH_READ				,	{	L"The analyzer is unable to read information required to operate. Contact Technical Service for assistance.\n",											// English
											L"L'analyseur n'est pas en mesure de lire les informations requises pour fonctionner. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Das Analysegerät kann für den Betrieb erforderliche Informationen nicht abrufen. Wenden Sie sich an den technischen Service.\n",						// German
											L"El analizador es incapaz de leer información necesaria para funcionar. Póngase en contacto con el servicio técnico.\n",								// Spanish
											L"L'analizzatore non è in grado di leggere le informazioni necessarie per il funzionamento. Per assistenza contattare il servizio tecnico.\n",			// Italian
											L"O analisador não consegue ler informação necessária para funcionar. Contacte o Serviço de Assistência Técnica para obter assistência.\n",				// Portuguese
											L"O analisador não é capaz de ler as informações necessárias para operar. Entre em contato com o Serviço Técnico para obter assistência.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_FLASH_SAVE				,	{	L"The analyzer is unable to save information required to operate. Contact Technical Service for assistance.\n",												// English
											L"L'analyseur n'est pas en mesure d'enregistrer les informations requises pour fonctionner. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Das Analysegerät kann für den Betrieb erforderliche Informationen nicht speichern. Wenden Sie sich an den technischen Service.\n",							// German
											L"El analizador es incapaz de guardar información necesaria para funcionar. Póngase en contacto con el servicio técnico.\n",									// Spanish
											L"L'analizzatore non è in grado di salvare le informazioni necessarie per il funzionamento. Per assistenza contattare il servizio tecnico.\n",				// Italian
											L"O analisador não consegue gravar informação necessária para funcionar. Contacte o Serviço de Assistência Técnica para obter assistência.\n",				// Portuguese
											L"O analisador não é capaz de salvar as informações necessárias para operar. Entre em contato com o Serviço Técnico para obter assistência.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_HARDWARE				,	{	L"The analyzer has detected an error in the hardware. Contact Technical Service for further details.\n",					// English
											L"L'analyseur a détecté une erreur de matériel. Pour obtenir plus d'informations, contacter le service technique.\n",	// French
											L"Das Analysegerät hat einen Hardwarefehler festgestellt. Wenden Sie sich an den technischen Service.\n",				// German
											L"El analizador ha detectado un error en el equipo. Póngase en contacto con el servicio técnico.\n",						// Spanish
											L"L'analizzatore ha individuato un errore nell'hardware. Per ulteriore dettagli contattare il servizio tecnico.\n",		// Italian
											L"O analisador detetou um erro no hardware. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",	// Portuguese
											L"O analisador detectou um erro no hardware. Entre em contato com o Serviço Técnico para obter detalhes adicionais.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_ILLEGAL_BARCODE			,	{	L"The analyzer has detected an illegal barcode. Contact Technical Service for further details.\n",								// English
											L"L'analyseur a détecté un code à barres non autorisé. Pour obtenir plus d'informations, contacter le service technique.\n",		// French
											L"Das Analysegerät hat einen falschen Barcode festgestellt. Wenden Sie sich an den technischen Service.\n",						// German
											L"El analizador ha detectado un código de barras ilegal. Póngase en contacto con el servicio técnico.\n",						// Spanish
											L"L'analizzatore ha individuato un codice a barre illegale. Per ulteriore dettagli contattare il servizio tecnico.\n",			// Italian
											L"O analisador detetou um código de barras inválido. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",	// Portuguese
											L"O analisador detectou um código de barras ilegal. Entre em contato com o Serviço Técnico para obter detalhes adicionais.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_ILLEGAL_IP_ADDRESS		,	{	L"The IP Address entered cannot be used.  Please enter an IP address in the N.N.N.N format where N is a number between 0 and 255.\n",						// English
											L"L'adresse IP saisie ne peut pas être utilisée.  Entrer une adresse IP au format N.N.N.N, N étant un nombre compris entre 0 et 255.\n",						// French
											L"Die eingegebene IP-Adresse ist nicht nutzbar. Bitte geben Sie eine IP-Adresse im Format N.N.N.N ein, wobei N eine Zahl zwischen 0 und 255 ist.\n",			// German
											L"La dirección IP introducida no puede utilizarse.  Por favor, introduzca una dirección IP con el formato N.N.N.N, donde N es un número entre 0 y 255.\n",	// Spanish
											L"Impossibile utilizzare l'indirizzo IP specificato.  Immettere un indirizzo IP nel formato N.N.N.N. dove N è un numero compreso fra 0 e 255.\n",			// Italian
											L"O endereço IP introduzido não pode ser utilizado.  Introduza um endereço IP com o formato N.N.N.N, onde N é um número entre 0 e 255.\n",					// Portuguese
											L"O endereço IP inserido não pode ser usado.  Insira um endereço IP no formato N.N.N.N, em que N é um número entre 0 e 255.\n"								// Portuguese (Brazil)
										}
	},

	{ EISTR_ILLEGAL_IP_MASK			,	{	L"The IP Mask entered cannot be used.  Please enter an IP Mask in the N.N.N.N format where N is a number between 0 and 255.\n",								// English
											L"Le masque IP saisi ne peut pas être utilisé.  Entrer un masque IP au format N.N.N.N, N étant un nombre compris entre 0 et 255.\n",							// French
											L"Die eingegebene IP-Maske ist nicht nutzbar. Bitte geben Sie eine IP-Maske im Format N.N.N.N ein, wobei N eine Zahl zwischen 0 und 255 ist.\n",				// German
											L"La máscara IP introducida no puede utilizarse.  Por favor, introduzca una máscara IP con el formato N.N.N.N, donde N es un número entre 0 y 255.\n",		// Spanish
											L"Impossibile utilizzare la maschera IP specificata.  Immettere una maschera IP nel formato N.N.N.N. dove N è un numero compreso fra 0 e 255.\n",			// Italian
											L"A máscara de IP introduzida não pode ser utilizada.  Introduza uma máscara de IP com o formato N.N.N.N, onde N é um número entre 0 e 255.\n",				// Portuguese
											L"A máscara IP inserida não pode ser usada.  Insira uma máscara IP no formato N.N.N.N, em que N é um número entre 0 e 255.\n"								// Portuguese (Brazil)
										}
	},
	{ EISTR_ILLEGAL_SOCKET			,	{	L"The socket entered cannot be used.  Please enter a socket number between 0 and 65536.\n",								// English
											L"Le socket saisi ne peut pas être utilisé.  Entrer un numéro de socket compris entre 0 et 65536.\n",					// French
											L"Der eingegebene Anschluss (Socket) ist nicht nutzbar. Bitte eine Anschlussnummer zwischen 0 und 65536 eingeben.\n",	// German
											L"El socket introducido no puede utilizarse.  Por favor, introduzca un número de enchufe entre 0 y 65536.\n",			// Spanish
											L"Impossibile utilizzare il socket specificato.  Immettere un numero di socket compreso fra 0 e 65536.\n",				// Italian
											L"A tomada introduzida não pode ser utilizada.  Introduza um número de tomada entre 0 e 65 536.\n",						// Portuguese
											L"O soquete inserido não pode ser usado.  Insira um número de soquete entre 0 e 65.536.\n"								// Portuguese (Brazil)
										}
	},
	{ EISTR_INSUFFICIENT_DILUENT	,	{	L"Insufficient reagents exist in the disc to complete the run. Rerun sample with new disc.\n",														// English
											L"La quantité de réactifs dans le disque est insuffisante pour exécuter la procédure. Réexécuter l'échantillon à l'aide du nouveau disque.\n",		// French
											L"In der Scheibe befinden sich nicht ausreichend Reagenzien, um den Vorgang abzuschließen. Wiederholen Sie die Probe mit einer neuen Scheibe.\n",	// German
											L"No hay suficientes reactivos en el disco para completar su uso. Vuelva a utilizar la muestra con un nuevo disco.\n",								// Spanish
											L"Nel disco non vi sono sufficienti reagenti per completare l'operazione. Riavviare il campione con un nuovo disco.\n",								// Italian
											L"Não existem reagentes suficientes no disco para concluir a execução. Executar novamente a amostra utilizando um disco novo.\n",					// Portuguese
											L"Existem reagentes insuficientes no disco para concluir a execução. Execute novamente a amostra com o novo disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_SAMPLE		,	{	L"The disc has not received enough sample to function properly. Rerun using a new disc.\n",										// English
											L"Le disque n'a pas reçu suffisamment d'échantillons pour fonctionner correctement. Réexécuter avec un nouveau disque.\n",		// French
											L"Die Scheibe enthält keine ausreichende Probenmenge für eine ordnungsgemäße Funktion. Mit einer neuen Scheibe wiederholen.\n",	// German
											L"El disco no ha recibido suficiente muestra para funcionar correctamente. Úselo con un nuevo disco.\n",							// Spanish
											L"Il disco non ha ricevuto campione sufficiente per funzionare nel modo corretto. Riavviare utilizzando un nuovo disco.\n",		// Italian
											L"O disco não recebeu amostra suficiente para funcionar adequadamente. Executar novamente utilizando um disco novo.\n",			// Portuguese
											L"O disco não recebeu amostra suficiente para funcionar corretamente. Execute novamente usando um novo disco.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_STORAGE	,	{	L"There is insufficient free space on the USB storage device to complete the service archive. Please remove the USB storage device, and connect a USB storage device with at least 256 MB of available space.\n",										// English
											L"Espace de stockage sur périphérique USB insuffisant pour réaliser l'archive de service. Retirer le périphérique de stockage USB et brancher un périphérique de stockage USB ayant au moins 25 Mo d'espace disponible.\n",								// French
											L"Es ist nicht genügend Speicherkapazität auf dem USB-Speichergerät frei, um das Service-Archiv zu vollenden. Bitte entfernen Sie das USB-Speichergerät und schließen Sie ein USB-Speichergerät mit mindestens 256 MB freiem Speicher an.\n",			// German
											L"El dispositivo de almacenamiento USB no tiene espacio suficiente para finalizar el archivo de servicio. Extraiga el dispositivo y conecte otro con al menos 256 MB de espacio disponible.\n",															// Spanish
											L"Spazio libero sul dispositivo di archiviazione USB insufficiente per completare l'archivio di servizio. Pregasi rimuovere il dispositivo di archiviazione USB e collegare un dispositivo di archiviazione con almeno 256 MB di spazio disponibile.\n",	// Italian
											L"O dispositivo de armazenamento USB não tem espaço suficiente para finalizar o ficheiro de serviço. Remova o dispositivo de armazenamento USB e insira outro com pelo menos 256 MB de espaço disponível.\n",											// Portuguese
											L"Não há espaço livre suficiente no dispositivo de armazenamento USB para concluir o arquivo de serviço. Remova o dispositivo de armazenamento USB e conecte um dispositivo de armazenamento USB com pelo menos 256 MB de espaço disponível.\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_INTERNAL_COMM			,	{	L"The analyzer lost internal communication. If symptom continues, contact Technical Service.\n",												// English
											L"L'analyseur a perdu sa capacité de communication interne. En cas de persistance des symptômes, contacter le service technique.\n",			// French
											L"Das Analysegerät hat die interne Kommunikation verloren. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",		// German
											L"El analizador perdió la comunicación interna. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"L'analizzatore ha perso la comunicazione interna. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"O analisador perdeu a comunicação interna. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",							// Portuguese
											L"O analisador perdeu a comunicação interna. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_INTERNAL_SOFTWARE		,	{	L"The analyzer detected an error with the software.  If symptom continues, contact Technical Service.\n",									// English
											L"L'analyseur a détecté une erreur logiciel. En cas de persistance des symptômes, contacter le service technique.\n",						// French
											L"Das Analysegerät hat einen Softwarefehler festgestellt. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",		// German
											L"El analizador detectó un error con el software. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"L'analizzatore ha individuato un errore nel software. Se i sintomi continuano contattare il servizio tecnico.\n",							// Italian
											L"O analisador detetou um erro no software.  Se o problema persistir, contacte o serviço de Assistência Técnica.\n",							// Portuguese
											L"O analisador detectou um erro no software.  Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_IQC						,	{	L"The analyzer Intelligent Quality Control (iQC) reagent dye beads differ too much from the expected values. Rerun using a new disc.\n",													// English
											L"Les billes de teinture du réactif Intelligent Quality Control (iQC) divergent trop des valeurs prévues. Réexécuter avec un nouveau disque.\n",											// French
											L"Die Intelligent Quality Control (iQC) Reagenz-Farbstoff-Beads des Analysegeräts weichen zu stark von den erwarteten Werten ab. Mit einer neuen Scheibe wiederholen.\n",				// German
											L"Las cuentas de tinte de reactivo Intelligent Quality Control (iQC) del analizador son muy distintas a los valores esperados. Úselo con un nuevo disco.\n",								// Spanish
											L"I granuli di colore reagente Intelligent Quality Control (iQC) dell'analizzatore sono troppo diversi dai valori previsti. Riavviare utilizzando un nuovo disco.\n",					// Italian
											L"As esferas de reagente corante do Controlo de qualidade inteligente (iQC) do analisador são muito diferentes dos valores esperados. Executar novamente utilizando um disco novo.\n",	// Portuguese
											L"As esferas do corante reagente de Controle de qualidade inteligente (iQC) do analisador diferem muito dos valores esperados. Execute novamente usando um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_LAMP					,	{	L"The lamp is checked each time to make sure it is still flashing when the optical cuvette is sampled. Detectors did not recognize the lamp flashes. If symptom continues, contact Technical Service.\n",																					// English
											L"La lampe fait à chaque fois l'objet d'une vérification afin de s'assurer qu'elle fonctionne toujours lors du prélèvement sur la cuvette optique. Les détecteurs n'ont pas reconnu les clignotements de la lampe. En cas de persistance des symptômes, contacter le service technique.\n",	// French
											L"Die Lampe wird jedes Mal auf Blinkzeichen überprüft, wenn eine Probe der optischen Cuvette untersucht wird. Es wurden keine Blinkzeichen der Lampen festgestellt. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",												// German
											L"Siempre se comprueba la lámpara para asegurar que sigue emitiendo flashes cuando se pone la muestra en la cubeta óptica. Los detectores no registraron la emisión de flashes de la lámpara. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",		// Spanish
											L"La lampada viene controllata ogni volta per assicurarsi che stia ancora lampeggiando quando viene campionata la cuvette ottica. I rilevatori non hanno riconosciuto i flash luminosi. Se i sintomi continuano contattare il servizio tecnico.\n",											// Italian
											L"A lâmpada é sempre verificada para garantir que emite flashes quando a amostra é colocada na cuvete ótica. Os detetores não registram a emissão de flashes da lâmpada. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",												// Portuguese
											L"A lâmpada é verificada a cada vez, para certificar-se de que ainda esteja piscando quando a cubeta óptica for amostrada. Os detectores não reconheceram a luz da lâmpada. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"												// Portuguese (Brazil)
										} 
	},
	{ EISTR_LIGHT_NOISE				,	{	L"There was high variability in the signal. This may impact the accuracy of the analyzer's measurement of absorbance. If symptom continues, contact Technical Service.\n",											// English
											L"Le signal d'arrière-plan a présenté une variabilité élevée. ce qui peut affecter la précision de la mesure d'absorbance par l'analyseur. En cas de persistance des symptômes, contacter le service technique.\n",	// French
											L"Es bestand eine hohe Signalvarianz. Dies kann die Genauigkeit der Absorbierungsmessung des Analysegeräts beeinträchtigen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",				// German
											L"Había una gran variabilidad en la señal. Esto puede afectar a la precisión de la medida de absorbancia del analizador. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"Grande variabilità nel segnale. Esso potrebbe influire sulla precisione della misurazione di assorbenza dell'analizzatore. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"Foi observada uma grande variabilidade no sinal. Isto pode afetar a precisão da medição da absorvância do analisador. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",						// Portuguese
											L"Houve alta variabilidade no sinal. Isso pode afetar a precisão da medição de absorbância do analisador. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"											// Portuguese (Brazil)
										} 
	},
	{ EISTR_MISSING_FILTER_CURV		,	{	L"The filter curve calibration factors have not been loaded into the analyzer. Contact Technical Service for assistance.\n",									// English
											L"Les facteurs de calibrage de la courbe du filtre n'ont pas été chargés sur l'analyseur. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Die Kalibrierungsfaktoren der Filterkurve wurden nicht in das Analysegerät geladen. Wenden Sie sich an den technischen Service.\n",						// German
											L"Los factores de calibración de la curva del filtro no se han cargado en el analizador. Póngase en contacto con el servicio técnico.\n",					// Spanish
											L"I fattori di calibrazione della curva del filtro non sono stati caricati nell'analizzatore. Per assistenza contattare il servizio tecnico.\n",				// Italian
											L"Os fatores da curva de calibração do filtro não foram carregados no analisador. Contacte o Serviço de Assistência Técnica para obter assistência.\n",		// Portuguese
											L"Os fatores de calibração da curva do filtro não foram carregados no analisador. Entre em contato com o Serviço Técnico para obter assistência.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_MOTOR_CONTROLLER		,	{	L"The analyzer detected an error with the motor controller. If symptom continues, contact Technical Service.\n",												// English
											L"L'analyseur a détecté une erreur sur le contrôleur du moteur. En cas de persistance des symptômes, contacter le service technique.\n",						// French
											L"Das Analysegerät hat einen Fehler an der Motorsteuerung festgestellt. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",			// German
											L"El analizador detectó un error con el controlador del motor. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"L'analizzatore ha individuato un errore con il controllore a motore. Se i sintomi continuano contattare il servizio tecnico.\n",							// Italian
											L"O analisador detetou um erro no controlador do motor. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",								// Portuguese
											L"O analisador detectou um erro no controlador do motor. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_NEEDS_SERVICE			,	{	L"The analyzer may need servicing. Contact Technical Service for further details.\n",													// English
											L"Il se peut que l'analyseur nécessite une intervention technique. Pour obtenir plus d'informations, contacter le service technique.\n",	// French
											L"Das Analysegerät muß möglicherweise gewartet werden. Wenden Sie sich an den technischen Service.\n",									// German
											L"Puede que el analizador deba ser reparado. Póngase en contacto con el servicio técnico.\n",											// Spanish
											L"L'analizzatore potrebbe aver bisogno di manutenzione. Per ulteriore dettagli contattare il servizio tecnico.\n",						// Italian
											L"O analisador pode necessitar de assistência. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",				// Portuguese
											L"O analisador pode precisar de manutenção. Entre em contato com o Serviço Técnico para obter detalhes adicionais.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_NO_FILES_TO_ARCHIVE		,	{	L"There are no files within the date range specified to archive.\n",					// English
											L"Aucun fichier à archiver dans la plage de dates spécifiée.\n",						// French
											L"Für den spezifizierten Datumsbereich sind keine Dateien zu archivieren.\n",		// German
											L"No hay ningún archivo en el intervalo de fechas especificado para archivar.\n",	// Spanish
											L"Non sono presenti file da archiviare nel range di date specificato.\n",			// Italian
											L"Não existem ficheiros no intervalo de datas especificado para arquivar. \n",		// Portuguese
											L"Não há arquivos dentro do intervalo de datas especificado para arquivar. \n"		// Portuguese (Brazil)
										}	
	},

	{ EISTR_NO_FILES_TO_TRANSMIT	,	{	L"There are no files within the date range specified to transmit.\n",				// English
											L"Aucun fichier à transmettre dans la plage de dates spécifiée.\n",					// French
											L"Für den spezifizierten Datumsbereich sind keine Dateien zu übertragen.\n",			// German
											L"No hay ningún archivo en el intervalo de fechas especificado para transmitir.\n",	// Spanish
											L"Non sono presenti file da trasmettere nel range di date specificato.\n",			// Italian
											L"Não existem ficheiros no intervalo de datas especificado para transmitir.\n",		// Portuguese
											L"Não há arquivos dentro do intervalo de datas especificado para transmitir.\n"		// Portuguese (Brazil)
										}	
	},

	{ EISTR_NO_RESULTS_TO_TRANSMIT	,	{	L"There are no results within the date range specified to transmit.\n",				// English
											L"Aucun résultat à transmettre dans la plage de dates spécifiée.\n",					// French
											L"Für den spezifizierten Datumsbereich sind keine Ergebnisse zu übertragen.\n",		// German
											L"No hay ningún resultado en el intervalo de fechas especificado para transmitir.\n",// Spanish
											L"Non sono presenti risultati da trasmettere nel range di date specificato.\n",		// Italian
											L"Não existem resultados no intervalo de datas especificado para transmitir.\n",		// Portuguese
											L"Não há resultados dentro do intervalo de datas especificado para transmitir.\n"	// Portuguese (Brazil)
										}	
	},

	{ EISTR_ORDER_WAS_CANCELED		,	{	L"The selected order was canceled by the external order entry system.\n",						// English
											L"La commande sélectionnée a été annulée par le système d'entrée de commande externe.\n",		// French
											L"Die ausgewählte Bestellung wurde vom externen Bestelleingabesystem storniert.\n",				// German
											L"El pedido seleccionado ha sido cancelado por el sistema externo de entrada de pedidos.\n",		// Spanish
											L"Ordine selezionato cancellato dal sistema di immissione ordini esterno.\n",					// Italian
											L"O pedido selecionado foi cancelado pelo sistema de entrada de pedidos externo.\n",				// Portuguese
											L"A solicitação selecionada foi cancelada pelo sistema externo de entrada de solicitações.\n"	// Portuguese (Brazil)
										}
	},

	{ EISTR_OP_ID_LIST_FULL			,	{	L"All the available space for Operator ID's is used.  Please remove at least one Operator ID before adding a new Operator ID.\n",													// English
											L"Tout l'espace disponible réservé aux ID opérateur est utilisé. Veuillez supprimer au moins un ID opérateur avant d'en ajouter un nouveau.\n",										// French
											L"Der gesamte Speicherplatz für Bediener-IDs ist belegt.  Bitte entfernen Sie mindestens eine Bediener-ID, bevor Sie eine neue hinzufügen.\n",										// German
											L"Se ha utilizado todo el espacio disponible para números de identificación de operarios. Elimine al menos un número de identificación de operario antes de añadir otro nuevo.\n",	// Spanish
											L"È stato utilizzato tutto lo spazio disponibile per l'ID operatore. Rimuovere almeno un ID operatore prima di aggiungere un nuovo ID operatore.\n",									// Italian
											L"Foi utilizado todo o espaço disponível para IDs de operadores.  Elimine pelo menos uma ID de operador antes de adicionar uma nova ID de operador.\n",								// Portuguese
											L"Todo o espaço disponível para os IDs de operador está usado.  Remova pelo menos um ID de operador antes de adicionar um novo ID de operador.\n"									// Portuguese (Brazil)
										}
	},

	{ EISTR_OP_RENEWAL_REQUIRED		,	{	L"The date set authorizing the operator to use the analyzer has passed.  Please contact technical support for more information.\n",										// English
											L"La date prévue pour autorisation de l'opérateur à utiliser l'analyseur est dépassée. Pour plus d'informations, contacter l'assistance technique.\n",					// French
											L"Das angegebene Datum, bis zu dem der Bediener das Analysegerät benutzen kann, ist abgelaufen. Wenden Sie sich an den technischen Service für weitere Information.\n",	// German
											L"La fecha para autorizar al operario a usar el analizador ya ha pasado. Póngase en contacto con el servicio técnico.\n",												// Spanish
											L"La data impostata per l'analisi dei campioni di controllo è stata superata. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",						// Italian
											L"A data definida para autorizar o operador a utilizar o analisador foi excedida.  Para obter mais informações, contacte o serviço de Assistência Técnica.\n",			// Portuguese
											L"A data configurada, autorizando o operador a usar o analisador, já passou.  Entre em contato com o suporte técnico para obter mais informações.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_PATIENT_IDS_DONT_MATCH	,	{	L"The patient ID entered does not match the ID transmitted to the analyzer.\n",							// English
											L"L'ID patient entrée ne correspond pas à l'ID transmise à l'analyseur.\n",								// French
											L"Die eingegebene Patienten-ID stimmt nicht mit der an das Analysegerät übermittelten ID überein.\n",	// German
											L"El Id. del paciente introducido no coincide con el Id. transmitido al analizador.\n",					// Spanish
											L"L'ID paziente inserito non corrisponde a quello trasmesso all'analizzatore.\n",						// Italian
											L"A ID de doente introduzida não coincide com a ID transmitida ao analisador.\n",						// Portuguese
											L"O ID do paciente inserido não corresponde ao ID transmitido para o analisador.\n"						// Portuguese (Brazil)
										}
	},
	{ EISTR_POWER_SUPPLY			,	{	L"The analyzer has detected power fluctuations that prevent the unit from operating correctly. If symptom continues, contact Technical Service.\n",																// English
											L"L'analyseur a détecté des fluctuations dans l'alimentation qui empêchent l'appareil de fonctionner correctement. En cas de persistance des symptômes, contacter le service technique.\n",						// French
											L"Das Analysegerät hat Energieschwankungen festgestellt, die den ordnungsgemäßen Betrieb der Einheit beeinträchtigen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",				// German
											L"El analizador ha detectado fluctuaciones de la corriente que no permiten el funcionamiento correcto de la unidad. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"L'analizzatore ha individuato fluttuazioni di corrente che non permettono di funzionare correttamente. Se i sintomi continuano contattare il servizio tecnico.\n",												// Italian
											L"O analisador detetou flutuações de corrente que não permitem o correto funcionamento da unidade. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",										// Portuguese
											L"O analisador detectou flutuações de energia que impedem a unidade de funcionar corretamente. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"												// Portuguese (Brazil)
										} 
	},
	{ EISTR_PRINTER_NOT_FOUND		,	{	L"The external printer is not available. Check that the printer is connected to the analyzer, is turned on and has paper. If symptom continues, contact Technical Service.\n",																	// English
											L"L'imprimante externe n'est pas disponible. Vérifier que l'imprimante est bien raccordée à l'analyseur, qu'elle fonctionne et qu'elle dispose d'une réserve de papier. En cas de persistance des symptômes, contacter le service technique.\n",	// French
											L"Der externe Drucker ist nicht verfügbar. Prüfen Sie die Verbindung zwischen Drucker und Analysegerät und ob der Drucker eingeschaltet ist und über Papier verfügt. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",	// German
											L"La impresora externa no está disponible. Compruebe que la impresora está conectada al analizador, que está encendida y que tiene papel. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.n",				// Spanish
											L"La stampante esterna non è disponibile. Verificare che la stampante sia collegata all'analizzatore, che sia accesa e che non manchi carta. Se i sintomi continuano contattare il servizio tecnico.\n",											// Italian
											L"A impressora externa não está disponível. Verifique que se a impressora está ligada ao analisador, que está ligada e que tem papel. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",									// Portuguese
											L"A impressora externa não está disponível. Verifique se a impressora está conectada ao analisador, se está ligada e se tem papel. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"											// Portuguese (Brazil)
										} 
	},
	{ EISTR_PROCESS_INIT			,	{	L"The analyzer is unable to start a process required to operate. Contact Technical Service for assistance.\n",									// English
											L"L'analyseur ne parvient pas à lancer la procédure requise pour fonctionner. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Das Analysegerät kann einen für den Betrieb erforderlichen Vorgang nicht starten. Wenden Sie sich an den technischen Service.\n",				// German
											L"El analizador no puede iniciar un proceso necesario para funcionar. Póngase en contacto con el servicio técnico.\n",							// Spanish
											L"L'analizzatore non è in grado di iniziare una procedura necessaria al funzionamento. Per assistenza contattare il servizio tecnico.\n",		// Italian
											L"O analisador não consegue iniciar um processo necessário para funcionar. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											L"O analisador não é capaz de iniciar um processo necessário para operar. Entre em contato com o Serviço Técnico para obter assistência.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_PRODUCT_CODE			,	{	L"The product codes in the analyzer do not match. Contact Technical Service for more details.\n",									// English
											L"Les codes produits dans l'analyseur ne correspondent pas. Pour plus d'informations, contacter le service technique.\n",			// French
											L"Die Produktcodes in dem Analysegerät passen nicht zusammen. Wenden Sie sich an den technischen Service.\n",						// German
											L"Los códigos de producto en el analizador no se corresponden. Póngase en contacto con el servicio técnico para más información.\n",	// Spanish
											L"I codici prodotto nell'analizzatore non combaciano. Per ulteriore dettagli contattare il servizio tecnico.\n",						// Italian
											L"Os códigos de produto no analisador não coincidem. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",		// Portuguese
											L"Os códigos de produto no analisador não correspondem. Entre em contato com o Serviço Técnico para obter mais detalhes.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_REAGENT_DISTRIBUTION	,	{	L"Reagent beads have not dissolved as expected. Rerun sample with a new disc.\n",											// English
											L"Les billes de réactif ne se sont pas dissoutes comme prévu. Réexécuter l'échantillon à l'aide d'un nouveau disque.\n",		// French
											L"Reagenz-Beads wurden nicht erwartungsgemäß aufgelöst. Wiederholen Sie die Probe mit einer neuen Scheibe.\n",				// German
											L"Las cuentas de reactivo no se han disuelto como se esperaba. Vuelva a utilizar la muestra con un nuevo disco.\n",			// Spanish
											L"I granuli di reagente non si sono dissolti come previsto. Riavviare il campione con un nuovo disco.\n",					// Italian
											L"As esferas de reagente não se dissolveram conforme esperado. Executar novamente a amostra utilizando um disco novo.\n",	// Portuguese
											L"As esferas de reagente não se dissolveram conforme esperado. Execute novamente a amostra com um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_RESULT_CORRUPTION		,	{	L"The saved results are corrupt.  All the previous results have been lost.  A new set of saved results has been started.  If symptom continues, contact Technical Service.\n"																// English
											L"Les résultats enregistrés sont corrompus.  Tous les résultats antérieurs ont été perdus.  Un nouvel ensemble de résultats enregistrés a été initié.  Si les symptômes persistent, contacter le service technique.\n",						// French
											L"Die gespeicherten Ergebnisse sind fehlerhaft. Alle bisherigen Ergebnisse sind verloren gegangen. Es wurde ein neuer Satz gespeicherter Ergebnisse gestartet. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",	// German
											L"Los resultados guardados están dañados.  Se han perdido todos los resultados anteriores.  Se ha iniciado una nueva serie de resultados guardados.  Si los síntomas continúan, comuníquese con el servicio técnico.\n",						// Spanish
											L"I risultati salvati sono danneggiati.  Tutti i risultati precedenti sono andati perduti.  È stato iniziato un nuovo insieme di risultati.  Se il sintomo persiste, contattare il servizio tecnico.\n",										// Italian
											L"Os resultados guardados estão danificados.  Todos os resultados anteriores foram perdidos.  Foi iniciado um novo conjunto de resultados guardados.  Se o problema persistir, contacte o serviço de Assistência Técnica.\n",				// Portuguese
											L"Os resultados salvos estão corrompidos.  Todos os resultados anteriores foram perdidos.  Um novo conjunto de resultados salvos foi iniciado.  Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"							// Portuguese (Brazil)
										}
	},
	{ EISTR_ROTOR_DETECTED			,	{	L"A disc may be in the analyzer and may prevent the analyzer from starting normally. Press Continue to remove the disc.\n",										// English
											L"Il peut y avoir dans l'analyseur un disque susceptible de l'empêcher de démarrer normalement. Appuyer sur Continuer pour retirer le disque.\n",				// French
											L"Eine Scheibe kann sich in dem Analysegerät befinden und den Startvorgang des Analysegeräts beeinträchtigen. Zur Entfernung der Scheibe drücken Sie Weiter.\n",	// German
											L"Puede que haya un disco en el analizador, lo que impide que el analizador se inicie correctamente. Pulse en Continuar para retirar el disco.\n",				// Spanish
											L"Nell'analizzatore potrebbe essere rimasto un disco che impedisce un'attivazione normale dell'analizzatore. Premere Continua per rimuovere il disco.\n",		// Italian
											L"Pode estar um disco no analisador, o que pode impedir que o analisador inicie corretamente. Prima Continuar para remover o disco.\n",							// Portuguese
											L"Um disco pode estar no analisador e impedir que o analisador inicie normalmente. Pressione Continuar para remover o disco.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_RQC						,	{	L"The Reagent QC beads recovered below the limit. The disc may not have been stored correctly. Rerun with a new disc.\n",												// English
											L"La récupération des billes de réactif QC est inférieure à la limite. Il se peut que le disque n'ait pas été rangé correctement. Réexécuter avec un nouveau disque.\n",	// French
											L"Die Reagenz-QC-Beads liegen unter dem Grenzwert. Die Scheibe wurde möglicherweise nicht ordnungsgemäß gelagert. Mit einer neuen Scheibe wiederholen.\n",				// German
											L"Cuentas QC de reactivo recuperadas por debajo del límite. Puede que el disco no se haya guardado correctamente. Debe usarse con un nuevo disco.\n",					// Spanish
											L"I granuli QC di reagente si sono ristabiliti sotto al limite. Il disco potrebbe non essere stato conservato correttamente. Riavviare con un nuovo disco.\n",			// Italian
											L"As esferas de reagente de CQ foram recuperadas abaixo do limite. O disco pode não ter sido guardado corretamente. Executar novamente utilizando um disco novo.\n",		// Portuguese
											L"As esferas de Reagente de QC se recuperaram abaixo do limite. O disco pode não ter sido armazenado corretamente. Execute novamente com um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_BLANK			,	{	L"The sample blank bead appears to be turbid. This can be due to sample quality. Confirm sample integrity and rerun with a new disc.\n",															// English
											L"La bille vierge de l'échantillon semble trouble, ce qui peut venir de la qualité de l'échantillon. Vérifier l'intégrité de l'échantillon et réexécuter avec un nouveau disque.\n",				// French
											L"Die Leer-Beadprobe ist trübe. Dies kann auf die Probenqualität zurückgeführt werden. Bestätigen Sie die Unversehrtheit der Probe und wiederholen Sie den Vorgang mit einer neuen Scheibe.\n",	// German
											L"La cuenta virgen de la muestra parece estar turbia. Esto puede deberse a la calidad de la muestra. Confirme la integridad de la muestra y vuelva a utilizar el nuevo disco.\n",				// Spanish
											L"I granuli campione vuoti sembrano torbidi. Ciò può essere causato dalla qualità del campione. Confermare l'integrità del campione e riavviare con un nuovo disco.\n",							// Italian
											L"A esfera de amostra de branco parece estar turva. Isto pode ser devido à qualidade da amostra. Confirme a integridade da amostra e execute novamente utilizando um disco novo.\n",				// Portuguese
											L"A esfera em branco da amostra parece estar turva. Isso pode ser decorrente da qualidade da amostra. Confirme a integridade da amostra e execute novamente com um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_DISTRIBUTION		,	{	L"Sample has not reached the chemistries. Rerun with a new disc.\n",										// English
											L"L'échantillon n'est pas parvenu jusqu'aux produits chimiques. Réexécuter avec un nouveau disque.\n",	// French
											L"Die Probe hat nicht die Chemie erreicht. Mit einer neuen Scheibe wiederholen.\n",						// German
											L"La muestra no ha alcanzado los compuestos químicos. Debe usarse con un nuevo disco.\n",				// Spanish
											L"Il campione non ha raggiunto le sostanze chimiche. Riavviare con un nuovo disco.\n",					// Italian
											L"A amostra não atingiu as substâncias químicas. Executar novamente utilizando um disco novo.\n",		// Portuguese
											L"A amostra não atingiu as substâncias químicas. Execute novamente com um novo disco.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_MIX				,	{	L"The sample has not mixed with reagent materials sufficiently. Rerun with a new disc.\n",									// English
											L"L'échantillon ne s'est pas suffisamment mélangé avec les matières de réactifs. Réexécuter avec un nouveau disque.\n",		// French
											L"Die Probe hat sich nicht ausreichend mit Reagenzmaterialien vermischt. Mit einer neuen Scheibe wiederholen.\n",			// German
											L"La muestra no se ha mezclado suficientemente con los materiales reactivos. Debe usarse con un nuevo disco.\n",				// Spanish
											L"Il campione non si è mescolato a sufficienza con le sostanze reagenti. Riavviare con un nuovo disco.\n",					// Italian
											L"A amostra não se misturou suficientemente com os materiais dos reagentes. Executar novamente utilizando um disco novo.\n",	// Portuguese
											L"A amostra não foi misturada com os materiais reagentes suficientemente. Execute novamente com um novo disco.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_QUALITY			,	{	L"The sample may have one or more physical interferents. Confirm the sample integrity and rerun disc.\n",											// English
											L"Il se peut que l'échantillon présente un ou plusieurs éléments interférents. Vérifier l'intégrité de l'échantillon et réexécuter le disque.\n",	// French
											L"Die Probe hat möglicherweise eine oder mehrere physische störende Bestandteile. Unversehrtheit der Probe bestätigen und Scheibe neu starten.\n",	// German
											L"Puede que la muestra tenga una o varias interferencias físicas. Confirme la integridad de la muestra y vuelva a usar el disco.\n",					// Spanish
											L"Il campione potrebbe avere uno o più fattori di interferenza fisica. Confermare l'integrità del campione e riavviare il disco.\n",					// Italian
											L"A amostra pode ter uma ou várias interferências físicas. Confirme a integridade da amostra e execute novamente o disco.\n",						// Portuguese
											L"A amostra pode ter um ou mais interferentes físicos. Confirme a integridade da amostra e execute novamente o disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_SATURATION				,	{	L"The photometer in the analyzer has detected a brighter than normal flash. If symptom continues, contact Technical Service.\n",												// English
											L"Le photomètre de l'analyseur a détecté un clignotement plus lumineux que la normale. En cas de persistance des symptômes, contacter le service technique.\n",				// French
											L"Das Fotometer in dem Analysegerät hat ein helleres Blinkzeichen als normal festgestellt. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",		// German
											L"El fotómetro del analizador ha detectado un flash más brillante de lo normal. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"Il fotometro nell'analizzatore ha individuato un flash più luminoso del normale. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"O fotómetro no analisador detetou um flash mais brilhante do que o normal. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",							// Portuguese
											L"O fotômetro no analisador detectou um flash mais brilhante que o normal. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_SOFTWARE_EXPIRED		, {		L"This Analyzer is for Investigational Use Only.  Please return the Analyzer to Abaxis Research and Development for renewal.\n",															// English
											L"Cet analyseur est utilisé à des fins d'investigation uniquement. Retourner l'analyseur à Abaxis Research and Development pour le faire remplacer.\n",									// French
											L"Dieses Analysegerät ist ausschließlich für die Verwendung als Untersuchungsgerät bestimmt. Geben Sie das Analysegerät zur Erneuerung an Abaxis Research and Development zurück.\n",	// German									// German
											L"Este analizador es solo para su uso en investigación. Devuelva el analizador al departamento de investigación y desarrollo de Abaxis para su renovación.\n",							// Spanish
											L"Questo Analizzatore è destinato esclusivamente a scopo di ricerca.  Restituire l'Analizzatore a Abaxis Ricerca e Sviluppo per il rinnovo.\n",											// Italian
											L"Este analisador destina-se apenas a ser utilizado para investigação.  Devolva o analisador ao departamento de investigação e desenvolvimento da Abaxis para renovação.\n",				// Portuguese
											L"Este analisador destina-se apenas ao uso em investigação.  Devolva o analisador para a Abaxis Research and Development, para renovação.\n"												// Portuguese (Brazil)
                                      }
    },
	{ EISTR_SOFTWARE_UPDATE			,	{	L"The disc inserted requires a software update to operate correctly. Contact Technical Service for further assistance.\n",														// English
											L"Le disque inséré nécessite une mise à jour de logiciel pour pouvoir fonctionner correctement. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											L"Die eingesetzte Scheibe erfordert ein Software-Update für einen ordnungsgemäßen Betrieb. Wenden Sie sich an den technischen Service.\n",										// German
											L"El disco insertado necesita la actualización del software para operar correctamente. Póngase en contacto con el servicio técnico.\n",											// Spanish
											L"Il disco inserito richiede un aggiornamento del software per funzionare correttamente. Per ulteriore assistenza contattare il servizio tecnico.\n",							// Italian
											L"O disco inserido necessita de uma atualização do software para funcionar adequadamente. Contacte o Serviço de Assistência Técnica para obter assistência.\n",					// Portuguese
											L"O disco inserido requer uma atualização de software para funcionar corretamente. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SPINDLE_MOTOR_STUCK		,	{	L"Spindle motor not turning properly. Contact Technical Service for further assistance.\n",											// English
											L"Le moteur de l'axe ne tourne pas correctement. Pour obtenir une assistance complémentaire, contacter le service technique.\n",		// French
											L"Der Spindelmotor dreht sich nicht ordnungsgemäß. Wenden Sie sich an den technischen Service.\n",									// German
											L"El motor del eje no está girando correctamente. Póngase en contacto con el servicio técnico.\n",									// Spanish
											L"L'asse motore non gira correttamente. Per ulteriore assistenza contattare il servizio tecnico\n",									// Italian
											L"O motor de eixo não está a rodar adequadamente. Contacte o Serviço de Assistência Técnica para obter assistência.\n",				// Portuguese
											L"O motor do fuso não está girando corretamente. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_STARTING_ABSORBANCE		,	{	L"Starting absorbance of the sample blank bead is outside the limit, possibly due to an exogenous substance or distribution error. Rerun with new disc.\n",																		// English
											L"L'absorbance de départ de la bille vierge de l'échantillon n'entre pas dans les limites prescrites, éventuellement du fait d'une substance exogène ou d'une erreur de distribution. Réexécuter à l'aide du nouveau disque.\n",	// French
											L"Die Startabsorbierung der Leer-Beadprobe befindet sich möglicherweise aufgrund einer exogenen Substanz oder eines Verteilungsfehlers außerhalb des Grenzwerts. Mit einer neuen Scheibe wiederholen.\n",						// German
											L"La absorbancia de inicio de la cuenta virgen de muestra está fuera del límite, probablemente debido a una sustancia exógena o a un error de distribución. Utilice un nuevo disco.\n",											// Spanish
											L"L'assorbenza iniziale dei granuli campione vuoti non rientra nei limiti, forse a causa di una sostanza esogena o a un errore di distribuzione. Riavviare con un nuovo disco.\n",												// Italian
											L"A absorvância inicial da esfera de amostra de branco está fora do limite, possivelmente devido a uma sustância exógena ou erro de distribuição. Executar novamente utilizando um disco novo.\n",								// Portuguese
											L"A absorção inicial da esfera em branco da amostra está fora do limite, possivelmente devido a uma substância exógena ou um erro de distribuição. Execute novamente com um novo disco.\n"										// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_SENSOR				,	{	L"The disc temperature sensor is not operating. Contact Technical Service for further assistance.\n",										// English
											L"Le détecteur de température du disque ne fonctionne pas. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											L"Der Temperatursensor der Scheibe ist außer Betrieb. Wenden Sie sich an den technischen Service.\n",										// German
											L"El sensor de temperatura del disco no está funcionando. Póngase en contacto con el servicio técnico.\n",									// Spanish
											L"Il sensore di temperatura del disco non è in funzione. Per ulteriore assistenza contattare il servizio tecnico.\n",						// Italian
											L"O sensor de temperatura do disco não está a funcionar. Contacte o Serviço de Assistência Técnica para obter assistência.\n",				// Portuguese
											L"O sensor de temperatura do disco não está funcionando. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_VARIATION			,	{	L"The temperature deviated more than one degree during the run. The accuracy of certain chemistries may be affected. Rerun with new disc.\n",													// English
											L"La température a présenté un écart de plus d'un degré au cours de l'exécution. La précision de certains produits chimiques pourra en être affectée. Réexécuter à l'aide du nouveau disque.\n",	// French
											L"Der Temperatur wich um mehr als ein Grad während des Betriebs ab. Die Genauigkeit bestimmter Chemie kann beeinträchtigt worden sein. Mit einer neuen Scheibe wiederholen.\n",					// German
											L"La temperatura cambió en más de un grado durante el análisis. Puede que afecte a la precisión de análisis de ciertas sustancias químicas. Utilice un nuevo disco.\n",							// Spanish
											L"Durante il funzionamento la temperatura è variata di più di un grado. La precisione di alcune sostanze chimiche potrebbe esserne stata influenzata. Riavviare con un nuovo disco.\n",			// Italian
											L"A alteração da temperatura foi superior a um grau durante a execução. A precisão de determinadas sustâncias químicas pode ser afetada. Executar novamente utilizando um disco novo.\n",		// Portuguese
											L"A temperatura desviou mais de um grau durante a execução. A precisão de certas substâncias químicas pode estar afetada. Execute novamente com um novo disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMPERATURE				,	{	L"The temperature of the disc either varied too much or was not in the proper range during the run. If symptom continues, contact Technical Service.\n",																			// English
											L"La température du disque a soit varié dans des proportions trop importantes, soit ne s'inscrit pas dans la fourchette prescrite en cours d'exécution. En cas de persistance des symptômes, contacter le service technique.\n",	// French
											L"Die Scheibentemperatur variierte zu stark oder befand sich während des Betriebs außerhalb des optimalen Bereichs. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",									// German
											L"La temperatura del disco varió mucho o no estaba en el intervalo adecuado durante el análisis. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",										// Spanish
											L"Durante il funzionamento la temperatura del disco ha subito variazioni eccessive oppure non era nel range adatto. Se i sintomi continuano contattare il servizio tecnico.\n",													// Italian
											L"A temperatura do disco variou muito ou não esteve dentro do intervalo adequado durante a análise. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",														// Portuguese
											L"A temperatura do disco variou muito ou não permaneceu no intervalo adequado durante a execução. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"																// Portuguese (Brazil)
										} 
	},
	{ EISTR_TIMEOUT					,	{	L"The analyzer is not receiving required data in a timely fashion. If symptom continues, contact Technical Service.\n",										// English
											L"L'analyseur ne reçoit pas les données voulues dans les délais. En cas de persistance des symptômes, contacter le service technique.\n",					// French
											L"Das Analysegerät empfängt die erforderlichen Daten nicht rechtzeitig. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",			// German
											L"El analizador no está recibiendo a tiempo los datos necesarios. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"L'analizzatore non riceve tempestivamente i dati necessari. Se i sintomi continuano contattare il servizio tecnico.\n",									// Italian
											L"O analisador não está a receber atempadamente os dados necessários. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",				// Portuguese
											L"O analisador não está recebendo os dados necessários em tempo hábil. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_TOO_HOT					,	{	L"The analyzer is getting too hot and should be allowed to cool down for a while before further use.\n",						// English
											L"L'analyseur présente une surchauffe. Il convient de le laisser refroidir un moment avant de continuer à l'utiliser.\n",	// French
											L"Das Analysegerät wird zu heiß und sollte vor der weiteren Verwendung abkühlen.\n",											// German
											L"El analizador se está sobrecalentando y se debería dejar enfriar antes de volver a usarlo.\n",								// Spanish
											L"L'analizzatore si sta riscaldando eccessivamente e deve essere fatto raffreddare prima di essere usato nuovamente.\n",		// Italian
											L"O analisador está aquecer muito e deve-se deixar arrefecer durante algum tempo antes de o voltar a utilizar.\n",			// Portuguese
											L"O analisador está se tornando quente demais e deve ser reservado para esfriar por um tempo antes de continuar o uso.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_TOP_HEATER_HIGH			,	{	L"The top heater plate draws too much current when turned on at full power. Contact Technical Service for assistance.\n",														// English
											L"La plaque chauffante supérieure consomme trop d'électricité lorsqu'elle fonctionne à fond. Pour obtenir une assistance, contacter le service technique.\n",					// French
											L"Die obere Heizplatte verbraucht bei Maximalbetrieb zu viel Strom. Wenden Sie sich an den technischen Service.\n",																// German
											L"La placa del calentador superior utiliza mucha corriente cuando se enciende a toda potencia. Póngase en contacto con el servicio técnico.\n",									// Spanish
											L"Il disco di riscaldamento superiore prende troppa corrente quando è acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",							// Italian
											L"A placa do aquecedor superior utiliza elevada quantidade de corrente quando é ligada na potência máxima. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											L"A placa superior do aquecedor consome muita corrente quando ligada na potência máxima. Entre em contato com o Serviço Técnico para obter assistência.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_TOP_HEATER_LOW			,	{	L"The top heater plate did not draw enough current when turned on at full power. Contact Technical Service for assistance.\n",															// English
											L"La plaque chauffante supérieure n'a pas consommé suffisamment d'électricité lorsqu'elle a fonctionné à fond. Pour obtenir une assistance, contacter le service technique.\n",			// French
											L"Die obere Heizplatte verbrauchte bei Maximalbetrieb zu wenig Strom. Wenden Sie sich an den technischen Service.\n",																	// German
											L"La placa del calentador superior no utilizó suficiente corriente cuando se puso a toda potencia. Póngase en contacto con el servicio técnico.\n",										// Spanish
											L"Il disco di riscaldamento superiore non ha preso sufficiente corrente quando acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",							// Italian
											L"A placa superior do aquecedor não utiliza quantidade suficiente de corrente quando ligada na potência máxima. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											L"A placa superior do aquecedor não atraiu corrente suficiente quando ligada na potência máxima. Entre em contato com o Serviço Técnico para obter assistência.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_UNAUTHORIZED_ADMIN		,	{	L"The ID entered for the administrator is not in the system. Please contact technical support for more information.\n",						// English
											L"L'ID saisi pour l'administrateur n'est pas sur le système. Pour plus d'informations, contacter l'assistance technique.\n",					// French
											L"Die eingegebene Administrator-ID existiert nicht im System. Wenden Sie sich bitte an den technischen Service für weitere Information.\n",	// German
											L"Esta Id. administrador no está en el sistema. Póngase en contacto con el servicio técnico. \n",											// Spanish
											L"L'ID immesso per l'amministratore non è presente nel sistema. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",		// Italian
											L"A ID introduzida para o administrador não está no sistema. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",		// Portuguese
											L"O ID inserido para o administrador não está no sistema. Entre em contato com o suporte técnico para obter mais informações.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_UNAUTHORIZED_OP			,	{	L"The ID entered for the operator is not in the system. Please contact technical support for more information.\n",						// English
											L"L'ID saisi pour l'opérateur n'est pas sur le système. Pour plus d'informations, contacter l'assistance technique.\n",					// French
											L"Die eingegebene Bediener-ID existiert nicht im System. Wenden Sie sich bitte an den technischen Service für weitere Information.\n",	// German
											L"Esta Id. operario no está en el sistema. Póngase en contacto con el servicio técnico.\n",												// Spanish
											L"L'ID immesso per l'operatore non è presente nel sistema. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",		// Italian
											L"A ID introduzida para o operador não está no sistema. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",		// Portuguese
											L"O ID inserido para o operador não está no sistema. Entre em contato com o suporte técnico para obter mais informações.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_WRONG_ROTOR				,	{	L"The disc inserted is not intended for this device. If symptom continues, contact Technical Service.\n",										// English
											L"Le disque inséré n'est pas prévu pour cet appareil. En cas de persistance des symptômes, contacter le service technique.\n",					// French
											L"Die eingesetzte Scheibe ist für dieses Gerät nicht geeignet. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",		// German
											L"El disco insertado no es apto para este aparato. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"Il disco inserito non è adatto a questo strumento. Se i sintomi continuano contattare il servizio tecnico.\n",									// Italian
											L"O disco inserido não é adequado para este dispositivo. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",					// Portuguese
											L"O disco inserido não é destinado a este dispositivo. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"						// Portuguese (Brazil)
										} 
	},


// Printer									ENGLISH,						FRENCH,							GERMAN,							SPANISH,							ITALIAN, 						PORTUGUESE,							PORTUGUESE (BRAZIL)
	{ PSTR_PRINTER_TEST				,	{	L"Printer Test",				L"Test imprimante",				L"Druckertest",					L"Prueba de impresora",				L"Prova stampante",				L"Teste da impressora",				L"Teste da impressora"	}	},

// Results (display and print)				ENGLISH,						FRENCH,							GERMAN,							SPANISH,							ITALIAN, 						PORTUGUESE,							PORTUGUESE (BRAZIL)
	{ RSTR_ACCEPTABLE_MINIMUM		,	{	L"Acceptable Minimum:",			L"Minimum Acceptable :",		L"Verwertbares min.:",			L"Minimo Aceptable:",				L"Minimo Accettabile:",			L"Mínimo aceitável:",				L"Mínimo aceitável:"	}	},
	{ RSTR_AGE						,	{	L"Age:",						L"âge :",						L"Alter",						L"Edad:",							L"Età:",						L"Idade:",							L"Idade:"	}	},
	{ RSTR_CHEMISTRY_QC				,	{	L"Chemistry QC:",				L"Chimie CQ :",					L"Chemie-QK:",					L"CC Quimico:",						L"QC Chimica:",					L"CQ Químico:",						L"QC de química:"	}	},
	{ RSTR_CONFIRM_LOW_RECOVERIES	,	{	L"! CONFIRM LOW RECOVERIES !",	L"! CONFIRMER FAIBLE RÉCUP !",	L"! NDR. WIEDERHERST. BESTÄT.!", L"! CONFIRME BAJA RECUPERACIÒN !",	L"! CONFERMA POCHI RECUPERI !",	L"! CONFIRME BAIXA RECUPERAÇÃO !",	L"! CONFIRM. RECUPER. BAIXAS !"	}	},
	{ RSTR_CONTROL_ID				,	{	L"Control ID:",					L"ID contr.:",					L"Kontr.-ID",					L"Id. ctrol:",						L"ID contrl:",					L"ID do controlo:",					L"ID de controle:"	}	},
	{ RSTR_CONTROL_RENEWAL			,	{	L"Control Renewal Required",	L"Renouv. contr. nécess.",		L"Kontrollerneuerung benötigt",	L"Renovar control",					L"Necess. rinnovo contr.",		L"Necessário renovar o controlo",	L"Renov. do controle necessária"	}	},
	{ RSTR_DEMONSTRATION_SOFTWARE	,	{	L"Demonstration Software",		L"Logiciel de démonstration",	L"Demonstrationssoftware",		L"Demo",							L"Software di dimostrazione",	L"Software de demonstração",		L"Software de demonstração"	}	},
	{ RSTR_ERROR					,	{	L"ERROR",						L"ERREUR",						L"FEHLER",						L"ERROR",							L"ERRORE",						L"ERRO",							L"ERRO"	}	},
	{ RSTR_GENDER					,	{	L"Gender:",						L"Sexe :",						L"Geschlecht:",					L"Sexo:",							L"Sesso:",						L"Sexo:",							L"Gênero:"	}	},
	{ RSTR_INSTRUMENT_QC			,	{	L"Instrument QC:",				L"Instrument CQ:",				L"Instrument QK:",				L"Control calidad instr:",			L"Strum. CQ:",					L"CQ do instrumento:",				L"QC de instrumento:"	}	},
	{ RSTR_NC						,	{	L"N/C",							L"N/C",							L"N/B",							L"N/C",								L"N/C",							L"N/B",								L"N/C"	}	},
	{ RSTR_NEG						,	{	L"NEG",							L"NEG",							L"NEG",							L"NEG",								L"NEG",							L"NEG",								L"NEG"	}	},
	{ RSTR_OPERATOR_ID				,	{	L"Operator ID:",				L"ID opérateur :",				L"Bediener-ID:",				L"Id. operario:",					L"ID operatore:",				L"ID do operador:",					L"ID do operador:"	}	},
	{ RSTR_OPERATOR_RENEWAL			,	{	L"Operator Renewal Required",	L"Renouv. opér. nécess.",		L"Bedienererneuerung benötigt",	L"Renovar operario",				L"Necess. rinnovo operat.",		L"Necessário renovar o operador",	L"Renov. de operador necessária"	}	},
	{ RSTR_PATIENT_ID				,	{	L"Patient ID:",					L"ID patient :",				L"Patienten-ID:",				L"Id. paciente:",					L"ID paziente:",				L"ID do doente:",					L"ID do paciente:"	}	},
	{ RSTR_POS						,	{	L"POS",							L"POS",							L"POS",							L"POS",								L"POS",							L"POS",								L"POS"	}	},
	{ RSTR_QC_OK					,	{	L"QC     OK",					L"QC     OK",					L"QUALITAETSKONTROLLE    OK",	L"CONTROL DE CALIDAD     OK",		L"CONTROLLO DI QUALITA   OK",	L"CQ     OK",						L"QC     OK"	}	},
	{ RSTR_RACE						,	{	L"Race:",						L"Race :",						L"Rasse:",						L"Raza:",							L"Razza:",						L"Raça:",							L"Raça:"	}	},
	{ RSTR_RANGE					,	{	L"Range:",						L"Registre :",					L"Bereich:",					L"Rango:",							L"Banda",						L"Intervalo:",						L"Intervalo:"	}	},
	{ RSTR_RANGE_LABEL				,	{	L"Range Label:",				L"Étiq.fourch:",				L"Bereichskz.:",				L"Intervalo:",						L"Etich.range:",				L"Rót. Inter.:",					L"Rót. inter.:"	}	},
	{ RSTR_RESEARCH_SOFTWARE		,	{	L"Research Software",			L"Logiciel recherche",			L"Research Software",			L"Software investig.",				L"Software ricerca",			L"Software de pesquisa",			L"Software de pesquisa"	}	},
	{ RSTR_ROTOR_LOT_NUM			,	{	L"Disc Lot Number:",			L"N° de lot disque :",			L"Scheiben-Losnr.:",			L"No. de lote del disco:",			L"Numero lotto disco:",			L"Número lote disco:",				L"Núm. de lote do disco:"	}	},
	{ RSTR_SAMPLE_TYPE				,	{	L"Sample Type:",				L"Type éch. :",					L"Probentyp:",					L"Muestra:",						L"Tipo camp.:",					L"Tipo amost.:",					L"Tipo amostr:"	}	},
	{ RSTR_SERIAL_NUM				,	{	L"Serial Number:",				L"N° de série :",				L"Seriennr.:",					L"Número de serie:",				L"Numero di serie:",			L"Número de série:",				L"Número de série:"	}	},
	{ RSTR_UNAUTHORIZED_OPERATOR	,	{	L"Unauthorized Operator",		L"Opér. non autorisé",			L"Unautorisierter Bediener",	L"Operario no autorizado",			L"Operat. non autorizz.",		L"Operador não autorizado",			L"Operador não autorizado"	}	},
	{ RSTR_UNKNOWN					,	{	L"Unknown",						L"Inconnu",						L"Unbekannt",					L"Desconocido",						L"Sconosciuto",					L"Desconhecido",					L"Desconhecido"	}	},

// Sample ID labels for results				ENGLISH,			FRENCH,				GERMAN,				SPANISH,			ITALIAN, 			PORTUGUESE,				PORTUGUESE (BRAZIL)
	{ RSTR_ADMISSION_ID				,	{	L"Admission ID:",	L"ID admission :",	L"Zugangs-ID:",		L"Id. admisión:",	L"ID ammissione:",	L"ID de Admissão:",		L"ID de admissão:"	}	},
	{ RSTR_ALTERNATE_ID				,	{	L"Alternate ID:",	L"Autre ID :",		L"ID wechseln:",	L"Id. suplente:",	L"ID alternativo:",	L"ID suplente:",		L"ID alternativo:"	}	},
	{ RSTR_DOCTOR_ID				,	{	L"Doctor ID:",		L"ID médecin :",	L"Arzt-ID:",		L"Id. del médico:",	L"ID medico:",		L"ID do médico:",		L"ID do médico:"	}	},
	{ RSTR_DOB						,	{	L"DOB:",			L"DOB :",			L"GD:",				L"F. nac.:",		L"DN:",				L"DOB:",				L"DN:"	}	},
	{ RSTR_LOCATION					,	{	L"Location:",		L"Emplcmt:",		L"Ort:",			L"Ubicac.:",		L"Sede:",			L"Localização:",		L"Local:"	}	},
	{ RSTR_PHONE_NUM				,	{	L"Phone Number:",	L"N° tél :",		L"Telefonnummer:",	L"Teléfono:",		L"Num. telefono:",	L"Núm. telefone:",		L"Núm. telefone:"	}	},
	{ RSTR_SAMPLE_ID				,	{	L"Sample ID:",		L"ID éch. :",		L"Proben-ID:",		L"Id. de muestra:",	L"ID campione:",	L"ID da amostra:",		L"ID da amostra:"	}	},
	
// Age labels for results					ENGLISH,	FRENCH,			GERMAN,		SPANISH,	ITALIAN, 		PORTUGUESE,		PORTUGUESE (BRAZIL)
	{ RSTR_DAY						,	{	L"Day",		L"Jour",		L"Tag",		L"Día",		L"Giorno",		L"Dia",			L"Dia"	}	},
	{ RSTR_DAYS						,	{	L"Days",	L"Jours",		L"Tage",	L"Días",	L"Giorni",		L"Dias",		L"Dias"	}	},
	{ RSTR_WEEK						,	{	L"Week",	L"Semaine",		L"Woche",	L"Semana",	L"Settimana",	L"Semana",		L"Semana"	}	},
	{ RSTR_WEEKS					,	{	L"Weeks",	L"Semaines",	L"Wochen",	L"Semanas",	L"Settimane",	L"Semanas",		L"Semanas"	}	},
	{ RSTR_MONTH					,	{	L"Month",	L"Mois",		L"Monat",	L"Mes",		L"Mese",		L"Mês",			L"Mês"	}	},
	{ RSTR_MONTHS					,	{	L"Months",	L"Mois",		L"Monate",	L"Meses",	L"Mesi",		L"Meses",		L"Meses"	}	},
	{ RSTR_YEAR						,	{	L"Year",	L"Année",		L"Jahr",	L"Año",		L"Anno",		L"Ano",			L"Ano"	}	},
	{ RSTR_YEARS					,	{	L"Years",	L"Années",		L"Jahre",	L"Años",	L"Anni",		L"Anos",		L"Anos"	}	},

// Genders									ENGLISH,		FRENCH,			GERMAN,				SPANISH,		ITALIAN, 	   		PORTUGUESE,	   		PORTUGUESE (BRAZIL)
	{ GSTR_FEMALE					,	{	L"Female",		L"Féminin",		L"Weiblich",		L"Mujer",		L"Uomo",			L"Feminino",		L"Sexo feminino"	}	},
	{ GSTR_MALE						,	{	L"Male",		L"Masculin",	L"Männlich",		L"Hombre",		L"Donna",			L"Masculino",		L"Sexo masculino"	}	},
	{ GSTR_NEUTERED					,	{	L"Neutered",	L"Châtré",		L"Kastriert",		L"Castrado",	L"Castrato",		L"Castrado",		L"Castrado"	}	},
	{ GSTR_SPAYED					,	{	L"Spayed",		L"Châtrée",		L"Sterilisiert",	L"Castrada",	L"Sterilizzato",	L"Esterilizado",	L"Esterilizado"	}	},
	{ GSTR_UNKNOWN					,	{	L"Unknown",		L"Inconnu",		L"Unbekannt",		L"Desconocido",	L"Sconosciuto",		L"Desconhecido",	L"Desconhecido"	}	},
	
// Race labels for results					ENGLISH,					FRENCH,						GERMAN,						SPANISH,						ITALIAN, 				PORTUGUESE,					PORTUGUESE (BRAZIL)
//	  RSTR_UNKNOWN - defined above
	{ RSTR_WHITE					,	{	L"White",					L"Blanche",					L"Weiß",					L"Blanco",					L"Bianco",					L"Branco",					L"Branco"	}	},
	{ RSTR_BLACK					,	{	L"Black",					L"Noire",					L"Schwarz",					L"Negro",					L"Nero",					L"Negro",					L"Preto"	}	},
	{ RSTR_ASIAN_PACIFIC_ISLANDER	,	{	L"Asian/Pacific Islander",	L"Asiat./orig. îles Pac.",	L"Asiatisch/Pazifisch",		L"Asiático/Isl. Pacífico",	L"Isolano Asia/Pacifico",	L"Ilhas Asiát./ Pacífico",	L"Asiático/ilhas Pacíf."	}	},
	{ RSTR_NATIVE_AMERICAN			,	{	L"Native American",			L"Amérindienne",			L"Indianische Abstammung",	L"Americano nativo",		L"Nativo americano",		L"Americano nativo",		L"Americano nativo"	}	},
	{ RSTR_HISPANIC					,	{	L"Hispanic",				L"Hispanique",				L"Hispanische Abstammung",	L"Hispano",					L"Ispanico",				L"Hispânico",				L"Hispânico"	}	},

// Species									ENGLISH,			FRENCH,				GERMAN,				SPANISH,	 		ITALIAN, 			PORTUGUESE,	 		PORTUGUESE (BRAZIL)
	{ SSTR_CONTROL					,	{	L"Control",			L"Contrôle",		L"Kontrolle",		L"Control",	 		L"Controllo",		L"Controlo",	 	L"Controle"	}	},
	{ SSTR_PATIENT					,	{	L"Patient",			L"Patient",			L"Patient",			L"Paciente",		L"Paziente",		L"Doente",	 		L"Paciente"	}	},
	{ SSTR_OTHER					,	{	L"Patient",			L"Patient",			L"Patient",			L"Paciente",		L"Paziente",		L"Doente",	 		L"Paciente"	}	},
	{ SSTR_SPECIES					,	{	L"Special",			L"Spécial",			L"Spezial",			L"Especial",		L"Speciale",		L"Especial",	 	L"Especial"	}	},
	
	{ SSTR_CANINES					,	{	L"Category 1", 		L"Catégorie 1", 	L"Kategorie 1", 	L"Categoría 1",		L"Categoria 1",		L"Categoria 1",		L"Categoria 1"	}	},
	{ SSTR_FELINES					,	{	L"Category 2", 		L"Catégorie 2", 	L"Kategorie 2", 	L"Categoría 2",		L"Categoria 2",		L"Categoria 2",		L"Categoria 2"	}	},
	{ SSTR_EQUINES					,	{	L"Category 3", 		L"Catégorie 3", 	L"Kategorie 3", 	L"Categoría 3",		L"Categoria 3",		L"Categoria 3",		L"Categoria 3"	}	},
	{ SSTR_BOVINES					,	{	L"Category 4", 		L"Catégorie 4", 	L"Kategorie 4", 	L"Categoría 4",		L"Categoria 4",		L"Categoria 4",		L"Categoria 4"	}	},
	{ SSTR_AVIANS					,	{	L"Category 5", 		L"Catégorie 5", 	L"Kategorie 5", 	L"Categoría 5",		L"Categoria 5",		L"Categoria 5",		L"Categoria 5"	}	},
	{ SSTR_REPTILES_AMPHIBIANS		,	{	L"Category 6", 		L"Catégorie 6", 	L"Kategorie 6", 	L"Categoría 6",		L"Categoria 6",		L"Categoria 6",		L"Categoria 6"	}	},
	{ SSTR_SMALL_MAMMALS			,	{	L"Category 7", 		L"Catégorie 7", 	L"Kategorie 7", 	L"Categoría 7",		L"Categoria 7",		L"Categoria 7",		L"Categoria 7"	}	},
	{ SSTR_LARGE_MAMMALS			,	{	L"Category 8", 		L"Catégorie 8", 	L"Kategorie 8", 	L"Categoría 8",		L"Categoria 8",		L"Categoria 8",		L"Categoria 8"	}	},

	// Canines
	{ SSTR_DOG						,	{	L"Demographic 1",	L"Elmts démogr. 1", L"Demographie 1", 	L"Demografía 1",	L"Demografia 1",	L"Demografia 1",	L"Dados demogr. 1"	}	},
	{ SSTR_COYOTE					,	{	L"Demographic 2",	L"Elmts démogr. 2", L"Demographie 2", 	L"Demografía 2",	L"Demografia 2",	L"Demografia 2",	L"Dados demogr. 2"	}	},
	{ SSTR_DINGO					,	{	L"Demographic 3", 	L"Elmts démogr. 3", L"Demographie 3", 	L"Demografía 3",	L"Demografia 3",	L"Demografia 3",	L"Dados demogr. 3"	}	},
	{ SSTR_FOX						,	{	L"Demographic 4", 	L"Elmts démogr. 4", L"Demographie 4", 	L"Demografía 4",	L"Demografia 4",	L"Demografia 4",	L"Dados demogr. 4"	}	},
	{ SSTR_JACKAL					,	{	L"Demographic 5", 	L"Elmts démogr. 5", L"Demographie 5", 	L"Demografía 5",	L"Demografia 5",	L"Demografia 5",	L"Dados demogr. 5"	}	},
	{ SSTR_WOLF						,	{	L"Demographic 6", 	L"Elmts démogr. 6", L"Demographie 6", 	L"Demografía 6",	L"Demografia 6",	L"Demografia 6",	L"Dados demogr. 6"	}	},

	// Felines
	{ SSTR_CAT						,	{	L"Demographic 7", 	L"Elmts démogr. 7", L"Demographie 7"	L"Demografía 7", 	L"Demografia 7",	L"Demografia 7",	L"Dados demogr. 7"	}	},
	{ SSTR_BOBCAT					,	{	L"Demographic 8", 	L"Elmts démogr. 8", L"Demographie 8",	L"Demografía 8", 	L"Demografia 8",	L"Demografia 8",	L"Dados demogr. 8"	}	},
	{ SSTR_CHEETAH					,	{	L"Demographic 9", 	L"Elmts démogr. 9", L"Demographie 9",	L"Demografía 9", 	L"Demografia 9",	L"Demografia 9",	L"Dados demogr. 9"	}	},
	{ SSTR_JAGUAR					,	{	L"Demographic 10", 	L"Elmts démogr. 10", L"Demographie 10",	L"Demografía 10",	L"Demografia 10",	L"Demografia 10",	L"Dados demogr. 10"	}	},
	{ SSTR_LEOPARD					,	{	L"Demographic 11", 	L"Elmts démogr. 11", L"Demographie 11",	L"Demografía 11",	L"Demografia 11",	L"Demografia 11",	L"Dados demogr. 11"	}	},
	{ SSTR_LION						,	{	L"Demographic 12", 	L"Elmts démogr. 12", L"Demographie 12",	L"Demografía 12",	L"Demografia 12",	L"Demografia 12",	L"Dados demogr. 12"	}	},
	{ SSTR_LYNX						,	{	L"Demographic 13", 	L"Elmts démogr. 13", L"Demographie 13",	L"Demografía 13",	L"Demografia 13",	L"Demografia 13",	L"Dados demogr. 13"	}	},
	{ SSTR_PANTHER					,	{	L"Demographic 14", 	L"Elmts démogr. 14", L"Demographie 14",	L"Demografía 14",	L"Demografia 14",	L"Demografia 14",	L"Dados demogr. 14"	}	},
	{ SSTR_PUMA						,	{	L"Demographic 15", 	L"Elmts démogr. 15", L"Demographie 15",	L"Demografía 15",	L"Demografia 15",	L"Demografia 15",	L"Dados demogr. 15"	}	},
	{ SSTR_TIGER					,	{	L"Demographic 16", 	L"Elmts démogr. 16", L"Demographie 16",	L"Demografía 16",	L"Demografia 16",	L"Demografia 16",	L"Dados demogr. 16"	}	},
	
	// Equines
	{ SSTR_HORSE					,	{	L"Demographic 17", 	L"Elmts démogr. 17", L"Demographie 17",	L"Demografía 17",	L"Demografia 17",	L"Demografia 17",	L"Dados demogr. 17"	}	},
	{ SSTR_DONKEY					,	{	L"Demographic 18", 	L"Elmts démogr. 18", L"Demographie 18",	L"Demografía 18",	L"Demografia 18",	L"Demografia 18",	L"Dados demogr. 18"	}	},
	{ SSTR_PONY						,	{	L"Demographic 19", 	L"Elmts démogr. 19", L"Demographie 19",	L"Demografía 19",	L"Demografia 19",	L"Demografia 19",	L"Dados demogr. 19"	}	},
	{ SSTR_ZEBRA					,	{	L"Demographic 20", 	L"Elmts démogr. 20", L"Demographie 20",	L"Demografía 20",	L"Demografia 20",	L"Demografia 20",	L"Dados demogr. 20"	}	},
	
	// Bovines
	{ SSTR_COW						,	{	L"Demographic 21", 	L"Elmts démogr. 21", L"Demographie 21",	L"Demografía 21",	L"Demografia 21",	L"Demografia 21",	L"Dados demogr. 21"	}	},
	{ SSTR_WATER_BUFFALO			,	{	L"Demographic 22", 	L"Elmts démogr. 22", L"Demographie 22",	L"Demografía 22",	L"Demografia 22",	L"Demografia 22",	L"Dados demogr. 22"	}	},
	{ SSTR_STEER					,	{	L"Demographic 23", 	L"Elmts démogr. 23", L"Demographie 23",	L"Demografía 23",	L"Demografia 23",	L"Demografia 23",	L"Dados demogr. 23"	}	},
	{ SSTR_OX						,	{	L"Demographic 24", 	L"Elmts démogr. 24", L"Demographie 24",	L"Demografía 24",	L"Demografia 24",	L"Demografia 24",	L"Dados demogr. 24"	}	},
	{ SSTR_BULL						,	{	L"Demographic 25", 	L"Elmts démogr. 25", L"Demographie 25",	L"Demografía 25",	L"Demografia 25",	L"Demografia 25",	L"Dados demogr. 25"	}	},
	{ SSTR_BUFFALO					,	{	L"Demographic 26", 	L"Elmts démogr. 26", L"Demographie 26",	L"Demografía 26",	L"Demografia 26",	L"Demografia 26",	L"Dados demogr. 26"	}	},
	{ SSTR_BISON					,	{	L"Demographic 27", 	L"Elmts démogr. 27", L"Demographie 27",	L"Demografía 27",	L"Demografia 27",	L"Demografia 27",	L"Dados demogr. 27"	}	},
	
	// Avians
	{ SSTR_BIRD						,	{	L"Demographic 28", 	L"Elmts démogr. 28", L"Demographie 28",	L"Demografía 28",	L"Demografia 28",	L"Demografia 28",	L"Dados demogr. 28"	}	},
	{ SSTR_AFRICAN_GRAY				,	{	L"Demographic 29", 	L"Elmts démogr. 29", L"Demographie 29",	L"Demografía 29",	L"Demografia 29",	L"Demografia 29",	L"Dados demogr. 29"	}	},
	{ SSTR_AMAZON					,	{	L"Demographic 30", 	L"Elmts démogr. 30", L"Demographie 30",	L"Demografía 30",	L"Demografia 30",	L"Demografia 30",	L"Dados demogr. 30"	}	},
	{ SSTR_CHICKEN					,	{	L"Demographic 31", 	L"Elmts démogr. 31", L"Demographie 31",	L"Demografía 31",	L"Demografia 31",	L"Demografia 31",	L"Dados demogr. 31"	}	},
	{ SSTR_COCKATIEL				,	{	L"Demographic 32", 	L"Elmts démogr. 32", L"Demographie 32",	L"Demografía 32",	L"Demografia 32",	L"Demografia 32",	L"Dados demogr. 32"	}	},
	{ SSTR_COCKATOO					,	{	L"Demographic 33", 	L"Elmts démogr. 33", L"Demographie 33",	L"Demografía 33",	L"Demografia 33",	L"Demografia 33",	L"Dados demogr. 33"	}	},
	{ SSTR_CONURE					,	{	L"Demographic 34", 	L"Elmts démogr. 34", L"Demographie 34",	L"Demografía 34",	L"Demografia 34",	L"Demografia 34",	L"Dados demogr. 34"	}	},
	{ SSTR_DOVE						,	{	L"Demographic 35", 	L"Elmts démogr. 35", L"Demographie 35",	L"Demografía 35",	L"Demografia 35",	L"Demografia 35",	L"Dados demogr. 35"	}	},
	{ SSTR_DUCK						,	{	L"Demographic 36", 	L"Elmts démogr. 36", L"Demographie 36",	L"Demografía 36",	L"Demografia 36",	L"Demografia 36",	L"Dados demogr. 36"	}	},
	{ SSTR_FLAMINGO					,	{	L"Demographic 37", 	L"Elmts démogr. 37", L"Demographie 37",	L"Demografía 37",	L"Demografia 37",	L"Demografia 37",	L"Dados demogr. 37"	}	},
	{ SSTR_GOOSE					,	{	L"Demographic 38", 	L"Elmts démogr. 38", L"Demographie 38",	L"Demografía 38",	L"Demografia 38",	L"Demografia 38",	L"Dados demogr. 38"	}	},
	{ SSTR_LOVEBIRD					,	{	L"Demographic 39", 	L"Elmts démogr. 39", L"Demographie 39",	L"Demografía 39",	L"Demografia 39",	L"Demografia 39",	L"Dados demogr. 39"	}	},
	{ SSTR_MACAW					,	{	L"Demographic 40", 	L"Elmts démogr. 40", L"Demographie 40",	L"Demografía 40",	L"Demografia 40",	L"Demografia 40",	L"Dados demogr. 40"	}	},
	{ SSTR_MYNAH					,	{	L"Demographic 41", 	L"Elmts démogr. 41", L"Demographie 41",	L"Demografía 41",	L"Demografia 41",	L"Demografia 41",	L"Dados demogr. 41"	}	},
	{ SSTR_OSTRICH					,	{	L"Demographic 42", 	L"Elmts démogr. 42", L"Demographie 42",	L"Demografía 42",	L"Demografia 42",	L"Demografia 42",	L"Dados demogr. 42"	}	},
	{ SSTR_PARAKEET					,	{	L"Demographic 43", 	L"Elmts démogr. 43", L"Demographie 43",	L"Demografía 43",	L"Demografia 43",	L"Demografia 43",	L"Dados demogr. 43"	}	},
	{ SSTR_PARROT					,	{	L"Demographic 44", 	L"Elmts démogr. 44", L"Demographie 44",	L"Demografía 44",	L"Demografia 44",	L"Demografia 44",	L"Dados demogr. 44"	}	},
	{ SSTR_TOUCAN					,	{	L"Demographic 45", 	L"Elmts démogr. 45", L"Demographie 45",	L"Demografía 45",	L"Demografia 45",	L"Demografia 45",	L"Dados demogr. 45"	}	},
	
	// Reptiles/amphibians
	{ SSTR_REPTILE					,	{	L"Demographic 46",	L"Elmts démogr. 46", L"Demographie 46",	L"Demografía 46",	L"Demografia 46",	L"Demografia 46",	L"Dados demogr. 46"	}	},
	{ SSTR_ALLIGATOR				,	{	L"Demographic 47",	L"Elmts démogr. 47", L"Demographie 47",	L"Demografía 47",	L"Demografia 47",	L"Demografia 47",	L"Dados demogr. 47"	}	},
	{ SSTR_AMPHIBIAN				,	{	L"Demographic 48",	L"Elmts démogr. 48", L"Demographie 48",	L"Demografía 48",	L"Demografia 48",	L"Demografia 48",	L"Dados demogr. 48"	}	},
	{ SSTR_BEARDED_DRAGON			,	{	L"Demographic 49",	L"Elmts démogr. 49", L"Demographie 49",	L"Demografía 49",	L"Demografia 49",	L"Demografia 49",	L"Dados demogr. 49"	}	},
	{ SSTR_BOA_CONSTRICTOR			,	{	L"Demographic 50",	L"Elmts démogr. 50", L"Demographie 50",	L"Demografía 50",	L"Demografia 50",	L"Demografia 50",	L"Dados demogr. 50"	}	},
	{ SSTR_CHAMELEON				,	{	L"Demographic 51",	L"Elmts démogr. 51", L"Demographie 51",	L"Demografía 51",	L"Demografia 51",	L"Demografia 51",	L"Dados demogr. 51"	}	},
	{ SSTR_CROCODILE				,	{	L"Demographic 52",	L"Elmts démogr. 52", L"Demographie 52",	L"Demografía 52",	L"Demografia 52",	L"Demografia 52",	L"Dados demogr. 52"	}	},
	{ SSTR_FROG						,	{	L"Demographic 53",	L"Elmts démogr. 53", L"Demographie 53",	L"Demografía 53",	L"Demografia 53",	L"Demografia 53",	L"Dados demogr. 53"	}	},
	{ SSTR_GECKO					,	{	L"Demographic 54",	L"Elmts démogr. 54", L"Demographie 54",	L"Demografía 54",	L"Demografia 54",	L"Demografia 54",	L"Dados demogr. 54"	}	},
	{ SSTR_IGUANA					,	{	L"Demographic 55",	L"Elmts démogr. 55", L"Demographie 55",	L"Demografía 55",	L"Demografia 55",	L"Demografia 55",	L"Dados demogr. 55"	}	},
	{ SSTR_LIZARD					,	{	L"Demographic 56",	L"Elmts démogr. 56", L"Demographie 56",	L"Demografía 56",	L"Demografia 56",	L"Demografia 56",	L"Dados demogr. 56"	}	},
	{ SSTR_MONITOR					,	{	L"Demographic 57",	L"Elmts démogr. 57", L"Demographie 57",	L"Demografía 57",	L"Demografia 57",	L"Demografia 57",	L"Dados demogr. 57"	}	},
	{ SSTR_PYTHON					,	{	L"Demographic 58",	L"Elmts démogr. 58", L"Demographie 58",	L"Demografía 58",	L"Demografia 58",	L"Demografia 58",	L"Dados demogr. 58"	}	},
	{ SSTR_SALAMANDER				,	{	L"Demographic 59",	L"Elmts démogr. 59", L"Demographie 59",	L"Demografía 59",	L"Demografia 59",	L"Demografia 59",	L"Dados demogr. 59"	}	},
	{ SSTR_SNAKE					,	{	L"Demographic 60",	L"Elmts démogr. 60", L"Demographie 60",	L"Demografía 60",	L"Demografia 60",	L"Demografia 60",	L"Dados demogr. 60"	}	},
	{ SSTR_TOAD						,	{	L"Demographic 61",	L"Elmts démogr. 61", L"Demographie 61",	L"Demografía 61",	L"Demografia 61",	L"Demografia 61",	L"Dados demogr. 61"	}	},
	{ SSTR_TORTOISE					,	{	L"Demographic 62",	L"Elmts démogr. 62", L"Demographie 62",	L"Demografía 62",	L"Demografia 62",	L"Demografia 62",	L"Dados demogr. 62"	}	},
	{ SSTR_TURTLE					,	{	L"Demographic 63",	L"Elmts démogr. 63", L"Demographie 63",	L"Demografía 63",	L"Demografia 63",	L"Demografia 63",	L"Dados demogr. 63"	}	},
	
	// Small mammals
	{ SSTR_SMALL_MAMMAL				,	{	L"Demographic 64",	L"Elmts démogr. 64", L"Demographie 64",	L"Demografía 64",	L"Demografia 64",	L"Demografia 64",	L"Dados demogr. 64"	}	},
	{ SSTR_BAT						,	{	L"Demographic 65",	L"Elmts démogr. 65", L"Demographie 65",	L"Demografía 65",	L"Demografia 65",	L"Demografia 65",	L"Dados demogr. 65"	}	},
	{ SSTR_CHINCHILLA				,	{	L"Demographic 66",	L"Elmts démogr. 66", L"Demographie 66",	L"Demografía 66",	L"Demografia 66",	L"Demografia 66",	L"Dados demogr. 66"	}	},
	{ SSTR_FERRET					,	{	L"Demographic 67",	L"Elmts démogr. 67", L"Demographie 67",	L"Demografía 67",	L"Demografia 67",	L"Demografia 67",	L"Dados demogr. 67"	}	},
	{ SSTR_GERBIL					,	{	L"Demographic 68",	L"Elmts démogr. 68", L"Demographie 68",	L"Demografía 68",	L"Demografia 68",	L"Demografia 68",	L"Dados demogr. 68"	}	},
	{ SSTR_GUINEA_PIG				,	{	L"Demographic 69",	L"Elmts démogr. 69", L"Demographie 69",	L"Demografía 69",	L"Demografia 69",	L"Demografia 69",	L"Dados demogr. 69"	}	},
	{ SSTR_HAMSTER					,	{	L"Demographic 70",	L"Elmts démogr. 70", L"Demographie 70",	L"Demografía 70",	L"Demografia 70",	L"Demografia 70",	L"Dados demogr. 70"	}	},
	{ SSTR_HEDGEHOG					,	{	L"Demographic 71",	L"Elmts démogr. 71", L"Demographie 71",	L"Demografía 71",	L"Demografia 71",	L"Demografia 71",	L"Dados demogr. 71"	}	},
	{ SSTR_LEMUR					,	{	L"Demographic 72",	L"Elmts démogr. 72", L"Demographie 72",	L"Demografía 72",	L"Demografia 72",	L"Demografia 72",	L"Dados demogr. 72"	}	},
	{ SSTR_MINK						,	{	L"Demographic 73",	L"Elmts démogr. 73", L"Demographie 73",	L"Demografía 73",	L"Demografia 73",	L"Demografia 73",	L"Dados demogr. 73"	}	},
	{ SSTR_MOUSE					,	{	L"Demographic 74",	L"Elmts démogr. 74", L"Demographie 74",	L"Demografía 74",	L"Demografia 74",	L"Demografia 74",	L"Dados demogr. 74"	}	},
	{ SSTR_OPOSSUM					,	{	L"Demographic 75",	L"Elmts démogr. 75", L"Demographie 75",	L"Demografía 75",	L"Demografia 75",	L"Demografia 75",	L"Dados demogr. 75"	}	},
	{ SSTR_PRAIRIE_DOG				,	{	L"Demographic 76",	L"Elmts démogr. 76", L"Demographie 76",	L"Demografía 76",	L"Demografia 76",	L"Demografia 76",	L"Dados demogr. 76"	}	},
	{ SSTR_RABBIT					,	{	L"Demographic 77",	L"Elmts démogr. 77", L"Demographie 77",	L"Demografía 77",	L"Demografia 77",	L"Demografia 77",	L"Dados demogr. 77"	}	},
	{ SSTR_RACCOON					,	{	L"Demographic 78",	L"Elmts démogr. 78", L"Demographie 78",	L"Demografía 78",	L"Demografia 78",	L"Demografia 78",	L"Dados demogr. 78"	}	},
	{ SSTR_RAT						,	{	L"Demographic 79",	L"Elmts démogr. 79", L"Demographie 79",	L"Demografía 79",	L"Demografia 79",	L"Demografia 79",	L"Dados demogr. 79"	}	},
	{ SSTR_SKUNK					,	{	L"Demographic 80",	L"Elmts démogr. 80", L"Demographie 80",	L"Demografía 80",	L"Demografia 80",	L"Demografia 80",	L"Dados demogr. 80"	}	},
	{ SSTR_SQUIRREL					,	{	L"Demographic 81",	L"Elmts démogr. 81", L"Demographie 81",	L"Demografía 81",	L"Demografia 81",	L"Demografia 81",	L"Dados demogr. 81"	}	},
	
	// Large mammals
	{ SSTR_LARGE_MAMMAL				,	{	L"Demographic 82",	L"Elmts démogr. 82", L"Demographie 82",	L"Demografía 82",	L"Demografia 82",	L"Demografia 82",	L"Dados demogr. 82"	}	},
	{ SSTR_ANTELOPE					,	{	L"Demographic 83",	L"Elmts démogr. 83", L"Demographie 83",	L"Demografía 83",	L"Demografia 83",	L"Demografia 83",	L"Dados demogr. 83"	}	},
	{ SSTR_APE						,	{	L"Demographic 84",	L"Elmts démogr. 84", L"Demographie 84",	L"Demografía 84",	L"Demografia 84",	L"Demografia 84",	L"Dados demogr. 84"	}	},
	{ SSTR_BABOON					,	{	L"Demographic 85",	L"Elmts démogr. 85", L"Demographie 85",	L"Demografía 85",	L"Demografia 85",	L"Demografia 85",	L"Dados demogr. 85"	}	},
	{ SSTR_BEAR						,	{	L"Demographic 86",	L"Elmts démogr. 86", L"Demographie 86",	L"Demografía 86",	L"Demografia 86",	L"Demografia 86",	L"Dados demogr. 86"	}	},
	{ SSTR_CAMEL					,	{	L"Demographic 87",	L"Elmts démogr. 87", L"Demographie 87",	L"Demografía 87",	L"Demografia 87",	L"Demografia 87",	L"Dados demogr. 87"	}	},
	{ SSTR_CHIMPANZEE				,	{	L"Demographic 88",	L"Elmts démogr. 88", L"Demographie 88",	L"Demografía 88",	L"Demografia 88",	L"Demografia 88",	L"Dados demogr. 88"	}	},
	{ SSTR_DEER						,	{	L"Demographic 89",	L"Elmts démogr. 89", L"Demographie 89",	L"Demografía 89",	L"Demografia 89",	L"Demografia 89",	L"Dados demogr. 89"	}	},
	{ SSTR_ELEPHANT					,	{	L"Demographic 90",	L"Elmts démogr. 90", L"Demographie 90",	L"Demografía 90",	L"Demografia 90",	L"Demografia 90",	L"Dados demogr. 90"	}	},
	{ SSTR_GIRAFFE					,	{	L"Demographic 91",	L"Elmts démogr. 91", L"Demographie 91",	L"Demografía 91",	L"Demografia 91",	L"Demografia 91",	L"Dados demogr. 91"	}	},
	{ SSTR_GOAT						,	{	L"Demographic 92",	L"Elmts démogr. 92", L"Demographie 92",	L"Demografía 92",	L"Demografia 92",	L"Demografia 92",	L"Dados demogr. 92"	}	},
	{ SSTR_KANGAROO					,	{	L"Demographic 93",	L"Elmts démogr. 93", L"Demographie 93",	L"Demografía 93",	L"Demografia 93",	L"Demografia 93",	L"Dados demogr. 93"	}	},
	{ SSTR_LLAMA					,	{	L"Demographic 94",	L"Elmts démogr. 94", L"Demographie 94",	L"Demografía 94",	L"Demografia 94",	L"Demografia 94",	L"Dados demogr. 94"	}	},
	{ SSTR_MONKEY					,	{	L"Demographic 95",	L"Elmts démogr. 95", L"Demographie 95",	L"Demografía 95",	L"Demografia 95",	L"Demografia 95",	L"Dados demogr. 95"	}	},
	{ SSTR_PIG						,	{	L"Demographic 96",	L"Elmts démogr. 96", L"Demographie 96",	L"Demografía 96",	L"Demografia 96",	L"Demografia 96",	L"Dados demogr. 96"	}	},
	{ SSTR_SEA_LION					,	{	L"Demographic 97",	L"Elmts démogr. 97", L"Demographie 97",	L"Demografía 97",	L"Demografia 97",	L"Demografia 97",	L"Dados demogr. 97"	}	},
	{ SSTR_SEAL						,	{	L"Demographic 98",	L"Elmts démogr. 98", L"Demographie 98",	L"Demografía 98",	L"Demografia 98",	L"Demografia 98",	L"Dados demogr. 98"	}	},
	{ SSTR_SHEEP					,	{	L"Demographic 99",	L"Elmts démogr. 99", L"Demographie 99",	L"Demografía 99",	L"Demografia 99",	L"Demografia 99",	L"Dados demogr. 99"	}	},
	
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
	{ STR_FEB						,	{	L"Feb",			L"Fév",		L"Feb",			L"Feb",			L"Feb",			L"Fev",			L"Fev"	}	},
	{ STR_MAR						,	{	L"Mar",			L"Mar",		L"Mär",			L"Mar",			L"Mar",			L"Mar",			L"Mar"	}	},
	{ STR_APR						,	{	L"Apr",			L"Avr",		L"Apr",			L"Abr",			L"Apr",			L"Abr",			L"Abr"	}	},
	{ STR_MAY						,	{	L"May",			L"Mai",		L"Mai",			L"May",			L"Mag",			L"Mai",			L"Mai"	}	},
	{ STR_JUN						,	{	L"Jun",			L"Jun",		L"Jun",			L"Jun",			L"Giu",			L"Jun",			L"Jun"	}	},
	{ STR_JUL						,	{	L"Jul",			L"Jul",		L"Jul",			L"Jul",			L"Lug",			L"Jul",			L"Jul"	}	},
	{ STR_AUG						,	{	L"Aug",			L"Aoû",		L"Aug",			L"Ago",			L"Ago",			L"Ago",			L"Ago"	}	},
	{ STR_SEP						,	{	L"Sep",			L"Sep",		L"Sep",			L"Sep",			L"Set",			L"Set",			L"Set"	}	},
	{ STR_OCT						,	{	L"Oct",			L"Oct",		L"Okt",			L"Oct",			L"Ott",			L"Out",			L"Out"	}	},
	{ STR_NOV						,	{	L"Nov",			L"Nov",		L"Nov",			L"Nov",			L"Nov",			L"Nov",			L"Nov"	}	},
	{ STR_DEC						,	{	L"Dec",			L"Déc",		L"Dez",			L"Dic",			L"Dic",			L"Dez",			L"Dez"	}	},

	{ STR_SUNDAY					,	{	L"Sunday",		L"Dimanche",	L"Sonntag",		L"Domingo",		L"Domenica",	L"Domingo",		L"Domingo"	}	},
	{ STR_MONDAY					,	{	L"Monday",		L"Lundi",		L"Montag",		L"Lunes",		L"Lunedì",		L"Segunda",		L"Segunda-feira"	}	},
	{ STR_TUESDAY					,	{	L"Tuesday",		L"Mardi",		L"Dienstag",	L"Martes",		L"Martedì",		L"Terça",		L"Terça-feira"	}	},
	{ STR_WEDNESDAY					,	{	L"Wednesday",	L"Mercredi",	L"Mittwoch",	L"Miércoles",	L"Mercoledì",	L"Quarta",		L"Quarta-feira"	}	},
	{ STR_THURSDAY					,	{	L"Thursday",	L"Jeudi",		L"Donnerstag",	L"Jueves",		L"Giovedì",		L"Quinta",		L"Quinta-feira"	}	},
	{ STR_FRIDAY					,	{	L"Friday",		L"Vendredi",	L"Freitag",		L"Viernes",		L"Venerdì",		L"Sexta",		L"Sexta-feira"	}	},
	{ STR_SATURDAY					,	{	L"Saturday",	L"Samedi",		L"Samstag",		L"Sábado",		L"Sabato",		L"Sábado",		L"Sábado"	}	},
	{ STR_LANGUAGE                  ,	{	L"English",		L"Français",	L"Deutsch",		L"Español",     L"Italiano",	L"Português",	L"Português Brasil"}	},
	{ STR_NONE						,	{} }
};


WesternStringArr_t  VET_WESTERN_STRINGS[]  = {
// 											ENGLISH,					FRENCH,								GERMAN,								SPANISH,								ITALIAN, 								PORTUGUESE,									PORTUGUESE (BRAZIL)
	{ STR_PRODUCT_NAME				,	{	L"VetScan VS2",				L"VetScan VS2",						L"VetScan VS2",						L"VetScan VS2",							L"VetScan VS2",							L"VetScan VS2",								L"VetScan VS2"	}	},

// Top
	{ TSTR_ADD_SPECIES				,	{	L"Add Species",				L"Ajouter espèces",					L"Arten hinzufügen",				L"Añadir especie",						L"Aggiungi specie",						L"Adicionar espécies",						L"Adicionar espécie"	}	},
	{ TSTR_FACTORY_DEFAULT_SPECIES	,	{	L"Factory Default Species",	L"Paramètres par défaut espèces",	L"Voreingestellte Standardarten",	L"Especies predeterminadas",			L"Specie predefinita di fabbrica",		L"Espécies predeterminadas de fábrica",		L"Espécie padrão de fábrica"	}	},
	{ TSTR_INSERT_ROTOR				,	{	L"Insert Rotor",			L"Insérer rotor",					L"Rotor einsetzen",					L"Insertar rotor",						L"Inserisci rotore",					L"Inserir rotor",							L"Inserir rotor"	}	},
	{ TSTR_LOAD_ROTOR				,	{	L"Load Rotor",				L"Charger rotor",					L"Rotor laden",						L"Cargar rotor",						L"Carica rotore",						L"Carregar rotor",							L"Carregar rotor"	}	},
	{ TSTR_REMOVE_ROTOR				,	{	L"Remove Rotor",			L"Retirer rotor",					L"Rotor entfernen",					L"Retirar rotor",						L"Rimuovi rotore",						L"Remover rotor",							L"Remover rotor"	}	},
	{ TSTR_REMOVE_SPECIES			,	{	L"Remove Species",			L"Retirer espèces",					L"Arten entfernen",					L"Eliminar especie",					L"Rimuovi specie",						L"Remover espécies",						L"Remover espécies"	}	},
	{ TSTR_SELECT_SPECIES			,	{	L"Select Species",			L"Sélectionner espèces",			L"Arten auswählen",					L"Seleccionar especie",					L"Seleziona specie",					L"Selecionar espécies",						L"Selecionar espécies"	}	},
	{ TSTR_SPECIES					,	{	L"Species",					L"Espèces",							L"Arten",							L"Especie",								L"Specie",								L"Espécies",								L"Espécies"	}	},
	{ TSTR_SPECIES_AVAILABLE		,	{	L"Species Available",		L"Espèces disponibles",				L"Verfügbare Arten",				L"Especies disponibles",				L"Specie disponibili",					L"Espécies disponíveis",					L"Espécies disponíveis"	}	},

// Middle									ENGLISH,																		FRENCH,																							GERMAN,																										SPANISH,																							ITALIAN,																								PORTUGUESE,																									PORTUGUESE (BRAZIL)
	{ MSTR_ADD_SPECIES				,	{	L"Add Species",																	L"Ajouter espèces",																				L"Arten hinzufügen",																						L"Añadir especie",																					L"Aggiungi specie",																						L"Adicionar espécies",																						L"Adicionar espécie"	}	},
	{ MSTR_DETERMINING_ROTOR_TYPE	,	{	L"Determining rotor type",														L"Déterm. type de rotor",																		L"Rotortyp festlegen",																						L"Determinando el tipo de rotor",																	L"Determinazione tipo del rotore",																		L"A determinar o tipo de rotor",																			L"Determinando o tipo de rotor"	}	},
	{ MSTR_FACTORY_DEFAULT_SPECIES	,	{	L"Factory Default Species",														L"Param. par défaut espèces",																	L"Voreingestellte Arten",																					L"Especie predeterminada de fábrica",																L"Specie predefinita di fabbrica",																		L"Espécies predeterminadas de fábrica",																		L"Espécie padrão de fábrica"	}	},
	{ MSTR_FEMALE					,	{	L"Female",																		L"Femelle",																						L"Weiblich",																								L"Hembra",																							L"Femmina",																								L"Fêmea",																									L"Sexo feminino"	}	},
	{ MSTR_LAST_ROTOR				,	{	L"Last Rotor",																	L"Dernier rotor",																				L"Letzter Rotor",																							L"Último rotor",																					L"Ultimo rotore",																						L"Último rotor",																							L"Último rotor"	}	},
	{ MSTR_LOAD_ROTOR_TO_ANALYZE	,	{	L"To analyze additional sample, load new rotor",								L"Pour analyser un échantillon supplémentaire, charger un nouveau rotor",						L"Zur Analyse einer weiteren Probe neuen Rotor laden",														L"Para analizar otra muestra, cargue un rotor nuevo",												L"Per esaminare ulteriori campioni, caricare nuovo rotore",												L"Para analisar outra amostra, carregue novo rotor",														L"Para analisar uma amostra adicional, carregue um novo rotor"	}	},
	{ MSTR_MALE						,	{	L"Male",																		L"Mâle",																						L"Männlich",																								L"Macho",																							L"Maschio",																								L"Macho",																									L"Sexo masculino"	}	},
	{ MSTR_NO_RESULTS				,	{	L"No saved results to view. Please run a rotor prior to recalling results.",	L"Aucun résultat enregistré à afficher. Exécuter un rotor avant de rappeler les résultats.",	L"Keine gespeicherten Ergebnisse zur Anzeige. Vor Wiederaufrufen der Ergebnisse einen Rotor starten.",		L"No hay resultados guardados para ver. Utilice un rotor antes de buscar resultados.",				L"Nessun risultato salvato da visualizzare. Avviare un rotore prima di richiamare i risultati.",		L"No existem resultados guardados para visualizar. Utilize um rotor antes de recuperar resultados.",		L"Nenhum resultado salvo para visualização. Execute um rotor antes de recuperar os resultados."	}	},
	{ MSTR_PLEASE_REMOVE_ROTOR		,	{	L"Please remove the rotor from the analyzer",									L"Retirer le rotor de l'analyseur",																L"Bitte den Rotor aus dem Analysegerät entfernen",															L"Retire el rotor del analizador",																	L"Rimuovere il rotore dall'analizzatore",																L"Retire o rotor do analisador",																			L"Remova o rotor do analisador"	}	},
	{ MSTR_PRESS_YES_REF_RANGES		,	{	L"Press Yes to return the active species list to the factory default",			L"Appuyer sur Oui pour rétablir la liste d'espèces actives par défaut",							L"Drücken Sie Ja, um die Werkseinstellungen für die Liste der aktiven Arten wieder herzustellen",			L"Pulse Sí para reestablecer los valores predeterminados de la lista de especies activas",			L"Premere Sì per mandare l'elenco specie attive nelle impostazioni di fabbrica",						L"Prima Sim para reestabelecer a lista de espécies ativa para os valores predeterminados de fábrica",		L"Pressione Sim para retornar a lista de espécies ativas ao padrão de fábrica"	}	},
	{ MSTR_REMOVE_SPECIES			,	{	L"Remove Species",																L"Retirer espèces",																				L"Arten entfernen",																							L"Eliminar especie",																				L"Rimuovi specie",																						L"Remover espécies",																						L"Remover espécies"	}	},
	{ MSTR_ROTORS_RUN				,	{	L"Rotors Run",																	L"Exécutions rotors",																			L"Rotoren in Betrieb",																						L"Usar rotores",																					L"Rotori in funzione",																					L"Utilizar rotores",																						L"Execução de rotores"	}	},
	{ MSTR_SPECIES					,	{	L"Species",																		L"Espèces",																						L"Arten",																									L"Esp.",																							L"Specie",																								L"Espécies",																								L"Espécies"	}	},

// Info text
	{ ISTR_LOAD_ROTOR_1				,	{	L"Remove reagent rotor from its foil pouch and transfer 100ul of whole blood, plasma or serum into the rotor.\n",				// English
											L"Retirer le rotor de réactif de sa pochette alu et transférer 100 ul de sang entier, de plasma ou de sérum dans le rotor.\n",	// French
											L"Reagenzrotor aus dem Plastikbeutel entfernen und 100ul Vollblut, Plasma oder Serum in den Rotor einfüllen.\n",					// German
											L"Retire el rotor de reactivo de su bolsa metalizada y transfiera 100 ul de sangre, plasma o suero al rotor.\n",					// Spanish
											L"Rimuovere il rotore reagente dalla custodia metallizzata e trasferire 100ul di sangue intero, plasma o siero nel rotore.\n",	// Italian
											L"Retire o rotor de reagente da sua bolsa metalizada e transfira 100 ul de sangue total, plasma ou soro para o rotor.\n",		// Portuguese
											L"Remova o rotor de reagente da bolsa de alumínio e transfira 100 µl de sangue total, plasma ou soro para o rotor.\n"			// Portuguese (Brazil)
										} 
	},
	{ ISTR_LOAD_ROTOR_2				,	{	L"Insert the reagent rotor into the analyzer, close the drawer and follow the on-screen prompts.\n",								// English
											L"Insérer le rotor de réactif dans l'analyseur, fermer le tiroir et suivre les instructions qui s'affichent à l'écran.\n",		// French
											L"Reagenzrotor in das Analysegerät einführen, Fach schließen und Bildschirmanweisungen befolgen.\n",								// German
											L"Inserte el rotor de reactivo en el analizador, cierre el cajón y siga las instrucciones de pantalla.\n",						// Spanish
											L"Inserire il rotore reagente nell'analizzatore, chiudere il cassetto e seguire le istruzioni a schermo.\n",						// Italian
											L"Insira o rotor de reagente no analisador, feche a gaveta e siga as instruções no ecrã.\n",										// Portuguese
											L"Insira o rotor de reagente no analisador, feche a gaveta e siga as instruções na tela.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_SEND				,	{	L"Printer: Print a copy of a species reference range.\n\n"
											L"Internal Archive: Save a copy of all active reference ranges internal to the analyzer for future recall.\n\n"
											L"External Archive: Transmit an ASCII dump of a species reference range to an external system.\n",																		// English
											L"Imprimante : imprimer un exemplaire de fourchette de référence d'espèces.\n\n"
											L"Archive interne : enregistrer un exemplaire de toutes les fourchettes de référence actives internes à l'analyseur afin de pouvoir les récupérer ultérieurement.\n\n"
											L"Archive externe : transmettre un vidage de mémoire ASCII d'une fourchette de référence d'espèces à un système externe.\n",												// French
											L"Drucker: Kopie eines Artenreferenzbereichs drucken.\n\n"
											L"Internes Archiv: Eine Kopie aller aktiven Referenzbereiche für spätere Verwendung intern im Analysegerät speichern.\n\n"
											L"Externes Archiv: ASCII-Auszug eines Artenreferenzbereichs an ein externes System übertragen.\n",																		// German
											L"Impresora: imprimir una copia de un intervalo de referencia de especies.\n\n"
											L"Archivo interno: guardar una copia de todos los intervalos de referencia activos internos para el analizador para usos futuros.\n\n"
											L"Archivo externo: hacer un volcado de datos ASCII de un intervalo de referencia de especies a un sistema externo.\n",													// Spanish
											L"Stampante: stampa una copia di un range di riferimento per specie.\n\n"
											L"Archivio interno: salva una copia di tutti i range di riferimento attivi interni all'analizzatore per utilizzo futuro.\n\n"
											L"Archivio esterno: trasmette una serie ASCII di range di riferimento per specie ad un sistema esterno.\n",																// Italian
											L"Impressora: Imprime uma cópia de um intervalo de referência de espécies.\n\n"
											L"Ficheiro interno: Guardar uma cópia de todos os intervalos de referência ativos internos no analisador para utilização futura.\n\n"
											L"Ficheiro externo: Transmitir um dump de ASCII de um intervalo de referência de espécies para um sistema externo.\n",													// Portuguese
											L"Impressora: Imprima uma cópia de um intervalo de referência de espécies.\n\n"
											L"Arquivo interno: Salve uma cópia de todos os intervalos de referência ativos internos no analisador, para futura recuperação.\n\n"
											L"Arquivo externo: Transmita um despejo ASCII de um intervalo de referência de espécies para um sistema externo.\n"														// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_ADD			,	{	L"Species n: The next available generic species label between 1 and 99.\n\n"
											L"Control n: The next available control label between 1 and 99.\n\n"
											L"Select: Provides a list of common species labels to select from.\n",																// English
											L"Espèce n : la prochaine étiquette d'espèce générique disponible comprise entre 1 et 99.\n\n"
											L"Contrôle n : la prochaine étiquette de contrôle disponible comprise entre 1 et 99.\n\n"
											L"Sélectionner : fournit une liste d'étiquettes d'espèces communes à partir de laquelle la sélection pourra être effectuée.\n",		// French
											L"Arten n: Das nächste verfügbare generische Artenetikett zwischen 1 und 99.\n\n"
											L"Steuerung n: Das nächste verfügbare Steuerungsetikett zwischen 1 und 99.\n\n"
											L"Auswählen: Stellt eine Liste üblicher Artenetiketten zur Auswahl.\n",																// German
											L"Especie n: la próxima etiqueta de especie genérica disponible entre 1 y 99.\n\n"
											L"Control n: la próxima etiqueta de control disponible entre 1 y 99.\n\n"
											L"Seleccionar: proporciona una lista para selección de etiquetas de especies comunes.\n",											// Spanish
											L"Specie n: la successiva etichetta di specie generica disponibile tra 1 e 99.\n\n"
											L"Controllo n: la successiva etichetta di controllo disponibile tra 1 e 99.\n\n"
											L"Seleziona: fornisce un elenco delle etichette delle specie comuni tra cui scegliere.\n",											// Italian
											L"Espécies n: A próxima etiqueta de espécies genérica disponível entre 1 e 99.\n\n"
											L"Controlo n: A próxima etiqueta de controlo disponível entre 1 e 99.\n\n"
											L"Selecionar: Fornece uma lista para seleção de etiquetas de espécies frequentes.\n",												// Portuguese
											L"N espécies: O próximo rótulo de espécie genérico disponível entre 1 e 99.\n\n"
											L"N controle: O próximo rótulo de controle disponível entre 1 e 99.\n\n"
											L"Selecionar: Fornece uma lista de rótulos de espécies comuns para seleção.\n"														// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_MODIFY		,	{	L"Analyte: Provides a list of all active reference ranges for a particular analyte.\n\n"
											L"Species: Provides a list of all analytes for all reference ranges.\n",																// English
											L"Substance à analyser : fournit une liste de toutes les fourchettes de référence actives pour une substance à analyser donnée.\n\n"
											L"Espèce : fournit une liste de toutes les substances à analyser pour toutes les fourchettes de référence.\n",						// French
											L"Analyt: Anzeige einer Liste aller aktiven Referenzbereiche für ein bestimmtes Analyt.\n\n"
											L"Arten: Anzeige einer Liste aller Analyte für alle Referenzbereiche.\n",															// German
											L"Analito: proporciona una lista de todos los intervalos de referencia activos para un analito particular.\n\n"
											L"Especie: proporciona una lista de todos los analitos para todos los intervalos de referencia.\n",									// Spanish
											L"Analita: fornisce un elenco di tutti i range di riferimento attivi per un particolare analita.\n\n"
											L"Specie: fornisce un elenco di tutti gli analiti per ogni range di riferimento.\n",													// Italian
											L"Analito: Fornece uma lista de todos os intervalos de referencia ativos para um analito específico.\n\n"
											L"Espécies: Fornece uma lista de todos os analitos para todos os intervalos de referência.\n",										// Portuguese
											L"Analito: Fornece uma lista de todos os intervalos de referência ativos para um determinado analito.\n\n"
											L"Espécies: Fornece uma lista de todos os analitos para todos os intervalos de referência.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_REMOVE		,	{	L"Removing a species from the sample type list will remove the item from the list of species shown when running a rotor, and any values entered will be permanently lost.\n",																	// English
											L"La suppression d'une espèce dans la liste de type d'échantillons a pour effet de supprimer cet élément dans la liste d'espèces affichées lors de l'exécution d'un rotor, et toutes les valeurs saisies seront irrémédiablement perdues.\n",	// French
											L"Die Entfernung einer Art aus der Probentypliste entfernt das Objekt aus der Artenliste, die bei laufendem Rotor angezeigt wird, und sämtliche eingegebene Werte gehen dauerhaft verloren.\n",													// German
											L"Al eliminar una especie de la lista del tipo de muestra se eliminará la entrada de la lista de especies mostrada al usar el rotor, y cualquier valor introducido se perderá permanentemente.\n",												// Spanish
											L"Rimuovendo una specie dall'elenco dei tipi si rimuoverà quella voce dall'elenco di specie proposto quando un rotore è in funzione e tutti i valori immessi saranno persi in modo permanente.\n",												// Italian
											L"Ao eliminar uma espécie da lista de tipo de amostra será eliminado o item da lista de espécies exibida quando se utiliza um rotor, e qualquer valor introduzido será permanentemente perdido.\n",												// Portuguese
											L"A remoção de uma espécie da lista de tipos de amostra removerá o item da lista de espécie mostrada durante a execução de um rotor e quaisquer valores inseridos serão permanentemente perdidos.\n"												// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_ENZYME_UNITS			,	{	L"Selecting a unit on this screen will change the units for ALT, ALP, AMY, AST, CK and GGT  simultaneously to the same units.\n",					// English
											L"L'unité sélectionnée dans cet écran sera appliquée simultanément à ALT, ALP, AMY, AST, CK et GGT.\n",												// French
											L"Mit der Auswahl einer Einheit in diesem Menü werden automatisch die Einheiten für ALT, ALP, AMY, AST, CK und GGT auf diese Einheit umgestellt.\n",	// German
											L"Al seleccionar una unidad en esta pantalla, se cambiará ALT, ALP, AMY, AST, CK y GGT simultáneamente a la misma unidad.\n",						// Spanish
											L"La selezione di un'unità in questa schermata modifica simultaneamente le unità per ALT, ALP, AMY, AST, CK e GGT nelle stesse unità.\n",			// Italian
											L"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades ALT, ALP, AMY, AST, CK e GGT para as mesmas unidades.\n",		// Portuguese
											L"A seleção de uma unidade nessa tela mudará as unidades para ALT, ALP, AMY, AST, CK e GGT simultaneamente para as mesmas unidades.\n"				// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_LIPID_UNITS			,	{	L"Selecting a unit on this screen will change the units for CHOL.\n",						// English
											L"L'unité sélectionnée dans cet écran sera appliquée à CHOL.\n",								// French
											L"Mit der Auswahl einer Einheit in diesem Menü wird die Einheit für CHOL umgestellt.\n",		// German
											L"Al seleccionar una unidad en esta pantalla, se cambiará la unidad de COL.\n",				// Spanish
											L"La selezione di un'unità in questa schermata modifica le unità per COL.\n",				// Italian
											L"Ao selecionar uma unidade neste ecrã, serão alteradas as unidades CHOL.\n",				// Portuguese
											L"A seleção de uma unidade nessa tela mudará as unidades para CHOL.\n"						// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_PROTEIN_UNITS		,	{	L"Selecting a unit on this screen will change the units for ALB, GLOB and TP simultaneously to the same units.\n",									// English
											L"L'unité sélectionnée dans cet écran sera appliquée simultanément à ALB, GLOB et TP.\n",															// French
											L"Mit der Auswahl einer Einheit in diesem Menü werden automatischdie Einheiten für ALB, GLOB und TP auf diese Einheit umgestellt.\n",				// German
											L"Al seleccionar una unidad en esta pantalla, se cambiará ALB, GLOB y TP simultáneamente a la misma unidad.\n",										// Spanish
											L"La selezione di un'unità in questa schermata modifica simultaneamente le unità per ALB e GLOB nelle stesse unità.\n",								// Italian
											L"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades ALB, GLOB e TP para as mesmas unidades.\n",						// Portuguese
											L"A seleção de uma unidade nessa mudará as unidades para ALB, GLOB e TP simultaneamente para as mesmas unidades.\n"									// Portuguese (Brazil)
										} 
	},
	
// Error screens							ENGLISH,							FRENCH,									GERMAN,								SPANISH,											ITALIAN, 									PORTUGUESE,								PORTUGUESE (BRAZIL)
	{ ESTR_ILLEGAL_ROTOR_TYPE		,	{	L"Illegal Rotor Type",				L"Type rotor non autorisé",				L"Falscher Rotortyp",				L"Tipo de rotor ilegal",							L"Tipo rotore illegale",					L"Tipo de rotor inválido",				L"Tipo de rotor ilegal"	}	},
	{ ESTR_NEW_ROTOR_TYPE			,	{	L"New Rotor Type",					L"Nouveau type de rotor",				L"Neuer Rotortyp",					L"Nuevo tipo de rotor",								L"Nuovo tipo rotore",						L"Tipo novo de rotor",					L"Novo tipo de rotor"	}	},
	{ ESTR_PLEASE_REMOVE_ROTOR		,	{	L"Please Remove Rotor",				L"Retirer le rotor",					L"Bitte Rotor entfernen",			L"Retire el rotor",									L"Si prega di rimuovere il rotore",			L"Retirar rotor",						L"Remova o rotor"	}	},
	{ ESTR_ROTOR_DATE_ILLEGAL		,	{	L"Rotor Date Illegal",				L"Date rotor non autorisée",			L"Falsches Rotorendatum",			L"Fecha de rotor ilegal",							L"Data rotore illegale",					L"Data do rotor inválida",				L"Data do rotor ilegal"	}	},
	{ ESTR_ROTOR_DATE_IMPROPER		,	{	L"Rotor Date Improper",				L"Date rotor incorrecte",				L"Unzulässiges Rotorendatum",		L"Fecha rotor no adecuada",							L"Data rotore impropria",					L"Data do rotor inadequada",			L"Data do rotor imprópria"	}	},
	{ ESTR_ROTOR_EXPIRED			,	{	L"Rotor Expired",					L"Expiration rotor",					L"Abgelaufener Rotor",				L"Rotor caducado",									L"Rotore scaduto",							L"Rotor expirado",						L"Rotor expirado"	}	},
	{ ESTR_UPDATED_ROTOR_TYPE		,	{	L"Updated Rotor Type",				L"Type de rotor actualisé",				L"Aktualisierter Rotortyp",			L"Tipo de rotor actualizado",						L"Tipo rotore aggiornato",					L"Tipo de rotor atualizado",			L"Tipo de rotor atualizado"	}	},

// Warning screens							ENGLISH,							FRENCH,									GERMAN,								SPANISH,											ITALIAN, 									PORTUGUESE,								PORTUGUESE (BRAZIL)
	{ ESTR_ROTOR_DETECTED			,	{	L"Rotor detected during warming",	L"Rotor détecté en cours de chauffage",	L"Rotor beim Aufwärmen vorgefunden",	L"Rotor detectado durante el calentamiento inicial",	L"Rotore individuato durante riscaldamento",	L"Rotor detetado durante o aquecimento",	L"Rotor detectado durante o aquecimento"	}	},

// Error info text
	{ EISTR_AMBIENT_TEMP			,	{	L"The temperature outside the analyzer is either too hot or too cold to run a rotor. Contact Technical Service for assistance.\n",															// English
											L"La température à l'extérieur de l'analyseur est trop élevée ou trop faible pour permettre le fonctionnement d'un rotor. Pour obtenir une assistance, contacter le service technique.\n",	// French
											L"Die Temperatur außerhalb des Analysegeräts ist für den Rotorenbetrieb zu heiß oder zu kalt. Wenden Sie sich an den technischen Service.\n",												// German
											L"La temperatura fuera del analizador es demasiado alta o demasiado baja para usar el rotor. Póngase en contacto con el servicio técnico.\n",												// Spanish
											L"La temperatura esterna all'analizzatore è troppo alta o troppo bassa per permettere il funzionamento di un rotore. Per assistenza contattare il servizio tecnico.\n",						// Italian
											L"A temperatura fora do analisador é demasiado alta ou demasiado baixa para executar um rotor. Contacte o Serviço de Assistência Técnica para obter assistência.\n",							// Portuguese
											L"A temperatura externa do analisador está muito alta ou muito baixa para executar um rotor. Entre em contato com o Serviço Técnico para obter assistência.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_ANALYZER_DATE			,	{	L"Date in analyzer is earlier than that of the rotor being run. Check dates in analyzer and rerun with new rotor.\n",											// English
											L"La date de l'analyseur est antérieure à celle du rotor en cours d'utilisation. Vérifier les dates sur l'analyseur et réexécuter le nouveau rotor.\n",			// French
											L"Das Datum im Analysegerät liegt vor dem des Rotorenbetriebs. Prüfen Sie das Datum im Analysegerät und nehmen Sie den Betrieb mit einem neuen Rotor auf.\n",	// German
											L"La fecha del analizador es anterior a la fecha del rotor que se va a usar. Compruebe las fechas del analizador y vuelva a usar el nuevo rotor.\n",				// Spanish
											L"La data dell'analizzatore è precedente a quella di attivazione del rotore. Controllare le date dell'analizzatore e riavviare con un nuovo rotore.\n",			// Italian
											L"A data do analisador é anterior à data do rotor que foi iniciado. Verifique as datas do analisador e reinicie com um novo rotor.\n",							// Portuguese
											L"A data no analisador é anterior à do rotor em execução. Verifique as datas no analisador e execute novamente com um novo rotor.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE					,	{	L"The analyzer was unable to read the rotor's barcode. If symptom continues, contact Technical Service.\n",														// English
											L"L'analyseur n'a pas pu lire le code à barres du rotor. En cas de persistance des symptômes, contacter le service technique.\n",								// French
											L"Das Analysegerät konnte den Barcode des Rotors nicht lesen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",						// German
											L"El analizador no pudo leer el código de barras del rotor. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",			// Spanish
											L"L'analizzatore non è riuscito a leggere il codice a barre del rotore. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"O analisador não consegue ler o código de barras do rotor. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",								// Portuguese
											L"O analisador não foi capaz de ler o código de barras do rotor. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE_FORMAT			,	{	L"The analyzer did not recognize the rotor. Contact Technical Service for further assistance.\n",						// English
											L"L'analyseur n'a pas reconnu le rotor. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											L"Das Analysegerät konnte den Rotor nicht erkennen. Wenden Sie sich an den technischen Service.\n",						// German
											L"El analizador no ha reconocido el rotor. Póngase en contacto con el servicio técnico.\n",								// Spanish
											L"L'analizzatore non ha riconosciuto il rotore. Per ulteriore assistenza contattare il servizio tecnico.\n",				// Italian
											L"O analisador não consegue reconhecer o rotor. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											L"O analisador não reconheceu o rotor. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MISSING			,	{	L"The analyzer did not detect all the chemistry beads in the rotor. Rerun sample using new rotor.\n",											// English
											L"L'analyseur n'a pas détecté toutes les billes chimiques dans le rotor. Réexécuter l'échantillon à l'aide d'un nouveau rotor.\n",				// French
											L"Das Analysegerät konnte nicht alle chemischen Beads in dem Rotor erkennen. Wiederholen Sie die Probe mit einem neuen Rotor.\n",				// German
											L"El analizador no detectó todas las cuentas químicas en el rotor. Vuelva a utilizar la muestra usando un nuevo rotor.\n",						// Spanish
											L"L'analizzatore non ha individuato tutti i granuli chimici nel rotore. Riavviare il campione con un nuovo rotore.\n",							// Italian
											L"O analisador não detetou todas as esferas de substâncias químicas no rotor. Executar novamente a amostra utilizando um rotor novo.\n",			// Portuguese
											L"O analisador não detectou todas as esferas de substância química no rotor. Execute novamente a amostra usando o novo rotor.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MIX				,	{	L"The analyzer will not be able to determine the exposure status if the QC beads have not mixed correctly. Rerun with a new rotor.\n",									// English
											L"L'analyseur ne parviendra pas à déterminer l'état d'exposition si les billes QC ne sont pas correctement mélangées. Réexécuter avec un nouveau rotor.\n",				// French
											L"Das Analysegerät kann den Status der Einwirkung nicht feststellen, wenn die QC-Beads nicht ordnungsgemäß vermischt wurden. Mit einem neuen Rotor wiederholen.\n",		// German
											L"El analizador no será capaz de determinar el estado de exposición si las cuentas QC no se mezclan correctamente. Debe usarse con un nuevo rotor.\n",					// Spanish
											L"L'analizzatore non potrà determinare lo stato di esposizione se i granuli di QC non sono stati mescolati correttamente. Riavviare con un nuovo rotore.\n",				// Italian
											L"O analisador não conseguirá determinar o estado de exposição se as esferas de CQ não forem misturadas adequadamente. Executar novamente utilizando um rotor novo.\n",	// Portuguese
											L"O analisador não poderá determinar o status de exposição se as esferas de QC não tiverem sido misturadas corretamente. Execute novamente com um novo rotor.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_CUVETTE_MARK			,	{	L"The analyzer was unable to detect the rotor. If symptom continues, contact Technical Service.\n",												// English
											L"L'analyseur n'est pas parvenu à détecter le rotor. En cas de persistance des symptômes, contacter le service technique.\n",					// French
											L"Das Analysegerät konnte den Rotor nicht finden. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",					// German
											L"El analizador no fue capaz de detectar el rotor. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"L'analizzatore non è riuscito a individuare il rotore. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"O analisador não consegue detetar o rotor. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",								// Portuguese
											L"O analisador não foi capaz de detectar o rotor. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_EXPIRED_ROTOR			,	{	L"The rotor has expired and should not be used. Contact Technical Service for further details.\n",												// English
											L"La date de péremption du rotor est dépassée. Ne pas l'utiliser. Pour obtenir plus d'informations, contacter le service technique.\n",			// French
											L"Der Rotor ist abgelaufen und sollte ersetzt werden. Wenden Sie sich an den technischen Service.\n",											// German
											L"El rotor ha caducado y no debería utilizarse. Póngase en contacto con el servicio técnico.\n",													// Spanish
											L"Il rotore è scaduto e non dovrebbe essere usato. Per ulteriore dettagli contattare il servizio tecnico.\n",									// Italian
											L"O rotor expirou e não deveria ser utilizado. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",						// Portuguese
											L"O rotor expirou e não deve ser usado. Entre em contato com o Serviço Técnico para obter detalhes adicionais.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_DILUENT	,	{	L"Insufficient reagents exist in the rotor to complete the run. Rerun sample with new rotor.\n",													// English
											L"La quantité de réactifs dans le rotor est insuffisante pour exécuter la procédure. Réexécuter l'échantillon à l'aide du nouveau rotor.\n",		// French
											L"In dem Rotor befinden sich nicht ausreichend Reagenzien, um den Vorgang abzuschließen. Wiederholen Sie die Probe mit einem neuen Rotor.\n",	// German
											L"No hay suficientes reactivos en el rotor para completar su uso. Vuelva a utilizar la muestra con un nuevo rotor.\n",							// Spanish
											L"Nel rotore non vi sono sufficienti reagenti per completare l'operazione. Riavviare il campione con un nuovo rotore.\n",						// Italian
											L"Não existem reagentes suficientes no rotor para concluir a execução. Executar novamente a amostra utilizando um rotor novo.\n",				// Portuguese
											L"Existem reagentes insuficientes no rotor para concluir a execução. Execute novamente a amostra com o novo rotor.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_SAMPLE		,	{	L"The rotor has not received enough sample to function properly. Rerun using a new rotor.\n",													// English
											L"Le rotor n'a pas reçu suffisamment d'échantillon pour fonctionner correctement. Réexécuter avec un nouveau rotor.\n",							// French
											L"Der Rotor enthält keine ausreichende Probenmenge für eine ordnungsgemäße Funktion. Mit einem neuen Rotor wiederholen.\n",						// German
											L"El rotor no ha recibido suficiente muestra para funcionar correctamente. Úselo con un nuevo rotor.\n",											// Spanish
											L"Il rotore non ha ricevuto campione sufficiente per funzionare nel modo corretto. Riavviare utilizzando un nuovo rotore.\n",					// Italian
											L"O rotor não recebeu amostra suficiente para funcionar adequadamente. Executar novamente utilizando um rotor novo.\n",							// Portuguese
											L"O rotor não recebeu amostra suficiente para funcionar corretamente. Execute novamente usando um novo rotor.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_IQC						,	{	L"The analyzer Intelligent Quality Control (iQC) reagent dye beads differ too much from the expected values. Rerun using a new rotor.\n",												// English
											L"Les billes de teinture du réactif Intelligent Quality Control (iQC) divergent trop des valeurs prévues. Réexécuter avec un nouveau rotor.\n",											// French
											L"Die Intelligent Quality Control (iQC) Reagenz-Farbstoff-Beads des Analysegeräts weichen zu stark von den erwarteten Werten ab. Mit einem neuen Rotor wiederholen.\n",					// German
											L"Las cuentas de tinte de reactivo Intelligent Quality Control (iQC) del analizador son muy distintas a los valores esperados. Úselo con un nuevo rotor.\n",								// Spanish
											L"I granuli di colore reagente Intelligent Quality Control (iQC) dell'analizzatore sono troppo diversi dai valori previsti. Riavviare utilizzando un nuovo rotore.\n",					// Italian
											L"As esferas de reagente corante do Controlo de qualidade inteligente (iQC) do analisador são muito diferentes dos valores esperados. Executar novamente utilizando um rotor novo.\n",	// Portuguese
											L"As esferas do corante reagente de Controle de qualidade inteligente (iQC) do analisador diferem muito dos valores esperados. Execute novamente usando um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_REAGENT_DISTRIBUTION	,	{	L"Reagent beads have not dissolved as expected. Rerun sample with a new rotor.\n",											// English
											L"Les billes de réactif ne se sont pas dissoutes comme prévu. Réexécuter l'échantillon à l'aide d'un nouveau rotor.\n",		// French
											L"Reagenz-Beads wurden nicht erwartungsgemäß aufgelöst. Wiederholen Sie die Probe mit einem neuen Rotor.\n",					// German
											L"Las cuentas de reactivo no se han disuelto como se esperaba. Vuelva a utilizar la muestra con un nuevo rotor.\n",			// Spanish
											L"I granuli di reagente non si sono dissolti come previsto. Riavviare il campione con un nuovo rotore.\n",					// Italian
											L"As esferas de reagente não se dissolveram conforme esperado. Executar novamente a amostra utilizando um rotor novo.\n",	// Portuguese
											L"As esferas de reagente não se dissolveram conforme esperado. Execute novamente a amostra com um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_ROTOR_DETECTED			,	{	L"A rotor may be in the analyzer and may prevent the analyzer from starting normally. Press Continue to remove the rotor.\n",								// English
											L"Il peut y avoir dans l'analyseur un rotor susceptible de l'empêcher de démarrer normalement. Appuyer sur Continuer pour retirer le rotor.\n",				// French
											L"Ein Rotor kann sich in dem Analysegerät befinden und den Startvorgang des Analysegeräts beeinträchtigen. Zur Entfernung des Rotors drücken Sie Weiter.\n",	// German
											L"Puede que haya un rotor en el analizador, lo que impide que el analizador se inicie correctamente. Pulse en Continuar para retirar el rotor.\n",			// Spanish
											L"Nell'analizzatore potrebbe essere rimasto un rotore che impedisce un'attivazione normale dell'analizzatore. Premere Continua per rimuovere il rotore.\n", 	// Italian
											L"Pode estar um rotor no analisador, o que pode impedir que o analisador inicie corretamente. Prima Continuar para remover o rotor.\n",						// Portuguese
											L"Um rotor pode estar no analisador e impedir que o analisador inicie normalmente. Pressione Continuar para remover o rotor.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_RQC						,	{	L"The Reagent QC beads recovered below the limit. The rotor may not have been stored correctly. Rerun with a new rotor.\n",													// English
											L"La récupération des billes de réactif QC est inférieure à la limite. Il se peut que le rotor n'ait pas été rangé correctement. Réexécuter avec un nouveau rotor.\n",		// French
											L"Die wiederhergestellten Reagenz-QC-Beads liegen unter dem Grenzwert. Der Rotor wurde möglicherweise nicht ordnungsgemäß gelagert. Mit einem neuen Rotor wiederholen.\n",	// German
											L"Cuentas QC de reactivo recuperadas por debajo del límite. Puede que el rotor no se haya guardado correctamente. Debe usarse con un nuevo rotor.\n",						// Spanish
											L"I granuli QC di reagente si sono ristabiliti sotto al limite. Il rotore potrebbe non essere stato conservato correttamente. Riavviare con un nuovo rotore.\n",			   	// Italian
											L"As esferas de reagente de CQ foram recuperadas abaixo do limite. O rotor pode não ter sido guardado corretamente. Executar novamente utilizando um rotor novo.\n",			// Portuguese
											L"As esferas de Reagente de QC se recuperaram abaixo do limite. O rotor pode não ter sido armazenado corretamente. Execute novamente com um novo rotor.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_BLANK			,	{	L"The sample blank bead appears to be turbid. This can be due to sample quality. Confirm sample integrity and rerun with a new rotor.\n",														// English
											L"La bille vierge de l'échantillon semble trouble, ce qui peut venir de la qualité de l'échantillon. Vérifier l'intégrité de l'échantillon et réexécuter avec un nouveau rotor.\n",				// French
											L"Die Leer-Beadprobe ist trübe. Dies kann auf die Probenqualität zurückgeführt werden. Bestätigen Sie die Unversehrtheit der Probe und wiederholen Sie den Vorgang mit einem neuen Rotor.\n",	// German
											L"La cuenta virgen de la muestra parece estar turbia. Esto puede deberse a la calidad de la muestra. Confirme la integridad de la muestra y vuelva a utilizar el nuevo rotor.\n",				// Spanish
											L"I granuli campione vuoti sembrano torbidi. Ciò può essere causato dalla qualità del campione. Confermare l'integrità del campione e riavviare con un nuovo rotore.\n",							// Italian
											L"A esfera de amostra de branco parece estar turva. Isto pode ser devido à qualidade da amostra. Confirme a integridade da amostra e execute novamente utilizando um rotor novo.\n",				// Portuguese
											L"A esfera em branco da amostra parece estar turva. Isso pode ser decorrente da qualidade da amostra. Confirme a integridade da amostra e execute novamente com um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_DISTRIBUTION		,	{	L"Sample has not reached the chemistries. Rerun with a new rotor.\n",									// English
											L"L'échantillon n'est pas parvenu jusqu'aux produits chimiques. Réexécuter avec un nouveau rotor.\n",	// French
											L"Die Probe hat nicht die Chemie erreicht. Mit einem neuen Rotor wiederholen.\n",						// German
											L"La muestra no ha alcanzado los compuestos químicos. Debe usarse con un nuevo rotor.\n",				// Spanish
											L"Il campione non ha raggiunto le sostanze chimiche. Riavviare con un nuovo rotore.\n",					// Italian
											L"A amostra não atingiu as substâncias químicas. Executar novamente utilizando um rotor novo.\n",		// Portuguese
											L"A amostra não atingiu as substâncias químicas. Execute novamente com um novo rotor.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_MIX				,	{	L"The sample has not mixed with reagent materials sufficiently. Rerun with a new rotor.\n",										// English
											L"L'échantillon ne s'est pas suffisamment mélangé avec les matières de réactifs. Réexécuter avec un nouveau rotor.\n",			// French
											L"Die Probe hat sich nicht ausreichend mit Reagenzmaterialien vermischt. Mit einem neuen Rotor wiederholen.\n",					// German
											L"La muestra no se ha mezclado suficientemente con los materiales reactivos. Debe usarse con un nuevo rotor.\n",					// Spanish
											L"Il campione non si è mescolato a sufficienza con le sostanze reagenti. Riavviare con un nuovo rotore.\n",						// Italian
											L"A amostra não se misturou suficientemente com os materiais dos reagentes. Executar novamente utilizando um rotor novo.\n",		// Portuguese
											L"A amostra não foi misturada com os materiais reagentes suficientemente. Execute novamente com um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_QUALITY			,	{	L"The sample may have one or more physical interferents. Confirm the sample integrity and rerun rotor.\n",										// English
											L"Il se peut que l'échantillon présente un ou plusieurs éléments interférents. Vérifier l'intégrité de l'échantillon et réexécuter le rotor.\n",	// French
											L"Die Probe hat möglicherweise eine oder mehrere physische störende Bestandteile. Unversehrtheit der Probe bestätigen und Rotor neu starten.\n",	// German
											L"Puede que la muestra tenga una o varias interferencias físicas. Confirme la integridad de la muestra y vuelva a usar el rotor.\n",				// Spanish
											L"Il campione potrebbe avere uno o più fattori di interferenza fisica. Confermare l'integrità del campione e riavviare il rotore.\n",			// Italian
											L"A amostra pode ter uma ou várias interferências físicas. Confirme a integridade da amostra e execute novamente o rotor.\n",					// Portuguese
											L"A amostra pode ter um ou mais interferentes físicos. Confirme a integridade da amostra e execute novamente o rotor.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_SOFTWARE_UPDATE			,	{	L"The rotor inserted requires a software update to operate correctly. Contact Technical Service for further assistance.\n",														// English
											L"Le rotor inséré nécessite une mise à jour de logiciel pour pouvoir fonctionner correctement. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											L"Der eingesetzte Rotor erfordert ein Software-Update für einen ordnungsgemäßen Betrieb. Wenden Sie sich an den technischen Service.\n",											// German
											L"El rotor insertado necesita la actualización del software para operar correctamente. Póngase en contacto con el servicio técnico.\n",											// Spanish
											L"Il rotore inserito richiede un aggiornamento del software per funzionare correttamente. Per ulteriore assistenza contattare il servizio tecnico.\n",							// Italian
											L"O rotor inserido necessita de uma atualização do software para funcionar adequadamente. Contacte o Serviço de Assistência Técnica para obter assistência.\n",					// Portuguese
											L"O rotor inserido requer uma atualização de software para funcionar corretamente. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_STARTING_ABSORBANCE		,	{	L"Starting absorbance of the sample blank bead is outside the limit, possibly due to an exogenous substance or distribution error. Rerun with new rotor.\n",																		// English
											L"L'absorbance de départ de la bille vierge de l'échantillon n'entre pas dans les limites prescrites, éventuellement du fait d'une substance exogène ou d'une erreur de distribution. Réexécuter à l'aide du nouveau rotor.\n",	// French
											L"Die Startabsorbierung der Leer-Beadprobe befindet sich möglicherweise aufgrund einer exogenen Substanz oder eines Verteilungsfehlers außerhalb des Grenzwerts. Mit einem neuen Rotor wiederholen.\n",							// German
											L"La absorbancia de inicio de la cuenta virgen de muestra está fuera del límite, probablemente debido a una sustancia exógena o a un error de distribución. Usar con un nuevo rotor.\n",											// Spanish
											L"L'assorbenza iniziale dei granuli campione vuoti non rientra nei limiti, forse a causa di una sostanza esogena od a un errore di distribuzione. Riavviare il campione con un nuovo rotore.\n",									// Italian
											L"A absorvância inicial da esfera de amostra de branco está fora do limite, possivelmente devido a uma sustância exógena ou erro de distribuição. Executar novamente utilizando um rotor novo.\n",								// Portuguese
											L"A absorção inicial da esfera em branco da amostra está fora do limite, possivelmente devido a uma substância exógena ou um erro de distribuição. Execute novamente com um novo rotor.\n"										// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_SENSOR				,	{	L"The rotor temperature sensor is not operating. Contact Technical Service for further assistance.\n",										// English
											L"Le détecteur de température du rotor ne fonctionne pas. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											L"Der Temperatursensor des Rotors ist außer Betrieb. Wenden Sie sich an den technischen Service.\n",											// German
											L"El sensor de temperatura del rotor no está funcionando. Póngase en contacto con el servicio técnico.\n",									// Spanish
											L"Il sensore di temperatura del rotore non è in funzione. Per ulteriore assistenza contattare il servizio tecnico.\n",						// Italian
											L"O sensor de temperatura do rotor não está a funcionar. Contacte o Serviço de Assistência Técnica para obter assistência.\n",				// Portuguese
											L"O sensor de temperatura do rotor não está funcionando. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_VARIATION			,	{	L"The temperature deviated more than one degree during the run. The accuracy of certain chemistries may be affected. Rerun with new rotor.\n",															// English
											L"La température a présenté un écart de plus d'un degré au cours de l'exécution. La précision de certains produits chimiques pourra en être affectée. Réexécuter à l'aide du nouveau rotor.\n",			// French
											L"Der Temperatur wich um mehr als ein Grad während des Betriebs ab. Die Genauigkeit bestimmter Chemie kann beeinträchtigt worden sein. Mit einem neuen Rotor wiederholen.\n",							// German
											L"La temperatura cambió en más de un grado durante el análisis. Puede que afecte a la precisión del análisis de ciertas sustancias químicas. Utilice un nuevo rotor.\n",									// Spanish
											L"Durante il funzionamento la temperatura è variata di più di un grado. La precisione di alcune sostanze chimiche potrebbe esserne stata influenzata. Riavviare il campione con un nuovo rotore.\n",		// Italian
											L"A alteração da temperatura foi superior a um grau durante a execução. A precisão de determinadas sustâncias químicas pode ser afetada. Executar novamente utilizando um rotor novo.\n",				// Portuguese
											L"A temperatura desviou mais de um grau durante a execução. A precisão de certas substâncias químicas pode estar afetada. Execute novamente com um novo rotor.\n"										// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMPERATURE				,	{	L"The temperature of the rotor either varied too much or was not in the proper range during the run. If symptom continues, contact Technical Service.\n",																			// English
											L"La température du rotor a soit varié dans des proportions trop importantes, soit ne s'inscrit pas dans la fourchette prescrite en cours d'exécution. En cas de persistance des symptômes, contacter le service technique.\n",		// French
											L"Die Rotortemperatur variierte zu stark oder befand sich während des Betriebs außerhalb des optimalen Bereichs. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",											// German
											L"La temperatura del rotor varió mucho o no estaba en el intervalo adecuado durante el análisis. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",											// Spanish
											L"Durante il funzionamento la temperatura del rotore ha subito variazioni eccessive oppure non era nel range adatto. Se i sintomi continuano contattare il servizio tecnico.\n",														// Italian
											L"A temperatura do rotor variou muito ou não esteve dentro do intervalo adequado durante a análise. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",															// Portuguese
											L"A temperatura do rotor variou muito ou não permaneceu no intervalo adequado durante a execução. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"																	// Portuguese (Brazil)
										} 
	},
	{ EISTR_WRONG_ROTOR				,	{	L"The rotor inserted is not intended for this device. If symptom continues, contact Technical Service.\n",										// English
											L"Le rotor inséré n'est pas prévu pour cet appareil. En cas de persistance des symptômes, contacter le service technique.\n",					// French
											L"Der eingesetzte Rotor ist für dieses Gerät nicht geeignet. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",			// German
											L"El rotor insertado no es apto para este aparato. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											L"Il rotore inserito non è adatto a questo strumento. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											L"O rotor inserido não é adequado para este dispositivo. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",					// Portuguese
											L"O rotor inserido não é destinado a este dispositivo. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"						// Portuguese (Brazil)
										} 
	},
	
// Results (display and print)				ENGLISH,				FRENCH,					GERMAN,					SPANISH,					ITALIAN, 					PORTUGUESE,						PORTUGUESE (BRAZIL)
	{ RSTR_ROTOR_LOT_NUM			,	{	L"Rotor Lot Number:",	L"N° de lot rotor :",	L"Rotor-Losnr.:",		L"No. de lote del rotor:",	L"Numero lotto rotore:",	L"Número lote rotor:",			L"Núm. de lote do rotor:"	}	},
	{ RSTR_DOV						,	{	L"DOV:",				L"DOV :",				L"LI:",					L"Úl. vac.:",				L"DV:",						L"DOV:",						L"Data vacinação:"	}	},
	{ RSTR_KENNEL_NUM				,	{	L"Kennel Number:",		L"Numéro chenil :",		L"Zwingernr.:",			L"No. de caseta:",			L"Numero Kennel:",			L"Número de Kennel:",			L"Número do canil:"	}	},
	{ RSTR_OWNER_ID					,	{	L"Owner ID:",			L"ID prop.:",			L"Besitzer-ID:",		L"Id. del dueño:",			L"ID propr.:",				L"ID do propr.:",				L"ID propriet.:"	}	},
	
// Genders									ENGLISH,	FRENCH,		GERMAN,			SPANISH,		ITALIAN, 	   	PORTUGUESE,		PORTUGUESE (BRAZIL)
	{ GSTR_FEMALE					,	{	L"Female",	L"Femelle",	L"Weiblich",	L"Hembra",		L"Femmina",		L"Fêmea",		L"Sexo feminino"	}	},
	{ GSTR_MALE						,	{	L"Male",	L"Mâle",	L"Männlich",	L"Macho",		L"Maschio",		L"Macho",		L"Sexo masculino"	}	},

// Species									ENGLISH,				FRENCH,					GERMAN,					SPANISH,				ITALIAN, 				PORTUGUESE,				PORTUGUESE (BRAZIL)	
	{ SSTR_OTHER					,	{	L"Other",				L"Autre",				L"Sonstige",			L"Otro",				L"Altro",				L"Outro",				L"Outros"	}	},
	{ SSTR_SPECIES					,	{	L"Species",				L"Espèces",				L"Arten",				L"Especie",				L"Specie",				L"Espécies",			L"Espécies"	}	},	
	{ SSTR_CANINES					,	{	L"Canines",				L"Canins",				L"Hunde",				L"Caninos",				L"Canini",				L"Caninas",				L"Caninos"	}	},
	{ SSTR_FELINES					,	{	L"Felines",				L"Félins",				L"Katzen",				L"Felinos",				L"Felini",				L"Felinas",				L"Felinos"	}	},
	{ SSTR_EQUINES					,	{	L"Equines",				L"Chevalins",			L"Pferde",				L"Equinos",				L"Equini",				L"Equinas",				L"Equinos"	}	},
	{ SSTR_BOVINES					,	{	L"Bovines",				L"Bovins",				L"Rinder",				L"Bovinos",				L"Bovini",				L"Bovinas",				L"Bovinos"	}	},
	{ SSTR_AVIANS					,	{	L"Avians",				L"Aviaires",			L"Vögel",				L"Aves",				L"Aviari",				L"Aviárias",			L"Aves"	}	},
	{ SSTR_REPTILES_AMPHIBIANS		,	{	L"Reptiles/Amphibians",	L"Reptiles/Amphibies",	L"Reptilien/Amphibien",	L"Reptiles/anfibios",	L"Rettili/Anfibi",		L"Répteis/Anfibios",	L"Répteis/anfíbios"	}	},
	{ SSTR_SMALL_MAMMALS			,	{	L"Small Mammals",		L"Petits mammifères",	L"Kleinsäugetiere",		L"Pequeños mamíferos",	L"Piccoli mammiferi",	L"Pequenos mamíferos",	L"Mamíferos pequenos"	}	},
	{ SSTR_LARGE_MAMMALS			,	{	L"Large Mammals",		L"Grands mammifères",	L"Großsäugetiere",		L"Grandes mamíferos",	L"Grandi mammiferi",	L"Grandes mamíferos",	L"Mamíferos grandes"	}	},

	// Canines
	{ SSTR_DOG						,	{	L"Dog",					L"Chien",				L"Hund",				L"Perro",				L"Cane",				L"Cão",					L"Cão"	}	},
	{ SSTR_COYOTE					,	{	L"Coyote",				L"Coyote",				L"Kojote",				L"Coyote",				L"Coyote",				L"Coiote",				L"Coiote"	}	},
	{ SSTR_DINGO					,	{	L"Dingo",				L"Dingo",				L"Dingo",				L"Dingo",				L"Dingo",				L"Dingo",				L"Cão selvagem"	}	},
	{ SSTR_FOX						,	{	L"Fox",					L"Renard",				L"Fuchs",				L"Zorro",				L"Volpe",				L"Raposa",				L"Raposa"	}	},
	{ SSTR_JACKAL					,	{	L"Jackal",				L"Chacal",				L"Schakal",				L"Chacal",				L"Sciacallo",			L"Chacal",				L"Chacal"	}	},
	{ SSTR_WOLF						,	{	L"Wolf",				L"Loup",				L"Wolf",				L"Lobo",				L"Lupo",				L"Lobo",				L"Lobo"	}	},

	// Felines
	{ SSTR_CAT						,	{	L"Cat",					L"Chat",				L"Katze",				L"Gato",				L"Gatto",				L"Gato",				L"Gato"	}	},
	{ SSTR_BOBCAT					,	{	L"Bobcat",				L"Lynx roux",			L"Rotluchs",			L"Lince rojo",			L"Lince rossa",			L"Lince vermelho",		L"Lince"	}	},
	{ SSTR_CHEETAH					,	{	L"Cheetah",				L"Guépard",				L"Gepard",				L"Guepardo",			L"Ghepardo",			L"Chita",				L"Gato selvagem"	}	},
	{ SSTR_JAGUAR					,	{	L"Jaguar",				L"Jaguar",				L"Jaguar",				L"Jaguar",				L"Giaguaro",			L"Jaguar",				L"Jaguar"	}	},
	{ SSTR_LEOPARD					,	{	L"Leopard",				L"Léopard",				L"Leopard",				L"Leopardo",			L"Leopardo",			L"Leopardo",			L"Leopardo"	}	},
	{ SSTR_LION						,	{	L"Lion",				L"Lion",				L"Löwe",				L"León",				L"Leone",				L"Leão",				L"Leão"	}	},
	{ SSTR_LYNX						,	{	L"Lynx",				L"Lynx",				L"Luchs",				L"Lince",				L"Lince",				L"Lince",				L"Gato selvagem"	}	},
	{ SSTR_PANTHER					,	{	L"Panther",				L"Panthère",			L"Panther",				L"Pantera",				L"Pantera",				L"Pantera",				L"Pantera"	}	},
	{ SSTR_PUMA						,	{	L"Puma",				L"Puma",				L"Puma",				L"Puma",				L"Puma",				L"Puma",				L"Puma"	}	},
	{ SSTR_TIGER					,	{	L"Tiger",				L"Tigre",				L"Tiger",				L"Tigre",				L"Tigre",				L"Tigre",				L"Tigre"	}	},
	
	// Equines
	{ SSTR_HORSE					,	{	L"Horse",				L"Cheval",				L"Pferd",				L"Caballo",				L"Cavallo",				L"Cavalo",				L"Cavalo"	}	},
	{ SSTR_DONKEY					,	{	L"Donkey",				L"Ane",					L"Esel",				L"Burro",				L"Asino",				L"Burro",				L"Asno"	}	},
	{ SSTR_PONY						,	{	L"Pony",				L"Poney",				L"Pony",				L"Poni",				L"Pony",				L"Pónei",				L"Pônei"	}	},
	{ SSTR_ZEBRA					,	{	L"Zebra",				L"Zèbre",				L"Zebra",				L"Cebra",				L"Zebra",				L"Zebra",				L"Zebra"	}	},
	
	// Bovines
	{ SSTR_COW						,	{	L"Cow",					L"Vache",				L"Kuh",					L"Vaca",				L"Mucca",				L"Vaca",					L"Vaca"	}	},
	{ SSTR_WATER_BUFFALO			,	{	L"Water Buffalo",		L"Buffle (d'Asie)",		L"Wasserbüffel",		L"Búfalo de agua",		L"Bufalo d'acqua",		L"Búfalo de água",		L"Búfalo d´água"	}	},
	{ SSTR_STEER					,	{	L"Steer",				L"Bouvillon",			L"Stier",				L"Novillo",				L"Manzo",				L"Boi",					L"Novilho"	}	},
	{ SSTR_OX						,	{	L"Ox",					L"Boeuf",				L"Ochse",				L"Buey",				L"Bue",					L"Boi",					L"Boi"	}	},
	{ SSTR_BULL						,	{	L"Bull",				L"Taureau",				L"Bulle",				L"Toro",				L"Toro",				L"Touro",				L"Touro"	}	},
	{ SSTR_BUFFALO					,	{	L"Buffalo",				L"Buffle",				L"Büffel",				L"Búfalo",				L"Bufalo",				L"Búfalo",				L"Búfalo"	}	},
	{ SSTR_BISON					,	{	L"Bison",				L"Bison",				L"Bison",				L"Bisonte",				L"Bisonte",				L"Bisonte",				L"Bisão"	}	},
	
	// Avians
	{ SSTR_BIRD						,	{	L"Bird",				L"Oiseau",				L"Vogel",				L"Pájaro",				L"Uccello",				L"Pássaro",				L"Pássaro"	}	},
	{ SSTR_AFRICAN_GRAY				,	{	L"African Gray",		L"Perroquet gris",		L"Afrik.Graupapagei",	L"Rucio africano",		L"Pap. grigio afr.",	L"Papagaio africano",	L"Papagaio africano"	}	},
	{ SSTR_AMAZON					,	{	L"Amazon",				L"Amazone",				L"Amazone",				L"Amazona",				L"Amazzoni",			L"Amazonas",			L"Amazona"	}	},
	{ SSTR_CHICKEN					,	{	L"Chicken",				L"Poulet",				L"Huhn",				L"Pollo",				L"Pollo",				L"Galinha",				L"Frango"	}	},
	{ SSTR_COCKATIEL				,	{	L"Cockatiel",			L"Calopsitte élégte",	L"Nymphensittich",		L"Carolina",			L"Cockatiel",			L"Caturra",				L"Calopsita"	}	},
	{ SSTR_COCKATOO					,	{	L"Cockatoo",			L"Cacatoès",			L"Kakadu",				L"Cacatúa",				L"Cacatua",				L"Catatua",				L"Cacatua"	}	},
	{ SSTR_CONURE					,	{	L"Conure",				L"Conure",				L"Sittiche",			L"Lorito americano",	L"Conuro",				L"Conure",				L"Jandaia"	}	},
	{ SSTR_DOVE						,	{	L"Dove",				L"Colombe",				L"Taube",				L"Paloma",				L"Colombo",				L"Pomba",				L"Pombo"	}	},
	{ SSTR_DUCK						,	{	L"Duck",				L"Canard",				L"Ente",				L"Pato",				L"Anatra",				L"Pato",				L"Pato"	}	},
	{ SSTR_FLAMINGO					,	{	L"Flamingo",			L"Flamand",				L"Flamingo",			L"Flamenco",			L"Fenicottero",			L"Flamingo",			L"Flamingo"	}	},
	{ SSTR_GOOSE					,	{	L"Goose",				L"Oie",					L"Gans",				L"Ganso",				L"Oca",					L"Ganso",				L"Ganso"	}	},
	{ SSTR_LOVEBIRD					,	{	L"Lovebird",			L"Inséparable",			L"Erdbeerkopf",			L"Inseparable",			L"Inseparabile",		L"Agapornis",			L"Pássaro-do-amor"	}	},
	{ SSTR_MACAW					,	{	L"Macaw",				L"Ara",					L"Ara",					L"Guacamayo",			L"Ara",					L"Arara",				L"Arara"	}	},
	{ SSTR_MYNAH					,	{	L"Mynah",				L"Mainate",				L"Beo",					L"Grácula religiosa",	L"Storno triste",		L"Mainá",				L"Mainá"	}	},
	{ SSTR_OSTRICH					,	{	L"Ostrich",				L"Autruche",			L"Strauß",				L"Avestruz",			L"Struzzo",				L"Avestruz",			L"Avestruz"	}	},
	{ SSTR_PARAKEET					,	{	L"Parakeet",			L"Starique perroq.",	L"Wellensittich",		L"Periquito",			L"Parrocchetto",		L"Periquito",			L"Periquito"	}	},
	{ SSTR_PARROT					,	{	L"Parrot",				L"Perroquet",			L"Papagei",				L"Loro",				L"Pappagallo",			L"Papagaio",			L"Papagaio"	}	},
	{ SSTR_TOUCAN					,	{	L"Toucan",				L"Toucan",				L"Tukan",				L"Tucán",				L"Tucano",				L"Tucano",				L"Tucano"	}	},
	
	// Reptiles/amphibians
	{ SSTR_REPTILE					,	{	L"Reptile",				L"Reptile",				L"Reptil",				L"Reptil",				L"Rettile",				L"Réptil",				L"Réptil"	}	},
	{ SSTR_ALLIGATOR				,	{	L"Alligator",			L"Alligator",			L"Alligator",			L"Caimán",				L"Alligatore",			L"Aligátor",			L"Jacaré"	}	},
	{ SSTR_AMPHIBIAN				,	{	L"Amphibian",			L"Amphibie",			L"Amphibie",			L"Anfibio",				L"Anfibio",				L"Anfíbio",				L"Anfíbio"	}	},
	{ SSTR_BEARDED_DRAGON			,	{	L"Bearded Dragon",		L"Lézard à barbe",		L"Bartagame",			L"Dragón barbado",		L"Pogona vitticeps",	L"Dragão barbudo",		L"Dragão farpado"	}	},
	{ SSTR_BOA_CONSTRICTOR			,	{	L"Boa Constrictor",		L"Boa constrictor",		L"Boa constrictor",		L"Boa constrictor",		L"Boa Costrittore",		L"Jibóia",				L"Jiboia"	}	},
	{ SSTR_CHAMELEON				,	{	L"Chameleon",			L"Caméléon",			L"Chamäleon",			L"Camaleón",			L"Camaleonte",			L"Camaleão",			L"Camaleão"	}	},
	{ SSTR_CROCODILE				,	{	L"Crocodile",			L"Crocodile",			L"Krokodil",			L"Cocodrilo",			L"Coccodrillo",			L"Crocodilo",			L"Crocodilo"	}	},
	{ SSTR_FROG						,	{	L"Frog",				L"Grenouille",			L"Frosch",				L"Rana",				L"Rana",				L"Rã",					L"Rã"	}	},
	{ SSTR_GECKO					,	{	L"Gecko",				L"Gecko",				L"Gecko",				L"Lagartija",			L"Geco",				L"Osga",				L"Lagartixa"	}	},
	{ SSTR_IGUANA					,	{	L"Iguana",				L"Iguane",				L"Leguan",				L"Iguana",				L"Iguana",				L"Iguana",				L"Iguana"	}	},
	{ SSTR_LIZARD					,	{	L"Lizard",				L"Lézard",				L"Eidechse",			L"Lagarto",				L"Lucertola",			L"Lagarto",				L"Lagarto"	}	},
	{ SSTR_MONITOR					,	{	L"Monitor",				L"Varan",				L"Waran",				L"Varano",				L"Varano",				L"Monitor",				L"Lagarto monitor"	}	},
	{ SSTR_PYTHON					,	{	L"Python",				L"Python",				L"Python",				L"Pitón",				L"Pitone",				L"Pitão",				L"Píton"	}	},
	{ SSTR_SALAMANDER				,	{	L"Salamander",			L"Salamandre",			L"Salamander",			L"Salamandra",			L"Salamandra",			L"Salamandra",			L"Salamandra"	}	},
	{ SSTR_SNAKE					,	{	L"Snake",				L"Serpent",				L"Schlange",			L"Serpiente",			L"Serpente",			L"Cobra",				L"Cobra"	}	},
	{ SSTR_TOAD						,	{	L"Toad",				L"Crapaud",				L"Kröte",				L"Sapo",				L"Rospo",				L"Sapo",				L"Sapo"	}	},
	{ SSTR_TORTOISE					,	{	L"Tortoise",			L"Tortue",				L"Schildkröte",			L"Tortuga",				L"Testuggine",			L"Testudinidae",		L"Cágado"	}	},
	{ SSTR_TURTLE					,	{	L"Turtle",				L"Tortue marine",		L"Wasserschildkröte",	L"Tortuga marina",		L"Tartaruga",			L"Tartaruga",			L"Tartaruga"	}	},
	
	// Small mammals
	{ SSTR_SMALL_MAMMAL				,	{	L"Small Mammal",		L"Petit mammifère",		L"Kleinsäugetier",		L"Mamífero pequeño",	L"Piccolo mammifero",	L"Pequeno mamífero",	L"Mamífero pequeno"	}	},
	{ SSTR_BAT						,	{	L"Bat",					L"Chauve-souris",		L"Fledermaus",			L"Murciélago",			L"Pipistrello",			L"Morcego",				L"Morcego"	}	},
	{ SSTR_CHINCHILLA				,	{	L"Chinchilla",			L"Chinchilla",			L"Chinchilla",			L"Chinchilla",			L"Cincillà",			L"Chinchila",			L"Chinchila"	}	},
	{ SSTR_FERRET					,	{	L"Ferret",				L"Furet",				L"Frettchen",			L"Hurón",				L"Furetto",				L"Furão",				L"Furão"	}	},
	{ SSTR_GERBIL					,	{	L"Gerbil",				L"Gerbille",			L"Rennmaus",			L"Jerbo",				L"Gerbillo",			L"Gerbo",				L"Gerbo"	}	},
	{ SSTR_GUINEA_PIG				,	{	L"Guinea Pig",			L"Cochon d'Inde",		L"Meerschweinchen",		L"Cobaya",				L"Cavia",				L"Cobaia",				L"Cobaia"	}	},
	{ SSTR_HAMSTER					,	{	L"Hamster",				L"Hamster",				L"Hamster",				L"Hámster",				L"Criceto",				L"Hamster",				L"Hamster"	}	},
	{ SSTR_HEDGEHOG					,	{	L"Hedgehog",			L"Hérisson",			L"Igel",				L"Erizo",				L"Riccio",				L"Ouriço",				L"Ouriço"	}	},
	{ SSTR_LEMUR					,	{	L"Lemur",				L"Lémurien",			L"Lemur",				L"Lémur",				L"Lemure",				L"Lêmure",				L"Lêmure"	}	},
	{ SSTR_MINK						,	{	L"Mink",				L"Vison",				L"Nerz",				L"Visón",				L"Visone",				L"Marta",				L"Marta"	}	},
	{ SSTR_MOUSE					,	{	L"Mouse",				L"Souris",				L"Maus",				L"Ratón",				L"Topo",				L"Rato",				L"Camundongo"	}	},
	{ SSTR_OPOSSUM					,	{	L"Opossum",				L"Opossum",				L"Opossum",				L"Zarigüeya",			L"Opossum",				L"Marsupial",			L"Gambá"	}	},
	{ SSTR_PRAIRIE_DOG				,	{	L"Prairie Dog",			L"Chien de prairie" ,	L"Präriehund",			L"Perro de Pradera",	L"Cane prateria",		L"Cão de pradaria",		L"Cão dos prados"	}	},
	{ SSTR_RABBIT					,	{	L"Rabbit",				L"Lapin",				L"Hase",				L"Conejo",				L"Coniglio",			L"Coelho",				L"Coelho"	}	},
	{ SSTR_RACCOON					,	{	L"Raccoon",				L"Raton laveur",		L"Waschbär",			L"Mapache",				L"Procione",			L"Guaxinim",			L"Guaxinim"	}	},
	{ SSTR_RAT						,	{	L"Rat",					L"Rat",					L"Ratte",				L"Rata",				L"Ratto",				L"Ratazana",			L"Rato"	}	},
	{ SSTR_SKUNK					,	{	L"Skunk",				L"Moufette",			L"Stinktier",			L"Mofeta",				L"Moffetta",			L"Gambá",				L"Doninha"	}	},
	{ SSTR_SQUIRREL					,	{	L"Squirrel",			L"Écureuil",			L"Eichhörnchen",		L"Ardilla",				L"Scoiattolo",			L"Esquilo",				L"Esquilo"	}	},
	
	// Large mammals
	{ SSTR_LARGE_MAMMAL				,	{	L"Large Mammal",		L"Grand mammifère",		L"Großsäugetier",		L"Grandes mamíferos",	L"Grande mammifero",	L"Grande mamífero",		L"Mamífero grande"	}	},
	{ SSTR_ANTELOPE					,	{	L"Antelope",			L"Antilope",			L"Antilope",			L"Antílope",			L"Antilope",			L"Antílope",			L"Antílope"	}	},
	{ SSTR_APE						,	{	L"Ape",					L"Grand singe",			L"Menschenaffe",		L"Simio",				L"Scimmia",				L"Símio",				L"Símio"	}	},
	{ SSTR_BABOON					,	{	L"Baboon",				L"Babouin",				L"Pavian",				L"Babuino",				L"Babbuino",			L"Babuíno",				L"Babuíno"	}	},
	{ SSTR_BEAR						,	{	L"Bear",				L"Ours",				L"Bär",					L"Oso",					L"Orso",				L"Urso",				L"Urso"	}	},
	{ SSTR_CAMEL					,	{	L"Camel",				L"Chameau",				L"Kamel",				L"Camello",				L"Cammello",			L"Camelo",				L"Camelo"	}	},
	{ SSTR_CHIMPANZEE				,	{	L"Chimpanzee",			L"Chimpanzé",			L"Schimpanse",			L"Chimpancé",			L"Scimpanzé",			L"Chimpanzé",			L"Chimpanzé"	}	},
	{ SSTR_DEER						,	{	L"Deer",				L"Cerf",				L"Reh",					L"Venado",				L"Cervide",				L"Veado",				L"Veado"	}	},
	{ SSTR_ELEPHANT					,	{	L"Elephant",			L"Éléphant",			L"Elefant",				L"Elefante",			L"Elefante",			L"Elefante",			L"Elefante"	}	},
	{ SSTR_GIRAFFE					,	{	L"Giraffe",				L"Girafe",				L"Giraffe",				L"Jirafa",				L"Giraffa",				L"Girafa",				L"Girafa"	}	},
	{ SSTR_GOAT						,	{	L"Goat",				L"Chèvre",				L"Ziege",				L"Cabra",				L"Capra",				L"Cabra",				L"Bode"	}	},
	{ SSTR_KANGAROO					,	{	L"Kangaroo",			L"Kangourou",			L"Känguru",				L"Canguro",				L"Canguro",				L"Canguru",				L"Canguru"	}	},
	{ SSTR_LLAMA					,	{	L"Llama",				L"Lama",				L"Lama",				L"Llama",				L"Lama",				L"Lama",				L"Lhama"	}	},
	{ SSTR_MONKEY					,	{	L"Monkey",				L"Singe",				L"Affe",				L"Mono",				L"Scimmia",				L"Macaco",				L"Macaco"	}	},
	{ SSTR_PIG						,	{	L"Pig",					L"Cochon",				L"Schwein",				L"Cerdo",				L"Maiale",				L"Porco",				L"Porco"	}	},
	{ SSTR_SEA_LION					,	{	L"Sea Lion",			L"Lion de mer",			L"Seelöwe",				L"León marino",			L"Leone marino",		L"Leão-marinho",		L"Leão-marinho"	}	},
	{ SSTR_SEAL						,	{	L"Seal",				L"Phoque",				L"See-Elefant",			L"Foca",				L"Foca",				L"Foca",				L"Foca"	}	},
	{ SSTR_SHEEP					,	{	L"Sheep",				L"Mouton",				L"Schaf",				L"Oveja",				L"Pecora",				L"Ovelha",				L"Ovelha"	}	},
	
	{ STR_NONE						, {} }
};

#endif
