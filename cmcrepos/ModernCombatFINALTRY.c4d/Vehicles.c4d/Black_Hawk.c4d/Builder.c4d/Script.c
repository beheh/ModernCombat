/* Builder */

#strict


protected func Initialize()
{
  CreateObject(H_HE, 0, 30, GetOwner());
  RemoveObject();
  return(true);
}