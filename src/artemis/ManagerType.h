#ifndef Artemis_ManagerType_h__
#define Artemis_ManagerType_h__

namespace artemis
{
typedef unsigned int ManagerType;
/**
 * Type of Manager.
 * Use own enum to create new manager. Custom manager should have id > mtDEFAULT_MANAGERS_CNT
 * 
 * @author Vladimir Ivanov (ArCorvus)
 *
 */
enum ManagerTypes {
   mtEntityManager = 0,
   mtComponentManager,
   mtGroupManager,
   mtPlayerManager,
   mtTagManager,
   mtTeamManager,
   mtDEFAULT_MANAGERS_CNT
};
}
#endif // Artemis_ManagerType_h__
