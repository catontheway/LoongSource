--������13629
--����1�������̳[3040420]��ԭ��[m05: 1733, 26804, 722]ˢ�³�����ͨ���ʦ[1006412]

--�����̳�����麯��
function c3040420_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateColCreature(MapID, InstanceID, 1006412, 1733, 26804, 722, 1, "g12")
end

--ע��
aux.RegisterCreatureEvent(3040420, 6, "c3040420_OnInvest")
