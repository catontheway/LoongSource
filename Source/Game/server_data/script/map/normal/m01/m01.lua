
Marriage_Available = 0

--���m01�����أ�����ϵͳ���ţ���ȫ�ֱ���Marriage_Available��Ϊ1.
function m01_OnInit(MapID,InstanceID,CreatureID,CreatureTypeID)
	Marriage_Available = 1
end
aux.RegisterMapEvent("m01", 0, "m01_OnInit")
