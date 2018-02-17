#ifndef MP4V2_ITMF_TAGS_H
#define MP4V2_ITMF_TAGS_H

/**************************************************************************//**
 *
 *  @defgroup mp4_itmf_tags MP4v2 iTMF (iTunes Metadata Format) Tags
 *  @{
 *
 *  This is a high-level API used to manage iTMF metadata.
 *
 *  It provides more type-safety and simplified memory management as compared
 *  to iTMF Generic API.
 *
 *  At the heart of this API is a read-only structure that holds all known
 *  items and their current values. The value is always a pointer which if
 *  NULL indicates its corresponding atom does not exist. Thus, one must
 *  always check if the pointer is non-NULL before attempting to extract
 *  its value.
 *
 *  The structure may not be directly modified. Instead, <b>set</b> functions
 *  corresponding to each item are used to modify the backing-store of
 *  the read-only structure. Setting the value ptr to NULL will effectively
 *  remove it. Setting the value ptr to real data will immediately make a
 *  copy of the value in the backing-store and the read-only structure
 *  will correctly reflect the change.
 *
 *  The hidden data cache memory is automatically managed. Thus the user need
 *  only guarantee the data is available during the lifetime of the set-function
 *  call.
 *
 *  <b>iTMF Tags read workflow:</b>
 *
 *      @li MP4TagsAlloc()
 *      @li MP4TagsFetch()
 *      @li inspect each tag of interest...
 *      @li MP4TagsStore() (if modified)
 *      @li MP4TagsFree()
 *
 *  <b>iTMF Tags read/modify/add/remove workflow:</b>
 *
 *      @li MP4TagsAlloc()
 *      @li MP4TagsFetch()
 *      @li inspect each tag of interest...
 *      @li MP4TagsSetName(), MP4TagsSetArtist()...
 *      @li MP4TagsStore()
 *      @li MP4TagsFree()
 *
 *  @par Warning:
 *  Care must be taken when using multiple mechanisms to modify an open mp4
 *  file as it is not thread-safe, nor does it permit overlapping different
 *  API workflows which have a begin/end to their workflow. That is to say
 *  do not interleave an iTMF Generic workflow with an iTMF Tags workflow.
 *
 *****************************************************************************/

/** Enumeration of possible MP4TagArtwork::type values. */
typedef enum MP4TagArtworkType_e
{
    MP4_ART_UNDEFINED = 0,
    MP4_ART_BMP       = 1,
    MP4_ART_GIF       = 2,
    MP4_ART_JPEG      = 3,
    MP4_ART_PNG       = 4
} MP4TagArtworkType;

/** Data object representing a single piece of artwork. */
typedef struct MP4TagArtwork_s
{
    void*             data; /**< raw picture data */
    uint32_t          size; /**< data size in bytes */
    MP4TagArtworkType type; /**< data type */
} MP4TagArtwork;

typedef struct MP4TagTrack_s
{
    uint16_t index;
    uint16_t total;
} MP4TagTrack;

typedef struct MP4TagDisk_s
{
    uint16_t index;
    uint16_t total;
} MP4TagDisk;

/** Tags <b>convenience</b> structure.
 *
 *  This structure is used in the tags convenience API which allows for
 *  simplified retrieval and modification of the majority of known tags.
 *
 *  This is a read-only structure and each tag is present if and only if the
 *  pointer is a <b>non-NULL</b> value. The actual data is backed by a hidden
 *  data cache which is only updated when the appropriate metadata <b>set</b>
 *  function is used, or if MP4TagsFetch() is invoked. Thus, if other API
 *  is used to manipulate relevent atom structure of the MP4 file, the user
 *  is responsible for re-fetching the data in this structure.
 */
typedef struct MP4Tags_s
{
    void* __handle; /* internal use only */

    const char*        name;
    const char*        artist;
    const char*        albumArtist; 
    const char*        album;
    const char*        grouping;
    const char*        composer;
    const char*        comments;
    const char*        genre;
    const uint16_t*    genreType;
    const char*        releaseDate;
    const MP4TagTrack* track;
    const MP4TagDisk*  disk;
    const uint16_t*    tempo;
    const uint8_t*     compilation;

    const char*     tvShow;
    const char*     tvNetwork;
    const char*     tvEpisodeID;
    const uint32_t* tvSeason;
    const uint32_t* tvEpisode;

    const char* description;
    const char* longDescription;
    const char* lyrics;

    const char* sortName;
    const char* sortArtist;
    const char* sortAlbumArtist;
    const char* sortAlbum;
    const char* sortComposer;
    const char* sortTVShow;

    const MP4TagArtwork* artwork;
    uint32_t             artworkCount;

    const char* copyright;
    const char* encodingTool;
    const char* encodedBy;
    const char* purchaseDate;

    const uint8_t* podcast;
    const char*    keywords;  /* TODO: Needs testing */
    const char*    category;    

    const uint8_t* hdVideo;
    const uint8_t* mediaType;
    const uint8_t* contentRating;
    const uint8_t* gapless;

    const char*     iTunesAccount;
    const uint8_t*  iTunesAccountType;
    const uint32_t* iTunesCountry;
    const uint32_t* contentID;
    const uint32_t* artistID;
    const uint64_t* playlistID;
    const uint32_t* genreID;
    const uint32_t* composerID;
    const char*     xid;
} MP4Tags;

/** Allocate tags convenience structure for reading and settings tags.
 *
 *  This function allocates a new structure which represents a snapshot
 *  of all the tags therein, tracking if the tag is missing,
 *  or present and with value. It is the caller's responsibility to free
 *  the structure with MP4TagsFree().
 *
 *  @return structure with all tags missing.
 */
