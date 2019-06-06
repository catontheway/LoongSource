
--ScriptData ��0λΪ��¼��ɱ�˼�������ʦ�ͲƱ�����
--ScriptData ��1λΪ��¼������ͼ�����Ƿ��Ѵ�����
--ScriptData ��2λΪ��¼����BOSSID
--ScriptData ��3λΪ��¼����1527�Ƿ��
--ScriptData ��4λΪ�Ƿ�ˢ������BOSS��
--ScriptData ��5λΪBOSS�Ƿ�Ѫ����40%
--ScriptData ��6λΪ��¼��һ�����Ƿ񿪹�
--ScriptData ��7λΪ��¼ɱ�˶���Թ���Ļ�
--ScriptData ��8λΪ��¼ɱ�˶���Ǳ����ħ�����˶���Ĺ��
--ScriptData ��9λΪ��ͼ����1id
--ScriptData ��10λΪ��ͼ����2id
--ScriptData ��11λΪ�����Ƿ��ѷ�
--ScriptData ��12λΪ�����Ƿ��ѷ�
--ScriptData ��13λΪ��¼ɱ���˶��ٸ�С�ֺ�BOSS

function m46_OnPlayerEnter(MapID, InstanceID, RoleID)
    local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end

	local Kanshou1_id = map.GetMapScriptData(MapID, InstanceID, 1, 9)
	local Kanshou2_id = map.GetMapScriptData(MapID, InstanceID, 1, 10)

	if Kanshou1_id==0 and Kanshou2_id==0 then
		Kanshou1_id = map.MapCreateCreature(MapID, InstanceID, 1010408, 261, 3840, 1506)
		Kanshou2_id = map.MapCreateCreature(MapID, InstanceID, 1010409, 969, 3840, 1372)
		map.SetMapScriptData(MapID, InstanceID, 1, 9, Kanshou1_id)
		map.SetMapScriptData(MapID, InstanceID, 1, 10, Kanshou2_id)
	end
end

--ע��
aux.RegisterMapEvent("m46", 2, "m46_OnPlayerEnter")



--��Կ��2615703���ߵ�������1596������1527,����1530ɾ��9422001������1531ɾ��9422101
function m46_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 1596 then
		local BOOL_EnterTrigger1596 = map.GetMapScriptData(MapID,InstanceID,1,3)
		if BOOL_EnterTrigger1596 == 0 then --�ж��Ƿ񱻴�����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 525)	-- "�����ӵ��Կ�ײ���ͨ�����š�"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	--[[elseif ObjID == 1530 then
	    local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 94220)
	    if bhave == true then
	        unit.CancelBuff(MapID, InstanceID, RoleID, 9422001)
		end
	elseif ObjID == 1531 then
	    local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 94221)
	    if bhave == true then
	        unit.CancelBuff(MapID, InstanceID, RoleID, 9422101)
		end]]
	end
end
aux.RegisterMapEvent("m46",6,"m46_OnEnterTrigger")

--�ɷ�ʹ�ú���
function i2615703_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false

	--������������ܵ�������ʹ��
	if MapID ~= 3084408015 then
	    bRet = 43
	else
	    --�õ���ҵ�ǰ����
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x <= 728 or x >= 772 or z <= 338 or z >= 367 then  --�������������귶Χ������ʹ��
	        bRet = 43
	    end
	end

	--����
	return bRet, bIgnore
end

--Կ��ʹ��Ч��
function i2615703_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	map.SetMapScriptData(MapID,InstanceID,1,3,1)--��¼�Ѿ�������1596
	map.OpenCloseDoor(MapID, InstanceID, 1527, 1)
end

--ע��
aux.RegisterItemEvent(2615703, 1, "i2615703_QuestUsable")
aux.RegisterItemEvent(2615703, 0, "i2615703_CanUse")

--�������15095ˢ������
function q15095_OnComplete(MapID, InstanceID, questID, ownerID, enderID)
	map.MapCreateCreature(MapID, InstanceID, 3075208, 499, 5664, 1759)
end

--ע��
aux.RegisterQuestEvent(15095, 1, "q15095_OnComplete")


