

-- �������ս������ʾ��
function c1006729_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ3�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1006729, 2, "c1006729_OnEnterCombat")

--����8  ��ɱ 1006729 �󣬸� 1006720 ��ӷ�����ǿbuff(2017601)������50%


function c1006729_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--unit.AddBuff(MapID, InstanceID, 1006720, 2017601, 1006720)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end
--ע��
aux.RegisterCreatureEvent(1006729, 4, "c1006729_OnDie")
