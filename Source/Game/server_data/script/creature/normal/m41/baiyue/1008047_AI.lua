

-- �������ս������ʾ��
function c1008047_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ5�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008047, 2, "c1008047_OnEnterCombat")

--��������
--����ɱ ����ħ˧ 1008046 ˢ�� �̽�ħ˧ 1008047 ���꣺x 1902 y 21000 z 670
function c1008047_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")
	map.MapCreateCreature(MapID, InstanceID, 1008046, 1902, 21000, 670)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1769)	--����ħ˧�ѳ���������[1902, 670]��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end
aux.RegisterCreatureEvent(1008047, 4, "c1008047_OnDie")