function m46_OnRoleEnterArea(MapID, InstanceID, RoleID, ObjAreaID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local ADD_record = map.GetMapScriptData(MapID, InstanceID, 1, 1)

	if ObjAreaID==1610 then --���ŵ�һ����ˢ������ʬ
		local temp = ADD_record % 2
		if temp == 0 then
			map.MapCreateCreature(MapID, InstanceID, 1010402, 751, 3840, 185)
			map.MapCreateCreature(MapID, InstanceID, 1010402, 727, 3840, 185)
			map.MapCreateCreature(MapID, InstanceID, 1010402, 774, 3840, 185)
			map.SetMapScriptData(MapID, InstanceID, 1, 1, ADD_record+1)
		end

	elseif ObjAreaID==1632 then --���ŵڶ�����ˢ��ʬ
		local temp = math.floor(ADD_record/2) % 2
		if temp == 0 then
			map.MapCreateCreature(MapID, InstanceID, 1010402, 736, 3840, 273)
			map.MapCreateCreature(MapID, InstanceID, 1010402, 768, 3840, 273)
			map.MapCreateCreature(MapID, InstanceID, 1010402, 752, 3840, 287)
			map.SetMapScriptData(MapID, InstanceID, 1, 1, ADD_record+2)
		end

	elseif ObjAreaID==1662 then --�����ģ�ˢ��ʬС��

		local temp = math.floor(ADD_record/8) % 2
		if temp == 0 then
			map.MapCreateCreature(MapID, InstanceID, 1010402, 736, 3840, 617)
			map.MapCreateCreature(MapID, InstanceID, 1010402, 749, 3840, 609)
			map.MapCreateCreature(MapID, InstanceID, 1010402, 761, 3840, 590)
			map.MapCreateCreature(MapID, InstanceID, 1010402, 744, 3840, 593)
			map.MapCreateCreature(MapID, InstanceID, 1010420, 748, 3840, 604)
			map.SetMapScriptData(MapID, InstanceID, 1, 1, ADD_record+8)
		end

	elseif ObjAreaID==1665 then --�����ģ�ˢ��ʬС��

		local temp = math.floor(ADD_record/4) % 2
		if temp == 0 then
			map.MapCreateCreature(MapID, InstanceID, 1010402, 732, 3840, 679)
			map.MapCreateCreature(MapID, InstanceID, 1010402, 768, 3840, 679)
			map.MapCreateCreature(MapID, InstanceID, 1010402, 751, 3840, 692)
			map.SetMapScriptData(MapID, InstanceID, 1, 1, ADD_record+4)
		end

	elseif ObjAreaID==1692 then --ͨ���壬ˢ׽�Ź�
		local temp = math.floor(ADD_record/16) % 2
		local temp2 = map.GetMapScriptData(MapID, InstanceID, 1, 6)
		if temp == 0 and temp2==1 then
			map.MapCreateCreature(MapID, InstanceID, 1010425, 643, 3840, 710)
			map.MapCreateCreature(MapID, InstanceID, 1010425, 643, 3840, 704)
			map.MapCreateCreature(MapID, InstanceID, 1010425, 643, 3840, 716)
			map.SetMapScriptData(MapID, InstanceID, 1, 1, ADD_record+16)
		end

	elseif ObjAreaID==1693 then --6��ǰ�����ţ�ˢ��
		local temp = math.floor(ADD_record/32) % 2
		local temp2 = map.GetMapScriptData(MapID, InstanceID, 1, 6)
		if temp == 0 and temp2==1 then
			map.OpenCloseDoor(MapID, InstanceID, 1526, 1)
			map.MapCreateCreature(MapID, InstanceID, 1010402, 394, 3840, 700)
			map.MapCreateCreature(MapID, InstanceID, 1010402, 394, 3840, 718)
			map.MapCreateCreature(MapID, InstanceID, 1010402, 394, 3840, 709)
			map.MapCreateCreature(MapID, InstanceID, 1010420, 384, 3840, 705)
			map.MapCreateCreature(MapID, InstanceID, 1010420, 384, 3840, 714)
			map.SetMapScriptData(MapID, InstanceID, 1, 1, ADD_record+32)
		end

	elseif ObjAreaID==1702 then --����7��ˢһ��С��

		local temp = math.floor(ADD_record/256) % 2
		if temp == 0 then
			map.MapCreateCreature(MapID, InstanceID, 1010418, 141, 3840, 683)
			map.MapCreateCreature(MapID, InstanceID, 1010418, 149, 3840, 688)
			map.MapCreateCreature(MapID, InstanceID, 1010418, 146, 3840, 677)
			map.MapCreateCreature(MapID, InstanceID, 1010418, 135, 3840, 676)
			map.MapCreateCreature(MapID, InstanceID, 1010418, 134, 3840, 691)
			map.SetMapScriptData(MapID, InstanceID, 1, 1, ADD_record+256)
		end

	elseif ObjAreaID==1703 then --����7��ˢһ��С��


	elseif ObjAreaID==1715 then --ͨ��13
		local temp = math.floor(ADD_record/64) % 2
		if temp == 0 then
			map.MapCreateCreature(MapID, InstanceID, 1010425, 551, 3840, 1010)
			map.MapCreateCreature(MapID, InstanceID, 1010425, 546, 3840, 1014)
			map.MapCreateCreature(MapID, InstanceID, 1010425, 546, 3840, 1003)
			map.SetMapScriptData(MapID, InstanceID, 1, 1, ADD_record+64)
		end

	elseif ObjAreaID==1716 then --ͨ��13
		local temp = math.floor(ADD_record/128) % 2
		if temp == 0 then
			map.MapCreateCreature(MapID, InstanceID, 1010425, 617, 3840, 1014)
			map.MapCreateCreature(MapID, InstanceID, 1010425, 615, 3840, 1006)
			map.MapCreateCreature(MapID, InstanceID, 1010425, 620, 3840, 1001)
			map.MapCreateCreature(MapID, InstanceID, 1010426, 635, 3840, 1002)
			map.MapCreateCreature(MapID, InstanceID, 1010426, 609, 3840, 1004)
			map.SetMapScriptData(MapID, InstanceID, 1, 1, ADD_record+128)
		end

	elseif ObjAreaID==1754 then --��ͽ��

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1748)	--�ڴ˴�ʹ����Ԫħ�ϼ��ɷ�ס��ͽ�������ߵ�·�ڣ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif ObjAreaID==1795 then --��ͽ��

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1748)	--�ڴ˴�ʹ����Ԫħ�ϼ��ɷ�ס��ͽ�������ߵ�·�ڣ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	end


