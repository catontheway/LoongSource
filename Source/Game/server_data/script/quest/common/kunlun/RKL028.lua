--RKL028
--����ɼ����Ͳ�[3070704],��������BUFF����[2019701]

function c3070704_OnCheck(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    unit.AddBuff(MapID, InstanceID, RoleID, 2019701, RoleID)
end

--ע��
aux.RegisterCreatureEvent(3070704, 6, "c3070704_OnCheck")
