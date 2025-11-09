/* ICU 50 to ICU 76 compatibility wrapper header */
#ifndef ICU_COMPAT_H
#define ICU_COMPAT_H

/* Symbol aliases for ICU compatibility */
#ifdef __linux__
#define U_ICU_VERSION_MAJOR_NUM 76

/* ICU 50 to 76 symbol mapping */
#define u_strlen_50 u_strlen_76
#define unorm_normalize_50 unorm_normalize_76
#define unorm_quickCheck_50 unorm_quickCheck_76
#define ubrk_next_50 ubrk_next_76
#define ubrk_following_50 ubrk_following_76
#define ubrk_open_50 ubrk_open_76
#define ubrk_close_50 ubrk_close_76
#define ucnv_toUChars_50 ucnv_toUChars_76
#define ucnv_convert_50 ucnv_convert_76
#define ucnv_close_50 ucnv_close_76
#define ucnv_setToUCallBack_50 ucnv_setToUCallBack_76
#define ucnv_open_50 ucnv_open_76
#define ucnv_fromUChars_50 ucnv_fromUChars_76
#define ucnv_getDefaultName_50 ucnv_getDefaultName_76
#define ures_getStringByKey_50 ures_getStringByKey_76
#define ures_getInt_50 ures_getInt_76
#define ures_open_50 ures_open_76
#define ures_getByKey_50 ures_getByKey_76
#define ures_close_50 ures_close_76
#define u_strFromUTF8_50 u_strFromUTF8_76
#define u_strToUTF8_50 u_strToUTF8_76
#define u_strcmp_50 u_strcmp_76
#define u_strFoldCase_50 u_strFoldCase_76
#define u_cleanup_50 u_cleanup_76
#define uloc_getDefault_50 uloc_getDefault_76
#define utf8_nextCharSafeBody_50 utf8_nextCharSafeBody_76
#define udata_setAppData_50 udata_setAppData_76
#define UCNV_TO_U_CALLBACK_STOP_50 UCNV_TO_U_CALLBACK_STOP_76

#endif /* __linux__ */
#endif /* ICU_COMPAT_H */
