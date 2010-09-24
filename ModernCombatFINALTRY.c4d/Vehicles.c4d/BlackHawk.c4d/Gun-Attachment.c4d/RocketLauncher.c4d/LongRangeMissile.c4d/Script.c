/*-- Langstreckenrakete --*/

#strict 2
#include MISL


public func MaxTime()		{return 400;}			//Maximale Flugzeigt

public func StartSpeed()	{return 20;}			//Startgeschwindigkeit
public func Acceleration()	{return 4;}			//Beschleunigung
public func MaxSpeed()		{return 150;}			//Maximale Geschwindigkeit

public func SecureTime()	{return 50;}			//Mindestflugzeit
public func SecureDistance()	{return 50;}			//Mindestabstand

public func ExplosionDamage()	{return 10;}			//Explosionsschaden
public func ExplosionRadius()	{return 25;}			//Radius

public func MaxTurn()		{return 7;}			//max. Drehung
public func MaxTracerTurn()	{return 9;}			//max. Drehung bei Zielverfolgung