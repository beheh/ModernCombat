/*-- Stahlbr¸cke --*/

#strict 2

public func IsRepairable()	{return !norepair;}
public func MaxDamage()		{return 150;}
public func RepairSpeed()	{return 2;}
public func BonusPointRepair()	{return 50;}

local ruins, destroyed, norepair;


/* Initialisierung */

protected func Initialize()
{
  //Ruinen standardm‰ﬂig ausgeschaltet
  ruins = false;

  return 1;
}

/* Ruinen (de)aktivieren */

protected func SwitchMode()
{
  ruins = !ruins;
}

protected func SetIrreparable()
{
  norepair = true;
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
    Sound("StructureHit*.ogg");

    //Soll Ruine erstellen?
    if(ruins)
    {
      //Aussehen ‰ndern
      SetGraphics("Destroyed");
      //SolidMask setzen
      SetSolidMask(0,0,72,24);
    }
    else
    {
      //Zum Wrack werden
      ChangeDef(_HBB);

      //SolidMask entfernen
      this->SetSolidMask();

      //Verschwinden
      this->FadeOut();
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