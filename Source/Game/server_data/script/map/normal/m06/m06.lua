


--��¼m06��ͼScriptData����ͬʱ���ڵ�����
	--m06��ͼ����1��14217 5�����ķ��� 1007107 ��¼����
	--m06��ͼ����2��14217 һ�����ķ�������1007108 ��¼����
	--m06��ͼ����3��14237 ��������Ӷ�Ա 1008006 ��¼����
	--m06��ͼ����4��14237 ��������Ӷӳ� 1008007 ��¼����
	--m06��ͼ����5��14254 BOSS��̶���� 1007113 ��¼����
	--m06��ͼ����6��14026 ڵ���ͬ�� 1007026 ��¼����
	--m06��ͼ����7����¼ս�����еĻ���ˢ������



--������ʧ������������ScriptData ��ֵ��1
function m06_OnDisappear(MapID,InstanceID,CreatureID,CreatureTypeID)
	if CreatureTypeID == 1007107 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,1)
		map.SetMapScriptData(MapID,InstanceID,1,1,a-1)
	elseif CreatureTypeID == 1007108 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,2)
		map.SetMapScriptData(MapID,InstanceID,1,2,a-1)
	elseif CreatureTypeID == 1008006 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,3)
		map.SetMapScriptData(MapID,InstanceID,1,3,a-1)
	elseif CreatureTypeID == 1008007 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,4)
		map.SetMapScriptData(MapID,InstanceID,1,4,a-1)
	elseif CreatureTypeID == 1007113 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,5)
		map.SetMapScriptData(MapID,InstanceID,1,5,a-1)
	elseif CreatureTypeID == 1007026 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,6)
		map.SetMapScriptData(MapID,InstanceID,1,6,a-1)
	end

end
aux.RegisterMapEvent("m06", 18, "m06_OnDisappear")
