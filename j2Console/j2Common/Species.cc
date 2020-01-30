#include <windows.h>
#include <string.h>
#include <stdio.h>

#include "Species.h"

// Sample types
typedef struct
{
	SampleType_t	sampleType;
	StringId_t		stringId;
} SpeciesEntry_t;

/*
 * NOTE: Add new speces here and also to the appropriate group in GetGroupList.
 */

SpeciesEntry_t	SPECIES[] =
{
	// Canines
	{ STYPE_DOG				, SSTR_DOG },
	{ STYPE_COYOTE			, SSTR_COYOTE },		
	{ STYPE_DINGO			, SSTR_DINGO },
	{ STYPE_FOX				, SSTR_FOX },
	{ STYPE_JACKAL			, SSTR_JACKAL },
	{ STYPE_WOLF			, SSTR_WOLF },

	// Felines
	{ STYPE_CAT				, SSTR_CAT },
	{ STYPE_BOBCAT			, SSTR_BOBCAT },
	{ STYPE_CHEETAH			, SSTR_CHEETAH },
	{ STYPE_JAGUAR			, SSTR_JAGUAR },
	{ STYPE_LEOPARD			, SSTR_LEOPARD },
	{ STYPE_LION			, SSTR_LION },
	{ STYPE_LYNX			, SSTR_LYNX },
	{ STYPE_PANTHER			, SSTR_PANTHER },
	{ STYPE_PUMA			, SSTR_PUMA },
	{ STYPE_TIGER			, SSTR_TIGER },
	
	// Equines
	{ STYPE_HORSE			, SSTR_HORSE },
	{ STYPE_DONKEY			, SSTR_DONKEY },
	{ STYPE_PONY			, SSTR_PONY },
	{ STYPE_ZEBRA			, SSTR_ZEBRA },
	
	// Bovines
	{ STYPE_COW				, SSTR_COW },
	{ STYPE_BISON			, SSTR_BISON },
	{ STYPE_BUFFALO			, SSTR_BUFFALO },
	{ STYPE_BULL			, SSTR_BULL },
	{ STYPE_WATER_BUFFALO	, SSTR_WATER_BUFFALO },
	{ STYPE_OX				, SSTR_OX },
	{ STYPE_STEER			, SSTR_STEER },
	
	// Avians
	{ STYPE_BIRD			, SSTR_BIRD },
	{ STYPE_AFRICAN_GRAY	, SSTR_AFRICAN_GRAY },
	{ STYPE_AMAZON			, SSTR_AMAZON },
	{ STYPE_CHICKEN			, SSTR_CHICKEN },
	{ STYPE_COCKATIEL		, SSTR_COCKATIEL },
	{ STYPE_COCKATOO		, SSTR_COCKATOO },
	{ STYPE_CONURE			, SSTR_CONURE },
	{ STYPE_DOVE			, SSTR_DOVE },
	{ STYPE_DUCK			, SSTR_DUCK },
	{ STYPE_FLAMINGO		, SSTR_FLAMINGO },
	{ STYPE_GOOSE			, SSTR_GOOSE },
	{ STYPE_LOVEBIRD		, SSTR_LOVEBIRD },
	{ STYPE_MACAW			, SSTR_MACAW },
	{ STYPE_MYNAH			, SSTR_MYNAH },
	{ STYPE_OSTRICH			, SSTR_OSTRICH },
	{ STYPE_PARAKEET		, SSTR_PARAKEET },
	{ STYPE_PARROT			, SSTR_PARROT },
	{ STYPE_TOUCAN			, SSTR_TOUCAN },
	
	// Reptiles/amphibians
	{ STYPE_REPTILE			, SSTR_REPTILE },
	{ STYPE_ALLIGATOR		, SSTR_ALLIGATOR },
	{ STYPE_AMPHIBIAN		, SSTR_AMPHIBIAN },
	{ STYPE_BEARDED_DRAGON	, SSTR_BEARDED_DRAGON },
	{ STYPE_BOA_CONSTRICTOR	, SSTR_BOA_CONSTRICTOR },
	{ STYPE_CHAMELEON		, SSTR_CHAMELEON },
	{ STYPE_CROCODILE		, SSTR_CROCODILE },
	{ STYPE_FROG			, SSTR_FROG },
	{ STYPE_GECKO			, SSTR_GECKO },
	{ STYPE_IGUANA			, SSTR_IGUANA },
	{ STYPE_LIZARD			, SSTR_LIZARD },
	{ STYPE_MONITOR			, SSTR_MONITOR },
	{ STYPE_PYTHON			, SSTR_PYTHON },
	{ STYPE_SALAMANDER		, SSTR_SALAMANDER },
	{ STYPE_SNAKE			, SSTR_SNAKE },
	{ STYPE_TOAD			, SSTR_TOAD },
	{ STYPE_TORTOISE		, SSTR_TORTOISE },
	{ STYPE_TURTLE			, SSTR_TURTLE },
	
	// Small mammals
	{ STYPE_SMALL_MAMMAL	, SSTR_SMALL_MAMMAL },
	{ STYPE_BAT				, SSTR_BAT },
	{ STYPE_CHINCHILLA		, SSTR_CHINCHILLA },
	{ STYPE_FERRET			, SSTR_FERRET },
	{ STYPE_GERBIL			, SSTR_GERBIL },
	{ STYPE_GUINEA_PIG		, SSTR_GUINEA_PIG },
	{ STYPE_HAMSTER			, SSTR_HAMSTER },
	{ STYPE_HEDGEHOG		, SSTR_HEDGEHOG },
	{ STYPE_LEMUR			, SSTR_LEMUR },
	{ STYPE_MINK			, SSTR_MINK },
	{ STYPE_MOUSE			, SSTR_MOUSE },
	{ STYPE_OPOSSUM			, SSTR_OPOSSUM },
	{ STYPE_PRAIRIE_DOG		, SSTR_PRAIRIE_DOG },
	{ STYPE_RACCOON			, SSTR_RACCOON },
	{ STYPE_RAT				, SSTR_RAT },
	{ STYPE_RABBIT			, SSTR_RABBIT },
	{ STYPE_SKUNK			, SSTR_SKUNK },
	{ STYPE_SQUIRREL		, SSTR_SQUIRREL },
	
	// Large mammals
	{ STYPE_LARGE_MAMMAL	, SSTR_LARGE_MAMMAL },
	{ STYPE_ANTELOPE		, SSTR_ANTELOPE },
	{ STYPE_APE				, SSTR_APE },
	{ STYPE_BABOON			, SSTR_BABOON },
	{ STYPE_BEAR			, SSTR_BEAR },
	{ STYPE_CAMEL			, SSTR_CAMEL },
	{ STYPE_CHIMPANZEE		, SSTR_CHIMPANZEE },
	{ STYPE_DEER			, SSTR_DEER },
	{ STYPE_ELEPHANT		, SSTR_ELEPHANT },
	{ STYPE_GIRAFFE			, SSTR_GIRAFFE },
	{ STYPE_GOAT			, SSTR_GOAT },
	{ STYPE_KANGAROO		, SSTR_KANGAROO },
	{ STYPE_LLAMA			, SSTR_LLAMA },
	{ STYPE_MONKEY			, SSTR_MONKEY },
	{ STYPE_PIG				, SSTR_PIG },
	{ STYPE_SEA_LION		, SSTR_SEA_LION },
	{ STYPE_SEAL			, SSTR_SEAL },
	{ STYPE_SHEEP			, SSTR_SHEEP },
	{ STYPE_NONE			, STR_BLANK },
};


