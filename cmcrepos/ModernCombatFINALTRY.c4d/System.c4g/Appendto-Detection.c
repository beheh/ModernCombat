/*-- Ortung --*/

//Das Appendto macht bestimmte Objekte identifizierbar (z.B. für Sensorbälle).

#strict 2

#appendto NADE	//Handgranaten
#appendto BBTP	//Sprengfallen
#appendto C4EX	//C4 Ladungen
#appendto ESHL	//Gewehrgranaten
#appendto MISL	//Raketen


public func IsDetectable()	{return !this->~IsFadingOut();}