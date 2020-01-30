#ifndef __SPECIES_H_INCLUDED
#define __SPECIES_H_INCLUDED

#include "AStrings.h"
#include "CommonDefines.h"

/*
 * Sample types
 * When adding a species, it must also be added to Species.cc and ParseRefRanges.cc.
 */

typedef enum
{
	STYPE_NONE					= -2,		// Used by ref ranges next n species/other
	STYPE_ALL					= -1,		// Used by print/archive ref ranges

	// ParseRefRanges depends on STYPE_CONTROL thru STYPE_LAST_CONTROL being contiguous.
	STYPE_CONTROL				= 0,
	STYPE_FIRST_CONTROL			= 1,		// Control 1 thru 99
	STYPE_LAST_CONTROL			= 99,

	// ParseRefRanges depends on STYPE_OTHER thru STYPE_LAST_SPECIES being contiguous.
	STYPE_OTHER					= 100,
	STYPE_PATIENT				= STYPE_OTHER,
	STYPE_FIRST_SPECIES			= 101,		// Species 1 thru 99
	STYPE_LAST_SPECIES			= 199,

	// Canines
	STYPE_DOG,
	STYPE_FIRST_NAMED_SPECIES = STYPE_DOG,
	STYPE_COYOTE,
	STYPE_DINGO,
	STYPE_FOX,
	STYPE_JACKAL,
	STYPE_WOLF,

	// Felines
	STYPE_CAT,
	STYPE_BOBCAT,
	STYPE_CHEETAH,
	STYPE_JAGUAR,
	STYPE_LEOPARD,
	STYPE_LION,
	STYPE_LYNX,
	STYPE_PANTHER,
	STYPE_PUMA,
	STYPE_TIGER,
	
	// Equines
	STYPE_HORSE,
	STYPE_DONKEY,
	STYPE_PONY,
	STYPE_ZEBRA,
	
	// Bovines
	STYPE_COW,
	STYPE_WATER_BUFFALO,
	STYPE_STEER,
	STYPE_OX,
	STYPE_BULL,
	STYPE_BUFFALO,
	STYPE_BISON,
	
	// Avians
	STYPE_BIRD,
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
	STYPE_TOUCAN,
	
	// Reptiles/amphibians
	STYPE_REPTILE,
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
	STYPE_TURTLE,
	
	// Small mammals
	STYPE_SMALL_MAMMAL,
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
	STYPE_SQUIRREL,
	
	// Large mammals
	STYPE_LARGE_MAMMAL,
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
	STYPE_SHEEP,
	
	STYPE_LAST_NAMED_SPECIES	= STYPE_SHEEP,

	STYPE_NUM_SPECIES
} SampleType_t;

#define NUM_NAMED_SPECIES	(STYPE_LAST_NAMED_SPECIES - STYPE_FIRST_NAMED_SPECIES + 1)

// Species group types.
typedef enum
{
	GTYPE_CANINES,
	GTYPE_FIRST_GROUP	= GTYPE_CANINES,
	GTYPE_FELINES,
	GTYPE_EQUINES,
	GTYPE_BOVINES,
	GTYPE_AVIANS,
	GTYPE_REPTILES_AMPHIBIANS,
	GTYPE_SMALL_MAMMALS,
	GTYPE_LARGE_MAMMALS,
	GTYPE_LAST_GROUP	= GTYPE_LARGE_MAMMALS,
	GTYPE_NUM_GROUPS
} SpeciesGroup_t;

typedef struct
{
	int						len;
	const SampleType_t *	list;
} SpeciesGroupList_t;

class Species
{
public:
						Species();
	//const char *		operator[]( SampleType_t sampleType );
	//const char *		operator[]( SpeciesGroup_t group );
	const wchar_t *		English( SampleType_t sampleType, AStrings *strings );
    const wchar_t *     GetSampleTypeString (SampleType_t sampleType , AStrings *strings );         
    const wchar_t *     GetSampleTypePrintString (SampleType_t sampleType , AStrings *strings );         
    const wchar_t *     GetGroupString (SpeciesGroup_t group, AStrings *strings );
	SpeciesGroupList_t	GetGroupList( SpeciesGroup_t group );
	bool				IsAvianReptilian( SampleType_t sampleType );
	bool				IsCanine( SampleType_t sampleType );
	bool				IsEquine( SampleType_t sampleType );
	PatientType_t		GetPatientType( SampleType_t sampleType );

private:
	StringId_t	speciesNames[ NUM_NAMED_SPECIES ];
	//AStrings	strings;
};

#endif	// __SPECIES_H_INCLUDED
