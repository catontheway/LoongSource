
-- �������ս������ʾ��
function c1006720_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ3�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1006720, 2, "c1006720_OnEnterCombat")

--����7  ��ɱ 1006720 �󣬸� 1006729 ��ӹ�����ǿbuff(2017501)�����˺���50%

function c1006720_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--unit.AddBuff(MapID, InstanceID, 1006729, 2017501, 1006729)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end
--ע��
aux.RegisterCreatureEvent(1006720,4,"c1006720_OnDie")