MP4V2_EXPORT
const MP4Tags* MP4TagsAlloc( void );

/** Fetch data from mp4 file and populate structure.
 *
 *  The tags structure and its hidden data-cache is updated to
 *  reflect the actual tags values found in the <b>hFile</b>.
 *
 *  @param tags structure to fetch (write) into.
 *  @param hFile handle of file to fetch data from.
 *
 *  @return <b>true</b> on success, <b>false</b> on failure.
 */
MP4V2_EXPORT
bool MP4TagsFetch( const MP4Tags* tags, MP4FileHandle hFile );

/** Store data to mp4 file from structure.
 *
 *  The tags structure is pushed out to the mp4 file,
 *  adding tags if needed, removing tags if needed, and updating
 *  the values to modified tags.
 *
 *  @param tags structure to store (read) from.
 *  @param hFile handle of file to store data to.
 *
 *  @return <b>true</b> on success, <b>false</b> on failure.
 */
MP4V2_EXPORT
bool MP4TagsStore( const MP4Tags* tags, MP4FileHandle hFile );

/** Free tags convenience structure.
 *
 *  This function frees memory associated with the structure.
 *
 *  @param tags structure to destroy.
 */
MP4V2_EXPORT
void MP4TagsFree( const MP4Tags* tags );

/** Accessor that indicates whether a tags structure
 * contains any metadata
 *
 * @param tags the structure to inspect
 *
 * @param hasMetadata populated with false if @p tags
 * contains no metadata, true if @p tags contains metadata
 *
 * @retval false error determining if @p tags contains
 * metadata
 *
 * @retval true successfully determined if @p tags contains
 * metadata
 */
MP4V2_EXPORT
bool MP4TagsHasMetadata ( const MP4Tags* tags, bool *hasMetadata );

MP4V2_EXPORT bool MP4TagsSetName            ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetArtist          ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetAlbumArtist     ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetAlbum           ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetGrouping        ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetComposer        ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetComments        ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetGenre           ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetGenreType       ( const MP4Tags*, const uint16_t* );
MP4V2_EXPORT bool MP4TagsSetReleaseDate     ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetTrack           ( const MP4Tags*, const MP4TagTrack* );
MP4V2_EXPORT bool MP4TagsSetDisk            ( const MP4Tags*, const MP4TagDisk* );
MP4V2_EXPORT bool MP4TagsSetTempo           ( const MP4Tags*, const uint16_t* );
MP4V2_EXPORT bool MP4TagsSetCompilation     ( const MP4Tags*, const uint8_t* );

MP4V2_EXPORT bool MP4TagsSetTVShow          ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetTVNetwork       ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetTVEpisodeID     ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetTVSeason        ( const MP4Tags*, const uint32_t* );
MP4V2_EXPORT bool MP4TagsSetTVEpisode       ( const MP4Tags*, const uint32_t* );

MP4V2_EXPORT bool MP4TagsSetDescription     ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetLongDescription ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetLyrics          ( const MP4Tags*, const char* );

MP4V2_EXPORT bool MP4TagsSetSortName        ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetSortArtist      ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetSortAlbumArtist ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetSortAlbum       ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetSortComposer    ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetSortTVShow      ( const MP4Tags*, const char* );

MP4V2_EXPORT bool MP4TagsAddArtwork         ( const MP4Tags*, MP4TagArtwork* );
MP4V2_EXPORT bool MP4TagsSetArtwork         ( const MP4Tags*, uint32_t, MP4TagArtwork* );
MP4V2_EXPORT bool MP4TagsRemoveArtwork      ( const MP4Tags*, uint32_t );

MP4V2_EXPORT bool MP4TagsSetCopyright       ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetEncodingTool    ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetEncodedBy       ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetPurchaseDate    ( const MP4Tags*, const char* );

MP4V2_EXPORT bool MP4TagsSetPodcast         ( const MP4Tags*, const uint8_t* );
MP4V2_EXPORT bool MP4TagsSetKeywords        ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetCategory        ( const MP4Tags*, const char* );

MP4V2_EXPORT bool MP4TagsSetHDVideo         ( const MP4Tags*, const uint8_t* );
MP4V2_EXPORT bool MP4TagsSetMediaType       ( const MP4Tags*, const uint8_t* );
MP4V2_EXPORT bool MP4TagsSetContentRating   ( const MP4Tags*, const uint8_t* );
MP4V2_EXPORT bool MP4TagsSetGapless         ( const MP4Tags*, const uint8_t* );

MP4V2_EXPORT bool MP4TagsSetITunesAccount     ( const MP4Tags*, const char* );
MP4V2_EXPORT bool MP4TagsSetITunesAccountType ( const MP4Tags*, const uint8_t* );
MP4V2_EXPORT bool MP4TagsSetITunesCountry     ( const MP4Tags*, const uint32_t* );
MP4V2_EXPORT bool MP4TagsSetContentID         ( const MP4Tags*, const uint32_t* );
MP4V2_EXPORT bool MP4TagsSetArtistID          ( const MP4Tags*, const uint32_t* );
MP4V2_EXPORT bool MP4TagsSetPlaylistID        ( const MP4Tags*, const uint64_t* );
MP4V2_EXPORT bool MP4TagsSetGenreID           ( const MP4Tags*, const uint32_t* );
MP4V2_EXPORT bool MP4TagsSetComposerID        ( const MP4Tags*, const uint32_t* );
MP4V2_EXPORT bool MP4TagsSetXID               ( const MP4Tags*, const char* );

/** @} ***********************************************************************/

#endif /* MP4V2_ITMF_TAGS_H */
