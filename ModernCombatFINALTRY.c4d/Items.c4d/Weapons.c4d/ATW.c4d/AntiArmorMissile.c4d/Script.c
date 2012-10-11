/*-- Antipanzerungsrakete --*/

#strict 2
#include MISL

public func TracerCompatible()	{return false;}

public func MaxTime()		{return 250;}			//Maximale Flugzeit

public func StartSpeed()	{return 1;}			//Startgeschwindigkeit
public func Acceleration()	{return 5;}			//Beschleunigung
public func MaxSpeed()		{return 120;}			//Maximale Geschwindigkeit

public func SecureTime()	{return 21;}			//Mindestflugzeit
public func MaxDamage()		{return 30;}			//Maximalschaden bis Absturz

public func ExplosionDamage()	{return 25;}			//Explosionsschaden
public func ExplosionRadius()	{return 25;}			//Radius

public func MaxTurn()		{return 5;}			//max. Drehung


private func Detonate()
{
  //Extraschaden f�r Strukturen
  for(var obj in FindObjects(Find_Distance(50),
  		Find_Category(C4D_Structure | C4D_Vehicle),
  		Find_Exclude()))
  DoDmg(BoundBy(InvertA1(ObjectDistance(obj), 40),0,20), DMG_Explosion, obj, 0, GetOwner()+1, GetID());

  //Schaden verursachen
  DamageObjects(ExplosionRadius(), ExplosionDamage()/2, this);
  Explode(ExplosionDamage()*3/2);
}
