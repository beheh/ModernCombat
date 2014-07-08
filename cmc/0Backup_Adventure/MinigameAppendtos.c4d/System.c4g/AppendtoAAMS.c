/*-- Neues Script --*/

#strict 2

#appendto AAMS

private func Detonate()
{
	// Extraverhalten für Blackhawk Defense: Antipanzerungsraketen sollen nur bei Blackhawks detonieren.
	if(!FindObject(MBKD) || FindObjects(Find_ID(BKHK), Find_Distance(50)))
	{
  	//Extraschaden für Strukturen
  	for(var obj in FindObjects(Find_Distance(50),
  			Find_Category(C4D_Structure | C4D_Vehicle),
  			Find_Exclude()))
  	DoDmg(BoundBy(InvertA1(ObjectDistance(obj), 40),0,20), DMG_Explosion, obj, 0, GetOwner()+1, GetID());

  	//Schaden verursachen
  	DamageObjects(ExplosionRadius(), ExplosionDamage()/2, this);
  	if(FindObject(MBKD))
  		FindObject(MBKD)->~BlackhawkDestroyed(this);
  	
  	Explode(ExplosionDamage()*3/2);
  }
  else
  	DecoExplode(ExplosionDamage()*3/2);
}
