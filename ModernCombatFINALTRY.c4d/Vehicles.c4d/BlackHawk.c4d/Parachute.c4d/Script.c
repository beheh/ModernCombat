/*-- Fallschirm --*/

#strict 2


/* Erstellung */

public func ControlDigDouble(pObj)
{
  Set(pObj);
}

public func Set(object pObj)
{
  Sound("ParachuteOpen*.ogg");
  SetAction("Open", pObj);
}

/* Timer */

protected func Fly()
{
  //Freiflug
  if(WildcardMatch(GetAction(),"*Free*"))
  {
    //Windbeeinflussung
    SetXDir(GetWind(GetX(), GetY()) / 8);
    SetYDir(30, 0, 10);
    //In Wasser zusammenfallen
    if(GetContact(0,-1, CNAT_Bottom) || InLiquid())
    {
      SetAction("FoldFree");
      Sound("ParachuteClose.ogg");
    }
  }
  //Mit Objekt
  else
  {
    var targ = GetActionTarget();
    if (!targ)
    {
      SetAction("StartFlyFree");
      return;
    }

    //Windbeeinflussung
    SetXDir(GetWind(GetX(), GetY()) /8 , targ);

    //Fall verlangsamen
    var y = Interpolate2(GetYDir(targ, 100), 300, 1, 5);
    SetYDir(y, targ, 100);

    //Manuelles Losmachen durch [Doppelstop] des Objektanhangs
    if(GetPlrDownDouble(GetController(targ)))
      SetAction("StartFlyFree");

    //Bei Wasser- oder Bodenkontakt zusammenfallen
    if(GetContact(0, -1, CNAT_Bottom) || GetContact(targ, -1, CNAT_Bottom) || InLiquid() || InLiquid(targ))
      Close();

    //Lebewesen
    if (GetCategory(targ) & C4D_Living)
      if (GetID(Contained(targ)) == FKDT)
      {
        SetAction("StartFlyFree");
        SetPosition (GetX(), GetY()+22); 
      }
      else
        if (GetID(targ) != FKDT && GetProcedure(targ) != "FLOAT" && GetProcedure(targ) != "FLIGHT")
          Close();

    if (Contained())
    {
      Exit();
      SetAction("Fly", targ, 0, true);
    }
  }
}

/* Auf- und einklappen */

protected func Opening()
{
  if(GetActTime() > 25)
  {
    SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, this);
    SetAction("Fly", GetActionTarget());
    return;
  }
  var w = Sin(36 * GetActTime() / 10, 1000);
  var h = 40 * GetActTime();

  SetObjDrawTransform(w, 0, 0, 0, h, InvertA1(500 * GetObjHeight() * h / 1000, 1000 * GetObjHeight()) - 500 * GetObjHeight(), this);
}

public func Close()
{
  SetAction("Fold", GetActionTarget());
  Sound("ParachuteClose.ogg");
}

private func Folded()
{
  //Verschwinden
  FadeOut();
}