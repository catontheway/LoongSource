


--��¼m02��ͼScriptData����ͬʱ���ڵ�����
--m02��ͼ����1��10423 �����۷�[1003081]��¼����
--m02��ͼ����2��10437 ����Ů��[1003165]��¼����
--m02��ͼ����3��10438 ��ˮ��Ů��[1003166]��¼����
--m02��ͼ����4��10439 �޷�Ů��[1003167]��¼����
	--m02��ͼ����5��13233 ����ç������[1006207]��¼����


--������ʧ������������ScriptData����1 ��ֵ��1
function m02_OnDisappear(MapID,InstanceID,CreatureID,CreatureTypeID)
	if CreatureTypeID == 1003165 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,2)
		if a < 1 or a > 50  then
			a = 1
		end
		map.SetMapScriptData(MapID,InstanceID,1,2,a-1)
	elseif CreatureTypeID == 1003081 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,1)
		if a < 1 or a > 50  then
			a = 1
		end
		map.SetMapScriptData(MapID,InstanceID,1,1,a-1)
	elseif CreatureTypeID == 1003166 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,3)
		if a < 1 or a > 50  then
			a = 1
		end
		map.SetMapScriptData(MapID,InstanceID,1,3,a-1)
	elseif CreatureTypeID == 1003167 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,4)
		if a < 1 or a > 50  then
			a = 1
		end
		map.SetMapScriptData(MapID,InstanceID,1,4,a-1)
	elseif CreatureTypeID == 1006207 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,5)
		if a < 1 or a > 50  then
			a = 1
		end
		map.SetMapScriptData(MapID,InstanceID,1,5,a-1)
	--[[elseif CreatureTypeID == 1006209 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,6)
		if a < 1 or a > 50  then
			a = 1
		end
		map.SetMapScriptData(MapID,InstanceID,1,6,a-1)]]
	end

end
aux.RegisterMapEvent("m02", 18, "m02_OnDisappear")


Marriage_Available = 0

--����m02ʱ����ȫ�ֱ���TW_Endorsor_On=1����ˢ��NPC������ü
function m02_OnInit(MapID,InstanceID,CreatureID,CreatureTypeID)
	if TW_Endorsor_On == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 4900446, 2270, 6648, 2266, 1, "")
	end
	if Open_XJ_XiaoLongNv == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 4900449, 2171, 6646, 2219, 1, "")
		map.MapCreateColCreature(MapID, InstanceID, 4900450, 2166, 6646, 2219, 1, "")
	    XJ_XiaoLongNV_InitQuestionTable()
	end

	--for i = 20,29 do
		--if (act.GetActScriptData(151, 1, i) == 0 and act.GetActScriptData(151, 1, i-20) ~= 0) or act.GetActIsStart(151) or act.GetActScriptData(151, 1, 0) == 0 then
			map.MapCreateCreature(3017298127, -1, 4900490, 2140, 6646, 2258)
			--break
		--end
	--end
	map.MapCreateCreature(3017298127, -1, 4900491, 2133, 6646, 2258)

end
aux.RegisterMapEvent("m02", 0, "m02_OnInit")

