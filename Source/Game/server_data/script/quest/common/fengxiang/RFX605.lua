--С�����壡��11605
--����1�������������λ��[3020320]ʱ50%���ʻ��BUFF��ʴ[2011601]

--�������屻����ʱ�ű�����
function c3020320_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local Rate = math.random(1, 100)
	if Rate <= 50 then
		unit.AddBuff(mapID, InstanceID, RoleID, 2011601, RoleID)
	end
end

--ע��
aux.RegisterCreatureEvent(3020320, 6, "c3020320_OnInvest")
