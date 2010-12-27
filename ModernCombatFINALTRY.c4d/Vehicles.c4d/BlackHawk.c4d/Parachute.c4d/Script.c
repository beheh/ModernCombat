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
  //Freiflug oder mit Objektanhang?
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
  else
  {
    var targ = GetActionTarget();

    //Windbeeinflussung
    SetXDir(GetWind(GetX(), GetY()) /8 , targ);

    //Fall verlangsamen
    SetYDir(30,targ,10);

    //Manuelles Losmachen durch [Doppelstop] des Objektanhangs
    if(GetPlrDownDouble(GetController(targ)))
      SetAction("StartFlyFree");

    //Bei Wasser- oder Bodenkontakt zusammenfallen
    if(GetContact(0, -1, CNAT_Bottom) || GetContact(targ, -1, CNAT_Bottom) || InLiquid() || InLiquid(targ))
      Close();

    if (GetProcedure(targ) != "FLOAT" && GetProcedure(targ) != "FLIGHT")
      Close();
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