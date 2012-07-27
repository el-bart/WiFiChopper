/*
 * ForceLink.hpp
 *
 * helper macros for force-link functionlity.
 *
 */
#ifndef INCLUDE_BUILD_PROCESS_FORCELINK_HPP_FILE
#define INCLUDE_BUILD_PROCESS_FORCELINK_HPP_FILE

/** \brief marks object as the one to be force-linked.
 *  \param name project-unique name (must be valid C++ name).
 *
 * this feature is required for static libraries, since linker removes
 * object that nothing depends on, such as automatic registrators which are
 * crutial for some code structures like abstract factories.
 *
 * \note since 'name' must be project-wide unique it is good to use naming
 *       convention simillar to header guards - use name of all namespaces
 *       ond registerd object's file name in to prevent name collisions.
 */
#define FORCE_LINK_THIS_OBJECT(name) extern const bool FORCE_LINK_THIS_OBJECT__##name##__END=true;

#endif
