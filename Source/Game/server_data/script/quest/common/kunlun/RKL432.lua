--15432���޵�һ��2615217�������������һ��ʹ�û������ȫ��
--15432���޵�����2615218������һ������һ��ʹ�û������ȫ��
--15432���޵�����2615219������һ�����һ��ʹ�û������ȫ��
--15432����ȫ�飨2615228������ָ���ص�ʹ���ٻ����޵���1009219��1226,14595,274��

--�ɷ�ʹ�ú���
function i2615217_CanUse(MapID, InstanceID, TypeID, TargetID)
    local i = role.GetRoleItemNum(TargetID, 2615218)
    local k = role.GetRoleItemNum(TargetID, 2615219)
    if i >= 1 and k >= 1 then
        return 0, false
    else
        return 32, false
    end
end

--ʹ��Ч��
function i2615217_QuestUsable(MapID, InstanceID, TypeID, TargetID)
    role.RemoveFromRole(MapID, InstanceID, TargetID, 2615218, 1, 420)
    role.RemoveFromRole(MapID, InstanceID, TargetID, 2615219, 1, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 2615228, 1, -1, 8, 420)

end

--ע��
aux.RegisterItemEvent(2615217, 1, "i2615217_QuestUsable")
aux.RegisterItemEvent(2615217, 0, "i2615217_CanUse")

--�ɷ�ʹ�ú���
function i2615218_CanUse(MapID, InstanceID, TypeID, TargetID)
    local i = role.GetRoleItemNum(TargetID, 2615217)
    local k = role.GetRoleItemNum(TargetID, 2615219)
    if i >= 1 and k >= 1 then
        return 0, false
    else
        return 32, false
    end
end

--ʹ��Ч��
function i2615218_QuestUsable(MapID, InstanceID, TypeID, TargetID)
    role.RemoveFromRole(MapID, InstanceID, TargetID, 2615217, 1, 420)
    role.RemoveFromRole(MapID, InstanceID, TargetID, 2615219, 1, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 2615228, 1, -1, 8, 420)
end

--ע��
aux.RegisterItemEvent(2615218, 1, "i2615218_QuestUsable")
aux.RegisterItemEvent(2615218, 0, "i2615218_CanUse")

--�ɷ�ʹ�ú���
function i2615219_CanUse(MapID, InstanceID, TypeID, TargetID)
    local i = role.GetRoleItemNum(TargetID, 2615218)
    local k = role.GetRoleItemNum(TargetID, 2615217)
    if i >= 1 and k >= 1 then
        return 0, false
    else
        return 32, false
    end
end

--ʹ��Ч��
function i2615219_QuestUsable(MapID, InstanceID, TypeID, TargetID)
    role.RemoveFromRole(MapID, InstanceID, TargetID, 2615218, 1, 420)
    role.RemoveFromRole(MapID, InstanceID, TargetID, 2615217, 1, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 2615228, 1, -1, 8, 420)

end

--ע��
aux.RegisterItemEvent(2615219, 1, "i2615219_QuestUsable")
aux.RegisterItemEvent(2615219, 0, "i2615219_CanUse")

--�ɷ�ʹ�ú���
function i2615228_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	--����������أ�����ʹ��
	if MapID ~= 3017299407 then
	    bRet = 43
	else
	--�������ָ���ص㣬����ʹ��
	    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x < 1156 or x > 1296 or z < 204 or z > 344 then
	          bRet = 43
            end
	end
    return bRet, bIgnore
end

--ʹ��Ч��
function i2615228_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       map.MapCreateColCreature(MapID, InstanceID, 1009219, 1226, 14595, 274, 1, "")
end

--ע��
aux.RegisterItemEvent(2615228, 1, "i2615228_QuestUsable")
aux.RegisterItemEvent(2615228, 0, "i2615228_CanUse")
