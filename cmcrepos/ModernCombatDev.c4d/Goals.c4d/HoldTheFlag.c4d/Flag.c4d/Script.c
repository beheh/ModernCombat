/*-- Flagge --*/

#strict 2

local callback;

public func GetClonk()
{
  if(WildcardMatch(GetAction(), "*Attach*"))
    return GetActionTarget();
  else
    return 0;
}

/* TimerCall */

private func Wee()
{
  if(WildcardMatch(GetAction(), "*Fly*")) return Wind2Fly();
  if(WildcardMatch(GetAction(), "*Attach*")) return Clonk2Rotate();
}

private func Wind2Fly()
{
  if(GetWind() > 0)
    SetDir(DIR_Right);
  else
    SetDir(DIR_Left);
}

private func Clonk2Rotate()
{
  if(!GetAlive(GetActionTarget())) return AttachTargetLost();

  SetDir(GetActionTarget()->GetDir());

  if(GetDir(GetActionTarget()) == DIR_Left)
    {
    //if(Inside(GetR(), 15, 55)) SetR(GetR()+RandomX(-2,3));
    /*else */SetR(BoundBy(GetR(), 35, 55));
    }
  if(GetDir(GetActionTarget()) == DIR_Right)
    {
    //if(Inside(GetR(), -55, -15)) SetR(GetR()+RandomX(-3,2));
    /*else */SetR(BoundBy(GetR(), -55, -35));
    }
}

protected func Activate(object pCallback, int dwColor)
{
  callback = pCallback;

  if(!dwColor) dwColor = RGB(255,255,255);
  SetColorDw(dwColor);

  // bei Dunkelheit besser zu sehen
  /*var tmp = AddLightAmbience(30,this());
  var r,g,b,a;
  SplitRGBaValue(dwColor,r,g,b,a);
  tmp->ChangeColor(RGBa(r,g,b,a+30));
  SetVisibility(VIS_All,tmp);*/
  //Ach Käse.

  AddEffect("Collect", this, 101, 5, this);
}

/* Einsamml0rn!!11 */
protected func Collected(object pClonk)
{
  SetColorDw(GetColorDw(pClonk));
  SetAction("Attach", pClonk);

  if(callback)
    if(callback->~CollectFlag(pClonk,this))
    {
      SetAction("Lost");
      SetActionTargets();
      SetColorDw(RGB(255,255,255));
      return false;
    }

  return true;
}

protected func CheckCollect(object pClonk)
{
  if(WildcardMatch(GetAction(), "*Attach*")) return ;
  if(Collected(pClonk))
    return true;
  else
    return false;
}

protected func AttachTargetLost()
{
  if(callback)
    callback->~DropFlag(GetClonk(),this);
    
  SetDir();
  SetAction("Lost");
  SetActionTargets();
  SetColorDw(RGB(255,255,255));
}

// verbessertes Einsammeln
public func FxCollectTimer(target, no)
{
  if(WildcardMatch(GetAction(target),"*Attach*"))
  {
    if(IsFakeDeath(GetActionTarget()))
    {
      SetAction("Lost");
      SetColorDw(RGB(255,255,255));
      SetActionTargets();
      SetDir();
      if(Contained())//Wird bei FakeDeath gerne eingesaugt. O_o
        Exit();
    }
  }
  else
  {
    var c;
    while(c = FindObject(0, -20, -20, 40, 40, OCF_CrewMember, 0,0, NoContainer(), c))
      if(c->GetOCF() & OCF_Alive)
        if(CheckCollect(c))
          return ;
  }
}

/* Kaputt! */

public func Destruction()
{
  // Flaggen dürfen nicht einfach weg sein
  var flag = GetID();
  if(!flag) flag = HTFF;
  var nFlag = CreateObject(flag, 0,0, NO_OWNER);
  nFlag->Activate(callback,GetColorDw());
  
  if(callback)
  {
    callback->~SetFlag(nFlag);
    callback->~ResetFlag();
  }
}

public func NoWarp() { return true; }