Species::
Species()
{
	int i;
	
	memset( speciesNames, 0, sizeof( speciesNames ) );
	
	for ( i = 0; SPECIES[ i ].sampleType != STYPE_NONE; i++ )
	{
		speciesNames[ SPECIES[ i ].sampleType - STYPE_FIRST_NAMED_SPECIES ] = SPECIES[ i ].stringId;
	}
}

#if 0
const char *
Species::
operator[]( SampleType_t sampleType )
{
	static char	buff[ 100 ];
	
	if ( sampleType == STYPE_CONTROL )
	{
		return strings[ SSTR_CONTROL ];
	}
	else if ( sampleType == STYPE_OTHER )
	{
		return strings[ SSTR_OTHER ];
	}
	else if ( sampleType >= STYPE_FIRST_CONTROL && sampleType <= STYPE_LAST_CONTROL )
	{
		sprintf( buff, "%s %d", strings[ SSTR_CONTROL ], sampleType - STYPE_FIRST_CONTROL + 1 );
		return buff;
	}
	else if ( sampleType >= STYPE_FIRST_SPECIES && sampleType <= STYPE_LAST_SPECIES )
	{
		sprintf( buff, "%s %d", strings[ SSTR_SPECIES ], sampleType - STYPE_FIRST_SPECIES + 1 );
		return buff;
	}
	else if ( sampleType >= STYPE_FIRST_NAMED_SPECIES && sampleType <= STYPE_LAST_NAMED_SPECIES )
	{
		return strings[ speciesNames[ sampleType - STYPE_FIRST_NAMED_SPECIES ] ];
	}
	else
	{
		return "";
	}
}
#else
const wchar_t *
Species::
GetSampleTypeString( SampleType_t sampleType , AStrings *strings )
{
	static wchar_t	buff[ 100 ];
	
	if ( sampleType == STYPE_CONTROL )
	{
		return (*strings)[ SSTR_CONTROL ];
	}
	else if ( sampleType == STYPE_OTHER )
	{
		return (*strings)[ SSTR_OTHER ];
	}
	else if ( sampleType >= STYPE_FIRST_CONTROL && sampleType <= STYPE_LAST_CONTROL )
	{
		wsprintf( buff, L"%s %d", (*strings)[ SSTR_CONTROL ], sampleType - STYPE_FIRST_CONTROL + 1 );
		return buff;
	}
	else if ( sampleType >= STYPE_FIRST_SPECIES && sampleType <= STYPE_LAST_SPECIES )
	{
		wsprintf( buff, L"%s %d", (*strings)[ SSTR_SPECIES ], sampleType - STYPE_FIRST_SPECIES + 1 );
		return buff;
	}
	else if ( sampleType >= STYPE_FIRST_NAMED_SPECIES && sampleType <= STYPE_LAST_NAMED_SPECIES )
	{
		return (*strings)[ speciesNames[ sampleType - STYPE_FIRST_NAMED_SPECIES ] ];
	}
	else
	{
		return L"";
	}
}
const wchar_t *
Species::
GetSampleTypePrintString( SampleType_t sampleType , AStrings *strings )
{
	static wchar_t	buff[ 100 ];
	
	if ( sampleType == STYPE_CONTROL )
	{
		return strings->PrintString (SSTR_CONTROL);
	}
	else if ( sampleType == STYPE_OTHER )
	{
		return strings->PrintString (SSTR_OTHER);
	}
	else if ( sampleType >= STYPE_FIRST_CONTROL && sampleType <= STYPE_LAST_CONTROL )
	{
		wsprintf( buff, L"%s %d", strings->PrintString (SSTR_CONTROL), sampleType - STYPE_FIRST_CONTROL + 1 );
		return buff;
	}
	else if ( sampleType >= STYPE_FIRST_SPECIES && sampleType <= STYPE_LAST_SPECIES )
	{
		wsprintf( buff, L"%s %d", strings->PrintString (SSTR_SPECIES), sampleType - STYPE_FIRST_SPECIES + 1 );
		return buff;
	}
	else if ( sampleType >= STYPE_FIRST_NAMED_SPECIES && sampleType <= STYPE_LAST_NAMED_SPECIES )
	{
		return strings->PrintString (speciesNames[ sampleType - STYPE_FIRST_NAMED_SPECIES ]);
	}
	else
	{
		return L"";
	}
}
#endif

