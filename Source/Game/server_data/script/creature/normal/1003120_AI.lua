--���ﺰ���ű���ʾ���ű�

-- �������ս������
function c1003120_OnEnterCombat(MapID, InstanceID, CreatureID)

		   cre.MonsterSay(MapID, InstanceID, CreatureID, 79)
	--�õ�����ĵ�ǰĿ��
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
	g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1003120, 2, "c1003120_OnEnterCombat")

--��������
function c1003120_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 1, "jinjieshili")
end

aux.RegisterCreatureEvent(1003120, 4, "c1003120_OnDie")
