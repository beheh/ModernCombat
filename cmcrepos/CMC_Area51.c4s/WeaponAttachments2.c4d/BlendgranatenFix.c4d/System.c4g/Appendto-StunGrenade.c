/*-- Sensorball --*/

#strict 2

#appendto STUN


public func FxIntFlashbangTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var i = EffectVar(0,pTarget,iEffectNumber)--;
  if(i <= 0) return -1;
  if(!EffectVar(1,pTarget,iEffectNumber)) return -1;
  
  var a = BoundBy(255-(i*255/100),0,255);
  EffectVar(1,pTarget,iEffectNumber)->SetAlpha(a);
  
  var val, num, pCursor, c, flag;
  if(!Contained())
  {
  	Message("<c %x>{{SM07}}</c>", pTarget, RGBa(255, 255, 255, BoundBy(255-a, 1, 254)));
    /* some weird 'n ol' stuff
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      pCursor = GetCursor(GetPlayerByIndex(i))->~GetRealCursor();
      if(!pCursor) pCursor = GetCursor(GetPlayerByIndex(i));
      if(!pCursor) continue;
      if(Contained(pCursor)) continue;
      num = GetEffect("IntFlashbang", pCursor);
      if(num)
      {
        val = 255-BoundBy((EffectVar(0,pCursor,num)-1)*255/100,0,255);
        if(255-a < val) val = 255-a;
      }
      else
      {
        val = 255-a;
      }
      flag = 0;
      if(c != 0) flag = MSG_Multiple;
      CustomMessage(Format("<c %x>{{SM07}}</c>", RGBa(255,255,255,BoundBy(val, 1, 254))), pTarget, GetPlayerByIndex(i), 0, 0, 0, 0, 0, flag); 
      c++;
    }*/
  }
  else
  {
    Message("@", pTarget); 
  }
}