const wchar_t *
Species::
English( SampleType_t sampleType, AStrings *strings )
{
	static wchar_t	buff[ 100 ];
	
	if ( sampleType == STYPE_CONTROL )
	{
		return (*strings).English( SSTR_CONTROL );
	}
	else if ( sampleType == STYPE_OTHER )
	{
		return (*strings).English( SSTR_OTHER );
	}
	else if ( sampleType >= STYPE_FIRST_CONTROL && sampleType <= STYPE_LAST_CONTROL )
	{
		wsprintf( buff, L"%s %d", (*strings).English( SSTR_CONTROL ), sampleType - STYPE_FIRST_CONTROL + 1 );
		return buff;
	}
	else if ( sampleType >= STYPE_FIRST_SPECIES && sampleType <= STYPE_LAST_SPECIES )
	{
		wsprintf( buff, L"%s %d", (*strings).English( SSTR_SPECIES ), sampleType - STYPE_FIRST_SPECIES + 1 );
		return buff;
	}
	else if ( sampleType >= STYPE_FIRST_NAMED_SPECIES && sampleType <= STYPE_LAST_NAMED_SPECIES )
	{
		return (*strings).English( speciesNames[ sampleType - STYPE_FIRST_NAMED_SPECIES ] );
	}
	else
	{
		return L"";
	}
}