end

aux.RegisterMapEvent("m46", 7, "m46_OnRoleEnterArea")

--��Ԫħ��
function i2615704_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false

	--������������ܵ�������ʹ��
	if MapID ~= 3084408015 then
	    bRet = 43
	else
	    --�õ���ҵ�ǰ����
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		local left_door=0
		local right_door=0

	    if x >= 319 and x <= 349 and z >= 1386 and z <= 1433 then  --�������������귶Χ������ʹ��
	        left_door = 1
	    end

	    if x >= 620 and x <= 642 and z >= 1285 and z <= 1333 then  --�������������귶Χ������ʹ��
	        right_door = 1
	    end

		if left_door==0 and right_door==0 then
			bRet = 43
		end
	end

	--����
	return bRet, bIgnore
end

function i2615704_OnUse(MapID, InstanceID, TypeID, TargetID)
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)

	    if x >= 319 and x <= 349 and z >= 1386 and z <= 1433 then
	        map.SetMapScriptData(MapID, InstanceID, 1, 11, 1)
	        map.MapCreateCreature(MapID, InstanceID, 3075220, x, y, z)
	    end

	    if x >= 620 and x <= 642 and z >= 1285 and z <= 1333 then
	        map.SetMapScriptData(MapID, InstanceID, 1, 12, 1)
	        map.MapCreateCreature(MapID, InstanceID, 3075220, x, y, z)
	    end
end

--ע��
aux.RegisterItemEvent(2615704, 1, "i2615704_OnUse")
aux.RegisterItemEvent(2615704, 0, "i2615704_CanUse")
