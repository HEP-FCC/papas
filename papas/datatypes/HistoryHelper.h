#ifndef HistoryHelper_h
#define HistoryHelper_h

#include "papas/datatypes/Identifier.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DirectedAcyclicGraph.h"

namespace papas {
  
class HistoryHelper {
public:
  HistoryHelper(const PapasEvent& papasEvent);
  Ids linkedIds(IdType id) const;
  Ids filteredIds(Ids ids, const Identifier::ItemType type, const Identifier::SubType subtype) const;
  
private:
  
  
  const PapasEvent& m_papasEvent;
};

}

#endif /* HistoryHelper_h */