#if 0
const char *
Species::
operator[]( SpeciesGroup_t group )
{
	switch ( group )
	{
		case GTYPE_CANINES:				return strings[ SSTR_CANINES ];				break;
		case GTYPE_FELINES:				return strings[ SSTR_FELINES ];				break;
		case GTYPE_EQUINES:				return strings[ SSTR_EQUINES ];				break;
		case GTYPE_BOVINES:				return strings[ SSTR_BOVINES ];				break;
		case GTYPE_AVIANS:				return strings[ SSTR_AVIANS ];				break;
		case GTYPE_REPTILES_AMPHIBIANS:	return strings[ SSTR_REPTILES_AMPHIBIANS ];	break;
		case GTYPE_SMALL_MAMMALS:		return strings[ SSTR_SMALL_MAMMALS ];		break;
		case GTYPE_LARGE_MAMMALS:		return strings[ SSTR_LARGE_MAMMALS ];		break;
		default:						return "";									break;
	}
}
#else
const wchar_t *
Species::
GetGroupString (SpeciesGroup_t group, AStrings *strings )
{
	switch ( group )
	{
		case GTYPE_CANINES:				return (*strings)[ SSTR_CANINES ];				break;
		case GTYPE_FELINES:				return (*strings)[ SSTR_FELINES ];				break;
		case GTYPE_EQUINES:				return (*strings)[ SSTR_EQUINES ];				break;
		case GTYPE_BOVINES:				return (*strings)[ SSTR_BOVINES ];				break;
		case GTYPE_AVIANS:				return (*strings)[ SSTR_AVIANS ];				break;
		case GTYPE_REPTILES_AMPHIBIANS:	return (*strings)[ SSTR_REPTILES_AMPHIBIANS ];	break;
		case GTYPE_SMALL_MAMMALS:		return (*strings)[ SSTR_SMALL_MAMMALS ];		break;
		case GTYPE_LARGE_MAMMALS:		return (*strings)[ SSTR_LARGE_MAMMALS ];		break;
		default:						return L"";									break;
	}
}
#endif

