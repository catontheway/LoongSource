-- �������ս������/ʾ���ű�
function c1006531_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 30101)
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end
aux.RegisterCreatureEvent(1006531, 2, "c1006531_OnEnterCombat")



--����ʱ�ű�
--��ɱ ������[1006531] �� �����[1006532]֮��ˢ�����͵�[1006533]

function c1006531_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
       --������������
	cre.MonsterSay(MapID, InstanceID, TargetID, 30102)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end
--ע��
aux.RegisterCreatureEvent(1006531, 4, "c1006531_OnDie")




