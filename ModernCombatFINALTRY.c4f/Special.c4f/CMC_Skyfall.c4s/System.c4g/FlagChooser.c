/*-- Flaggenwähler --*/

//Spieler spawnen bei Besitznahme in einem Apache.

#strict 2
#appendto OSPW


public func Spawn()
{
  if(!Contents()) return RemoveObject();

  var obj = Contents();
  Exit(obj);

  //Flugbereiten Apache erstellen
  var apache = CreateObject(APCE,0,0, GetOwner(obj));
  apache->SetAction("Fly");
  apache->EngineStarted();
  LocalN("throttle", apache) = 100;

  //Clonk hineinsetzen
  Enter(apache, obj);

  RemoveObject();

  return 1;
}