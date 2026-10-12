#ifndef QUICKGUIDEPRECATED_H
#define QUICKGUIDEPRECATED_H

/// Marks a declaration as deprecated, so that the compiler warns where it is used.
#if defined(_MSC_VER)
#   define QUICKGUI_DEPRECATED(message) __declspec(deprecated(message))
#elif defined(__GNUC__) || defined(__clang__)
#   define QUICKGUI_DEPRECATED(message) __attribute__((deprecated(message)))
#else
#   define QUICKGUI_DEPRECATED(message)
#endif

#endif
