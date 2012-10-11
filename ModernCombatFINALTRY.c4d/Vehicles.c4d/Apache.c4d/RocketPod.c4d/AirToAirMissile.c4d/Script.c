/*-- Luft-Luft-Rakete --*/

#strict 2
#include MISL


public func MaxTime()		{return 100;}			//Maximale Flugzeigt

public func StartSpeed()	{return 25;}			//Startgeschwindigkeit
public func Acceleration()	{return 4;}			//Beschleunigung
public func MaxSpeed()		{return 110;}			//Maximale Geschwindigkeit

public func SecureTime()	{return 18;}			//Mindestflugzeit

public func ExplosionDamage()	{return 24;}			//Explosionsschaden

public func TracerCompatible()	{return false;}			//Peilsendersuchende Rakete
public func Guideable()		{return false;}			//Kann gesteuert werden

public func MaxTurn()		{return 7;}			//max. Drehung


private func Detonate()
{
  Explode(ExplosionDamage());
}