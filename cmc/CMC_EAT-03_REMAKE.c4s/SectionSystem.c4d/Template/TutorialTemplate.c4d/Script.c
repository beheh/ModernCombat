/*-- Template --*/

#strict 2

#include TSYS

// Dieses Templateobjekt ist leider nicht mehr auf den aktuellsten Stand, sollte daher nicht als Templateobjekt genutzt werden. (Ich hab halt kein Bock st�ndig zu aktualisieren >_<)

public func TutorialIndex() { return -1; }

public func UsedRules() { return []; } // Auflistung von IDs. Bsp: [NOFD, FDMG]; 
public func SectionName() { return "Template"; } // Sektionsname.
public func Condition(object pByObj) { return 0; } // Bedingung, die erf�llt sein muss um Spielbar zu sein.
public func HideInMenu() { return false; } // Falls es NICHT im Men� erscheinen soll: false

public func Initialize() { } // ...
public func InitializePlayer(int iPlr) { } // Wird f�r jedem Spieler beim Sektionswechsel aufgerufen.
public func CreateInterior() { } // ...
public func CreateEquipment() { } // ...
public func CreateDecoration() { } // ...
public func CreateOptionalFeatures() { } // ...
public func RemovePlayer(int iPlr) { } // Falls Spieler iPlr entfernt wird.
public func RejectRelaunch(int iPlr) { return false; } // Relaunch ablehnen?

public func ExitArrived(object pByObj) { return _inherited(pByObj); } // Wird aufgerufen, falls pByObj den Ausgang erreicht hat.
public func OnClonkRelaunch(object pCrew) { } // Falls ein Crewobjekt positioniert wird nach Sektionswechsel
