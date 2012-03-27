/*-- Neues Script --*/

#strict 2

#appendto _HBR

/* Zerstörung */

public func MaxDamage() { return 150; }
public func IsRepairable() { return true; }

func Damage()
{
  if(GetDamage() > MaxDamage())
  {
   	//Effekte
   	if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,150,0,0,40,100);
   	if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,230,0,0,40,15,RGB(40,20,20));
   	CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
   	Sound("StructuralDamage*.ogg");

		//Hängen evtl. Balken an der Brücke? Dann löschen.
		for(var obj in FindObjects(Find_Func("IsBar"), Find_ActionTarget(this)))
		{
			if(obj)
				RemoveObject(obj);
		}

   	//Zum Wrack werden
   	ChangeDef(_HBB);
   	SetSolidMask();

   	//Verschwinden
   	FadeOut();
  }
}