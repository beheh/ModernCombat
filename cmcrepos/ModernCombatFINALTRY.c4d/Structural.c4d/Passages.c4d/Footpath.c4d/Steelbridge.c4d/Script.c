/*-- Stahlbr¸cke --*/

#strict 2

public func MaxDamage()		{return 150;}
public func IsRepairable()	{return true;}

local ruins, destroyed;


/* Initialisierung */

protected func Initialize()
{
  //Ruinen standardm‰ﬂig eingeschaltet
  ruins = true;

  return 1;
}
/* Ruinen (de)aktivieren */

protected func SwitchMode()
{
  if(ruins)
    ruins = 0;
  else
    ruins = 1;
}

/* Zerstˆrung */

func Damage(int change)
{
  if(GetDamage() > MaxDamage() && !destroyed)
  {
    destroyed = 1;

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,150,0,0,40,100);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,230,0,0,40,15,RGB(40,20,20));
    CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("StructuralDamage*.ogg");

    //Soll Ruine erstellen?
    if(ruins)
    {
      //Aussehen ‰ndern
      SetGraphics("Destroyed");
    }
    else
    {
      //Zum Wrack werden
      ChangeDef(_HBB);

      //SolidMask entfernen
      SetSolidMask();

      //Verschwinden
      FadeOut();
    }
  }

  if(destroyed)
  {
    //Schaden ¸ber Maximalwert verhindern
    if(change > 0)
    {
      if(GetDamage() > MaxDamage())
        DoDamage(-(GetDamage()-MaxDamage()));
    }

    //Reparatur abschlieﬂen
    if(GetDamage() <= 0 && destroyed)
    {
      //Aussehen und SolidMask wiederherstellen
      SetGraphics();
      SetSolidMask(0,0,72,24);

      destroyed = 0;
    }
  }
}