SpeciesGroupList_t
Species::
GetGroupList( SpeciesGroup_t group )
{
	SpeciesGroupList_t	groupList;
	
	const SampleType_t	canines[] = {	STYPE_DOG,
										STYPE_COYOTE,
										STYPE_DINGO,
										STYPE_FOX,
										STYPE_JACKAL,
										STYPE_WOLF, };

	const SampleType_t	felines[] = {	STYPE_CAT,
										STYPE_BOBCAT,
										STYPE_CHEETAH,
										STYPE_JAGUAR,
										STYPE_LEOPARD,
										STYPE_LION,
										STYPE_LYNX,
										STYPE_PANTHER,
										STYPE_PUMA,
										STYPE_TIGER, };
	
	const SampleType_t	equines[] = {	STYPE_HORSE,
										STYPE_DONKEY,
										STYPE_PONY,
										STYPE_ZEBRA, };
	
	const SampleType_t	bovines[] = {	STYPE_COW,
										STYPE_WATER_BUFFALO,
										STYPE_STEER,
										STYPE_OX,
										STYPE_BULL,
										STYPE_BUFFALO,
										STYPE_BISON, };
	
	const SampleType_t avians[] = {		STYPE_BIRD,
										STYPE_AFRICAN_GRAY,
										STYPE_AMAZON,
										STYPE_CHICKEN,	
										STYPE_COCKATIEL,
										STYPE_COCKATOO,
										STYPE_CONURE,
										STYPE_DOVE,
										STYPE_DUCK,
										STYPE_FLAMINGO,
										STYPE_GOOSE,
										STYPE_LOVEBIRD,
										STYPE_MACAW,
										STYPE_MYNAH,
										STYPE_OSTRICH,
										STYPE_PARAKEET,
										STYPE_PARROT,
										STYPE_TOUCAN, };
	
	const SampleType_t repamph[] = {	STYPE_REPTILE,
										STYPE_ALLIGATOR,
										STYPE_AMPHIBIAN,
										STYPE_BEARDED_DRAGON,
										STYPE_BOA_CONSTRICTOR,
										STYPE_CHAMELEON,
										STYPE_CROCODILE,
										STYPE_FROG,
										STYPE_GECKO,
										STYPE_IGUANA,
										STYPE_LIZARD,
										STYPE_MONITOR,
										STYPE_PYTHON,
										STYPE_SALAMANDER,
										STYPE_SNAKE,
										STYPE_TOAD,
										STYPE_TORTOISE,
										STYPE_TURTLE, };
	
	const SampleType_t smammals[] = {	STYPE_SMALL_MAMMAL,
										STYPE_BAT,
										STYPE_CHINCHILLA,
										STYPE_FERRET,
										STYPE_GERBIL,
										STYPE_GUINEA_PIG,
										STYPE_HAMSTER,
										STYPE_HEDGEHOG,
										STYPE_LEMUR,
										STYPE_MINK,
										STYPE_MOUSE,
										STYPE_OPOSSUM,
										STYPE_PRAIRIE_DOG,
										STYPE_RABBIT,
										STYPE_RACCOON,
										STYPE_RAT,
										STYPE_SKUNK,
										STYPE_SQUIRREL, };
	
	const SampleType_t lmammals[] = {	STYPE_LARGE_MAMMAL,
										STYPE_ANTELOPE,
										STYPE_APE,
										STYPE_BABOON,
										STYPE_BEAR,
										STYPE_CAMEL,
										STYPE_CHIMPANZEE,
										STYPE_DEER,
										STYPE_ELEPHANT,
										STYPE_GIRAFFE,
										STYPE_GOAT,
										STYPE_KANGAROO,
										STYPE_LLAMA,
										STYPE_MONKEY,
										STYPE_PIG,
										STYPE_SEA_LION,
										STYPE_SEAL,
										STYPE_SHEEP, };
	
// GLD
// Probably want to do the alpha sorting here.
	switch ( group )
	{
		case GTYPE_CANINES:
			groupList.list = canines;
			groupList.len = sizeof( canines ) / sizeof( SampleType_t );
			break;
		case GTYPE_FELINES:
			groupList.list = felines;
			groupList.len = sizeof( felines ) / sizeof( SampleType_t );
			break;
		case GTYPE_EQUINES:
			groupList.list = equines;
			groupList.len = sizeof( equines ) / sizeof( SampleType_t );
			break;
		case GTYPE_BOVINES:
			groupList.list = bovines;
			groupList.len = sizeof( bovines ) / sizeof( SampleType_t );
			break;
		case GTYPE_AVIANS:
			groupList.list = avians;
			groupList.len = sizeof( avians ) / sizeof( SampleType_t );
			break;
		case GTYPE_REPTILES_AMPHIBIANS:
			groupList.list = repamph;
			groupList.len = sizeof( repamph ) / sizeof( SampleType_t );
			break;
		case GTYPE_SMALL_MAMMALS:
			groupList.list = smammals;
			groupList.len = sizeof( smammals ) / sizeof( SampleType_t );
			break;
		case GTYPE_LARGE_MAMMALS:
			groupList.list = lmammals;
			groupList.len = sizeof( lmammals ) / sizeof( SampleType_t );
			break;
		default:
			groupList.list = 0;
			groupList.len = 0;
			break;
	}
	
	return groupList;
}

