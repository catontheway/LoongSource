


--��¼m05��ͼScriptData����ͬʱ���ڵ�����
	--m05��ͼ����1��13449 ��ţ��[1006321]��¼����
	--m05��ͼ����2��13469 ����ħ[1006151]��¼����
	--m05��ͼ����3��13602 �ĸ��ٱ�[1006404]��¼����



--������ʧ������������ScriptData ��ֵ��1
function m05_OnDisappear(MapID,InstanceID,CreatureID,CreatureTypeID)
	if CreatureTypeID == 1006321 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,1)
		map.SetMapScriptData(MapID,InstanceID,1,1,a-1)
	elseif CreatureTypeID == 1006151 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,2)
		map.SetMapScriptData(MapID,InstanceID,1,2,a-1)
	elseif CreatureTypeID == 1006404 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,3)
		map.SetMapScriptData(MapID,InstanceID,1,3,a-1)
	end

end
aux.RegisterMapEvent("m05", 18, "m05_OnDisappear")
