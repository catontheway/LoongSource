--ScriptData ����
--0:��Ʒ 3200513 ����
--1:ɱС������
--2:����������
--3:������BOSS
function m16_OnEnterArea(MapID, InstanceID, RoleID, ObjID)
--����ɫ����objIDΪ2907�ĵ�ͼ����ʱ������ɫ���͵�x=936 y=3918 z=2342 ���λ��
	if ObjID == 2907 then
	role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3000521935, 936, 3918, 2342)
--����ɫ����objIDΪ2908�ĵ�ͼ����ʱ������ɫ���͵�x=936 y=3918 z=2342 ���λ��
	elseif ObjID == 2908 then
	role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3000521935, 936, 3918, 2342)
	end
end

aux.RegisterMapEvent("m16", 7, "m16_OnEnterArea")

--����ɫ���븱�����޷�ʹ�ò��ּ���
--m16_SkillLimit = {}
--m16_SkillLimit[11005] = true
--m16_SkillLimit[11009] = true
--m16_SkillLimit[11012] = true
--m16_SkillLimit[11014] = true
--m16_SkillLimit[90012] = true

--function m16_CanUseSkill(MapID, InstanceID, SkillTypeID)
--    if m16_SkillLimit[SkillTypeID] then
--        return false
--    end
--    return true
--end

--aux.RegisterMapEvent("m16", 21, "m16_CanUseSkill")

--���븱��ʱɾ����ҵ�����buff
function m16_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	 end
end

--ע��
aux.RegisterMapEvent("m16", 2, "m16_OnPlayerEnter")
