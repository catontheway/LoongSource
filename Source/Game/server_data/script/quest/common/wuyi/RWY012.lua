--RWY012 �ϻ궾��
--����1����ȡ�����ÿ�ε���������ϻ�ݣ�����õ������ϻ��֮�󣬻��һ��˲��BUFF�ϻ� ֮��2015901��˲����ʧ10%������ֵ

function x3050031_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	unit.AddBuff(MapID, InstanceID, RoleID, 2015901, RoleID)
end

aux.RegisterCreatureEvent(3050031, 6, "x3050031_OnInvest")

