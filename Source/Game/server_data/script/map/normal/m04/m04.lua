


--��¼m04��ͼScriptData����ͬʱ���ڵ�����
--m04����1 ��12274��1005140����ͥ���� 2��
--m04����2 ��12274��1005139����ͥ���� 1��
--m04����3 ����ԯ���� �ٻ��̾��� ��ʧ����
--m04����4 ����ԯ���� �ٻ��Ͼ��� ��ʧ����


--������ʧ���ߣ���ScriptData ��ֵ��1
function m04_OnDisappear(MapID,InstanceID,CreatureID,CreatureTypeID)
	if CreatureTypeID == 1005140 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,1)
		map.SetMapScriptData(MapID,InstanceID,1,1,a-1)
	elseif CreatureTypeID == 1005139 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,2)
		map.SetMapScriptData(MapID,InstanceID,1,2,a-1)
	end
end
aux.RegisterMapEvent("m04", 18, "m04_OnDisappear")


--����m04ʱ����ȫ�ֱ���Salary_NPC_On=1����ˢ��NPC������н��ʹ��
function m04_OnInit(MapID,InstanceID,CreatureID,CreatureTypeID)
	if Salary_NPC_On == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 3610202, 2158, 18338, 2396, 1, "")
	end
	if Open_Arena == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 4006512, 2071, 18345, 2666, 1, "")
	end

	--for i = 20,29 do
		--if (act.GetActScriptData(151, 1, i) == 0 and act.GetActScriptData(151, 1, i-20) ~= 0) or act.GetActIsStart(151)  or act.GetActScriptData(151, 1, 0) == 0 then
			map.MapCreateCreature(3017299663, -1, 4900490, 2093, 18345, 2724)
			--break
		--end
	--end
	map.MapCreateCreature(3017299663, -1, 4900491, 2093, 18345, 2718)

end
aux.RegisterMapEvent("m04", 0, "m04_OnInit")

