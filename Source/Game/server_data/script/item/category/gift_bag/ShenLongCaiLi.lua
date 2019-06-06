function ShenLongCaiLi(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330702, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330703, 1, -1, 8, 420)

    --�����ҵȼ�����50���ɶ�����һ�������ߵ�֤��
    local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
    if level>=50 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 3303502, 1, -1, 8, 420)
    end

end

function ShenLongCaiLi_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
    local FreeSize = role.GetBagFreeSize(TargetID)
	local RequireSize = 2
	
    if level>=50 and Salary_NPC_On==1 then
        RequireSize =3
    end

    if(FreeSize < RequireSize) then
        --��ʾ��ұ����ռ䲻��
        bRet = 40
    end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330701, 1, "ShenLongCaiLi")
aux.RegisterItemEvent(1330701, 0, "ShenLongCaiLi_CanUse")
