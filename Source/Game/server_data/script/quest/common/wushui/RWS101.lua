--��������̳��3040621�������˼�棨2613613��25%��˼��棨2613612��25%����BUFF������ҧ�ˣ�2018801��
function c3040621_OnCheck(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
       local i = math.random(1,4)
       local a = role.GetRoleItemNum(RoleID, 2613613)
       local b = role.GetRoleItemNum(RoleID, 2613612)
       if i==1 and a<3 then
           role.AddRoleItem(MapID, InstanceID, RoleID, 2613613, 1, -1, 8, 420)
       elseif i==2 and b<3 then
           role.AddRoleItem(MapID, InstanceID, RoleID, 2613612, 1, -1, 8, 420)
       elseif i==3 or i==4 then
           unit.AddBuff(MapID, InstanceID, RoleID, 2018801, RoleID)
       end
end

aux.RegisterCreatureEvent(3040621, 6, "c3040621_OnCheck")

--ʹ����˼��2613613��Ч��2019101
--�ɷ�ʹ�ú���
function i2613613_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false

	--���������ˮ������ʹ��
	if MapID ~= 3017299919 then
	    bRet = 43
	end

	--����
	return bRet, bIgnore
end

--ʹ��Ч��
function i2613613_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       unit.AddBuff(MapID, InstanceID, TargetID, 2019101, TargetID)
end

--ע��
aux.RegisterItemEvent(2613613, 1, "i2613613_QuestUsable")
aux.RegisterItemEvent(2613613, 0, "i2613613_CanUse")

--ʹ��˼���2613612��Ч��2019001
function i2613612_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false

	--���������ˮ������ʹ��
	if MapID ~= 3017299919 then
	    bRet = 43
	end

	--����
	return bRet, bIgnore
end

--ʹ��Ч��
function i2613612_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       unit.AddBuff(MapID, InstanceID, TargetID, 2019001, TargetID)
end

--ע��
aux.RegisterItemEvent(2613612, 1, "i2613612_QuestUsable")
aux.RegisterItemEvent(2613612, 0, "i2613612_CanUse")


--ʹ�ù�ζ����2613614��Ч��3000601
function i2613614_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	--����
	return bRet, bIgnore
end

--ʹ��Ч��
function i2613614_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       unit.AddBuff(MapID, InstanceID, TargetID, 3000601, TargetID)
end

--ע��
aux.RegisterItemEvent(2613614, 1, "i2613614_QuestUsable")
aux.RegisterItemEvent(2613614, 0, "i2613614_CanUse")
