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
	{ TSTR_ADD_SPECIES				,	{	"Add Demographic",						"Ajouter éléments démogr.",				"Demographie hinzufügen",				"Añadir demografía",					"Aggiungi demografia",					"Adicionar demografia",					"Adicionar dados demográficos"	}	},
	{ TSTR_ADMINISTRATOR			,	{	"Administrator",						"Administrateur",						"Administrator",						"Administrador",						"Amministratore",						"Administrador",						"Administrador"	}	},
	{ TSTR_ADMINISTRATOR_ID			,	{	"Administrator ID",						"ID administrateur",					"Administrator-ID",						"Id. del administrador",				"ID amministratore",					"ID do administrador",					"ID do administrador"	}	},
	{ TSTR_ADVANCED_SETTINGS		,	{	"Advanced Settings",					"Paramètres avancés",					"Erweiterte Einstellungen",				"Configuración avanzada",				"Impostazioni avanzate",				"Configurações avançadas",				"Configurações avançadas"	}	},
	{ TSTR_ALERT_VOLUME				,	{	"Alert Volume",							"Alerte volume",						"Mengenalarm",							"Volumen de alerta",					"Volume allarme",						"Volume de alerta",						"Volume do alerta"	}	},
	{ TSTR_ALL_RESULTS_BY_DATE		,	{	"All Results Sorted by Date",			"Tous résultats triés par date",		"Alle Ergebnisse nach Datum",			"Resultados por fecha",					"Tutti i risultati ordinati per data",	"Todos os resultados por data",			"Todos resultados classif. por data"	}	},
	{ TSTR_ALL_RESULTS_BY_ID		,	{	"All Results Sorted by ID",				"Tous résultats triés par ID",			"Alle Ergebnisse nach ID",				"Resultados por Id.",					"Tutti i risultati ordinati per ID",	"Todos os resultados por ID",			"Todos resultados classif. por ID"	}	},
	{ TSTR_ALL_UNITS				,	{	"All Units",							"Toutes unités",						"Alle Einheiten",						"Todas la unidades",					"Tutte le unità",						"Todas as unidades",					"Todas as unidades"	}	},
	{ TSTR_ALTERNATE_ID				,	{	"Alternate ID",							"Autre ID",								"ID ändern",							"Id. suplente",							"ID alternativo",						"ID suplente",							"ID alternativo"	}	},
	{ TSTR_ANALYTE_OPTIONS			,	{	"Analyte Options", 						"Options à analyser",					"Analytoptionen",						"Opciones de analito", 					"Opzioni analita"						"Opções de analito",					"Opções de analito"	}	},
	{ TSTR_ANALYTES					,	{	"Analytes",								"Substances à analyser",				"Analyte",								"Analitos",								"Analiti",								"Analitos",								"Analitos"	}	},
	{ TSTR_ANALYZER_INFORMATION		,	{	"Analyzer Information",					"Information analyseur",				"Analysegerät-Angaben",					"Información del analizador",			"Informazioni analizzatore",			"Informação do analisador",				"Informações do analisador"	}	},
	{ TSTR_ANALYZER_SETTINGS		,	{	"Analyzer Settings",					"Paramètres analyseur",					"Analysegerät-Einstellungen",			"Configuración del analizador",			"Impostazioni analizzatore",			"Configurações do analisador",			"Configurações do analisador"	}	},
	{ TSTR_ARCHIVE_REF_RANGES		,	{	"Archive Reference Ranges",				"Fourchettes de réf. archives",			"Archiv-Referenzbereiche",				"Guardar intervalos de referencia",		"Range di riferimento in archivio",		"Guardar intervalos de referência",		"Arquivar intervalos de referência"	}	},
	{ TSTR_ASTM_ORDER_METHOD		,	{	"ASTM Order Method",					"Méthode d'ordre ASTM",					"ASTM: Anforderungsmethode",			"Método de pedido de ASTM",				"Metodo ordine ASTM",					"Método de pedido de ASTM",				"Método de solicitação de ASTM"	}	},
	{ TSTR_ASTM_PHYSICAL_LAYER		,	{	"ASTM Physical Layer",					"Couche physique ASTM",					"ASTM: Physikalische Ebene",			"Nivel físico ASTM",					"Livello fisico ASTM",					"Nível físico de ASTM",					"Camada física de ASTM"	}	},
	{ TSTR_ASTM_TCPIP_SETTINGS		,	{	"ASTM TCP/IP Settings", 				"Paramètres TCP/IP ASTM",				"ASTM: TCP/IP-Einstellungen",			"Configuración TCP/IP ASTM",			"Impostazioni TCP/IP ASTM",				"Configurações de TCP/IP de ASTM",		"Configurações de TCP/IP do ASTM"	}	},
	{ TSTR_BACKLIGHT_BRIGHTNESS		,	{	"Backlight Brightness",					"Lumière rétroéclairage",				"Helligkeit Hintergrundbeleuchtung",	"Brillo de la luz de fondo",			"Luminosità retroilluminazione",		"Brilho da luz de fundo",				"Brilho da luz de fundo"	}	},
	{ TSTR_CALIBRATE_SCREEN			,	{	"Calibrate Screen",						"Calibrer écran",						"Bildschirm kalibrieren",				"Calibrar pantalla",					"Calibra schermo",						"Calibrar ecrã",						"Calibrar tela"	}	},
	{ TSTR_CDROM_UPDATE				,	{	"CD ROM Update",						"Mise à jour CD ROM",					"CD-ROM-Aktualisierung",				"Actualización CD-ROM",					"Aggiorna CD ROM",						"Atualização do CD-ROM",				"Atualização em CD ROM"	}	},
	{ TSTR_CHECK_TCPIP_SETTINGS		,	{	"Check TCP/IP Settings", 				"Vérifier les paramètres TCP/IP",		"TCP/IP-Einstellungen überprüfen",		"Comprobar configuración TCP/IP",		"Verifica impostazioni TCP/IP",			"Verificar configurações de TCP/IP",	"Verificar configurações TCP/IP"	}	},
	{ TSTR_CLICK_VOLUME				,	{	"Click Volume",							"Volume clic",							"Menge anklicken",						"Pulsar volumen",						"Fai clic su Volume",					"Clique em Volume",						"Clicar em Volume"	}	},
	{ TSTR_CLINICAL_ARCHIVE			,	{	"Clinical Archive",						"Archive clinique",						"Klinisches Archiv",					"Archivo clínico",						"Archivio clinico",						"Ficheiro clínico",						"Arquivo clínico"	}	},
	{ TSTR_CONFIGURE_PRINTER		,	{	"Configure Printer",					"Configurer Imprimante",				"Drucker konfigurieren",				"Configurar impresora",					"Configura stampante",					"Configurar impressora",				"Configurar impressora"	}	},
	{ TSTR_CONFIRM					,	{	"Confirm",								"Confirmer",							"Bestätigen",							"Confirmar",							"Conferma",								"Confirmar",							"Confirmar"	}	},
	{ TSTR_CONFIRM_ORDER			,	{	"Confirm Order",						"Confirmer ordre",						"Anforderung bestätigen",				"Confirmar pedido",						"Conferma ordine",						"Confirmar pedido",						"Confirmar solicitação"	}	},
	{ TSTR_CONFIRM_PASSWORD			,	{	"Confirm Password",						"Confirmer mot de passe",				"Passwort bestätigen",					"Confirmar contraseña",					"Conferma password",					"Confirmar palavra-passe",				"Confirmar senha"	}	},
	{ TSTR_CONTROL					,	{	"Control:",								"Contrôle :",							"Kontrolle:",							"Control:",								"Controllo:",							"Controlo:",							"Controle:"	}	},
	{ TSTR_CONTROL_BY_DATE			,	{	"Control Results Sorted by Date",		"Contrôler résultats triés par date",	"Ergebnisse nach Dat. überprüfen",		"Resultados control por fecha",			"Controlla risult. ordinati per data",	"Resultados de controlo por data",		"Result. controle classif. por data"	}	},
	{ TSTR_CONTROL_EXPIRATION_DATE	,	{	"Control Expiration Date",				"Contrôler date de péremption",			"Ablaufdatum überprüfen",				"Fecha de vencimiento de control",		"Controlla data di scadenza",			"Prazo de validade do controlo",		"Data de expiração do controle"	}	},
	{ TSTR_CONTROL_UPDATE			,	{	"Control Update",						"Contrôler mise à jour",				"Update überprüfen",					"Actualización de control",				"Controlla aggiornamento",				"Atualização do controlo",				"Atualização do controle"	}	},
	{ TSTR_CRITICAL_STOP			,	{	"CRITICAL STOP",						"ARRÊT CRITIQUE",						"KRITISCHER ABBRUCH",					"PARADA CRÍTICA",						"INTERRUZIONE CRITICA",					"PARAGEM CRÍTICA",						"PARADA CRÍTICA"	}	},
	{ TSTR_DATA_ENTRY_OPTIONS		,	{	"Data Entry Options",					"Options de saisie des données",		"Dateneingabeoptionen",					"Opciones de entrada de datos",			"Opzioni immissione dati",				"Opções de entrada de dados",			"Opções de inserção de dados"	}	},		
	{ TSTR_DISPLAY_SETTINGS			,	{	"Display Settings",						"Affichage paramètres",					"Anzeigeeinstellungen",					"Configuración de pantalla",			"Impostazioni display",					"Configurações do visor",				"Configurações do display"	}	},
	{ TSTR_ELECTROLYTES_UNITS		,	{	"Electrolytes Units",					"Unités électrolytes",					"Elektrolyteinheiten",					"Unidades de electrolitos",				"Unità elettroliti",					"Unidades de eletrólitos",				"Unidades de eletrólitos"	}	},
	{ TSTR_ENTER_ADMISSION_ID		,	{	"Enter Admission ID",					"Saisir ID admission",					"Zugangs-ID eingeben",					"Introducir Id. Admisión",				"Inserisci ID ammissione",				"Introduzir ID de admissão",			"Inserir ID de admissão"	}	},
	{ TSTR_ENTER_AGE				,	{	"Enter Age",							"Saisir l'âge",							"Alter eingeben",						"Introducir edad",						"Inserisci età",						"Introduzir idade",						"Inserir idade"	}	},
	{ TSTR_ENTER_AGE_RANGE			,	{	"Enter Age Range",						"Saisir fourchette d'âge",				"Altersbereich eingeben",				"Introducir intervalo de edad",			"Inserisci fascia d'età",				"Introduzir intervalo de idades",		"Inserir faixa etária"	}	},
	{ TSTR_ENTER_ALTERNATE_ID		,	{	"Enter Alternate ID",					"Saisir autre ID",						"Alternative ID eingeben",				"Introducir Id. suplente",				"Inserisci ID alternativo",				"Introduzir ID suplente",				"Inserir ID alternativo"	}	},
	{ TSTR_ENTER_ANALYZER_IP_ADDRESS,	{	"Enter Analyzer IP Address",			"Saisir l'adresse IP de l'analyseur",	"Analysegeräte-IP-Adresse eingeben",	"Introducir dirección IP analizador",	"Immettere indir. IP analizzatore",		"Introduzir endereço IP analisador",	"Inserir endereço IP do analisador"	}	},
	{ TSTR_ENTER_CONTROL_ID			,	{	"Enter Control ID",						"Entrer ID contrôle",					"Kontrollen-ID eingeben",				"Introducir Id. de control",			"Inserisci ID controllo",				"Introduzir ID do controlo",			"Inserir ID de controle"	}	},
	{ TSTR_ENTER_DATE				,	{	"Enter Date",							"Saisir date",							"Datum eingeben",						"Introducir fecha",						"Inserisci data",						"Introduzir data",						"Inserir data"	}	},
	{ TSTR_ENTER_DATE_OF_BIRTH		,	{	"Enter Date of Birth",					"Saisir date de naissance",				"Geburtsdatum eingeben",				"Introducir fecha de nacimiento",		"Inserisci data di nascita",			"Introduzir data de nascimento",		"Inserir data de nascimento"	}	},
	{ TSTR_ENTER_DOCTOR_ID			,	{	"Enter Doctor ID",						"Saisir ID médecin",					"Arzt-ID eingeben",						"Introducir Id. del médico",			"Inserisci ID medico",					"Introduzir ID do médico",				"Inserir ID do médico"	}	},
	{ TSTR_ENTER_END_DATE			,	{	"Enter End Date",						"Saisir la date de fin",				"Enddatum eingeben",					"Escriba fecha final",					"Inserisci data fine",					"Introduzir data final",				"Inserir data final"	}	},
	{ TSTR_ENTER_GATEWAY_IP_ADDRESS	,	{	"Enter Gateway IP Address",				"Saisir adresse IP de la passerelle",	"Gateway-IP-Adresse eingeben",			"Introd. dirección IP puerta enlace",	"Immettere indir. IP gateway",			"Introduzir endereço IP da porta",		"Inserir endereço IP do gateway"	}	},
	{ TSTR_ENTER_ID					,	{	"Enter ID",								"Saisir ID",							"ID eingeben",							"Introducir Id.",					 	"Inserisci ID",							"Introduzir ID",						"Inserir ID"	}	},
	{ TSTR_ENTER_ID_TO_CANCEL		,	{	"Enter ID to Cancel",					"Saisir ID pour annuler",				"ID zum Abbrechen eingeben",			"Introducir ID para cancelar",		 	"Inserisci ID per annullare",			"Introduzir ID para cancelar",		  	"Inserir ID para cancelar"}},
	{ TSTR_ENTER_IP_MASK			,	{	"Enter Analyzer IP Mask", 				"Saisir le masque IP de l'analyseur",	"IP-Maske des Analysegeräts eingeben",	"Introducir máscara IP analizador",	 	"Inserisci ID per annullare",			"Introduzir máscara IP do analisador",	"Inserir máscara de IP do analisador"	}	},
	{ TSTR_ENTER_KENNEL_NUMBER		,	{	"Enter Kennel Number",					"Saisir n° chenil",						"Zwingernr. eingeben",					"Introducir número de caseta",		 	"Inserisci numero Kennel",				"Introduzir número de Kennel",		  	"Inserir número do canil"	}	},
	{ TSTR_ENTER_LAST_VACCINATION	,	{	"Enter Last Vaccination",				"Saisir dernière vaccination",			"Letzte Impfung eingeben",				"Introducir última vacuna",			 	"inserisci ultima vaccinazione",		"Introduzir última vacinação",		  	"Inserir última vacinação"	}	},
	{ TSTR_ENTER_LOCATION			,	{	"Enter Location",						"Entrer emplacement",					"Ort eingeben",							"Introducir ubicación",				 	"Inserisci centro",						"Introduzir localização",			  	"Inserir local"	}	},
	{ TSTR_ENTER_OPERATOR_ID		,	{	"Enter Operator ID",					"Saisir ID opérateur",					"Bediener-ID eingeben",					"Introducir Id. del operario",		 	"Inserisci ID operatore",				"Introduzir ID do operador",		  	"Inserir ID do operador"	}	},
	{ TSTR_ENTER_OWNER_ID			,	{	"Enter Owner ID",						"Saisir ID propriétaire",				"Besitzer-ID eingeben",					"Introducir id. del dueño",			 	"Inserisci ID proprietario",			"Introduzir ID do proprietário",	  	"Inserir ID do proprietário"	}	},
	{ TSTR_ENTER_PASSWORD			,	{	"Enter Password",						"Saisir mot de passe",					"Passwort eingeben",					"Introducir contraseña",			 	"Inserisci password",					"Introduzir palavra-passe",			  	"Inserir senha"	}	},
	{ TSTR_ENTER_PATIENT_ID			,	{	"Enter Patient ID",						"Saisir ID patient",					"Patienten-ID eingeben",				"Introducir Id. Paciente",			 	"Inserisci ID paziente",				"Introduzir ID de doente",			  	"Inserir ID do paciente"	}	},
	{ TSTR_ENTER_PHONE_NUMBER		,	{	"Enter Phone Number",					"Saisir n° de tél",						"Telefonnummer eingeben",				"Introducir número de teléfono",	 	"Inserisci numero di telefono",			"Introduzir número de telefone",	  	"Inserir número do telefone"	}	},
	{ TSTR_ENTER_SAMPLE_ID			,	{	"Enter Sample ID",						"Saisir ID échantillon",				"Proben-ID eingeben",					"Introducir Id. de la muestra",		 	"Inserisci ID campione",				"Introduzir ID da amostra",			  	"Inserir ID da amostra"	}	},
	{ TSTR_ENTER_SERVER_IP_ADDRESS	,	{	"Enter Server IP Address",				"Saisir l'adresse IP du serveur",		"Server-IP-Adresse eingeben",		  	"Introducir dirección IP servidor",	 	"Immettere l'indirizzo IP del server",	"Introduzir endereço IP do servidor", 	"Inserir endereço IP do servidor"	}	},
	{ TSTR_ENTER_SERVER_SOCKET		,	{	"Enter Server Socket",					"Saisir le socket du serveur",			"Server-Socket eingeben",			  	"Introducir socket del servidor",	 	"Immettere il socket del server",		"Introduzir tomada do servidor",	  	"Inserir soquete do servidor"	}	},
	{ TSTR_ENTER_START_DATE			,	{	"Enter Start Date",						"Saisir la date de début",				"Anfangsdatum eingeben",			  	"Escriba fecha inicial",			 	"Inserisci data inizio",				"Introduzir data inicial",			  	"Inserir data de início"	}	},
	{ TSTR_ENZYMES_UNITS			,	{	"Enzymes Units",						"Unités enzymes",						"Enzymeinheiten",					  	"Unidades de enzimas",				 	"Unità enzimi",							"Unidades de enzimas",				  	"Unidades de enzimas"	}	},
	{ TSTR_ERROR					,	{	"ERROR",								"ERREUR",								"FEHLER",							  	"ERROR",							 	"ERRORE",								"ERRO",								  	"ERRO"	}	},
	{ TSTR_FACTORY_DEFAULT_SPECIES	,	{	"Factory Default Demographics",			"Paramètres par défaut élts démogr.",	"Voreingestellte Demographien",		  	"Demografía predeterminada",		 	"Demografia predefinita di fabbrica",	"Valores de demografia de fábrica",	  	"Dados demog. padrão de fábrica"	}	},
	{ TSTR_FACTORY_DEFAULTS			,	{	"Factory Defaults",						"Paramètres par défaut en usine",		"Werkseinstellungen",				  	"Valores predeterminados",			 	"Impostazioni di fabbrica",				"Valores de fábrica",				  	"Padrões de fábrica"	}	},
	{ TSTR_GROUP_UNITS				,	{	"Group Units",							"Unités groupe",						"Gruppeneinheiten",					  	"Unidades de grupo",				 	"Unità gruppo",							"Unidades de grupo",				  	"Unidades do grupo"	}	},
	{ TSTR_HOME						,	{	"Home",									"Page d'accueil",						"Startseite",						  	"Inicio",							 	"Home",									"Início",							  	"Início"	}	},
	{ TSTR_INCLUDE_REF_RANGES		,	{	"Include Reference Ranges",				"Inclure fourchettes de référence",		"Referenzbereiche angeben",			  	"Incluir intervalos de referencia",	 	"Includi range di riferimento",			"Incluir intervalos de referência",	  	"Incluir intervalos de referência"	}	},
	{ TSTR_INFORMATION				,	{	"Information",							"Information",							"Information",						  	"Información",						 	"Informazioni",							"Informação",						  	"Informações"	}	},
	{ TSTR_INSERT_ROTOR				,	{	"Insert Disc",							"Insérer disque",						"Scheibe einsetzen",				  	"Insertar disco",					 	"Inserisci disco",						"Inserir disco",					  	"Inserir disco"	}	},
	{ TSTR_INSERT_SAMPLE			,	{	"Insert Sample",						"Insérer échantillon",					"Probe einsetzen",					  	"Insertar muestra",					 	"Inserisci campione",					"Inserir amostra",					  	"Inserir amostra"	}	},
	{ TSTR_INTERNAL_ARCHIVES		,	{	"Internal Archives",					"Archives internes",					"Interne Archive",					  	"Archivos internos",				 	"Archivi interni",						"Ficheiros internos",				  	"Arquivos internos"	}	},
	{ TSTR_LIPIDS_UNITS				,	{	"Lipids Units",							"Unités lipides",						"Lipideinheiten",					  	"Unidades de lípidos",				 	"Unità lipidi",							"Unidades de lípidos",				  	"Unidades de lipídios"	}	},
	{ TSTR_LOAD_CD					,	{	"Load CD",								"Charger CD",							"CD laden",							  	"Cargar CD",						 	"Carica CD",							"Carregar CD",						  	"Carregar CD"	}	},
	{ TSTR_LOAD_CONTROLS_CD			,	{	"Load Controls CD",						"Charger CD commandes",					"CD-Steuerung laden",				  	"Cargar controles CD",				 	"Carica CD di controllo",				"Carregar CD de controlos",			  	"Carregar CD de controles"	}	},
	{ TSTR_LOAD_ORDER				,	{	"Load Order",							"Charger ordre",						"Anforderung laden",				  	"Cargar pedido",					 	"Carica ordine",						"Carregar pedido",					  	"Carregar solicitação"	}	},
	{ TSTR_LOAD_ROTOR				,	{	"Load Disc",							"Charger disque",						"Scheibe laden",					  	"Cargar disco",						 	"Carica disco",							"Carregar disco",					  	"Carregar disco"	}	},
	{ TSTR_LOCKOUT_SETTINGS			,	{	"Lockout Settings",						"Paramètres verrouillage",				"Lookout-Eimstellungen",			  	"Confguración de bloqueo",			 	"Impostazioni del blocco",				"Configurações de bloqueio",		  	"Configurações de bloqueio"	}	},
	{ TSTR_MANAGE_ADMINISTRATORS	,	{	"Manage Administrators",				"Gestion administrateurs",				"Administratoren verwalten",		  	"Gestionar administradores",		 	"Gestisci amministratori",				"Gerir administradores",			  	"Gerenciar administradores"	}	},
	{ TSTR_MANAGE_OPERATORS			,	{	"Manage Operators",						"Gestion opérateurs",					"Bediener verwalten",				  	"Gestionar operarios",				 	"Gestisci operatori",					"Gerir operadores",					  	"Gerenciar operadores"	}	},
	{ TSTR_MINERALS_UNITS			,	{	"Minerals Units",						"Unités minérales",						"Mineralieneinheiten",				  	"Unidades de minerales",			 	"Unità minerali",						"Unidades de minerais",				  	"Unidades de minerais"	}	},
	{ TSTR_MODIFY_REF_RANGES		,	{	"Modify Reference Range",				"Modifier fourchette de référence",		"Referenzbereich ändern",			  	"Modificar intervalo de referencia", 	"Modifica range di riferimento",		"Alterar intervalo de referência",	  	"Modificar intervalo de referência"	}	},
	{ TSTR_NEW_ADMINISTRATOR_ID		,	{	"New Administrator ID",					"Nouvel ID administrateur",				"Neue Administratoren-ID",			  	"Nueva Id. del administrador",		 	"Nuova ID amministratore",				"Nova ID do administrador",			  	"Novo ID do administrador"	}	},
	{ TSTR_NEW_OPERATOR_ID			,	{	"New Operator ID",						"Nouvel ID opérateur",					"Neue Bediener-ID",					  	"Nueva Id. del operario",				"Nuova ID operatore",					"Nova ID do operador",				  	"ID do novo operador"	}	},
	{ TSTR_NO_RESULTS				,	{	"No Results",							"Aucun résultat",						"Keine Ergebnisse",					  	"No hay resultados",					"Nessun risultato",						"Sem resultados",					  	"Nenhum resultado"	}	},
	{ TSTR_NUM_ERROR_COPIES			,	{	"# Error Copies",						"Nbre copies erronées",					"# Fehler-Kopien",					  	"N° de copias de error",				"N. Copie errore",						"N.° de cópias de erro",			  	"No. de cópias de erro"	}	},
	{ TSTR_NUM_IQC_COPIES			,	{	"# iQC Copies",							"Nbre copies iQC",						"# iQC-Kopien",						  	"N° de copias iQC",						"N. Copie iQC",							"N.° de cópias do iQC",				  	"No. de cópias de iQC"	}	},
	{ TSTR_NUM_RESULTS_COPIES		,	{	"# Results Copies",						"Nbre copie résultats",					"# Ergebniskopien",					  	"N° de copias de resultados",			"N. Copie risultati",					"N.° de cópias de resultados",		  	"No. de cópias dos resultados"	}	},
	{ TSTR_OPERATOR					,	{	"Operator",								"Opérateur",							"Bediener",							  	"Operario",								"Operatore",							"Operador",							  	"Operador"	}	},
	{ TSTR_OPERATOR_EXPIRATION		,	{	"Operator Expiration",					"Expiration opérateur",					"Ungültiger Bediener",				  	"Vencimiento del operario",				"Scadenza operatore",					"Prazo de validade do operador",	  	"Expiração do operador"	}	},
	{ TSTR_OPERATOR_ID				,	{	"Operator ID",							"ID opérateur",							"Bediener-ID",						  	"Id. del operario",						"ID operatore",							"ID do operador",					  	"ID do operador"	}	},
	{ TSTR_PATIENT					,	{	"Patient:",								"Patient :",							"Patient:",							  	"Paciente:",							"Paziente:",							"Doente:",							  	"Paciente:"	}	},
	{ TSTR_PATIENT_BY_DATE			,	{	"Patient Results Sorted by Date",		"Résultats patients triés par date",	"Patientenergebnisse nach Datum",	  	"Resultados paciente por fecha",		"Risultati paz. ordinati per data",		"Resultados do doente por data",	  	"Result. paciente classif. por data"	}	},
	{ TSTR_PENDING_ORDERS			,	{	"Pending Orders",						"Ordres en cours",					  	"Ausstehende Anforderungen",		  	"Pedidos pendientes",					"Ordini in sospeso",					"Pedidos pendentes",				  	"Solicitações pendentes"	}	},
	{ TSTR_POWER_SAVE_WAIT			,	{	"Power Save Wait",						"Attente économiseur alim.",		  	"Energiesparmodus, warten",			  	"Espera de ahorro de energía",			"Attendere Modo risparmio",				"Espera de poupança de energia",	  	"Espera de economia de energia"	}	},
	{ TSTR_PRINTER_SETUP			,	{	"Printer Setup",						"Configuration imprimante",			  	"Druckereinrichtung",				  	"Configuración impresora",				"Impostazioni stampante",				"Configuração da impressora",		  	"Configuração da impressora"	}	},
	{ TSTR_PROTEINS_UNITS			,	{	"Proteins Units",						"Unités protéines",					  	"Proteineinheiten",					  	"Unidades de proteínas",				"Unità proteine",						"Unidades de proteínas",			  	"Unidades de proteínas"	}	},
	{ TSTR_RANGES					,	{	"Ranges",								"Fourchettes",						  	"Bereiche",							  	"Intervalos",							"Range",								"Intervalos",						  	"Intervalos"	}	},
	{ TSTR_READ_RESULTS				,	{	"Read Results",							"Lecture résultats",				  	"Ergebnisse auslesen",				  	"Leer resultados",						"Leggi risultati",						"Ler resultados",					  	"Ler resultados"	}	},
	{ TSTR_RECALL					,	{	"Recall",								"Rappel",							  	"Wiederaufruf",						  	"Recordar",								"Richiama",								"Recordar",							  	"Recuperar"	}	},
	{ TSTR_REMOVE_ROTOR				,	{	"Remove Disc",							"Retirer disque",					  	"Scheibe entfernen",				  	"Retirar disco",						"Rimuovi disco",						"Retirar disco",					  	"Remover disco"	}	},
	{ TSTR_REMOVE_SPECIES			,	{	"Remove Demographic",					"Retirer éléments démographiques",	  	"Demographien entfernen",			  	"Eliminar demografía",					"Rimuovi demografia",					"Eliminar demografia",				  	"Remover dados demográficos"	}	},
	{ TSTR_RETRIEVE_REF_RANGES		,	{	"Retrieve Reference Ranges",			"Récupérer fourchettes de référence", 	"Referenzbereiche empfangen",		  	"Recuperar intervalos de referencia",	"Recupera range di riferimento",		"Recuperar intervalos de referência", 	"Recuperar intervalos de referência"	}	},
//	{ TSTR_SAMPLE					,	{	"Sample",								"Échantillon",						  	"Probe",							  	"Muestra",								"Campione",								"Amostra",							  	"Amostra"	}	},
	{ TSTR_SCREEN_SAVER_WAIT		,	{	"Screen Saver Wait",					"Attente économiseur d'écran",		  	"Bildschirmschoner, warten",		  	"Espera del salvapantalla",				"Attendere salvaschermo",				"Espera de poupança do ecrã",		  	"Espera de proteção de tela"	}	},
	{ TSTR_SEARCH					,	{	"Search",								"Rechercher",						  	"Suchen",							  	"Búsqueda",								"Cerca",								"Pesquisar",						  	"Pesquisar"	}	},
	{ TSTR_SEARCH_ADMINISTRATORS	,	{	"Search Administrators",				"Rechercher administrateurs",		  	"Administratoren suchen",			  	"Buscar administradores",				"Cerca amministratori",					"Pesquisar administradores",		  	"Pesquisar administradores"	}	},
	{ TSTR_SEARCH_OPERATORS			,	{	"Search Operators",						"Rechercher opérateurs",			  	"Bediener suchen",					  	"Buscar operarios",						"Cerca operatori",						"Pesquisar operadores",				  	"Pesquisar operadores"	}	},
	{ TSTR_SECURITY_SETTINGS		,	{	"Security Settings",					"Paramètres de sécurité",			  	"Sicherheitseinstellungen",			  	"Configuración de seguridad",			"Impostazioni sicurezza",				"Configurações de segurança",		  	"Configurações de segurança"	}	},
	{ TSTR_SELECT_DISPLAY_LANG		,	{	"Select Display Language",				"Sélectionner affich. langue",		  	"Sprache auswählen",				  	"Seleccionar idioma",					"Seleziona lingua display",				"Selecionar idioma do visor",		  	"Selecionar idioma de exibição"	}	},
	{ TSTR_SELECT_GENDER			,	{	"Select Gender",						"Sélectionner le sexe",				  	"Geschlecht auswählen",				  	"Seleccionar sexo",						"Seleziona sesso",						"Selecionar sexo",					  	"Selecionar sexo"	}	},
	{ TSTR_SELECT_KEYBOARD			,	{	"Select Keyboard Type",					"Sélectionner type de clavier",		  	"Tastatur auswählen",				  	"Seleccionar tipo de teclado",			"Seleziona tipo tastiera",				"Selecionar tipo de teclado",		  	"Selecionar tipo de teclado"	}	},
	{ TSTR_SELECT_PRINTER			,	{	"Select Printer",						"Sélectionner imprimante",			  	"Drucker auswählen",				  	"Seleccionar impresora",				"Seleziona stampante",					"Selecionar impressora",			  	"Selecionar impressora"	}	},
	{ TSTR_SELECT_PROTOCOL			,	{	"Select Protocol",						"Sélectionner un protocole",		  	"Protokoll auswählen",				  	"Seleccionar protocolo",				"Selezionare protocollo",				"Selecionar protocolo",				  	"Selecionar protocolo"	}	},
	{ TSTR_SELECT_RACE				,	{	"Select Race",							"Choisir la race",					  	"Abstammung auswählen",				  	"Seleccione raza",						"Seleziona razza",						"Selecionar corrida",				  	"Selecionar raça"	}	},
	{ TSTR_SELECT_REPORT			,	{	"Select Report",						"Sélectionner rapport",				  	"Bericht auswählen",				  	"Seleccionar informe",					"Select rapporto",						"Selecionar relatório",				  	"Selecionar relatório"	}	},
	{ TSTR_SELECT_REPORTS			,	{	"Select Reports",						"Sélectionner rapports",			  	"Berichte auswählen",				  	"Seleccionar informes",					"Seleziona rapporti",					"Selecionar relatórios",			  	"Selecionar relatórios"	}	},
	{ TSTR_SELECT_SPECIES			,	{	"Select Demographic",					"Sélectionner éléments démogr.",	  	"Demographie auswählen",			  	"Seleccionar demografía",				"Seleziona demografia",					"Selecionar demografia",			  	"Selecionar dados demográficos"	}	},
	{ TSTR_SELECT_TYPE				,	{	"Select Type",							"Sélectionner type",				  	"Typ auswählen",					  	"Seleccionar tipo",						"Seleziona tipo",						"Selecionar tipo",					  	"Selecionar tipo"	}	},
	{ TSTR_SEND_REF_RANGES			,	{	"Send Reference Ranges",				"Envoyer fourchettes de réf.",		  	"Referenzbereiche senden",			  	"Enviar intervalos de referencia",		"Imposta range di riferimento",			"Enviar intervalos de referência",	  	"Enviar intervalos de referência"	}	},
	{ TSTR_SERVICE_ARCHIVE			,	{	"Service Archive",						"Archive service",					  	"Service-Archiv",					  	"Archivo de servicio",					"Archivio di servizio",					"Ficheiro do serviço",				  	"Arquivo de serviço"	}	},
	{ TSTR_SET_DATE					,	{	"Set Date",								"Définir date",						  	"Datum einstellen",					  	"Establecer fecha",						"Imposta data",							"Configurar data",					  	"Configurar data"	}	},
	{ TSTR_SET_DEFAULT				,	{	"Set Default",							"Définir paramètres par défaut",	  	"Standard einstellen",				  	"Establecer predeterminados",			"Imposta come predefinito",				"Configurar valores predeterminados", 	"Configurar padrão"	}	},
	{ TSTR_SET_LANGUAGE				,	{	"Set Language",							"Définir la langue",				  	"Sprache einstellen",				  	"Establecer idioma",					"Imposta lingua",						"Configurar idioma",				  	"Configurar idioma"	}	},
	{ TSTR_SET_PROTOCOL				,	{	"Set Protocol",							"Définir le protocole",				  	"Protokoll einstellen",				  	"Establecer protocolo",					"Imposta protocollo",					"Configurar protocolo",				  	"Configurar protocolo"	}	},
	{ TSTR_SET_TIME					,	{	"Set Time",								"Définir l'heure",					  	"Zeit einstellen",					  	"Establecer hora",						"Imposta tempo",						"Configurar hora",					  	"Configurar hora"	}	},
	{ TSTR_SET_UNITS				,	{	"Set Units",							"Définir les unités",				  	"Einheiten einstellen",				  	"Establecer unidades",					"Imposta unità",						"Configurar unidades",				  	"Configurar unidades"	}	},
	{ TSTR_SINGLE_ANALYTE			,	{	"Single Analyte",						"Une seule substance à analyser",	  	"Einzelnes Analyt",					  	"Analito único",						"Analita singolo",						"Analito único",					  	"Analito único"	}	},
	{ TSTR_SOFTWARE_UPDATE			,	{	"Software Update",						"Mise à jour logiciel",				  	"Software-Update",					  	"Actualización del software",			"Aggiornamento software",				"Atualização do software",			  	"Atualização de software"	}	},
	{ TSTR_SOUND_SETTINGS			,	{	"Sound Settings",						"Paramètres sonores",				  	"Sound-Einstellungen",				  	"Configuración de sonido",				"Impostazioni audio",					"Configurações do som",				  	"Configurações do som"	}	},
	{ TSTR_SPECIES					,	{	"Demographic",							"Éléments démographiques",			  	"Demographie",						  	"Demografía",							"Demografia",							"Demografia",						  	"Dados demográficos"	}	},
	{ TSTR_SPECIES_AVAILABLE		,	{	"Demographics Available",				"Éléments démographiques disponibles",	"Verfügbare Demographien",			  	"Demografías disponibles",				"Demografia disponibile",				"Demografia disponível",			  	"Dados demográficos disponíveis"	}	},
	{ TSTR_STATUS_VOLUME			,	{	"Status Volume",						"État volume",						  	"Mengenstatus",						  	"Volumen de estado",					"Volume di stato",						"Volume de estado",					  	"Volume do status"	}	},
	{ TSTR_TCPIP_SETTINGS			,	{	"TCP/IP Settings",						"Paramètres TCP/IP",				  	"TCP/IP-Einstellungen",				  	"Configuración TCP/IP",					"Impostazioni TCP/IP",					"Configurações de TCP/IP",			  	"Configurações de TCP/IP"	}	},
	{ TSTR_TRANSMIT_ALL				,	{	"Transmit All",							"Transmettre tout",					  	"Alle übertragen",					  	"Transmitir todo",						"Trasmetti tutti",						"Transmitir tudo",					  	"Transmitir todos"	}	},
	{ TSTR_TRANSMIT_DATE_RANGE		,	{	"Transmit Date Range",					"Transmettre la plage de dates",	  	"Datumsbereich übertragen",			  	"Transmitir intervalo de fechas",		"Trasmetti range di date",				"Transmitir intervalo de data",		  	"Transmitir intervalo de datas"	}	},
	{ TSTR_UNITS					,	{	"Units",								"Unités",							  	"Einheiten",						  	"Unidades",								"Unità",								"Unidades",							  	"Unidades"	}	},
	{ TSTR_VIEW_RESULT				,	{	"View Result",							"Afficher résultat",				  	"Ergebnis ansehen",					  	"Ver resultado",						"Visualizza risultati",					"Visualizar resultado",				  	"Visualizar resultado"	}	},
	{ TSTR_WARNING					,	{	"WARNING",								"MISE EN GARDE",					  	"ACHTUNG",							  	"AVISO",								"ATTENZIONE",							"ADVERTÊNCIA",						  	"AVISO"	}	},

// Middle (Long Button Stings > 35 Characters Unlikely to Display Correctly)
//											ENGLISH,																			FRENCH,																											GERMAN,																												SPANISH,																								ITALIAN, 																											PORTUGUESE,																									PORTUGUESE (BRAZIL)
//											          11111111112222222222233333												          11111111112222222222233333																			          11111111112222222222233333																				          11111111112222222222233333																	          11111111112222222222233333																				          11111111112222222222233333																		          11111111112222222222233333																				          	
//											 12345678901234567890123456789012345												 12345678901234567890123456789012345																			 12345678901234567890123456789012345																				 12345678901234567890123456789012345																	 12345678901234567890123456789012345																				 12345678901234567890123456789012345																		 12345678901234567890123456789012345	
	{ MSTR_ADD_ADMINISTRATOR		,	{	"Add Administrator",																"Ajouter administrateur",																						"Admin. hinzufügen",																								"Añadir administrador",																					"Agg. amministratore",																								"Adicionar administrador",																					"Adicionar administrador"	}	},
	{ MSTR_ADD_CONTROL_VALUES		,	{	"Add Control Values",																"Ajouter valeurs de contrôle",																					"Steuerungswerte hinzufügen",																						"Añadir valores de control",																			"Aggiungi valori di controllo",																						"Adicionar valores de controlo",																			"Adicionar valores de controle"	}	},
	{ MSTR_ADD_OPERATOR				,	{	"Add Operator",																		"Ajouter opérateur",																							"Bediener hinzufügen",																								"Añadir operario",																						"Aggiungi operatore",																								"Adicionar operator",																						"Adicionar operador"	}	},
	{ MSTR_ADD_SPECIES				,	{	"Add Demographic",																	"Ajouter éléments démographiques",																				"Demographie hinzufügen",																							"Añadir demografía",																					"Aggiungi demografia",																								"Adicionar demografia",																						"Adicionar dados demográficos"	}	},
	{ MSTR_ADMISSION_ID				,	{	"Admission ID",																		"ID admission",																									"Zugangs-ID",																										"Id. admisión",																							"ID ammissione",																									"ID de Admissão",																							"ID de admissão"	}	},
	{ MSTR_ADMINISTRATOR_ID			,	{	"Administrator ID",																	"ID administrateur",																							"Administrator-ID",																									"Id. del administrador",																				"ID amministratore",																								"ID do administrador",																						"ID do administrador"	}	},
	{ MSTR_ADMINISTRATORS			,	{	"Administrators",																	"Administrateurs",																								"Administratoren",																									"Administradores",																						"Amministratori",																									"Administradores",																							"Administradores"	}	},
	{ MSTR_AGE						,	{	"Age",																				"Age",																											"Alter",																											"Edad",																									"Età",																												"Idade",																									"Idade"	}	},
	{ MSTR_ALERTS					,	{	"Alerts",																			"Avertisseurs",																									"Warnungen",																										"Alertas",																								"Allarmi",																											"Alertas",																									"Alertas"	}	},
	{ MSTR_ALL						,	{	"All",																				"Tous",																											"Alle",																												"Todo",																									"Tutti",																											"Tudo",																										"Todos"	}	},
	{ MSTR_ALL_ANALYTES				,	{	"All Analytes",																		"Toutes subs. à analyser",																						"Alle Analyte",																										"Todos los analitos",																					"Tutti gli analiti",																								"Todos os analitos",																						"Todos os analitos"	}	},
	{ MSTR_ALTERNATE_ID				,	{	"Alternate ID",																		"Autre ID",																										"ID ändern",																										"Id. suplente",																							"ID alternativo",																									"ID suplente",																								"ID alternativo"	}	},
	{ MSTR_ALWAYS					,	{	"Always",																			"Toujours",																										"Immer",																											"Siempre",																								"Sempre",																											"Sempre",																									"Sempre"	}	},
	{ MSTR_ANALYSIS_COMPLETE		,	{	"Analysis Complete",																"Analyse terminée",																								"Analyse abgeschlossen",																							"Análisis completado",																					"Analisi completa",																									"Análise concluída",																						"Análise concluída"	}	},
	{ MSTR_ANALYTE					,	{	"Analyte",																			"Substance à analyser",																							"Analyt",																											"Analito",																								"Analita",																											"Analito",																									"Analito"	}	},
	{ MSTR_ANALYTE_GROUPS			,	{	"Analyte Groups",																	"Gr. subst. à analyser",																						"Analytgruppen",																									"Grupos analito",																						"Gruppi analita",																									"Grupos de analito",																						"Grupos de analitos"	}	},
	{ MSTR_ANALYTE_OPTIONS			,	{	"Analyte Options",																	"Options à analyser",																							"Analytoptionen",																									"Opciones de analito",																					"Opzioni analita",																									"Opções de analito",																						"Opções de analito"	}	},
	{ MSTR_ANALYZE					,	{	"Analyze",																			"Analyser",																										"Analysieren",																										"Analizar",																								"Analizzare",																										"Analisar",																									"Analisar"	}	},
	{ MSTR_ANALYZER_IP				,	{	"Analyzer IP",																		"IP de l'analyseur",																							"Analysegerät-IP",																									"IP del analizador",																					"IP analizzatore",																									"IP do analisador",																							"IP do analisador"	}	},
	{ MSTR_ANALYZER_PRINTER			,	{	"Analyzer Printer",																	"Imprimte analyseur",																							"Drucker-Analysege.",																								"Impres. Analiz.",																						"Stamp. Analizzatore",																								"Impressora do analisador",																					"Impressora do analisador"	}	},
	{ MSTR_ANALYZING_SAMPLE			,	{	"Analyzing Sample...",																"Analyse éch. en cours...",																						"Probe wird analysiert ...",																						"Analizando muestra...",																				"Esame camp. in corso",																								"A analisar amostra...",																					"Analisando amostra..."	}	},
	{ MSTR_ASCII_TEXT				,	{	"ASCII Text",																		"Texte ASCII",																									"ASCII-Text",																										"Texto ASCII",																							"Testo ASCII",																										"Texto ASCII",																								"Texto ASCII"	}	},
	{ MSTR_ASIAN_PACIFIC_ISLANDER	,	{	"Asian/Pacific Islander",															"Asiat./orig. îles Pac.",																						"Asiatisch/Pazifisch",																								"Asiático/Isl. Pacífico",																				"Isolano Asia/Pacifico",																							"Ilhas Asiáticas/do Pacífico",																				"Asiático/ilhas do Pacífico"	}	},
	{ MSTR_ASTM_1394_7				,	{	"ASTM 1394-97",																		"ASTM 1394-97",																									"ASTM 1394-97",																										"ASTM 1394-97",																							"ASTM 1394-97",																										"ASTM 1394-97",																								"ASTM 1394-97"	}	},
	{ MSTR_ASTM_ORDERS				,	{	"ASTM Orders",																		"Ordres ASTM",																									"ASTM Anforderungen",																								"Pedidos ASTM",																							"Ordini ASTM",																										"Pedidos ASTM",																								"Solicitações ASTM"	}	},
	{ MSTR_AUTO						,	{	"Auto",																				"Auto",																											"Automat.",																											"Auto",																									"Auto",																												"Auto",																										"Auto"	}	},
	{ MSTR_BACKLIGHT				,	{	"Backlight",																		"Rétroéclairage", 																								"Hintergrundbeleucht.",																								"Luz",																									"Retroilluminazione",																								"Luz de fundo",																								"Luz de fundo"	}	},
	{ MSTR_BASE_TEST				,	{	"Base Test",																		"Test de base",																									"Basistest",																										"Prueba base",																							"Test base",																										"Teste base",																								"Teste de base"	}	},
	{ MSTR_BLACK					,	{	"Black",																			"Noire",																										"Schwarz",																											"Negro",																								"Nero",																												"Negro",																									"Preto"	}	},
	{ MSTR_BOTH						,	{	"Both",																				"Les deux", 																									"Beides",																											"Ambos",																								"Entrambe",																											"Ambos",																									"Ambos"	}	},
	{ MSTR_BROWSE					,	{	"Browse",																			"Naviguer", 																									"Durchsuchen",																										"Examinar",																								"Sfoglia",																											"Navegar",																									"Procurar"	}	},
	{ MSTR_CALIBRATE				,	{	"Calibrate",																		"Calibrer", 																									"Kalibrieren",																										"Calibrar",																								"Calibra",																											"Calibrar",																									"Calibrar"	}	},
	{ MSTR_CAM_CYCLES				,	{	"Cam Cycles",																		"Cycles cam", 																									"Nockenzyklen",																										"Ciclos de la leva",																					"Cicli camma",																										"Ciclos Cam",																								"Ciclos da cam"	}	},
	{ MSTR_CANCEL_ANALYSIS			,	{	"Cancel Analysis?",																	"Annuler analyse ?", 																							"Analyse abbrechen?",																								"¿Cancelar análisis?",																					"Annullare analisi?",																								"Cancelar análise?",																						"Cancelar análise?"	}	},
	{ MSTR_CANCEL_ARCHIVE			,	{	"Cancel Archive?",																	"Annuler archive ?", 																							"Archiv stornieren?",																								"¿Cancelar archivo?",																					"Annulla archivio?",																								"Cancelar arquivo?",																						"Cancelar arquivo?"	}	},
	{ MSTR_CANCEL_ORDER				,	{	"Cancel Order",																		"Annuler l'ordre",																								"Anforderung abbrechen",																							"Cancelar pedido",																						"Annulla ordine",																									"Cancelar pedido",																							"Cancelar solicitação"	}	},
	{ MSTR_CANCEL_TRANSMISSION		,	{	"Cancel Transmission?",																"Annuler transmission ?", 																						"Übertragung abbrechen?",																							"¿Cancelar transmisión?",																				"Annullare trasmissione?",																							"Cancelar transmissão?",																					"Cancelar transmissão?"	}	},
	{ MSTR_CANCELED					,	{	"Canceled",																			"Annulé", 																										"Abgebrochen",																										"Cancelado",																							"Annullato",																										"Cancelado",																								"Cancelado"	}	},
	{ MSTR_CANCELING_ANALYSIS		,	{	"Canceling analysis, please wait",													"Annulation analyse en cours, veuillez patienter",																"Analyse wird abgebrochen, bitte warten",																			"Cancelando análisis. Espere, por favor.",																"Annullamento analisi, attendere",																					"A cancelar análise, espere por favor",																		"Cancelando a análise, aguarde"	}	},
	{ MSTR_CHANGE_EXPIRATION		,	{	"Change Expiration",																"Modifier expiration",																							"Ablaufdatum ändern",																								"Cambiar vencimiento",																					"Modifica scadenza",																								"Alterar prazo de validade",																				"Alterar expiração"	}	},
	{ MSTR_CHANGE_PASSWORD			,	{	"Change Password",																	"Modifier mot de passe",																						"Passwort ändern",																									"Cambiar contraseña",																					"Modifica password",																								"Alterar palavra-passe",																					"Alterar senha"	}	},
	{ MSTR_CHANGE_SETTINGS			,	{	"Change Settings",																	"Modifier les paramètres",																						"Einstellungen ändern",																								"Cambiar configuración",																				"Modifica impostazioni",																							"Alterar configurações",																					"Alterar configurações"	}	},
	{ MSTR_CHECK_SETTINGS			,	{	"Check Settings",																	"Vérifier les paramètres",																						"Einstell. Üperprüfen",																								"Comprobar config.",																					"Verifica impostazioni",																							"Verificar configurações",																					"Verificar configurações"	}	},
	{ MSTR_CLEAR					,	{	"Clear",																			"Supprimer",																									"Freigeben",																										"Borrar",																								"Pulisci",																											"Eliminar",																									"Limpar"	}	},
	{ MSTR_CLINICAL_ARCHIVE			,	{	"Clinical Archive",																	"Archive clinique",																								"Klinisches Archiv",																								"Archivo clínico",																						"Archivio clinico",																									"Ficheiro clínico",																							"Arquivo clínico"	}	},
	{ MSTR_CLINICAL_ARCHIVE_ELLIPSIS,	{	"Clinical Archive...",																"Archive clinique...",																							"Klinisches Archiv...",																								"Archivo clínico..",																					"Archivio clinico...",																								"Ficheiro clínico...",																						"Arquivo clínico..."	}	},
	{ MSTR_CLOSE_DRAWER_TO_ANALYZE	,	{	"Close drawer to analyze a sample",													"Fermer tiroir pour analyser un échantillon",																	"Zur Probenanalyse Fach schließen",																					"Cerrar el cajón para analizar una muestra",															"Chiudi cassetto per analizzare campione",																			"Fechar a gaveta para analisar uma amostra",																"Feche a gaveta para analisar uma amostra"	}	},
	{ MSTR_CLOSING_DRAWER			,	{	"Closing Drawer...",																"Fermeture tiroir en cours...",																					"Fach wird geschlossen ...",																						"Cerrando cajón...",																					"Chiusura cassetto...",																								"A fechar a gaveta...",																						"Fechando gaveta..."	}	},
	{ MSTR_CONFIGURE				,	{	"Configure",																		"Configurer",																									"Konfigurieren",																									"Config.",																								"Configura",																										"Configurar",																								"Configurar"	}	},
	{ MSTR_CONFIRM_TRANSMIT_ALL		,	{	"Press Yes to transmit all results saved in the analyzer",							"App. Oui pour trans. tous résultats enreg. ds analyseur",														"\"Ja\", um alle im Analysegerät gesp. Erg. zu übertrag.",															"Pulse Sí para transmitir todos los resultados guardados en el analizador",								"Premere Sì per trasmettere tutti i risultati savalti nell'analizzatore",											"Prima Sim para transmitir todos os resultados guardados no analisador",									"Pressione Sim para transmitir todos os resultados salvos no analisador"	}	},
	{ MSTR_CONNECTION_FAILED		,	{	"Connection failed",																"Échec de la connexion",																						"Verbindung nicht hergestellt",																						"Falló conexión",																						"Connessione non riuscita",																							"Falha de ligação",																							"Falha na conexão"	}	},
	{ MSTR_CONNECTION_SUCCESSFUL	,	{	"Connection successful",															"Connexion établie",																							"Verbindung hergestellt",																							"Conexión establecida",																					"Connessione riuscita",																								"Ligação estabelecida",																						"Conexão bem-sucedida"	}	},
	{ MSTR_CONTROLS					,	{	"Controls",																			"Contrôles",																									"Steuerungen",																										"Controles",																							"Controlli",																										"Controlos",																								"Controles"	}	},
	{ MSTR_CURRENT					,	{	"Current",																			"Actuel",																										"Laufend",																											"Actual",																								"Attuale",																											"Atual",																									"Atual"	}	},
	{ MSTR_DATA_ENTRY_OPTIONS		,	{	"Data Entry Options",																"Saisie des données",																							"Dateneingabeoptionen",																								"Opc. entrada datos",																					"Opzioni imm. dati",																								"Opções de entrada de dados",																				"Opções de inserção de dados"	}	},
	{ MSTR_DATE						,	{	"Date",																				"Date",																											"Datum",																											"Fecha",																								"Data",																												"Data",																										"Data"	}	},
	{ MSTR_DATE_OF_BIRTH			,	{	"Date of Birth (DOB)",																"Date de naissance (DOB)",																						"Geburtsdatum (GD)",																								"Fecha de nacimiento",																					"Data di nascita (DN)",																								"Data de nascimento (DDN)",																					"Data de nascimento (DN)"	}	},
	{ MSTR_DATE_OF_VACCINATION		,	{	"Last Vaccination (DOV)",															"Dernier vaccin (DOV)",																							"Letzte Impfung (LI)",																								"Última vacuna",																						"Ultima vaccinazione (DV)",																							"Última vacinação (DDV)",																					"Última vacinação (Data de vacin.))"	}	},
	{ MSTR_DAY						,	{	"Day",																				"Jour",																											"Tag",																												"Día",																									"Giorno",																											"Dia",																										"Dia"	}	},
	{ MSTR_DAYS						,	{	"Days",																				"Jours",																										"Tage",																												"Días",																									"Giorni",																											"Dias",																										"Dias"	}	},
	{ MSTR_DEFAULT					,	{	"Default",																			"V/déft",																										"Standard",																											"Predeterm.",																							"Predef.",																											"Predeterminado",																							"Padrão"	}	},
	{ MSTR_DELETE_ADMINISTRATOR		,	{	"Delete Administrator",																"Suppr. administrateur",																						"Administrator löschen",																							"Eliminar administrador",																				"Cancella amministratore",																							"Eliminar administrador",																					"Excluir administrador"	}	},
	{ MSTR_DELETE_OPERATOR			,	{	"Delete Operator",																	"Suppr. opérateur", 																							"Bediener löschen",																									"Eliminar operario",																					"Cancella operatore",																								"Eliminar operador",																						"Excluir operador"	}	},
	{ MSTR_DETERMINING_ROTOR_TYPE	,	{	"Determining disc type",															"Détermination type de disque",																					"Scheibentyp festlegen",																							"Determinando el tipo de disco",																		"Determinazione tipo del disco",																					"A determinar o tipo de disco",																				"Determinando o tipo de disco"	}	},
	{ MSTR_DISABLED					,	{	"Disabled",																			"Désactivé",																									"Deaktiviert",																										"Inactivo",																								"Disattivato",																										"Inativo",																									"Desativado"	}	},
	{ MSTR_DOB						,	{	"DOB",																				"DOB",																											"GD",																												"F. nac.",																								"DN",																												"DDN",																										"DN"	}	},
	{ MSTR_DOCTOR_ID				,	{	"Doctor ID",																		"ID médecin",																									"Arzt-ID",																											"Id. médico",																							"ID medico",																										"ID do médico",																								"ID do médico"	}	},
	{ MSTR_DOV						,	{	"DOV",																				"DOV",																											"LI",																												"Úl. vac.",																								"DV",																												"DDV",																										"Data de vacinação"	}	},
	{ MSTR_DRAWER_CYCLES			,	{	"Drawer Cycles",																	"Cycles tiroir",																								"Fachzyklen",																										"Ciclos del cajón",																						"Cicli cassetto",																									"Ciclos da gaveta",																							"Ciclos da gaveta"	}	},
	{ MSTR_EGFR						,	{	"eGFR", 																			"eGFR",																											"eGFR",																												"eGFR",																									"eGFR",																												"TFGe",																										"TFGe"	}	},
	{ MSTR_ELECTROLYTES				,	{	"Electrolytes",																		"Électrolytes",																									"Elektrolyte",																										"Electrolitos",																							"Elettroliti",																										"Eletrólitos",																								"Eletrólitos"	}	},
	{ MSTR_ENABLED					,	{	"Enabled",																			"Activé",																										"Aktiviert",																										"Activado",																								"Attivato",																											"Ativado",																									"Ativado"	}	},
	{ MSTR_ENTER_AGE_RANGE			,	{	"Enter Age Range",																	"Saisir fourchette d'âge",																						"Altersbereich eingeben",																							"Introducir intervalo de edad",																			"Inserisci fascia d'età",																							"Introduzir intervalo de idades",																			"Inserir faixa etária"	}	},
	{ MSTR_ENTER_PATIENT_ID			,	{	"Enter Patient ID",																	"Entrer ID  patient",																							"Patienten-ID eingeben",																							"Introducir Id. del paciente",																			"Inserisci ID paziente",																							"Introduzir ID de doente",																					"Inserir ID do paciente"	}	},
	{ MSTR_ENZYMES					,	{	"Enzymes",																			"Enzymes",																										"Enzyme",																											"Encimas",																								"Enzimi",																											"Enzimas",																									"Enzimas"	}	},
	{ MSTR_ERROR_READING_CD			,	{	"Error reading CD",																	"Erreur lecture CD",																							"Fehler beim Lesen der CD",																							"Error al leer el CD",																					"Errore di lettura CD",																								"Erro de leitura do CD",																					"Erro ao ler o CD"	}	},
	{ MSTR_ERROR_REPORT				,	{	"Error Report",																		"Rapport erreur",																								"Fehlerbericht",																									"Informe de errores",																					"Rapporto di errore",																								"Relatório de erros",																						"Relatório de erros"	}	},
	{ MSTR_ERRORS					,	{	"Errors",																			"Erreurs",																										"Fehler",																											"Errores",																								"Errori",																											"Erros",																									"Erros"	}	},
	{ MSTR_EXPIRATION_DATE			,	{	"Expiration Date",																	"Date de péremption",																							"Ablaufdatum",																										"Fecha de vencimiento",																					"Data di scadenza",																									"Prazo de validade",																						"Data de expiração"	}	},
	{ MSTR_EXTERNAL_ARCHIVE			,	{	"External Archive",																	"Archive externe",																								"Externes Archiv",																									"Archivo externo",																						"Archivio esterno",																									"Ficheiro externo",																							"Arquivo externo"	}	},
	{ MSTR_EXTERNAL_PRINTER			,	{	"External Printer",																	"Imprimante externe",																							"Externer Drucker",																									"Impresora externa",																					"Stampante esterna",																								"Impressora externa",																						"Impressora externa"	}	},
	{ MSTR_FACTORY_DEFAULT_SPECIES	,	{	"Factory Default Demographics",														"Param./défaut élmts démogr.",																					"Voreingestellte Demographien",																						"Demografía predeterminada",																			"Demografia predefinita di fabbrica",																				"Valores de demografia de fábrica",																			"Dados demogr. padrão de fábrica"	}	},
	{ MSTR_FEMALE					,	{	"Female",																			"Féminin",																										"Weiblich",																											"Mujer",																								"Uomo",																												"Feminino",																									"Sexo feminino"	}	},
	{ MSTR_FLASHES					,	{	"Flashes",																			"Flashs",																										"Blinkzeichen",																										"Flashes",																								"Flash luminosi",					  																				"Flashes",																									"Flashes"	}	},
	{ MSTR_FRACTION					,	{	"Fraction",																			"Fraction",																										"Anteil",																											"Fracción",																								"Frazione",																											"Fração",																									"Fração"	}	},
	{ MSTR_FULL						,	{	"Full",																				"Complet",																										"Voll",																												"Lleno",																								"Completo",																											"Completo",																									"Cheio"	}	},
	{ MSTR_GATEWAY_IP				,	{	"Gateway IP",																		"IP de passerelle",																								"Gateway-IP",																										"IP puerta enlace",																						"IP gateway",																										"IP da porta",																								"IP do gateway"	}	},
	{ MSTR_GENDER					,	{	"Gender",																			"Sexe",																											"Geschlecht",																										"Sexo",																									"Sesso",																											"Sexo",																										"Gênero"	}	},
	{ MSTR_HISPANIC					,	{	"Hispanic",																			"Hispanique",																									"Hispanische Abst.",																								"Hispano",																								"Ispanico",																											"Hispânico",																								"Hispânico"	}	},
	{ MSTR_HOUR						,	{	"Hour",																				"Heure",																										"Stunde",																											"Hora",																									"Ora",																												"Hora",																										"Hora"	}	},
	{ MSTR_HR						,	{	"hr",																				"h",																											"Std.",																												"hr",																									"Or",																												"h",																										"h"	}	},
	{ MSTR_INFANT					,	{	"Infant",																			"Nourrisson",																									"Säugling",																											"Niño",																									"Cucciolo",																											"Criança",																									"Lactente"	}	},
	{ MSTR_INSERT_CD				,	{	"Insert CD, close drawer, and press Continue",										"Insérer CD, fermer tiroir et appuyer sur Continuer",															"CD einlegen, Fach schließen und Weiter drücken",																	"Insertar CD, cerrar bandeja y pulsar Continuar",														"Inserisci CD, chiudi cassetto e premi Continua",																	"Insira o CD, feche a gaveta e prima Continuar",															"Insira o CD, feche a gaveta e pressione Continuar"	}	},
	{ MSTR_INTERNAL_ARCHIVE			,	{	"Internal Archive",																	"Archive interne",																								"Internes Archiv",																									"Archivo interno",																						"Archivio interno",																									"Ficheiro interno",																							"Arquivo interno"	}	},
	{ MSTR_IP_MASK					,	{	"Subnet Mask",																		"Masq. sous-rés",																								"Subnetzmaske",																										"Másc. de subred",																						"Masch. sottorete",																									"Máscara de subrede",																						"Máscara de sub-rede"	}	},
	{ MSTR_IQC						,	{	"iQC",																				"iQC",																											"iQC",																												"iQC",																									"iQC",																												"iQC",																										"iQC"	}	},
	{ MSTR_IS_NOT_VALID				,	{	"is not valid",																		"incorrect",																									"ungültig",																											"no es válido",																							"non valido",																										"inválido",																									"não é válido"	}	},
	{ MSTR_KENNEL_NUMBER			,	{	"Kennel Number",																	"Numéro chenil",																								"Zwingernr.",																										"Número de caseta",																						"Numero Kennel",																									"Número de Kennel",																							"Número do canil"	}	},
	{ MSTR_KEYBOARD					,	{	"Keyboard",																			"Clavier",																										"Eingabegerät",																										"Teclado",																								"Tastiera",																											"Teclado",																									"Teclado"	}	},
	{ MSTR_LAST_ROTOR				,	{	"Last Disc",																		"Dernier disque",																								"Letzte Scheibe",																									"Último disco",																							"Ultimo disco",																										"Último disco",																								"Último disco"	}	},
	{ MSTR_LAST_MODIFIED			,	{	"Last Modified",																	"Dernière modif.",																								"Zuletzt geändert",																									"Último cambio",																						"Ultima modifica",																									"Última alteração",																							"Última modificação"	}	},
	{ MSTR_LIPIDS					,	{	"Lipids",																			"Lipides",																										"Lipide",																											"Lípidos",																								"Lipidi",																											"Lípidos",																									"Lipídios"	}	},
	{ MSTR_LOAD_FUSE_SETTINGS		,	{	"Load Fuse Settings", 																"Charger les paramètres de fusible",																			"Sicherungseinstellungen laden",																					"Cargar configuración del fusible",																		"Carica impostazioni fusibile",																						"Carregar configurações de fusíveis",																		"Carregar configurações de fusível"	}	},
	{ MSTR_LOAD_ROTOR_TO_ANALYZE	,	{	"To analyze additional sample, load new disc",										"Pour analyser un échantillon supplémentaire, charger un nouveau disque",										"Zur Analyse einer weiteren Probe neue Scheibe laden",																"Para analizar otra muestra, cargue un disco nuevo",													"Per esaminare ulteriori campioni, caricare nuovo disco",															"Para analisar outra amostra, carregue um novo disco",														"Para analisar uma amostra adicional, carregue um novo disco"	}	},
	{ MSTR_LOCATION					,	{	"Location",																			"Emplacement",																									"Ort",																												"Ubicación",																							"Sede",																												"Localização",																								"Local"	}	},
	{ MSTR_LOCKOUT					,	{	"Lockout",																			"Verrouillage",																									"Verriegelung",																										"Bloqueo",																								"Serrata",																											"Bloqueio",																									"Bloquear"	}	},
	{ MSTR_LOWER					,	{	"Lower",																			"Inférieur",																									"Niedriger",																										"Inferior",																								"Basso",																											"Inferior",																									"Inferior"	}	},
	{ MSTR_MALE						,	{	"Male",																				"Masculin",																										"Männlich",																											"Hombre",																								"Donna",																											"Masculino",																								"Sexo masculino"	}	},
	{ MSTR_MANUAL_SETTINGS			,	{	"Manual Settings", 																	"Paramètres manuels",																							"Manuelle Einstellungen",																							"Configuración manual",																					"Impostazioni manuali",																								"Configurações manuais",																					"Configurações manuais"	}	},
	{ MSTR_MIN						,	{	"min",																				"min",																											"min.",																												"min",																									"min",																												"min",																										"min"	}	},
	{ MSTR_MINERALS					,	{	"Minerals",																			"Minéraux",																										"Mineralien",																										"Minerales",																							"Minerali",																											"Minerais",																									"Minerais"	}	},
	{ MSTR_MINUTE					,	{	"Minute",																			"Minute",																										"Minute",																											"Minuto",																								"Minuto",																											"Minuto",																									"Minuto"	}	},
	{ MSTR_MONTH					,	{	"Month",																			"Mois",																											"Monat",																											"Mes",																									"Mese",																												"Mês",																										"Mês"	}	},
	{ MSTR_MOS						,	{	"Mos.",																				"Mois",																											"Monate",																											"Meses",																								"Meses",																											"Meses",																									"Meses"	}	},
	{ MSTR_NAME						,	{	"Name:",																			"Nom :",																										"Name:",																											"Nombre:",																								"Nome:",																											"Nome:",																									"Nome:"	}	},
	{ MSTR_NATIVE_AMERICAN			,	{	"Native American",																	"Amérindienne",																									"Indianische Abst.",																								"Americano nativo",																						"Nativo americano",																									"Americano nativo",																							"Americano nativo"	}	},
	{ MSTR_NEONATE					,	{	"Neonate",																			"Nouveau-né",																									"Neugeborenes",																										"Neonato",																								"Neonato",																											"Recém-nascido",																							"Neonato"	}	},
	{ MSTR_NEUTERED					,	{	"Neutered",																			"Châtré",																										"Kastriert",																										"Castrado",																								"Castrato",																											"Castrado",																									"Castrado"	}	},
	{ MSTR_NEVER					,	{	"Never",																			"Jamais",																										"Nie",																												"Nunca",																								"Mai",																												"Nunca",																									"Nunca"	}	},
	{ MSTR_NEW						,	{	"New",																				"Nouveau",																										"Neu",																												"Nuevo",																								"Nuovo",																											"Novo",																										"Novo"	}	},
	{ MSTR_NO_RESULTS				,	{	"No saved results to view. Please run a disc prior to recalling results.",			"Aucun résultat enregistré à afficher. Exécuter un disque avant de rappeler les résultats.",					"Keine gespeicherten Ergebnisse zur Anzeige. Vor Wiederaufrufen der Ergebnisse eine Scheibe starten.",				"No hay resultados guardados para ver. Utilice un disco antes de buscar resultados.",					"Nessun risultato salvato da visualizzare. Avviare un disco prima di richiamare i risultati.",						"No existem resultados guardados para visualizar. Utilize um disco antes de recuperar resultados.",			"Nenhum resultado salvo para visualização. Execute um disco antes de recuperar os resultados."	}	},
	{ MSTR_NON_SI					,	{	"Non SI",																			"Non SI",																										"Kein SI",																											"No SI",																								"Non SI",																											"Não SI",																									"Não SI"	}	},
	{ MSTR_NONE						,	{	"None",																				"Aucun",																										"Kein",																												"Ninguno",																								"Nessuno",																											"Nenhum",																									"Nenhum"	}	},
	{ MSTR_OFF						,	{	"Off",																				"Off",																											"Aus",																												"Apagado",																								"Off",																												"Desligado",																								"Desligado"	}	},
	{ MSTR_OPENING_DRAWER			,	{	"Opening Drawer...",																"Ouverture tiroir en cours...",																					"Fach öffnen ...",																									"Abriendo bandeja...",																					"Apertura cassetto in corso...",																					"A abrir a gaveta...",																						"Abrindo gaveta..."	}	},
	{ MSTR_OPERATOR_ID				,	{	"Operator ID",																		"ID opérateur",																									"Bediener-ID",																										"Id. operario:",																						"ID operatore",																										"ID do operador",																							"ID do operador"	}	},
	{ MSTR_OPERATORS				,	{	"Operators",																		"Opérateurs",																									"Bediener",																											"Operarios",																							"Operatori",																										"Operadores",																								"Operadores"	}	},
	{ MSTR_OWNER_ID					,	{	"Owner ID",																			"ID propriétaire",																								"Besitzer-ID",																										"Id. dueño",																							"ID propr.",																										"ID do proprietário",																						"ID do proprietário"	}	},
	{ MSTR_PATIENT_CONTROL_ID		,	{	"Patient/Control ID",																"ID patient/contrôle",																							"Patienten-/Kontrollen-ID",																							"Id. paciente/control",																					"ID paziente/controllo",																							"ID do doente/controlo",																					"ID do paciente/controle"	}	},
	{ MSTR_PATIENT_ID				,	{	"Patient ID",																		"ID patient",																									"Patienten-ID",																										"Id. paciente",																							"ID paziente",																										"ID do doente",																								"ID do paciente"	}	},
	{ MSTR_PATIENT_TYPE				,	{	"Patient Type",																		"Type de patient",																								"Patiententyp",																										"Tipo de paciente",																						"Tipo di paziente",																									"Tipo de doente",																							"Tipo de paciente"	}	},
	{ MSTR_PATIENTS					,	{	"Patients",																			"Patients",																										"Patienten",																										"Pacientes",																							"Pazienti",																											"Doentes",																									"Pacientes"	}	},
	{ MSTR_PENDING_ORDER			,	{	"Pending Order",																	"Ordre en cours",																								"Ausstehende Anforderung",																							"Pedido pendiente",																						"Ordine in sospeso",																								"Pedido pendente",																							"Solicitação pendente"	}	},
	{ MSTR_PHONE_NUMBER				,	{	"Phone Number",																		"N° tél",																										"Telefonnummer",																									"Número de teléfono",																					"Num. telefono",																									"Número de telefone",																						"Número de telefone"	}	},
	{ MSTR_PLEASE_REMOVE_ROTOR		,	{	"Please remove the disc from the analyzer",											"Retirer le disque de l'analyseur",																				"Bitte die Scheibe aus dem Analysegerät entfernen",																	"Retire el disco del analizador",																		"Rimuovere il disco dall'analizzatore",																				"Retire o disco do analisador",																				"Remova o disco do analisador"	}	},
	{ MSTR_PLEASE_WAIT_CDUPDATE		,	{	"Please wait. Do not remove power. Analyzer will restart when update is complete.", "Veuillez patienter. Ne pas couper l'alimentation. L'analyseur redémarre une fois la mise à jour terminée.",	"Bitte warten. Bitte Stromversorgung nicht abschalten. Analysegerät wird nach vollständigem Update neu gestartet.",	"Espere, por favor. No apague el aparato. El analizador se reiniciará cuando acabe la actualización.",	"Si prega di attendere. Non togliere corrente. L'analizzatore si riavvierà al completamento dell'aggiornamento.",	"Espere, por favor. Não desligue. O analisador será reiniciado quando terminar a atualização.",				"Aguarde. Não remova a energia. O analisador será reiniciado quando a atualização concluir."	}	},
	{ MSTR_POTC_1A					,	{	"POTC-1A",																			"POTC-1A",																										"POTC-1A",																											"POTC-1A",																								"POTC-1A",																											"POTC-1A",																									"POTC-1A"	}	},
	{ MSTR_POWER_SAVE				,	{	"Power Save",																		"Écono. alim.",																									"Energiesparmodus",																									"Ahorro de energía",																					"Modo risparmio",																									"Poupança de energia",																						"Economia de energia"	}	},
	{ MSTR_PREMATURE				,	{	"Premature",																		"Prématuré",																									"Vorzeitig",																										"Prematuro",																							"Prematuro",																										"Prematuro",																								"Prematuro"	}	},
	{ MSTR_PRESS_YES_DEFAULTS		,	{	"Press Yes to return all settings to factory defaults",								"Appuyer sur Oui pour rétablir tous les paramètres par défaut",													"Drücken Sie Ja, um die Werkseinstellungen wieder herzustellen",													"Pulse Sí para reestablecer la configuración predeterminada original",									"Premere Sì per ripristinare tutte le impostazioni di fabbrica",													"Prima Sim para reestabelecer a configuração para os valores predeterminados de fábrica",					"Pressione Sim para retornar todas as configurações aos padrões de fábrica"	}	},
	{ MSTR_PRESS_YES_REF_RANGES		,	{	"Press Yes to return the active demographics list to the factory default",			"Appuyer sur Oui pour rétablir la liste d'éléments démographiques actifs par défaut",							"Drücken Sie Ja, um die Werkseinstellungen für die Liste der aktiven Demographien wieder herzustellen",				"Pulse Sí para reestablecer los valores predeterminados originales de la lista de demografías activas",	"Premere Sì per mandare l'elenco demografie attive nelle impostazioni di fabbrica",									"Prima Sim para reestabelecer a lista de demografias ativa para os valores predeterminados de fábrica",		"Pressione Sim para retornar a lista de dados demográficos ativos ao padrão de fábrica"	}	},
	{ MSTR_PRESS_YES_TO_START		,	{	"Press Yes to start",																"Appuyer sur Oui pour redémarrer",																				"Drücken Sie Ja für Start",																							"Pulse Sí para empezar",																				"Premere Sì per avviare",																							"Prima Sim para iniciar",																					"Pressione Sim para começar"	}	},
	{ MSTR_PRINT					,	{	"Print",																			"Impr.",																										"Drucken",																											"Imprimir",																								"Stampa",																											"Imprimir",																									"Imprimir"	}	},
	{ MSTR_PRINTED					,	{	"Printed",																			"Imprimé",																										"Gedruckt",																											"Impreso",																								"Stampato",																											"Impresso",																									"Impresso"	}	},
	{ MSTR_PRINTER					,	{	"Printer",																			"Imprimante",																									"Drucker",																											"Impr.",																								"Stampante",																										"Impressora",																								"Impressora"	}	},
	{ MSTR_PROTEINS					,	{	"Proteins",																			"Protéines",																									"Proteine",																											"Proteínas",																							"Proteine",																											"Proteínas",																								"Proteínas"	}	},
	{ MSTR_RACE						,	{	"Race",																				"Race",																											"Rasse",																											"Raza",																									"Razza",																											"Corrida",																									"Raça"	}	},
	{ MSTR_READING_CD				,	{	"Reading CD...",																	"Lecture du CD...",																								"CD wird gelesen...",																								"Leyendo CD...",																						"Lettura CD...",																									"A ler o CD...",																							"Lendo CD..."	}	},
	{ MSTR_RECALL_RESULTS			,	{	"Recall Results", 																	"Rappeler résultats",																							"Ergebnisse neu abrufen",																							"Buscar resultados",																					"Recupera i risultati",																								"Recuperar resultados",																						"Recuperar resultados"	}	},
	{ MSTR_REMOVE_SPECIES			,	{	"Remove Demographic",																"Retirer éléments démographiques",																				"Demographien entfernen",																							"Eliminar demografía",																					"Rimuovi demografia",																								"Eliminar demografia",																						"Remover dado demográfico"	}	},
	{ MSTR_RESULTS					,	{	"Results",																			"Résultats",																									"Ergebnisse",																										"Result.",																								"Risultati",																										"Resultados",																								"Resultados"	}	},
	{ MSTR_RESULTS_QUEUED			,	{	"Results Queued",																	"Résultats en file d'attente",																					"Zu übertragende Ergebnisse",																						"Resultados en cola",																					"Risultati accodati",																								"Resultados em fila",																						"Resultados em fila"	}	},
	{ MSTR_RETRIEVE					,	{	"Retrieve",																			"Récupérer",																									"Abrufen",																											"Recuperar",																							"Recupera",																											"Recuperar",																								"Recuperar"	}	},
	{ MSTR_ROTORS_RUN				,	{	"Discs Run",																		"Exécution disques",																							"Scheiben in Betrieb",																								"Usar discos",																							"Disco in funzione",																								"Utilizar discos",																							"Execução de discos"	}	},
	{ MSTR_SAMPLE_ID				,	{	"Sample ID",																		"ID échantillon",																								"Proben-ID",																										"Id. de muestra",																						"ID campione",																										"ID da amostra",																							"ID da amostra"	}	},
	{ MSTR_SAVING_RESULTS			,	{	"Saving Results...",																"Enregistrement des résultats en cours...",																		"Ergebnisse werden gespeichert...",																					"Guardando resultados...",																				"Salvataggio risultati...",																							"A guardar os resultados...",																					"Salvando resultados..."	}	},
	{ MSTR_SCREEN_CLICKS			,	{	"Screen Clicks",																	"Clics écran",																									"Bildschirmklicks",																									"Clics de pantalla",																					"Clic schermo",																										"Cliques no ecrã",																							"Cliques de tela"	}	},
	{ MSTR_SCREEN_SAVER				,	{	"Screen Saver",																		"Économiseur d'écran",																							"Bildschirmschoner",																								"Salvapantalla",																						"Screensaver",																										"Proteção do ecrã",																							"Protetor de tela"	}	},
	{ MSTR_SEARCH					,	{	"Search",																			"Rechercher",																									"Suchen",																											"Búsqueda",																								"Cerca",																											"Pesquisar",																								"Pesquisar"	}	},
	{ MSTR_SELECT					,	{	"Select",																			"Sélectionner",																									"Auswählen",																										"Seleccionar",																							"Seleziona",																										"Selecionar",																								"Selecionar"	}	},
	{ MSTR_SELECT_REPORTS			,	{	"Select Reports",																	"Sélectionner rapports",																						"Berichte auswählen",																								"Seleccionar informes",																					"Seleziona rapporti",																								"Selecionar relatórios",																					"Selecionar relatórios"	}	},
	{ MSTR_SEND						,	{	"Send",																				"Envoyer",																										"Senden",																											"Enviar",																								"Invia",																											"Enviar",																									"Enviar"	}	},
	{ MSTR_SENDING					,	{	"Sending...",																		"Envoi en cours...",																							"Sendet ...",																										"Enviando...",																							"Invio in corso...",																								"A enviar...",																								"Enviando..."	}	},
	{ MSTR_SENDING_REPORT			,	{	"Sending Report...",																"Envoi de rapport en cours...",																					"Sende Bericht ...",																								"Enviando informe...",																					"Invio rapporto in corso...",																						"A enviar relatório...",																					"Enviando relatório..."	}	},
	{ MSTR_SERIAL_NO				,	{	"Serial No.:",																		"N° de série :",																								"Seriennr.:",																										"N° de serie:",																							"Numero di serie:",																									"N.° de série:",																							"No. de série:"	}	},
	{ MSTR_SERVER_IP				,	{	"Server IP",																		"IP du serveur",																								"Server-IP",																										"IP de servidor",																						"IP server",																										"IP do servidor",																							"IP do servidor"	}	},
	{ MSTR_SERVER_SOCKET			,	{	"Server Socket",																	"Socket serveur",																								"Server-Socket",																									"Socket servidor",																						"Socket server",																									"Tomada do servidor",																						"Soquete do servidor"	}	},
	{ MSTR_SERVICE_ARCHIVE			,	{	"Service Archive",																	"Archive service",																								"Service-Archiv",																									"Archivo de servicio",																					"Archivio di servizio",																								"Ficheiro do serviço",																						"Arquivo de serviço"	}	},
	{ MSTR_SERVICE_ARCHIVE_ELLIPSIS	,	{	"Service Archive...",																"Archive service...",																							"Service-Archiv ...",																								"Archivo de servicio...",																				"Archivio di servizio...",																							"Ficheiro do serviço...",																					"Arquivo de serviço..."	}	},
	{ MSTR_SET_DEFAULT				,	{	"Set Default",																		"Déf. param./défaut",																							"Standard einstellen",																								"Establ. predeterm.",																					"Imposta predef.",																									"Configurar valores predeterminados",																		"Configurar padrão"	}	},
	{ MSTR_SETTINGS					,	{	"Settings",																			"Paramètres",																									"Einstellungen",																									"Configuración",																						"Impostazioni",																										"Configurações",																							"Configurações"	}	},	
	{ MSTR_SI						,	{	"SI",																				"SI",																											"SI",																												"SI",																									"SI",																												"SI",																										"SI"	}	},
	{ MSTR_SINGLE_ANALYTE			,	{	"Single Analyte",																	"1 seule subs. à analyser",																						"Einzelnes Analyt",																									"Analito único",																						"Analita singolo",																									"Analito único",																							"Analito único"	}	},
	{ MSTR_SPAYED					,	{	"Spayed",																			"Châtrée",																										"Sterilisiert",																										"Castrada",																								"Sterilizzato",																										"Esterilizado",																								"Esterilizado"	}	},
	{ MSTR_SPECIAL					,	{	"Special",																			"Spécial",																										"Spezial",																											"Especial",																								"Speciale",																											"Especial",																									"Especial"	}	},
	{ MSTR_SPECIES					,	{	"Demographic",																		"Élmts démogr.",																								"Demographien",																										"Demografía",																							"Demografia",																										"Demografia",																								"Dado demográfico"	}	},
	{ MSTR_STATUS					,	{	"Status",																			"État",																											"Status",																											"Estado",																								"Stato",																											"Estado",																									"Status"	}	},
	{ MSTR_TCPIP					,	{	"TCP/IP",																			"TCP/IP",																										"TCP/IP",																											"TCP/IP",																								"TCP/IP",																											"TCP/IP",																									"TCP/IP"	}	},
	{ MSTR_TEST						,	{	"Test",																				"Test",																											"Test",																												"Prueba",																								"Prova",																											"Teste",																									"Teste"	}	},
	{ MSTR_TESTING_CONNECTION		,	{	"Testing connection...",															"Test de la connexion en cours...",																				"Verbindung wird geprüft...",																						"Probando conexión...",																					"Test della connessione...",																						"A testar ligação...",																						"Testando conexão..."	}	},
	{ MSTR_THIS_ANALYZER			,	{	"This Analyzer",																	"Cet analyseur",																								"Dieses Analysegerät",																								"Este analizador",																						"Questo analizzatore",																								"Este analisador",																							"Este analisador"	}	},
	{ MSTR_TODDLER					,	{	"Toddler",																			"Bébé",																											"Kleinkind",																										"Párvulo",																								"Bambino ai primi passi",																							"Bebé",																										"Criança"	}	},
	{ MSTR_TOUCH_SCREEN_TO_CALIB	,	{	"Touch screen to calibrate or wait to cancel.",										"Toucher l'écran pour calibrer ou attendre pour annuler.",														"Zur Kalibrierung Bildschirm berühren oder Abbruch abwarten.",														"Toque la pantalla para calibrar o espere para cancelar.",												"Toccare lo schermo per calibrare, oppure attendere per annullare.",												"Toque no ecrã para calibrar ou espere para cancelar.",														"Toque na tela para calibrar ou aguarde para cancelar."	}	},
	{ MSTR_TRANSMIT_ALL				,	{	"Transmit All",																		"Transmettre tout",																								"Alle übertragen",																									"Transmitir todo",																						"Trasmetti tutti",																									"Transmitir tudo",																							"Transmitir todos"	}	},
	{ MSTR_TRANSMIT_DATE_RANGE		,	{	"Transmit Date Range",																"Transm. plage de dates",																						"Datumsbereich übertr.",																							"Trans. intervalo/fechas",																				"Trasmetti range di date",																							"Transmitir intervalo de data",																				"Transmitir intervalo de datas"	}	},
	{ MSTR_TRANSMITTING_ELLIPSIS	,	{	"Transmitting...",																	"Transmission...",																								"Übertragung...",																									"Transmitiendo...",																						"Trasmissione in corso...",																							"A transmitir...",																							"Transmitindo..."	}	},
	{ MSTR_TRANSMITTING_ALL			,	{	"Transmitting All...",																"Trans. en cours...",																							"Alle werden übertr. ...",																							"Transmitiendo todo...",																				"Trasm. tutti in corso...",																							"A transmitir tudo...",																						"Transmitindo todos..."	}	},
	{ MSTR_UNITS					,	{	"Units",																			"Unités",																										"Einheiten",																										"Unidades",																								"Unità",																											"Unidades",																									"Unidades"	}	},
	{ MSTR_UNKNOWN					,	{	"Unknown",																			"Inconnu",																										"Unbekannt",																										"Desconocido",																							"Sconosciuto",																										"Desconhecido",																								"Desconhecido"	}	},
	{ MSTR_UPDATE_SOFTWARE			,	{	"Update Software",																	"Mise à jour logiciel",																							"Software aktualisieren",																							"Actualizar programa",																					"Aggiorna software",																								"Atualizar o Software",																						"Atualizar software"	}	},
	{ MSTR_UPDATING_CONTROLS		,	{	"Updating Controls...",																"Actualisation des commandes en cours...",																		"Steuerungen werden aktualisiert ...",																				"Actualizando controles...",																			"Aggiornamento controlli in corso...",																				"A atualizar controlos...",																					"Atualizando controles..."	}	},
	{ MSTR_UPDATING_SOFTWARE		,	{	"Updating Software...",																"Mise à jour logiciel...",																						"Softw. wird aktual. ...",																							"Actualizando programa",																				"Aggior. softw...",																									"A atualizar o Software...",																				"Atualizando software..."	}	},
	{ MSTR_UPPER					,	{	"Upper",																			"Haut",																											"Oben",																												"Superior",																								"Superiore",																										"Superior",																									"Superior"	}	},
	{ MSTR_USB						,	{	"USB (Serial)",																		"USB (série)",																									"USB (ser. Anschl.)",																								"USB (serie)",																							"USB (seriale)",																									"USB (série)",																								"USB (serial)"	}	},
	{ MSTR_VERSION					,	{	"Version:",																			"Version :",																									"Version:",																											"Versión:",																								"Versione:",																										"Versão:",																									"Versão:"	}	},
	{ MSTR_WARMING					,	{	"Warming",																			"Réchauffement",																								"Aufwärmen",																										"Calentando",																							"Riscaldamento",																									"A aquecer",																								"Aquecendo"	}	},
	{ MSTR_WHITE					,	{	"White",																			"Blanc",																										"Weiss",																											"Blanco",																								"Bianco",																											"Branco",																									"Branco"	}	},
	{ MSTR_WKS						,	{	"Wks.",																				"Sem.",																											"Wks.",																												"Sem.",																									"Sett.",																											"Sem.",																										"Semanas"	}	},
	{ MSTR_WORK_LIST				,	{	"Work List",																		"Liste de tâches",																								"Auftragsliste",																									"Lista de trabajo",																						"Elenco di lavoro",																									"Lista de trabalho",																						"Lista de trabalho"	}	},
	{ MSTR_XMIT						,	{	"Xmit",																				"Trans",																										"Xmit",																												"Transm.",																								"Xmit",																												"Transm.",																									"Transmit"	}	},
	{ MSTR_XML						,	{	"XML",																				"XML",																											"XML",																												"XML",																									"XML",																												"XML",																										"XML"	}	},
	{ MSTR_YEAR						,	{	"Year",																				"Année",																										"Jahr",																												"Año",																									"Anno",																												"Ano",																										"Ano"	}	},
	{ MSTR_YRS						,	{	"Yrs.",																				"Ans.",																											"Jahre",																											"Años",																									"Anni",																												"Anos",																										"Anos"	}	},
	{ MSTR_ZERO_SEC					,	{	"Zero Sec",																			"Zéro sec",																										"Null Sek.",																										"Cero seg",																								"Zero Sec",																											"Zero seg",																									"Zero seg"	}	},

// Bottom									ENGLISH,				FRENCH,							GERMAN,						SPANISH,				ITALIAN, 					PORTUGUESE,				PORTUGUESE (BRAZIL)
	{ BSTR_ADD						,	{	"Add",					"Ajouter",						"Hinzufügen",				"Añadir",				"Aggiungi",					"Adicionar",			"Adicionar"	}	},
	{ BSTR_BACK						,	{	"Back",					"Retour",						"Zurück",					"Volver",				"Indietro",					"Voltar",				"Voltar"	}	},
	{ BSTR_CANCEL					,	{	"Cancel",				"Annuler",						"Abbrechen",				"Cancelar",				"Annulla",					"Cancelar",				"Cancelar"	}	},
	{ BSTR_CLOSE 					,	{	"Close",				"Fermer",						"Schließen",				"Cerrar",				"Chiudi",					"Fechar",				"Fechar"	}	},
	{ BSTR_CONFIRM					,	{	"Confirm",				"Confirmer",					"Bestätigen",				"Confirmar",			"Conferma",					"Confirmar",			"Confirmar"	}	},
	{ BSTR_CONTINUE					,	{	"Continue",				"Continuer",					"Fortsetzen",				"Continuar",			"Continua",					"Continuar",			"Continuar"	}	},
	{ BSTR_DATE						,	{	"Date",					"Date",							"Datum",					"Fecha",				"Data",						"Data",					"Data"		}	},
	{ BSTR_DONE						,	{	"Done",					"Terminé",						"Erledigt",					"Terminado",			"Fatto",					"Terminar",				"Concluído"	}	},
	{ BSTR_ENTER					,	{	"Enter",				"Entrée",						"Eingabe",					"Entrar",				"Invio",					"Introduzir",			"Entrar"	}	},
	{ BSTR_EXIT						,	{	"Exit",					"Quit",							"Exit",						"Salir",				"Esci",						"Sair",					"Sair"		}	},
	{ BSTR_HOME						,	{	"Home ",				"Accueil",						"Home",						"Inicio",				"Home",						"Início",				"Início"	}	},
	{ BSTR_NEXT						,	{	"Next",					"Suivant",						"Weiter",					"Sig.",					"Successivo",				"Seguinte",				"Próximo"	}	},
	{ BSTR_NO						,	{	"No",					"Non",							"Nein",						"No",					"No",						"Não",					"Não"		}	},
	{ BSTR_OPEN						,	{	"Open",					"Ouvrir",						"Öffnen",					"Abrir",				"Aperto",					"Abrir",				"Abrir"		}	},
	{ BSTR_PERFORMING_IQC			,	{	"Performing iQC...",	"Exécution iQC en cours...",	"iQC wird ausgeführt...",	"Realizando iQC...",	"Esecuz. iQC in corso...",	"A executar iQC...",	"Executando iQC..."	}	},
	{ BSTR_PRINT					,	{	"Print",				"Impr.",						"Drucken",					"Impr.",				"Stampa",					"Imprimir",				"Imprimir"	}	},
	{ BSTR_RESTART					,	{	"Restart",				"Redémarrer",					"Erneut starten",			"Reiniciar",			"Riavvia",					"Reiniciar",			"Reiniciar"	}	},
	{ BSTR_SAVE						,	{	"Save",					"Enreg.",						"Save",						"Guard.",				"Salva",					"Guardar",				"Salvar"	}	},
	{ BSTR_SEARCH					,	{	"Search",				"Rechercher",					"Suchen",					"Búsqueda",				"Cerca",					"Pesquisar",			"Pesquisar"	}	},
	{ BSTR_YES						,	{	"Yes",					"Oui",							"Ja",						"Sí",					"Sì",						"Sim",					"Sim"		}	},

// Info text
	{ ISTR_LOAD_ROTOR_1				,	{	"Remove reagent disc from its foil pouch and transfer 100ul of whole blood, plasma or serum into the disc.\n",					// English
											"Retirer le disque de réactif de sa pochette alu et transférer 100 ul de sang entier, de plasma ou de sérum dans le rotor.\n",	// French
											"Reagenzscheibe aus dem Plastikbeutel entfernen und 100ul Vollblut, Plasma oder Serum in die Scheibe einfüllen.\n",				// German
											"Retire el disco de reactivo de su bolsa metalizada y transfiera 100 ul de sangre, plasma o suero al disco.\n",					// Spanish
											"Rimuovere il disco reagente dalla custodia metallizzata e trasferire 100ul di sangue intero, plasma o siero nel disco.\n",		// Italian
											"Retire o disco de reagente da sua bolsa metalizada e transfira 100 ul de sangue total, plasma ou soro para o disco.\n",		// Portuguese
											"Remova o disco de reagente da bolsa de alumínio e transfira 100 ul de sangue total, plasma ou soro para o disco.\n"			// Portuguese (Brazil)
										} 
	},
	{ ISTR_LOAD_ROTOR_2				,	{	"Insert the reagent disc into the analyzer, close the drawer and follow the on-screen prompts.\n",								// English
											"Insérer le disque de réactif dans l'analyseur, fermer le tiroir et suivre les instructions qui s'affichent à l'écran.\n",		// French
											"Reagenzscheibe in das Analysegerät einsetzen, Fach schließen und Bildschirmanweisungen befolgen.\n",							// German
											"Inserte el disco de reactivo en el analizador, cierre el cajón y siga las instrucciones de pantalla.\n",						// Spanish
											"Inserire il disco reagente nell'analizzatore, chiudere il cassetto e seguire le istruzioni a schermo.\n",						// Italian
											"Insira o disco de reagente no analisador, feche a gaveta e siga as instruções do ecrã.\n",										// Portuguese
											"Insira o disco de reagente no analisador, feche a gaveta e siga as instruções na tela.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_LOAD_ROTOR_3				,	{	"View, read or download results.\n",				// English
											"Afficher, lire et télécharger les résultats.\n",	// French
											"Ergebnisse ansehen, lesen oder herunterladen.\n",	// German
											"Visualice, lea o descargue resultados.\n",			// Spanish
											"Visualizzare, leggere o scaricare i risultati.\n",	// Italian
											"Visualize, leia ou descarregue os resultados.\n",	// Portuguese
											"Visualizar, ler ou baixar resultados.\n"			// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_RESTORE			,	{	"Restores reference ranges to a particular date and time. The restored reference ranges replace the current reference ranges. All changes after the date reference ranges were archived will be lost. It is recommended that an archive of the current reference ranges be made prior to restoring an earlier set.\n",																				// English
											"Rétablit les fourchettes de référence sur une date et une heure données. Les fourchettes de référence rétablies remplacent les fourchettes de référence actuelles. Tous les changements effectués après l'archivage des fourchettes de date de références seront perdus. Il est conseillé de créer une archive des fourchettes de référence actuelles avant de rétablir un ensemble antérieur.\n",	// French
											"Setzt die Referenzbereiche auf ein bestimmtes Datum und Zeit zurück. Die wiederhergestellten Referenzbereiche ersetzen die aktuellen Referenzbereiche. Sämtliche nach der Archivierung der Datumsreferenzbereiche vorgenommenen Änderungen gehen verloren. Es wird empfohlen, die aktuellen Referenzbereiche zu archivieren, vor Wiederherstellung eines früheren Satzes.\n",						// German
											"Recupera los intervalos de referencia de una fecha y hora específicos. Los intervalos de referencia reestablecidos reemplazan a los intervalos de referencia actuales. Se perderán todos los cambios tras la fecha en la que se archivaron los intervalos de referencia. Se recomienda hacer un archivo de los intervalos de referencia actuales antes de reestablecer un grupo anterior.\n",		// Spanish
											"Ripristina i range di riferimento ad una particolare data e ora. I range di riferimento ripristinati sostituiscono i range di riferimento attuali. Tutte le modifiche successive alla data di archiviazione dei range di riferimento andranno perse. Si raccomanda di creare un archivio degli attuali range di riferimento prima di ripristinare un set precedente.\n",							// Italian
											"Restaura os intervalos de referência de una data e hora específicas. Os intervalos de referência restaurados substituem os intervalos de referência atuais. Serão perdidas todas as alterações após a data do arquivo dos intervalos de referência. É recomendado realizar um arquivo dos intervalos de referência atuais antes de recuperar um conjunto anterior.\n",								// Portuguese
											"Restaura os intervalos de referência para uma data e hora específicas. Os intervalos de referência restaurados substituem os intervalos de referência atuais. Todas as alterações após o arquivamento dos intervalos de referência de datas serão perdidas. É recomendável fazer um arquivo dos intervalos de referência atuais antes de restaurar um conjunto anterior.\n"						// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_SELECT			,	{	"Send: Archive reference ranges for future reference.\n\n"
											"Retrieve: Restore an internal reference range archive.\n",											// English
											"Envoyer : archiver les fourchettes de référence afin de pouvoir s'y reporter ultérieurement.\n\n"
											"Récupérer : restaurer une archive de fourchette de référence interne.\n",							// French
											"Senden: Referenzbereiche für zukünftige Verwendung archivieren.\n\n"
											"Empfangen: Internes Archiv für Referenzbereiche wiederherstellen.\n",								// German
											"Enviar: archivar intervalos de referencia para usos futuros.\n\n"
											"Recuperar: reestablecer un archivo de intervalos de referencia interno.\n",						// Spanish
											"Invia: archivia range di riferimento per utilizzo futuro.\n\n"
											"Recupera: ripristina un archivio di range di riferimento interno.\n",								// Italian
											"Enviar: Guardar intervalos de referência para utilização futura.\n\n"
											"Recuperar: Restaura um ficheiro de intervalos de referência interno.\n",							// Portuguese
											"Enviar: Arquive os intervalos de referência para futura referência.\n\n"
											"Recuperar: Restaure um arquivo de intervalo de referência interno.\n"								// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_SEND				,	{	"Printer: Print a copy of a demographic reference range.\n\n"
											"Internal Archive: Save a copy of all active reference ranges internal to the analyzer for future recall.\n\n"
											"External Archive: Transmit an ASCII dump of a demographic reference range to an external system.\n",																	// English
											"Imprimante : imprimer un exemplaire de fourchette de référence démographique.\n\n"
											"Archive interne : enregistrer un exemplaire de toutes les fourchettes de référence actives internes à l'analyseur afin de pouvoir les récupérer ultérieurement.\n\n"
											"Archive externe : transmettre un vidage de mémoire ASCII d'une fourchette de référence démographique à un système externe.\n",											// French
											"Drucker: Kopie eines Demographie-Referenzbereichs drucken.\n\n"
											"Internes Archiv: Eine Kopie aller aktiven Referenzbereiche für spätere Verwendung intern im Analysegerät speichern.\n\n"
											"Externes Archiv: ASCII-Auszug eines Demographie-Referenzbereichs an ein externes System übertragen.\n",																// German
											"Impresora: imprimir una copia de un intervalo de referencia de demografía.\n\n"
											"Archivo interno: guardar una copia de todos los intervalos de referencia activos internos para el analizador para usos futuros.\n\n"
											"Archivo externo: hacer un volcado de datos ASCII de un intervalo de referencia de demografía a un sistema externo.\n",													// Spanish
											"Stampante: stampa una copia di un range di riferimento per demografia.\n\n"
											"Archivio interno: salva una copia di tutti i range di riferimento attivi interni all'analizzatore per utilizzo futuro.\n\n"
											"Archivio esterno: trasmette una serie ASCII di range di riferimento per demografia ad un sistema esterno.\n",															// Italian
											"Impressora: Imprime uma cópia de um intervalo de referência de demografia.\n\n"
											"Ficheiro interno: Guardar uma cópia de todos os intervalos de referência ativos internos no analisador para utilização futura.\n\n"
											"Ficheiro externo: Transmitir um dump de ASCII de um intervalo de referência de demografia para um sistema externo.\n",													// Portuguese
											"Impressora: Imprima uma cópia de um intervalo de referência de dados demográficos.\n\n"
											"Arquivo interno: Salve uma cópia de todos os intervalos de referência ativos internos no analisador, para futura recuperação.\n\n"
											"Arquivo externo: Transmita um despejo ASCII de um intervalo de referência de dados demográficos para um sistema externo.\n"											// Portuguese (Brazil)
										} 
	},
	{ ISTR_ASTM_ORDER_METHOD		,	{	"Work List: Select the order from a list.\n\n"
											"Enter Patient ID: Select the order from a list then confirm order based on entered patient ID.\n\n"
											"Disabled: ASTM orders are disabled.\n",																												// English
											"Liste de tâches : Sélectionner l'ordre dans une liste.\n\n"
											"Entrer ID patient : Sélectionner l'ordre dans une liste, puis confirmer l'ordre en function de l'ID patient entrée.\n\n"
											"Désactivé : Les orders ASTM sont  désactivés.",																										// French
											"Auftragsliste: Die Anforderung aus einer Liste auswählen.\n\n"
											"Patienten-ID eingeben: Die Anforderung aus einer Liste auswählen und Anforderung anhand der Patienten-ID bestätigen.\n\n"
											"Deaktiviert: ASTM-Anforderungen sind deaktiviert.",																									// German
											"Lista de trabajo: Seleccione el idioma en una lista.\n\n"
											"Introducir Id. del paciente: Seleccione el pedido en una lista y, a continuación, confirme el pedido en función del Id. del paciente introducido.\n\n"
											"Desactivado: Los pedidos ASTM están desactivados.",																									// Spanish
											"Elenco di lavoro: Selezionare l'ordine da un elenco.\n\n"
											"Inserisci ID paziente: Selezionare l'ordine da un elenco, quindi confermarlo in base all'ID paziente inserito.\n\n"
											"Disattivato: Gli ordini ASTM sono disattivati.",																										// Italian
											"Lista de trabalho: Seleciona o idioma de uma lista\n\n"
											"Introduzir ID de doente: Seleciona o pedido de uma lista e, em seguida, confirma o pedido baseado na ID do doente introduzido.\n\n"
											"Inativo: Os pedidos de ASTM estão desativados.\n",																										// Portuguese
											"Lista de trabalho: Selecione a solicitação em uma lista\n\n"
											"Inserir ID do paciente: Selecione a solicitação em uma lista e confirme-a com base no ID do paciente inserido.\n\n"
											"Desativado: As solicitações de ASTM estão desativadas.\n"																								// Portuguese (Brazil)
										}
	},
	{ ISTR_ASTM_PHYSICAL_LAYER		,	{	"USB (Serial): The analyzer is connected from the USB \"B\" port to a local computer using a USB cable.\n\n"
											"TCP/IP: The analyzer is connected using a USB-to-Ethernet adapter to a network.\n",																	// English
											"USB (série) : l'analyseur est connecté du port USB \"B\" à un ordinateur local par un câble USB.\n\n"
											"TCP/IP: l'analyseur est connecté à un réseau par le biais d'un adaptateur USB-Ethernet.\n",															// French
											"USB (serieller Anschluss): Das Analysesystem ist über den USB-Anschluss \"B\" und ein USB-Kabel an einen lokalen Computer angeschlossen.\n\n"
											"TCP/IP: Das Analysegerät ist über einen USB- Ethernet-Adapter an ein Netzwerk angeschlossen.\n",														// German
											"USB (serie): El analizador está conectado desde un puerto USB \"B\" a un ordenador local mediante un cable USB.\n\n"
											"TCP/IP: El analizador está conectado a la red mediante un adaptador USB a Ethernet.\n",																// Spanish
											"USB (seriale): L'analizzatore è connesso dalla porta USB \"B\" a un computer locale tramite un cavo USB.\n\n"
											"TCP/IP: L'analizzatore è connesso alla rete tramite un adattatore USB-Ethernet.\n",																	// Italian
											"USB (série): O analisador está ligado a partir da porta USB \"B\" de um computador local através de um cabo USB.\n\n"
											"TCP/IP: O analisador está ligado a uma rede através de um adaptador USB para Ethernet.\n",																// Portuguese
											"USB (serial): O analisador é conectado da porta USB \"B\" a um computador local usando um cabo USB.\n\n"
											"TCP/IP: O analisador é conectado usando um adaptador USB para Ethernet em uma rede.\n"																	// Portuguese (Brazil)
										}
	},	
	{ ISTR_CD_UPDATE				,	{	"The CD ROM drive is located on the rear of the analyzer. Press the small rectangular button to open the drawer. Insert the CD onto the spindle and make sure it clicks into place. Close the CD ROM drive. Press Continue. After the analyzer has restarted, open the CD ROM drive and remove the CD. Store the CD in a safe place.\n",														// English
											"Le lecteur de CD ROM se trouve à l'arrière de l'analyseur. Appuyer sur le petit bouton rectangulaire pour ouvrir le tiroir. Insérer le CD sur l'axe et vérifier qu'il est bien en place. Fermer le lecteur de CD ROM. Appuyer sur Continuer. Une fois que l'analyseur a redémarré, ouvrir le lecteur de CD ROM et retirer le CD. Ranger le CD en lieu sûr.\n",									// French
											"Das CD-ROM-Laufwerk befindet sich auf der Rückseite des Analysegeräts. Zur Öffnung des Fachs die kleine rechteckige Taste drücken. Die CD in die Spindel einsetzen und einrasten lassen. Schließen Sie das CD-ROM-Laufwerk. Drücken Sie auf Weiter. Nach dem Neustart des Analysegeräts öffnen Sie das CD-ROM-Laufwerk und entnehmen die CD. Verwahren Sie die CD an einem sicheren Ort.\n",	// German
											"La unidad de CD-ROM está en la parte trasera del analizador. Pulse el botón rectangular pequeño para abrir la bandeja. Inserte el CD en el eje y asegúrese de que hace clic cuando se coloca en su sitio. Cierre la unidad de CD-ROM. Pulse Continuar. Después de reiniciar el analizador, abra la unidad de CD-ROM y retire el CD. Guarde el CD en un lugar seguro.\n",						// Spanish
											"Il drive del CD ROM si trova sul retro dell'analizzatore. Per aprire il cassetto premere il piccolo pulsante rettangolare. Inserire il CD sul supporto e assicurarsi che sia ben fissato. Chiudere il drive del CD ROM. Premere Continua. Una volta riavviato l'analizzatore, aprire il drive del CD ROM e rimuovere il CD. Conservare il CD in un luogo sicuro.\n",							// Italian
											"A unidade de CD-ROM está localizada na parte posterior do analisador. Prima o botão retangular pequeno para abrir a gaveta. Insira o CD no eixo e certifique-se de que ouve um clique quando é colocado no lugar. Feche a unidade de CD-ROM. Prima Continuar. Depois de o analisador ter reiniciado, abra a unidade de CD-ROM e retire o CD. Guarde o CD num local seguro.\n",					// Portuguese
											"A unidade de CD-ROM está localizada na parte traseira do analisador. Pressione o pequeno botão retangular para abrir a gaveta. Insira o CD no eixo e verifique se ele se encaixa no lugar. Feche a unidade de CD-ROM. Pressione Continuar. Depois que o analisador reiniciar, abra a unidade de CD-ROM e remova o CD. Guarde o CD em um local seguro.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_COMM_PROTOCOL			,	{	"None: No transmission of information is possible.\n\n"
											"ASCII Text: ASCII text dump of information unidirectional from the analyzer.\n\n"
											"ASTM: ASTM 1394 E 97 over ASTM E 1381 02 Standard Specification for Transferring Information Between Clinical Instruments and Computer Systems.\n\n"
											"XML: Extensible Markup Language output in proprietary Abaxis format.\n",																							// English
											"Aucune : aucune transmission d'information n'est possible.\n\n"
											"Texte ASCII : vidage de texte ASCII d'information unidirectionnelle à partir de l'analyseur.\n\n"
											"ASTM : Spécification des normes ASTM 1394 E 97/ASTM E 1381 02 pour la transmission de données entre des instruments cliniques et des systèmes informatiques.\n\n"
											"XML : Sortie en langage XML (Extensible Markup Language) dans le format propriétaire d'Abaxis.\n",																	// French
											"Keine: Keine Datenübertragung möglich.\n\n"
											"ASCII-Text: ASCII-Textauszug einseitig vom Analysegerät.\n\n"
											"ASTM: ASTM 1394 E 97 über ASTM E 1381 02 Standardspezifikation für die Übertragung von Informationen zwischen klinischen Geräten und Computersystemen.\n\n"
											"XML: Ausgabe als Extensible Language in proprietärem Abaxis-Format.\n",																							// German
											"Ninguna: no es posible realizar la transmisión de la información.\n\n"
											"Texto ASCII: volcado de datos en texto ASCII unidireccional desde el analizador.\n\n"
											"ASTM 1394 E 97 sobre ASTM E 1381 02 Especificación estándar sobre transferencia de información entre instrumentos clínicos y sistemas informáticos.\n\n"
											"XML: Salida en lenguaje de marcado extensible en formato propiedad de Abaxis.\n",																					// Spanish
											"Nessuno: non è possibile alcuna trasmissione di informazioni.\n\n"
											"Testo ASCII: serie unidirezionale di informazioni in testo ASCII dall'analizzatore.\n\n"
											"ASTM: ASTM 1394 E 97 sulla specifica standard ASTM E 1381 02 per il trasferimento di informazioni tra strumenti clinici e computer.\n\n"
											"XML: Output di tipo Extensible Markup Language in formato proprietario Abaxis.\n",																					// Italian
											"Nenhum: Não é possível realizar a transmissão de informação.\n\n"
											"Texto ASCII: Dump de informações de texto ASCII, unidirecional do analisador.\n\n"
											"ASTM: ASTM 1394 E 97 sobre ASTM E 1381 02 Especificação padrão sobre transferência de informação entre instrumentos clínicos e sistemas informáticos.\n\n"
											"XML: Saída em linguagem de marcação extensível em formato propriedade da Abaxis.\n",																				// Portuguese
											"Nenhum: Nenhuma transmissão de informações é possível.\n\n"
											"Texto ASCII: Despejo em texto ASCII de informações unidirecionais do analisador.\n\n"
											"ASTM: ASTM 1394 E 97 via ASTM E 1381 02 Especificação padrão para transferência de informações entre instrumentos clínicos e sistemas de computadores.\n\n"
											"XML: Saída da linguagem de marcação extensível no formato patenteado da Abaxis.\n"																					// Portuguese (Brazil)
										} 
	},
	{ ISTR_DATE_INVALID				,	{	"Only valid calendar days may be entered. Please set the date to today's date.\n",														// English
											"Saisir uniquement des jours civils valides. Définir la date d'aujourd'hui comme date du jour.\n",										// French
											"Nur gültige Kalendertage eingeben. Datumsanzeige auf das heutige Datum einstellen.\n",													// German
											"Solo se pueden introducir días naturales correctos. Cambie la fecha al día de hoy.\n",													// Spanish
											"Possono essere inseriti solo giorni validi. Si prega di impostare la data odierna.\n",													// Italian
											"Apenas podem ser introduzidos dias de calendário válidos. Defina a data para a data do dia de hoje.\n",								// Portuguese
											"Apenas dias corridos válidos podem ser inseridos. Configure a data como a data de hoje.\n"												// Portuguese (Brazil)
										} 
	},
	{ ISTR_DELETE_ADMINISTRATOR		,	{	"Removing an administrator from the administrator list will permanently remove the administrator, and all information for that administrator will be lost.\n",					// English
											"La suppression d'un administrateur dans la liste d'administrateurs a pour effet de le supprimer et toutes les informations associées seront perdues.\n",						// French
											"Die Entfernung eines Administrators aus der Administratorliste entfernt den Administrator dauerhaft, und sämtliche Informationen zu diesem Administrator gehen verloren.\n",	// German
											"Al eliminar un administrador de la lista de administradores se le elimina permanentemente. Se perderá toda la información de ese administrador.\n",							// Spanish
											"La rimozione di un amministratore dall'elenco amministratori sarà permanente e tutte le informazioni per tale amministratore saranno perse.\n",								// Italian
											"Ao eliminar um administrador da lista de administradores irá eliminar permanentemente o administrador, e toda a informação desse administrador será perdida.\n",				// Portuguese
											"A remoção de um administrador da lista de administradores o removerá permanentemente e todas as informações desse administrador serão perdidas.\n"								// Portuguese (Brazil)
										} 
	},
	{ ISTR_DELETE_OPERATOR			,	{	"Removing an operator from the operator list will permanently remove the operator, and all information for that operator will be lost.\n",										// English
											"La suppression d'un opérateur dans la liste d'opérateurs a pour effet de le supprimer et toutes les informations associées seront perdues.\n",									// French
											"Die Entfernung eines Bedieners aus der Bedienerliste entfernt den Bediener dauerhaft, und sämtliche Informationen zu diesem Bediener gehen verloren.\n",						// German
											"Al eliminar un operario de la lista de operarios se le elimina permanentemente. Se perderá toda la información de ese operario.\n",											// Spanish
											"La rimozione di un operatore dall'elenco operatori sarà permanente e tutte le informazioni per tale operatore saranno perse.\n",												// Italian
											"Ao eliminar um operador da lista de operadores irá eliminar permanentemente o operador, e toda a informação desse operador será perdida.\n",									// Portuguese
											"A remoção de um operador da lista de operadores o removerá permanentemente e todas as informações desse operador serão perdidas.\n"											// Portuguese (Brazil)
										} 
	},
	{ ISTR_DISPLAY_SETTINGS			,	{	"Backlight: Controls the backlight level for the display.\n\n"
											"Screen Saver: Sets the time from the last screen press until the screen saver starts.\n\n"
											"Power Save: Sets the time from the last screen press until the display is turned off.\n\n"
											"Calibrate: Aligns the touch screen to the LCD display.\n",																				// English
											"Rétroéclairage : commande le niveau de rétroéclairage de l'écran.\n\n"
											"Économiseur d'écran : définit la durée entre la dernière activation de l'écran et la mise en marche de l'économiseur d'écran.\n\n"
											"Économiseur d'alimentation : définit la durée entre la dernière activation de l'écran et le moment où l'écran est arrêté.\n\n"
											"Calibrage : alignement de l'écran tactile avec l'écran à cristaux liquides.\n",														// French
											"Hintergrundbeleuchtung: Steuert die Hintergrundbeleuchtung des Bildschirms.\n\n"
											"Bildschirmschoner: Zur Einstellung des Zeitintervalls von der letzten Bildschirmberührung bis zum Start des Bildschirmschoners.\n\n"
											"Energiesparmodus: Zur Einstellung der Zeit von der letzten Bildschirmberührung bis zur Abschaltung des Bildschirms.\n\n"
											"Kalibrieren: Richtet den Touch-Screen am LCD-Bildschirm aus.\n",																		// German
											"Luz de fondo: controla el nivel de la luz de fondo de la pantalla.\n\n"
											"Salvapantalla: marca el tiempo desde la última presión sobre la pantalla hasta la activación del salvapantalla.\n\n"
											"Ahorro de energía: marca el tiempo desde la última presión de pantalla hasta que la pantalla se apaga.\n\n"
											"Calibrar: alinea la pantalla táctil a la pantalla de cristal líquido.\n",																// Spanish
											"Retroilluminazione: controlla il livello di retroilluminazione per il display.\n\n"
											"Screensaver: imposta il tempo tra l'ultima pressione sullo schermo e l'avvio dello screensaver.\n\n"
											"Modo risparmio: imposta il tempo tra l'ultima pressione sullo schermo allo spegnimento del display.\n\n"
											"Calibra: allinea il touchscreen al display LCD.\n",																					// Italian
											"Luz de fundo: Controla o nível da luz de fundo do visor.\n\n"
											"Proteção do ecrã: Define o tempo desde a última vez que o ecrã é premido até ao início da proteção do ecrã.\n\n"
											"Poupança de energia: Define o tempo desde a última vez que o ecrã é premido até que o visor é desligado.\n\n"
											"Calibrar: Alinha o ecrã tátil com o visor LCD.\n",																						// Portuguese
											"Luz de fundo: Controla o nível de luz de fundo do visor.\n\n"
											"Protetor de tela: Configura o tempo desde a última pressão de tela até a proteção de tela iniciar.\n\n"
											"Economia de energia: Configura o tempo desde a última pressão de tela até o display desligar.\n\n"
											"Calibrar: Alinha a tela sensível ao toque com o display LCD.\n"																		// Portuguese (Brazil)
										} 
	},
	{ ISTR_FACTORY_DEFAULTS			,	{	"Restoring the analyzer to factory defaults resets the analyzer to the same conditions as when the analyzer left the factory. It is recommended that the reference ranges be archived prior to resetting the analyzer to factory defaults. Saved results and internal reference range archives will still be available after the settings are reset to factory defaults.\n",																						// English
											"Le fait de rétablir les valeurs par défaut de l'analyseur remet l'analyseur en état sortie d'usine. Il est conseillé d'archiver les fourchettes de référence avant de rétablir les valeurs par défaut de l'analyseur. Les résultats enregistrés et les archives des fourchettes de référence internes seront toujours disponibles une fois les paramètres par défaut rétablis.\n",																					// French
											"Die Wiederherstellung der Werkseinstellungen des Analysegeräts stellt die Einstellungen des Analysegeräts auf den Auslieferungszustand zurück. Es wird empfohlen, die Referenzbereiche vor dem Zurücksetzen der Referenzbereiche auf die Werkseinstellungen zu archivieren. Gespeicherte Ergebnisse und interne Referenzbereichsarchive sind auch nach Wiederherstellung der Werkseinstellungen verfügbar.\n",														// German
											"La restauración del analizador a los valores predeterminados originales reestablece las mismas condiciones de fábrica del analizador. Se recomienda archivar los intervalos de referencia antes de reestablecer los valores predeterminados de fábrica del analizador. Los archivos de resultados y los intervalos de referencia internos estarán disponibles después de reestablecer los valores predeterminados originales del analizador.\n",					// Spanish
											"Se si ripristinano le impostazioni predefinite, l'analizzatore ritorna alle condizioni in cui ha lasciato la fabbrica. Si raccomanda di archiviare i range di riferimento prima di resettare l'analizzatore sulle impostazioni di fabbrica. I risultati salvati e gli archivi dei range di riferimento interni saranno disponibili anche dopo aver ripristinato le impostazioni di fabbrica.\n",																	// Italian
											"O restauro do analisador para os valores predeterminados de fábrica restaura o analisador para as mesmas condições do analisador quando saiu da fábrica. É recomendado arquivar os intervalos de referência antes de restaurar o analisador para os valores predeterminados de fábrica. Os ficheiros de resultados e dos intervalos de referência internos continuarão disponíveis após o restauro do analisador para os valores predeterminados de fábrica.\n",	// Portuguese
											"A restauração do analisador para os padrões de fábrica o redefine para as mesmas condições de quando ele saiu da fábrica. É recomendável que os intervalos de referência sejam arquivados antes de redefinir o analisador para os padrões de fábrica. Os resultados salvos e os arquivos do intervalo de referência interno ainda estarão disponíveis depois que as configurações forem redefinidas para os padrões de fábrica.\n"									// Portuguese (Brazil)
										} 
	},
	{ ISTR_LANGUAGE_SET				,	{	"Both: Both the display and the external keyboard will be set to the same language at the same time.\n\n"
											"Display: Only the language used on the display will be changed.\n\n"
											"Keyboard: Only the keyboard type will be changed. The keyboard selected must match the keyboard used.\n",										// English
											"Les deux : l'écran et le clavier externe seront dans la même langue en même temps.\n\n"
											"Écran : seule la langue utilisée à l'écran sera modifiée.\n\n"
											"Clavier : seule le type de clavier sera modifié. Le clavier sélectionné doit correspondre au clavier utilisé.\n",								// French
											"Beide: Anzeige und externes Eingabegerät werden gleichzeitig auf dieselbe Sprache eingestellt.\n\n"
											"Anzeige: Nur die Sprache der Anzeige wird verändert.\n\n"
											"Eingabegerät: Nur der Eingabegerätetyp wird verändert. Das ausgewählte Eingabegerät muss mit dem verwendeten Eingabegerät übereinstimmen.\n",	// German
											"Ambos: se establecerán simultáneamente tanto la pantalla como el teclado externo con el mismo idioma.\n\n"
											"Pantalla: solo se cambiará el idioma de la pantalla.\n\n"
											"Teclado: solo cambiará el tipo de teclado. El teclado seleccionado deberá coincidir con el teclado usado.\n",									// Spanish
											"Entrambi: sia il display che la tastiera esterna saranno impostati sulla stessa lingua nello stesso momento.\n\n"
											"Display: viene modificata solo la lingua utilizzata dal display.\n\n"
											"Tastiera: viene modificato solo il tipo di tastiera. La tastiera selezionata deve coincidere con la tastiera utilizzata.\n",					// Italian
											"Ambos: O visor e o teclado externo serão definidos simultaneamente com o mesmo idioma.\n\n"
											"Visor: Apenas será alterado o idioma utilizado no visor.\n\n"
											"Teclado: Apenas será alterado o tipo de teclado. O teclado selecionado deverá corresponder ao teclado utilizado.\n",							// Portuguese
											"Ambos: O display e o teclado externo serão configurados para o mesmo idioma ao mesmo tempo.\n\n"
											"Display: Apenas o idioma usado no display será alterado.\n\n"
											"Teclado: Apenas o tipo de teclado será alterado. O teclado selecionado deve corresponder ao teclado usado.\n"									// Portuguese (Brazil)
										} 
	},
	{ ISTR_PRINTER_CONFIGURE		,	{	"Set Default: Select the printer to use for printing.\n\n"
											"Select Reports: Select the type and number of reports.\n",									// English
											"Imprimante par défaut : sélectionner l'imprimante à utiliser pour l'impression.\n\n"
											"Sélectionner des rapports : sélectionner le type et le nombre de rapports.\n",				// French
											"Standard einstellen: Drucker auswählen.\n\n"
											"Berichte auswählen: Typ und Zahl der Berichte auswählen.\n",								// German
											"Establecer valores predeterminados: seleccione la impresora que se va a utilizar.\n\n"
											"Seleccionar informes: seleccione el tipo y número de informes.\n",							// Spanish
											"Imposta come predefinito: seleziona la stampante da utilizzare per la stampa.\n\n"
											"Seleziona rapporti: seleziona il tipo e il numero di rapporti.\n",							// Italian
											"Configurar valores predeterminados: Selecione a impressora a utilizar para imprimir.\n\n"
											"Selecionar relatórios: Selecione o tipo e o número de relatórios.\n",						// Portuguese
											"Configurar padrão: Selecione a impressora a ser usada para a impressão.\n\n"
											"Selecionar relatórios: Selecione o tipo e o número de relatórios.\n"						// Portuguese (Brazil)
										} 
	},
	{ ISTR_PRINTER_SELECT_REPORTS	,	{	"Results: Select the number of results reports to print at the end of each analysis.\n\n"
											"iQC: Select the number of iQC reports to print at the end of each analysis.\n\n"
											"Error: Select the number of error reports to print at the end of each analysis. The reports can be printed automatically when there is an error, or may be printed for every analysis.\n",								// English
											"Résultats : sélectionner le nombre de rapports de résultats à imprimer à l'issue de chaque analyse.\n\n"
											"iQC : sélectionner le nombre de rapports iQC à imprimer à l'issue de chaque analyse.\n\n"
											"Erreur : sélectionner le nombre de rapports d'erreurs à imprimer à l'issue de chaque analyse. Des rapports peuvent s'imprimer automatiquement en cas d'erreur ou bien pour chaque analyse.\n",							// French
											"Ergebnisse: Anzahl der nach Abschluss jeder Analyse zu druckenden Ergebnisberichte auswählen.\n\n"
											"iQC: Anzahl der nach Abschluss jeder Analyse zu druckenden iQC-Berichte auswählen.\n\n"
											"Fehler: Anzahl der nach Abschluss jeder Analyse zu druckenden Fehlerberichte auswählen. Bei Vorliegen eines Fehlers können die Berichte automatisch oder für jede Analyse gedruckt werden.\n",							// German
											"Resultados: seleccione el número de informes de resultados para imprimir al final de cada análisis.\n\n"
											"iQC: seleccione el número de informes de iQC para imprimir al final de cada análisis.\n\n"
											"Error: seleccione el número de informes de error para imprimir al final de cada análisis. Los informes se pueden imprimir automáticamente cuando hay un error, o se pueden imprimir con cada análisis.\n",				// Spanish
											"Risultati: seleziona il numero di rapporti di risultati da stampare al termine di ogni analisi.\n\n"
											"iQC: seleziona il numero di rapporti di iQC da stampare al termine di ogni analisi.\n\n"
											"Errore: seleziona il numero di rapporti di errore da stampare al termine di ogni analisi. I rapporti possono essere stampati automaticamente in caso di errore, oppure essere stampati per ogni analisi.\n",			// Italian
											"Resultados: Seleccone o número de relatórios de resultados a imprimir no final de cada análise.\n\n"
											"iQC: Selecione o número de relatórios de iQC a imprimir no final de cada análise.\n\n"
											"Erro: Selecione o número de relatórios de erros a imprimir no final de cada análise. Os relatórios podem ser impressos automaticamente quando existir um erro, ou podem ser impressos para cada análise.\n",			// Portuguese
											"Resultados: Selecione o número de relatórios de resultados a serem impressos no final de cada análise.\n\n"
											"iQC: Selecione o número de relatórios de iQC a serem impressos no final de cada análise.\n\n"
											"Erro: Selecione o número de relatórios de erro a serem impressos no final de cada análise. Os relatórios podem ser impressos automaticamente quando houver um erro ou podem ser impressos para todas as análises.\n"	// Portuguese (Brazil)
										} 
	},
	{ ISTR_PRINTER_SETUP			,	{	"Configure: Select the printer to use and the number and type of reports to print.\n\n"
											"Test: Print a test page on the printer.\n",																			// English
											"Configuration : sélectionner l'imprimante à utiliser, le nombre et le type de rapports à imprimer.\n\n"
											"Test : imprimer une page de test sur l'imprimante.\n",																	// French
											"Konfigurieren: Verwendeten Drucker sowie Zahl und Typ der zu druckenden Berichte auswählen.\n\n"
											"Test: Testseite drucken.\n",																							// German
											"Configurar: seleccione la impresora que se va a utilizar y el número y tipo de informes que se van a imprimir.\n\n"
											"Test: imprima una página de prueba en la impresora.\n",																// Spanish
											"Configura: seleziona la stampante da usare, nonché il numero e il tipo di rapporti da stampare.\n\n"
											"Prova: stampa una pagina di prova.\n",																					// Italian
											"Configurar: Selecione a impressora a utilizar e o número e tipo de relatórios a imprimir.\n\n"
											"Teste: Imprima uma página de teste na impressora.\n",																	// Portuguese
											"Configurar: Selecione a impressora a ser usada e o número e tipo de relatórios a serem impressos.\n\n"
											"Teste: Imprima uma página de teste na impressora.\n"																	// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_ADD			,	{	"Special n: The next available generic demographic label between 1 and 99.\n\n"
											"Control n: The next available control label between 1 and 99.\n\n",													// English
											"Spécial n : la prochaine étiquette démographique générique disponible comprise entre 1 et 99.\n\n"
											"Contrôle n : la prochaine étiquette de contrôle disponible comprise entre 1 et 99.\n",									// French
											"Spezielles n: Das nächste verfügbare Demographie-Etikett zwischen 1 und 99.\n\n"
											"Steuerung n: Das nächste verfügbare Steuerungsetikett zwischen 1 und 99.\n",											// German
											"Especial n: la próxima etiqueta de demografía genérica disponible entre 1 y 99.\n\n"
											"Control n: la próxima etiqueta de control disponible entre 1 y 99.\n",													// Spanish
											"Speciale n: la successiva etichetta di demografia generica disponibile tra 1 e 99.\n\n"
											"Controllo n: la successiva etichetta di controllo disponibile tra 1 e 99.\n",											// Italian
											"Especial n: A próxima etiqueta de demografia genérica disponível entre 1 e 99.\n\n"
											"Controlo n: A próxima etiqueta de controlo disponível entre 1 e 99.\n",												// Portuguese
											"N especial: O próximo rótulo de dado demográfico genérico disponível entre 1 e 99.\n\n"
											"N controle: O próximo rótulo de controle disponível entre 1 e 99.\n"													// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_INCLUDE		,	{	"Last Modified XXX: The last reference range modified.\n\n"
											"All: Allows the user to select from a list of all reference ranges known to the analyzer.\n",														// English
											"Dernière modification XXX : la dernière fourchette de référence modifiée.\n\n"
											"Tous : permet à l'utilisateur d'effectuer une sélection à partir de la liste de toutes les fourchettes de références connues de l'analyseur.\n",	// French
											"Letzte Änderung XXX: Letzter geänderter Referenzbereich.\n\n"
											"Alle: Ermöglicht dem Benutzer die Auswahl aus einer Liste aller im Analysegerät verfügbaren Referenzbereiche.\n",									// German
											"Última modificación XXX: el último intervalo de referencia modificado.\n\n"
											"Todo: permite al usuario elegir de una lista de todos los intervalos de referencia conocidos por el analizador.\n",								// Spanish
											"Ultima modifica XXX: l'ultimo range di riferimento modificato.\n\n"
											"Tutto: consente all'utente di selezionare da un elenco tutti i range di riferimento noti all'analizzatore.\n",										// Italian
											"Última modificação XXX: O último intervalo de referência modificado.\n\n"
											"Tudo: Permite ao utilizador selecionar a partir de uma lista com todos os intervalos de referência conhecidos para o analisador.\n",				// Portuguese
											"Última modificação de XXX: O último intervalo de referência modificado.\n\n"
											"Todos: Permite ao usuário selecionar em uma lista de todos os intervalos de referência conhecidos pelo analisador.\n"								// Portuguese (Brazil)
										}
	},
	{ ISTR_REF_RANGES_MODIFY		,	{	"Analyte: Provides a list of all active reference ranges for a particular analyte.\n\n"
											"Demographic: Provides a list of all analytes for all reference ranges.\n",																			// English
											"Substance à analyser : fournit une liste de toutes les fourchettes de référence actives pour une substance à analyser donnée.\n\n"
											"Élément démographique : fournit une liste de toutes les substances à analyser pour toutes les fourchettes de référence.\n",						// French
											"Analyt: Anzeige einer Liste aller aktiven Referenzbereiche für ein bestimmtes Analyt.\n\n"
											"Demographie: Anzeige einer Liste aller Analyte für alle Referenzbereiche.\n",																		// German
											"Analito: proporciona una lista de todos los intervalos de referencia activos para un analito particular.\n\n"
											"Demografía: proporciona una lista de todos los analitos para todos los intervalos de referencia.\n",												// Spanish
											"Analita: fornisce un elenco di tutti i range di riferimento attivi per un particolare analita.\n\n"
											"Demografia: fornisce un elenco di tutti gli analiti per ogni range di riferimento.\n",																// Italian
											"Analito: Fornece uma lista de todos os intervalos de referencia ativos para um analito específico.\n\n"
											"Demografia: Fornece uma lista de todos os analitos para todos os intervalos de referência.\n",														// Portuguese
											"Analito: Fornece uma lista de todos os intervalos de referência ativos para um determinado analito.\n\n"
											"Dados demográficos: Fornece uma lista de todos os analitos para todos os intervalos de referência.\n"												// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_REMOVE		,	{	"Removing a demographic from the sample type list will remove the item from the list of demographics shown when running a disc, and any values entered will be permanently lost.\n",																						// English
											"La suppression d'un élément démographique dans la liste de type d'échantillons a pour effet de supprimer cet élément dans la liste d'éléments démographiques affichés lors de l'exécution d'un disque, et toutes les valeurs saisies seront irrémédiablement perdues.\n",	// French
											"Die Entfernung einer Demographie aus der Probentypliste entfernt das Objekt aus der Demographieliste, die bei laufendem Rotor angezeigt wird, und sämtliche eingegebene Werte gehen dauerhaft verloren.\n",																// German
											"Al eliminar una demografía de la lista de tipo de muestra se eliminará la entrada de la lista de demografías mostrada al usar el disco y cualquier valor introducido se perderá permanentemente.\n",																		// Spanish
											"Rimuovendo una demografia dall'elenco dei tipi si rimuoverà quella voce dall'elenco di demografie proposto quando un disco è in funzione e tutti i valori immessi saranno persi in modo permanente.\n",																	// Italian
											"Ao eliminar uma demografia da lista de tipo de amostra, será eliminado o item da lista de demografias exibida quando se utiliza um disco, e qualquer valor introduzido será permanentemente perdido.\n",																	// Portuguese
											"A remoção de um dado demográfico da lista de tipos de amostra removerá o item da lista de dados demográficos mostrada durante a execução de um disco e quaisquer valores inseridos serão permanentemente perdidos.\n"														// Portuguese (Brazil)
										} 
	},
	{ ISTR_SECURITY_LOCKOUT			,	{	"Analyze: When checked, only a authorized operator can open the drawer at the start or the end of an analysis.\n\n"
											"Recall Results: When checked, only a authorized operator can recall results.\n\n"
											"Settings: When checked, only a authorized operator can change settings.\n",																// English
											"Analyse : lors de la vérification, seul un opérateur autorisé peut ouvrir le tiroir au début ou à la fin d'une analyse.\n\n"
											"Rappeler résultats : lors de la vérification, seul un opérateur autorisé peut rappeler les résultats.\n\n"
											"Paramètres : lors de la vérification, seul un opérateur autorisé peut modifier les paramètres.\n",											// French
											"Analysieren: Wenn aktiviert, kann nur ein autorisierter Bediener das Fach bei Start oder Ende einer Analyse öffnen. \n\n"
											"Ergebnisse erneut abrufen: Wenn aktiviert, kann nur ein autorisierter Bediener Ergebnisse erneut abrufen.\n\n"
											"Einstellungen: Wenn aktiviert, kann nur ein autorisierter Bediener Einstellungen ändern.\n",												// German
											"Analizar: cuando se activa, solo un operario autorizado puede abrir el cajón al comienzo o al final de un análisis.\n\n"				
											"Buscar resultados: cuando se activa, solo un operario autorizado puede buscar los resultados.\n\n"
											"Configuración: cuando se activa, solo un operario autorizado puede cambiar la configuración.\n",											// Spanish
											"Analizzare: quando spuntato, esclusivamente un operatore autorizzato può aprire il cassetto all'inizio o alla fine di un'analisi.\n\n"
											"Recupera i risultati: quando spuntato, esclusivamente un operatore autorizzato può recuperare i risultati.\n\n"
											"Impostazioni; quando spuntato, esclusivamente un operatore autorizzato può modificare le impostazioni.\n",									// Italian
											"Analisar: Quando selecionado, apenas um operador autorizado pode abrir a gaveta no início ou no final de uma análise.\n\n"
											"Recuperar resultados: Quando selecionado, apenas um operador autorizado pode recuperar os resultados.\n\n"
											"Configurações: Quando selecionado, apenas um operador autorizado pode alterar as configurações.\n",										// Portuguese
											"Analisar: Quando marcado, apenas um operador autorizado pode abrir a gaveta no início ou no final de uma análise.\n\n"
											"Recuperar resultados: Quando marcado, somente um operador autorizado pode recuperar os resultados.\n\n"
											"Configurações: Quando marcado, apenas um operador autorizado pode alterar as configurações.\n"												// Portuguese (Brazil)
										}	
	},
	{ ISTR_SECURITY_SETTINGS		,	{	"Administrators: Can access the settings in the second settings screen. Security features take effect when the first administrator is added.  Security features are disabled when all administrators are deleted.\n\n"
											"Operators: Authorized operators for the analyzer. Adding an operator to the list will turn on the Operator ID prompt when analyzing a sample.\n\n"
											"Controls: The date that a new set of control samples must be run by. Analyzing a sample after this date will result in a warning when the sample is analyzed.\n",																																	// English
											"Administrateurs : ils peuvent accéder aux paramètres sur le second écran de paramètres. Les caractéristiques de sécurité entrent en vigueur lors de l'ajout du premier administrateur. Les caractéristiques de sécurité sont désactivées lorsque tous les administrateurs sont supprimés.\n\n"
											"Opérateurs : opérateurs autorisés pour l'analyseur. L'ajout d'un opérateur à la liste a pour effet d'activer un message d'invitation à saisir l'ID opérateur lors de l'analyse d'un échantillon.\n\n"
											"Contrôles : date butoir à laquelle un nouvel ensemble d'échantillons de contrôle doit avoir été exécuté. Le fait d'analyser un échantillon après cette date entraîne un message d'avertissement lors de l'analyse de l'échantillon.\n",															// French
											"Administrator: Haben Zugang zu den Einstellungen in dem Zweiteinstellungsbildschirm. Sicherheitsmerkmale werden bei Hinzufügung des ersten Administrators wirksam. Sicherheitsmerkmale werden bei Löschung aller Administratoren deaktiviert.\n\n"
											"Bediener: Befugte Bediener des Analysegeräts. Die Hinzufügung eines Bedieners zur Liste aktiviert bei Analyse einer Probe die Aufforderung zur Eingabe der Bediener-ID.\n\n"
											"Steuerungen: Das Datum, zu dem ein neuer Satz von Kontrollproben vorgenommen werden muss. Die Analyse einer Probe nach diesem Datum führt zu einem Warnhinweis bei Analyse der Probe.\n",																											// German
											"Administradores: pueden acceder a la configuración en la segunda pantalla de configuraciones. Las medidas de seguridad empiezan a adoptarse después de añadir el primer administrador. Las medidas de seguridad están inactivas al eliminar a todos los administradores.\n\n"
											"Operarios: operarios autorizados para uso del analizador. Al añadir un operario a la lista se activará un mensaje de identificación de operario al analizar la muestra.\n\n"
											"Controles: la fecha en la que se debe usar un nuevo grupo de muestras de control. El análisis de una muestra después de esta fecha motivará un mensaje de aviso durante el análisis.\n",																											// Spanish
											"Amministratori: possono accedere alle impostazioni nel secondo schermo di impostazioni. Le impostazioni di sicurezza entrano in vigore quando si aggiunge il primo amministratore. Le impostazioni di sicurezza sono disattivate quando tutti gli amministratori sono cancellati.\n\n"
											"Operatori: operatori autorizzati all'uso dell'analizzatore. Se si aggiunge un operatore all'elenco, durante l'analisi di un campione viene attivato il prompt \"ID operatore\".\n\n"
											"Controlli: la data in cui deve essere effettuata una nuova serie di controlli campione. Se l'analisi di un campione viene eseguita dopo tale data, verrà visualizzato un messaggio di attenzione.\n",																								// Italian
											"Administradores: Podem aceder às configurações no segundo ecrã de configurações. Os recursos de segurança tornam-se efetivos quando o primeiro administrador é adicionado.  Os recursos de segurança ficam inativos quando todos os administradores são eliminados.\n\n"
											"Operadores: Operadores autorizados para utilização do analisador. Adicionar um operador à lista irá ativar um pedido de ID do operador ao analisar a amostra.\n\n"
											"Controlos: A data na qual se deve utilizar um grupo novo de amostras controlo. A análise de uma amostra após esta data resultará numa mensagem de aviso durante a análise.\n",																														// Portuguese
											"Administradores: Podem acessar as configurações na segunda tela de configurações. Os recursos de segurança entram em vigor quando o primeiro administrador é adicionado.  Os recursos de segurança são desativados quando todos os administradores são excluídos.\n\n"
											"Operadores: Operadores autorizados para o analisador. Adicionar um operador à lista ativará o prompt do ID do operador ao analisar uma amostra.\n\n"
											"Controles: A data até a qual um novo conjunto de amostras de controle deve ser executado. A análise de uma amostra após essa data resultará em um aviso quando a amostra for analisada.\n"																											// Portuguese (Brazil)
										} 
	},
	{ ISTR_SECURITY_SETTINGS_LOCKOUT,	{	"Administrators: Can access the settings in the second settings screen. Security features take effect when the first administrator is added.  Security features are disabled when all administrators are deleted.\n\n"
											"Operators: Authorized operators for the analyzer. Adding an operator to the list will turn on the Operator ID prompt when analyzing a sample.\n\n"
											"Controls: The date that a new set of control samples must be run by. Analyzing a sample after this date will result in a warning when the sample is analyzed.\n\n"
											"Lockout: Limits use of the analyzer functions to authorized operators.  Create authorized operators prior to enabling lockout.\n",																																									// English
											"Administrateurs : ils peuvent accéder aux paramètres sur le second écran de paramètres. Les caractéristiques de sécurité entrent en vigueur lors de l'ajout du premier administrateur. Les caractéristiques de sécurité sont désactivées lorsque tous les administrateurs sont supprimés.\n\n"
											"Opérateurs : opérateurs autorisés pour l'analyseur. L'ajout d'un opérateur à la liste a pour effet d'activer un message d'invitation à saisir l'ID opérateur lors de l'analyse d'un échantillon.\n\n"
											"Contrôles : date butoir à laquelle un nouvel ensemble d'échantillons de contrôle doit avoir été exécuté. Le fait d'analyser un échantillon après cette date entraîne un message d'avertissement lors de l'analyse de l'échantillon.\n\n"
											"Verrouillage : limite l'utilisation des fonctions de l'analyseur aux opérateurs autorisés. Les opérateurs autorisés doivent être créés avant d'autoriser le verrouillage.\n",																														// French
											"Administrator: Haben Zugang zu den Einstellungen in dem Zweiteinstellungsbildschirm. Sicherheitsmerkmale werden bei Hinzufügung des ersten Administrators wirksam. Sicherheitsmerkmale werden bei Löschung aller Administratoren deaktiviert.\n\n"
											"Bediener: Befugte Bediener des Analysegeräts. Die Hinzufügung eines Bedieners zur Liste aktiviert bei Analyse einer Probe die Aufforderung zur Eingabe der Bediener-ID.\n\n"
											"Steuerungen: Das Datum, zu dem ein neuer Satz von Kontrollproben vorgenommen werden muss. Die Analyse einer Probe nach diesem Datum führt zu einem Warnhinweis bei Analyse der Probe.\n\n"
											"Verriegelung: Beschränkt die Verwendung von Analysegerätfunktionen auf autorisierte Bediener. Erstellen Sie autorisierte Bediener bevor Sie die Verriegelung aktivieren.\n",																														// German
											"Administradores: pueden acceder a la configuración en la segunda pantalla de configuración. Las características de seguridad se activan cuando se agrega el primer administrador y se desactivan si se eliminan todos los administradores.\n\n"
											"Operarios: operarios autorizados a utilizar el analizador. Al añadir un operario a la lista, se preguntará el ID del operario al analizar una muestra.\n\n"
											"Controles: fecha en la que se debe ejecutar un nuevo conjunto de muestras de control. Si se analiza una muestra después de esta fecha, aparecerá una advertencia cuando se analice la muestra.\n\n"
											"Bloqueo: limita el uso de las funciones del analizador a los operarios autorizados. Debe crear operarios autorizados antes de activar el bloqueo.\n",																																				// Spanish
											"Amministratori: possono accedere alle impostazioni nel secondo schermo di impostazioni. Le impostazioni di sicurezza entrano in vigore quando si aggiunge il primo amministratore. Le impostazioni di sicurezza sono disattivate quando tutti gli amministratori sono cancellati.\n\n"
											"Operatori: operatori autorizzati all'uso dell'analizzatore. Se si aggiunge un operatore all'elenco, durante l'analisi di un campione viene attivato il prompt \"ID operatore\".\n\n"
											"Controlli: la data in cui deve essere effettuata una nuova serie di controlli campione. Se l'analisi di un campione viene eseguita dopo tale data, verrà visualizzato un messaggio di attenzione.\n\n",																							
											"Serrata: limita l'uso delle funzioni dell'analizzatore agli operatori autorizzati.  Creare operatori autorizzati prima di abilitare la serrata.\n"																																					// Italian
											"Administradores: Podem aceder às configurações no segundo ecrã de configurações. Os recursos de segurança tornam-se efetivos quando o primeiro administrador é adicionado.  Os recursos de segurança ficam inativos quando todos os administradores são eliminados.\n\n"
											"Operadores: Operadores autorizados para utilização do analisador. Adicionar um operador à lista irá ativar um pedido de ID do operador ao analisar a amostra.\n\n"
											"Controlos: A data na qual se deve utilizar um grupo novo de amostras controlo. A análise de uma amostra após esta data resultará numa mensagem de aviso durante a análise.\n\n"
											"Bloqueio: Limita a utilização das funções do analisador aos operadores autorizados.  Deve criar operadores autorizados antes de ativar o bloqueio.\n",																																				// Portuguese
											"Administradores: Podem acessar as configurações na segunda tela de configurações. Os recursos de segurança entram em vigor quando o primeiro administrador é adicionado.  Os recursos de segurança são desativados quando todos os administradores são excluídos.\n\n"
											"Operadores: Operadores autorizados para o analisador. Adicionar um operador à lista ativará o prompt do ID do operador ao analisar uma amostra.\n\n"
											"Controles: A data até a qual um novo conjunto de amostras de controle deve ser executado. A análise de uma amostra após essa data resultará em um aviso quando a amostra for analisada.\n\n"
											"Bloquear: Limita o uso das funções do analisador a operadores autorizados.  Crie operadores autorizados antes de ativar o bloqueio.\n"																																								// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_ELECTROLYTE_UNITS	,	{	"Selecting a unit on this screen will change the units for CL, K, NA and tCO2 simultaneously to the same units.\n",										// English
											"L'unité sélectionnée dans cet écran sera appliquée simultanément à CL, K, NA et tCO2.\n",																// French
											"Mit der Auswahl einer Einheit in diesem Menü werden automatisch die Einheiten für CL, K, NA und tCO2 auf diese Einheit umgestellt.\n",					// German
											"Al seleccionar una unidad en esta pantalla, se cambiará CL, K, NA y tCO2 simultáneamente a la misma unidad.\n",										// Spanish
											"La selezione di un'unità in questa schermata modifica simultaneamente le unità per CL, K, NA e tCO2 nelle stesse unità.\n",							// Italian
											"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades CL, K, NA e tCO2 para as mesmas unidades.\n",						// Portuguese
											"A seleção de uma unidade nessa tela mudará as unidades para CL, K, NA e tCO2 simultaneamente para as mesmas unidades.\n"								// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_ENZYME_UNITS			,	{	"Selecting a unit on this screen will change the units for ALT, ALP, AMY, AST, CK, GGT and LD simultaneously to the same units.\n",						// English
											"L'unité sélectionnée dans cet écran sera appliquée simultanément à ALT, ALP, AMY, AST, CK, GGT et LD.\n",												// French
											"Mit der Auswahl einer Einheit in diesem Menü werden automatisch die Einheiten für ALT, ALP, AMY, AST, CK, GGT und LD auf diese Einheit umgestellt.\n",	// German
											"Al seleccionar una unidad en esta pantalla, se cambiará ALT, ALP, AMY, AST, CK, GGT y LD simultáneamente a la misma unidad.\n",						// Spanish
											"La selezione di un'unità in questa schermata modifica simultaneamente le unità per ALT, ALP, AMY, AST, CK, GGT ed LD nelle stesse unità.\n",			// Italian
											"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades ALT, ALP, AMY, AST, CK, GGT e LD para as mesmas unidades.\n",		// Portuguese
											"A seleção de uma unidade nessa tela mudará as unidades para ALT, ALP, AMY, AST, CK, GGT e LD simultaneamente para as mesmas unidades.\n"				// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_LIPID_UNITS			,	{	"Selecting a unit on this screen will change the units for CHOL, HDL, LDL, TRIG and VLDL simultaneously to the same units.\n",							// English
											"L'unité sélectionnée dans cet écran sera appliquée simultanément à CHOL, LHD, LBD, TRIG et LTBD.\n",													// French
											"Mit der Auswahl einer Einheit in diesem Menü werden automatisch die Einheiten für CHOL, HDL, LDL, TRIG und VLDL auf diese Einheit umgestellt.\n",		// German
											"Al seleccionar una unidad en esta pantalla, se cambiará COL, HDL, LDL, TRIG y PMBD simultáneamente a la misma unidad.\n",								// Spanish
											"La selezione di un'unità in questa schermata modifica simultaneamente le unità per COL, HDL, LDL, TRIG e VLDL nelle stesse unità.\n",					// Italian
											"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades CHOL, HDL, LDL, TRIG e VLDL para as mesmas unidades.\n",				// Portuguese
											"A seleção de uma unidade nessa tela mudará as unidades para CHOL, HDL, LDL, TRIG e VLDL simultaneamente para as mesmas unidades.\n"					// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_MINERAL_UNITS		,	{	"Selecting a unit on this screen will change the units for CA and MG simultaneously to the same units.\n",												// English
											"L'unité sélectionnée dans cet écran sera appliquée simultanément à CA et MG.\n",																		// French
											"Mit der Auswahl einer Einheit in diesem Menü werden automatisch die Einheiten für CA und MG auf diese Einheit umgestellt.\n",							// German
											"Al seleccionar una unidad en esta pantalla, se cambiará CA y MG simultáneamente a la misma unidad.\n",													// Spanish
											"La selezione di un'unità in questa schermata modifica simultaneamente le unità per CA ed MG nelle stesse unità.\n",									// Italian
											"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades CA e MG para as mesmas unidades.\n",									// Portuguese
											"A seleção de uma unidade nessa tela mudará as unidades para CA e MG simultaneamente para as mesmas unidades.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_PROTEIN_UNITS		,	{	"Selecting a unit on this screen will change the units for ALB and TP simultaneously to the same units.\n",												// English
											"L'unité sélectionnée dans cet écran sera appliquée simultanément à ALB et TP.\n",																		// French
											"Mit der Auswahl einer Einheit in diesem Menü werden automatisch die Einheiten für ALB und TP auf diese Einheit umgestellt.\n",							// German
											"Al seleccionar una unidad en esta pantalla, se cambiará ALB y TP simultáneamente a la misma unidad.\n",												// Spanish
											"La selezione di un'unità in questa schermata modifica simultaneamente le unità per ALB e TP nelle stesse unità.\n",									// Italian
											"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades ALB e TP para as mesmas unidades.\n",								// Portuguese
											"A seleção de uma unidade nessa mudará as unidades para ALB e TP simultaneamente para as mesmas unidades.\n"											// Portuguese (Brazil)
										} 
	},
	{ ISTR_SOUND_SETTINGS			,	{	"Screen Click: Sets the volume level of the sound made when touching a button on the display.\n\n"
											"Alerts: Sets the volume level when a warning or error screen is displayed.\n\n"
											"Status: Sets the volume level for status sounds.\n",																									// English
											"Clic écran : cette fonction définit le volume sonore lors de l'activation d'un bouton à l'écran.\n\n"
											"Avertisseurs sonores : cette fonction définit le volume lors de l'affichage d'un avertissement ou d'un écran d'erreur.\n\n"
											"État : définit le volume sonore pour les indicateurs d'état.\n",																						// French
											"Bildschirmklick: Einstellung der Tonlautstärke bei Berührung einer Taste auf der Anzeige.\n\n"
											"Alarme: Einstellung der Tonlautstärke bei Erscheinen der Warn- oder Fehleranzeige.\n\n"
											"Status: Einstellung der Lautstärke für Statustöne.\n",																									// German
											"Clic de pantalla: establece el volumen del sonido que se hace cuando se toca un botón en la pantalla.\n\n"
											"Alertas: establece el volumen del sonido que se hace cuando aparece una ventana de aviso o error.\n\n"
											"Estado: establece el volumen del sonido de estado.\n",																									// Spanish
											"Clic schermo: imposta il livello del volume del suono che si sente quando si tocca un pulsante sullo schermo.\n\n"
											"Allarmi: imposta il livello del volume dei messaggi di attenzione o errore visualizzati a schermo.\n\n"
											"Stato: imposta il livello del volume per i suoni di stato.\n",																							// Italian
											"Clique no ecrã: Define o nível de volume do som que se ouve quando se toca uma tecla no visor.\n\n"
											"Alertas: Define o nível de volume do som que se ouve quando é visualizado um ecrã de aviso ou erro.\n\n"
											"Estado: Define o nível de volume para os sons de estado.\n",																							// Portuguese
											"Clique na tela: Configura o nível de volume do som produzido ao tocar em um botão no visor.\n\n"
											"Alertas: Configura o nível do volume quando uma tela de aviso ou erro é exibida.\n\n"
											"Status: Configura o nível de volume para os sons de status.\n"																							// Portuguese (Brazil)
										} 
	},
	{ ISTR_TCPIP_SETTINGS			,	{	"Change Settings: Change the TCP/IP Settings where Analyzer IP is the Static IP Address for the analyzer, the Subnet Mask is the IP Address mask for the analyzer, the Gateway IP is the IP Address of the router for the analyzer to reach other networks, the Server IP is the IP address of the server that will host the ASTM connection, and the Server Socket is the Socket (Port) on the Server that will be used for the ASTM TCP/IP connection.\n\n"
											"Check Settings: The analyzer will attempt to make an ASTM connection using the settings under the Change Settings button.  The analyzer will display the result of the connection attempt.  Ensure the ASTM server is running and configured to accept a connection before checking the settings.\n",																																																																												// English																																																																																																																																																																																																																												// English                 					
											"Modifier les paramètres : modifier les paramètres TCP/IP si le champ Adresse IP de l'analyseur est renseigné par l'adresse IP statique de l'appareil, le champ Masque de sous-réseau contient le masque d'adresse IP de l'analyseur, le champ Adresse IP de la passerelle désigne l'adresse IP du routeur qui permet à l'analyseur de communiquer avec d'autres réseaux. Le champ Adresse IP du serveur est renseigné par l'adresse IP du serveur qui hébergera la connexion ASTM et enfin le champ Socket du serveur désigne le Socket (Port) du serveur qui sera utilisé pour la connexion TCP/IP ASTM.\n\n"
											"Vérifier les paramètres : l'analyseur tentera d'établir une connexion ASTM sur la base des paramètres définis sous le bouton Modifier les paramètres.  L'analyseur affichera le résultat de la tentative de connexion.  Vérifier que le serveur ASTM est en marche et qu'il est configuré pour accepter une connexion avant de vérifier les paramètres.\n",																																																														// French
											"Einstellungen ändern: Die TCP/IP-Einstellungen so ändern, dass die IP-Adresse des Analysegeräts die statische IP-Adresse für das Analysegerät ist, die Subnetzmaske die IP-Adressmaske für das Analysegerät ist, die Gateway-IP-Adresse die IP-Adresse des Routers für das Analysegerät ist, über den dieses andere Netzwerke erreichen kann, die Server-IP-Adresse die IP-Adresse des Servers ist, der als Host für den ASTM-Anschluss dient, und der Server-Socket der Socket (Anschluss ) des Servers ist, der für die ASTM-TCP/IP-Verbindung genutzt werden soll.\n\n"
											"Einstellungen überprüfen: Das Analysegerät wird versuchen, eine ASTM-Verbindung herzustellen, und bedient sich dabei der über die Taste \"Einstellungen ändern\" eingegebenen Einstellungen. Das Analysegerät zeigt das Resultat des Verbindungsversuchs an. Vor dem Überprüfen der Einstellungen sicherstellen, dass der ASTM-Server in Betrieb und für die Annahme einer Verbindung konfiguriert ist.\n",																																																		// German
											"Cambiar configuración: Cambie la configuración de TCP/IP, donde la dirección IP del analizador es la dirección IP estática del analizador, la máscara de la subred es la máscara de la dirección IP del analizador, la dirección IP de la puerta de enlace es la dirección IP del enrutador para conectividad entre el analizador y otras redes, la dirección IP del servidor es la dirección IP del servidor que alojará la conexión ASTM y el socket del servidor es el socket (puerto) del servidor que se utilizará para la conexión TCP/IP ASTM.\n\n"
											"Comprobar configuración: El analizador intentará realizar una conexión ASTM usando los parámetros que aparecen tras pulsar el botón \"Cambiar configuración\".  El analizador mostrará el resultado del intento de conexión.  Asegúrese de que el servidor ASTM está funcionando y configurado para aceptar una conexión antes de comprobar los parámetros de configuración.\n",																																																									// Spanish
											"Modifica impostazioni: Modificare le impostazioni TCP/IP dove IP analizzatore è l'indirizzo IP statico per l'analizzatore, Maschera di sottorete è la maschera dell'indirizzo IP per l'analizzatore, IP gateway è l'indirizzo IP del router per consentire all'analizzatore di raggiungere altre reti, IP server è l'indirizzo IP del server per l'hosting della connessione ASTM, e Socket server è il Socket (porta) sul server che verrà utilizzato per la connessione TCP/IP ASTM.\n\n"
											"Verifica impostazioni: L'analizzatore tenterà di effettuare una connessione ASTM utilizzando le impostazioni sotto il pulsante Modifica impostazioni.  L'analizzatore visualizzerà il risultato del tentativo di connessione.  Prima di eseguire la verifica delle impostazioni, accertarsi che il server ASTM sia attivo e configurato per accettare una connessione.\n",																																																											// Italian
											"Alterar configurações: Altera as configurações de TCP/IP, nas quais o IP do analisador é o endereço IP estático do analisador, a máscara de subrede é a máscara do endereço IP do analisador, o IP do Portal é o endereço IP do rotor para que o analisador se ligue a outras redes, o IP do servidor é o endereço IP do servidor que alojará a ligação ASTM e a tomada do servidor é a tomada (Porta) no Servidor que será utilizada para a ligação TCP/IP ASTM.\n\n"
											"Verificar configurações: O analisador tentará realizar uma ligação ASTM utilizando as definições que surgem sob o botão Alterar configurações.  O analisador apresentará o resultado da tentativa de ligação.  Certifique-se que o servidor ASTM está a funcionar e que está configurado para aceitar uma ligação antes de verificar as configurações.\n",																																																															// Portuguese
											"Alterar configurações: Altere as configurações de TCP/IP em que o IP do analisador é o endereço IP estático do analisador, a máscara de sub-rede é a máscara do endereço IP do analisador, o IP do gateway é o endereço IP do roteador para que o analisador alcance outras redes, IP do servidor é o endereço IP do servidor que hospedará a conexão ASTM e Soquete do servidor é o soquete (porta) do servidor que será usado para a conexão TCP/IP de ASTM.\n\n"
											"Verificar configurações: O analisador tentará fazer uma conexão ASTM usando as configurações no botão Alterar configurações.  O analisador exibirá o resultado da tentativa de conexão.  Certifique-se de que o servidor ASTM esteja em execução e configurado para aceitar uma conexão antes de verificar as configurações.\n"																																																																					// Portuguese (Brazil)
										}
	},		
	{ ISTR_TOUCH_CALIBRATE			,	{	"Pressing Yes will align the touch screen to the LCD display. It is necessary to use a soft-pointed stylus to accurately align the touch screen. Using a hard-pointed object will damage the touch screen. Pressing the screen other than at the center of the target will misalign the touch screen. If you touch the screen in the wrong location, complete the alignment by touching the remaining targets and do not touch the final alignment button. If you have questions contact technical support before performing the alignment.\n",																											// English
											"Appuyer sur Oui pour aligner l'écran tactile et l'écran à cristaux liquides. Pour aligner correctement l'écran tactile, utiliser un stylet à pointe souple. Toute utilisation d'un objet pointu risquerait d'endommager l'écran tactile. Le fait d'appuyer sur l'écran ailleurs qu'au centre de la cible entraîne un défaut d'alignement de l'écran tactile. Si vous touchez l'écran au mauvais endroit, terminer l'alignement en touchant les cibles restantes et ne pas toucher le bouton d'alignement final. Pour toute question, contacter l'assistance technique avant de procéder à l'alignement.\n",											// French
											"Drücken auf Ja richtet den Touch-Screen auf das LCD-Display aus. Zur genauen Ausrichtung des Touch-Screens muss ein Eingabestift mit weicher Spitze verwendet werden. Die Verwendung eines Objekts mit harter Spitze beschädigt den Touch-Screen. Drücken der Anzeige außerhalb der Zielmitte führt zu einer falschen Ausrichtung des Touch-Screens. Wenn Sie den Screen an der falschen Stelle berühren, schließen Sie die Ausrichtung durch Berührung der verbleibenden Ziele ab. Die endgültige Ausrichtungstaste nicht berühren. Bei Fragen vor der Ausrichtung den technischen Support kontaktieren.\n",											// German
											"Al pulsar Sí se alineará la pantalla táctil con la pantalla de cristal líquido. Si fuese necesario, utilice un lápiz con punta blanda para alinear con precisión la pantalla de contacto. Si se utiliza un objeto con punta dura, la pantalla táctil resultará dañada. Si se presiona la pantalla en otro lugar distinto al centro del objetivo, la pantalla táctil resultará desalineada. Si toca la pantalla en el lugar equivocado, complete la alineación tocando los objetivos restantes y no toque el botón de alineación final. Si tiene alguna duda o pregunta, llame al servicio de asistencia técnica antes de realizar la alineación.\n",	// Spanish
											"Premendo Sì il touchscreen verrà allineato al display LCD. Per allineare accuratamente il touchscreen è necessario utilizzare una penna stilo a punta morbida. Oggetti a punta rigida possono danneggiare il touchscreen. Se si preme lo schermo non al centro dell'obiettivo, il touchscreen verrà allineato male. Se si tocca lo schermo nel punto sbagliato, completare l'allineamento toccando gli altri obiettivi e non toccare il pulsante di allineamento finale. Per eventuali dubbi, prima di effettuare l'allineamento contattare l'assistenza tecnica.\n",																					// Italian
											"Ao premir Sim irá alinhar o ecrã tátil com o visor LCD. É necessário utilizar um ponteiro de ponta suave para alinhar o ecrã tátil com precisão. Se utilizar um objeto de ponta dura irá provocar danos no ecrã tátil. Ao pressionar o ecrã noutro local que não seja o centro do alvo, irá desalinhar o ecrã tátil. Se tocar no ecrã no local errado, conclua o alinhamento tocando nos alvos restantes e não toque na tecla final de alinhamento. Se tiver alguma dúvida, contacte a serviço de assistência técnica antes de efetuar o alinhamento.\n",																								// Portuguese
											"Pressionar Sim alinhará a tela sensível ao toque com o display LCD. É necessário usar uma caneta de ponta macia para alinhar precisamente a tela sensível ao toque. O uso de um objeto pontiagudo danificará a tela sensível ao toque. Pressionar a tela fora do centro do alvo desalinhará a tela sensível ao toque. Se você tocar na tela no local errado, conclua o alinhamento tocando nos alvos restantes e não toque no botão de alinhamento final. Se tiver dúvidas, entre em contato com o suporte técnico antes de realizar o alinhamento.\n"																									// Portuguese (Brazil)
										} 
	},
	{ ISTR_UNITS_GLOBAL				,	{	"Non SI: Also known as Common Units. The units in which all results are calculated and the units commonly used in the United States.\n\n"
											"SI: Units identified by Abaxis as the commonly used SI units.",																																// English
											"Non SI : également désignées \"Unités ordinaires\". Il s'agit des unités dans lesquelles tous les résultats sont calculés, ainsi que des unités ordinairement utilisées aux États-Unis.\n\n"
											"SI : unités identifiées par Abaxis comme étant les unités SI ordinairement utilisées.\n",																										// French
											"Kein SI: Auch als Häufige Einheiten bekannt. Die Einheiten zur Ergebnisermittlung sind die in den USA gebräuchlichen Einheiten.\n\n"
											"SI: Als Abaxis identifizierte Einheiten als häufig verwendete SI-Einheiten.\n",																												// German
											"No SI: también se le conoce como Unidades comunes. La unidades en las que se calculan todos los resultados y las unidades comúnmente usadas en los Estados Unidos.\n\n"
											"SI: unidades identificadas por Abaxis como las unidades del sistema internacional usadas normalmente.\n",																						// Spanish
											"Non SI: conosciuto anche come Unità comuni. Le unità in cui tutti i risultati sono calcolati e le unità comunemente usate negli Stati Uniti.\n\n"
											"SI: unità identificate come Abaxis come le unità SI comunemente usate.\n",																														// Italian
											"Não SI: Também conhecidas como Unidades comuns. As unidades nas quais todos os resultados são calculados e as unidades frequentemente utilizadas nos Estados Unidos.\n\n"
											"SI: Unidades identificadas pela Abaxis como as unidades do sistema internacional utilizadas frequentemente.\n",																				// Portuguese
											"Não SI: Também conhecido como Unidades comuns. As unidades nas quais todos os resultados são calculados e as unidades comumente usadas nos Estados Unidos.\n\n"
											"SI: Unidades identificadas pela Abaxis como as unidades SI comumente usadas.\n"																												// Portuguese (Brazil)
										} 
	},
	

// Error screens							ENGLISH,							FRENCH,										GERMAN,									SPANISH,												ITALIAN, 								PORTUGUESE,											PORTUGUESE (BRAZIL)
	{ ESTR_ADMIN_ID_LIST_FULL		,	{	"Administrator ID List Full",		"Liste ID administrateur pleine",			"Vollständige Administrator-ID-Liste",	"Lista de id. de administradores completa",				"Elenco ID Amministratore completo",	"Lista de ID de administradores completa",			"Lista de IDs de administrador completa"	}	},
	{ ESTR_AMBIENT_TEMP				,	{	"Ambient Temp. Error",				"Temp. ambiante Erreur",					"Umgebungstemp. Fehler",				"Temp. ambiente Error",									"Temp. ambiente Errore",				"Temperatura ambiente Erro",						"Temp. ambiente Erro"	}	},
	{ ESTR_ANALYSIS_CANCELED		,	{	"Analysis canceled by operator",	"Anal. annulée par l'opérateur",			"Anal. vom Bedien. abgebrochen",		"Anál. cancelado por oper.",							"Anal. annullata dall'operatore",		"Análise cancelada pelo operador",					"Análise cancelada pelo operador"	}	},
	{ ESTR_ANALYSIS_TIMEOUT			,	{	"Analysis Time-out",				"Temps d'analyse dépassé",					"Analysezeit abgelaufen",				"Tiempo análisis agotado",								"Time-out analisi",						"Tempo de análise esgotado",						"Tempo limite da análise"	}	},
	{ ESTR_ANALYZER_TIMEOUT			,	{	"Analyzer Time-out",				"Dél. exéc. analyseur exp.",				"Analysegerät-Unterbr.",				"Tiempo inactiv. analizad.",							"Time-out analizzatore",				"Tempo inat. analisador",							"Tempo limite do analis."	}	},
	{ ESTR_ANALYZER_TOO_HOT			,	{	"Analyzer Too Hot",					"Analyseur trop chaud",						"Analysegerät zu heiß",					"Analizador muy caliente",								"Analizzatore troppo caldo",			"Analisador muito quente",							"Analisador muito quente"	}	},
	{ ESTR_BARCODE_FORMAT			,	{	"Barcode Format Error",				"Erreur format code barres",				"Barcode Formatfehler",					"Error formato cód. barras",							"Err. formato cod. a barre",			"Erro formato cód. barras",							"Erro formato cód. barras"	}	},
	{ ESTR_BARCODE_SENSOR			,	{	"Barcode Sensor Error",				"Erreur détect. c. barres",					"Barcode Sensorfehler",					"Error sensor cód. barras",								"Err. sens. codice a barre",			"Erro sensor cód. barras",							"Erro sensor cód. barras"	}	},
	{ ESTR_BARCODE_TIMEOUT			,	{	"Barcode Time-out Error",			"Dél. exéc. c. barres exp.",				"Barcode Unterbr.-Fehler",				"Err tiem inact cód barr",								"Err. time-out cod. barre",				"Erro tempo inat. cód bar",							"Erro temp lim. cód. barra"	}	},
	{ ESTR_BEADS_MISSING			,	{	"Beads Missing Error",				"Erreur billes manquantes",					"Beads fehlen, Fehler",					"Err. pérdida cuentas",									"Errore granuli mancanti",				"Erro de perda de esferas",							"Erro de esferas ausentes"	}	},
	{ ESTR_CAM_DOWN_JAM				,	{	"CAM Down Jam",						"Blocage CAM haut",							"CAM oben Stau",						"LEVA bloqueada abajo",									"Blocco CAMMA giù",						"CAM bloqueada abaixo",								"Emperramento CAM p/ baixo"	}	},
	{ ESTR_CAM_UP_JAM				,	{	"CAM Up Jam",						"Blocage CAM bas",							"CAM unten Stau",						"LEVA bloqueada arriba",								"Blocco CAMMA su",						"CAM bloqueada acima",								"Emperramento CAM p/ cima"	}	},
	{ ESTR_CD_READ					,	{	"Error Reading CD ROM",				"Erreur lecture CD ROM",					"Fehler beim Lesen der CD-ROM",			"Error al leer el CD-ROM",								"Errore di lettura CD ROM",				"Erro de leitura do CD-ROM",						"Erro ao ler o CD ROM"	}	},
	{ ESTR_CORRUPTED_CALIBRATION	,	{	"Corrupted Calibration",			"Calibrage corrompu",						"Fehlerhafte Kalibrierung",				"Calibración errónea",									"Calibrazione corrotta",				"Calibração corrompida",							"Calibração corrompida"	}	},
	{ ESTR_CUVETTE_SENSOR			,	{	"Cuvette Sensor Error",				"Erreur détecteur cuvette",					"Cuvette Sensorfehler",					"Error sensor cubeta",									"Errore sensore cuvette",				"Erro do sensor de cuvetes",						"Erro do sensor da cuveta"	}	},
	{ ESTR_CUVETTE_TIMEOUT			,	{	"Cuvette Time-out Error",			"Err. délai exéc. cuvette",					"Cuvette Unterbr.-Fehler",				"Err tiem inact cubeta",								"Errore time-out cuvette",				"Erro tempo inat. cuvete",							"Erro tempo limite cuveta"	}	},
	{ ESTR_DILUENT					,	{	"Diluent Error",					"Erreur diluant",							"Fehler des Verdünners",				"Error del diluyente",									"Errore diluente",						"Erro de solvente",									"Erro de diluente"	}	},
	{ ESTR_DILUENT_ABSORBANCE		,	{	"Diluent Absorbance Error",			"Erreur absorbance diluant",				"Verdünner Absorb.-Fehler",				"Err absorbancia diluyente",							"Err. assorb. diluente",				"Erro absorvância solvente",						"Erro absorção do diluente"	}	},
	{ ESTR_DILUTION					,	{	"Dilution Error",					"Erreur dilution",							"Verdünnungsfehler",					"Error de dilución",									"Errore diluizione",					"Erro de diluição",									"Erro de diluição"	}	},
	{ ESTR_DISTRIBUTION				,	{	"Distribution Error",				"Erreur distribution",						"Verteilungsfehler",					"Error de distribución",								"Errore distribuzione",					"Erro de distribuição",								"Erro de distribuição"	}	},
	{ ESTR_DRAWER_CLOSED_JAM		,	{	"Drawer Closed Jam",				"Blocage fermeture tiroir",					"Fach geschlossen Stau",				"Bloqueo de la bandeja cerrada",						"Blocco cassetto chiuso",				"Bloqueio da gaveta fechada",						"Emperramento de gaveta fechada"	}	},
	{ ESTR_DRAWER_OPEN_JAM			,	{	"Drawer Open Jam",					"Blocage tiroir ouvert",					"Fach offen Stau",						"Bloqueo de la bandeja abierta",						"Blocco cassetto aperto",				"Bloqueio da gaveta aberta",						"Emperramento de gaveta aberta"	}	},
	{ ESTR_ENGINE_RESET				,	{	"Asynchronous Reset",				"Réinitialisation asynchr.",				"Asynchroner Resetvorgang",				"Restablecim. asíncrono",								"Ripristino asincrono",					"Reinício assíncrono",								"Redefinição assíncrona"	}	},
	{ ESTR_ILLEGAL_IP_ADDRESS		,	{	"Illegal IP Address",				"Adresse IP non valide",					"Falsche IP-Adresse",					"Dirección IP no válida",	   							"Indirizzo IP non valido",				"Endereço IP inválido",								"Endereço IP ilegal"	}	},
	{ ESTR_ILLEGAL_IP_MASK			,	{	"Illegal IP Mask",					"Masque IP non valide",						"Falsche IP-Maske",						"Máscara IP no válida",									"Maschera IP non valida",				"Máscara de IP inválida",							"Máscara IP ilegal"	}	},
	{ ESTR_ILLEGAL_ROTOR_TYPE		,	{	"Illegal Disc Type",				"Type disque non autorisé",					"Falscher Scheibentyp",					"Tipo de disco ilegal",									"Tipo disco illegale",					"Tipo de disco inválido",							"Tipo de disco ilegal"	}	},
	{ ESTR_ILLEGAL_SOCKET			,	{	"Illegal Socket Value",				"Valeur de socket non valide",				"Falscher Socket-Wert",					"Valor de socket no válido",							"Valore socket non valido",				"Valor da tomada inválido",							"Valor de soquete ilegal"	}	},
	{ ESTR_INITIALIZATION			,	{	"Initialization Error",				"Erreur d'initialisation",					"Initialisierungsfehler",				"Error de inicio",										"Errore inizializzazione",				"Erro de inicialização",							"Erro de inicialização"	}	},
	{ ESTR_INSUFF_SAMPLE			,	{	"Insufficient Sample Error",		"Erreur éch. insuffisant",					"Unzureich. Probe, Fehler",				"Err. muestra insuficien",								"Err. campione insuff.",				"Erro amostra insuficiente",						"Erro amostra insuficiente"	}	},
	{ ESTR_INSUFF_STORAGE			,	{	"Insufficient Storage Space",		"Espace de stockage insuffisant",			"Ungenügend Speicherplatz",				"Espacio insuficiente",									"Spazio archiviazione insufficiente",	"Espaço de armazenamento insuficiente",				"Espaço de armazenamento insuficiente"	}	},
	{ ESTR_INTERNAL_COMM			,	{	"Internal Comm. Error",				"Comm. interne Erreur",						"Interne Komm. Fehler",					"Com. interna Error",									"Comm. interno Errore",					"Com. interna Erro",								"Com. interna Erro"	}	},
	{ ESTR_INTERNAL_HEATER			,	{	"Internal Heater Error",			"Err. disp. chauf. interne",				"Interner Fehler Heizgerät",			"Err. calentador interno",								"Errore riscald. interno",				"Erro de aquecedor interno",						"Erro do aquecedor interno"	}	},
	{ ESTR_INTERNAL_LOGIC			,	{	"Internal Logic Error",				"Erreur logique interne",					"Interner Logic-Fehler",				"Error lógico interno",									"Errore logica interna",				"Erro lógico interno",								"Erro da lógica interna"	}	},
	{ ESTR_INTERNAL_PS				,	{	"Internal Power Error",				"Erreur d'alim. interne",					"Interner Energiefehler",				"Error de energía interno",								"Errore corrente interna",				"Erro de energia interno",							"Erro de energia interna"	}	},
	{ ESTR_INTERNAL_SOFTWARE		,	{	"Internal Software Error",			"Erreur logiciel interne",					"Interner Softwarefehler",				"Error de software interno",							"Errore software interno",				"Erro de software interno",							"Erro de software interno"	}	},
	{ ESTR_IQC_BEAD					,	{	"iQC Bead Error",					"Erreur bille iQC",							"iQC-Bead-Fehler",						"Error de cuenta iQC",									"Errore granuli iQC",					"Erro de esfera iQC",								"Erro de esfera iQC"	}	},
	{ ESTR_IQC_BEAD_MIX				,	{	"iQC Bead Mix Error",				"Erreur mix billes iQC",					"iQC-Bead-Mischfehler",					"Error mezcla cuenta iQC",								"Err. miscela granuli iQC",				"Erro de mist. esfera iQC",							"Erro mistura esferas iQC"	}	},
	{ ESTR_IQC_PRECISION			,	{	"iQC Precision Error",				"Erreur précision iQC",						"iQC-Präzisionsfehler",					"Error de precisión iQC",								"Errore precisione iQC",				"Erro de precisão iQC",								"Erro de precisão de iQC"	}	},
	{ ESTR_IQC_RATIO				,	{	"iQC Ratio Error",					"Erreur ratio iQC",							"iQC-Verhältnisfehler",					"Error de proporción iQC",								"Errore rapporto iQC",					"Erro de proporção iQC",							"Erro de relação de iQC"	}	},
	{ ESTR_LAMP_LEVEL				,	{	"Flash Lamp Level Error",			"Erreur niveau lampe flash",				"Blinkleuchte-Stufenfehler",			"Error nivel lámpara flash",							"Err. liv. lampada flash",				"Erro nível flash lâmpada",							"Erro nível lâmpada flash"	}	},
	{ ESTR_LAMP_NOISE				,	{	"Flash Lamp Noise Error",			"Erreur bruit lampe flash",					"Blinkleuchte-Geräuschfeh.",			"Error ruido lámpara flash",							"Err. rumore lamp. flash",				"Erro ruído flash lâmpada",							"Erro ruído lâmpada flash"	}	},
	{ ESTR_MEMORY_READ				,	{	"Memory Read Error",				"Erreur de lecture mémoire",				"Fehler bei Speicheraufruf",			"Error lectura memoria",								"Errore lettura memoria",				"Erro leitura memória",								"Erro leitura memória"	}	},
	{ ESTR_MEMORY_SAVE				,	{	"Memory Save Error",				"Erreur enregistr. mémoire",				"Fehler bei Speicherung",				"Err. guardar memoria",									"Errore salv. Memoria",					"Erro guardar na memória",							"Erro salvamento memória"	}	},
	{ ESTR_NEW_ROTOR_TYPE			,	{	"New Disc Type",					"Nouveau type de disque",					"Neuer Scheibentyp",					"Nuevo tipo de disco",									"Nuovo tipo disco",						"Tipo novo de disco",								"Novo tipo de disco"	}	},
	{ ESTR_NO_FILES_TO_ARCHIVE		,	{	"No Files to Archive",				"Aucun fichier à archiver",					"Keine zu archiv. Dateien",				"No archi para archivar",								"Nessun file da archiviare",			"Nenhum ficheiro arquivar",							"Sem arquivo para arquivar"	}	},
	{ ESTR_NO_FILES_TO_TRANSMIT		,	{	"No Files to Transmit",				"Aucun fich. à transmettre",				"Keine zu übertr. Dateien",				"No archi para transmit",								"Nessun file da trasm.",				"Nenhum ficheiro transmit.",						"Sem arquivo p/ transmitir"	}	},
	{ ESTR_NO_RESULTS_TO_TRANSMIT	,	{	"No Results to Transmit",			"Aucun résulat à trans.",					"Keine Erg. zu übertragen",				"No result para transmit",								"Nessun risult. da trasm.",				"Nenhum result. transmitir",						"Sem result. p/ transmitir"	}	},
	{ ESTR_OP_ID_LIST_FULL			,	{	"Operator ID List Full",			"Liste ID opérateur pleine", 				"Vollst. Bediener-ID-Liste",			"List id. operari complet",								"Elenco ID Oper. completo",				"Lista ID operadores comp.",						"Lista IDs oper. completa"	}	},
	{ ESTR_ORDER_WAS_CANCELED		,	{	"Order Was Canceled",				"Commande a été annulée",					"Bestellung storniert",					"El pedido se ha cancelado",							"Ordine cancellato", 					"O pedido foi cancelado",							"Solicitação cancelada"	}	},
	{ ESTR_PATIENT_IDS_DONT_MATCH	,	{	"Patient IDs Do Not Match",			"ID patient ne corr. pas",					"Patienten-IDs unstimmig",				"No coinciden Id.  pacien",								"ID paziente non corrisp.",				"IDs doente não coincidem",							"IDs paciente não corresp."	}	},
	{ ESTR_PHOTOMETER				,	{	"Photometer Error",					"Erreur photomètre",						"Fotometer-Fehler",						"Error del fotómetro",									"Errore fotometro",						"Erro do fotómetro",								"Erro de fotômetro"	}	},
	{ ESTR_PLEASE_REMOVE_ROTOR		,	{	"Please Remove Disc",				"Retirer le disque",						"Bitte Scheibe entfernen",				"Retire el disco",										"Rimuovere il disco",					"Retirar disco",									"Remova o disco"	}	},
	{ ESTR_PRINTER_NOT_FOUND		,	{	"Printer Not Found",				"Imprimante introuvable",					"Kein Drucker gefunden",				"No se encuentra impresora",							"Stampante non trovata",				"Impressora não localizada",						"Impressora não encontrada"	}	},
	{ ESTR_PRODUCT_CODE				,	{	"Illegal Product Code",				"Code produit non autorisé",				"Falscher Produktcode",					"Código de producto ilegal",							"Codice prodotto illegale",				"Cód. produto inválido",							"Código de produto ilegal"	}	},
	{ ESTR_RESULT_CORRUPTION		,	{	"Saved Results Error",				"Erreur dans les résultats enregistrés",	"Fehler bei gespeicherten Ergebnissen",	"Error de resultados guardados",						"Errore risultati salvati",				"Erro de resultados guardados",						"Erro de resultados salvos"	}	},
	{ ESTR_ROTOR_DATE_ILLEGAL		,	{	"Disc Date Illegal",				"Date disque non autorisée",				"Falsches Scheibendatum",				"Fecha de disco ilegal",								"Data disco illegale",					"Data do disco inválida",							"Data do disco ilegal"	}	},
	{ ESTR_ROTOR_DATE_IMPROPER		,	{	"Disc Date Improper",				"Date disque incorrecte",					"Unzul. Scheibendatum",					"Fecha disco no adecuada",								"Data disco impropria",					"Data do disco inadequada",							"Data do disco imprópria"	}	},
	{ ESTR_ROTOR_EXPIRED			,	{	"Disc Expired",						"Expiration disque",						"Abgelaufene Scheibe",					"Disco caducado",										"Disco scaduto",						"Disco expirado",									"Disco expirado"	}	},
	{ ESTR_RQC						,	{	"RQC Error",						"Erreur RQC",								"RQC-Fehler",							"Error RQC",											"Errore RQC",							"Erro RQC",											"Erro de RQC"	}	},
	{ ESTR_SAMPLE_BLANK_BEAD		,	{	"Sample Blank Bead Error",			"Erreur bille vierge éch.",					"Probe leer, Bead-Fehler",				"Err.  cuent. virg.  muest",							"Err. granuli camp. vuoti",				"Erro esf. branco amostra",							"Erro esf. vazia amostra"	}	},
	{ ESTR_SAMPLE_DISTRIBUTION		,	{	"Sample Distribution Error",		"Erreur distribution éch.",					"Probenverteilungsfehler",				"Err. distrib. muestra",								"Err. distrib. campione",				"Erro de distrib. amostra",							"Erro distr. de amostra"	}	},
	{ ESTR_SAMPLE_MIX				,	{	"Sample Mix Error",					"Erreur mix échantillon",					"Probenmischfehler",					"Error mezcla muestra",									"Errore miscela campione",				"Erro de mist. da amostra",							"Erro mistura de amostra"	}	},
	{ ESTR_SAMPLE_QUALITY			,	{	"Sample Quality Error",				"Erreur qualité échantil.",					"Fehler, Probenqualität",				"Error calidad muestra",								"Errore qualità campione",				"Erro de qual. da amostra",							"Erro qualidade da amostra"	}	},
	{ ESTR_SPINDLE_MOTOR			,	{	"Spindle Motor Error",				"Erreur moteur de l'axe",					"Fehler, Spindelmotor",					"Error del motor del eje",								"Errore asse motore",					"Erro do eixo do motor",							"Erro de motor do fuso"	}	},
	{ ESTR_SPINDLE_MOTOR_STUCK		,	{	"Spindle Motor Error",				"Erreur moteur de l'axe",					"Fehler, Spindelmotor",					"Error del motor del eje",								"Errore asse motore",					"Erro do eixo do motor",							"Erro de motor do fuso"	}	},
	{ ESTR_TEMPERATURE				,	{	"Temperature Error",				"Erreur température",						"Temperaturfehler",						"Error de temperatura",									"Errore temperatura",					"Erro de temperatura",								"Erro de temperatura"	}	},
	{ ESTR_UPDATED_ROTOR_TYPE		,	{	"Updated Disc Type",				"Type de disque actualisé",					"Aktual. Scheibentyp",					"Tipo de disco actualizado",							"Tipo disco aggiornato",				"Tipo de disco atualizado",							"Tipo de disco atualizado"	}	},

// Warning screens							ENGLISH,																													FRENCH,																																		GERMAN,																																	SPANISH,																													ITALIAN, 																														PORTUGUESE,																															PORTUGUESE (BRAZIL)
	{ ESTR_ADMIN_NOT_FOUND			,	{	"Administrator ID not found",																								"ID admin. introuvable",																													"Administrator-ID nicht gefunden",																										"No se encontró Id. administrador",																							"ID amministratore non trovato",																								"ID de administrador não encontrada",																								"ID de administrador não encontrado"	}	},
	{ ESTR_CANNOT_REMOVE_OP_ID		,	{	"All operators must be deleted before the Operator ID screen can be removed",												"Tous les opérateurs doivent être supprimés avant retrait de l'écran ID opérat.",															"Alle Bediener müssen gelöscht werden, bevor der Bediener-ID-Bildschirm entfernt werden kann",											"Debe eliminar todos los operarios antes de eliminar la pantalla Id. operario",												"Prima di rimuovere lo schermo ID operatore, annullare tutti gli operatori",													"Devem ser eliminados todos os operadores antes do ecrã de ID de operadores poder ser eliminado",									"Todos os operadores devem ser excluídos antes que a tela de ID de operador possa ser removida"	}	},
	{ ESTR_CONTROL_EXPIRED			,	{	"Control renewal required",																									"Renouvell. contrôle obligatoire",																											"Kontrollerneuerung erforderlich",																										"Debe renovar control",																										"Necessario rinnovo controllo",																									"É necessário renovar o controlo",																									"Renovação do controle necessária"	}	},
	{ ESTR_DATA_LOSS				,	{	"Changing software will result in data loss.  Press CANCEL to archive data prior to upgrading software.",					"La mise à niveau va occasionner la perte de données. Appuyer sur ANNULER pour archiver les données avant de mettre à niveau le logiciel.",	"Bei einer Softwareaktualisierung werden Daten gelöscht. ABBRECHEN betätigen, um Daten vor der Softwareaktualisierung zu archivieren.",	"Al cambiar el software perderá sus datos. Pulse CANCELAR si desea archivar los datos antes de actualizar el software.",	"La modifica di software determina la perdita di dati. Premere ANNULLA per archiviare i dati prima di aggiornare il software.",	"Ao alterar o software irá perder os seus dados. Prima CANCELAR se desejar arquivar os dados antes da atualização do software.",	"A alteração do software resultará em perda de dados. Pressione CANCELAR para arquivar os dados antes de atualizar o software."	}	},
	{ ESTR_DUPLICATE_ADMIN_ID		,	{	"Duplicate Administrator ID entered",																						"ID admin. saisi en double",																												"Doppelte Administrator-ID eingegeben",																									"Esta Id. administrador ya existe",																							"Inserito duplicato ID amministratore",																							"A ID do administrador já existe",																									"ID de administrador duplicado inserido"	}	},
	{ ESTR_DUPLICATE_OP_ID			,	{	"Duplicate Operator ID entered",																							"ID opérateur saisi en double",																												"Doppelte Bediener-ID eingegeben",																										"Esta Id. operario ya existe",																								"Inserito duplicato ID operatore",																								"A ID do operador já existe",																										"Código de operador duplicado digitado"	}	},
	{ ESTR_OP_RENEWAL_REQUIRED		,	{	"Operator renewal required",																								"Renouvell. opérateur obligatoire",																											"Bediener-Erneuerung erforderlich",																										"Debe renovar operario",																									"Necessario rinnovo operatore",																									"É necessário renovar o operador",																									"Renovação de operador necessária"	}	},
	{ ESTR_REMOVE_FROM_ADMIN_LIST	,	{	"will be removed from administrators list",																					"sera retiré de la liste d'administrateurs",																								"wird aus der Administratorenliste entfernt",																							"se eliminará de la lista de administradores", 																				"sarà rimosso dall'elenco amministratori",																						"será eliminado da lista de administradores",																						"será removido da lista de administradores"	}	},
	{ ESTR_REMOVE_FROM_OP_LIST		,	{	"will be removed from operators list",																						"sera retiré de la liste d'opérateurs",																										"wird aus der Bedienerliste entfernt",																									"se eliminará de la lista de operarios",																					"sarà rimosso dall'elenco operatori",																							"será eliminado da lista de operadores",																							"será removido da lista de operadores"	}	},
	{ ESTR_REMOVE_FROM_TYPE_LIST	,	{	"will be removed from Type list",																							"sera retiré du type de liste",																												"wird aus der Typenliste entfernt",																										"se eliminará de la lista de tipos",																						"sarà rimosso dall'elenco Tipi",																								"será eliminado da lista de tipos",																									"será removido da lista de Tipo"	}	},
	{ ESTR_RESTORING_REF_RANGES		,	{	"Restoring Reference Ranges to",																							"Rétablissement des fourchettes de référence sur",																							"Wiederherstellung der Referenzbereiche auf",																							"Reestablecer los intervalos de referencia a",																				"Riportare i range di riferimento a",																							"Restaurar os intervalos de referência para",																						"Restaurando intervalos de referência para"	}	},
	{ ESTR_ROTOR_DETECTED			,	{	"Disc detected during warming",																								"Disque détecté en cours de chauffage",																										"Scheibe beim Aufwärmen vorgefunden",																									"Disco detectado durante el calentamiento inicial",																			"Disco individuato durante riscaldamento",																						"Disco detetado durante o aquecimento",																								"Disco detectado durante o aquecimento"	}	},
	{ ESTR_UNAUTHORIZED_OP			,	{	"Unauthorized operator",																									"Opérateur non-autorisé",																													"Unautorisierter Bediener",																												"Operario no autorizado",																									"Operatore non autorizzato",																									"Operador não autorizado",																											"Operador não autorizado"	}	},

// Error info text
	{ EISTR_ADMIN_ID_LIST_FULL		,	{	"All the available space for Administrator ID's is used.  Please remove at least one Administrator ID before adding a new Administrator ID.\n",													// English
											"Tout l'espace disponible réservé aux ID administrateur est utilisé. Veuillez supprimer au moins un ID administrateur avant d'en ajouter un nouveau.\n",										// French
											"Der gesamte Speicherplatz für Administrator-IDs ist belegt. Bitte entfernen Sie mindestens eine Administrator-ID, bevor Sie eine neue hinzufügen.\n",											// German
											"Se ha utilizado todo el espacio disponible para números de identificación de administradores. Elimine al menos un número de identificación de administrador antes de añadir otro nuevo.\n",	// Spanish
											"È stato utilizzato tutto lo spazio disponibile per l'ID amministratore. Rimuovere almeno un ID amministratore prima di aggiungere un nuovo ID amministratore.\n",								// Italian
											"Foi utilizado todo o espaço disponível para IDs de administradores.  Elimine pelo menos uma ID de administrador antes de adicionar uma ID de administrador nova.\n",							// Portuguese
											"Todo o espaço disponível para os IDs de administrador está usado.  Remova pelo menos um ID de administrador antes de adicionar um novo ID de administrador.\n"									// Portuguese (Brazil)
										}
	},

	{ EISTR_ADMIN_NOT_FOUND			,	{	"The administrator ID entered is not in the system. Please re-enter the ID.\n",						// English
											"L'ID admin. n'est pas saisi sur le système. Saisir à nouveau l'ID\n",								// French
											"Die eingegebene Administrator-ID existiert nicht im System. Bitte geben Sie die ID erneut ein.\n",	// German
											"Esta Id. administrador no está en el sistema. Por favor, vuelva a escribirla.\n",					// Spanish
											"L'ID amministratore inserito non presente nel sistema. Pregasi inserire ID nuovamente.\n",			// Italian
											"A ID de administrador introduzida não está no sistema. Introduza novamente a ID.\n",				// Portuguese
											"O ID de administrador inserido não está no sistema. Insira novamente o ID.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_AMBIENT_TEMP			,	{	"The temperature outside the analyzer is either too hot or too cold to run a disc. Contact Technical Service for assistance.\n",															// English
											"La température à l'extérieur de l'analyseur est trop élevée ou trop faible pour permettre le fonctionnement d'un disque. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Die Temperatur außerhalb des Analysegeräts ist für den Scheibenbetrieb zu heiß oder zu kalt. Wenden Sie sich an den technischen Service.\n",												// German
											"La temperatura fuera del analizador es demasiado alta o demasiado baja para usar el disco. Póngase en contacto con el servicio técnico.\n",												// Spanish
											"La temperatura esterna all'analizzatore è troppo alta o troppo bassa per permettere il funzionamento di un disco. Per assistenza contattare il servizio tecnico.\n",						// Italian
											"A temperatura fora do analisador é demasiado alta ou demasiado baixa para executar um disco. Contacte o Serviço de Assistência Técnica para obter assistência.\n",							// Portuguese
											"A temperatura externa do analisador está muito alta ou muito baixa para executar um disco. Entre em contato com o Serviço Técnico para obter assistência.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_ANALYSIS_CANCELED		,	{	"The analysis was cancelled by pressing the Cancel on the screen.\n",					// English
											"Annulation de l'analyse par pression sur la touche Annuler sur l'écran\n",				// French
											"Die Analyse wurde mit der Schaltfläche Abbrechen auf dem Bildschirm abgebrochen.\n",	// German
											"El análisis fue cancelado al pulsar Cancelar en la pantalla.\n",						// Spanish
											"L'analisi è stata annullata premendo il tasto Annulla sullo schermo.\n",				// Italian
											"A análise foi cancelada ao premir Cancelar no ecrã.\n",								// Portuguese
											"A análise foi cancelada pela pressão do botão Cancelar na tela.\n"						// Portuguese (Brazil)
										}
	},
	{ EISTR_ANALYSIS_TIMEOUT		,	{	"The analyzer is not completing the analysis within the time allotted. Contact Technical Service for assistance.\n",								// English
											"L'Analyseur n'a pas terminé l'analyse dans le temps imparti. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											"Das Analysegerät führt die Analyse nicht im vorgesehenen Zeitraum durch. Wenden Sie sich an den technischen Service.\n",						// German
											"El analizador no está completando el análisis en el tiempo asignado. Póngase en contacto con el servicio técnico.\n",							// Spanish
											"L'analizzatore non sta completando l'analisi entro il tempo assegnato. Per assistenza contattare il servizio tecnico\n",						// Italian
											"O analisador não está a concluir a análise no tempo atribuído.  Contacte o Serviço de Assistência Técnica para obter assistência\n",			// Portuguese
											"O analisador não está concluindo a análise dentro do tempo alocado.  Entre em contato com o Serviço Técnico para obter assistência\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_ANALYZER_DATE			,	{	"Date in analyzer is earlier than that of the disc being run. Check dates in analyzer and rerun with new disc.\n",												// English
											"La date de l'analyseur est antérieure à celle du disque en cours d'utilisation. Vérifier les dates sur l'analyseur et réexécuter le nouveau disque.\n",		// French
											"Das Datum im Analysegerät liegt vor dem des Scheibenbetriebs. Prüfen Sie das Datum im Analysegerät und nehmen Sie den Betrieb mit einer neuen Scheibe auf.\n",	// German
											"La fecha del analizador es anterior a la fecha del disco que se va a usar. Compruebe las fechas del analizador y vuelva a usar el nuevo disco.\n",				// Spanish
											"La data dell'analizzatore è precedente a quella di attivazione del disco Controllare le date dell'analizzatore e riavviare con un nuovo disco.\n",				// Italian
											"A data do analisador é anterior à data do disco que foi iniciado. Verifique as datas do analisador e reinicie com um disco novo.\n",							// Portuguese
											"A data no analisador é anterior à do disco em execução. Verifique as datas no analisador e execute novamente com um novo disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_BACKGROUND_ABSORBANCE	,	{	"Background absorbance is too high. Contact Technical Service for assistance.\n",								// English
											"L'absorbance d'arrière-plan est trop élevée. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Die Hintergrundabsorbierung ist zu hoch. Wenden Sie sich an den technischen Service.\n",						// German
											"La absorbancia de fondo es muy alta. Póngase en contacto con el servicio técnico.\n",							// Spanish
											"L'assorbimento di fondo è troppo alto. Per assistenza contattare il servizio tecnico.\n",						// Italian
											"A absorvância de fundo é muito elevada. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											"A absorbância do fundo está muito alta. Entre em contato com o Serviço Técnico para obter assistência.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_BACKGROUND_NOISE		,	{	"The analyzer detected a high background signal. This may impact the accuracy of the analyzer's absorbance measurement. If symptom continues, contact Technical Service.\n",														// English
											"L'analyseur a détecté un signal d'arrière-plan élevé. Ceci peut affecter la précision de la mesure d'absorbance par l'analyseur. En cas de persistance des symptômes, contacter le service technique.\n",							// French
											"Das Analysegerät hat ein hohes Hintergrundsignal festgestellt. Dies kann die Genauigkeit der Absorbierungsmessung des Analysegeräts beeinträchtigen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",	// German
											"El analizador detecta una señal de fondo alta. Esto puede afectar a la precisión de la medida de absorbancia del analizador. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",			// Spanish
											"L'analizzatore ha individuato un forte segnale di fondo. Esso potrebbe influire sulla precisione della misurazione di assorbenza dell'analizzatore. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											"O analisador detetou um sinal de fundo elevado. Isto pode afetar a precisão da medição da absorvância do analisador. Se o problema persistir, contacte o Serviço de Assistência Técnica.\n",										// Portuguese
											"O analisador detectou um sinal de fundo alto. Isso pode afetar a precisão da medição de absorbância do analisador. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"												// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE					,	{	"The analyzer was unable to read the disc's barcode. If symptom continues, contact Technical Service.\n",												// English
											"L'analyseur n'a pas pu lire le code à barres du disque. En cas de persistance des symptômes, contacter le service technique.\n",						// French
											"Das Analysegerät konnte den Barcode der Scheibe nicht lesen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",				// German
											"El analizador no pudo leer el código de barras del disco. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"L'analizzatore non è riuscito a leggere il codice a barre del disco. Se i sintomi continuano contattare il servizio tecnico.\n",						// Italian
											"O analisador não consegue ler o código de barras do disco. Se o problema persistir, contacte o Serviço de Assistência Técnica.\n",						// Portuguese
											"O analisador não foi capaz de ler o código de barras do disco. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE_FORMAT			,	{	"The analyzer did not recognize the disc. Contact Technical Service for further assistance.\n",							// English
											"L'analyseur n'a pas reconnu le disque. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											"Das Analysegerät konnte die Scheibe nicht erkennen. Wenden Sie sich an den technischen Service.\n",					// German
											"El analizador no ha reconocido el disco. Póngase en contacto con el servicio técnico.\n",								// Spanish
											"L'analizzatore non ha riconosciuto il disco. Per ulteriore assistenza contattare il servizio tecnico.\n",				// Italian
											"O analisador não consegue reconhecer o disco. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											"O analisador não reconheceu o disco. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MISSING			,	{	"The analyzer did not detect all the chemistry beads in the disc. Rerun sample using new disc.\n",										// English
											"L'analyseur n'a pas détecté toutes les billes chimiques dans le disque. Réexécuter l'échantillon à l'aide d'un nouveau disque.\n",		// French
											"Das Analysegerät konnte nicht alle chemischen Beads in der Scheibe erkennen. Wiederholen Sie die Probe mit einer neuen Scheibe.\n",	// German
											"El analizador no detectó todas las cuentas químicas en el disco. Vuelva a utilizar la muestra usando un nuevo disco.\n",				// Spanish
											"L'analizzatore non ha individuato tutti i granuli chimici nel disco. Riavviare il campione con un nuovo disco.\n",						// Italian
											"O analisador não detetou todas as esferas químicas no disco. Executar novamente a amostra utilizando um disco novo.\n",				// Portuguese
											"O analisador não detectou todas as esferas de químicas no disco. Execute novamente a amostra usando um novo disco.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MIX				,	{	"The analyzer will not be able to determine the exposure status if the QC beads have not mixed correctly. Rerun with a new disc.\n",									// English
											"L'analyseur ne parviendra pas à déterminer l'état d'exposition si les billes QC ne sont pas correctement mélangées. Réexécuter avec un nouveau disque.\n",				// French
											"Das Analysegerät kann den Status der Einwirkung nicht feststellen, wenn die QC-Beads nicht ordnungsgemäß vermischt wurden. Mit einem neuen Rotor wiederholen.\n",		// German
											"El analizador no será capaz de determinar el estado de exposición si las cuentas QC no se mezclan correctamente. Debe usarse con un nuevo disco.\n",					// Spanish
											"L'analizzatore non potrà determinare lo stato di esposizione se i granuli di QC non sono stati mescolati correttamente. Riavviare con un nuovo disco.\n",				// Italian
											"O analisador não conseguirá determinar o estado de exposição se as esferas de CQ não forem misturadas adequadamente. Executar novamente utilizando um disco novo.\n",	// Portuguese
											"O analisador não poderá determinar o status de exposição se as esferas de QC não tiverem sido misturadas corretamente. Execute novamente com um novo disco.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_BIAS					,	{	"The analyzer detected a high bias signal. This may impact the accuracy of the analyzer's measurement of absorbance. If symptom continues, contact Technical Service.\n",														// English
											"L'analyseur a détecté un signal de polarisation élevé, ce qui peut affecter la précision de la mesure d'absorbance par l'analyseur. En cas de persistance des symptômes, contacter le service technique.\n",					// French
											"Das Analysegerät hat ein hohes Störsignal festgestellt. Dies kann die Genauigkeit der Absorbierungsmessung des Analysegeräts beeinträchtigen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",		// German
											"El analizador detecta una señal de polarización alta. Esto puede afectar a la precisión de la medida de absorbancia del analizador. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"L'analizzatore ha individuato un forte segnale di bias. Esso potrebbe influire sulla precisione della misurazione di assorbenza dell'analizzatore. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											"O analisador detetou um sinal de desvio elevado. Isto pode afetar a precisão da medição da absorvância do analisador. Se o problema persistir, contacte o Serviço de Assistência Técnica.\n",									// Portuguese
											"O analisador detectou um sinal de polarização alto. Isso pode afetar a precisão da medição de absorbância do analisador. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_BOTTOM_HEATER_HIGH		,	{	"The bottom heater plate draws too much current when turned on at full power. Contact Technical Service for assistance.\n",												// English
											"La plaque chauffante inférieure consomme trop d'électricité lorsqu'elle fonctionne à fond. Pour obtenir une assistance, contacter le service technique.\n",			// French
											"Die untere Heizplatte verbraucht bei Maximalbetrieb zu viel Strom. Wenden Sie sich an den technischen Service.\n",														// German
											"La placa del calentador inferior utiliza mucha corriente cuando se enciende a toda potencia. Póngase en contacto con el servicio técnico.\n",							// Spanish
											"Il disco di riscaldamento inferiore prende troppa corrente quando è acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",					// Italian
											"A placa inferior do aquecedor utiliza demasiada de corrente quando ligada na potência máxima. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											"A placa inferior do aquecedor consome muita corrente quando ligada na potência máxima. Entre em contato com o Serviço Técnico para obter assistência.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_BOTTOM_HEATER_LOW		,	{	"The bottom heater plate did not draw enough current when turned on at full power. Contact Technical Service for assistance.\n",										// English
											"La plaque chauffante inférieure n'a pas consommé trop d'électricité lorsqu'elle a fonctionné à fond. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Die untere Heizplatte verbrauchte bei Maximalbetrieb zu wenig Strom. Wenden Sie sich an den technischen Service.\n",													// German
											"La placa del calentador inferior no utilizó suficiente corriente cuando se puso a toda potencia. Póngase en contacto con el servicio técnico.\n",						// Spanish
											"Il disco di riscaldamento inferiore non ha preso sufficiente corrente quando è acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",			// Italian
											"A placa inferior do aquecedor não utiliza corrente suficiente quando ligada na potência máxima. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											"A placa inferior do aquecedor não atraiu corrente suficiente quando ligada na potência máxima. Entre em contato com o Serviço Técnico para obter assistência.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_CANNOT_REMOVE_OP_ID		,	{	"To remove the operator ID prompt it is necessary to remove all operator IDs first.  Please use the Operator Administration menu to remove all operators, and then return here to remove the operator prompt.\n",																// English
											"Pour supprimer l'invite ID opérateur, il est nécessaire de commencer par supprimer tous les ID opérateur.  Supprimer tous les opérateurs à partir du menu Administration opérateur, puis revenir ici pour supprimer l'invite opérateur.\n",									// French
											"Um die Bediener-ID-Eingabeaufforderung zu entfernen, müssen Sie zuerst alle Bediener-IDs löschen. Bitte machen Sie dies über das Bediener-Administrationsmenü und gehen zurück, um die Bediener-Eingabeaufforderung zu entfernen.\n",											// German
											"Para eliminar el aviso de Id. operario debe eliminar todas las Id. operador. Para esto, vaya al menú Administración de operarios para eliminar todos los operarios y después vuelva aquí para eliminar el aviso.\n",															// Spanish
											"Per rimuovere il prompt ID operatore si devono prima rimuovere tutti gli ID operatore. Pregasi utilizzare il menu Amministrazione operatore per rimuovere tutti gli operatori, quindi tornare a questo punto e rimuovere il prompt operatore.\n",								// Italian
											"Para eliminar o aviso de ID de operador é necessário eliminar primeiro todas as ID de operador.  Para remover todos os operadores utilize o menú Administração de operadores para eliminar todos os operadores e, em seguida, volte novamente aqui para eliminar o aviso.\n",	// Portuguese
											"Para remover o prompt do ID de operador, é necessário remover todos os IDs de operador antes.  Use o menu Administração do operador para remover todos os operadores e, em seguida, retorne aqui para remover o prompt do operador.\n"											// Portuguese (Brazil)
										} 
	},
	{ EISTR_CDROM					,	{	"The analyzer is unable to read information from the CD ROM. Contact Technical Service for assistance.\n",									// English
											"L'analyseur n'est pas en mesure de lire des informations sur le CD ROM. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Das Analysegerät kann die CD-ROM nicht lesen. Wenden Sie sich an den technischen Service.\n",												// German
											"El analizador no puede leer la información del CD-ROM. Póngase en contacto con el servicio técnico.\n",									// Spanish
											"L'analizzatore non è in grado di leggere le informazioni dal CD ROM. Per assistenza contattare il servizio tecnico.\n",					// Italian
											"O analisador não consegue ler a informação do CD-ROM. Contacte o Serviço de Assistência Técnica para obter assistência.\n",				// Portuguese
											"O analisador não é capaz de ler as informações do CD-ROM. Entre em contato com o Serviço Técnico para obter assistência.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_CONTROL_EXPIRED			,	{	"The date set for analyzing control samples has passed.  Please contact technical support for more information.\n",										// English
											"La date prévue pour l'analyse des échantillons de contrôle est dépassée. Pour plus d'informations, contacter l'assistance technique.\n",				// French
											"Das angegebene Datum, um die Kontrollproben zu analysieren, ist abgelaufen. Wenden Sie sich an den technischen Service für weitere Information.\n",	// German
											"La fecha para analizar las muestras de control ya ha pasado. Póngase en contacto con el servicio técnico.\n",											// Spanish
											"La data impostata per l'analisi dei campioni di controllo è stata superata. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",		// Italian
											"A data definida para analisar as amostras controlo foi excedida.  Para obter mais informações, contacte o Serviço de Assistência Técnica.\n",			// Portuguese
											"A data configurada para analisar as amostras de controle já passou.  Entre em contato com o suporte técnico para obter mais informações.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_CUVETTE_MARK			,	{	"The analyzer was unable to detect the disc. If symptom continues, contact Technical Service.\n",												// English
											"L'analyseur n'est pas parvenu à détecter le disque. En cas de persistance des symptômes, contacter le service technique.\n",					// French
											"Das Analysegerät konnte die Scheibe nicht finden. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",					// German
											"El analizador no fue capaz de detectar el disco. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"L'analizzatore non è riuscito a individuare il disco. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"O analisador não consegue detetar o disco. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",								// Portuguese
											"O analisador não conseguiu detectar o disco. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_DARK_NOISE				,	{	"There was high variability in the background signal. This degrades the accuracy of the analyzer to measure absorbance. If symptom continues, contact Technical Service.\n",														// English
											"Le signal d'arrière-plan a présenté une variabilité élevée, ce qui réduit la précision de l'analyseur pour la mesure d'absorbance. En cas de persistance des symptômes, contacter le Service technique.\n",						// French
											"Es bestand eine hohe Hintergrundsignalvarianz. Dies beeinträchtigt die Genauigkeit der Absorbierungsmessung des Analysegeräts. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",							// German
											"Había una gran variabilidad en la señal de fondo. Esto rebaja la precisión del analizador para analizar la medida de la absorbancia. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"Grande variabilità nel segnale di fondo. Ciò influisce negativamente la precisione dell'analizzatore nella misurazione dell'assorbenza. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"Foi observada uma grande variabilidade no sinal de fundo. Isto reduz a precisão do analisador para realizar medições de absorvância. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",						// Portuguese
											"Houve alta variabilidade no sinal de fundo. Isso degrada a precisão do analisador para medir a absorbância. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"														// Portuguese (Brazil)
										} 
	},
	{ EISTR_DATA_LOSS				,	{	"The software needs to change the format of the data on the analyzer.  It is not possible to convert the data and change the software.  Pressing CANCEL will allow the existing data to be archived prior to upgrading the software.  Use the Transmit All selection under the Recall screen to archive the existing data external to the analyzer.  Verify the external archive prior to upgrading the software.\n",								// English
											"Le logiciel doit modifier le format des données sur l'analyseur. Il n'est pas possible de convertir les données et de mettre à niveau le logiciel. Appuyer sur ANNULER pour archiver les données avant la mise à niveau du logiciel. Appuyer sur Transmettre tout sous l'écran Rappel pour archiver les données de l'analyseur sur un support externe. Vérifier l'archive externe avant de mettre à niveau le logiciel.\n",						// French
											"Die Software ändert das Datenformat auf dem Analysesystem. Es ist nicht möglich, die Daten zu konvertieren und die Software zu aktualisieren. ABBRECHEN betätigen, um die bestehenden Daten vor der Softwareaktualisierung zu archivieren. Die Funktion „Alle übertragen“ im Menü „Wiederaufruf“ verwenden, um die bestehenden Daten auf einem externen Gerät zu archivieren. Das externe Archiv vor der Softwareaktualisierung überprüfen.\n",	// German
											"El software debe cambiar el formato de los datos del analizador. No se pueden convertir los datos y cambiar el software. Pulse CANCELAR si desea archivar los datos existentes antes de actualizar el software. Para archivar los datos existentes fuera del analizador, use la opción Trasmitir todo de la pantalla Recordar. Antes de actualizar el software, verifique el archivo externo.\n",													// Spanish
											"Il software deve modificare il formato dei dati sull'analizzatore. Non è possibile convertire i dati e cambiare il software. Premendo ANNULLA si consente l'archiviazione dei dati esistenti prima dell'aggiornamento del software. Usare la selezione Trasmetti tutti nella schermata Richiama per archiviare i dati esistenti all'esterno dell'analizzatore. Verificare l'archivio esterno prima di aggiornare il software.\n",					// Italian
											"O software necessita alterar o formato dos dados do analisador.  Não é possível converter os dados e alterar o software.  Ao premir CANCELAR permitirá arquivar os dados existentes antes de atualizar o software.  Para arquivar os dados existentes externos ao analisador, utilize a opção Transmitir tudo no ecrã Recuperar.  Antes de atualizar o software, verifique o ficheiro externo.\n",													// Portuguese
											"O software precisa alterar o formato dos dados no analisador.  Não é possível converter os dados e alterar o software.  Pressione CANCELAR para que os dados existentes sejam arquivados antes de atualizar o software.  Use a seleção Transmitir todos na tela Recuperar para arquivar os dados existentes externos ao analisador.  Verifique o arquivo externo antes de atualizar o software.\n"													// Portuguese (Brazil)
										}
	},
	{ EISTR_DILUTION				,	{	"The analyzer may not have delivered the correct amount of sample to the reagents. Confirm the reported values.\n",						// English
											"Il se peut que l'analyseur n'ait pas dispensé la quantité d'échantillon voulue sur les réactifs. Confirmer les valeurs fournies.\n",	// French
											"Das Analysegerät hat möglicherweise die falsche Probenmenge den Reagenzien zugeführt. Berichtete Werte bestätigen.\n",					// German
											"Es posible que el analizador no proporcione la cantidad correcta de muestra a los reactivos. Confirme los valores dados.\n",			// Spanish
											"L'analizzatore potrebbe non aver portato la quantità corretta di campione ai reagenti. Confermare i valori riportati.\n",				// Italian
											"É possível que o analisador não tenha fornecido a quantidade adequada de amostra aos reagentes. Confirme os valores reportados.\n",	// Portuguese
											"O analisador pode não ter entregue a quantidade correta de amostra aos reagentes. Confirme os valores relatados.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_DRAWER_CLOSE			,	{	"The drawer appears to be unable to close correctly. Remove any visible obstructions. If symptom continues, contact Technical Service.\n",											// English
											"Le tiroir ne semble pas se fermer correctement. Éliminer tout élément visible responsable du blocage. En cas de persistance des symptômes, contacter le service technique.\n",		// French
											"Das Fach kann nicht ordnungsgemäß geschlossen werden. Entfernen Sie sichtbare Hindernisse. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",				// German
											"Parece que la bandeja no puede cerrarse correctamente. Retire todo obstáculo visible. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"Sembra che il cassetto non riesca a chiudersi perfettamente. Rimuovere qualsiasi ostruzione visibile. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											"Parece que a gaveta não fecha corretamente. Remova qualquer obstáculo visível. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",								// Portuguese
											"A gaveta parece incapaz de fechar corretamente. Remova quaisquer obstruções visíveis. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_DRAWER_OPEN				,	{	"The drawer appears to be unable to open correctly. Remove any visible obstructions. If symptom continues, contact Technical Service.\n",											// English
											"Le tiroir ne semble pas s'ouvrir correctement. Éliminer tout élément visible responsable du blocage. En cas de persistance des symptômes, contacter le service technique.\n",		// French
											"Das Fach kann nicht ordnungsgemäß geöffnet werden. Entfernen Sie sichtbare Hindernisse. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",				// German
											"Parece que la bandeja no puede abrirse correctamente. Retire todo obstáculo visible. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"Sembra che il cassetto non riesca ad aprirsi perfettamente. Rimuovere qualsiasi ostruzione visibile. Se i sintomi continuano contattare il servizio tecnico.\n",					// Italian
											"Parece que a gaveta não abre corretamente. Remova qualquer obstáculo visível. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",								// Portuguese
											"A gaveta parece incapaz de abrir corretamente. Remova quaisquer obstruções visíveis. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_DUPLICATE_ADMIN_ID		,	{	"The Administrator ID entered already exists in the system. Duplicate IDs are not allowed.\n",		// English
											"L'ID administrateur saisi existe déjà sur le système. Les ID en double ne sont pas autorisés.\n",	// French
											"Die eingegebene Administrator-ID existiert bereits im System. Doppelte IDs sind nicht erlaubt.\n",	// German
											"Esta Id. administrador ya existe en el sistema. No se permiten Id. duplicadas.\n",					// Spanish
											"L'ID amministratore inserito è già presente nel sistema. Non sono consentiti ID duplicati.\n",		// Italian
											"A ID de administrador introduzida já existe no sistema. Não são permitidas IDs duplicadas.\n",		// Portuguese
											"O ID de administrador inserido já existe no sistema. IDs duplicados não são permitidos.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_DUPLICATE_OP_ID			,	{	"The Operator ID entered already exists in the system. Duplicate IDs are not allowed.\n",		// English
											"L'ID opérateur saisi existe déjà sur le système. Les ID en double ne sont pas autorisés.\n",	// French
											"Die eingegebene Bediener-ID existiert bereits im System. Doppelte IDs sind nicht erlaubt.\n",	// German
											"Esta Id. operario ya existe en el sistema. No se permiten Id. duplicadas.\n",					// Spanish
											"L'ID operatore inserito è già presente nel sistema. Non sono consentiti ID duplicati.\n",		// Italian
											"A ID de operador introduzida já existe no sistema. Não são permitidas IDs duplicadas.\n",		// Portuguese
											"O ID de operador inserido já existe no sistema. IDs duplicados não são permitidos.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_ENGINE_RESET			,	{	"A component of the analyzer has restarted unexpectedly. Contact Technical Service for assistance.\n",									// English
											"Un des composants de l'analyseur a redémarré de manière inattendue. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Eine Komponente des Analysegeräts wurde unerwartet neu gestartet. Wenden Sie sich an den technischen Service.\n",						// German
											"Un componente del analizador se ha reiniciado inesperadamente. Póngase en contacto con el servicio técnico.\n",						// Spanish
											"Un componente dell'analizzatore si è riavviato in modo imprevisto. Per assistenza contattare il servizio tecnico\n",					// Italian
											"Um componente do analisador foi reiniciado inesperadamente.  Contacte o Serviço de Assistência Técnica para obter assistência.\n",		// Portuguese
											"Um componente do analisador foi reiniciado inesperadamente.  Entre em contato com o Serviço Técnico para obter assistência\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_EXPIRED_ROTOR			,	{	"The disc has expired and should not be used. Contact Technical Service for further details.\n",										// English
											"La date de péremption du disque est dépassée. Ne pas l'utiliser. Pour obtenir plus d'informations, contacter le service technique.\n",	// French
											"Die Scheibe ist abgelaufen und sollte ersetzt werden. Wenden Sie sich an den technischen Service.\n",									// German
											"El disco ha caducado y no debería utilizarse. Póngase en contacto con el servicio técnico.\n",											// Spanish
											"Il disco è scaduto e non dovrebbe essere usato. Per ulteriore dettagli contattare il servizio tecnico.\n",								// Italian
											"O disco expirou e não deverá ser utilizado. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",				// Portuguese
											"O disco expirou e não deve ser usado. Entre em contato com o Serviço Técnico para obter detalhes adicionais.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_FILTER_CURV				,	{	"The filter curve calibration factors have been corrupted. Contact Technical Service for assistance.\n",								// English
											"Les facteurs de calibrage de la courbe du filtre sont corrompus. Pour obtenir une assistance, contacter le service technique.\n",		// French
											"Die Kalibrierungsfaktoren der Filterkurve sind fehlerhaft. Wenden Sie sich an den technischen Service.\n",								// German
											"Los factores de calibración de la curva del filtro están dañados. Póngase en contacto con el servicio técnico.\n",						// Spanish
											"I fattori di calibrazione della curva del filtro sono corrotti. Per assistenza contattare il servizio tecnico.\n",						// Italian
											"Os fatores da curva de calibração do filtro estão danificados. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											"Os fatores de calibração da curva do filtro foram corrompidos. Entre em contato com o Serviço Técnico para obter assistência.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_FLASH_LAMP_NOISE		,	{	"During one of the system checks, the analyzer attempted to compute an absorbance from a cluster of flashes that are too variable from each other. If symptom continues, contact Technical Service.\n",															// English
											"Lors de l'une des vérifications du système, l'analyseur a tenté de calculer l'absorbance à partir d'un ensemble de flashs qui varient trop les uns des autres. En cas de persistance des symptômes, contacter le service technique.\n",						// French
											"Bei einer Systemprüfung unternahm das Analysegerät den Versuch, eine Absorbierung aus einer Gruppe von Lichtstrahlen zu berechnen, die zu sehr voneinander abweichen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",				// German
											"Durante una de las comprobaciones del sistema, el analizador intentó calcular una absorbancia de un grupo de flashes que son excesivamente distintos unos de otros. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"Durante uno dei controlli di sistema l'analizzatore ha cercato di calcolare l'assorbenza in un gruppo di flash luminosi troppo diversi tra loro. Se i sintomi continuano contattare il servizio tecnico.\n",													// Italian
											"Durante uma das verificações do sistema, o analisador tentou calcular uma absorvância de um grupo de flashes extremamente diferentes uns dos outros. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",									// Portuguese
											"Durante uma das verificações do sistema, o analisador tentou calcular uma absorbância de um conjunto de flashes que são muito variáveis um em relação ao outro. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_FLASH_READ				,	{	"The analyzer is unable to read information required to operate. Contact Technical Service for assistance.\n",											// English
											"L'analyseur n'est pas en mesure de lire les informations requises pour fonctionner. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Das Analysegerät kann für den Betrieb erforderliche Informationen nicht abrufen. Wenden Sie sich an den technischen Service.\n",						// German
											"El analizador es incapaz de leer información necesaria para funcionar. Póngase en contacto con el servicio técnico.\n",								// Spanish
											"L'analizzatore non è in grado di leggere le informazioni necessarie per il funzionamento. Per assistenza contattare il servizio tecnico.\n",			// Italian
											"O analisador não consegue ler informação necessária para funcionar. Contacte o Serviço de Assistência Técnica para obter assistência.\n",				// Portuguese
											"O analisador não é capaz de ler as informações necessárias para operar. Entre em contato com o Serviço Técnico para obter assistência.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_FLASH_SAVE				,	{	"The analyzer is unable to save information required to operate. Contact Technical Service for assistance.\n",												// English
											"L'analyseur n'est pas en mesure d'enregistrer les informations requises pour fonctionner. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Das Analysegerät kann für den Betrieb erforderliche Informationen nicht speichern. Wenden Sie sich an den technischen Service.\n",							// German
											"El analizador es incapaz de guardar información necesaria para funcionar. Póngase en contacto con el servicio técnico.\n",									// Spanish
											"L'analizzatore non è in grado di salvare le informazioni necessarie per il funzionamento. Per assistenza contattare il servizio tecnico.\n",				// Italian
											"O analisador não consegue gravar informação necessária para funcionar. Contacte o Serviço de Assistência Técnica para obter assistência.\n",				// Portuguese
											"O analisador não é capaz de salvar as informações necessárias para operar. Entre em contato com o Serviço Técnico para obter assistência.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_HARDWARE				,	{	"The analyzer has detected an error in the hardware. Contact Technical Service for further details.\n",					// English
											"L'analyseur a détecté une erreur de matériel. Pour obtenir plus d'informations, contacter le service technique.\n",	// French
											"Das Analysegerät hat einen Hardwarefehler festgestellt. Wenden Sie sich an den technischen Service.\n",				// German
											"El analizador ha detectado un error en el equipo. Póngase en contacto con el servicio técnico.\n",						// Spanish
											"L'analizzatore ha individuato un errore nell'hardware. Per ulteriore dettagli contattare il servizio tecnico.\n",		// Italian
											"O analisador detetou um erro no hardware. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",	// Portuguese
											"O analisador detectou um erro no hardware. Entre em contato com o Serviço Técnico para obter detalhes adicionais.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_ILLEGAL_BARCODE			,	{	"The analyzer has detected an illegal barcode. Contact Technical Service for further details.\n",								// English
											"L'analyseur a détecté un code à barres non autorisé. Pour obtenir plus d'informations, contacter le service technique.\n",		// French
											"Das Analysegerät hat einen falschen Barcode festgestellt. Wenden Sie sich an den technischen Service.\n",						// German
											"El analizador ha detectado un código de barras ilegal. Póngase en contacto con el servicio técnico.\n",						// Spanish
											"L'analizzatore ha individuato un codice a barre illegale. Per ulteriore dettagli contattare il servizio tecnico.\n",			// Italian
											"O analisador detetou um código de barras inválido. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",	// Portuguese
											"O analisador detectou um código de barras ilegal. Entre em contato com o Serviço Técnico para obter detalhes adicionais.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_ILLEGAL_IP_ADDRESS		,	{	"The IP Address entered cannot be used.  Please enter an IP address in the N.N.N.N format where N is a number between 0 and 255.\n",						// English
											"L'adresse IP saisie ne peut pas être utilisée.  Entrer une adresse IP au format N.N.N.N, N étant un nombre compris entre 0 et 255.\n",						// French
											"Die eingegebene IP-Adresse ist nicht nutzbar. Bitte geben Sie eine IP-Adresse im Format N.N.N.N ein, wobei N eine Zahl zwischen 0 und 255 ist.\n",			// German
											"La dirección IP introducida no puede utilizarse.  Por favor, introduzca una dirección IP con el formato N.N.N.N, donde N es un número entre 0 y 255.\n",	// Spanish
											"Impossibile utilizzare l'indirizzo IP specificato.  Immettere un indirizzo IP nel formato N.N.N.N. dove N è un numero compreso fra 0 e 255.\n",			// Italian
											"O endereço IP introduzido não pode ser utilizado.  Introduza um endereço IP com o formato N.N.N.N, onde N é um número entre 0 e 255.\n",					// Portuguese
											"O endereço IP inserido não pode ser usado.  Insira um endereço IP no formato N.N.N.N, em que N é um número entre 0 e 255.\n"								// Portuguese (Brazil)
										}
	},

	{ EISTR_ILLEGAL_IP_MASK			,	{	"The IP Mask entered cannot be used.  Please enter an IP Mask in the N.N.N.N format where N is a number between 0 and 255.\n",								// English
											"Le masque IP saisi ne peut pas être utilisé.  Entrer un masque IP au format N.N.N.N, N étant un nombre compris entre 0 et 255.\n",							// French
											"Die eingegebene IP-Maske ist nicht nutzbar. Bitte geben Sie eine IP-Maske im Format N.N.N.N ein, wobei N eine Zahl zwischen 0 und 255 ist.\n",				// German
											"La máscara IP introducida no puede utilizarse.  Por favor, introduzca una máscara IP con el formato N.N.N.N, donde N es un número entre 0 y 255.\n",		// Spanish
											"Impossibile utilizzare la maschera IP specificata.  Immettere una maschera IP nel formato N.N.N.N. dove N è un numero compreso fra 0 e 255.\n",			// Italian
											"A máscara de IP introduzida não pode ser utilizada.  Introduza uma máscara de IP com o formato N.N.N.N, onde N é um número entre 0 e 255.\n",				// Portuguese
											"A máscara IP inserida não pode ser usada.  Insira uma máscara IP no formato N.N.N.N, em que N é um número entre 0 e 255.\n"								// Portuguese (Brazil)
										}
	},
	{ EISTR_ILLEGAL_SOCKET			,	{	"The socket entered cannot be used.  Please enter a socket number between 0 and 65536.\n",								// English
											"Le socket saisi ne peut pas être utilisé.  Entrer un numéro de socket compris entre 0 et 65536.\n",					// French
											"Der eingegebene Anschluss (Socket) ist nicht nutzbar. Bitte eine Anschlussnummer zwischen 0 und 65536 eingeben.\n",	// German
											"El socket introducido no puede utilizarse.  Por favor, introduzca un número de enchufe entre 0 y 65536.\n",			// Spanish
											"Impossibile utilizzare il socket specificato.  Immettere un numero di socket compreso fra 0 e 65536.\n",				// Italian
											"A tomada introduzida não pode ser utilizada.  Introduza um número de tomada entre 0 e 65 536.\n",						// Portuguese
											"O soquete inserido não pode ser usado.  Insira um número de soquete entre 0 e 65.536.\n"								// Portuguese (Brazil)
										}
	},
	{ EISTR_INSUFFICIENT_DILUENT	,	{	"Insufficient reagents exist in the disc to complete the run. Rerun sample with new disc.\n",														// English
											"La quantité de réactifs dans le disque est insuffisante pour exécuter la procédure. Réexécuter l'échantillon à l'aide du nouveau disque.\n",		// French
											"In der Scheibe befinden sich nicht ausreichend Reagenzien, um den Vorgang abzuschließen. Wiederholen Sie die Probe mit einer neuen Scheibe.\n",	// German
											"No hay suficientes reactivos en el disco para completar su uso. Vuelva a utilizar la muestra con un nuevo disco.\n",								// Spanish
											"Nel disco non vi sono sufficienti reagenti per completare l'operazione. Riavviare il campione con un nuovo disco.\n",								// Italian
											"Não existem reagentes suficientes no disco para concluir a execução. Executar novamente a amostra utilizando um disco novo.\n",					// Portuguese
											"Existem reagentes insuficientes no disco para concluir a execução. Execute novamente a amostra com o novo disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_SAMPLE		,	{	"The disc has not received enough sample to function properly. Rerun using a new disc.\n",										// English
											"Le disque n'a pas reçu suffisamment d'échantillons pour fonctionner correctement. Réexécuter avec un nouveau disque.\n",		// French
											"Die Scheibe enthält keine ausreichende Probenmenge für eine ordnungsgemäße Funktion. Mit einer neuen Scheibe wiederholen.\n",	// German
											"El disco no ha recibido suficiente muestra para funcionar correctamente. Úselo con un nuevo disco.\n",							// Spanish
											"Il disco non ha ricevuto campione sufficiente per funzionare nel modo corretto. Riavviare utilizzando un nuovo disco.\n",		// Italian
											"O disco não recebeu amostra suficiente para funcionar adequadamente. Executar novamente utilizando um disco novo.\n",			// Portuguese
											"O disco não recebeu amostra suficiente para funcionar corretamente. Execute novamente usando um novo disco.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_STORAGE	,	{	"There is insufficient free space on the USB storage device to complete the service archive. Please remove the USB storage device, and connect a USB storage device with at least 256 MB of available space.\n",										// English
											"Espace de stockage sur périphérique USB insuffisant pour réaliser l'archive de service. Retirer le périphérique de stockage USB et brancher un périphérique de stockage USB ayant au moins 25 Mo d'espace disponible.\n",								// French
											"Es ist nicht genügend Speicherkapazität auf dem USB-Speichergerät frei, um das Service-Archiv zu vollenden. Bitte entfernen Sie das USB-Speichergerät und schließen Sie ein USB-Speichergerät mit mindestens 256 MB freiem Speicher an.\n",			// German
											"El dispositivo de almacenamiento USB no tiene espacio suficiente para finalizar el archivo de servicio. Extraiga el dispositivo y conecte otro con al menos 256 MB de espacio disponible.\n",															// Spanish
											"Spazio libero sul dispositivo di archiviazione USB insufficiente per completare l'archivio di servizio. Pregasi rimuovere il dispositivo di archiviazione USB e collegare un dispositivo di archiviazione con almeno 256 MB di spazio disponibile.\n",	// Italian
											"O dispositivo de armazenamento USB não tem espaço suficiente para finalizar o ficheiro de serviço. Remova o dispositivo de armazenamento USB e insira outro com pelo menos 256 MB de espaço disponível.\n",											// Portuguese
											"Não há espaço livre suficiente no dispositivo de armazenamento USB para concluir o arquivo de serviço. Remova o dispositivo de armazenamento USB e conecte um dispositivo de armazenamento USB com pelo menos 256 MB de espaço disponível.\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_INTERNAL_COMM			,	{	"The analyzer lost internal communication. If symptom continues, contact Technical Service.\n",												// English
											"L'analyseur a perdu sa capacité de communication interne. En cas de persistance des symptômes, contacter le service technique.\n",			// French
											"Das Analysegerät hat die interne Kommunikation verloren. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",		// German
											"El analizador perdió la comunicación interna. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"L'analizzatore ha perso la comunicazione interna. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"O analisador perdeu a comunicação interna. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",							// Portuguese
											"O analisador perdeu a comunicação interna. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_INTERNAL_SOFTWARE		,	{	"The analyzer detected an error with the software.  If symptom continues, contact Technical Service.\n",									// English
											"L'analyseur a détecté une erreur logiciel. En cas de persistance des symptômes, contacter le service technique.\n",						// French
											"Das Analysegerät hat einen Softwarefehler festgestellt. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",		// German
											"El analizador detectó un error con el software. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"L'analizzatore ha individuato un errore nel software. Se i sintomi continuano contattare il servizio tecnico.\n",							// Italian
											"O analisador detetou um erro no software.  Se o problema persistir, contacte o serviço de Assistência Técnica.\n",	// Portuguese
											"O analisador detectou um erro no software.  Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_IQC						,	{	"The analyzer Intelligent Quality Control (iQC) reagent dye beads differ too much from the expected values. Rerun using a new disc.\n",													// English
											"Les billes de teinture du réactif Intelligent Quality Control (iQC) divergent trop des valeurs prévues. Réexécuter avec un nouveau disque.\n",											// French
											"Die Intelligent Quality Control (iQC) Reagenz-Farbstoff-Beads des Analysegeräts weichen zu stark von den erwarteten Werten ab. Mit einer neuen Scheibe wiederholen.\n",				// German
											"Las cuentas de tinte de reactivo Intelligent Quality Control (iQC) del analizador son muy distintas a los valores esperados. Úselo con un nuevo disco.\n",								// Spanish
											"I granuli di colore reagente Intelligent Quality Control (iQC) dell'analizzatore sono troppo diversi dai valori previsti. Riavviare utilizzando un nuovo disco.\n",					// Italian
											"As esferas de reagente corante do Controlo de qualidade inteligente (iQC) do analisador são muito diferentes dos valores esperados. Executar novamente utilizando um disco novo.\n",	// Portuguese
											"As esferas do corante reagente de Controle de qualidade inteligente (iQC) do analisador diferem muito dos valores esperados. Execute novamente usando um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_LAMP					,	{	"The lamp is checked each time to make sure it is still flashing when the optical cuvette is sampled. Detectors did not recognize the lamp flashes. If symptom continues, contact Technical Service.\n",																					// English
											"La lampe fait à chaque fois l'objet d'une vérification afin de s'assurer qu'elle fonctionne toujours lors du prélèvement sur la cuvette optique. Les détecteurs n'ont pas reconnu les clignotements de la lampe. En cas de persistance des symptômes, contacter le service technique.\n",	// French
											"Die Lampe wird jedes Mal auf Blinkzeichen überprüft, wenn eine Probe der optischen Cuvette untersucht wird. Es wurden keine Blinkzeichen der Lampen festgestellt. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",												// German
											"Siempre se comprueba la lámpara para asegurar que sigue emitiendo flashes cuando se pone la muestra en la cubeta óptica. Los detectores no registraron la emisión de flashes de la lámpara. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",		// Spanish
											"La lampada viene controllata ogni volta per assicurarsi che stia ancora lampeggiando quando viene campionata la cuvette ottica. I rilevatori non hanno riconosciuto i flash luminosi. Se i sintomi continuano contattare il servizio tecnico.\n",											// Italian
											"A lâmpada é sempre verificada para garantir que emite flashes quando a amostra é colocada na cuvete ótica. Os detetores não registram a emissão de flashes da lâmpada. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",												// Portuguese
											"A lâmpada é verificada a cada vez, para certificar-se de que ainda esteja piscando quando a cubeta óptica for amostrada. Os detectores não reconheceram a luz da lâmpada. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"												// Portuguese (Brazil)
										} 
	},
	{ EISTR_LIGHT_NOISE				,	{	"There was high variability in the signal. This may impact the accuracy of the analyzer's measurement of absorbance. If symptom continues, contact Technical Service.\n",											// English
											"Le signal d'arrière-plan a présenté une variabilité élevée. ce qui peut affecter la précision de la mesure d'absorbance par l'analyseur. En cas de persistance des symptômes, contacter le service technique.\n",	// French
											"Es bestand eine hohe Signalvarianz. Dies kann die Genauigkeit der Absorbierungsmessung des Analysegeräts beeinträchtigen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",				// German
											"Había una gran variabilidad en la señal. Esto puede afectar a la precisión de la medida de absorbancia del analizador. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"Grande variabilità nel segnale. Esso potrebbe influire sulla precisione della misurazione di assorbenza dell'analizzatore. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"Foi observada uma grande variabilidade no sinal. Isto pode afetar a precisão da medição da absorvância do analisador. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",						// Portuguese
											"Houve alta variabilidade no sinal. Isso pode afetar a precisão da medição de absorbância do analisador. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"											// Portuguese (Brazil)
										} 
	},
	{ EISTR_MISSING_FILTER_CURV		,	{	"The filter curve calibration factors have not been loaded into the analyzer. Contact Technical Service for assistance.\n",									// English
											"Les facteurs de calibrage de la courbe du filtre n'ont pas été chargés sur l'analyseur. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Die Kalibrierungsfaktoren der Filterkurve wurden nicht in das Analysegerät geladen. Wenden Sie sich an den technischen Service.\n",						// German
											"Los factores de calibración de la curva del filtro no se han cargado en el analizador. Póngase en contacto con el servicio técnico.\n",					// Spanish
											"I fattori di calibrazione della curva del filtro non sono stati caricati nell'analizzatore. Per assistenza contattare il servizio tecnico.\n",				// Italian
											"Os fatores da curva de calibração do filtro não foram carregados no analisador. Contacte o Serviço de Assistência Técnica para obter assistência.\n",		// Portuguese
											"Os fatores de calibração da curva do filtro não foram carregados no analisador. Entre em contato com o Serviço Técnico para obter assistência.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_MOTOR_CONTROLLER		,	{	"The analyzer detected an error with the motor controller. If symptom continues, contact Technical Service.\n",												// English
											"L'analyseur a détecté une erreur sur le contrôleur du moteur. En cas de persistance des symptômes, contacter le service technique.\n",						// French
											"Das Analysegerät hat einen Fehler an der Motorsteuerung festgestellt. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",			// German
											"El analizador detectó un error con el controlador del motor. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"L'analizzatore ha individuato un errore con il controllore a motore. Se i sintomi continuano contattare il servizio tecnico.\n",							// Italian
											"O analisador detetou um erro no controlador do motor. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",								// Portuguese
											"O analisador detectou um erro no controlador do motor. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_NEEDS_SERVICE			,	{	"The analyzer may need servicing. Contact Technical Service for further details.\n",													// English
											"Il se peut que l'analyseur nécessite une intervention technique. Pour obtenir plus d'informations, contacter le service technique.\n",	// French
											"Das Analysegerät muß möglicherweise gewartet werden. Wenden Sie sich an den technischen Service.\n",									// German
											"Puede que el analizador deba ser reparado. Póngase en contacto con el servicio técnico.\n",											// Spanish
											"L'analizzatore potrebbe aver bisogno di manutenzione. Per ulteriore dettagli contattare il servizio tecnico.\n",						// Italian
											"O analisador pode necessitar de assistência. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",				// Portuguese
											"O analisador pode precisar de manutenção. Entre em contato com o Serviço Técnico para obter detalhes adicionais.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_NO_FILES_TO_ARCHIVE		,	{	"There are no files within the date range specified to archive.\n",					// English
											"Aucun fichier à archiver dans la plage de dates spécifiée.\n",						// French
											"Für den spezifizierten Datumsbereich sind keine Dateien zu archivieren.\n",		// German
											"No hay ningún archivo en el intervalo de fechas especificado para archivar.\n",	// Spanish
											"Non sono presenti file da archiviare nel range di date specificato.\n",			// Italian
											"Não existem ficheiros no intervalo de datas especificado para arquivar. \n",		// Portuguese
											"Não há arquivos dentro do intervalo de datas especificado para arquivar. \n"		// Portuguese (Brazil)
										}	
	},

	{ EISTR_NO_FILES_TO_TRANSMIT	,	{	"There are no files within the date range specified to transmit.\n",				// English
											"Aucun fichier à transmettre dans la plage de dates spécifiée.\n",					// French
											"Für den spezifizierten Datumsbereich sind keine Dateien zu übertragen.\n",			// German
											"No hay ningún archivo en el intervalo de fechas especificado para transmitir.\n",	// Spanish
											"Non sono presenti file da trasmettere nel range di date specificato.\n",			// Italian
											"Não existem ficheiros no intervalo de datas especificado para transmitir.\n",		// Portuguese
											"Não há arquivos dentro do intervalo de datas especificado para transmitir.\n"		// Portuguese (Brazil)
										}	
	},

	{ EISTR_NO_RESULTS_TO_TRANSMIT	,	{	"There are no results within the date range specified to transmit.\n",				// English
											"Aucun résultat à transmettre dans la plage de dates spécifiée.\n",					// French
											"Für den spezifizierten Datumsbereich sind keine Ergebnisse zu übertragen.\n",		// German
											"No hay ningún resultado en el intervalo de fechas especificado para transmitir.\n",// Spanish
											"Non sono presenti risultati da trasmettere nel range di date specificato.\n",		// Italian
											"Não existem resultados no intervalo de datas especificado para transmitir.\n",		// Portuguese
											"Não há resultados dentro do intervalo de datas especificado para transmitir.\n"	// Portuguese (Brazil)
										}	
	},

	{ EISTR_ORDER_WAS_CANCELED		,	{	"The selected order was canceled by the external order entry system.\n",						// English
											"La commande sélectionnée a été annulée par le système d'entrée de commande externe.\n",		// French
											"Die ausgewählte Bestellung wurde vom externen Bestelleingabesystem storniert.\n",				// German
											"El pedido seleccionado ha sido cancelado por el sistema externo de entrada de pedidos.\n",		// Spanish
											"Ordine selezionato cancellato dal sistema di immissione ordini esterno.\n",					// Italian
											"O pedido selecionado foi cancelado pelo sistema de entrada de pedidos externo.\n",				// Portuguese
											"A solicitação selecionada foi cancelada pelo sistema externo de entrada de solicitações.\n"	// Portuguese (Brazil)
										}
	},

	{ EISTR_OP_ID_LIST_FULL			,	{	"All the available space for Operator ID's is used.  Please remove at least one Operator ID before adding a new Operator ID.\n",													// English
											"Tout l'espace disponible réservé aux ID opérateur est utilisé. Veuillez supprimer au moins un ID opérateur avant d'en ajouter un nouveau.\n",										// French
											"Der gesamte Speicherplatz für Bediener-IDs ist belegt.  Bitte entfernen Sie mindestens eine Bediener-ID, bevor Sie eine neue hinzufügen.\n",										// German
											"Se ha utilizado todo el espacio disponible para números de identificación de operarios. Elimine al menos un número de identificación de operario antes de añadir otro nuevo.\n",	// Spanish
											"È stato utilizzato tutto lo spazio disponibile per l'ID operatore. Rimuovere almeno un ID operatore prima di aggiungere un nuovo ID operatore.\n",									// Italian
											"Foi utilizado todo o espaço disponível para IDs de operadores.  Elimine pelo menos uma ID de operador antes de adicionar uma nova ID de operador.\n",								// Portuguese
											"Todo o espaço disponível para os IDs de operador está usado.  Remova pelo menos um ID de operador antes de adicionar um novo ID de operador.\n"									// Portuguese (Brazil)
										}
	},

	{ EISTR_OP_RENEWAL_REQUIRED		,	{	"The date set authorizing the operator to use the analyzer has passed.  Please contact technical support for more information.\n",										// English
											"La date prévue pour autorisation de l'opérateur à utiliser l'analyseur est dépassée. Pour plus d'informations, contacter l'assistance technique.\n",					// French
											"Das angegebene Datum, bis zu dem der Bediener das Analysegerät benutzen kann, ist abgelaufen. Wenden Sie sich an den technischen Service für weitere Information.\n",	// German
											"La fecha para autorizar al operario a usar el analizador ya ha pasado. Póngase en contacto con el servicio técnico.\n",												// Spanish
											"La data impostata per l'analisi dei campioni di controllo è stata superata. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",						// Italian
											"A data definida para autorizar o operador a utilizar o analisador foi excedida.  Para obter mais informações, contacte o serviço de Assistência Técnica.\n",			// Portuguese
											"A data configurada, autorizando o operador a usar o analisador, já passou.  Entre em contato com o suporte técnico para obter mais informações.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_PATIENT_IDS_DONT_MATCH	,	{	"The patient ID entered does not match the ID transmitted to the analyzer.\n",							// English
											"L'ID patient entrée ne correspond pas à l'ID transmise à l'analyseur.\n",								// French
											"Die eingegebene Patienten-ID stimmt nicht mit der an das Analysegerät übermittelten ID überein.\n",	// German
											"El Id. del paciente introducido no coincide con el Id. transmitido al analizador.\n",					// Spanish
											"L'ID paziente inserito non corrisponde a quello trasmesso all'analizzatore.\n",						// Italian
											"A ID de doente introduzida não coincide com a ID transmitida ao analisador.\n",						// Portuguese
											"O ID do paciente inserido não corresponde ao ID transmitido para o analisador.\n"						// Portuguese (Brazil)
										}
	},
	{ EISTR_POWER_SUPPLY			,	{	"The analyzer has detected power fluctuations that prevent the unit from operating correctly. If symptom continues, contact Technical Service.\n",																// English
											"L'analyseur a détecté des fluctuations dans l'alimentation qui empêchent l'appareil de fonctionner correctement. En cas de persistance des symptômes, contacter le service technique.\n",						// French
											"Das Analysegerät hat Energieschwankungen festgestellt, die den ordnungsgemäßen Betrieb der Einheit beeinträchtigen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",				// German
											"El analizador ha detectado fluctuaciones de la corriente que no permiten el funcionamiento correcto de la unidad. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"L'analizzatore ha individuato fluttuazioni di corrente che non permettono di funzionare correttamente. Se i sintomi continuano contattare il servizio tecnico.\n",												// Italian
											"O analisador detetou flutuações de corrente que não permitem o correto funcionamento da unidade. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",										// Portuguese
											"O analisador detectou flutuações de energia que impedem a unidade de funcionar corretamente. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"												// Portuguese (Brazil)
										} 
	},
	{ EISTR_PRINTER_NOT_FOUND		,	{	"The external printer is not available. Check that the printer is connected to the analyzer, is turned on and has paper. If symptom continues, contact Technical Service.\n",																	// English
											"L'imprimante externe n'est pas disponible. Vérifier que l'imprimante est bien raccordée à l'analyseur, qu'elle fonctionne et qu'elle dispose d'une réserve de papier. En cas de persistance des symptômes, contacter le service technique.\n",	// French
											"Der externe Drucker ist nicht verfügbar. Prüfen Sie die Verbindung zwischen Drucker und Analysegerät und ob der Drucker eingeschaltet ist und über Papier verfügt. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",	// German
											"La impresora externa no está disponible. Compruebe que la impresora está conectada al analizador, que está encendida y que tiene papel. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.n",				// Spanish
											"La stampante esterna non è disponibile. Verificare che la stampante sia collegata all'analizzatore, che sia accesa e che non manchi carta. Se i sintomi continuano contattare il servizio tecnico.\n",											// Italian
											"A impressora externa não está disponível. Verifique que se a impressora está ligada ao analisador, que está ligada e que tem papel. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",									// Portuguese
											"A impressora externa não está disponível. Verifique se a impressora está conectada ao analisador, se está ligada e se tem papel. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"											// Portuguese (Brazil)
										} 
	},
	{ EISTR_PROCESS_INIT			,	{	"The analyzer is unable to start a process required to operate. Contact Technical Service for assistance.\n",									// English
											"L'analyseur ne parvient pas à lancer la procédure requise pour fonctionner. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Das Analysegerät kann einen für den Betrieb erforderlichen Vorgang nicht starten. Wenden Sie sich an den technischen Service.\n",				// German
											"El analizador no puede iniciar un proceso necesario para funcionar. Póngase en contacto con el servicio técnico.\n",							// Spanish
											"L'analizzatore non è in grado di iniziare una procedura necessaria al funzionamento. Per assistenza contattare il servizio tecnico.\n",		// Italian
											"O analisador não consegue iniciar um processo necessário para funcionar. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											"O analisador não é capaz de iniciar um processo necessário para operar. Entre em contato com o Serviço Técnico para obter assistência.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_PRODUCT_CODE			,	{	"The product codes in the analyzer do not match. Contact Technical Service for more details.\n",									// English
											"Les codes produits dans l'analyseur ne correspondent pas. Pour plus d'informations, contacter le service technique.\n",			// French
											"Die Produktcodes in dem Analysegerät passen nicht zusammen. Wenden Sie sich an den technischen Service.\n",						// German
											"Los códigos de producto en el analizador no se corresponden. Póngase en contacto con el servicio técnico para más información.\n",	// Spanish
											"I codici prodotto nell'analizzatore non combaciano. Per ulteriore dettagli contattare il servizio tecnico.\n",						// Italian
											"Os códigos de produto no analisador não coincidem. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",		// Portuguese
											"Os códigos de produto no analisador não correspondem. Entre em contato com o Serviço Técnico para obter mais detalhes.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_REAGENT_DISTRIBUTION	,	{	"Reagent beads have not dissolved as expected. Rerun sample with a new disc.\n",											// English
											"Les billes de réactif ne se sont pas dissoutes comme prévu. Réexécuter l'échantillon à l'aide d'un nouveau disque.\n",		// French
											"Reagenz-Beads wurden nicht erwartungsgemäß aufgelöst. Wiederholen Sie die Probe mit einer neuen Scheibe.\n",				// German
											"Las cuentas de reactivo no se han disuelto como se esperaba. Vuelva a utilizar la muestra con un nuevo disco.\n",			// Spanish
											"I granuli di reagente non si sono dissolti come previsto. Riavviare il campione con un nuovo disco.\n",					// Italian
											"As esferas de reagente não se dissolveram conforme esperado. Executar novamente a amostra utilizando um disco novo.\n",	// Portuguese
											"As esferas de reagente não se dissolveram conforme esperado. Execute novamente a amostra com um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_RESULT_CORRUPTION		,	{	"The saved results are corrupt.  All the previous results have been lost.  A new set of saved results has been started.  If symptom continues, contact Technical Service.\n"																// English
											"Les résultats enregistrés sont corrompus.  Tous les résultats antérieurs ont été perdus.  Un nouvel ensemble de résultats enregistrés a été initié.  Si les symptômes persistent, contacter le service technique.\n",						// French
											"Die gespeicherten Ergebnisse sind fehlerhaft. Alle bisherigen Ergebnisse sind verloren gegangen. Es wurde ein neuer Satz gespeicherter Ergebnisse gestartet. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",	// German
											"Los resultados guardados están dañados.  Se han perdido todos los resultados anteriores.  Se ha iniciado una nueva serie de resultados guardados.  Si los síntomas continúan, comuníquese con el servicio técnico.\n",						// Spanish
											"I risultati salvati sono danneggiati.  Tutti i risultati precedenti sono andati perduti.  È stato iniziato un nuovo insieme di risultati.  Se il sintomo persiste, contattare il servizio tecnico.\n",										// Italian
											"Os resultados guardados estão danificados.  Todos os resultados anteriores foram perdidos.  Foi iniciado um novo conjunto de resultados guardados.  Se o problema persistir, contacte o serviço de Assistência Técnica.\n",				// Portuguese
											"Os resultados salvos estão corrompidos.  Todos os resultados anteriores foram perdidos.  Um novo conjunto de resultados salvos foi iniciado.  Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"							// Portuguese (Brazil)
										}
	},
	{ EISTR_ROTOR_DETECTED			,	{	"A disc may be in the analyzer and may prevent the analyzer from starting normally. Press Continue to remove the disc.\n",										// English
											"Il peut y avoir dans l'analyseur un disque susceptible de l'empêcher de démarrer normalement. Appuyer sur Continuer pour retirer le disque.\n",				// French
											"Eine Scheibe kann sich in dem Analysegerät befinden und den Startvorgang des Analysegeräts beeinträchtigen. Zur Entfernung der Scheibe drücken Sie Weiter.\n",	// German
											"Puede que haya un disco en el analizador, lo que impide que el analizador se inicie correctamente. Pulse en Continuar para retirar el disco.\n",				// Spanish
											"Nell'analizzatore potrebbe essere rimasto un disco che impedisce un'attivazione normale dell'analizzatore. Premere Continua per rimuovere il disco.\n",		// Italian
											"Pode estar um disco no analisador, o que pode impedir que o analisador inicie corretamente. Prima Continuar para remover o disco.\n",							// Portuguese
											"Um disco pode estar no analisador e impedir que o analisador inicie normalmente. Pressione Continuar para remover o disco.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_RQC						,	{	"The Reagent QC beads recovered below the limit. The disc may not have been stored correctly. Rerun with a new disc.\n",												// English
											"La récupération des billes de réactif QC est inférieure à la limite. Il se peut que le disque n'ait pas été rangé correctement. Réexécuter avec un nouveau disque.\n",	// French
											"Die Reagenz-QC-Beads liegen unter dem Grenzwert. Die Scheibe wurde möglicherweise nicht ordnungsgemäß gelagert. Mit einer neuen Scheibe wiederholen.\n",				// German
											"Cuentas QC de reactivo recuperadas por debajo del límite. Puede que el disco no se haya guardado correctamente. Debe usarse con un nuevo disco.\n",					// Spanish
											"I granuli QC di reagente si sono ristabiliti sotto al limite. Il disco potrebbe non essere stato conservato correttamente. Riavviare con un nuovo disco.\n",			// Italian
											"As esferas de reagente de CQ foram recuperadas abaixo do limite. O disco pode não ter sido guardado corretamente. Executar novamente utilizando um disco novo.\n",		// Portuguese
											"As esferas de Reagente de QC se recuperaram abaixo do limite. O disco pode não ter sido armazenado corretamente. Execute novamente com um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_BLANK			,	{	"The sample blank bead appears to be turbid. This can be due to sample quality. Confirm sample integrity and rerun with a new disc.\n",															// English
											"La bille vierge de l'échantillon semble trouble, ce qui peut venir de la qualité de l'échantillon. Vérifier l'intégrité de l'échantillon et réexécuter avec un nouveau disque.\n",				// French
											"Die Leer-Beadprobe ist trübe. Dies kann auf die Probenqualität zurückgeführt werden. Bestätigen Sie die Unversehrtheit der Probe und wiederholen Sie den Vorgang mit einer neuen Scheibe.\n",	// German
											"La cuenta virgen de la muestra parece estar turbia. Esto puede deberse a la calidad de la muestra. Confirme la integridad de la muestra y vuelva a utilizar el nuevo disco.\n",				// Spanish
											"I granuli campione vuoti sembrano torbidi. Ciò può essere causato dalla qualità del campione. Confermare l'integrità del campione e riavviare con un nuovo disco.\n",							// Italian
											"A esfera de amostra de branco parece estar turva. Isto pode ser devido à qualidade da amostra. Confirme a integridade da amostra e execute novamente utilizando um disco novo.\n",				// Portuguese
											"A esfera em branco da amostra parece estar turva. Isso pode ser decorrente da qualidade da amostra. Confirme a integridade da amostra e execute novamente com um novo disco.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_DISTRIBUTION		,	{	"Sample has not reached the chemistries. Rerun with a new disc.\n",										// English
											"L'échantillon n'est pas parvenu jusqu'aux produits chimiques. Réexécuter avec un nouveau disque.\n",	// French
											"Die Probe hat nicht die Chemie erreicht. Mit einer neuen Scheibe wiederholen.\n",						// German
											"La muestra no ha alcanzado los compuestos químicos. Debe usarse con un nuevo disco.\n",				// Spanish
											"Il campione non ha raggiunto le sostanze chimiche. Riavviare con un nuovo disco.\n",					// Italian
											"A amostra não atingiu as substâncias químicas. Executar novamente utilizando um disco novo.\n",		// Portuguese
											"A amostra não atingiu as substâncias químicas. Execute novamente com um novo disco.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_MIX				,	{	"The sample has not mixed with reagent materials sufficiently. Rerun with a new disc.\n",									// English
											"L'échantillon ne s'est pas suffisamment mélangé avec les matières de réactifs. Réexécuter avec un nouveau disque.\n",		// French
											"Die Probe hat sich nicht ausreichend mit Reagenzmaterialien vermischt. Mit einer neuen Scheibe wiederholen.\n",			// German
											"La muestra no se ha mezclado suficientemente con los materiales reactivos. Debe usarse con un nuevo disco.\n",				// Spanish
											"Il campione non si è mescolato a sufficienza con le sostanze reagenti. Riavviare con un nuovo disco.\n",					// Italian
											"A amostra não se misturou suficientemente com os materiais dos reagentes. Executar novamente utilizando um disco novo.\n",	// Portuguese
											"A amostra não foi misturada com os materiais reagentes suficientemente. Execute novamente com um novo disco.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_QUALITY			,	{	"The sample may have one or more physical interferents. Confirm the sample integrity and rerun disc.\n",											// English
											"Il se peut que l'échantillon présente un ou plusieurs éléments interférents. Vérifier l'intégrité de l'échantillon et réexécuter le disque.\n",	// French
											"Die Probe hat möglicherweise eine oder mehrere physische störende Bestandteile. Unversehrtheit der Probe bestätigen und Scheibe neu starten.\n",	// German
											"Puede que la muestra tenga una o varias interferencias físicas. Confirme la integridad de la muestra y vuelva a usar el disco.\n",					// Spanish
											"Il campione potrebbe avere uno o più fattori di interferenza fisica. Confermare l'integrità del campione e riavviare il disco.\n",					// Italian
											"A amostra pode ter uma ou várias interferências físicas. Confirme a integridade da amostra e execute novamente o disco.\n",						// Portuguese
											"A amostra pode ter um ou mais interferentes físicos. Confirme a integridade da amostra e execute novamente o disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_SATURATION				,	{	"The photometer in the analyzer has detected a brighter than normal flash. If symptom continues, contact Technical Service.\n",												// English
											"Le photomètre de l'analyseur a détecté un clignotement plus lumineux que la normale. En cas de persistance des symptômes, contacter le service technique.\n",				// French
											"Das Fotometer in dem Analysegerät hat ein helleres Blinkzeichen als normal festgestellt. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",		// German
											"El fotómetro del analizador ha detectado un flash más brillante de lo normal. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"Il fotometro nell'analizzatore ha individuato un flash più luminoso del normale. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"O fotómetro no analisador detetou um flash mais brilhante do que o normal. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",							// Portuguese
											"O fotômetro no analisador detectou um flash mais brilhante que o normal. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_SOFTWARE_UPDATE			,	{	"The disc inserted requires a software update to operate correctly. Contact Technical Service for further assistance.\n",														// English
											"Le disque inséré nécessite une mise à jour de logiciel pour pouvoir fonctionner correctement. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											"Die eingesetzte Scheibe erfordert ein Software-Update für einen ordnungsgemäßen Betrieb. Wenden Sie sich an den technischen Service.\n",										// German
											"El disco insertado necesita la actualización del software para operar correctamente. Póngase en contacto con el servicio técnico.\n",											// Spanish
											"Il disco inserito richiede un aggiornamento del software per funzionare correttamente. Per ulteriore assistenza contattare il servizio tecnico.\n",							// Italian
											"O disco inserido necessita de uma atualização do software para funcionar adequadamente. Contacte o Serviço de Assistência Técnica para obter assistência.\n",					// Portuguese
											"O disco inserido requer uma atualização de software para funcionar corretamente. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SPINDLE_MOTOR_STUCK		,	{	"Spindle motor not turning properly. Contact Technical Service for further assistance.\n",											// English
											"Le moteur de l'axe ne tourne pas correctement. Pour obtenir une assistance complémentaire, contacter le service technique.\n",		// French
											"Der Spindelmotor dreht sich nicht ordnungsgemäß. Wenden Sie sich an den technischen Service.\n",									// German
											"El motor del eje no está girando correctamente. Póngase en contacto con el servicio técnico.\n",									// Spanish
											"L'asse motore non gira correttamente. Per ulteriore assistenza contattare il servizio tecnico\n",									// Italian
											"O motor de eixo não está a rodar adequadamente. Contacte o Serviço de Assistência Técnica para obter assistência.\n",				// Portuguese
											"O motor do fuso não está girando corretamente. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"			// Portuguese (Brazil)
										}
	},
	{ EISTR_STARTING_ABSORBANCE		,	{	"Starting absorbance of the sample blank bead is outside the limit, possibly due to an exogenous substance or distribution error. Rerun with new disc.\n",																		// English
											"L'absorbance de départ de la bille vierge de l'échantillon n'entre pas dans les limites prescrites, éventuellement du fait d'une substance exogène ou d'une erreur de distribution. Réexécuter à l'aide du nouveau disque.\n",	// French
											"Die Startabsorbierung der Leer-Beadprobe befindet sich möglicherweise aufgrund einer exogenen Substanz oder eines Verteilungsfehlers außerhalb des Grenzwerts. Mit einer neuen Scheibe wiederholen.\n",						// German
											"La absorbancia de inicio de la cuenta virgen de muestra está fuera del límite, probablemente debido a una sustancia exógena o a un error de distribución. Utilice un nuevo disco.\n",											// Spanish
											"L'assorbenza iniziale dei granuli campione vuoti non rientra nei limiti, forse a causa di una sostanza esogena o a un errore di distribuzione. Riavviare con un nuovo disco.\n",												// Italian
											"A absorvância inicial da esfera de amostra de branco está fora do limite, possivelmente devido a uma sustância exógena ou erro de distribuição. Executar novamente utilizando um disco novo.\n",								// Portuguese
											"A absorção inicial da esfera em branco da amostra está fora do limite, possivelmente devido a uma substância exógena ou um erro de distribuição. Execute novamente com um novo disco.\n"										// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_SENSOR				,	{	"The disc temperature sensor is not operating. Contact Technical Service for further assistance.\n",										// English
											"Le détecteur de température du disque ne fonctionne pas. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											"Der Temperatursensor der Scheibe ist außer Betrieb. Wenden Sie sich an den technischen Service.\n",										// German
											"El sensor de temperatura del disco no está funcionando. Póngase en contacto con el servicio técnico.\n",									// Spanish
											"Il sensore di temperatura del disco non è in funzione. Per ulteriore assistenza contattare il servizio tecnico.\n",						// Italian
											"O sensor de temperatura do disco não está a funcionar. Contacte o Serviço de Assistência Técnica para obter assistência.\n",				// Portuguese
											"O sensor de temperatura do disco não está funcionando. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_VARIATION			,	{	"The temperature deviated more than one degree during the run. The accuracy of certain chemistries may be affected. Rerun with new disc.\n",													// English
											"La température a présenté un écart de plus d'un degré au cours de l'exécution. La précision de certains produits chimiques pourra en être affectée. Réexécuter à l'aide du nouveau disque.\n",	// French
											"Der Temperatur wich um mehr als ein Grad während des Betriebs ab. Die Genauigkeit bestimmter Chemie kann beeinträchtigt worden sein. Mit einer neuen Scheibe wiederholen.\n",					// German
											"La temperatura cambió en más de un grado durante el análisis. Puede que afecte a la precisión de análisis de ciertas sustancias químicas. Utilice un nuevo disco.\n",							// Spanish
											"Durante il funzionamento la temperatura è variata di più di un grado. La precisione di alcune sostanze chimiche potrebbe esserne stata influenzata. Riavviare con un nuovo disco.\n",			// Italian
											"A alteração da temperatura foi superior a um grau durante a execução. A precisão de determinadas sustâncias químicas pode ser afetada. Executar novamente utilizando um disco novo.\n",		// Portuguese
											"A temperatura desviou mais de um grau durante a execução. A precisão de certas substâncias químicas pode estar afetada. Execute novamente com um novo disco.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMPERATURE				,	{	"The temperature of the disc either varied too much or was not in the proper range during the run. If symptom continues, contact Technical Service.\n",																			// English
											"La température du disque a soit varié dans des proportions trop importantes, soit ne s'inscrit pas dans la fourchette prescrite en cours d'exécution. En cas de persistance des symptômes, contacter le service technique.\n",	// French
											"Die Scheibentemperatur variierte zu stark oder befand sich während des Betriebs außerhalb des optimalen Bereichs. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",									// German
											"La temperatura del disco varió mucho o no estaba en el intervalo adecuado durante el análisis. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",										// Spanish
											"Durante il funzionamento la temperatura del disco ha subito variazioni eccessive oppure non era nel range adatto. Se i sintomi continuano contattare il servizio tecnico.\n",													// Italian
											"A temperatura do disco variou muito ou não esteve dentro do intervalo adequado durante a análise. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",														// Portuguese
											"A temperatura do disco variou muito ou não permaneceu no intervalo adequado durante a execução. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"																// Portuguese (Brazil)
										} 
	},
	{ EISTR_TIMEOUT					,	{	"The analyzer is not receiving required data in a timely fashion. If symptom continues, contact Technical Service.\n",										// English
											"L'analyseur ne reçoit pas les données voulues dans les délais. En cas de persistance des symptômes, contacter le service technique.\n",					// French
											"Das Analysegerät empfängt die erforderlichen Daten nicht rechtzeitig. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",			// German
											"El analizador no está recibiendo a tiempo los datos necesarios. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"L'analizzatore non riceve tempestivamente i dati necessari. Se i sintomi continuano contattare il servizio tecnico.\n",									// Italian
											"O analisador não está a receber atempadamente os dados necessários. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",				// Portuguese
											"O analisador não está recebendo os dados necessários em tempo hábil. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_TOO_HOT					,	{	"The analyzer is getting too hot and should be allowed to cool down for a while before further use.\n",						// English
											"L'analyseur présente une surchauffe. Il convient de le laisser refroidir un moment avant de continuer à l'utiliser.\n",	// French
											"Das Analysegerät wird zu heiß und sollte vor der weiteren Verwendung abkühlen.\n",											// German
											"El analizador se está sobrecalentando y se debería dejar enfriar antes de volver a usarlo.\n",								// Spanish
											"L'analizzatore si sta riscaldando eccessivamente e deve essere fatto raffreddare prima di essere usato nuovamente.\n",		// Italian
											"O analisador está aquecer muito e deve-se deixar arrefecer durante algum tempo antes de o voltar a utilizar.\n",			// Portuguese
											"O analisador está se tornando quente demais e deve ser reservado para esfriar por um tempo antes de continuar o uso.\n"	// Portuguese (Brazil)
										} 
	},
	{ EISTR_TOP_HEATER_HIGH			,	{	"The top heater plate draws too much current when turned on at full power. Contact Technical Service for assistance.\n",														// English
											"La plaque chauffante supérieure consomme trop d'électricité lorsqu'elle fonctionne à fond. Pour obtenir une assistance, contacter le service technique.\n",					// French
											"Die obere Heizplatte verbraucht bei Maximalbetrieb zu viel Strom. Wenden Sie sich an den technischen Service.\n",																// German
											"La placa del calentador superior utiliza mucha corriente cuando se enciende a toda potencia. Póngase en contacto con el servicio técnico.\n",									// Spanish
											"Il disco di riscaldamento superiore prende troppa corrente quando è acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",							// Italian
											"A placa do aquecedor superior utiliza elevada quantidade de corrente quando é ligada na potência máxima. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											"A placa superior do aquecedor consome muita corrente quando ligada na potência máxima. Entre em contato com o Serviço Técnico para obter assistência.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_TOP_HEATER_LOW			,	{	"The top heater plate did not draw enough current when turned on at full power. Contact Technical Service for assistance.\n",															// English
											"La plaque chauffante supérieure n'a pas consommé suffisamment d'électricité lorsqu'elle a fonctionné à fond. Pour obtenir une assistance, contacter le service technique.\n",			// French
											"Die obere Heizplatte verbrauchte bei Maximalbetrieb zu wenig Strom. Wenden Sie sich an den technischen Service.\n",																	// German
											"La placa del calentador superior no utilizó suficiente corriente cuando se puso a toda potencia. Póngase en contacto con el servicio técnico.\n",										// Spanish
											"Il disco di riscaldamento superiore non ha preso sufficiente corrente quando acceso alla massima potenza. Per assistenza contattare il servizio tecnico.\n",							// Italian
											"A placa superior do aquecedor não utiliza quantidade suficiente de corrente quando ligada na potência máxima. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											"A placa superior do aquecedor não atraiu corrente suficiente quando ligada na potência máxima. Entre em contato com o Serviço Técnico para obter assistência.\n"						// Portuguese (Brazil)
										} 
	},
	{ EISTR_UNAUTHORIZED_ADMIN		,	{	"The ID entered for the administrator is not in the system. Please contact technical support for more information.\n",						// English
											"L'ID saisi pour l'administrateur n'est pas sur le système. Pour plus d'informations, contacter l'assistance technique.\n",					// French
											"Die eingegebene Administrator-ID existiert nicht im System. Wenden Sie sich bitte an den technischen Service für weitere Information.\n",	// German
											"Esta Id. administrador no está en el sistema. Póngase en contacto con el servicio técnico. \n",											// Spanish
											"L'ID immesso per l'amministratore non è presente nel sistema. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",		// Italian
											"A ID introduzida para o administrador não está no sistema. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",		// Portuguese
											"O ID inserido para o administrador não está no sistema. Entre em contato com o suporte técnico para obter mais informações.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_UNAUTHORIZED_OP			,	{	"The ID entered for the operator is not in the system. Please contact technical support for more information.\n",						// English
											"L'ID saisi pour l'opérateur n'est pas sur le système. Pour plus d'informations, contacter l'assistance technique.\n",					// French
											"Die eingegebene Bediener-ID existiert nicht im System. Wenden Sie sich bitte an den technischen Service für weitere Information.\n",	// German
											"Esta Id. operario no está en el sistema. Póngase en contacto con el servicio técnico.\n",												// Spanish
											"L'ID immesso per l'operatore non è presente nel sistema. Pregasi contattare l'assistenza tecnica per maggiori informazioni.\n",		// Italian
											"A ID introduzida para o operador não está no sistema. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",		// Portuguese
											"O ID inserido para o operador não está no sistema. Entre em contato com o suporte técnico para obter mais informações.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_WRONG_ROTOR				,	{	"The disc inserted is not intended for this device. If symptom continues, contact Technical Service.\n",										// English
											"Le disque inséré n'est pas prévu pour cet appareil. En cas de persistance des symptômes, contacter le service technique.\n",					// French
											"Die eingesetzte Scheibe ist für dieses Gerät nicht geeignet. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",		// German
											"El disco insertado no es apto para este aparato. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"Il disco inserito non è adatto a questo strumento. Se i sintomi continuano contattare il servizio tecnico.\n",									// Italian
											"O disco inserido não é adequado para este dispositivo. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",					// Portuguese
											"O disco inserido não é destinado a este dispositivo. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"						// Portuguese (Brazil)
										} 
	},


// Printer									ENGLISH,						FRENCH,							GERMAN,							SPANISH,							ITALIAN, 						PORTUGUESE,							PORTUGUESE (BRAZIL)
	{ PSTR_PRINTER_TEST				,	{	"Printer Test",					"Test imprimante",				"Druckertest",					"Prueba de impresora",				"Prova stampante",				"Teste da impressora",				"Teste da impressora"	}	},

// Results (display and print)				ENGLISH,						FRENCH,							GERMAN,							SPANISH,							ITALIAN, 						PORTUGUESE,							PORTUGUESE (BRAZIL)
	{ RSTR_ACCEPTABLE_MINIMUM		,	{	"Acceptable Minimum:",			"Minimum Acceptable :",			"Verwertbares min.:",			"Minimo Aceptable:",				"Minimo Accettabile:",			"Mínimo aceitável:",				"Mínimo aceitável:"	}	},
	{ RSTR_AGE						,	{	"Age:",							"âge :",						"Alter",						"Edad:",							"Età:",							"Idade:",							"Idade:"	}	},
	{ RSTR_CHEMISTRY_QC				,	{	"Chemistry QC:",				"Chimie CQ :",					"Chemie-QK:",					"CC Quimico:",						"QC Chimica:",					"CQ Químico:",						"QC de química:"	}	},
	{ RSTR_CONFIRM_LOW_RECOVERIES	,	{	"! CONFIRM LOW RECOVERIES !",	"! CONFIRMER FAIBLE RÉCUP !",	"! NDR. WIEDERHERST. BESTÄT.!",	"! CONFIRME BAJA RECUPERACIÒN !",	"! CONFERMA POCHI RECUPERI !",	"! CONFIRME BAIXA RECUPERAÇÃO !",	"! CONFIRM. RECUPER. BAIXAS !"	}	},
	{ RSTR_CONTROL_ID				,	{	"Control ID:",					"ID contr.:",					"Kontr.-ID",					"Id. ctrol:",						"ID contrl:",					"ID do controlo:",					"ID de controle:"	}	},
	{ RSTR_CONTROL_RENEWAL			,	{	"Control Renewal Required",		"Renouv. contr. nécess.",		"Kontrollerneuerung benötigt",	"Renovar control",					"Necess. rinnovo contr.",		"Necessário renovar o controlo",	"Renov. do controle necessária"	}	},
	{ RSTR_DEMONSTRATION_SOFTWARE	,	{	"Demonstration Software",		"Logiciel de démonstration",	"Demonstrationssoftware",		"Demo",								"Software di dimostrazione",	"Software de demonstração",			"Software de demonstração"	}	},
	{ RSTR_ERROR					,	{	"ERROR",						"ERREUR",						"FEHLER",						"ERROR",							"ERRORE",						"ERRO",								"ERRO"	}	},
	{ RSTR_GENDER					,	{	"Gender:",						"Sexe :",						"Geschlecht:",					"Sexo:",							"Sesso:",						"Sexo:",							"Gênero:"	}	},
	{ RSTR_INSTRUMENT_QC			,	{	"Instrument QC:",				"Instrument CQ:",				"Instrument QK:",				"Control calidad instr:",			"Strum. CQ:",					"CQ do instrumento:",				"QC de instrumento:"	}	},
	{ RSTR_NC						,	{	"N/C",							"N/C",							"N/B",							"N/C",								"N/C",							"N/B",								"N/C"	}	},
	{ RSTR_NEG						,	{	"NEG",							"NEG",							"NEG",							"NEG",								"NEG",							"NEG",								"NEG"	}	},
	{ RSTR_OPERATOR_ID				,	{	"Operator ID:",					"ID opérateur :",				"Bediener-ID:",					"Id. operario:",					"ID operatore:",				"ID do operador:",					"ID do operador:"	}	},
	{ RSTR_OPERATOR_RENEWAL			,	{	"Operator Renewal Required",	"Renouv. opér. nécess.",		"Bedienererneuerung benötigt",	"Renovar operario",					"Necess. rinnovo operat.",		"Necessário renovar o operador",	"Renov. de operador necessária"	}	},
	{ RSTR_PATIENT_ID				,	{	"Patient ID:",					"ID patient :",					"Patienten-ID:",				"Id. paciente:",					"ID paziente:",					"ID do doente:",					"ID do paciente:"	}	},
	{ RSTR_POS						,	{	"POS",							"POS",							"POS",							"POS",								"POS",							"POS",								"POS"	}	},
	{ RSTR_QC_OK					,	{	"QC     OK",					"QC     OK",					"QUALITAETSKONTROLLE    OK",	"CONTROL DE CALIDAD     OK",		"CONTROLLO DI QUALITA   OK",	"CQ     OK",						"QC     OK"	}	},
	{ RSTR_RACE						,	{	"Race:",						"Race :",						"Rasse:",						"Raza:",							"Razza:",						"Raça:",							"Raça:"	}	},
	{ RSTR_RANGE					,	{	"Range:",						"Registre :",					"Bereich:",						"Rango:",							"Banda",						"Intervalo:",						"Intervalo:"	}	},
	{ RSTR_RANGE_LABEL				,	{	"Range Label:",					"Étiq.fourch:",					"Bereichskz.:",					"Intervalo:",						"Etich.range:",					"Rót. Inter.:",						"Rót. inter.:"	}	},
	{ RSTR_RESEARCH_SOFTWARE		,	{	"Research Software",			"Logiciel recherche",			"Research Software",			"Software investig.",				"Software ricerca",				"Software de pesquisa",				"Software de pesquisa"	}	},
	{ RSTR_ROTOR_LOT_NUM			,	{	"Disc Lot Number:",				"N° de lot disque :",			"Scheiben-Losnr.:",				"No. de lote del disco:",			"Numero lotto disco:",			"Número lote disco:",				"Núm. de lote do disco:"	}	},
	{ RSTR_SAMPLE_TYPE				,	{	"Sample Type:",					"Type éch. :",					"Probentyp:",					"Muestra:",							"Tipo camp.:",					"Tipo amost.:",						"Tipo amostr:"	}	},
	{ RSTR_SERIAL_NUM				,	{	"Serial Number:",				"N° de série :",				"Seriennr.:",					"Número de serie:",					"Numero di serie:",				"Número de série:",					"Número de série:"	}	},
	{ RSTR_UNAUTHORIZED_OPERATOR	,	{	"Unauthorized Operator",		"Opér. non autorisé",			"Unautorisierter Bediener",		"Operario no autorizado",			"Operat. non autorizz.",		"Operador não autorizado",			"Operador não autorizado"	}	},
	{ RSTR_UNKNOWN					,	{	"Unknown",						"Inconnu",						"Unbekannt",					"Desconocido",						"Sconosciuto",					"Desconhecido",						"Desconhecido"	}	},

// Sample ID labels for results				ENGLISH,			FRENCH,				GERMAN,				SPANISH,			ITALIAN, 			PORTUGUESE,				PORTUGUESE (BRAZIL)
	{ RSTR_ADMISSION_ID				,	{	"Admission ID:",	"ID admission :",	"Zugangs-ID:",		"Id. admisión:",	"ID ammissione:",	"ID de Admissão:",		"ID de admissão:"	}	},
	{ RSTR_ALTERNATE_ID				,	{	"Alternate ID:",	"Autre ID :",		"ID wechseln:",		"Id. suplente:",	"ID alternativo:",	"ID suplente:",			"ID alternativo:"	}	},
	{ RSTR_DOCTOR_ID				,	{	"Doctor ID:",		"ID médecin :",		"Arzt-ID:",			"Id. del médico:",	"ID medico:",		"ID do médico:",		"ID do médico:"	}	},
	{ RSTR_DOB						,	{	"DOB:",				"DOB :",			"GD:",				"F. nac.:",			"DN:",				"DOB:",					"DN:"	}	},
	{ RSTR_LOCATION					,	{	"Location:",		"Emplcmt:",			"Ort:",				"Ubicac.:",			"Sede:",			"Localização:",			"Local:"	}	},
	{ RSTR_PHONE_NUM				,	{	"Phone Number:",	"N° tél :",			"Telefonnummer:",	"Teléfono:",		"Num. telefono:",	"Núm. telefone:",		"Núm. telefone:"	}	},
	{ RSTR_SAMPLE_ID				,	{	"Sample ID:",		"ID éch. :",		"Proben-ID:",		"Id. de muestra:",	"ID campione:",		"ID da amostra:",		"ID da amostra:"	}	},
	
// Age labels for results					ENGLISH,	FRENCH,		GERMAN,		SPANISH,	ITALIAN, 		PORTUGUESE,		PORTUGUESE (BRAZIL)
	{ RSTR_DAY						,	{	"Day",		"Jour",		"Tag",		"Día",		"Giorno",		"Dia",			"Dia"	}	},
	{ RSTR_DAYS						,	{	"Days",		"Jours",	"Tage",		"Días",		"Giorni",		"Dias",			"Dias"	}	},
	{ RSTR_WEEK						,	{	"Week",		"Semaine",	"Woche",	"Semana",	"Settimana",	"Semana",		"Semana"	}	},
	{ RSTR_WEEKS					,	{	"Weeks",	"Semaines",	"Wochen",	"Semanas",	"Settimane",	"Semanas",		"Semanas"	}	},
	{ RSTR_MONTH					,	{	"Month",	"Mois",		"Monat",	"Mes",		"Mese",			"Mês",			"Mês"	}	},
	{ RSTR_MONTHS					,	{	"Months",	"Mois",		"Monate",	"Meses",	"Mesi",			"Meses",		"Meses"	}	},
	{ RSTR_YEAR						,	{	"Year",		"Année",	"Jahr",		"Año",		"Anno",			"Ano",			"Ano"	}	},
	{ RSTR_YEARS					,	{	"Years",	"Années",	"Jahre",	"Años",		"Anni",			"Anos",			"Anos"	}	},

// Genders									ENGLISH,	FRENCH,		GERMAN,			SPANISH,		ITALIAN, 	   	PORTUGUESE,	   	PORTUGUESE (BRAZIL)
	{ GSTR_FEMALE					,	{	"Female",	"Féminin",	"Weiblich",		"Mujer",		"Uomo",			"Feminino",		"Sexo feminino"	}	},
	{ GSTR_MALE						,	{	"Male",		"Masculin",	"Männlich",		"Hombre",		"Donna",		"Masculino",	"Sexo masculino"	}	},
	{ GSTR_NEUTERED					,	{	"Neutered",	"Châtré",	"Kastriert",	"Castrado",		"Castrato",		"Castrado",		"Castrado"	}	},
	{ GSTR_SPAYED					,	{	"Spayed",	"Châtrée",	"Sterilisiert",	"Castrada",		"Sterilizzato",	"Esterilizado",	"Esterilizado"	}	},
	{ GSTR_UNKNOWN					,	{	"Unknown",	"Inconnu",	"Unbekannt",	"Desconocido",	"Sconosciuto",	"Desconhecido",	"Desconhecido"	}	},
	
// Race labels for results					ENGLISH,					FRENCH,						GERMAN,						SPANISH,						ITALIAN, 				PORTUGUESE,					PORTUGUESE (BRAZIL)
//	  RSTR_UNKNOWN - defined above
	{ RSTR_WHITE					,	{	"White",					"Blanche",					"Weiß",						"Blanco",					"Bianco",					"Branco",					"Branco"	}	},
	{ RSTR_BLACK					,	{	"Black",					"Noire",					"Schwarz",					"Negro",					"Nero",						"Negro",					"Preto"	}	},
	{ RSTR_ASIAN_PACIFIC_ISLANDER	,	{	"Asian/Pacific Islander",	"Asiat./orig. îles Pac.",	"Asiatisch/Pazifisch",		"Asiático/Isl. Pacífico",	"Isolano Asia/Pacifico",	"Ilhas Asiát./ Pacífico",	"Asiático/ilhas Pacíf."	}	},
	{ RSTR_NATIVE_AMERICAN			,	{	"Native American",			"Amérindienne",				"Indianische Abstammung",	"Americano nativo",			"Nativo americano",			"Americano nativo",			"Americano nativo"	}	},
	{ RSTR_HISPANIC					,	{	"Hispanic",					"Hispanique",				"Hispanische Abstammung",	"Hispano",					"Ispanico",					"Hispânico",				"Hispânico"	}	},

// Species									ENGLISH,			FRENCH,				GERMAN,				SPANISH,	 		ITALIAN, 			PORTUGUESE,	 		PORTUGUESE (BRAZIL)
	{ SSTR_CONTROL					,	{	"Control",			"Contrôle",			"Kontrolle",		"Control",	 		"Controllo",		"Controlo",	 		"Controle"	}	},
	{ SSTR_PATIENT					,	{	"Patient",			"Patient",			"Patient",			"Paciente",		 	"Paziente",			"Doente",	 		"Paciente"	}	},
	{ SSTR_OTHER					,	{	"Patient",			"Patient",			"Patient",			"Paciente",		 	"Paziente",			"Doente",	 		"Paciente"	}	},
	{ SSTR_SPECIES					,	{	"Special",			"Spécial",			"Spezial",			"Especial",		 	"Speciale",			"Especial",	 		"Especial"	}	},
	
	{ SSTR_CANINES					,	{	"Category 1", 		"Catégorie 1", 		"Kategorie 1", 		"Categoría 1",		"Categoria 1",		"Categoria 1",		"Categoria 1"	}	},
	{ SSTR_FELINES					,	{	"Category 2", 		"Catégorie 2", 		"Kategorie 2", 		"Categoría 2",		"Categoria 2",		"Categoria 2",		"Categoria 2"	}	},
	{ SSTR_EQUINES					,	{	"Category 3", 		"Catégorie 3", 		"Kategorie 3", 		"Categoría 3",		"Categoria 3",		"Categoria 3",		"Categoria 3"	}	},
	{ SSTR_BOVINES					,	{	"Category 4", 		"Catégorie 4", 		"Kategorie 4", 		"Categoría 4",		"Categoria 4",		"Categoria 4",		"Categoria 4"	}	},
	{ SSTR_AVIANS					,	{	"Category 5", 		"Catégorie 5", 		"Kategorie 5", 		"Categoría 5",		"Categoria 5",		"Categoria 5",		"Categoria 5"	}	},
	{ SSTR_REPTILES_AMPHIBIANS		,	{	"Category 6", 		"Catégorie 6", 		"Kategorie 6", 		"Categoría 6",		"Categoria 6",		"Categoria 6",		"Categoria 6"	}	},
	{ SSTR_SMALL_MAMMALS			,	{	"Category 7", 		"Catégorie 7", 		"Kategorie 7", 		"Categoría 7",		"Categoria 7",		"Categoria 7",		"Categoria 7"	}	},
	{ SSTR_LARGE_MAMMALS			,	{	"Category 8", 		"Catégorie 8", 		"Kategorie 8", 		"Categoría 8",		"Categoria 8",		"Categoria 8",		"Categoria 8"	}	},

	// Canines
	{ SSTR_DOG						,	{	"Demographic 1",	"Elmts démogr. 1", 	"Demographie 1", 	"Demografía 1",		"Demografia 1",		"Demografia 1",		"Dados demogr. 1"	}	},
	{ SSTR_COYOTE					,	{	"Demographic 2",	"Elmts démogr. 2", 	"Demographie 2", 	"Demografía 2",		"Demografia 2",		"Demografia 2",		"Dados demogr. 2"	}	},
	{ SSTR_DINGO					,	{	"Demographic 3", 	"Elmts démogr. 3", 	"Demographie 3", 	"Demografía 3",		"Demografia 3",		"Demografia 3",		"Dados demogr. 3"	}	},
	{ SSTR_FOX						,	{	"Demographic 4", 	"Elmts démogr. 4", 	"Demographie 4", 	"Demografía 4",		"Demografia 4",		"Demografia 4",		"Dados demogr. 4"	}	},
	{ SSTR_JACKAL					,	{	"Demographic 5", 	"Elmts démogr. 5", 	"Demographie 5", 	"Demografía 5",		"Demografia 5",		"Demografia 5",		"Dados demogr. 5"	}	},
	{ SSTR_WOLF						,	{	"Demographic 6", 	"Elmts démogr. 6", 	"Demographie 6", 	"Demografía 6",		"Demografia 6",		"Demografia 6",		"Dados demogr. 6"	}	},

	// Felines
	{ SSTR_CAT						,	{	"Demographic 7", 	"Elmts démogr. 7", 	"Demographie 7"		"Demografía 7", 	"Demografia 7",		"Demografia 7",		"Dados demogr. 7"	}	},
	{ SSTR_BOBCAT					,	{	"Demographic 8", 	"Elmts démogr. 8", 	"Demographie 8",	"Demografía 8", 	"Demografia 8",		"Demografia 8",		"Dados demogr. 8"	}	},
	{ SSTR_CHEETAH					,	{	"Demographic 9", 	"Elmts démogr. 9", 	"Demographie 9",	"Demografía 9", 	"Demografia 9",		"Demografia 9",		"Dados demogr. 9"	}	},
	{ SSTR_JAGUAR					,	{	"Demographic 10", 	"Elmts démogr. 10", "Demographie 10",	"Demografía 10",	"Demografia 10",	"Demografia 10",	"Dados demogr. 10"	}	},
	{ SSTR_LEOPARD					,	{	"Demographic 11", 	"Elmts démogr. 11", "Demographie 11",	"Demografía 11",	"Demografia 11",	"Demografia 11",	"Dados demogr. 11"	}	},
	{ SSTR_LION						,	{	"Demographic 12", 	"Elmts démogr. 12", "Demographie 12",	"Demografía 12",	"Demografia 12",	"Demografia 12",	"Dados demogr. 12"	}	},
	{ SSTR_LYNX						,	{	"Demographic 13", 	"Elmts démogr. 13", "Demographie 13",	"Demografía 13",	"Demografia 13",	"Demografia 13",	"Dados demogr. 13"	}	},
	{ SSTR_PANTHER					,	{	"Demographic 14", 	"Elmts démogr. 14", "Demographie 14",	"Demografía 14",	"Demografia 14",	"Demografia 14",	"Dados demogr. 14"	}	},
	{ SSTR_PUMA						,	{	"Demographic 15", 	"Elmts démogr. 15", "Demographie 15",	"Demografía 15",	"Demografia 15",	"Demografia 15",	"Dados demogr. 15"	}	},
	{ SSTR_TIGER					,	{	"Demographic 16", 	"Elmts démogr. 16", "Demographie 16",	"Demografía 16",	"Demografia 16",	"Demografia 16",	"Dados demogr. 16"	}	},
	
	// Equines
	{ SSTR_HORSE					,	{	"Demographic 17", 	"Elmts démogr. 17", "Demographie 17",	"Demografía 17",	"Demografia 17",	"Demografia 17",	"Dados demogr. 17"	}	},
	{ SSTR_DONKEY					,	{	"Demographic 18", 	"Elmts démogr. 18", "Demographie 18",	"Demografía 18",	"Demografia 18",	"Demografia 18",	"Dados demogr. 18"	}	},
	{ SSTR_PONY						,	{	"Demographic 19", 	"Elmts démogr. 19", "Demographie 19",	"Demografía 19",	"Demografia 19",	"Demografia 19",	"Dados demogr. 19"	}	},
	{ SSTR_ZEBRA					,	{	"Demographic 20", 	"Elmts démogr. 20", "Demographie 20",	"Demografía 20",	"Demografia 20",	"Demografia 20",	"Dados demogr. 20"	}	},
	
	// Bovines
	{ SSTR_COW						,	{	"Demographic 21", 	"Elmts démogr. 21", "Demographie 21",	"Demografía 21",	"Demografia 21",	"Demografia 21",	"Dados demogr. 21"	}	},
	{ SSTR_WATER_BUFFALO			,	{	"Demographic 22", 	"Elmts démogr. 22", "Demographie 22",	"Demografía 22",	"Demografia 22",	"Demografia 22",	"Dados demogr. 22"	}	},
	{ SSTR_STEER					,	{	"Demographic 23", 	"Elmts démogr. 23", "Demographie 23",	"Demografía 23",	"Demografia 23",	"Demografia 23",	"Dados demogr. 23"	}	},
	{ SSTR_OX						,	{	"Demographic 24", 	"Elmts démogr. 24", "Demographie 24",	"Demografía 24",	"Demografia 24",	"Demografia 24",	"Dados demogr. 24"	}	},
	{ SSTR_BULL						,	{	"Demographic 25", 	"Elmts démogr. 25", "Demographie 25",	"Demografía 25",	"Demografia 25",	"Demografia 25",	"Dados demogr. 25"	}	},
	{ SSTR_BUFFALO					,	{	"Demographic 26", 	"Elmts démogr. 26", "Demographie 26",	"Demografía 26",	"Demografia 26",	"Demografia 26",	"Dados demogr. 26"	}	},
	{ SSTR_BISON					,	{	"Demographic 27", 	"Elmts démogr. 27", "Demographie 27",	"Demografía 27",	"Demografia 27",	"Demografia 27",	"Dados demogr. 27"	}	},
	
	// Avians
	{ SSTR_BIRD						,	{	"Demographic 28", 	"Elmts démogr. 28", "Demographie 28",	"Demografía 28",	"Demografia 28",	"Demografia 28",	"Dados demogr. 28"	}	},
	{ SSTR_AFRICAN_GRAY				,	{	"Demographic 29", 	"Elmts démogr. 29", "Demographie 29",	"Demografía 29",	"Demografia 29",	"Demografia 29",	"Dados demogr. 29"	}	},
	{ SSTR_AMAZON					,	{	"Demographic 30", 	"Elmts démogr. 30", "Demographie 30",	"Demografía 30",	"Demografia 30",	"Demografia 30",	"Dados demogr. 30"	}	},
	{ SSTR_CHICKEN					,	{	"Demographic 31", 	"Elmts démogr. 31", "Demographie 31",	"Demografía 31",	"Demografia 31",	"Demografia 31",	"Dados demogr. 31"	}	},
	{ SSTR_COCKATIEL				,	{	"Demographic 32", 	"Elmts démogr. 32", "Demographie 32",	"Demografía 32",	"Demografia 32",	"Demografia 32",	"Dados demogr. 32"	}	},
	{ SSTR_COCKATOO					,	{	"Demographic 33", 	"Elmts démogr. 33", "Demographie 33",	"Demografía 33",	"Demografia 33",	"Demografia 33",	"Dados demogr. 33"	}	},
	{ SSTR_CONURE					,	{	"Demographic 34", 	"Elmts démogr. 34", "Demographie 34",	"Demografía 34",	"Demografia 34",	"Demografia 34",	"Dados demogr. 34"	}	},
	{ SSTR_DOVE						,	{	"Demographic 35", 	"Elmts démogr. 35", "Demographie 35",	"Demografía 35",	"Demografia 35",	"Demografia 35",	"Dados demogr. 35"	}	},
	{ SSTR_DUCK						,	{	"Demographic 36", 	"Elmts démogr. 36", "Demographie 36",	"Demografía 36",	"Demografia 36",	"Demografia 36",	"Dados demogr. 36"	}	},
	{ SSTR_FLAMINGO					,	{	"Demographic 37", 	"Elmts démogr. 37", "Demographie 37",	"Demografía 37",	"Demografia 37",	"Demografia 37",	"Dados demogr. 37"	}	},
	{ SSTR_GOOSE					,	{	"Demographic 38", 	"Elmts démogr. 38", "Demographie 38",	"Demografía 38",	"Demografia 38",	"Demografia 38",	"Dados demogr. 38"	}	},
	{ SSTR_LOVEBIRD					,	{	"Demographic 39", 	"Elmts démogr. 39", "Demographie 39",	"Demografía 39",	"Demografia 39",	"Demografia 39",	"Dados demogr. 39"	}	},
	{ SSTR_MACAW					,	{	"Demographic 40", 	"Elmts démogr. 40", "Demographie 40",	"Demografía 40",	"Demografia 40",	"Demografia 40",	"Dados demogr. 40"	}	},
	{ SSTR_MYNAH					,	{	"Demographic 41", 	"Elmts démogr. 41", "Demographie 41",	"Demografía 41",	"Demografia 41",	"Demografia 41",	"Dados demogr. 41"	}	},
	{ SSTR_OSTRICH					,	{	"Demographic 42", 	"Elmts démogr. 42", "Demographie 42",	"Demografía 42",	"Demografia 42",	"Demografia 42",	"Dados demogr. 42"	}	},
	{ SSTR_PARAKEET					,	{	"Demographic 43", 	"Elmts démogr. 43", "Demographie 43",	"Demografía 43",	"Demografia 43",	"Demografia 43",	"Dados demogr. 43"	}	},
	{ SSTR_PARROT					,	{	"Demographic 44", 	"Elmts démogr. 44", "Demographie 44",	"Demografía 44",	"Demografia 44",	"Demografia 44",	"Dados demogr. 44"	}	},
	{ SSTR_TOUCAN					,	{	"Demographic 45", 	"Elmts démogr. 45", "Demographie 45",	"Demografía 45",	"Demografia 45",	"Demografia 45",	"Dados demogr. 45"	}	},
	
	// Reptiles/amphibians
	{ SSTR_REPTILE					,	{	"Demographic 46",	"Elmts démogr. 46", "Demographie 46",	"Demografía 46",	"Demografia 46",	"Demografia 46",	"Dados demogr. 46"	}	},
	{ SSTR_ALLIGATOR				,	{	"Demographic 47",	"Elmts démogr. 47", "Demographie 47",	"Demografía 47",	"Demografia 47",	"Demografia 47",	"Dados demogr. 47"	}	},
	{ SSTR_AMPHIBIAN				,	{	"Demographic 48",	"Elmts démogr. 48", "Demographie 48",	"Demografía 48",	"Demografia 48",	"Demografia 48",	"Dados demogr. 48"	}	},
	{ SSTR_BEARDED_DRAGON			,	{	"Demographic 49",	"Elmts démogr. 49", "Demographie 49",	"Demografía 49",	"Demografia 49",	"Demografia 49",	"Dados demogr. 49"	}	},
	{ SSTR_BOA_CONSTRICTOR			,	{	"Demographic 50",	"Elmts démogr. 50", "Demographie 50",	"Demografía 50",	"Demografia 50",	"Demografia 50",	"Dados demogr. 50"	}	},
	{ SSTR_CHAMELEON				,	{	"Demographic 51",	"Elmts démogr. 51", "Demographie 51",	"Demografía 51",	"Demografia 51",	"Demografia 51",	"Dados demogr. 51"	}	},
	{ SSTR_CROCODILE				,	{	"Demographic 52",	"Elmts démogr. 52", "Demographie 52",	"Demografía 52",	"Demografia 52",	"Demografia 52",	"Dados demogr. 52"	}	},
	{ SSTR_FROG						,	{	"Demographic 53",	"Elmts démogr. 53", "Demographie 53",	"Demografía 53",	"Demografia 53",	"Demografia 53",	"Dados demogr. 53"	}	},
	{ SSTR_GECKO					,	{	"Demographic 54",	"Elmts démogr. 54", "Demographie 54",	"Demografía 54",	"Demografia 54",	"Demografia 54",	"Dados demogr. 54"	}	},
	{ SSTR_IGUANA					,	{	"Demographic 55",	"Elmts démogr. 55", "Demographie 55",	"Demografía 55",	"Demografia 55",	"Demografia 55",	"Dados demogr. 55"	}	},
	{ SSTR_LIZARD					,	{	"Demographic 56",	"Elmts démogr. 56", "Demographie 56",	"Demografía 56",	"Demografia 56",	"Demografia 56",	"Dados demogr. 56"	}	},
	{ SSTR_MONITOR					,	{	"Demographic 57",	"Elmts démogr. 57", "Demographie 57",	"Demografía 57",	"Demografia 57",	"Demografia 57",	"Dados demogr. 57"	}	},
	{ SSTR_PYTHON					,	{	"Demographic 58",	"Elmts démogr. 58", "Demographie 58",	"Demografía 58",	"Demografia 58",	"Demografia 58",	"Dados demogr. 58"	}	},
	{ SSTR_SALAMANDER				,	{	"Demographic 59",	"Elmts démogr. 59", "Demographie 59",	"Demografía 59",	"Demografia 59",	"Demografia 59",	"Dados demogr. 59"	}	},
	{ SSTR_SNAKE					,	{	"Demographic 60",	"Elmts démogr. 60", "Demographie 60",	"Demografía 60",	"Demografia 60",	"Demografia 60",	"Dados demogr. 60"	}	},
	{ SSTR_TOAD						,	{	"Demographic 61",	"Elmts démogr. 61", "Demographie 61",	"Demografía 61",	"Demografia 61",	"Demografia 61",	"Dados demogr. 61"	}	},
	{ SSTR_TORTOISE					,	{	"Demographic 62",	"Elmts démogr. 62", "Demographie 62",	"Demografía 62",	"Demografia 62",	"Demografia 62",	"Dados demogr. 62"	}	},
	{ SSTR_TURTLE					,	{	"Demographic 63",	"Elmts démogr. 63", "Demographie 63",	"Demografía 63",	"Demografia 63",	"Demografia 63",	"Dados demogr. 63"	}	},
	
	// Small mammals
	{ SSTR_SMALL_MAMMAL				,	{	"Demographic 64",	"Elmts démogr. 64", "Demographie 64",	"Demografía 64",	"Demografia 64",	"Demografia 64",	"Dados demogr. 64"	}	},
	{ SSTR_BAT						,	{	"Demographic 65",	"Elmts démogr. 65", "Demographie 65",	"Demografía 65",	"Demografia 65",	"Demografia 65",	"Dados demogr. 65"	}	},
	{ SSTR_CHINCHILLA				,	{	"Demographic 66",	"Elmts démogr. 66", "Demographie 66",	"Demografía 66",	"Demografia 66",	"Demografia 66",	"Dados demogr. 66"	}	},
	{ SSTR_FERRET					,	{	"Demographic 67",	"Elmts démogr. 67", "Demographie 67",	"Demografía 67",	"Demografia 67",	"Demografia 67",	"Dados demogr. 67"	}	},
	{ SSTR_GERBIL					,	{	"Demographic 68",	"Elmts démogr. 68", "Demographie 68",	"Demografía 68",	"Demografia 68",	"Demografia 68",	"Dados demogr. 68"	}	},
	{ SSTR_GUINEA_PIG				,	{	"Demographic 69",	"Elmts démogr. 69", "Demographie 69",	"Demografía 69",	"Demografia 69",	"Demografia 69",	"Dados demogr. 69"	}	},
	{ SSTR_HAMSTER					,	{	"Demographic 70",	"Elmts démogr. 70", "Demographie 70",	"Demografía 70",	"Demografia 70",	"Demografia 70",	"Dados demogr. 70"	}	},
	{ SSTR_HEDGEHOG					,	{	"Demographic 71",	"Elmts démogr. 71", "Demographie 71",	"Demografía 71",	"Demografia 71",	"Demografia 71",	"Dados demogr. 71"	}	},
	{ SSTR_LEMUR					,	{	"Demographic 72",	"Elmts démogr. 72", "Demographie 72",	"Demografía 72",	"Demografia 72",	"Demografia 72",	"Dados demogr. 72"	}	},
	{ SSTR_MINK						,	{	"Demographic 73",	"Elmts démogr. 73", "Demographie 73",	"Demografía 73",	"Demografia 73",	"Demografia 73",	"Dados demogr. 73"	}	},
	{ SSTR_MOUSE					,	{	"Demographic 74",	"Elmts démogr. 74", "Demographie 74",	"Demografía 74",	"Demografia 74",	"Demografia 74",	"Dados demogr. 74"	}	},
	{ SSTR_OPOSSUM					,	{	"Demographic 75",	"Elmts démogr. 75", "Demographie 75",	"Demografía 75",	"Demografia 75",	"Demografia 75",	"Dados demogr. 75"	}	},
	{ SSTR_PRAIRIE_DOG				,	{	"Demographic 76",	"Elmts démogr. 76", "Demographie 76",	"Demografía 76",	"Demografia 76",	"Demografia 76",	"Dados demogr. 76"	}	},
	{ SSTR_RABBIT					,	{	"Demographic 77",	"Elmts démogr. 77", "Demographie 77",	"Demografía 77",	"Demografia 77",	"Demografia 77",	"Dados demogr. 77"	}	},
	{ SSTR_RACCOON					,	{	"Demographic 78",	"Elmts démogr. 78", "Demographie 78",	"Demografía 78",	"Demografia 78",	"Demografia 78",	"Dados demogr. 78"	}	},
	{ SSTR_RAT						,	{	"Demographic 79",	"Elmts démogr. 79", "Demographie 79",	"Demografía 79",	"Demografia 79",	"Demografia 79",	"Dados demogr. 79"	}	},
	{ SSTR_SKUNK					,	{	"Demographic 80",	"Elmts démogr. 80", "Demographie 80",	"Demografía 80",	"Demografia 80",	"Demografia 80",	"Dados demogr. 80"	}	},
	{ SSTR_SQUIRREL					,	{	"Demographic 81",	"Elmts démogr. 81", "Demographie 81",	"Demografía 81",	"Demografia 81",	"Demografia 81",	"Dados demogr. 81"	}	},
	
	// Large mammals
	{ SSTR_LARGE_MAMMAL				,	{	"Demographic 82",	"Elmts démogr. 82", "Demographie 82",	"Demografía 82",	"Demografia 82",	"Demografia 82",	"Dados demogr. 82"	}	},
	{ SSTR_ANTELOPE					,	{	"Demographic 83",	"Elmts démogr. 83", "Demographie 83",	"Demografía 83",	"Demografia 83",	"Demografia 83",	"Dados demogr. 83"	}	},
	{ SSTR_APE						,	{	"Demographic 84",	"Elmts démogr. 84", "Demographie 84",	"Demografía 84",	"Demografia 84",	"Demografia 84",	"Dados demogr. 84"	}	},
	{ SSTR_BABOON					,	{	"Demographic 85",	"Elmts démogr. 85", "Demographie 85",	"Demografía 85",	"Demografia 85",	"Demografia 85",	"Dados demogr. 85"	}	},
	{ SSTR_BEAR						,	{	"Demographic 86",	"Elmts démogr. 86", "Demographie 86",	"Demografía 86",	"Demografia 86",	"Demografia 86",	"Dados demogr. 86"	}	},
	{ SSTR_CAMEL					,	{	"Demographic 87",	"Elmts démogr. 87", "Demographie 87",	"Demografía 87",	"Demografia 87",	"Demografia 87",	"Dados demogr. 87"	}	},
	{ SSTR_CHIMPANZEE				,	{	"Demographic 88",	"Elmts démogr. 88", "Demographie 88",	"Demografía 88",	"Demografia 88",	"Demografia 88",	"Dados demogr. 88"	}	},
	{ SSTR_DEER						,	{	"Demographic 89",	"Elmts démogr. 89", "Demographie 89",	"Demografía 89",	"Demografia 89",	"Demografia 89",	"Dados demogr. 89"	}	},
	{ SSTR_ELEPHANT					,	{	"Demographic 90",	"Elmts démogr. 90", "Demographie 90",	"Demografía 90",	"Demografia 90",	"Demografia 90",	"Dados demogr. 90"	}	},
	{ SSTR_GIRAFFE					,	{	"Demographic 91",	"Elmts démogr. 91", "Demographie 91",	"Demografía 91",	"Demografia 91",	"Demografia 91",	"Dados demogr. 91"	}	},
	{ SSTR_GOAT						,	{	"Demographic 92",	"Elmts démogr. 92", "Demographie 92",	"Demografía 92",	"Demografia 92",	"Demografia 92",	"Dados demogr. 92"	}	},
	{ SSTR_KANGAROO					,	{	"Demographic 93",	"Elmts démogr. 93", "Demographie 93",	"Demografía 93",	"Demografia 93",	"Demografia 93",	"Dados demogr. 93"	}	},
	{ SSTR_LLAMA					,	{	"Demographic 94",	"Elmts démogr. 94", "Demographie 94",	"Demografía 94",	"Demografia 94",	"Demografia 94",	"Dados demogr. 94"	}	},
	{ SSTR_MONKEY					,	{	"Demographic 95",	"Elmts démogr. 95", "Demographie 95",	"Demografía 95",	"Demografia 95",	"Demografia 95",	"Dados demogr. 95"	}	},
	{ SSTR_PIG						,	{	"Demographic 96",	"Elmts démogr. 96", "Demographie 96",	"Demografía 96",	"Demografia 96",	"Demografia 96",	"Dados demogr. 96"	}	},
	{ SSTR_SEA_LION					,	{	"Demographic 97",	"Elmts démogr. 97", "Demographie 97",	"Demografía 97",	"Demografia 97",	"Demografia 97",	"Dados demogr. 97"	}	},
	{ SSTR_SEAL						,	{	"Demographic 98",	"Elmts démogr. 98", "Demographie 98",	"Demografía 98",	"Demografia 98",	"Demografia 98",	"Dados demogr. 98"	}	},
	{ SSTR_SHEEP					,	{	"Demographic 99",	"Elmts démogr. 99", "Demographie 99",	"Demografía 99",	"Demografia 99",	"Demografia 99",	"Dados demogr. 99"	}	},
	
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
	{ STR_FEB						,	{	"Feb",			"Fév",		"Feb",			"Feb",			"Feb",			"Fev",			"Fev"	}	},
	{ STR_MAR						,	{	"Mar",			"Mar",		"Mär",			"Mar",			"Mar",			"Mar",			"Mar"	}	},
	{ STR_APR						,	{	"Apr",			"Avr",		"Apr",			"Abr",			"Apr",			"Abr",			"Abr"	}	},
	{ STR_MAY						,	{	"May",			"Mai",		"Mai",			"May",			"Mag",			"Mai",			"Mai"	}	},
	{ STR_JUN						,	{	"Jun",			"Jun",		"Jun",			"Jun",			"Giu",			"Jun",			"Jun"	}	},
	{ STR_JUL						,	{	"Jul",			"Jul",		"Jul",			"Jul",			"Lug",			"Jul",			"Jul"	}	},
	{ STR_AUG						,	{	"Aug",			"Aoû",		"Aug",			"Ago",			"Ago",			"Ago",			"Ago"	}	},
	{ STR_SEP						,	{	"Sep",			"Sep",		"Sep",			"Sep",			"Set",			"Set",			"Set"	}	},
	{ STR_OCT						,	{	"Oct",			"Oct",		"Okt",			"Oct",			"Ott",			"Out",			"Out"	}	},
	{ STR_NOV						,	{	"Nov",			"Nov",		"Nov",			"Nov",			"Nov",			"Nov",			"Nov"	}	},
	{ STR_DEC						,	{	"Dec",			"Déc",		"Dez",			"Dic",			"Dic",			"Dez",			"Dez"	}	},

	{ STR_SUNDAY					,	{	"Sunday",		"Dimanche",	"Sonntag",		"Domingo",		"Domenica",		"Domingo",		"Domingo"	}	},
	{ STR_MONDAY					,	{	"Monday",		"Lundi",	"Montag",		"Lunes",		"Lunedì",		"Segunda",		"Segunda-feira"	}	},
	{ STR_TUESDAY					,	{	"Tuesday",		"Mardi",	"Dienstag",		"Martes",		"Martedì",		"Terça",		"Terça-feira"	}	},
	{ STR_WEDNESDAY					,	{	"Wednesday",	"Mercredi",	"Mittwoch",		"Miércoles",	"Mercoledì",	"Quarta",		"Quarta-feira"	}	},
	{ STR_THURSDAY					,	{	"Thursday",		"Jeudi",	"Donnerstag",	"Jueves",		"Giovedì",		"Quinta",		"Quinta-feira"	}	},
	{ STR_FRIDAY					,	{	"Friday",		"Vendredi",	"Freitag",		"Viernes",		"Venerdì",		"Sexta",		"Sexta-feira"	}	},
	{ STR_SATURDAY					,	{	"Saturday",		"Samedi",	"Samstag",		"Sábado",		"Sabato",		"Sábado",		"Sábado"	}	},

	{ STR_NONE						,	{} }
};


StringArr_t	VET_STRINGS[] = {
// 											ENGLISH,					FRENCH,								GERMAN,								SPANISH,								ITALIAN, 								PORTUGUESE,									PORTUGUESE (BRAZIL)
	{ STR_PRODUCT_NAME				,	{	"VetScan VS2",				"VetScan VS2",						"VetScan VS2",						"VetScan VS2",							"VetScan VS2",							"VetScan VS2",								"VetScan VS2"	}	},

// Top
	{ TSTR_ADD_SPECIES				,	{	"Add Species",				"Ajouter espèces",					"Arten hinzufügen",					"Añadir especie",						"Aggiungi specie",						"Adicionar espécies",						"Adicionar espécie"	}	},
	{ TSTR_FACTORY_DEFAULT_SPECIES	,	{	"Factory Default Species",	"Paramètres par défaut espèces",	"Voreingestellte Standardarten",	"Especies predeterminadas",				"Specie predefinita di fabbrica",		"Espécies predeterminadas de fábrica",		"Espécie padrão de fábrica"	}	},
	{ TSTR_INSERT_ROTOR				,	{	"Insert Rotor",				"Insérer rotor",					"Rotor einsetzen",					"Insertar rotor",						"Inserisci rotore",						"Inserir rotor",							"Inserir rotor"	}	},
	{ TSTR_LOAD_ROTOR				,	{	"Load Rotor",				"Charger rotor",					"Rotor laden",						"Cargar rotor",							"Carica rotore",						"Carregar rotor",							"Carregar rotor"	}	},
	{ TSTR_REMOVE_ROTOR				,	{	"Remove Rotor",				"Retirer rotor",					"Rotor entfernen",					"Retirar rotor",						"Rimuovi rotore",						"Remover rotor",							"Remover rotor"	}	},
	{ TSTR_REMOVE_SPECIES			,	{	"Remove Species",			"Retirer espèces",					"Arten entfernen",					"Eliminar especie",						"Rimuovi specie",						"Remover espécies",							"Remover espécies"	}	},
	{ TSTR_SELECT_SPECIES			,	{	"Select Species",			"Sélectionner espèces",				"Arten auswählen",					"Seleccionar especie",					"Seleziona specie",						"Selecionar espécies",						"Selecionar espécies"	}	},
	{ TSTR_SPECIES					,	{	"Species",					"Espèces",							"Arten",							"Especie",								"Specie",								"Espécies",									"Espécies"	}	},
	{ TSTR_SPECIES_AVAILABLE		,	{	"Species Available",		"Espèces disponibles",				"Verfügbare Arten",					"Especies disponibles",					"Specie disponibili",					"Espécies disponíveis",						"Espécies disponíveis"	}	},

// Middle									ENGLISH,																		FRENCH,																							GERMAN,																										SPANISH,																							ITALIAN,																								PORTUGUESE,																									PORTUGUESE (BRAZIL)
	{ MSTR_ADD_SPECIES				,	{	"Add Species",																	"Ajouter espèces",																				"Arten hinzufügen",																							"Añadir especie",																					"Aggiungi specie",																						"Adicionar espécies",																						"Adicionar espécie"	}	},
	{ MSTR_DETERMINING_ROTOR_TYPE	,	{	"Determining rotor type",														"Déterm. type de rotor",																		"Rotortyp festlegen",																						"Determinando el tipo de rotor",																	"Determinazione tipo del rotore",																		"A determinar o tipo de rotor",																				"Determinando o tipo de rotor"	}	},
	{ MSTR_FACTORY_DEFAULT_SPECIES	,	{	"Factory Default Species",														"Param. par défaut espèces",																	"Voreingestellte Arten",																					"Especie predeterminada de fábrica",																"Specie predefinita di fabbrica",																		"Espécies predeterminadas de fábrica",																		"Espécie padrão de fábrica"	}	},
	{ MSTR_FEMALE					,	{	"Female",																		"Femelle",																						"Weiblich",																									"Hembra",																							"Femmina",																								"Fêmea",																									"Sexo feminino"	}	},
	{ MSTR_LAST_ROTOR				,	{	"Last Rotor",																	"Dernier rotor",																				"Letzter Rotor",																							"Último rotor",																						"Ultimo rotore",																						"Último rotor",																								"Último rotor"	}	},
	{ MSTR_LOAD_ROTOR_TO_ANALYZE	,	{	"To analyze additional sample, load new rotor",									"Pour analyser un échantillon supplémentaire, charger un nouveau rotor",						"Zur Analyse einer weiteren Probe neuen Rotor laden",														"Para analizar otra muestra, cargue un rotor nuevo",												"Per esaminare ulteriori campioni, caricare nuovo rotore",												"Para analisar outra amostra, carregue novo rotor",															"Para analisar uma amostra adicional, carregue um novo rotor"	}	},
	{ MSTR_MALE						,	{	"Male",																			"Mâle",																							"Männlich",																									"Macho",																							"Maschio",																								"Macho",																									"Sexo masculino"	}	},
	{ MSTR_NO_RESULTS				,	{	"No saved results to view. Please run a rotor prior to recalling results.",		"Aucun résultat enregistré à afficher. Exécuter un rotor avant de rappeler les résultats.",		"Keine gespeicherten Ergebnisse zur Anzeige. Vor Wiederaufrufen der Ergebnisse einen Rotor starten.",		"No hay resultados guardados para ver. Utilice un rotor antes de buscar resultados.",				"Nessun risultato salvato da visualizzare. Avviare un rotore prima di richiamare i risultati.",			"No existem resultados guardados para visualizar. Utilize um rotor antes de recuperar resultados.",			"Nenhum resultado salvo para visualização. Execute um rotor antes de recuperar os resultados."	}	},
	{ MSTR_PLEASE_REMOVE_ROTOR		,	{	"Please remove the rotor from the analyzer",									"Retirer le rotor de l'analyseur",																"Bitte den Rotor aus dem Analysegerät entfernen",															"Retire el rotor del analizador",																	"Rimuovere il rotore dall'analizzatore",																"Retire o rotor do analisador",																				"Remova o rotor do analisador"	}	},
	{ MSTR_PRESS_YES_REF_RANGES		,	{	"Press Yes to return the active species list to the factory default",			"Appuyer sur Oui pour rétablir la liste d'espèces actives par défaut",							"Drücken Sie Ja, um die Werkseinstellungen für die Liste der aktiven Arten wieder herzustellen",			"Pulse Sí para reestablecer los valores predeterminados de la lista de especies activas",			"Premere Sì per mandare l'elenco specie attive nelle impostazioni di fabbrica",							"Prima Sim para reestabelecer a lista de espécies ativa para os valores predeterminados de fábrica",		"Pressione Sim para retornar a lista de espécies ativas ao padrão de fábrica"	}	},
	{ MSTR_REMOVE_SPECIES			,	{	"Remove Species",																"Retirer espèces",																				"Arten entfernen",																							"Eliminar especie",																					"Rimuovi specie",																						"Remover espécies",																							"Remover espécies"	}	},
	{ MSTR_ROTORS_RUN				,	{	"Rotors Run",																	"Exécutions rotors",																			"Rotoren in Betrieb",																						"Usar rotores",																						"Rotori in funzione",																					"Utilizar rotores",																							"Execução de rotores"	}	},
	{ MSTR_SPECIES					,	{	"Species",																		"Espèces",																						"Arten",																									"Esp.",																								"Specie",																								"Espécies",																									"Espécies"	}	},

// Info text
	{ ISTR_LOAD_ROTOR_1				,	{	"Remove reagent rotor from its foil pouch and transfer 100ul of whole blood, plasma or serum into the rotor.\n",				// English
											"Retirer le rotor de réactif de sa pochette alu et transférer 100 ul de sang entier, de plasma ou de sérum dans le rotor.\n",	// French
											"Reagenzrotor aus dem Plastikbeutel entfernen und 100ul Vollblut, Plasma oder Serum in den Rotor einfüllen.\n",					// German
											"Retire el rotor de reactivo de su bolsa metalizada y transfiera 100 ul de sangre, plasma o suero al rotor.\n",					// Spanish
											"Rimuovere il rotore reagente dalla custodia metallizzata e trasferire 100ul di sangue intero, plasma o siero nel rotore.\n",	// Italian
											"Retire o rotor de reagente da sua bolsa metalizada e transfira 100 ul de sangue total, plasma ou soro para o rotor.\n",		// Portuguese
											"Remova o rotor de reagente da bolsa de alumínio e transfira 100 µl de sangue total, plasma ou soro para o rotor.\n"			// Portuguese (Brazil)
										} 
	},
	{ ISTR_LOAD_ROTOR_2				,	{	"Insert the reagent rotor into the analyzer, close the drawer and follow the on-screen prompts.\n",								// English
											"Insérer le rotor de réactif dans l'analyseur, fermer le tiroir et suivre les instructions qui s'affichent à l'écran.\n",		// French
											"Reagenzrotor in das Analysegerät einführen, Fach schließen und Bildschirmanweisungen befolgen.\n",								// German
											"Inserte el rotor de reactivo en el analizador, cierre el cajón y siga las instrucciones de pantalla.\n",						// Spanish
											"Inserire il rotore reagente nell'analizzatore, chiudere il cassetto e seguire le istruzioni a schermo.\n",						// Italian
											"Insira o rotor de reagente no analisador, feche a gaveta e siga as instruções no ecrã.\n",										// Portuguese
											"Insira o rotor de reagente no analisador, feche a gaveta e siga as instruções na tela.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_ARCHIVE_SEND				,	{	"Printer: Print a copy of a species reference range.\n\n"
											"Internal Archive: Save a copy of all active reference ranges internal to the analyzer for future recall.\n\n"
											"External Archive: Transmit an ASCII dump of a species reference range to an external system.\n",																		// English
											"Imprimante : imprimer un exemplaire de fourchette de référence d'espèces.\n\n"
											"Archive interne : enregistrer un exemplaire de toutes les fourchettes de référence actives internes à l'analyseur afin de pouvoir les récupérer ultérieurement.\n\n"
											"Archive externe : transmettre un vidage de mémoire ASCII d'une fourchette de référence d'espèces à un système externe.\n",												// French
											"Drucker: Kopie eines Artenreferenzbereichs drucken.\n\n"
											"Internes Archiv: Eine Kopie aller aktiven Referenzbereiche für spätere Verwendung intern im Analysegerät speichern.\n\n"
											"Externes Archiv: ASCII-Auszug eines Artenreferenzbereichs an ein externes System übertragen.\n",																		// German
											"Impresora: imprimir una copia de un intervalo de referencia de especies.\n\n"
											"Archivo interno: guardar una copia de todos los intervalos de referencia activos internos para el analizador para usos futuros.\n\n"
											"Archivo externo: hacer un volcado de datos ASCII de un intervalo de referencia de especies a un sistema externo.\n",													// Spanish
											"Stampante: stampa una copia di un range di riferimento per specie.\n\n"
											"Archivio interno: salva una copia di tutti i range di riferimento attivi interni all'analizzatore per utilizzo futuro.\n\n"
											"Archivio esterno: trasmette una serie ASCII di range di riferimento per specie ad un sistema esterno.\n",																// Italian
											"Impressora: Imprime uma cópia de um intervalo de referência de espécies.\n\n"
											"Ficheiro interno: Guardar uma cópia de todos os intervalos de referência ativos internos no analisador para utilização futura.\n\n"
											"Ficheiro externo: Transmitir um dump de ASCII de um intervalo de referência de espécies para um sistema externo.\n",													// Portuguese
											"Impressora: Imprima uma cópia de um intervalo de referência de espécies.\n\n"
											"Arquivo interno: Salve uma cópia de todos os intervalos de referência ativos internos no analisador, para futura recuperação.\n\n"
											"Arquivo externo: Transmita um despejo ASCII de um intervalo de referência de espécies para um sistema externo.\n"														// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_ADD			,	{	"Species n: The next available generic species label between 1 and 99.\n\n"
											"Control n: The next available control label between 1 and 99.\n\n"
											"Select: Provides a list of common species labels to select from.\n",																// English
											"Espèce n : la prochaine étiquette d'espèce générique disponible comprise entre 1 et 99.\n\n"
											"Contrôle n : la prochaine étiquette de contrôle disponible comprise entre 1 et 99.\n\n"
											"Sélectionner : fournit une liste d'étiquettes d'espèces communes à partir de laquelle la sélection pourra être effectuée.\n",		// French
											"Arten n: Das nächste verfügbare generische Artenetikett zwischen 1 und 99.\n\n"
											"Steuerung n: Das nächste verfügbare Steuerungsetikett zwischen 1 und 99.\n\n"
											"Auswählen: Stellt eine Liste üblicher Artenetiketten zur Auswahl.\n",																// German
											"Especie n: la próxima etiqueta de especie genérica disponible entre 1 y 99.\n\n"
											"Control n: la próxima etiqueta de control disponible entre 1 y 99.\n\n"
											"Seleccionar: proporciona una lista para selección de etiquetas de especies comunes.\n",											// Spanish
											"Specie n: la successiva etichetta di specie generica disponibile tra 1 e 99.\n\n"
											"Controllo n: la successiva etichetta di controllo disponibile tra 1 e 99.\n\n"
											"Seleziona: fornisce un elenco delle etichette delle specie comuni tra cui scegliere.\n",											// Italian
											"Espécies n: A próxima etiqueta de espécies genérica disponível entre 1 e 99.\n\n"
											"Controlo n: A próxima etiqueta de controlo disponível entre 1 e 99.\n\n"
											"Selecionar: Fornece uma lista para seleção de etiquetas de espécies frequentes.\n",												// Portuguese
											"N espécies: O próximo rótulo de espécie genérico disponível entre 1 e 99.\n\n"
											"N controle: O próximo rótulo de controle disponível entre 1 e 99.\n\n"
											"Selecionar: Fornece uma lista de rótulos de espécies comuns para seleção.\n"														// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_MODIFY		,	{	"Analyte: Provides a list of all active reference ranges for a particular analyte.\n\n"
											"Species: Provides a list of all analytes for all reference ranges.\n",																// English
											"Substance à analyser : fournit une liste de toutes les fourchettes de référence actives pour une substance à analyser donnée.\n\n"
											"Espèce : fournit une liste de toutes les substances à analyser pour toutes les fourchettes de référence.\n",						// French
											"Analyt: Anzeige einer Liste aller aktiven Referenzbereiche für ein bestimmtes Analyt.\n\n"
											"Arten: Anzeige einer Liste aller Analyte für alle Referenzbereiche.\n",															// German
											"Analito: proporciona una lista de todos los intervalos de referencia activos para un analito particular.\n\n"
											"Especie: proporciona una lista de todos los analitos para todos los intervalos de referencia.\n",									// Spanish
											"Analita: fornisce un elenco di tutti i range di riferimento attivi per un particolare analita.\n\n"
											"Specie: fornisce un elenco di tutti gli analiti per ogni range di riferimento.\n",													// Italian
											"Analito: Fornece uma lista de todos os intervalos de referencia ativos para um analito específico.\n\n"
											"Espécies: Fornece uma lista de todos os analitos para todos os intervalos de referência.\n",										// Portuguese
											"Analito: Fornece uma lista de todos os intervalos de referência ativos para um determinado analito.\n\n"
											"Espécies: Fornece uma lista de todos os analitos para todos os intervalos de referência.\n"										// Portuguese (Brazil)
										} 
	},
	{ ISTR_REF_RANGES_REMOVE		,	{	"Removing a species from the sample type list will remove the item from the list of species shown when running a rotor, and any values entered will be permanently lost.\n",																	// English
											"La suppression d'une espèce dans la liste de type d'échantillons a pour effet de supprimer cet élément dans la liste d'espèces affichées lors de l'exécution d'un rotor, et toutes les valeurs saisies seront irrémédiablement perdues.\n",	// French
											"Die Entfernung einer Art aus der Probentypliste entfernt das Objekt aus der Artenliste, die bei laufendem Rotor angezeigt wird, und sämtliche eingegebene Werte gehen dauerhaft verloren.\n",													// German
											"Al eliminar una especie de la lista del tipo de muestra se eliminará la entrada de la lista de especies mostrada al usar el rotor, y cualquier valor introducido se perderá permanentemente.\n",												// Spanish
											"Rimuovendo una specie dall'elenco dei tipi si rimuoverà quella voce dall'elenco di specie proposto quando un rotore è in funzione e tutti i valori immessi saranno persi in modo permanente.\n",												// Italian
											"Ao eliminar uma espécie da lista de tipo de amostra será eliminado o item da lista de espécies exibida quando se utiliza um rotor, e qualquer valor introduzido será permanentemente perdido.\n",												// Portuguese
											"A remoção de uma espécie da lista de tipos de amostra removerá o item da lista de espécie mostrada durante a execução de um rotor e quaisquer valores inseridos serão permanentemente perdidos.\n"												// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_ENZYME_UNITS			,	{	"Selecting a unit on this screen will change the units for ALT, ALP, AMY, AST, CK and GGT  simultaneously to the same units.\n",					// English
											"L'unité sélectionnée dans cet écran sera appliquée simultanément à ALT, ALP, AMY, AST, CK et GGT.\n",												// French
											"Mit der Auswahl einer Einheit in diesem Menü werden automatisch die Einheiten für ALT, ALP, AMY, AST, CK und GGT auf diese Einheit umgestellt.\n",	// German
											"Al seleccionar una unidad en esta pantalla, se cambiará ALT, ALP, AMY, AST, CK y GGT simultáneamente a la misma unidad.\n",						// Spanish
											"La selezione di un'unità in questa schermata modifica simultaneamente le unità per ALT, ALP, AMY, AST, CK e GGT nelle stesse unità.\n",			// Italian
											"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades ALT, ALP, AMY, AST, CK e GGT para as mesmas unidades.\n",		// Portuguese
											"A seleção de uma unidade nessa tela mudará as unidades para ALT, ALP, AMY, AST, CK e GGT simultaneamente para as mesmas unidades.\n"				// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_LIPID_UNITS			,	{	"Selecting a unit on this screen will change the units for CHOL.\n",						// English
											"L'unité sélectionnée dans cet écran sera appliquée à CHOL.\n",								// French
											"Mit der Auswahl einer Einheit in diesem Menü wird die Einheit für CHOL umgestellt.\n",		// German
											"Al seleccionar una unidad en esta pantalla, se cambiará la unidad de COL.\n",				// Spanish
											"La selezione di un'unità in questa schermata modifica le unità per COL.\n",				// Italian
											"Ao selecionar uma unidade neste ecrã, serão alteradas as unidades CHOL.\n",				// Portuguese
											"A seleção de uma unidade nessa tela mudará as unidades para CHOL.\n"						// Portuguese (Brazil)
										} 
	},
	{ ISTR_SET_PROTEIN_UNITS		,	{	"Selecting a unit on this screen will change the units for ALB, GLOB and TP simultaneously to the same units.\n",									// English
											"L'unité sélectionnée dans cet écran sera appliquée simultanément à ALB, GLOB et TP.\n",															// French
											"Mit der Auswahl einer Einheit in diesem Menü werden automatischdie Einheiten für ALB, GLOB und TP auf diese Einheit umgestellt.\n",				// German
											"Al seleccionar una unidad en esta pantalla, se cambiará ALB, GLOB y TP simultáneamente a la misma unidad.\n",										// Spanish
											"La selezione di un'unità in questa schermata modifica simultaneamente le unità per ALB e GLOB nelle stesse unità.\n",								// Italian
											"Ao selecionar uma unidade neste ecrã, serão simultaneamente alteradas as unidades ALB, GLOB e TP para as mesmas unidades.\n",						// Portuguese
											"A seleção de uma unidade nessa mudará as unidades para ALB, GLOB e TP simultaneamente para as mesmas unidades.\n"									// Portuguese (Brazil)
										} 
	},
	
// Error screens							ENGLISH,							FRENCH,									GERMAN,								SPANISH,											ITALIAN, 									PORTUGUESE,								PORTUGUESE (BRAZIL)
	{ ESTR_ILLEGAL_ROTOR_TYPE		,	{	"Illegal Rotor Type",				"Type rotor non autorisé",				"Falscher Rotortyp",				"Tipo de rotor ilegal",								"Tipo rotore illegale",						"Tipo de rotor inválido",				"Tipo de rotor ilegal"	}	},
	{ ESTR_NEW_ROTOR_TYPE			,	{	"New Rotor Type",					"Nouveau type de rotor",				"Neuer Rotortyp",					"Nuevo tipo de rotor",								"Nuovo tipo rotore",						"Tipo novo de rotor",					"Novo tipo de rotor"	}	},
	{ ESTR_PLEASE_REMOVE_ROTOR		,	{	"Please Remove Rotor",				"Retirer le rotor",						"Bitte Rotor entfernen",			"Retire el rotor",									"Si prega di rimuovere il rotore",			"Retirar rotor",						"Remova o rotor"	}	},
	{ ESTR_ROTOR_DATE_ILLEGAL		,	{	"Rotor Date Illegal",				"Date rotor non autorisée",				"Falsches Rotorendatum",			"Fecha de rotor ilegal",							"Data rotore illegale",						"Data do rotor inválida",				"Data do rotor ilegal"	}	},
	{ ESTR_ROTOR_DATE_IMPROPER		,	{	"Rotor Date Improper",				"Date rotor incorrecte",				"Unzulässiges Rotorendatum",		"Fecha rotor no adecuada",							"Data rotore impropria",					"Data do rotor inadequada",				"Data do rotor imprópria"	}	},
	{ ESTR_ROTOR_EXPIRED			,	{	"Rotor Expired",					"Expiration rotor",						"Abgelaufener Rotor",				"Rotor caducado",									"Rotore scaduto",							"Rotor expirado",						"Rotor expirado"	}	},
	{ ESTR_UPDATED_ROTOR_TYPE		,	{	"Updated Rotor Type",				"Type de rotor actualisé",				"Aktualisierter Rotortyp",			"Tipo de rotor actualizado",						"Tipo rotore aggiornato",					"Tipo de rotor atualizado",				"Tipo de rotor atualizado"	}	},

// Warning screens							ENGLISH,							FRENCH,									GERMAN,								SPANISH,											ITALIAN, 									PORTUGUESE,								PORTUGUESE (BRAZIL)
	{ ESTR_ROTOR_DETECTED			,	{	"Rotor detected during warming",	"Rotor détecté en cours de chauffage",	"Rotor beim Aufwärmen vorgefunden",	"Rotor detectado durante el calentamiento inicial",	"Rotore individuato durante riscaldamento",	"Rotor detetado durante o aquecimento",	"Rotor detectado durante o aquecimento"	}	},

// Error info text
	{ EISTR_AMBIENT_TEMP			,	{	"The temperature outside the analyzer is either too hot or too cold to run a rotor. Contact Technical Service for assistance.\n",															// English
											"La température à l'extérieur de l'analyseur est trop élevée ou trop faible pour permettre le fonctionnement d'un rotor. Pour obtenir une assistance, contacter le service technique.\n",	// French
											"Die Temperatur außerhalb des Analysegeräts ist für den Rotorenbetrieb zu heiß oder zu kalt. Wenden Sie sich an den technischen Service.\n",												// German
											"La temperatura fuera del analizador es demasiado alta o demasiado baja para usar el rotor. Póngase en contacto con el servicio técnico.\n",												// Spanish
											"La temperatura esterna all'analizzatore è troppo alta o troppo bassa per permettere il funzionamento di un rotore. Per assistenza contattare il servizio tecnico.\n",						// Italian
											"A temperatura fora do analisador é demasiado alta ou demasiado baixa para executar um rotor. Contacte o Serviço de Assistência Técnica para obter assistência.\n",							// Portuguese
											"A temperatura externa do analisador está muito alta ou muito baixa para executar um rotor. Entre em contato com o Serviço Técnico para obter assistência.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_ANALYZER_DATE			,	{	"Date in analyzer is earlier than that of the rotor being run. Check dates in analyzer and rerun with new rotor.\n",											// English
											"La date de l'analyseur est antérieure à celle du rotor en cours d'utilisation. Vérifier les dates sur l'analyseur et réexécuter le nouveau rotor.\n",			// French
											"Das Datum im Analysegerät liegt vor dem des Rotorenbetriebs. Prüfen Sie das Datum im Analysegerät und nehmen Sie den Betrieb mit einem neuen Rotor auf.\n",	// German
											"La fecha del analizador es anterior a la fecha del rotor que se va a usar. Compruebe las fechas del analizador y vuelva a usar el nuevo rotor.\n",				// Spanish
											"La data dell'analizzatore è precedente a quella di attivazione del rotore. Controllare le date dell'analizzatore e riavviare con un nuovo rotore.\n",			// Italian
											"A data do analisador é anterior à data do rotor que foi iniciado. Verifique as datas do analisador e reinicie com um novo rotor.\n",							// Portuguese
											"A data no analisador é anterior à do rotor em execução. Verifique as datas no analisador e execute novamente com um novo rotor.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE					,	{	"The analyzer was unable to read the rotor's barcode. If symptom continues, contact Technical Service.\n",														// English
											"L'analyseur n'a pas pu lire le code à barres du rotor. En cas de persistance des symptômes, contacter le service technique.\n",								// French
											"Das Analysegerät konnte den Barcode des Rotors nicht lesen. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",						// German
											"El analizador no pudo leer el código de barras del rotor. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",			// Spanish
											"L'analizzatore non è riuscito a leggere il codice a barre del rotore. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"O analisador não consegue ler o código de barras do rotor. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",								// Portuguese
											"O analisador não foi capaz de ler o código de barras do rotor. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_BARCODE_FORMAT			,	{	"The analyzer did not recognize the rotor. Contact Technical Service for further assistance.\n",						// English
											"L'analyseur n'a pas reconnu le rotor. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											"Das Analysegerät konnte den Rotor nicht erkennen. Wenden Sie sich an den technischen Service.\n",						// German
											"El analizador no ha reconocido el rotor. Póngase en contacto con el servicio técnico.\n",								// Spanish
											"L'analizzatore non ha riconosciuto il rotore. Per ulteriore assistenza contattare il servizio tecnico.\n",				// Italian
											"O analisador não consegue reconhecer o rotor. Contacte o Serviço de Assistência Técnica para obter assistência.\n",	// Portuguese
											"O analisador não reconheceu o rotor. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"		// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MISSING			,	{	"The analyzer did not detect all the chemistry beads in the rotor. Rerun sample using new rotor.\n",											// English
											"L'analyseur n'a pas détecté toutes les billes chimiques dans le rotor. Réexécuter l'échantillon à l'aide d'un nouveau rotor.\n",				// French
											"Das Analysegerät konnte nicht alle chemischen Beads in dem Rotor erkennen. Wiederholen Sie die Probe mit einem neuen Rotor.\n",				// German
											"El analizador no detectó todas las cuentas químicas en el rotor. Vuelva a utilizar la muestra usando un nuevo rotor.\n",						// Spanish
											"L'analizzatore non ha individuato tutti i granuli chimici nel rotore. Riavviare il campione con un nuovo rotore.\n",							// Italian
											"O analisador não detetou todas as esferas de substâncias químicas no rotor. Executar novamente a amostra utilizando um rotor novo.\n",			// Portuguese
											"O analisador não detectou todas as esferas de substância química no rotor. Execute novamente a amostra usando o novo rotor.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_BEAD_MIX				,	{	"The analyzer will not be able to determine the exposure status if the QC beads have not mixed correctly. Rerun with a new rotor.\n",									// English
											"L'analyseur ne parviendra pas à déterminer l'état d'exposition si les billes QC ne sont pas correctement mélangées. Réexécuter avec un nouveau rotor.\n",				// French
											"Das Analysegerät kann den Status der Einwirkung nicht feststellen, wenn die QC-Beads nicht ordnungsgemäß vermischt wurden. Mit einem neuen Rotor wiederholen.\n",		// German
											"El analizador no será capaz de determinar el estado de exposición si las cuentas QC no se mezclan correctamente. Debe usarse con un nuevo rotor.\n",					// Spanish
											"L'analizzatore non potrà determinare lo stato di esposizione se i granuli di QC non sono stati mescolati correttamente. Riavviare con un nuovo rotore.\n",				// Italian
											"O analisador não conseguirá determinar o estado de exposição se as esferas de CQ não forem misturadas adequadamente. Executar novamente utilizando um rotor novo.\n",	// Portuguese
											"O analisador não poderá determinar o status de exposição se as esferas de QC não tiverem sido misturadas corretamente. Execute novamente com um novo rotor.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_CUVETTE_MARK			,	{	"The analyzer was unable to detect the rotor. If symptom continues, contact Technical Service.\n",												// English
											"L'analyseur n'est pas parvenu à détecter le rotor. En cas de persistance des symptômes, contacter le service technique.\n",					// French
											"Das Analysegerät konnte den Rotor nicht finden. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",					// German
											"El analizador no fue capaz de detectar el rotor. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"L'analizzatore non è riuscito a individuare il rotore. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"O analisador não consegue detetar o rotor. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",								// Portuguese
											"O analisador não foi capaz de detectar o rotor. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_EXPIRED_ROTOR			,	{	"The rotor has expired and should not be used. Contact Technical Service for further details.\n",												// English
											"La date de péremption du rotor est dépassée. Ne pas l'utiliser. Pour obtenir plus d'informations, contacter le service technique.\n",			// French
											"Der Rotor ist abgelaufen und sollte ersetzt werden. Wenden Sie sich an den technischen Service.\n",											// German
											"El rotor ha caducado y no debería utilizarse. Póngase en contacto con el servicio técnico.\n",													// Spanish
											"Il rotore è scaduto e non dovrebbe essere usato. Per ulteriore dettagli contattare il servizio tecnico.\n",									// Italian
											"O rotor expirou e não deveria ser utilizado. Para obter mais informações, contacte o serviço de Assistência Técnica.\n",						// Portuguese
											"O rotor expirou e não deve ser usado. Entre em contato com o Serviço Técnico para obter detalhes adicionais.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_DILUENT	,	{	"Insufficient reagents exist in the rotor to complete the run. Rerun sample with new rotor.\n",													// English
											"La quantité de réactifs dans le rotor est insuffisante pour exécuter la procédure. Réexécuter l'échantillon à l'aide du nouveau rotor.\n",		// French
											"In dem Rotor befinden sich nicht ausreichend Reagenzien, um den Vorgang abzuschließen. Wiederholen Sie die Probe mit einem neuen Rotor.\n",	// German
											"No hay suficientes reactivos en el rotor para completar su uso. Vuelva a utilizar la muestra con un nuevo rotor.\n",							// Spanish
											"Nel rotore non vi sono sufficienti reagenti per completare l'operazione. Riavviare il campione con un nuovo rotore.\n",						// Italian
											"Não existem reagentes suficientes no rotor para concluir a execução. Executar novamente a amostra utilizando um rotor novo.\n",				// Portuguese
											"Existem reagentes insuficientes no rotor para concluir a execução. Execute novamente a amostra com o novo rotor.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_INSUFFICIENT_SAMPLE		,	{	"The rotor has not received enough sample to function properly. Rerun using a new rotor.\n",													// English
											"Le rotor n'a pas reçu suffisamment d'échantillon pour fonctionner correctement. Réexécuter avec un nouveau rotor.\n",							// French
											"Der Rotor enthält keine ausreichende Probenmenge für eine ordnungsgemäße Funktion. Mit einem neuen Rotor wiederholen.\n",						// German
											"El rotor no ha recibido suficiente muestra para funcionar correctamente. Úselo con un nuevo rotor.\n",											// Spanish
											"Il rotore non ha ricevuto campione sufficiente per funzionare nel modo corretto. Riavviare utilizzando un nuovo rotore.\n",					// Italian
											"O rotor não recebeu amostra suficiente para funcionar adequadamente. Executar novamente utilizando um rotor novo.\n",							// Portuguese
											"O rotor não recebeu amostra suficiente para funcionar corretamente. Execute novamente usando um novo rotor.\n"									// Portuguese (Brazil)
										} 
	},
	{ EISTR_IQC						,	{	"The analyzer Intelligent Quality Control (iQC) reagent dye beads differ too much from the expected values. Rerun using a new rotor.\n",												// English
											"Les billes de teinture du réactif Intelligent Quality Control (iQC) divergent trop des valeurs prévues. Réexécuter avec un nouveau rotor.\n",											// French
											"Die Intelligent Quality Control (iQC) Reagenz-Farbstoff-Beads des Analysegeräts weichen zu stark von den erwarteten Werten ab. Mit einem neuen Rotor wiederholen.\n",					// German
											"Las cuentas de tinte de reactivo Intelligent Quality Control (iQC) del analizador son muy distintas a los valores esperados. Úselo con un nuevo rotor.\n",								// Spanish
											"I granuli di colore reagente Intelligent Quality Control (iQC) dell'analizzatore sono troppo diversi dai valori previsti. Riavviare utilizzando un nuovo rotore.\n",					// Italian
											"As esferas de reagente corante do Controlo de qualidade inteligente (iQC) do analisador são muito diferentes dos valores esperados. Executar novamente utilizando um rotor novo.\n",	// Portuguese
											"As esferas do corante reagente de Controle de qualidade inteligente (iQC) do analisador diferem muito dos valores esperados. Execute novamente usando um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_REAGENT_DISTRIBUTION	,	{	"Reagent beads have not dissolved as expected. Rerun sample with a new rotor.\n",											// English
											"Les billes de réactif ne se sont pas dissoutes comme prévu. Réexécuter l'échantillon à l'aide d'un nouveau rotor.\n",		// French
											"Reagenz-Beads wurden nicht erwartungsgemäß aufgelöst. Wiederholen Sie die Probe mit einem neuen Rotor.\n",					// German
											"Las cuentas de reactivo no se han disuelto como se esperaba. Vuelva a utilizar la muestra con un nuevo rotor.\n",			// Spanish
											"I granuli di reagente non si sono dissolti come previsto. Riavviare il campione con un nuovo rotore.\n",					// Italian
											"As esferas de reagente não se dissolveram conforme esperado. Executar novamente a amostra utilizando um rotor novo.\n",	// Portuguese
											"As esferas de reagente não se dissolveram conforme esperado. Execute novamente a amostra com um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_ROTOR_DETECTED			,	{	"A rotor may be in the analyzer and may prevent the analyzer from starting normally. Press Continue to remove the rotor.\n",								// English
											"Il peut y avoir dans l'analyseur un rotor susceptible de l'empêcher de démarrer normalement. Appuyer sur Continuer pour retirer le rotor.\n",				// French
											"Ein Rotor kann sich in dem Analysegerät befinden und den Startvorgang des Analysegeräts beeinträchtigen. Zur Entfernung des Rotors drücken Sie Weiter.\n",	// German
											"Puede que haya un rotor en el analizador, lo que impide que el analizador se inicie correctamente. Pulse en Continuar para retirar el rotor.\n",			// Spanish
											"Nell'analizzatore potrebbe essere rimasto un rotore che impedisce un'attivazione normale dell'analizzatore. Premere Continua per rimuovere il rotore.\n", 	// Italian
											"Pode estar um rotor no analisador, o que pode impedir que o analisador inicie corretamente. Prima Continuar para remover o rotor.\n",						// Portuguese
											"Um rotor pode estar no analisador e impedir que o analisador inicie normalmente. Pressione Continuar para remover o rotor.\n"								// Portuguese (Brazil)
										} 
	},
	{ EISTR_RQC						,	{	"The Reagent QC beads recovered below the limit. The rotor may not have been stored correctly. Rerun with a new rotor.\n",													// English
											"La récupération des billes de réactif QC est inférieure à la limite. Il se peut que le rotor n'ait pas été rangé correctement. Réexécuter avec un nouveau rotor.\n",		// French
											"Die wiederhergestellten Reagenz-QC-Beads liegen unter dem Grenzwert. Der Rotor wurde möglicherweise nicht ordnungsgemäß gelagert. Mit einem neuen Rotor wiederholen.\n",	// German
											"Cuentas QC de reactivo recuperadas por debajo del límite. Puede que el rotor no se haya guardado correctamente. Debe usarse con un nuevo rotor.\n",						// Spanish
											"I granuli QC di reagente si sono ristabiliti sotto al limite. Il rotore potrebbe non essere stato conservato correttamente. Riavviare con un nuovo rotore.\n",			   	// Italian
											"As esferas de reagente de CQ foram recuperadas abaixo do limite. O rotor pode não ter sido guardado corretamente. Executar novamente utilizando um rotor novo.\n",			// Portuguese
											"As esferas de Reagente de QC se recuperaram abaixo do limite. O rotor pode não ter sido armazenado corretamente. Execute novamente com um novo rotor.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_BLANK			,	{	"The sample blank bead appears to be turbid. This can be due to sample quality. Confirm sample integrity and rerun with a new rotor.\n",														// English
											"La bille vierge de l'échantillon semble trouble, ce qui peut venir de la qualité de l'échantillon. Vérifier l'intégrité de l'échantillon et réexécuter avec un nouveau rotor.\n",				// French
											"Die Leer-Beadprobe ist trübe. Dies kann auf die Probenqualität zurückgeführt werden. Bestätigen Sie die Unversehrtheit der Probe und wiederholen Sie den Vorgang mit einem neuen Rotor.\n",	// German
											"La cuenta virgen de la muestra parece estar turbia. Esto puede deberse a la calidad de la muestra. Confirme la integridad de la muestra y vuelva a utilizar el nuevo rotor.\n",				// Spanish
											"I granuli campione vuoti sembrano torbidi. Ciò può essere causato dalla qualità del campione. Confermare l'integrità del campione e riavviare con un nuovo rotore.\n",							// Italian
											"A esfera de amostra de branco parece estar turva. Isto pode ser devido à qualidade da amostra. Confirme a integridade da amostra e execute novamente utilizando um rotor novo.\n",				// Portuguese
											"A esfera em branco da amostra parece estar turva. Isso pode ser decorrente da qualidade da amostra. Confirme a integridade da amostra e execute novamente com um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_DISTRIBUTION		,	{	"Sample has not reached the chemistries. Rerun with a new rotor.\n",									// English
											"L'échantillon n'est pas parvenu jusqu'aux produits chimiques. Réexécuter avec un nouveau rotor.\n",	// French
											"Die Probe hat nicht die Chemie erreicht. Mit einem neuen Rotor wiederholen.\n",						// German
											"La muestra no ha alcanzado los compuestos químicos. Debe usarse con un nuevo rotor.\n",				// Spanish
											"Il campione non ha raggiunto le sostanze chimiche. Riavviare con un nuovo rotore.\n",					// Italian
											"A amostra não atingiu as substâncias químicas. Executar novamente utilizando um rotor novo.\n",		// Portuguese
											"A amostra não atingiu as substâncias químicas. Execute novamente com um novo rotor.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_MIX				,	{	"The sample has not mixed with reagent materials sufficiently. Rerun with a new rotor.\n",										// English
											"L'échantillon ne s'est pas suffisamment mélangé avec les matières de réactifs. Réexécuter avec un nouveau rotor.\n",			// French
											"Die Probe hat sich nicht ausreichend mit Reagenzmaterialien vermischt. Mit einem neuen Rotor wiederholen.\n",					// German
											"La muestra no se ha mezclado suficientemente con los materiales reactivos. Debe usarse con un nuevo rotor.\n",					// Spanish
											"Il campione non si è mescolato a sufficienza con le sostanze reagenti. Riavviare con un nuovo rotore.\n",						// Italian
											"A amostra não se misturou suficientemente com os materiais dos reagentes. Executar novamente utilizando um rotor novo.\n",		// Portuguese
											"A amostra não foi misturada com os materiais reagentes suficientemente. Execute novamente com um novo rotor.\n"				// Portuguese (Brazil)
										} 
	},
	{ EISTR_SAMPLE_QUALITY			,	{	"The sample may have one or more physical interferents. Confirm the sample integrity and rerun rotor.\n",										// English
											"Il se peut que l'échantillon présente un ou plusieurs éléments interférents. Vérifier l'intégrité de l'échantillon et réexécuter le rotor.\n",	// French
											"Die Probe hat möglicherweise eine oder mehrere physische störende Bestandteile. Unversehrtheit der Probe bestätigen und Rotor neu starten.\n",	// German
											"Puede que la muestra tenga una o varias interferencias físicas. Confirme la integridad de la muestra y vuelva a usar el rotor.\n",				// Spanish
											"Il campione potrebbe avere uno o più fattori di interferenza fisica. Confermare l'integrità del campione e riavviare il rotore.\n",			// Italian
											"A amostra pode ter uma ou várias interferências físicas. Confirme a integridade da amostra e execute novamente o rotor.\n",					// Portuguese
											"A amostra pode ter um ou mais interferentes físicos. Confirme a integridade da amostra e execute novamente o rotor.\n"							// Portuguese (Brazil)
										} 
	},
	{ EISTR_SOFTWARE_UPDATE			,	{	"The rotor inserted requires a software update to operate correctly. Contact Technical Service for further assistance.\n",														// English
											"Le rotor inséré nécessite une mise à jour de logiciel pour pouvoir fonctionner correctement. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											"Der eingesetzte Rotor erfordert ein Software-Update für einen ordnungsgemäßen Betrieb. Wenden Sie sich an den technischen Service.\n",											// German
											"El rotor insertado necesita la actualización del software para operar correctamente. Póngase en contacto con el servicio técnico.\n",											// Spanish
											"Il rotore inserito richiede un aggiornamento del software per funzionare correttamente. Per ulteriore assistenza contattare il servizio tecnico.\n",							// Italian
											"O rotor inserido necessita de uma atualização do software para funcionar adequadamente. Contacte o Serviço de Assistência Técnica para obter assistência.\n",					// Portuguese
											"O rotor inserido requer uma atualização de software para funcionar corretamente. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"					// Portuguese (Brazil)
										} 
	},
	{ EISTR_STARTING_ABSORBANCE		,	{	"Starting absorbance of the sample blank bead is outside the limit, possibly due to an exogenous substance or distribution error. Rerun with new rotor.\n",																		// English
											"L'absorbance de départ de la bille vierge de l'échantillon n'entre pas dans les limites prescrites, éventuellement du fait d'une substance exogène ou d'une erreur de distribution. Réexécuter à l'aide du nouveau rotor.\n",	// French
											"Die Startabsorbierung der Leer-Beadprobe befindet sich möglicherweise aufgrund einer exogenen Substanz oder eines Verteilungsfehlers außerhalb des Grenzwerts. Mit einem neuen Rotor wiederholen.\n",							// German
											"La absorbancia de inicio de la cuenta virgen de muestra está fuera del límite, probablemente debido a una sustancia exógena o a un error de distribución. Usar con un nuevo rotor.\n",											// Spanish
											"L'assorbenza iniziale dei granuli campione vuoti non rientra nei limiti, forse a causa di una sostanza esogena od a un errore di distribuzione. Riavviare il campione con un nuovo rotore.\n",									// Italian
											"A absorvância inicial da esfera de amostra de branco está fora do limite, possivelmente devido a uma sustância exógena ou erro de distribuição. Executar novamente utilizando um rotor novo.\n",								// Portuguese
											"A absorção inicial da esfera em branco da amostra está fora do limite, possivelmente devido a uma substância exógena ou um erro de distribuição. Execute novamente com um novo rotor.\n"										// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_SENSOR				,	{	"The rotor temperature sensor is not operating. Contact Technical Service for further assistance.\n",										// English
											"Le détecteur de température du rotor ne fonctionne pas. Pour obtenir une assistance complémentaire, contacter le service technique.\n",	// French
											"Der Temperatursensor des Rotors ist außer Betrieb. Wenden Sie sich an den technischen Service.\n",											// German
											"El sensor de temperatura del rotor no está funcionando. Póngase en contacto con el servicio técnico.\n",									// Spanish
											"Il sensore di temperatura del rotore non è in funzione. Per ulteriore assistenza contattare il servizio tecnico.\n",						// Italian
											"O sensor de temperatura do rotor não está a funcionar. Contacte o Serviço de Assistência Técnica para obter assistência.\n",				// Portuguese
											"O sensor de temperatura do rotor não está funcionando. Entre em contato com o Serviço Técnico para obter assistência adicional.\n"			// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMP_VARIATION			,	{	"The temperature deviated more than one degree during the run. The accuracy of certain chemistries may be affected. Rerun with new rotor.\n",															// English
											"La température a présenté un écart de plus d'un degré au cours de l'exécution. La précision de certains produits chimiques pourra en être affectée. Réexécuter à l'aide du nouveau rotor.\n",			// French
											"Der Temperatur wich um mehr als ein Grad während des Betriebs ab. Die Genauigkeit bestimmter Chemie kann beeinträchtigt worden sein. Mit einem neuen Rotor wiederholen.\n",							// German
											"La temperatura cambió en más de un grado durante el análisis. Puede que afecte a la precisión del análisis de ciertas sustancias químicas. Utilice un nuevo rotor.\n",									// Spanish
											"Durante il funzionamento la temperatura è variata di più di un grado. La precisione di alcune sostanze chimiche potrebbe esserne stata influenzata. Riavviare il campione con un nuovo rotore.\n",		// Italian
											"A alteração da temperatura foi superior a um grau durante a execução. A precisão de determinadas sustâncias químicas pode ser afetada. Executar novamente utilizando um rotor novo.\n",				// Portuguese
											"A temperatura desviou mais de um grau durante a execução. A precisão de certas substâncias químicas pode estar afetada. Execute novamente com um novo rotor.\n"										// Portuguese (Brazil)
										} 
	},
	{ EISTR_TEMPERATURE				,	{	"The temperature of the rotor either varied too much or was not in the proper range during the run. If symptom continues, contact Technical Service.\n",																			// English
											"La température du rotor a soit varié dans des proportions trop importantes, soit ne s'inscrit pas dans la fourchette prescrite en cours d'exécution. En cas de persistance des symptômes, contacter le service technique.\n",		// French
											"Die Rotortemperatur variierte zu stark oder befand sich während des Betriebs außerhalb des optimalen Bereichs. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",											// German
											"La temperatura del rotor varió mucho o no estaba en el intervalo adecuado durante el análisis. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",											// Spanish
											"Durante il funzionamento la temperatura del rotore ha subito variazioni eccessive oppure non era nel range adatto. Se i sintomi continuano contattare il servizio tecnico.\n",														// Italian
											"A temperatura do rotor variou muito ou não esteve dentro do intervalo adequado durante a análise. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",															// Portuguese
											"A temperatura do rotor variou muito ou não permaneceu no intervalo adequado durante a execução. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"																	// Portuguese (Brazil)
										} 
	},
	{ EISTR_WRONG_ROTOR				,	{	"The rotor inserted is not intended for this device. If symptom continues, contact Technical Service.\n",										// English
											"Le rotor inséré n'est pas prévu pour cet appareil. En cas de persistance des symptômes, contacter le service technique.\n",					// French
											"Der eingesetzte Rotor ist für dieses Gerät nicht geeignet. Bei Fortsetzung der Störung wenden Sie sich an den technischen Service.\n",			// German
											"El rotor insertado no es apto para este aparato. Si el mismo problema siguiera ocurriendo, póngase en contacto con el servicio técnico.\n",	// Spanish
											"Il rotore inserito non è adatto a questo strumento. Se i sintomi continuano contattare il servizio tecnico.\n",								// Italian
											"O rotor inserido não é adequado para este dispositivo. Se o problema persistir, contacte o serviço de Assistência Técnica.\n",					// Portuguese
											"O rotor inserido não é destinado a este dispositivo. Se o sintoma persistir, entre em contato com o Serviço Técnico.\n"						// Portuguese (Brazil)
										} 
	},
	
// Results (display and print)				ENGLISH,				FRENCH,					GERMAN,					SPANISH,					ITALIAN, 					PORTUGUESE,						PORTUGUESE (BRAZIL)
	{ RSTR_ROTOR_LOT_NUM			,	{	"Rotor Lot Number:",	"N° de lot rotor :",	"Rotor-Losnr.:",		"No. de lote del rotor:",	"Numero lotto rotore:",		"Número lote rotor:",			"Núm. de lote do rotor:"	}	},
	{ RSTR_DOV						,	{	"DOV:",					"DOV :",				"LI:",					"Úl. vac.:",				"DV:",						"DOV:",							"Data vacinação:"	}	},
	{ RSTR_KENNEL_NUM				,	{	"Kennel Number:",		"Numéro chenil :",		"Zwingernr.:",			"No. de caseta:",			"Numero Kennel:",			"Número de Kennel:",			"Número do canil:"	}	},
	{ RSTR_OWNER_ID					,	{	"Owner ID:",			"ID prop.:",			"Besitzer-ID:",			"Id. del dueño:",			"ID propr.:",				"ID do propr.:",				"ID propriet.:"	}	},
	
// Genders									ENGLISH,	FRENCH,		GERMAN,			SPANISH,		ITALIAN, 	   	PORTUGUESE,		PORTUGUESE (BRAZIL)
	{ GSTR_FEMALE					,	{	"Female",	"Femelle",	"Weiblich",		"Hembra",		"Femmina",		"Fêmea",		"Sexo feminino"	}	},
	{ GSTR_MALE						,	{	"Male",		"Mâle",		"Männlich",		"Macho",		"Maschio",		"Macho",		"Sexo masculino"	}	},

// Species									ENGLISH,				FRENCH,					GERMAN,					SPANISH,				ITALIAN, 				PORTUGUESE,				PORTUGUESE (BRAZIL)	
	{ SSTR_OTHER					,	{	"Other",				"Autre",				"Sonstige",				"Otro",					"Altro",				"Outro",				"Outros"	}	},
	{ SSTR_SPECIES					,	{	"Species",				"Espèces",				"Arten",				"Especie",				"Specie",				"Espécies",				"Espécies"	}	},
	
	{ SSTR_CANINES					,	{	"Canines",				"Canins",				"Hunde",				"Caninos",				"Canini",				"Caninas",				"Caninos"	}	},
	{ SSTR_FELINES					,	{	"Felines",				"Félins",				"Katzen",				"Felinos",				"Felini",				"Felinas",				"Felinos"	}	},
	{ SSTR_EQUINES					,	{	"Equines",				"Chevalins",			"Pferde",				"Equinos",				"Equini",				"Equinas",				"Equinos"	}	},
	{ SSTR_BOVINES					,	{	"Bovines",				"Bovins",				"Rinder",				"Bovinos",				"Bovini",				"Bovinas",				"Bovinos"	}	},
	{ SSTR_AVIANS					,	{	"Avians",				"Aviaires",				"Vögel",				"Aves",					"Aviari",				"Aviárias",				"Aves"	}	},
	{ SSTR_REPTILES_AMPHIBIANS		,	{	"Reptiles/Amphibians",	"Reptiles/Amphibies",	"Reptilien/Amphibien",	"Reptiles/anfibios",	"Rettili/Anfibi",		"Répteis/Anfibios",		"Répteis/anfíbios"	}	},
	{ SSTR_SMALL_MAMMALS			,	{	"Small Mammals",		"Petits mammifères",	"Kleinsäugetiere",		"Pequeños mamíferos",	"Piccoli mammiferi",	"Pequenos mamíferos",	"Mamíferos pequenos"	}	},
	{ SSTR_LARGE_MAMMALS			,	{	"Large Mammals",		"Grands mammifères",	"Großsäugetiere",		"Grandes mamíferos",	"Grandi mammiferi",		"Grandes mamíferos",	"Mamíferos grandes"	}	},

	// Canines
	{ SSTR_DOG						,	{	"Dog",					"Chien",				"Hund",					"Perro",				"Cane",					"Cão",					"Cão"	}	},
	{ SSTR_COYOTE					,	{	"Coyote",				"Coyote",				"Kojote",				"Coyote",				"Coyote",				"Coiote",				"Coiote"	}	},
	{ SSTR_DINGO					,	{	"Dingo",				"Dingo",				"Dingo",				"Dingo",				"Dingo",				"Dingo",				"Cão selvagem"	}	},
	{ SSTR_FOX						,	{	"Fox",					"Renard",				"Fuchs",				"Zorro",				"Volpe",				"Raposa",				"Raposa"	}	},
	{ SSTR_JACKAL					,	{	"Jackal",				"Chacal",				"Schakal",				"Chacal",				"Sciacallo",			"Chacal",				"Chacal"	}	},
	{ SSTR_WOLF						,	{	"Wolf",					"Loup",					"Wolf",					"Lobo",					"Lupo",					"Lobo",					"Lobo"	}	},

	// Felines
	{ SSTR_CAT						,	{	"Cat",					"Chat",					"Katze",				"Gato",					"Gatto",				"Gato",					"Gato"	}	},
	{ SSTR_BOBCAT					,	{	"Bobcat",				"Lynx roux",			"Rotluchs",				"Lince rojo",			"Lince rossa",			"Lince vermelho",		"Lince"	}	},
	{ SSTR_CHEETAH					,	{	"Cheetah",				"Guépard",				"Gepard",				"Guepardo",				"Ghepardo",				"Chita",				"Gato selvagem"	}	},
	{ SSTR_JAGUAR					,	{	"Jaguar",				"Jaguar",				"Jaguar",				"Jaguar",				"Giaguaro",				"Jaguar",				"Jaguar"	}	},
	{ SSTR_LEOPARD					,	{	"Leopard",				"Léopard",				"Leopard",				"Leopardo",				"Leopardo",				"Leopardo",				"Leopardo"	}	},
	{ SSTR_LION						,	{	"Lion",					"Lion",					"Löwe",					"León",					"Leone",				"Leão",					"Leão"	}	},
	{ SSTR_LYNX						,	{	"Lynx",					"Lynx",					"Luchs",				"Lince",				"Lince",				"Lince",				"Gato selvagem"	}	},
	{ SSTR_PANTHER					,	{	"Panther",				"Panthère",				"Panther",				"Pantera",				"Pantera",				"Pantera",				"Pantera"	}	},
	{ SSTR_PUMA						,	{	"Puma",					"Puma",					"Puma",					"Puma",					"Puma",					"Puma",					"Puma"	}	},
	{ SSTR_TIGER					,	{	"Tiger",				"Tigre",				"Tiger",				"Tigre",				"Tigre",				"Tigre",				"Tigre"	}	},
	
	// Equines
	{ SSTR_HORSE					,	{	"Horse",				"Cheval",				"Pferd",				"Caballo",				"Cavallo",				"Cavalo",				"Cavalo"	}	},
	{ SSTR_DONKEY					,	{	"Donkey",				"Ane",					"Esel",					"Burro",				"Asino",				"Burro",				"Asno"	}	},
	{ SSTR_PONY						,	{	"Pony",					"Poney",				"Pony",					"Poni",					"Pony",					"Pónei",				"Pônei"	}	},
	{ SSTR_ZEBRA					,	{	"Zebra",				"Zèbre",				"Zebra",				"Cebra",				"Zebra",				"Zebra",				"Zebra"	}	},
	
	// Bovines
	{ SSTR_COW						,	{	"Cow",					"Vache",				"Kuh",					"Vaca",					"Mucca",				"Vaca",					"Vaca"	}	},
	{ SSTR_WATER_BUFFALO			,	{	"Water Buffalo",		"Buffle (d'Asie)",		"Wasserbüffel",			"Búfalo de agua",		"Bufalo d'acqua",		"Búfalo de água",		"Búfalo d´água"	}	},
	{ SSTR_STEER					,	{	"Steer",				"Bouvillon",			"Stier",				"Novillo",				"Manzo",				"Boi",					"Novilho"	}	},
	{ SSTR_OX						,	{	"Ox",					"Boeuf",				"Ochse",				"Buey",					"Bue",					"Boi",					"Boi"	}	},
	{ SSTR_BULL						,	{	"Bull",					"Taureau",				"Bulle",				"Toro",					"Toro",					"Touro",				"Touro"	}	},
	{ SSTR_BUFFALO					,	{	"Buffalo",				"Buffle",				"Büffel",				"Búfalo",				"Bufalo",				"Búfalo",				"Búfalo"	}	},
	{ SSTR_BISON					,	{	"Bison",				"Bison",				"Bison",				"Bisonte",				"Bisonte",				"Bisonte",				"Bisão"	}	},
	
	// Avians
	{ SSTR_BIRD						,	{	"Bird",					"Oiseau",				"Vogel",				"Pájaro",				"Uccello",				"Pássaro",				"Pássaro"	}	},
	{ SSTR_AFRICAN_GRAY				,	{	"African Gray",			"Perroquet gris",		"Afrik.Graupapagei",	"Rucio africano",		"Pap. grigio afr.",		"Papagaio africano",	"Papagaio africano"	}	},
	{ SSTR_AMAZON					,	{	"Amazon",				"Amazone",				"Amazone",				"Amazona",				"Amazzoni",				"Amazonas",				"Amazona"	}	},
	{ SSTR_CHICKEN					,	{	"Chicken",				"Poulet",				"Huhn",					"Pollo",				"Pollo",				"Galinha",				"Frango"	}	},
	{ SSTR_COCKATIEL				,	{	"Cockatiel",			"Calopsitte élégte",	"Nymphensittich",		"Carolina",				"Cockatiel",			"Caturra",				"Calopsita"	}	},
	{ SSTR_COCKATOO					,	{	"Cockatoo",				"Cacatoès",				"Kakadu",				"Cacatúa",				"Cacatua",				"Catatua",				"Cacatua"	}	},
	{ SSTR_CONURE					,	{	"Conure",				"Conure",				"Sittiche",				"Lorito americano",		"Conuro",				"Conure",				"Jandaia"	}	},
	{ SSTR_DOVE						,	{	"Dove",					"Colombe",				"Taube",				"Paloma",				"Colombo",				"Pomba",				"Pombo"	}	},
	{ SSTR_DUCK						,	{	"Duck",					"Canard",				"Ente",					"Pato",					"Anatra",				"Pato",					"Pato"	}	},
	{ SSTR_FLAMINGO					,	{	"Flamingo",				"Flamand",				"Flamingo",				"Flamenco",				"Fenicottero",			"Flamingo",				"Flamingo"	}	},
	{ SSTR_GOOSE					,	{	"Goose",				"Oie",					"Gans",					"Ganso",				"Oca",					"Ganso",				"Ganso"	}	},
	{ SSTR_LOVEBIRD					,	{	"Lovebird",				"Inséparable",			"Erdbeerkopf",			"Inseparable",			"Inseparabile",			"Agapornis",			"Pássaro-do-amor"	}	},
	{ SSTR_MACAW					,	{	"Macaw",				"Ara",					"Ara",					"Guacamayo",			"Ara",					"Arara",				"Arara"	}	},
	{ SSTR_MYNAH					,	{	"Mynah",				"Mainate",				"Beo",					"Grácula religiosa",	"Storno triste",		"Mainá",				"Mainá"	}	},
	{ SSTR_OSTRICH					,	{	"Ostrich",				"Autruche",				"Strauß",				"Avestruz",				"Struzzo",				"Avestruz",				"Avestruz"	}	},
	{ SSTR_PARAKEET					,	{	"Parakeet",				"Starique perroq.",		"Wellensittich",		"Periquito",			"Parrocchetto",			"Periquito",			"Periquito"	}	},
	{ SSTR_PARROT					,	{	"Parrot",				"Perroquet",			"Papagei",				"Loro",					"Pappagallo",			"Papagaio",				"Papagaio"	}	},
	{ SSTR_TOUCAN					,	{	"Toucan",				"Toucan",				"Tukan",				"Tucán",				"Tucano",				"Tucano",				"Tucano"	}	},
	
	// Reptiles/amphibians
	{ SSTR_REPTILE					,	{	"Reptile",				"Reptile",				"Reptil",				"Reptil",				"Rettile",				"Réptil",				"Réptil"	}	},
	{ SSTR_ALLIGATOR				,	{	"Alligator",			"Alligator",			"Alligator",			"Caimán",				"Alligatore",			"Aligátor",				"Jacaré"	}	},
	{ SSTR_AMPHIBIAN				,	{	"Amphibian",			"Amphibie",				"Amphibie",				"Anfibio",				"Anfibio",				"Anfíbio",				"Anfíbio"	}	},
	{ SSTR_BEARDED_DRAGON			,	{	"Bearded Dragon",		"Lézard à barbe",		"Bartagame",			"Dragón barbado",		"Pogona vitticeps",		"Dragão barbudo",		"Dragão farpado"	}	},
	{ SSTR_BOA_CONSTRICTOR			,	{	"Boa Constrictor",		"Boa constrictor",		"Boa constrictor",		"Boa constrictor",		"Boa Costrittore",		"Jibóia",				"Jiboia"	}	},
	{ SSTR_CHAMELEON				,	{	"Chameleon",			"Caméléon",				"Chamäleon",			"Camaleón",				"Camaleonte",			"Camaleão",				"Camaleão"	}	},
	{ SSTR_CROCODILE				,	{	"Crocodile",			"Crocodile",			"Krokodil",				"Cocodrilo",			"Coccodrillo",			"Crocodilo",			"Crocodilo"	}	},
	{ SSTR_FROG						,	{	"Frog",					"Grenouille",			"Frosch",				"Rana",					"Rana",					"Rã",					"Rã"	}	},
	{ SSTR_GECKO					,	{	"Gecko",				"Gecko",				"Gecko",				"Lagartija",			"Geco",					"Osga",					"Lagartixa"	}	},
	{ SSTR_IGUANA					,	{	"Iguana",				"Iguane",				"Leguan",				"Iguana",				"Iguana",				"Iguana",				"Iguana"	}	},
	{ SSTR_LIZARD					,	{	"Lizard",				"Lézard",				"Eidechse",				"Lagarto",				"Lucertola",			"Lagarto",				"Lagarto"	}	},
	{ SSTR_MONITOR					,	{	"Monitor",				"Varan",				"Waran",				"Varano",				"Varano",				"Monitor",				"Lagarto monitor"	}	},
	{ SSTR_PYTHON					,	{	"Python",				"Python",				"Python",				"Pitón",				"Pitone",				"Pitão",				"Píton"	}	},
	{ SSTR_SALAMANDER				,	{	"Salamander",			"Salamandre",			"Salamander",			"Salamandra",			"Salamandra",			"Salamandra",			"Salamandra"	}	},
	{ SSTR_SNAKE					,	{	"Snake",				"Serpent",				"Schlange",				"Serpiente",			"Serpente",				"Cobra",				"Cobra"	}	},
	{ SSTR_TOAD						,	{	"Toad",					"Crapaud",				"Kröte",				"Sapo",					"Rospo",				"Sapo",					"Sapo"	}	},
	{ SSTR_TORTOISE					,	{	"Tortoise",				"Tortue",				"Schildkröte",			"Tortuga",				"Testuggine",			"Testudinidae",			"Cágado"	}	},
	{ SSTR_TURTLE					,	{	"Turtle",				"Tortue marine",		"Wasserschildkröte",	"Tortuga marina",		"Tartaruga",			"Tartaruga",			"Tartaruga"	}	},
	
	// Small mammals
	{ SSTR_SMALL_MAMMAL				,	{	"Small Mammal",			"Petit mammifère",		"Kleinsäugetier",		"Mamífero pequeño",		"Piccolo mammifero",	"Pequeno mamífero",		"Mamífero pequeno"	}	},
	{ SSTR_BAT						,	{	"Bat",					"Chauve-souris",		"Fledermaus",			"Murciélago",			"Pipistrello",			"Morcego",				"Morcego"	}	},
	{ SSTR_CHINCHILLA				,	{	"Chinchilla",			"Chinchilla",			"Chinchilla",			"Chinchilla",			"Cincillà",				"Chinchila",			"Chinchila"	}	},
	{ SSTR_FERRET					,	{	"Ferret",				"Furet",				"Frettchen",			"Hurón",				"Furetto",				"Furão",				"Furão"	}	},
	{ SSTR_GERBIL					,	{	"Gerbil",				"Gerbille",				"Rennmaus",				"Jerbo",				"Gerbillo",				"Gerbo",				"Gerbo"	}	},
	{ SSTR_GUINEA_PIG				,	{	"Guinea Pig",			"Cochon d'Inde",		"Meerschweinchen",		"Cobaya",				"Cavia",				"Cobaia",				"Cobaia"	}	},
	{ SSTR_HAMSTER					,	{	"Hamster",				"Hamster",				"Hamster",				"Hámster",				"Criceto",				"Hamster",				"Hamster"	}	},
	{ SSTR_HEDGEHOG					,	{	"Hedgehog",				"Hérisson",				"Igel",					"Erizo",				"Riccio",				"Ouriço",				"Ouriço"	}	},
	{ SSTR_LEMUR					,	{	"Lemur",				"Lémurien",				"Lemur",				"Lémur",				"Lemure",				"Lêmure",				"Lêmure"	}	},
	{ SSTR_MINK						,	{	"Mink",					"Vison",				"Nerz",					"Visón",				"Visone",				"Marta",				"Marta"	}	},
	{ SSTR_MOUSE					,	{	"Mouse",				"Souris",				"Maus",					"Ratón",				"Topo",					"Rato",					"Camundongo"	}	},
	{ SSTR_OPOSSUM					,	{	"Opossum",				"Opossum",				"Opossum",				"Zarigüeya",			"Opossum",				"Marsupial",			"Gambá"	}	},
	{ SSTR_PRAIRIE_DOG				,	{	"Prairie Dog",			"Chien de prairie" ,	"Präriehund",			"Perro de Pradera",		"Cane prateria",		"Cão de pradaria",		"Cão dos prados"	}	},
	{ SSTR_RABBIT					,	{	"Rabbit",				"Lapin",				"Hase",					"Conejo",				"Coniglio",				"Coelho",				"Coelho"	}	},
	{ SSTR_RACCOON					,	{	"Raccoon",				"Raton laveur",			"Waschbär",				"Mapache",				"Procione",				"Guaxinim",				"Guaxinim"	}	},
	{ SSTR_RAT						,	{	"Rat",					"Rat",					"Ratte",				"Rata",					"Ratto",				"Ratazana",				"Rato"	}	},
	{ SSTR_SKUNK					,	{	"Skunk",				"Moufette",				"Stinktier",			"Mofeta",				"Moffetta",				"Gambá",				"Doninha"	}	},
	{ SSTR_SQUIRREL					,	{	"Squirrel",				"Écureuil",				"Eichhörnchen",			"Ardilla",				"Scoiattolo",			"Esquilo",				"Esquilo"	}	},
	
	// Large mammals
	{ SSTR_LARGE_MAMMAL				,	{	"Large Mammal",			"Grand mammifère",		"Großsäugetier",		"Grandes mamíferos",	"Grande mammifero",		"Grande mamífero",		"Mamífero grande"	}	},
	{ SSTR_ANTELOPE					,	{	"Antelope",				"Antilope",				"Antilope",				"Antílope",				"Antilope",				"Antílope",				"Antílope"	}	},
	{ SSTR_APE						,	{	"Ape",					"Grand singe",			"Menschenaffe",			"Simio",				"Scimmia",				"Símio",				"Símio"	}	},
	{ SSTR_BABOON					,	{	"Baboon",				"Babouin",				"Pavian",				"Babuino",				"Babbuino",				"Babuíno",				"Babuíno"	}	},
	{ SSTR_BEAR						,	{	"Bear",					"Ours",					"Bär",					"Oso",					"Orso",					"Urso",					"Urso"	}	},
	{ SSTR_CAMEL					,	{	"Camel",				"Chameau",				"Kamel",				"Camello",				"Cammello",				"Camelo",				"Camelo"	}	},
	{ SSTR_CHIMPANZEE				,	{	"Chimpanzee",			"Chimpanzé",			"Schimpanse",			"Chimpancé",			"Scimpanzé",			"Chimpanzé",			"Chimpanzé"	}	},
	{ SSTR_DEER						,	{	"Deer",					"Cerf",					"Reh",					"Venado",				"Cervide",				"Veado",				"Veado"	}	},
	{ SSTR_ELEPHANT					,	{	"Elephant",				"Éléphant",				"Elefant",				"Elefante",				"Elefante",				"Elefante",				"Elefante"	}	},
	{ SSTR_GIRAFFE					,	{	"Giraffe",				"Girafe",				"Giraffe",				"Jirafa",				"Giraffa",				"Girafa",				"Girafa"	}	},
	{ SSTR_GOAT						,	{	"Goat",					"Chèvre",				"Ziege",				"Cabra",				"Capra",				"Cabra",				"Bode"	}	},
	{ SSTR_KANGAROO					,	{	"Kangaroo",				"Kangourou",			"Känguru",				"Canguro",				"Canguro",				"Canguru",				"Canguru"	}	},
	{ SSTR_LLAMA					,	{	"Llama",				"Lama",					"Lama",					"Llama",				"Lama",					"Lama",					"Lhama"	}	},
	{ SSTR_MONKEY					,	{	"Monkey",				"Singe",				"Affe",					"Mono",					"Scimmia",				"Macaco",				"Macaco"	}	},
	{ SSTR_PIG						,	{	"Pig",					"Cochon",				"Schwein",				"Cerdo",				"Maiale",				"Porco",				"Porco"	}	},
	{ SSTR_SEA_LION					,	{	"Sea Lion",				"Lion de mer",			"Seelöwe",				"León marino",			"Leone marino",			"Leão-marinho",			"Leão-marinho"	}	},
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
