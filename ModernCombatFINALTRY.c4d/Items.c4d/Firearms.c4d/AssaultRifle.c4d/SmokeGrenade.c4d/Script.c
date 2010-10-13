/*-- Rauchgranate --*/

#strict 2
#include ESHL

protected func SecureDistance()	{}
func ExplodeDelay()		{return 120;}


/* Treffer */

func HitObject(object pObj)
{
  if(pObj)
  {
   DoDmg(Distance(GetXDir(),GetYDir())/3,DMG_Projectile,pObj);
   CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
   if(GetOCF(pObj) & OCF_Living)
   {
    Sound("SharpnelImpact*.ogg");
   }
   else
   {
    Sound("BlockOff*.ogg");
    Sparks(30,RGB(255,128));
    CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
   }
  }
  Trigger();
}

func Trigger()
{
  //Effekte
  CreateParticle("Blast",0,0,0,0,5*10,RGB(255,255,128));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Smoke3",8,50,0,0,120,150,RGBa(255,255,255,120),RGBa(255,255,255,150));
  Sound("SGRN_Fused.ogg");

  //Rauch erzeugen
  CastObjects(SM4K,3,10);

  //Verschwinden
  RemoveObject();
}