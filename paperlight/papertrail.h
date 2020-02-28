#ifndef papertrail_h
#define papertrail_h

// Date mask lengths for substring matching
#define MASK_START 15
#define SAME_DATE     10  // "min_time_at":"2020-02-25T23:19:40+11:00" -> 2020-02-25
#define SAME_10_HOURS 12  // "min_time_at":"2020-02-25T23:19:40+11:00" -> 2020-02-25T2
#define SAME_HOUR     13  // "min_time_at":"2020-02-25T23:19:40+11:00" -> 2020-02-25T23
#define SAME_10_MINS  15  // "min_time_at":"2020-02-25T23:19:40+11:00" -> 2020-02-25T23:1
#define SAME_MINUTE   16  // "min_time_at":"2020-02-25T23:19:40+11:00" -> 2020-02-25T23:19

#endif
