#strict 2

global func DamageObjects(int iDistance, int iDamage, object pObject, int iX, int iY, int iAttachment)
{
  if(!pObject) pObject = this;
  var x = GetX(pObject)+iX;
  var y = GetY(pObject)+iY;
  var dealer = -1;
  dealer = GetController(pObject);
  if(dealer == -1) dealer = GetOwner(pObject);
  var icon = pObject->~GetKillIcon();
  if(!icon) icon = GetID(pObject);
  for(var obj in FindObjects(pObject->Find_Distance(iDistance,iX,iY),
                             pObject->Find_Exclude(this),
                             Find_NoContainer()))
  {
  	DoDmg(iDamage - (Distance(GetX(obj),GetY(obj),x,y)*iDamage/iDistance), DMG_Explosion, obj, 0, dealer+1, icon, iAttachment);
  }
  return true;
}
