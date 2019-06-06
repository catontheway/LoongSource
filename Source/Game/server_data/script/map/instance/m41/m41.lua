--m41ScriptData ����0 ����3334 ��¼��������
--m41ScriptData ����1 ����3609 ��¼��������
--m41ScriptData ����2 ����3611 ��¼��������
--m41ScriptData ����3-9 ���ڼ�ħ̨
--m41ScriptData ����10��11������ս ˢ������������
--m41ScriptData ����12, 13 ����ǰ��С��BOSS
--m41ScriptData ����14, 15 ���ں���С��BOSS
--m41ScriptData ����16 ������ȡ������������¼

--�Ի� ��ά 3060003 ����5������Ծbuff
function n3060003_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 268)	-- "�ҿ�����ʱ��ǿ���������Ҫ������"
		msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
		msg.AddMsgEvent(MsgID, 21, 5)	-- ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 207)	-- "ȡ��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif TalkIndex == 4 then
		unit.AddBuff(MapID, InstanceID, RoleID, 2016501, RoleID)
		TalkIndex = -1
	end
end
aux.RegisterCreatureEvent(3060003, 7, "n3060003_OnTalk")

function m41_OnEnterArea(MapID, InstanceID, RoleID, ObjID)
	--����ҽ����ͼ���� 3334 �������������꣺x 2325 y 26170 z 244
	if ObjID == 3334 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3084407759, 2325, 26170, 244)
	--ˢ�� ������ʿ 1008026 �������� b01 ·��Ѳ�� / ͬʱ������Ч txcj49 ���꣺x 890 y 14780 z 676
	--ˢ�� ������ʦ 1008027 �������� b02 ·��Ѳ�� / ͬʱ������Ч txcj49 ���꣺x 896 y 14780 z 676
	--ˢ�� ������ʿ 1008026 �������� b03 ·��Ѳ�� / ͬʱ������Ч txcj49 ���꣺x 902 y 14780 z 676
	elseif ObjID == 3609 then
		local Activated0 = map.GetMapScriptData(MapID,InstanceID,1,0) --����0��ֵΪ0��ˢ��
		if Activated0 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,0,1)
		map.MapCreateColCreature(MapID, InstanceID, 1008026, 890, 14780, 676, 1, "b01")
		map.MapCreateColCreature(MapID, InstanceID, 1008027, 896, 14780, 676, 1, "b02")
		map.MapCreateColCreature(MapID, InstanceID, 1008026, 902, 14780, 676, 1, "b03")
		end
	--ˢ�� ���ṭ�� 1008031 �������� b04 ·��Ѳ�� / ͬʱ������Ч txcj49 ���꣺x 895 y 14780 z 577
	--ˢ�� ���ṭ�� 1008031 �������� b04 ·��Ѳ�� / ͬʱ������Ч txcj49 ���꣺x 904 y 14784 z 574
	elseif ObjID == 3610 then
		local Activated1 = map.GetMapScriptData(MapID,InstanceID,1,1) -----����1��ֵΪ0��ˢ��
		if Activated1 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,1,1)
		map.MapCreateColCreature(MapID, InstanceID, 1008031, 895, 14780, 577, 1, "b04")
		map.MapCreateColCreature(MapID, InstanceID, 1008031, 904, 14784, 574, 1, "b04")
		end
	--ˢ�� �Ű���� 1008037 �������� b05 ·��Ѳ�� ���꣺x 1140 y 14780 z 365
	--ˢ�� �Ű���� 1008037 �������� b05 ·��Ѳ�� ���꣺x 1245 y 14780 z 480
	elseif ObjID == 3611 then
		local Activated2 = map.GetMapScriptData(MapID,InstanceID,1,2) -----����2��ֵΪ0��ˢ��
		if Activated2 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,2,1)
		map.MapCreateColCreature(MapID, InstanceID, 1008037, 1140, 14780, 365, 1, "b05")
		map.MapCreateColCreature(MapID, InstanceID, 1008037, 1245, 14780, 480, 1, "b05")
		end
	end
end

aux.RegisterMapEvent("m41", 7, "m41_OnEnterArea")



--����������꣺x 668 y 10277 z 841 �뾶40�ķ�Χ�� ʹ����Ʒ ��Խ���� 2614608�������ˢ�� 3060038
-- x 668 y 10277 z 841 �뾶40�ķ�Χ�� ʹ����Ʒ ��Խ���� 2614608
function I2614608_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)

	if MapID ~= 3084407759 then
		bRet = 43
	else
		local r = 40
		if x<668 - r or x>668 + r or z<841 - r or z>841 + r then
			bRet = 43
		end

	end
	return bRet, bIgnore
end

--ʹ�ð�Խ���� 2614608�������ˢ�� 3060038
function I2614608_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 3060038, x, y, z-5)
end


aux.RegisterItemEvent(2614608, 1, "I2614608_QuestUsable")
aux.RegisterItemEvent(2614608, 0, "I2614608_CanUse")

--���븱���󣬲��ּ����޷�ʹ��
--m41_SkillLimit = {}
--m41_SkillLimit[11005] = true
--m41_SkillLimit[11009] = true
--m41_SkillLimit[11012] = true
--m41_SkillLimit[11014] = true
--m41_SkillLimit[90012] = true

--function m41_CanUseSkill(MapID, InstanceID, SkillTypeID)
--    if m41_SkillLimit[SkillTypeID] then
--        return false
--    end
--    return true
--end

--aux.RegisterMapEvent("m41", 21, "m41_CanUseSkill")

--���븱��ʱɾ����ҵ�����buff
function m41_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	 end
end

--ע��
aux.RegisterMapEvent("m41", 2, "m41_OnPlayerEnter")

--������ʧ������������ScriptData ��ֵ��1
function m41_OnDisappear(MapID,InstanceID,CreatureID,CreatureTypeID)

	if CreatureTypeID == 1008006 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,10)
		if a < 1 then
		a = 1
		end
		map.SetMapScriptData(MapID,InstanceID,1,10,a-1)
	elseif CreatureTypeID == 1008007 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,11)
		if a < 1 then
		a = 1
		end
		map.SetMapScriptData(MapID,InstanceID,1,11,a-1)
	end

end
aux.RegisterMapEvent("m41", 18, "m41_OnDisappear")
