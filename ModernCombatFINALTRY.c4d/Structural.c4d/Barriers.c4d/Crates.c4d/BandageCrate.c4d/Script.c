/*-- Verbandskiste --*/

#strict 2
#include WCR2


/* Zerstörung */

func Damage()
{
  if(GetDamage() > 50)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",3,20,0,0,200,200);
    CastParticles("Paper",RandomX(20,30),40,0,0,20,35,RGB(180,180,180),RGBa(240,240,240,150));
    CastParticles("WoodSplinter",5,50,0,0,75,100);
    CastParticles("WoodCrateSplinter",4,70,0,0,100,100);
    Sound("CrateDestruct*.ogg");
    Sound("WoodCrack*.ogg");

    //Verband auswerfen
    var i = 5;
    while (--i)
    {
      //Erzeugen und verschleudern
      var obj = CreateObject(BDGE),
      angle = Random(91) - 45;
      SetSpeed(Sin(angle,50),-Cos(angle,50),obj);

      //Verschwinden planen
      ScheduleCall(obj,"Destruct",40*27);
    }

    //Verschwinden
    RemoveObject(0,1);
  }
}