/*
 * Return true if the given sampleType is in the Avians or the Reptiles/Amphibians,
 * or if it's a control or unnamed species.
 */

bool
Species::
IsAvianReptilian( SampleType_t sampleType )
{
	// Return true if control or unnamed species.
	if ( ( sampleType >= STYPE_CONTROL && sampleType <= STYPE_LAST_CONTROL ) ||
		 ( sampleType >= STYPE_OTHER && sampleType <= STYPE_LAST_SPECIES ) )
	{
		return true;
	}

	else
	{
		SpeciesGroupList_t	groupList;
		int					i;
		
		// Search the list of avians and return true if found.
		groupList = GetGroupList( GTYPE_AVIANS );
		for ( i = 0; i < groupList.len; i++ )
		{
			if ( sampleType == groupList.list[ i ] )
			{
				return true;
			}
		}
		
		// Search the list of reptiles and amphibians and return true if found.
		groupList = GetGroupList( GTYPE_REPTILES_AMPHIBIANS );
		for ( i = 0; i < groupList.len; i++ )
		{
			if ( sampleType == groupList.list[ i ] )
			{
				return true;
			}
		}
	}
	
	// Not found, so false.
	return false;
}

/*
 * Return true if the given sampleType is in the Canines,
 * or if it's a control.
 */

bool
Species::
IsCanine( SampleType_t sampleType )
{
	// Return true if control.
	if ( (sampleType >= STYPE_CONTROL) && (sampleType <= STYPE_LAST_CONTROL) )
	{
		return true;
	}

	else
	{
		SpeciesGroupList_t	groupList;
		int					i;
		
		// Search the list of canines and return true if found.
		groupList = GetGroupList( GTYPE_CANINES );
		for ( i = 0; i < groupList.len; i++ )
		{
			if ( sampleType == groupList.list[ i ] )
			{
				return true;
			}
		}
	}
	
	// Not found, so false.
	return false;
}

/*
 * Return true if the given sampleType is in the Equines, or if it's a control
 * or unnamed species.
 */

bool
Species::
IsEquine( SampleType_t sampleType )
{
	// Return true if control or unnamed species.
	if ( ( sampleType >= STYPE_CONTROL && sampleType <= STYPE_LAST_CONTROL ) ||
		 ( sampleType >= STYPE_OTHER && sampleType <= STYPE_LAST_SPECIES ) )
	{
		return true;
	}

	else
	{
		SpeciesGroupList_t	groupList;
		int					i;
		
		// Search the list of equines and return true if found.
		groupList = GetGroupList( GTYPE_EQUINES );
		for ( i = 0; i < groupList.len; i++ )
		{
			if ( sampleType == groupList.list[ i ] )
			{
				return true;
			}
		}
	}
	
	// Not found, so false.
	return false;
}

PatientType_t
Species::
GetPatientType( SampleType_t sampleType )
{
	SpeciesGroupList_t	groupList;
	int					i;
		
	// See if it's a canine.
	groupList = GetGroupList( GTYPE_CANINES );
	for ( i = 0; i < groupList.len; i++ )
	{
		if ( sampleType == groupList.list[ i ] )
		{
			return PATIENT_TYPE_DOG_FEMALE;
		}
	}

	// Feline?
	groupList = GetGroupList( GTYPE_FELINES );
	for ( i = 0; i < groupList.len; i++ )
	{
		if ( sampleType == groupList.list[ i ] )
		{
			return PATIENT_TYPE_CAT_MALE;
		}
	}

	// Equine?
	groupList = GetGroupList( GTYPE_EQUINES );
	for ( i = 0; i < groupList.len; i++ )
	{
		if ( sampleType == groupList.list[ i ] )
		{
			return PATIENT_TYPE_HORSE;
		}
	}

	// All the rest are "unspecified".
	return PATIENT_TYPE_UNSPECIFIED;


}

